#include <stdio.h>
#include <stdlib.h>
#include "model.h"

void print_life(life l) {
  putchar('+');
  for (int i = 0; i < l.w; i++) putchar('-');
  putchar('+'); putchar('\n');

  for (int j = 0; j < l.h; j++) {
    putchar('|');
    for (int i = 0; i < l.w; i++) {
      if (get_cell(l, i, j)) putchar('*');
      else putchar(' ');
    }
    putchar('|'); putchar('\n');
  }

  putchar('+');
  for (int i = 0; i < l.w; i++) putchar('-');
  putchar('+');
  putchar('\n');
}

void main(){
  region r[4] = {54387, 34298, 1290, 17};
  life l = { .w = 8, .h = 8, .regions = r };

  print_life(l);
  char c;
  while(1) {
    c = getchar();
    if (c == 'q') break;
    else if (c == 'r') {
      l.regions[0] = 54387; l.regions[1] = 34298;
      l.regions[2] = 1290; l.regions[3] = 17;
    }
    else if (c == 's') {
      corner tmp = { l.regions[0], l.regions[1], l.regions[2], l.regions[3] };
      cornerRef dst = { &l.regions[0], &l.regions[1], &l.regions[2], &l.regions[3] };
      roll_corner(tmp, -1, -1, dst);
    }
    else if (c == 'd') {
      corner tmp = { l.regions[0], l.regions[1], l.regions[2], l.regions[3] };
      cornerRef dst = { &l.regions[0], &l.regions[1], &l.regions[2], &l.regions[3] };
      roll_corner(tmp, 0, -1, dst);
    }
    else if (c == 'f') {
      corner tmp = { l.regions[0], l.regions[1], l.regions[2], l.regions[3] };
      cornerRef dst = { &l.regions[0], &l.regions[1], &l.regions[2], &l.regions[3] };
      roll_corner(tmp, 1, -1, dst);
    }
    else if (c == 'x') {
      corner tmp = { l.regions[0], l.regions[1], l.regions[2], l.regions[3] };
      cornerRef dst = { &l.regions[0], &l.regions[1], &l.regions[2], &l.regions[3] };
      roll_corner(tmp, -1, 1, dst);
    }
    else if (c == 'c') {
      corner tmp = { l.regions[0], l.regions[1], l.regions[2], l.regions[3] };
      cornerRef dst = { &l.regions[0], &l.regions[1], &l.regions[2], &l.regions[3] };
      roll_corner(tmp, 0, 1, dst);
    }
    else if (c == 'v') {
      corner tmp = { l.regions[0], l.regions[1], l.regions[2], l.regions[3] };
      cornerRef dst = { &l.regions[0], &l.regions[1], &l.regions[2], &l.regions[3] };
      roll_corner(tmp, 1, 1, dst);
    }
    else if (c == 'z') {
      corner tmp = { l.regions[0], l.regions[1], l.regions[2], l.regions[3] };
      cornerRef dst = { &l.regions[0], &l.regions[1], &l.regions[2], &l.regions[3] };
      roll_corner(tmp, -1, 0, dst);
    }
    else if (c == 'b') {
      corner tmp = { l.regions[0], l.regions[1], l.regions[2], l.regions[3] };
      cornerRef dst = { &l.regions[0], &l.regions[1], &l.regions[2], &l.regions[3] };
      roll_corner(tmp, 1, 0, dst);
    }
    else if (c == '\n') {
      print_life(l);
    }
  }
}
