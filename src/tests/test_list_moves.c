#include <assert.h>
#include "list_moves.h"

void test__moves__add(void)
{
  printf("Test de moves__add...");
  struct list_moves *moves = moves__empty();
  struct col_move_t move1 = {{5, 6}, WHITE};
  assert(moves__add(moves, move1));
  assert(moves->size == 1);
  assert(moves->moves[0].m.row == 5);
  printf("OK\n");
}


void test__moves__last_n(void)
{
  printf("Test de moves__last_n...");
  struct list_moves *moves = moves__empty();
  struct col_move_t move1 = {{5, 6}, WHITE};
  struct col_move_t move2 = {{1, 1}, BLACK};
  struct col_move_t move3 = {{4, 3}, WHITE};
  assert(moves__add(moves, move1));
  assert(moves__add(moves, move2));
  assert(moves__add(moves, move3));
  struct col_move_t *last = moves__last_n(moves, 2);
  assert(last[0].m.row == move2.m.row);
  assert(last[1].m.row == move3.m.row);
  printf("OK\n");
}


void test__moves__is_fake(void)
{
  printf("Test de moves__is_fake...");
  struct list_moves *moves = moves__empty();
  struct col_move_t move1 = {{5, 6}, WHITE};
  struct col_move_t move2 = {{1, 1}, BLACK};
  struct col_move_t move3 = fake_stroke;
  assert(moves__add(moves, move1));
  assert(moves__add(moves, move2));
  assert(moves__add(moves, move3));
  struct col_move_t *last = moves__last_n(moves, 2);
  assert(moves__is_fake(last, 2) == 1);
  printf("OK\n");
}


int main()
{
  printf("Test list_moves\n");
  test__moves__add();
  test__moves__last_n();
  test__moves__is_fake();
  return 0;
}
