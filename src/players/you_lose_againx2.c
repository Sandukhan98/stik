#include "board_game.h"
#include "eval_move.h"
#include "display.h"
#include <time.h>

static struct col_move_t *opening = NULL;
static struct board_game *my_bg = NULL;
static enum color_t my_color;
static const int max_depth = 3;
const int MAX = 10000000;
const int MIN = -10000000;

int minimax(int depth, struct col_move_t move, int maxmize, int (*eval_fun)(), int alpha, int beta);

char const *get_player_name()
{
    return "Lord of the game";
}

struct col_move_t *propose_opening(size_t size)
{
    opening = malloc(3 * sizeof(struct col_move_t));
    struct move_t first_move = {0, 0};
    struct move_t second_move = {size - 1, size - 1};
    struct move_t third_move = {0, 1};
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
    return 1;
}

void initialize(size_t size, enum color_t id)
{
    srand(412);
    my_bg = board_game_empty(size, 4);
    my_color = id;
}
time_t start_t, end_t;
int count = 0;
struct move_t play(struct col_move_t const previous_moves[], size_t n_moves)
{
    time(&start_t);
    count = 0;
    // printf("======================================================= \n");
    struct col_move_t m = {{0, 0}, my_color};
    struct col_move_t next_move = {{0, 0}, my_color};
    if (n_moves == 0)
    {
        int r = rand();
        m.m.row = r % (my_bg->size - 1) + 1;
        m.m.col = (r * r) % (my_bg->size - 1) + 1;
        m.c = my_color;
        board_game_setm(my_bg, m);
        return m.m;
    }
    for (size_t k = 0; k < n_moves; k++)
    {
        board_game_setm(my_bg, previous_moves[k]);
    }
    unsigned __int128 u = (my_bg->player_pieces[0]->bitboard | my_bg->player_pieces[1]->bitboard);
    struct bitboard *to_eval = bitboard_create(my_bg->size, UNION(NBORS(UNION(u, NBORS(u, 1, my_bg->size)), 1, my_bg->size), NBORS(u, 1, my_bg->size)));
    // struct bitboard *to_eval = bitboard_create(my_bg->size, NBORS(u, 1, my_bg->size));
    int best = MIN;
    char break_flag = 0;
    for (size_t i = 0; i < my_bg->size; i++)
    {
        for (size_t j = 0; j < my_bg->size; j++)
        {
            if (bitboard_get(to_eval, i, j))
            {
                m.m.row = i;
                m.m.col = j;
                m.c = my_color;
                board_game_setm(my_bg, m);
                int val;
                if (board_game_is_full(my_bg))
                {
                    board_game_removem(my_bg, m);
                    best = 0;
                    break_flag = 1;
                    next_move.m.row = i;
                    next_move.m.col = j;
                    break;
                }
                else if (board_game_win(my_bg, m))
                {
                    board_game_removem(my_bg, m);
                    best = WIN_MOVE;
                    break_flag = 1;
                    next_move.m.row = i;
                    next_move.m.col = j;
                    break;
                }
                else
                {
                    val = minimax(0, m, 0, eval_move, MIN, MAX);
                    // m.c = (my_color + 1) % 2;
                    val += eval_move(my_bg, my_color, m);
                    val *= max_depth;
                }
                // printf(" %ld   ||   %ld    =>   %d\n", m.m.row, m.m.col, val);
                board_game_removem(my_bg, m);
                if (best < val)
                {
                    next_move.m.row = i;
                    next_move.m.col = j;
                    best = val;
                }
            }
        }
        if (break_flag)
            break;
    }
    board_game_setm(my_bg, next_move);
    return next_move.m;
}

void finalize()
{
    if (opening != NULL)
        free(opening);
    if (my_bg != NULL)
        board_game_free(my_bg);
}

int minimax(int depth, struct col_move_t move, int maxmize, int (*eval_fun)(), int alpha, int beta)
{
    count++;
    time(&end_t);
    printf("time : %f s ||  %d  NODES\n",difftime(end_t, start_t),count);
    if (depth == max_depth)
        return (move.c == my_color) ? eval_fun(my_bg, my_color, move) : -eval_fun(my_bg, my_color, move);

    unsigned __int128 u = (my_bg->player_pieces[0]->bitboard | my_bg->player_pieces[1]->bitboard);
    struct bitboard *to_eval = bitboard_create(my_bg->size, UNION(NBORS(UNION(u, NBORS(u, 1, my_bg->size)), 1, my_bg->size), NBORS(u, 1, my_bg->size)));
    // struct bitboard *to_eval = bitboard_create(my_bg->size, NBORS(u, 1, my_bg->size));
    struct col_move_t m;
    char break_flag = 0;
    if (maxmize)
    {
        m.c = my_color;
        int best = MIN;
        for (size_t i = 0; i < my_bg->size; i++)
        {
            for (size_t j = 0; j < my_bg->size; j++)
            {
                if (bitboard_get(to_eval, i, j))
                {
                    m.m.row = i;
                    m.m.col = j;
                    m.c = my_color;
                    board_game_setm(my_bg, m);
                    int val;
                    if (board_game_is_full(my_bg))
                    {
                        board_game_removem(my_bg, m);
                        best = 0;
                        break_flag = 1;
                        break;
                    }
                    else if (board_game_win(my_bg, m))
                    {
                        board_game_removem(my_bg, m);
                        best = WIN_MOVE;
                        break_flag = 1;
                        break;
                    }
                    else
                    {
                        val = minimax(depth + 1, m, 0, eval_fun, alpha, beta);
                        // m.c = (my_color + 1) % 2;
                        val += eval_move(my_bg, my_color, m);
                        val *= (max_depth - depth - 1);
                    }
                    board_game_removem(my_bg, m);
                    best = max(best, val);
                    alpha = max(alpha, best);
                    if (beta <= alpha)
                    {
                        break_flag = 1;
                        break;
                    }
                }
            }
            if (break_flag)
                break;
        }
        return best;
    }
    else
    {
        m.c = (my_color + 1) % 2;
        int best = MAX;
        for (size_t i = 0; i < my_bg->size; i++)
        {
            for (size_t j = 0; j < my_bg->size; j++)
            {
                if (bitboard_get(to_eval, i, j))
                {
                    m.m.row = i;
                    m.m.col = j;
                    board_game_setm(my_bg, m);
                    int val;
                    if (board_game_is_full(my_bg))
                    {
                        board_game_removem(my_bg, m);
                        best = 0;
                        break_flag = 1;
                        break;
                    }
                    if (board_game_win(my_bg, m))
                    {
                        board_game_removem(my_bg, m);
                        best = -WIN_MOVE;
                        break_flag = 1;
                        break;
                    }
                    else
                    {
                        val = minimax(depth + 1, m, 1, eval_fun, alpha, beta);
                        // m.c = my_color;
                        val -= eval_move(my_bg, my_color, m);
                        val *= (max_depth - depth - 1);
                    }
                    board_game_removem(my_bg, m);
                    best = min(best, val);
                    beta = min(beta, best);
                    if (beta <= alpha)
                    {
                        break_flag = 1;
                        break;
                    }
                }
            }
            if (break_flag)
                break;
        }
        return best;
    }
}
