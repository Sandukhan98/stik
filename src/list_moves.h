#ifndef LIST_MOVES_H
#define LIST_MOVES_H

#include <stdio.h>
#include <stdlib.h>
#include "move.h"

struct list_moves
{
  struct col_move_t *moves;
  size_t size;
  size_t capacity;
};

// initialise a struct moves
struct list_moves *moves__empty();

// add the last turn in moves
int moves__add(struct list_moves *moves, struct col_move_t move);

// display the last n turns
struct col_move_t *moves__last_n(struct list_moves *moves, size_t n);

//return the number of fake strokes
int moves__is_fake(struct col_move_t *moves, size_t n);

// free
void moves__free(struct list_moves *moves);

#endif
