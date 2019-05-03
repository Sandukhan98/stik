#ifndef EVAL_MOVE_H
#define EVAL_MOVE_H
#include "board_game.h"
#define BEST_MOVE 300
#define WIN_MOVE 100000
int eval_move(struct board_game *my_bg, enum color_t my_color, struct col_move_t m);
int eval_move1(struct board_game *my_bg, enum color_t my_color, struct col_move_t m);
int eval_move2(struct board_game * my_bg,enum color_t my_color, struct col_move_t m);
int eval_minimax(struct board_game *my_bg, struct col_move_t *last_moves, int n_moves, enum color_t my_color);
#endif
