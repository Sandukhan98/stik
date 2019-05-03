#ifndef OPTION_H
#define OPTION_H

#define SIZE_MAX 10
#define SUCCESS 1
#define MAX_REACHED 999

struct options
{
  int opening;
  int size;
  char *tab[SIZE_MAX];
  int number_of_players;
};

struct options *options_new();
int options_with_opening(struct options *);
int options_size(struct options *);
int options_arg_number(struct options *);
char **options_player(struct options *);
void options_set_opening(struct options *, int);
void options_set_size(struct options *, int);
int options_add_players(struct options *, char **, int);
void options_free(struct options *);

#endif
