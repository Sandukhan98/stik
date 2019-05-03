#include "move.h"
#include "player.h"
#include "eval_move.h"
#include <stdlib.h>
#include <stdio.h>

static struct col_move_t *openingf = NULL;
static struct board_game *my_bg = NULL;

static enum color_t my_color;

char const *get_player_name()
{
  return "manual player";
}
struct col_move_t *propose_opening(size_t size)
{
  struct col_move_t *opening = malloc(3 * sizeof(struct col_move_t));
  openingf = opening;
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
  return 1;
}

void initialize(size_t size, enum color_t id)
{
  my_bg = board_game_empty(size, 2);
  my_color = id;
}

struct move_t play(struct col_move_t const previous_moves[], size_t n_moves)
{
  for (size_t i = 0; i < n_moves; i++)
  {
    board_game_setm(my_bg, previous_moves[i]);
  }
  struct col_move_t new_move;
  size_t row;
  size_t column;
  printf("row : ");
  scanf("%zd", &row);
  printf("column : ");
  scanf("%zd", &column);
  new_move.m.row = row;
  new_move.m.col = column;
  new_move.c = my_color;
  board_game_setm(my_bg, new_move);
  printf("manual eval ==== %d \n", eval_move(my_bg, my_color, new_move));
  // printf("manual eval1 ==== %d \n", eval_move1(my_bg, my_color, new_move));
  return new_move.m;
}

void finalize()
{
  if (openingf != NULL)
    free(openingf);
  if (my_bg != NULL)
    board_game_free(my_bg);
}
