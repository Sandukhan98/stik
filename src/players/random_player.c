#include "player.h"
#include "board_game.h"
#include <stdlib.h>
#include <time.h>

static struct col_move_t *opening = NULL;
static struct board_game *my_bg = NULL;
static enum color_t my_color;

char const *get_player_name()
{
  return "Randomizer";
}

struct col_move_t *propose_opening(size_t size)
{
  opening = malloc(3 * sizeof(struct col_move_t));
  srand(time(0));
  struct move_t first_move = {rand() % size, rand() % size};
  struct move_t second_move;
  struct move_t third_move;
  do
  {
    second_move.row = rand() % size;
    second_move.col = rand() % size;
  } while ((first_move.row == second_move.row) || (first_move.col == second_move.col));
  do
  {
    third_move.row = rand() % size;
    third_move.col = rand() % size;
  } while (((third_move.row == first_move.row) && (third_move.row == second_move.row)) || ((third_move.col == first_move.col) && (third_move.col == second_move.col)));
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
  srand(time(0));
}

int nb_legal_moves(struct board_game *bg)
{
  int size = (int)board_game_get_size(bg);
  struct col_move_t m;
  m.c = 0;
  int compt = 0;
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      m.m.col = j;
      m.m.row = i;
      if (board_game_legalm(bg, m))
        compt++;
    }
  }
  return compt;
}

struct move_t play(struct col_move_t const previous_moves[], size_t n_moves)
{
  for (int i = 0; i < (int)n_moves; i++)
  {
    board_game_setm(my_bg, previous_moves[i]);
  }
  int size = board_game_get_size(my_bg);
  struct move_t next_move;
  struct col_move_t parkour_move;
  parkour_move.c = my_color;
  int choose_random_move = rand() % nb_legal_moves(my_bg);
  int i = 0;
  int j = 0;
  int compt_legal_moves = 0;
  int tour_de_boucle = 0;
  while ((compt_legal_moves < choose_random_move) && (tour_de_boucle < (size * size)))
  {
    parkour_move.m.row = i;
    parkour_move.m.col = j;
    if (board_game_legalm(my_bg, parkour_move))
      compt_legal_moves++;
    if (j == (int)size - 1)
    {
      i++;
      j = 0;
    }
    else
    {
      j++;
    }
    tour_de_boucle++;
  }
  board_game_setm(my_bg, parkour_move);
  next_move = parkour_move.m;
  return next_move;
}

void finalize()
{
  if (opening != NULL)
  {
    free(opening);
    opening = NULL;
  }
  if (my_bg != NULL)
  {
    board_game_free(my_bg);
    my_bg = NULL;
  }
}
