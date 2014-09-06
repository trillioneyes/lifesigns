#ifndef _MODEL_H
#define _MODEL_H

#include <stdint.h>

// A region is a contiguous 4x4 area of the life state, represented as a 16
// bit integer.
//  0  1  2  3
//  4  5  6  7
//  8  9 10 11
// 12 13 14 15
typedef uint16_t region;

// Evolve the inner cells of the provided region, writing the result
// into dest. The other bits of dest are untouched.
// 0s are unchanged, 1s are changed/considered:
// 0 0 0 0
// 0 1 1 0
// 0 1 1 0
// 0 0 0 0
void evolve_inner(region source, region* dest);

typedef struct { region tl, tr, bl, br; } corner;
typedef struct { region *tl, *tr, *bl, *br; } cornerRef;

// Evolve the place where the four given regions intersect:
// tl tl tl tl  tr tr tr tr
// tl tl tl tl  tr tr tr tr
// tl tl tl tl  tr tr tr tr
// tl tl tl **  ** ** ** tr
// 
// bl bl bl **  ** ** ** br
// bl bl bl **  ** br br br
// bl bl bl **  ** br br br
// bl bl bl bl  br br br br
// Asterisks are considered/written, lowercase letters are unchanged.
// The top and left are untouched because this is a torus, and they'll
// be picked up later.
void evolve_corner(cornerRef src, cornerRef dst);

void roll_corner(cornerRef src, int i, int j, cornerRef dst);

typedef struct { int w, h; region* regions; }
  life;

int get_cell(life l, int i, int j);

void evolve(life source, life* dest);

int copy_life(life source, life* dest);

#endif
