#ifndef _MODEL_H
#define _MODEL_H

#include <stdint.h>

// A region is a contiguous 4x4 area of the life state, represented as a 16
// bit integer.
// 15 14 13 12
// 11 10  9  8
//  7  6  5  4
//  3  2  1  0
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
//
// Since it's a torus, a given intersection of four regions will be processed
// multiple times. Here's another diagram showing when each edge square will
// get updated:
// 33 33 33 22  22 22 22 33
// 33 .. .. 22  22 .. .. 33
// 33 .. .. 22  22 .. .. 33
// 11 11 11 00  00 00 00 11
//
// 11 11 11 00  00 00 00 11
// 11 .. .. 00  00 .. .. 11
// 11 .. .. 00  00 .. .. 11
// 33 33 33 22  22 22 22 33

void evolve_corner(cornerRef src, cornerRef dst);

void roll_corner(cornerRef src, int i, int j, cornerRef dst);


typedef struct { int w, h; region* regions; }
  life;

int get_cell(life l, int i, int j);

void evolve(life source, life* dest);

int copy_life(life source, life* dest);

#endif
