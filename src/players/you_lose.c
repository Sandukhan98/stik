#include "player.h"
#include <stdlib.h>
#include "eval_move.h"
#define BEST_MOVE 3000
#define MAX 99999999
#define MIN -99999999
static struct col_move_t *opening = NULL;
static struct board_game *my_bg = NULL;
static enum color_t my_color;

//the idea is : I play the best move for me from best moves if it was my oppenent who will play

char const *get_player_name()
{
  return "Dix Vitesse";
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
  srand(42);
  my_bg = board_game_empty(size, 4);
  my_color = id;
}

struct move_t play(struct col_move_t const previous_moves[], size_t n_moves)
{
  struct move_t next_move;
  struct col_move_t m;
  if (n_moves == 0)
  {
    int r = rand();
    m.m.row = r % (my_bg->size - 1) + 1;
    m.m.col = (r * r) % (my_bg->size - 1) + 1;
    m.c = my_color;
    board_game_setm(my_bg, m);
    next_move = m.m;
    return next_move;
  }
  for (size_t i = 0; i < n_moves; i++)
  {
    board_game_setm(my_bg, previous_moves[i]);
  }
  int best_val = MIN;
  int val = 0;
  int break_flag = 0;
  for (size_t i = 0; i < my_bg->size; i++)
  {
    for (size_t j = 0; j < my_bg->size; j++)
    {
      m.m.col = j;
      m.m.row = i;
      if (board_game_legalm(my_bg, m))
      {
        m.c = my_color;
        board_game_setm(my_bg, m);
        val = eval_move(my_bg, my_color, m);
        board_game_removem(my_bg, m);
        if (val >= WIN_MOVE)
        {
          next_move.col = j;
          next_move.row = i;
          break_flag = 1;
          break;
        }
        m.c = (my_color + 1) % 2;
        board_game_setm(my_bg, m);
        val += eval_move(my_bg, my_color, m);
        board_game_removem(my_bg, m);
        if (val > best_val)
        {
          best_val = val;
          next_move.col = j;
          next_move.row = i;
        }
      }
    }
    if (break_flag)
      break;
  }
  m.m = next_move;
  m.c = my_color;
  board_game_setm(my_bg, m);
  return next_move;
}

void finalize()
{
  if (opening != NULL)
    free(opening);
  if (my_bg != NULL)
    board_game_free(my_bg);
}
