#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "option.h"

int test_with_opening(struct options *opt)
{
  return options_with_opening(opt);
}

int test_correct_size(struct options *opt)
{
  return options_size(opt);
}

int test_arg_number(struct options *opt)
{
  return options_arg_number(opt);
}

int test_add_opt_player(struct options *opt, char **player, int size)
{
  return options_add_players(opt, player, size);
}

int main()
{
  struct options *opt = options_new();
  int i = 0;

  printf("\033[1;33m =======TEST OPTIONS======= \033[0m\n");

  //HERE WE DO TESTS ON A INITIALIZED OPTIONS STRUCT

  printf("Testing with a initialized options: NO-OPENING               ::");
  if (test_with_opening(opt) == 0)
  {
    printf("\033[1;32m PASSED \033[0m\n");
    i++;
  }
  else
  {
    printf("\033[1;31m FAILED \033[0m\n");
  }

  printf("Testing with a initialized options: CORRECT SIZE             ::");
  if (test_correct_size(opt) == 0)
  {
    printf("\033[1;32m PASSED \033[0m\n");
    i++;
  }
  else
  {
    printf("\033[1;31m FAILED \033[0m\n");
  }

  printf("Testing with a initialized options: CORRECT NUMBER OF PLAYERS::");
  if (test_arg_number(opt) == 0)
  {
    printf("\033[1;32m PASSED \033[0m\n");
    i++;
  }
  else
  {
    printf("\033[1;31m FAILED \033[0m\n");
  }

  //HERE WE CHANGE THE VALUE OF OPENING IN THE STRUCT

  options_set_opening(opt, 1);
  printf("Testing with an opening option: OPEN                         ::");
  if (test_with_opening(opt) == 1)
  {
    printf("\033[1;32m PASSED \033[0m\n");
    i++;
  }
  else
  {
    printf("\033[1;31m FAILED \033[0m\n");
  }

  //HERE WE SET A NEW SIZE OPTION

  options_set_size(opt, 5);
  printf("Testing with a given size option: CORRECT SIZE               ::");
  if (test_correct_size(opt) == 5)
  {
    printf("\033[1;32m PASSED \033[0m\n");
    i++;
  }
  else
  {
    printf("\033[1;31m FAILED \033[0m\n");
  }

  //HERE WE ADD A NEW PLAYER

  char *ham[] = {"TEAM 6279", "team 6279"};
  options_add_players(opt, ham, 2);
  printf("Testing with players in options: CORRECT NUMBER OF PLAYERS   ::");
  if (test_arg_number(opt) == 2)
  {
    printf("\033[1;32m PASSED \033[0m\n");
    i++;
  }
  else
  {
    printf("\033[1;31m FAILED \033[0m\n");
  }
  printf("Testing with players in options: CORRECT PLAYER              ::");
  char **a = options_player(opt);
  if (strcmp(a[options_arg_number(opt) - 1], "team 6279") == 0)
  {
    printf("\033[1;32m PASSED \033[0m\n");
    i++;
  }
  else
  {
    printf("\033[1;31m FAILED \033[0m\n");
  }

  options_free(opt);

  printf("%d / 7 test passed\n", i);

  return 0;
}
