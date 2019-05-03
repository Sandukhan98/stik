#ifndef _LIST_PLAYERS_H_
#define _LIST_PLAYERS_H_
#define PL_MAX_PLAYERS 10
#include "move.h"

struct player_t
{
  enum color_t c;
  char const *(*get_player_name)();
  struct col_move_t *(*propose_opening)(size_t);
  int (*accept_opening)(size_t, const struct col_move_t *);
  void (*initialize)(size_t, enum color_t);
  struct move_t (*play)(struct col_move_t *const, size_t);
  void (*finalize)();
  struct player_t *next;
  void *handler;
};

struct list_players
{
  struct player_t players[PL_MAX_PLAYERS]; //Tableau de pointeurs de struct player_t
  int numb;
};

// Return a new list_players dynamically allocated
struct list_players *list_players__empty();

// Return color of a player
enum color_t player__get_color(struct player_t const *player);

// Initialize each player of the list_players "lp" with the path of dynamic library corresponding to each player in "tab", return 1 if success and 0 if error
int list_players__initialize(struct list_players *lp, char **tab, int numb);

//Returne number of players
int list_players__numb(struct list_players const *lp);

// Return the player who plays next according to the "actual_player". If "actual_player" is NULL, return first element of list_players "lp"
struct player_t *list_players__next(struct player_t const *actual_player, struct list_players *lp);

// Finalize each player of the list_players "lp"
void list_players__finalize(struct list_players *lp, int numb);
// Free allocated memory
void list_players__free(struct list_players *lp);

#endif
