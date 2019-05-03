#include "move.h"
#include "player.h"
#include "board_game.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int passed_tests_numb = 0;
static int total_tests = 0;

static struct col_move_t *opening = NULL;
static struct board_game *my_bg = NULL;

void test(char msg[], int result)
{
  printf("%s ... : %s\n", msg, (result == 0) ? "\033[1;31m FAILED \033[0m" : "\033[1;32m PASSED \033[0m");
  if (result)
    passed_tests_numb++;
  total_tests++;
}

void print_move(struct col_move_t move)
{
  printf("(%zu,%zu)\n", move.m.row, move.m.col);
}

int test_opening_in_range(struct col_move_t *opening, size_t size)
{
  int ret = 1;
  for (int i = 0; i < 3; i++)
  {
    ret = ret && (opening[i].m.row <= size);
    ret = ret && (opening[i].m.col <= size);
  }
  return ret;
}

void test_random_opening()
{
  size_t size = 11;
  opening = propose_opening(size);
  for (int i = 0; i < 3; i++)
  {
    print_move(opening[i]);
  }
  test("Opening in range of the board's size", test_opening_in_range(opening, size));
  test("Accept his own (random) opening", accept_opening(size, opening));
  finalize();
}

int test_nb_legal_moves()
{
  int size = (int)board_game_get_size(my_bg);
  struct col_move_t m;
  m.c = 0;
  int compt = 0;
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      m.m.col = j;
      m.m.row = i;
      if (board_game_legalm(my_bg, m))
        compt++;
    }
  }
  return compt;
}

void test_play_random()
{
  size_t size = 5;
  my_bg = board_game_empty(size, 2);
  struct col_move_t m;
  m.c = 0;
  m.m.row = 0;
  test("Number of legal moves in empty board", test_nb_legal_moves(my_bg) == 25);
  for (int i = 0; i < 5; i++)
  {
    m.m.col = i;
    if (board_game_legalm(my_bg, m))
      board_game_setm(my_bg, m);
  }
  test("Number of legal moves in board with first line completed", test_nb_legal_moves(my_bg) == 20);
  finalize();
}

int main()
{
  printf("\n\033[1;33m =======TEST RANDOM_PLAYER=======\033[0m\n");
  test_random_opening();
  test_play_random();

  printf("%d / %d Tests passed\n\n", passed_tests_numb, total_tests);
  return EXIT_SUCCESS;
}
