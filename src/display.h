#ifndef DISP_H
#define DISP_H
#include <stdio.h>
#include <stdlib.h>
#include "board_game.h"
void display_move(struct move_t m, enum color_t c);
void init_display(size_t size, struct col_move_t *opening, char open, char *black, char *white);
void final_display(char win, char *state, struct col_move_t *m);
void display_board(struct board_game bg);
#endif