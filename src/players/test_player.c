#include "move.h"
#include "player.h"
#include <stdlib.h>
#include <stdio.h>
#define UNUSED(x) (void)x

static struct col_move_t *opening = NULL;

char const *get_player_name()
{
  return "test_player";
}

/* A DEBUG */
struct col_move_t *propose_opening(size_t size)
{
  UNUSED(size);
  opening = malloc(3 * sizeof(struct col_move_t));
  struct move_t first_move = {0, 0};
  struct move_t second_move = {0, 0};
  struct move_t third_move = {0, 0};
  opening[0].m = first_move;
  opening[0].c = BLACK;
  opening[1].m = second_move;
  opening[1].c = WHITE;
  opening[2].m = third_move;
  opening[2].c = BLACK;
  return opening;
}

int accept_opening(size_t size, const struct col_move_t *opening)
{
  if ((size >= 5) && (opening != NULL))
    return 1; // Accept all opening
  return 0;
}

void initialize(size_t size, enum color_t id)
{
  UNUSED(size);
  UNUSED(id);
  printf("test player is initialized\n");
  // Ne tient pas compte de size du plateau
}

struct move_t play(struct col_move_t const previous_moves[], size_t n_moves)
{
  return previous_moves[n_moves - 1].m;
}

void finalize()
{
  printf("test player is finalized\n");
  if (opening != NULL)
    free(opening);
}
