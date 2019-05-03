#include "player.h"
#include "board_game.h"
#include <stdlib.h>

static struct col_move_t *opening = NULL;
static struct board_game *my_bg = NULL;
static enum color_t my_color;

char const *get_player_name()
{
  return "Stubborn player";
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
  for (int i = 0; i < 3; i++)
  {
    if ((opening[i].c == BLACK) && (size >= 5))
      return 0;
  }
  return 1;
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
  size_t size = board_game_get_size(my_bg);
  struct move_t last_move;
  struct move_t new_move;
  last_move.row = 0;
  last_move.col = 0;
  if (n_moves > 0)
    last_move = previous_moves[n_moves - 1].m;
  if (last_move.col < size)
  {
    new_move.row = last_move.row;
    new_move.col = last_move.col + 1;
  }
  new_move.row = last_move.row + 1;
  new_move.col = 0;
  return new_move;
}

void finalize()
{
  if (opening != NULL)
    free(opening);
  if (my_bg != NULL)
    board_game_free(my_bg);
}
