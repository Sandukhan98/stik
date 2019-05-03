#include "player.h"
#include <stdlib.h>
#include "eval_move.h"
#include "display.h"

#define MIN -10000000
#define MAX 10000000

static struct col_move_t *opening = NULL;
static struct board_game *my_bg = NULL;
static enum color_t my_color;
int minimax(int depth, int maxmize, int alpha, int beta, struct col_move_t move, int (*eval_mm)(struct board_game *my_bg, enum color_t my_color, struct col_move_t move));

char const *get_player_name()
{
  return "Hey hey hey";
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
  int val = 0;
  // printf("==================================\n");
  for (size_t k = 0; k < n_moves; k++)
  {
    board_game_setm(my_bg, previous_moves[k]);
  }
  size_t size = my_bg->size;
  struct col_move_t m;
  struct move_t next_move;
  int max = MIN;
  int depth = 0;
  size_t row = 0, col = 0;
  unsigned __int128 u = (my_bg->player_pieces[0]->bitboard | my_bg->player_pieces[1]->bitboard);
  // struct bitboard *to_eval = bitboard_create(my_bg->size, NBORS(u, 1, my_bg->size));
  struct bitboard *to_eval = bitboard_create(my_bg->size, UNION(NBORS(UNION(u, NBORS(u, 1, my_bg->size)), 1, my_bg->size), NBORS(u, 1, my_bg->size)));
  m.c = my_color;
  for (size_t i = 0; i < size; i++)
  {
    for (size_t j = 0; j < size; j++)
    {
      m.m.row = i;
      m.m.col = j;
      if (bitboard_get(to_eval, i, j))
      {
        board_game_setm(my_bg, m);
        val = minimax(2, 0, MIN, MAX, m, eval_move);
        if (!(val >= WIN_MOVE || board_game_is_full(my_bg)))
          val = minimax(0, 0, MIN, MAX, m, eval_move);
        if (val > max)
        {
          max = val;
          row = i;
          col = j;
        }
        board_game_removem(my_bg, m);
      }
    }
  }
  // getchar();
  m.m.row = row;
  m.m.col = col;
  m.c = my_color;
  board_game_setm(my_bg, m);
  next_move.col = col;
  next_move.row = row;
  bitboard_free(to_eval);
  return next_move;
}

void finalize()
{
  if (opening != NULL)
    free(opening);
  if (my_bg != NULL)
    board_game_free(my_bg);
}

int minimax(int depth, int maxmize, int alpha, int beta, struct col_move_t move, int (*eval_mm)(struct board_game *my_bg, enum color_t my_color, struct col_move_t move))
{
  if (depth == 2)
  {
    // printf("here\n");
    int val = (my_color == move.c) ? eval_mm(my_bg, my_color, move) : -eval_mm(my_bg, move.c, move);
    // printf(" %ld   ||   %ld    =>   %d\n", move.m.row, move.m.col, val);
    // display_board(*my_bg);
    // getchar();
    return val;
  }
  unsigned __int128 u = (my_bg->player_pieces[0]->bitboard | my_bg->player_pieces[1]->bitboard);
  struct bitboard *to_eval = bitboard_create(my_bg->size, UNION(NBORS(UNION(u, NBORS(u, 1, my_bg->size)), 1, my_bg->size), NBORS(u, 1, my_bg->size)));
  // struct bitboard *to_eval = bitboard_create(my_bg->size, NBORS(u, 1, my_bg->size));
  if (maxmize)
  {
    char break_flag = 0;
    int max = MIN;
    size_t size = my_bg->size;
    struct col_move_t m;
    size_t i, j;
    int val;
    m.c = my_color;
    for (i = 0; i < size; i++)
    {
      for (j = 0; j < size; j++)
      {
        m.m.row = i;
        m.m.col = j;
        if (bitboard_get(to_eval, i, j))
        {
          board_game_setm(my_bg, m);
          val = minimax(2, 0, alpha, beta, m, eval_mm);
          if (val < WIN_MOVE && !board_game_is_full(my_bg))
            val = minimax(depth + 1, 0, alpha, beta, m, eval_mm);
          // val -= eval_mm(my_bg, my_color, move) + depth ;
          board_game_removem(my_bg, m);
          max = max(max, val);
          alpha = max(max, alpha);
          if (beta <= alpha)
          {
            break_flag = 1;
            break;
          }
        }
      }
      if (break_flag)
        break;
    }
    bitboard_free(to_eval);
    return max;
  }
  else
  {
    char break_flag = 0;
    int min = MAX;
    size_t size = my_bg->size;
    struct col_move_t m;
    size_t i, j;
    int val;
    m.c = (my_color + 1) % 2;
    for (i = 0; i < size; i++)
    {
      for (j = 0; j < size; j++)
      {
        m.m.row = i;
        m.m.col = j;
        if (bitboard_get(to_eval, i, j))
        {
          board_game_setm(my_bg, m);
          val = minimax(2, 1, alpha, beta, m, eval_mm);
          if (val >= -WIN_MOVE && !board_game_is_full(my_bg))
            val = minimax(depth + 1, 1, alpha, beta, m, eval_mm);
          // val += eval_mm(my_bg, my_color, move) + depth ;
          board_game_removem(my_bg, m);
          min = min(min, val);
          beta = min(beta, min);
          if (beta <= alpha)
          {
            break_flag = 1;
            break;
          }
        }
      }
      if (break_flag)
        break;
    }
    bitboard_free(to_eval);
    return min;
  }
}
