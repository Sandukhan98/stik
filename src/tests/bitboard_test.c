#include "bitboard.h"
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
void test_set_get()
{
    struct bitboard *be = bitboard_empty(10);
    bitboard_set(be, 5, 5);
    unsigned __int128 a = 0x80000000000000;
    if (INTER(be->bitboard, a) != 0)
        passed("set a bit in right position");
    else
        failed("set a bit in right position");
    if (bitboard_get(be, 5, 5))
        passed("get from a bit from the board");
    else
        failed("get from a bit from the board");
    bitboard_free(be);
}
void test_remove()
{
    struct bitboard *be = bitboard_empty(10);
    bitboard_set(be, 5, 5);
    bitboard_remove(be, 5, 5);
    if (be->bitboard == 0)
        passed("remove a bit from the board");
    else
        failed("remove a bit from the board");
    bitboard_free(be);
}
void test_big_row_line()
{
    struct bitboard *be = bitboard_empty(9);
    for (int i = 0; i < 3; i++)
    {
        bitboard_set(be, 2, 2 + i);
    }
    struct col_move_t m1;
    struct col_move_t m2;
    int n = bitboard_big_row_line(be, 2, 4, &m1, &m2);
    printf("%d ***",n);
    if (n == 3)
        passed("line of 3 pawns found in line of 1");
    else
        failed("line of 3 pawns found in line of 1");
    if (m1.m.row == 2 && m1.m.col == 5)
        passed("check for 3 in row return the right first limit");
    else
        failed("check for 3 in row return the right first limit");
    if (m2.m.row == 2 && m2.m.col == 1)
        passed("check for 3 in row return the right second limit");
    else
        failed("check for 3 in row return the right second limit");
    bitboard_free(be);
}

void test_big_col_line()
{
    struct bitboard *be = bitboard_empty(9);
    for (int i = 0; i < 4; i++)
    {
        bitboard_set(be, 2 + i, 2);
    }
    struct col_move_t m1;
    struct col_move_t m2;
    int n = bitboard_big_col_line(be, 4, 2, &m1, &m2);
    if (n == 4)
        passed("4 pawns in col are found in line of 4");
    else    
        failed("4 pawns in col are found in line of 4");
    if (m1.m.row == 6 && m1.m.col == 2)
        passed("check for 4 in col return the right first limit");
    else
        failed("check for 4 in col return the right first limit");
    if (m2.m.row == 1 && m2.m.col == 2)
        passed("check for 4 in col return the right second limit");
    else
        failed("check for 4 in col return the right second limit");
    bitboard_free(be);
}

void test_big_diagr_line()
{
    struct bitboard *be = bitboard_empty(9);
    for (int i = 0; i < 5; i++)
    {
        bitboard_set(be, 2 + i, 5 - i);
    }
    struct col_move_t m1;
    struct col_move_t m2;
    int n = bitboard_big_diagr_line(be, 3, 4, &m1, &m2);
    if (n == 5)
        passed("5 pawns in diagr are found in line of 5");
    else
        failed("5 pawns in diagr are found in line of 5");
    if (m1.m.row == 7 && m1.m.col == 0)
        passed("check for 5 in diagr return the right first limit");
    else
        failed("check for 5 in diagr return the right first limit");
    if (m2.m.row == 1 && m2.m.col == 6)
        passed("check for 5 in diagr return the right second limit");
    else
        failed("check for 5 in diagr return the right second limit");
    bitboard_free(be);
}

void test_big_diagl_line()
{
    struct bitboard *be = bitboard_empty(9);
    for (int i = 0; i < 2; i++)
    {
        bitboard_set(be, i, i);
    }
    struct col_move_t m1;
    struct col_move_t m2;
    int n = bitboard_big_diagl_line(be, 1, 1, &m1, &m2);
    if (n == 2)
        passed("2 pawns in diagr are found in line of 2");
    else
        failed("2 pawns in diagr are found in line of 2");
    if (m1.m.row == 2 && m1.m.col == 2)
        passed("check for 2 in diagl return the right first limit");
    else
        failed("check for 2 in diagl return the right first limit");
    if (m2.m.row > be->size && m2.m.col > be->size)
        passed("check for 2 in diagl return the right second limit");
    else
        failed("check for 2 in diagl return the right second limit");
    bitboard_free(be);
}

void test_hole_row()
{
    struct bitboard *be = bitboard_empty(10);
    for (int i = 0; i < 4; i++)
    {
        if (i != 2)
            bitboard_set(be, 5, 4 + i);
    }
    struct col_move_t m1;
    struct col_move_t m2;
    struct col_move_t hole;
    int n = bitboard_x_row_h(be, 5, 7, 3, &m1, &m2, &hole);
    // printf("== %ld   %ld \n",hole.m.row,hole.m.col);
    if (n != 0)
        passed("check hole row");
    else
        failed("check hole row");
    if (hole.m.col == 6)
        passed("right hole's position");
    else
        failed("right hole's position");
    if (m1.m.col == 8)
        passed("right bound 1");
    else
        failed("right bound 1");
    if (m2.m.col == 3)
        passed("right bound 2");
    else
        failed("right bound 2");
    bitboard_free(be);
}

void test_hole_col()
{
    struct bitboard *be = bitboard_empty(9);
    for (int i = 0; i < 5; i++)
    {
        if (i != 2)
            bitboard_set(be, 2 + i, 2);
    }
    struct col_move_t m1;
    struct col_move_t m2;
    struct col_move_t hole;
    int n = bitboard_x_col_h(be, 5, 2, 4, &m1, &m2, &hole);
    if (n != 0)
        passed("check hole col");
    else
        failed("check hole col");
    if (hole.m.row == 4)
        passed("right hole's position");
    else
        failed("right hole's position");
    if (m1.m.row == 7)
        passed("right bound 1");
    else
        failed("right bound 1");
    if (m2.m.row == 1)
        passed("right bound 2");
    else
        failed("right bound 2");
    bitboard_free(be);
}

void test_hole_diagr()
{
    struct bitboard *be = bitboard_empty(9);
    for (int i = 0; i < 5; i++)
    {
        if (i != 2)
            bitboard_set(be, 2 + i, 8 - i);
    }
    struct col_move_t m1;
    struct col_move_t m2;
    struct col_move_t hole;
    int n = bitboard_x_diagr_h(be, 5, 5, 4, &m1, &m2, &hole);
    if (n != 0)
        passed("check hole diagr");
    else
        failed("check hole diagr");
    if (hole.m.row == 4 && hole.m.col == 6)
        passed("right hole's position");
    else
        failed("right hole's position");
    if (m1.m.row == 7 && m1.m.col == 3)
        passed("right bound 1");
    else
        failed("right bound 1");
    if (m2.m.row == 1 && m2.m.col == 9)
        passed("right bound 2");
    else
        failed("right bound 2");
    bitboard_free(be);
}

void test_hole_diagl()
{
    struct bitboard *be = bitboard_empty(9);
    for (int i = 0; i < 5; i++)
    {
        if (i != 2)
            bitboard_set(be, 2 + i, 3 + i);
    }
    struct col_move_t m1;
    struct col_move_t m2;
    struct col_move_t hole;
    int n = bitboard_x_diagl_h(be, 5, 6, 4, &m1, &m2, &hole);
    if (n != 0)
        passed("check hole diagl");
    else
        failed("check hole diagl");
    if (hole.m.row == 4 && hole.m.col == 5)
        passed("right hole's position");
    else
        failed("right hole's position");
    if (m1.m.row == 7 && m1.m.col == 8)
        passed("right bound 1");
    else
        failed("right bound 1");
    if (m2.m.row == 1 && m2.m.col == 2)
        passed("right bound 2");
    else
        failed("right bound 2");
    bitboard_free(be);
}


int main()
{
    printf("\n\033[1;33m =======TEST bitboard=======\033[0m\n");
    test_set_get();
    test_remove();
    test_big_row_line();
    test_big_col_line();
    test_big_diagr_line();
    test_big_diagl_line();
    test_hole_row();
    test_hole_col();
    test_hole_diagr();
    test_hole_diagl();
    printf("%d / %d test passed\n", test_numb, total_test);
    return 0;
}