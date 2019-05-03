#ifndef BOARD_GAME_H
#define BOARD_GAME_H
#include "bitboard.h"
#define MAX_PLAYERS 10

struct board_game
{
    struct bitboard *player_pieces[MAX_PLAYERS];
    size_t size;
    size_t numb;
};
// creates new board game
struct board_game *board_game_empty(size_t size, size_t numb);
// returns the elementary board for the player that has the color c
struct bitboard *board_game_get_player(struct board_game *bg, enum color_t c);
// sets a move in the game board
void board_game_setm(struct board_game *bg, struct col_move_t m);
// return the pawn's color in the given position
enum color_t board_game_get_color(struct board_game *bg, struct move_t m);
// removes a move from the game board
void board_game_removem(struct board_game *bg, struct col_move_t m);
//returns the size of the board
size_t board_game_get_size(struct board_game *bg);
//returns the number of board's players
size_t board_game_get_numb(struct board_game *bg);
// checks if the move m is out of board
int board_game_is_out(struct board_game *bg, struct col_move_t m);
// checks if m is a legal move
int board_game_legalm(struct board_game *bg, struct col_move_t m);
// checks if the board game is full
int board_game_is_full(struct board_game *bg);
// checks if m is a wining move
int board_game_win(struct board_game *bg, struct col_move_t m);

// free a board game
void board_game_free(struct board_game *bg);
#endif