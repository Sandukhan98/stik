#include "eval_move.h"
#include "display.h"
#define legal(x, y) board_game_legalm(x, y)

void *bitboard_x_line[] = {bitboard_big_row_line, bitboard_big_col_line, bitboard_big_diagr_line, bitboard_big_diagl_line};
void *bitboard_x_line_h[] = {bitboard_x_row_h, bitboard_x_col_h, bitboard_x_diagr_h, bitboard_x_diagl_h};

//FUNCTION THAT SEARCH FOR AN X LENGTH ARRAY WITH TWO OPEN SIDES

int ligne_of_x_opened(struct board_game *my_bg, struct col_move_t m, int x)
{
  struct col_move_t mv1;
  struct col_move_t mv2;
  int count = 0;
  int (*fun)(struct bitboard * be, size_t row, size_t col, struct col_move_t * mv1, struct col_move_t * mv2);
  for (int i = 0; i < 4; i++)
  {
    fun = bitboard_x_line[i];
    if (fun(my_bg->player_pieces[m.c], m.m.row, m.m.col, &mv1, &mv2) >= x)
    {
      if (legal(my_bg, mv1) && legal(my_bg, mv2))
        count++;
    }
  }
  return count;
}

//FUNCTION THAT SEARCH FOR TWO FOUR ARRAYS WITH ONE OPEN SIDE

int two_four_with_one_end(struct board_game *my_bg, struct col_move_t m)
{
  int i = 0;
  struct col_move_t mv1;
  struct col_move_t mv2;
  int (*fun)(struct bitboard * be, size_t row, size_t col, struct col_move_t * mv1, struct col_move_t * mv2);
  mv1.c = 0;
  mv2.c = 0;
  for (int j = 0; j < 4; j++)
  {
    fun = bitboard_x_line[j];
    if (fun(my_bg->player_pieces[m.c], m.m.row, m.m.col, &mv1, &mv2) >= 4)
    {
      if (legal(my_bg, mv1) || legal(my_bg, mv2))
        i++;
      if (i == 2)
        return 1;
    }
  }
  return 0;
}

//FUNCTION THAT SEARCH TWO THREE ARRAYS OPEN IN BOTH SIDES

int two_three_without_end(struct board_game *my_bg, struct col_move_t m)
{
  int i = 0;
  struct col_move_t mv1;
  struct col_move_t mv2;
  int (*fun)(struct bitboard * be, size_t row, size_t col, struct col_move_t * mv1, struct col_move_t * mv2);
  mv1.c = 0;
  mv2.c = 0;
  for (int j = 0; j < 4; j++)
  {
    fun = bitboard_x_line[j];
    if (fun(my_bg->player_pieces[m.c], m.m.row, m.m.col, &mv1, &mv2) >= 3)
    {
      if (legal(my_bg, mv1) && legal(my_bg, mv2))
        i++;
      if (i == 2)
        return 1;
    }
  }
  return 0;
}

//FUNCTION THAT TELLS THE POSSIBILITY OF HAVING A THREE ARRAY OPEN IN BOTH SIDES AND A FOUR ARRAY CLOSED IN ONE SIDE

int three_four(struct board_game *my_bg, struct col_move_t m)
{
  int i1 = 0;
  int i2 = 0;
  struct col_move_t mv1;
  struct col_move_t mv2;
  int (*fun)(struct bitboard * be, size_t row, size_t col, struct col_move_t * mv1, struct col_move_t * mv2);
  mv1.c = 0;
  mv2.c = 0;
  //SEARCHING FOR A 4 ARRAY OPEN IN ONE SIDE
  for (int j = 0; j < 4; j++)
  {
    fun = bitboard_x_line[j];
    if (fun(my_bg->player_pieces[m.c], m.m.row, m.m.col, &mv1, &mv2) >= 4)
    {
      if (!(legal(my_bg, mv1) && legal(my_bg, mv2)) && (legal(my_bg, mv1) || legal(my_bg, mv2)))
        i1++;
    }
  }
  //SEARCHING FOR A 3 ARRAY OPEN IN BOTH SIDES
  for (int j = 0; j < 4; j++)
  {
    fun = bitboard_x_line[j];
    if (fun(my_bg->player_pieces[m.c], m.m.row, m.m.col, &mv1, &mv2) >= 3)
    {
      if (legal(my_bg, mv1) && legal(my_bg, mv2))
        i2++;
    }
  }
  //CHECKING THE RESULTS
  if ((i2 >= 1) && (i1 >= 1))
    return 1;
  return 0;
}

// checks if there is an oppenent pawn in adjacent case
int opp_adjacent(struct board_game *my_bg, enum color_t my_color, struct col_move_t m)
{
  struct col_move_t mv1;
  struct col_move_t mv2;
  int (*fun)(struct bitboard * be, size_t row, size_t col, struct col_move_t * mv1, struct col_move_t * mv2);
  for (int j = 0; j < 4; j++)
  {
    fun = bitboard_x_line[j];
    if (fun(my_bg->player_pieces[m.c], m.m.row, m.m.col, &mv1, &mv2) >= 1)
    {
      if (!legal(my_bg, mv1) && !board_game_is_out(my_bg, mv1))
      {
        if (bitboard_get(my_bg->player_pieces[(my_color + 1) % 2], mv1.m.row, mv1.m.col))
          return 1;
      }
      if (!legal(my_bg, mv2) && !board_game_is_out(my_bg, mv2))
      {
        if (bitboard_get(my_bg->player_pieces[(my_color + 1) % 2], mv2.m.row, mv2.m.col))
          return 1;
      }
    }
  }
  return 0;
}

int eval_move(struct board_game *my_bg, enum color_t my_color, struct col_move_t m)
{
  if (board_game_win(my_bg, m))
  {
    return WIN_MOVE;
  }
  if (ligne_of_x_opened(my_bg, m, 4))
  {
    return BEST_MOVE;
  }
  if (two_four_with_one_end(my_bg, m))
  {
    return BEST_MOVE;
  }
  if (two_three_without_end(my_bg, m))
  {
    return BEST_MOVE;
  }
  if (three_four(my_bg, m))
  {
    return BEST_MOVE;
  }
  int count = 0;
  count += 10 * ligne_of_x_opened(my_bg, m, 3) +  3 * ligne_of_x_opened(my_bg, m, 2);
  return count;
}

int two_four_with_one_end_h(struct board_game *my_bg, struct col_move_t m)
{
  int i = 0;
  struct col_move_t mv1;
  struct col_move_t mv2;
  struct col_move_t hole;
  int (*fun)(struct bitboard * be, size_t row, size_t col, struct col_move_t * mv1, struct col_move_t * mv2);
  int (*fun_h)(struct bitboard * be, size_t row, size_t col, size_t x, struct col_move_t * mv1, struct col_move_t * mv2, struct col_move_t * hole);
  mv1.c = 0;
  mv2.c = 0;
  for (int j = 0; j < 4; j++)
  {
    fun = bitboard_x_line[j];
    fun_h = bitboard_x_line_h[j];
    if ((fun(my_bg->player_pieces[m.c], m.m.row, m.m.col, &mv1, &mv2) >= 4 && (legal(my_bg, mv1) || legal(my_bg, mv2))) ||
        (fun_h(my_bg->player_pieces[m.c], m.m.row, m.m.col, 4, &mv1, &mv2, &hole) && (legal(my_bg, mv1) || legal(my_bg, mv2)) && legal(my_bg, hole)))
    {
      i++;
      if (i == 2)
        return 1;
    }
  }
  return 0;
}

int two_three_without_end_h(struct board_game *my_bg, struct col_move_t m)
{
  int i = 0;
  struct col_move_t mv1;
  struct col_move_t mv2;
  struct col_move_t hole;
  int (*fun)(struct bitboard * be, size_t row, size_t col, struct col_move_t * mv1, struct col_move_t * mv2);
  int (*fun_h)(struct bitboard * be, size_t row, size_t col, size_t x, struct col_move_t * mv1, struct col_move_t * mv2, struct col_move_t * hole);
  mv1.c = 0;
  mv2.c = 0;
  for (int j = 0; j < 4; j++)
  {
    fun = bitboard_x_line[j];
    fun_h = bitboard_x_line_h[j];
    if ((fun(my_bg->player_pieces[m.c], m.m.row, m.m.col, &mv1, &mv2) >= 3 && (legal(my_bg, mv1) && legal(my_bg, mv2))) ||
        (fun_h(my_bg->player_pieces[m.c], m.m.row, m.m.col, 3, &mv1, &mv2, &hole) && (legal(my_bg, mv1) && legal(my_bg, mv2)) && legal(my_bg, hole)))
    {
      i++;
      if (i == 2)
        return 1;
    }
  }
  return 0;
}

int three_four_h(struct board_game *my_bg, struct col_move_t m)
{
  int i1 = 0;
  int i2 = 0;
  struct col_move_t mv1;
  struct col_move_t mv2;
  struct col_move_t hole;
  int (*fun)(struct bitboard * be, size_t row, size_t col, struct col_move_t * mv1, struct col_move_t * mv2);
  int (*fun_h)(struct bitboard * be, size_t row, size_t col, size_t x, struct col_move_t * mv1, struct col_move_t * mv2, struct col_move_t * hole);
  mv1.c = 0;
  mv2.c = 0;
  int dir = -1;
  //SEARCHING FOR A 4 ARRAY OPEN IN ONE SIDE
  for (int j = 0; j < 4; j++)
  {
    fun = bitboard_x_line[j];
    fun_h = bitboard_x_line_h[j];
    if ((fun(my_bg->player_pieces[m.c], m.m.row, m.m.col, &mv1, &mv2) >= 4 && !(legal(my_bg, mv1) && legal(my_bg, mv2)) && (legal(my_bg, mv1) || legal(my_bg, mv2))) ||
        (fun_h(my_bg->player_pieces[m.c], m.m.row, m.m.col, 4, &mv1, &mv2, &hole) && !(legal(my_bg, mv1) && legal(my_bg, mv2)) && (legal(my_bg, mv1) || legal(my_bg, mv2)) && legal(my_bg, hole)))
      {i1++;
      dir = j;}
  }
  //SEARCHING FOR A 3 ARRAY OPEN IN BOTH SIDES
  for (int j = 0; j < 4; j++)
  {
    fun = bitboard_x_line[j];
    fun_h = bitboard_x_line_h[j];
    if (((fun(my_bg->player_pieces[m.c], m.m.row, m.m.col, &mv1, &mv2) >= 3 && (legal(my_bg, mv1) && legal(my_bg, mv2))) ||
        (fun_h(my_bg->player_pieces[m.c], m.m.row, m.m.col, 3, &mv1, &mv2, &hole) && (legal(my_bg, mv1) && legal(my_bg, mv2) && legal(my_bg, hole)))) && dir != j)
        {
      i2++;}
  }
  //CHECKING THE RESULTS
  if ((i2 >= 1) && (i1 >= 1))
    return 1;
  return 0;
}

int ligne_of_x_opened_h(struct board_game *my_bg, struct col_move_t m, int x)
{
  struct col_move_t mv1;
  struct col_move_t mv2;
  struct col_move_t hole;
  int (*fun)(struct bitboard * be, size_t row, size_t col, size_t x, struct col_move_t * mv1, struct col_move_t * mv2, struct col_move_t * hole);
  mv1.c = 0;
  for (int i = 0; i < 4; i++)
  {
    fun = bitboard_x_line_h[i];
    if (fun(my_bg->player_pieces[m.c], m.m.row, m.m.col, x, &mv1, &mv2, &hole))
    {
      if (legal(my_bg, mv1) && legal(my_bg, mv2) && legal(my_bg, hole))
        return 1;
    }
  }
  return 0;
}

int eval_move1(struct board_game *my_bg, enum color_t my_color, struct col_move_t m)
{
  if (board_game_win(my_bg, m))
  {
    return WIN_MOVE;
  }
  if (ligne_of_x_opened(my_bg, m, 4))
  {
    // printf("****** ligne_of_x_opened \n");
    return (m.c == my_color) ? BEST_MOVE + 100 : BEST_MOVE;
  }
  if (two_four_with_one_end_h(my_bg, m))
  {
    // printf("****** two_four_with_one_end_h\n");
    return BEST_MOVE;
  }
  if (two_three_without_end_h(my_bg, m))
  {
    // printf("****** two_three_without_end_h\n");
    return BEST_MOVE;
  }
  if (three_four_h(my_bg, m))
  {
    // printf("****** three_four_h\n");
    return BEST_MOVE;
  }
  int count = 0;
  count += 10 * ligne_of_x_opened(my_bg, m, 3) +  3 * ligne_of_x_opened(my_bg, m, 2);
  return count;
}

int eval_move2(struct board_game *my_bg, enum color_t my_color, struct col_move_t m)
{
  if (board_game_win(my_bg, m))
  {
    return BEST_MOVE + 100000;
  }
}

int eval_minimax(struct board_game *my_bg, struct col_move_t *last_moves, int n_moves, enum color_t my_color)
{
  int val = 0;
  int i, j;
  for (i = 0; i < n_moves; i++)
  {
    for (j = i + 1; j < n_moves; j++)
    {
      board_game_removem(my_bg, last_moves[j]);
    }
    if (last_moves[i].c == my_color)
    {
      val += eval_move2(my_bg, last_moves[i].c, last_moves[i]) - 100 * i;
    }
    else
    {
      val -= eval_move2(my_bg, last_moves[i].c, last_moves[i]) + 100 * i;
    }
    for (j = i + 1; j < n_moves; j++)
    {
      board_game_setm(my_bg, last_moves[j]);
    }
  }
  return val;
}
