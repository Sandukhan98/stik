#include "board_game.h"
static int test_numb = 0;
static int total_test = 0;


void passed(char *s)
{
    printf("test %s \033[1;32m PASSED \033[0m\n", s);
    test_numb++;
    total_test++;
}


void failed(char *s)
{
    printf("test %s \033[1;31m FAILED \033[0m\n", s);
    total_test++;
}


void test_board_empty()
{
    struct board_game *bg = board_game_empty(5, 2);
    if (bg->size == 5 && bg->numb == 2 && bg->player_pieces[0]->size == 5 && bg->player_pieces[1]->size == 5)
        passed("create empty board game");
    else
        failed("create empty board game");
    board_game_free(bg);
}
void test_get_player()
{
    struct board_game *bg = board_game_empty(5, 3);
    bitboard_set(bg->player_pieces[1], 3, 3);
    if (bitboard_get(board_game_get_player(bg, 1), 3, 3))
        passed("get player");
    else
        failed("get player");
    board_game_free(bg);
}


void test_get_size_numb()
{
    struct board_game *bg = board_game_empty(5, 3);
    if (board_game_get_size(bg) == 5 && board_game_get_numb(bg) == 3)
        passed("get set and get numb");
    else
        failed("get set and get numb");
    board_game_free(bg);
}
void test_get_color()
{
    struct board_game *bg = board_game_empty(5, 3);
    struct col_move_t move;
    move.m.col = 3;
    move.m.row = 3;
    move.c = WHITE;
    board_game_setm(bg, move);
    if (board_game_get_color(bg, move.m) == WHITE)
        passed("get color of pawn");
    else
        failed("get color of pawn");
    board_game_free(bg);
}


void test_set_move()
{
    struct board_game *bg = board_game_empty(9, 3);
    struct col_move_t move;
    move.m.col = 3;
    move.m.row = 3;
    move.c = WHITE;
    board_game_setm(bg, move);
    move.m.col = 3;
    move.m.row = 8;
    move.c = BLACK;
    board_game_setm(bg, move);
    if (bitboard_get(bg->player_pieces[0], 8, 3) && bitboard_get(bg->player_pieces[1], 3, 3))
        passed("set a move in board");
    else
        failed("set a move in board");
    board_game_free(bg);
}


void test_out()
{
    struct board_game *bg = board_game_empty(5, 3);
    struct col_move_t move;
    move.m.col = 3;
    move.m.row = 3;
    move.c = WHITE;
    if (!board_game_is_out(bg, move))
        passed("verify if a move in the board is out of the board");
    else
        failed("verify if a move in the board is out of the board");
    move.m.col = -1;
    move.m.row = 3;
    move.c = WHITE;
    if (!board_game_is_out(bg, move))
        passed("verify if a move with negative coordinate is out of the board");
    else
        failed("verify if a move with negative coordinate is out of the board");
    move.m.col = 3;
    move.m.row = 8;
    move.c = WHITE;
    if (!board_game_is_out(bg, move))
        passed("verify if a move with row value higher than board's size, is out of the board");
    else
        failed("verify if a move with row value higher than board's size, is out of the board");
    board_game_free(bg);
}


void test_legal_move()
{
    struct board_game *bg = board_game_empty(5, 3);
    struct col_move_t move;
    move.m.col = 3;
    move.m.row = 3;
    move.c = WHITE;
    if (board_game_legalm(bg, move))
        passed("verify a move in empty board");
    else
        failed("verify a move in empty board");
    board_game_setm(bg, move);
    if (!board_game_legalm(bg, move))
        passed("verify a move in board that has already the same move");
    else
        failed("verify a move in board that has already the same move");
    move.c = BLACK;
    if (!board_game_legalm(bg, move))
        passed("verify a move in board that has already the same position different color");
    else
        failed("verify a move in board that has already the same position different color");
    move.m.row = move.m.row - 4;
    if (board_game_legalm(bg, move))
        failed("check if a position out of range is legal");
    move.m.row = 4;
    move.m.col = 5;
    if (board_game_legalm(bg, move))
        failed("check if a position out of range is legal");
    else
        passed("check if a position out of range is legal");
    board_game_free(bg);
}


void test_is_full()
{
    struct board_game *bg = board_game_empty(5, 3);
    if (!board_game_is_full(bg))
        passed("if an empty board game is full (size 5)");
    else
        failed("if an empty board game is full (size 5)");
    struct col_move_t move;
    move.c = WHITE;
    for (size_t i = 0; i < 5; i++)
    {
        move.m.row = i;
        for (size_t j = 0; j < 5; j++)
        {
            move.m.col = j;
            move.c = (i + j) % 3;
            board_game_setm(bg, move);
            if ((i != 4 && j != 4) && board_game_is_full(bg))
                failed("if non-full board game is full (size 5)");
        }
    }
    if (board_game_is_full(bg))
        passed("if a full board game is full (size 5)");
    else
        failed("if a full board game is full(size 5)");
    board_game_free(bg);
}


void test_win()
{
    struct board_game *bg = board_game_empty(8, 2);
    struct col_move_t m;
    m.c = 0;
    for (size_t i = 0; i < 4; i++)
    {
        
            m.m.row = 2 + i;
            m.m.col = i;
            board_game_setm(bg, m);
            if (board_game_win(bg, m))
                failed("board_game_win return true for non wining move");
    }
    m.m.row = 6;
    m.m.col = 4;
    board_game_setm(bg, m);
    if (board_game_win(bg, m))
        passed("board_game_win return true for wining move");
    else
        failed("board_game_win return true for wining move");
    board_game_free(bg);
}


int main()
{
    printf("\n\033[1;33m =======TEST BOARD_GAME=======\033[0m\n");
    test_board_empty();
    test_get_player();
    test_get_size_numb();
    test_get_color();
    test_set_move();
    test_legal_move();
    test_is_full();
    test_win();
    printf("%d / %d test passed\n", test_numb, total_test);
    return 0;
}
