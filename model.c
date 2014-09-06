#include "model.h"

region write_inner_mask = 1632 /*0000 0110 0110 0000*/,
  write_tl_mask = 1 /*0000 0000 0000 0001*/,
  write_tr_mask = 14 /*0000 0000 0000 1110*/,
  write_bl_mask = 4368 /*0001 0001 0001 0000*/,
  write_br_mask = 59520 /*1110 1000 1000 0000*/;

int pop_count(region r) {
  int i = 0;
  while (r > 0) {
    i += r & 1;
    r >>= 1;
  }
  return i;
}

int get_cell_by_index(region r, int index) {
  return r & (1 << index);
}

int region_get_cell(region r, int i, int j) {
  return get_cell_by_index(r, i + j*4);
}

int get_cell(life l, int i, int j) {
  int regionX = i/4, regionY = j/4, regionIndex = regionY*l.w/4 + regionX,
    cellX = i - regionX*4, cellY = j - regionY*4;
  return region_get_cell(l.regions[regionIndex], cellX, cellY);
}

region inner_neighbor_mask(int index) {
  region triplet = 57344, duet = 40960;
  return triplet >> (index - 5)
    | duet >> (index - 1)
    | triplet >> (index + 3);
}

int inner_neighbors(region r, int index) {
    return pop_count(r & inner_neighbor_mask(index));
}

region cell_next_state(int cell, int num_neighbors) {
  return !(num_neighbors < 2
           || num_neighbors > 3
           || (num_neighbors == 2 && !cell));
}

region evolve_inner_cell(region r, int index) {
  int n = inner_neighbors(r, index);
  int cell = get_cell_by_index(r, index);
  return cell_next_state(cell, n) << (15 - index);
}

void set_portion(region src, region mask, region* dst) {
  // zero out the masked section
  *dst &= ~mask;
  // If we AND the source with the mask, we get 0s outside of the mask and
  // the source's bits inside.
  // Since the masked section on the destination is zeroed, we can write with
  // an OR.
  *dst |= src & mask;
}

void evolve_inner(region src, region* dst) {
  region result = evolve_inner_cell(src, 5)
    | evolve_inner_cell(src, 6)
    | evolve_inner_cell(src, 9)
    | evolve_inner_cell(src, 10);
  set_portion(result, write_inner_mask, dst);
}

region rshift(region r, int offset) {
  if (offset < 0) return r << -offset;
  else return r >> offset;
}
region lshift(region r, int offset) {
  if (offset < 0) return r >> -offset;
  else return r << offset;
}

region roll_region(region r, int i, int j) {
  int shift = i + 4*j;
  region result = lshift(r, shift);
  // Now there's a portion we need to zero out
  region rowMask = ((i<0) ? (15 << 4 + i) : (15 >> 4 - i)) & 15, mask = rowMask;
  for (int i = 0; i < 4; i++) {
    mask <<= 4; mask += rowMask;
  }
  set_portion(0, mask, &result);
  return result;
}

region rect_mask(int w, int h) {
  int dx, dy;
  if (w<0) dx = 4 + w;
  else dx = -4 + w;
  if (h<0) dy = 4 + h;
  else dy = -4 + h;
  return roll_region(~0, dx, dy);
}

void roll_corner(corner src, int i, int j, cornerRef dst) {
  if (i <= 0 && j <= 0) {
    *dst.tl = roll_region(src.tl, i, j);
    
    // Move from bl and tr to tl
    // Nothing from the left side of bl will be used, so we can roll left
    // without fear of loss
    *dst.bl = roll_region(src.bl, i, 0); 
    // Now move from br to bl, because it might appear in tl
    set_portion(roll_region(src.br, 4 + i, 0), rect_mask(i, 4), dst.bl);
    // Finally, we are ready to move the top of bl to the bottom of tl
    set_portion(roll_region(*dst.bl, 0, 4 + j), rect_mask(4, j), dst.tl);
    *dst.bl = roll_region(*dst.bl, 0, j);
    
    // Now repeat with br -> tr -> tl:
    *dst.tr = roll_region(src.tr, 0, j);
    set_portion(roll_region(src.br, 0, 4 + j), rect_mask(4, j), dst.tr);
    set_portion(roll_region(*dst.tr, 4 + i, 0), rect_mask(i, 4), dst.tl);
    *dst.tr = roll_region(*dst.tr, i, 0);
    
    // Finally, we can roll br
    *dst.br = roll_region(src.br, i, j);
  }
  else if (i > 0 && j <= 0) {
    // In this case we do the same thing, except we move bl -> br -> tr
    // and bl -> tl -> tr
    *dst.tr = roll_region(src.tr, i, j);
    
    *dst.br = roll_region(src.br, i, 0);
    set_portion(roll_region(src.bl, -4 + i, 0), rect_mask(i, 4), dst.br);
    set_portion(roll_region(*dst.br, 0, 4 + j), rect_mask(4, j), dst.tr);
    *dst.br = roll_region(*dst.br, 0, j);

    *dst.tl = roll_region(src.tl, 0, j);
    set_portion(roll_region(src.bl, 0, 4 + j), rect_mask(4, j), dst.tl);
    set_portion(roll_region(*dst.tl, -4 + i, 0), rect_mask(i, 4), dst.tr);
    *dst.tl = roll_region(*dst.tl, i, 0);

    *dst.bl = roll_region(src.bl, i, j);
  }
  else if (i <= 0 && j > 0) {
    *dst.bl = roll_region(src.bl, i, j);

    *dst.tl = roll_region(src.tl, i, 0);
    set_portion(roll_region(src.tr, 4 + i, 0), rect_mask(i, 4), dst.tl);
    set_portion(roll_region(*dst.tr, 0, -4 + j), rect_mask(4, j), dst.bl);
    *dst.tl = roll_region(*dst.tl, 0, j);

    *dst.br = roll_region(src.br, 0, j);
    set_portion(roll_region(src.tr, 0, -4 + j), rect_mask(4, j), dst.br);
    set_portion(roll_region(*dst.br, 4 + i, 0), rect_mask(i, 4), dst.bl);
    *dst.br = roll_region(*dst.br, i, 0);

    *dst.tr = roll_region(src.tr, i, j);
  }
  else if (i > 0 && j > 0) { // always true if we haven't taken another case
    *dst.br = roll_region(src.br, i, j);

    *dst.tr = roll_region(src.tr, i, 0);
    set_portion(roll_region(src.tl, -4 + i, 0), rect_mask(i, 4), dst.tr);
    set_portion(roll_region(*dst.tr, 0, -4 + j), rect_mask(4, j), dst.br);
    *dst.tr = roll_region(*dst.tr, 0, j);

    *dst.bl = roll_region(src.bl, 0, j);
    set_portion(roll_region(src.tl, 0, -4 + j), rect_mask(4, j), dst.bl);
    set_portion(roll_region(*dst.bl, -4 + j, 0), rect_mask(i, 4), dst.br);
    *dst.bl = roll_region(*dst.bl, i, 0);

    *dst.tl = roll_region(src.tl, i, j);
  }
}

void evolve_corner(corner src, cornerRef dst) {
  
}
