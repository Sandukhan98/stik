#ifndef BITBOARD_H
#define BITBOARD_H
#include <stdio.h>
#include <stdlib.h>
#include "move.h"
#define UNION(x, y) ((x) | (y))
#define INTER(x, y) ((x) & (y))
#define SYM_DIFF(x, y) ((x) ^ (y)) // symmetric difference SYM_DIFF(A,B)=(A\B)U(B\A)
#define max(a, b) ((a > b) ? a : b)
#define min(a, b) ((a < b) ? a : b)
#define UNUSED(x) (void)x
#define SHIFT_N(x, a, s) ((x >> (a * s)))
#define SHIFT_S(x, a, s) ((x << (a * s)))
#define SHIFT_E(x, a) ((x << a))
#define SHIFT_W(x, a) ((x >> a))
#define SHIFT_NW(x, a, s) ((x >> (a * s + a)))
#define SHIFT_SE(x, a, s) ((x << (a * s + a)))
#define SHIFT_NE(x, a, s) ((x >> (a * s - a)))
#define SHIFT_SW(x, a, s) ((x << (a * s - a)))
#define PLUS(x, a, s) (x | SHIFT_N(x, a, s) | SHIFT_S(x, a, s) | SHIFT_W(x, a) | SHIFT_E(x, a) | SHIFT_NW(x, a, s) | SHIFT_NE(x, a, s) | SHIFT_SW(x, a, s) | SHIFT_SE(x, a, s))
#define NBORS(x, a, s) (PLUS(x, a, s) & ~x)

struct bitboard
{
    unsigned __int128 bitboard;
    size_t size;
};
int bitboard_big_row_line(struct bitboard *be, size_t row, size_t col, struct col_move_t *mv1, struct col_move_t *mv2);
int bitboard_big_col_line(struct bitboard *be, size_t row, size_t col, struct col_move_t *mv1, struct col_move_t *mv2);
int bitboard_big_diagr_line(struct bitboard *be, size_t row, size_t col, struct col_move_t *mv1, struct col_move_t *mv2);
int bitboard_big_diagl_line(struct bitboard *be, size_t row, size_t col, struct col_move_t *mv1, struct col_move_t *mv2);
// creates new elementary board with the given size
struct bitboard *bitboard_empty(size_t size);
//create board elem from given bitboard
struct bitboard *bitboard_create(size_t size, unsigned __int128 bitboard);
// removes a bit from an elementary board in given position
void bitboard_remove(struct bitboard *be, size_t row, size_t col);
// sets a bit to one in given position
void bitboard_set(struct bitboard *be, size_t row, size_t col);
// returns the value of the bit in given position
char bitboard_get(struct bitboard *be, size_t row, size_t col);
// checks if from the given position there are x bits aligned in a row and puts in mv1 and mv2 positions of null bits that limit this group

// checks if from the given position there are x bits aligned in a column and puts in mv1 and mv2 positions of null bits that limit this group

// checks if from the given position there are x bits aligned in a right diagonal and puts in mv1 and mv2 positions of null bits that limit this group
int bitboard_x_diagr(struct bitboard *be, size_t row, size_t col, size_t x, struct col_move_t *mv1, struct col_move_t *mv2);
// checks if from the given position there are x bits aligned in a left diagonal and puts in mv1 and mv2 positions of null bits that limit this group
int bitboard_x_diagl(struct bitboard *be, size_t row, size_t col, size_t x, struct col_move_t *mv1, struct col_move_t *mv2);

int bitboard_x_row_h(struct bitboard *be, size_t row, size_t col, size_t x, struct col_move_t *mv1, struct col_move_t *mv2, struct col_move_t *hole);
int bitboard_x_col_h(struct bitboard *be, size_t row, size_t col, size_t x, struct col_move_t *mv1, struct col_move_t *mv2, struct col_move_t *hole);
int bitboard_x_diagr_h(struct bitboard *be, size_t row, size_t col, size_t x, struct col_move_t *mv1, struct col_move_t *mv2, struct col_move_t *hole);
int bitboard_x_diagl_h(struct bitboard *be, size_t row, size_t col, size_t x, struct col_move_t *mv1, struct col_move_t *mv2, struct col_move_t *hole);
// free an elementary board
void bitboard_free(struct bitboard *be);
#endif
