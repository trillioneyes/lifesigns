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
  int regionX = i/4, regionY = j/4, regionIndex = regionY*4 + regionX,
    cellX = i - regionX*4, cellY = j - regionY*4;
  return region_get_cell(l.regions[regionIndex], i, j);
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

void evolve_inner(region src, region* dst) {
  // zero out the inner section so that we can write to it with bitwise OR
  *dst &= ~0 & ~write_inner_mask;
  // compute the section we'll be dropping into the hole
  region result = evolve_inner_cell(src, 5)
    | evolve_inner_cell(src, 6)
    | evolve_inner_cell(src, 9)
    | evolve_inner_cell(src, 10);
  // and finally, write it!
  *dst |= result & write_inner_mask;
}

void evolve_corner(region tl, region tr, region bl, region br,
                   region* dtl, region* dtr, region* dbl, region* dbr) {
}
