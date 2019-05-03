#include "board_game.h"
#include "move.h"
#include "player.h"
#include <stdlib.h>

static struct col_move_t *opening = NULL;
static struct board_game *my_bg = NULL;
static enum color_t my_color;

char const *get_player_name()
{
  return "Dumb player";
}

struct col_move_t *propose_opening(size_t size)
{
  opening = malloc(3 * sizeof(struct col_move_t));
  struct move_t first_move = {0, 0};
  struct move_t second_move = {size - 1, size - 1};
  struct move_t third_move = {0, 1};
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
    return 1; // Accepte tout opening
  return 0;
}

void initialize(size_t size, enum color_t id)
{
  my_bg = board_game_empty(size, 4);
  my_color = id;
}

struct move_t play(struct col_move_t const previous_moves[], size_t n_moves)
{
  for (int i = 0; i < (int)n_moves; i++)
  {
    board_game_setm(my_bg, previous_moves[i]);
  }
  struct move_t next_move = {0, 2};
  return next_move;
}

void finalize()
{
  if (opening != NULL)
    free(opening);
  if (my_bg != NULL)
    board_game_free(my_bg);
}
