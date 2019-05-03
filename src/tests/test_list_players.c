#include "list_players.h"
#include <stdio.h>

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


void test_initialize()
{
    char *a[] = {"./test_player.so", "./test_player.so"};
    struct list_players *lp = list_players__empty();
    if (!list_players__initialize(lp, a, 5))
        passed("initialize list with number higher than max players and error must be returned");
    else
        failed("initialize list with number higher than max players and error must be returned");
    list_players__initialize(lp, a, 2);
    struct player_t *p = list_players__next(NULL, lp);
    struct col_move_t *m = p->propose_opening(10);
    if (m[0].m.row == 0 && m[1].m.row == 0 && m[0].c == BLACK && m[1].c == WHITE)
        passed("moves of opening after intialising list players");
    else
        failed("moves of opening after intialising list players");
    if (lp->players[0].next == &lp->players[1] && lp->players[1].next == &lp->players[0])
        passed("if players are well linked after initializing");
    else
        failed("if players are well linked after initializing");
    p->finalize();
    list_players__free(lp);
}


void test_list_numb()
{
    char *a[] = {"./test_player.so", "./test_player.so"};
    struct list_players *lp = list_players__empty();
    list_players__initialize(lp, a, 2);
    if (list_players__numb(lp) == 2)
        passed("if list_numb return the right value");
    else
        failed("if list_numb return the right value");
    list_players__free(lp);
}


void test_next()
{
    char *a[] = {"./test_player.so", "./test_player.so"};
    struct list_players *lp = list_players__empty();
    list_players__initialize(lp, a, 2);
    if (list_players__next(NULL, lp) == &lp->players[0])
        passed("next of NULL is first player");
    else
        failed("next of NULL is first player");
    if (list_players__next(&lp->players[0], lp) == &lp->players[1] && list_players__next(&lp->players[1], lp) == &lp->players[0])
        passed("list_players_next");
    else
        failed("list_players_next");
    list_players__free(lp);
}


int main()
{
    printf("\n\033[1;33m =======TEST LIST_PLAYERS=======\033[0m\n");
    test_initialize();
    test_list_numb();
    test_next();
    printf("%d / %d test passed\n", test_numb, total_test);
    return 0;
}
