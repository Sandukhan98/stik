#include "move.h"
#include "player.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int passed_tests_numb = 0;
static int total_tests = 0;

void test(char msg[], int result)
{
  printf("%s ... : %s\n", msg, (result == 0) ? "\033[1;31m FAILED \033[0m" : "\033[1;32m PASSED \033[0m");
  if (result)
    passed_tests_numb++;
  total_tests++;
}

int main()
{
  printf("\n\033[1;33m =======TEST SAME_MOVE_PLAYER=======\033[0m\n");
  test("Name of player is Dumb player", strcmp("Dumb player", get_player_name()) == 0);
  struct col_move_t *opening = propose_opening(5);
  test("Accept his own opening", accept_opening(5, opening));

  finalize();
  printf("%d / %d Tests passed\n\n", passed_tests_numb, total_tests);
  return EXIT_SUCCESS;
}
