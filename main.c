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
  life l;
  l.w = 4;
  l.h = 4;
  l.regions = malloc(sizeof(region));
  *l.regions = 54387;

  print_life(l);
  char c;
  while(1) {
    c = getchar();
    if (c == 'q') break;
    else {
      region tmp = *l.regions;
      evolve_inner(tmp, l.regions);
      print_life(l);
    }
  }
}
