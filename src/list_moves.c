#include "list_moves.h"

static struct col_move_t fake_stroke = {{-1, -1}, BLACK};

struct list_moves *moves__empty()
{
  struct list_moves *empty = malloc(sizeof(struct list_moves));
  empty->moves = malloc(sizeof(struct col_move_t));
  empty->capacity = 1;
  empty->size = 0;
  return empty;
}

int moves__add(struct list_moves *moves, struct col_move_t move)
{
  if (moves->size == moves->capacity)
  {
    moves->capacity *= 2;
    moves->moves = realloc(moves->moves, (moves->capacity) * sizeof(struct col_move_t));
  }
  moves->moves[moves->size] = move;
  moves->size += 1;
  return 1; // quelle erreur peut-il y avoir?
}

struct col_move_t *moves__last_n(struct list_moves *moves, size_t n)
{
  if (n < moves->size)
  {
    for (size_t i = moves->size; i < n; i++)
    {
      moves->moves[i] = fake_stroke;
    }
  }
  return &moves->moves[moves->size - n];
}

int moves__is_fake(struct col_move_t *moves, size_t n)
{
  int nb = 0;
  for (size_t i = 0; i < n; i++)
  {
    if (moves[i].m.row != fake_stroke.m.row && moves[i].m.col != fake_stroke.m.col)
    {
      nb += 1;
    }
  }
  return nb;
}

void moves__free(struct list_moves *moves)
{
  free(moves->moves);
  free(moves);
}
