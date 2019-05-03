#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "option.h"

struct options *options_new()
{
  struct options *opt = malloc(sizeof(struct options));
  opt->opening = 0;
  opt->size = 0;
  opt->number_of_players = 0;
  return opt;
}

int options_with_opening(struct options *opt)
{
  return opt->opening;
}

int options_size(struct options *opt)
{
  return opt->size;
}

int options_arg_number(struct options *opt)
{
  return opt->number_of_players;
}

char **options_player(struct options *opt)
{
  return opt->tab;
}

void options_set_opening(struct options *opt, int bool)
{
  opt->opening = bool;
}

void options_set_size(struct options *opt, int size)
{
  opt->size = size;
}

int options_add_players(struct options *opt, char **players, int number)
{
  if (options_arg_number(opt) < SIZE_MAX)
  {
    for (int i = 0; i < number; i++)
    {
      opt->tab[i] = players[i];
    }
    opt->number_of_players = number;
    return SUCCESS;
  }
  return MAX_REACHED;
}

void options_free(struct options *opt)
{
  free(opt);
}
