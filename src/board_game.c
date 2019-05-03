#include "limits.h"
#include "board_game.h"

struct board_game *board_game_empty(size_t size, size_t numb)
{
    struct board_game *bg = malloc(sizeof(struct board_game));
    bg->size = size;
    bg->numb = numb;
    for (int i = 0; i < (int)numb; i++)
    {
        bg->player_pieces[i] = bitboard_empty(size);
    }
    return bg;
}


size_t board_game_get_size(struct board_game *bg)
{
    return bg->size;
}


size_t board_game_get_numb(struct board_game *bg)
{
    return bg->numb;
}


struct bitboard *board_game_get_player(struct board_game *bg, enum color_t c)
{
    return bg->player_pieces[c];
}


void board_game_setm(struct board_game *bg, struct col_move_t m)
{
    bitboard_set(board_game_get_player(bg, m.c), m.m.row, m.m.col);
}


enum color_t board_game_get_color(struct board_game *bg, struct move_t m)
{
    struct col_move_t cm = {m, 0};
    if (board_game_legalm(bg, cm) || board_game_is_out(bg, cm))
        return -1;
    enum color_t res = 0;
    for (size_t i = 0; i < board_game_get_numb(bg); i++)
    {
        res += bitboard_get(bg->player_pieces[i], m.row, m.col) * i;
    }
    return res;
}


void board_game_removem(struct board_game *bg, struct col_move_t m)
{
    bitboard_remove(board_game_get_player(bg, m.c), m.m.row, m.m.col);
}


int board_game_is_out(struct board_game *bg, struct col_move_t m)
{
    return (m.m.col >= board_game_get_size(bg) || m.m.row >= board_game_get_size(bg));
}


int board_game_legalm(struct board_game *bg, struct col_move_t m)
{
    if (board_game_is_out(bg, m))
        return 0;
    for (size_t i = 0; i < board_game_get_numb(bg); i++)
    {
        if (bitboard_get(board_game_get_player(bg, i), m.m.row, m.m.col))
            return 0;
    }
    return 1;
}


int board_game_is_full(struct board_game *bg)
{
    unsigned __int128 full = ULONG_MAX;
    unsigned __int128 union_boards = 0;
    size_t s = board_game_get_size(bg);
    full = UNION(full << 64, ULONG_MAX);
    for (size_t i = 0; i < board_game_get_numb(bg); i++)
    {
        union_boards = UNION(union_boards, board_game_get_player(bg, i)->bitboard);
    }
    full = full >> (128 - s * s);
    return !SYM_DIFF(full, union_boards);
}


int board_game_win(struct board_game *bg, struct col_move_t m)
{
    struct col_move_t tmp1;
    struct col_move_t tmp2;
    return (bitboard_big_row_line(board_game_get_player(bg, m.c), m.m.row, m.m.col, &tmp1, &tmp2) >= 5 ||
            bitboard_big_col_line(board_game_get_player(bg, m.c), m.m.row, m.m.col, &tmp1, &tmp2) >= 5 ||
            bitboard_big_diagr_line(board_game_get_player(bg, m.c), m.m.row, m.m.col, &tmp1, &tmp2) >= 5 ||
            bitboard_big_diagl_line(board_game_get_player(bg, m.c), m.m.row, m.m.col, &tmp1, &tmp2) >= 5);
}


void board_game_free(struct board_game *bg)
{
    for (int i = 0; i < (int)bg->numb; i++)
    {
        bitboard_free(bg->player_pieces[i]);
    }
    free(bg);
}