#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "move.h"
#include "list_moves.h"
#include "list_players.h"
#include "option.h"
#include "board_game.h"

#ifdef DISP
#include "display.h"
#endif

static struct player_t *p;
static int win_flag = 0;
static int draw_flag = 0;
static struct col_move_t *last_moves;
static struct list_moves *moves;
static struct list_players *players;
static struct options *options;
static struct board_game *board;

void set_options(int argc, char *argv[]);
void initialize_game();
void play_with_opening();
void play_without_opening();
void finalize_game(struct col_move_t *m);

int main(int argc, char *argv[])
{
    options = options_new();
    set_options(argc, argv);
    size_t size = options_size(options);
    if (size > 11)
    {
        options_free(options);
        printf("given size is higher than 11\n");
        exit(0);
    }
    players = list_players__empty();
    if (!list_players__initialize(players, options_player(options), options_arg_number(options)))
    {
        options_free(options);
        list_players__free(players);
        printf("initialize list_players error\n");
        exit(0);
    }
    moves = moves__empty();
    board = board_game_empty(size, options_arg_number(options));
    initialize_game();
    if (options_with_opening(options))
        play_with_opening();
    play_without_opening();
}

void set_options(int argc, char *argv[])
{
    int opt, o_found, n_found;
    opt = 0;
    o_found = 0;
    n_found = 0;
    while ((opt = getopt(argc, argv, "n:o")) != -1)
    {
        switch (opt)
        {
        case 'n':
            n_found = 1;
            options_set_size(options, (size_t)atoi(optarg));
            break;
        case 'o':
            o_found = 1;
            break;
        default:;
        }
    }
    if (!n_found)
    {
        printf("no size is given\n");
        exit(0);
    }
    options_set_opening(options, o_found);
    if (optind >= argc)
    {
        printf("Expected arguments after options\n");
        exit(0);
    }
    argv += optind;
    options_add_players(options, argv, argc - optind);
}

int initialize_with_opening()
{
    p = list_players__next(NULL, players);
    struct col_move_t *moves_op;
    struct col_move_t col_m;
    struct move_t m;
    size_t size = board_game_get_size(board);
    printf("with opening\n");
    moves_op = p->propose_opening(size);
    for (int i = 0; i < 3; i++)
    {
        moves__add(moves, moves_op[i]);
        if (!board_game_legalm(board, moves_op[i]))
        {
            printf("%s is stupid\n", p->get_player_name());
            return 0;
        }
        board_game_setm(board, moves_op[i]);
    }
    if (list_players__next(p, players)->accept_opening(size, moves_op))
    {
        p->initialize(size, BLACK);
        list_players__next(p, players)->initialize(size, WHITE);
        p = list_players__next(p, players);
    }
    else
    {
        p->initialize(size, WHITE);
        p->c = WHITE;
        list_players__next(p, players)->initialize(size, BLACK);
        list_players__next(p, players)->c = BLACK;
    }
#ifdef DISP
    printf("display defined ;) make sure that server is ready and http://localhost:9000/test.html is opened in your browser\n");
    init_display(size, moves_op, 1, list_players__next(p, players)->get_player_name(), p->get_player_name());
#endif
    for (int i = 0; i < 2; i++)
    {
        last_moves = moves__last_n(moves, i + 3);
        m = p->play(last_moves, moves__is_fake(last_moves, i + 3));
        col_m.m = m;
        col_m.c = p->c;
        if (!board_game_legalm(board, col_m))
        {
            printf("%s is stupid\n", p->get_player_name());
            return 0;
        }
#ifdef DISP
        display_move(m, col_m.c);
#endif
        moves__add(moves, col_m);
        board_game_setm(board, col_m);
        p = list_players__next(p, players);
    }
    return 1;
}

int initialize_without_opening()
{
    p = list_players__next(NULL, players);
    struct col_move_t col_m;
    struct move_t m;
#ifdef DISP
    printf("display defined ;) make sure that server is ready and http://localhost:9000/test.html is opened in your browser\n");
    init_display(board_game_get_size(board), NULL, 0, p->get_player_name(), list_players__next(p, players)->get_player_name());
#endif
    for (size_t i = 0; (int)i < list_players__numb(players); i++)
    {
        p->initialize(board_game_get_size(board), i);
        last_moves = moves__last_n(moves, i);
        m = p->play(last_moves, moves__is_fake(last_moves, i));
#ifdef DISP
        display_move(m, i);
#endif
        col_m.m = m;
        col_m.c = i;
        if (!board_game_legalm(board, col_m))
        {
            printf("%s is stupid\n", p->get_player_name());
            return 0;
        }
        moves__add(moves, col_m);
        board_game_setm(board, col_m);
        p = list_players__next(p, players);
    }
    return 1;
}

void initialize_game()
{
    if (options_with_opening(options))
    {
        if (!initialize_with_opening())
        {
            finalize_game(NULL);
        }
        return;
    }
    if (!initialize_without_opening())
    {
        finalize_game(NULL);
    }
    return;
}

void play_with_opening()
{
    struct col_move_t col_m;
    struct move_t m;
    for (int i = 0; i < 116; i++)
    {
        last_moves = moves__last_n(moves, 2);
        m = p->play(last_moves, moves__is_fake(last_moves, 2));
        col_m.m = m;
        col_m.c = player__get_color(p);
        if (!board_game_legalm(board, col_m))
        {
            printf("%s is stupid\n", p->get_player_name());
            finalize_game(&col_m);
        }
#ifdef DISP
        display_move(m, col_m.c);
#endif
        moves__add(moves, col_m);
        board_game_setm(board, col_m);
        win_flag = board_game_win(board, col_m);
        draw_flag = board_game_is_full(board);
        if (win_flag || draw_flag)
        {
            if (win_flag)
                printf("%s won the game\n", p->get_player_name());
            else
                printf("DRAW !\n");
            finalize_game(&col_m);
        }
        p = list_players__next(p, players);
    }
    finalize_game(&col_m);
}

void play_without_opening()
{
    int n = options_arg_number(options);
    struct col_move_t col_m;
    struct move_t m;
    for (int i = 0; i < 121; i++)
    {
        last_moves = moves__last_n(moves, n);
        m = p->play(last_moves, moves__is_fake(last_moves, n));
        col_m.m = m;
        col_m.c = player__get_color(p);
        if (!board_game_legalm(board, col_m))
        {
            printf("%s is stupid\n", p->get_player_name());
            finalize_game(&col_m);
        }
#ifdef DISP
        display_move(m, col_m.c);
#endif
        moves__add(moves, col_m);
        board_game_setm(board, col_m);
        win_flag = board_game_win(board, col_m);
        draw_flag = board_game_is_full(board);
        if (win_flag || draw_flag)
        {
            if (win_flag)
                printf("%s won the game\n", p->get_player_name());
            else
                printf("DRAW !\n");
            finalize_game(&col_m);
        }
        p = list_players__next(p, players);
    }
    finalize_game(&col_m);
}

void finalize_game(struct col_move_t *m)
{
    list_players__finalize(players, options_arg_number(options));
    options_free(options);
    list_players__free(players);
    moves__free(moves);
    board_game_free(board);
#ifdef DISP
    if (draw_flag)
        final_display(0, "4", m);
    else if (win_flag)
        final_display(1, "2", m);
    else
        final_display(2, "3", m);
#endif
    UNUSED(m);
    exit(0);
}
