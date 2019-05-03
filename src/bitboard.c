#include "bitboard.h"



struct bitboard *bitboard_empty(size_t size)
{
    struct bitboard *e = malloc(sizeof(struct bitboard));
    e->bitboard = 0;
    e->size = size;
    return e;
}



struct bitboard *bitboard_create(size_t size,unsigned __int128 bitboard)
{
    struct bitboard *e = malloc(sizeof(struct bitboard));
    e->bitboard = bitboard;
    e->size = size;
    return e;
}



void bitboard_set(struct bitboard *be, size_t row, size_t col)
{
    unsigned __int128 pos = 1;
    pos = pos << (row * be->size + col); // move the first bit to the position calculated by (row*size+col)
    be->bitboard = UNION(be->bitboard, pos);
}



char bitboard_get(struct bitboard *be, size_t row, size_t col)
{
    unsigned __int128 pos = 1;
    pos = pos << (row * be->size + col); // move the first bit to the position calculated by (row*size+col)
    return (INTER(be->bitboard, pos) != 0);
}



void bitboard_remove(struct bitboard *be, size_t row, size_t col)
{
    unsigned __int128 pos = 1;
    pos = pos << (row * be->size + col); // move the first bit to the position calculated by (row*size+col)
    be->bitboard = SYM_DIFF(be->bitboard, pos);
}



int bitboard_big_row_line(struct bitboard *be, size_t row, size_t col, struct col_move_t *mv1, struct col_move_t *mv2)
{
    size_t s = be->size;
    size_t count = 0;
    unsigned __int128 pos = 1;
    pos = pos << (row * s + col);
    for(size_t i = col; (i < s) && (be->bitboard & pos); i++)
    {
        count++;
        pos = SHIFT_E(pos, 1);
    }
    mv1->m.row = row;
    mv1->m.col = col + count;
    pos = 1;
    pos = pos << (row * s + col);
    for(size_t i = col; (i < s) && (be->bitboard & pos); i--)
    {
        count++;
        pos = SHIFT_W(pos, 1);
    }
    mv2->m.row = row;
    mv2->m.col = mv1->m.col - count;
    return count - 1;
}


int bitboard_big_col_line(struct bitboard *be, size_t row, size_t col, struct col_move_t *mv1, struct col_move_t *mv2)
{
    size_t s = be->size;
    size_t count = 0;
    unsigned __int128 pos = 1;
    pos = pos << (row * s + col);
    for(size_t i = row; (i < s) && (be->bitboard & pos); i++)
    {
        count++;
        pos = SHIFT_S(pos, 1, s);
    }
    mv1->m.row = row + count;
    mv1->m.col = col;
    pos = 1;
    pos = pos << (row * s + col);
    for(size_t i = row; (i < s) && (be->bitboard & pos); i--)
    {
        count++;
        pos = SHIFT_N(pos, 1, s);
    }
    mv2->m.row = mv1->m.row - count;
    mv2->m.col = col;
    return count - 1;
}

void print_128(unsigned __int128 u)
{
    for(int i = 0; i < 128 ; i++)
    {
        printf("%d",(int)u%2);
        u = u >> 1;
    }
    printf("\n");
}

int bitboard_big_diagr_line(struct bitboard *be, size_t row, size_t col, struct col_move_t *mv1, struct col_move_t *mv2)
{
    size_t s = be->size;
    size_t count = 0;
    unsigned __int128 pos = 1;
    pos = pos << (row * s + col);
    for(size_t i = row, j = col; (i < s) && (j < s) && (be->bitboard & pos); i++ , j--)
    {
        count++;
        pos = SHIFT_SW(pos, 1, s);
    }
    mv1->m.row = row + count;
    mv1->m.col = col - count;
    pos = 1;
    pos = pos << (row * s + col);
    for(size_t i = row, j = col; (i < s) && (j < s) && (be->bitboard & pos); i-- , j++)
    {
        count++;
        pos = SHIFT_NE(pos, 1, s);
    }
    mv2->m.row = mv1->m.row - count;
    mv2->m.col = mv1->m.col + count;
    return count - 1;
}



int bitboard_big_diagl_line(struct bitboard *be, size_t row, size_t col, struct col_move_t *mv1, struct col_move_t *mv2)
{
    size_t s = be->size;
    size_t count = 0;
    unsigned __int128 pos = 1;
    pos = pos << (row * s + col);
    for(size_t i = row, j = col; (i < s) && (j < s) && (be->bitboard & pos); i++ , j++)
    {
        count++;
        pos = SHIFT_SE(pos, 1, s);
    }
    mv1->m.row = row + count;
    mv1->m.col = col + count;
    pos = 1;
    pos = pos << (row * s + col);
    for(size_t i = row, j = col; (i < s) && (j < s) && (be->bitboard & pos); i-- , j--)
    {
        count++;
        pos = SHIFT_NW(pos, 1, s);
    }
    mv2->m.row = mv1->m.row - count;
    mv2->m.col = mv1->m.col - count;
    return count - 1;
}


int bitboard_x_row_h(struct bitboard *be, size_t row, size_t col, size_t x, struct col_move_t *mv1, struct col_move_t *mv2, struct col_move_t *hole)
{

    mv1->m.row = row;
    mv2->m.row = row;
    hole->m.row = row;

    size_t start = col > x - 1 ? col - x : 0;
    for (size_t i = start; i <= col; i++)
    {
        hole->m.col = be->size;
        size_t tmp_count = 0;
        for (size_t j = i; j < i + x + 1 && j < be->size; j++)
        {
            if (bitboard_get(be, row, j))
                tmp_count++;
            else
                hole->m.col = j;
            // printf(" j = %ld  || bit = %d  || ========= %ld   %ld \n",j,bitboard_get(be, row, j),hole->m.row,hole->m.col);
        }
        if (tmp_count >= x)
        {
            mv1->m.col = (hole->m.col < i + x) ? i + x + 1 : i + x;
            mv2->m.col = bitboard_get(be, row, i) ? i - 1 : i;
            // printf("========= %ld   %ld \n",mv2->m.row,mv2->m.col);
            if (hole->m.col < i + x && hole->m.col > i)
                return 1;
        }
    }
    return 0;
}



int bitboard_x_col_h(struct bitboard *be, size_t row, size_t col, size_t x, struct col_move_t *mv1, struct col_move_t *mv2, struct col_move_t *hole)
{

    mv1->m.col = col;
    mv2->m.col = col;
    hole->m.col = col;

    size_t start = row > x - 1 ? row - x : 0;
    for (size_t i = start; i <= row; i++)
    {
        hole->m.row = be->size;
        size_t tmp_count = 0;
        for (size_t j = i; j < i + x + 1 && j < be->size; j++)
        {
            if (bitboard_get(be, j, col))
                tmp_count++;
            else
                hole->m.row = j;
        }
        if (tmp_count >= x)
        {
            mv1->m.row = (hole->m.row < i + x) ? i + x + 1 : i + x;
            mv2->m.row = bitboard_get(be, i, col) ? i - 1 : i;
            if (hole->m.row < i + x && hole->m.row > i)
                return 1;
        }
    }
    return 0;
}



int bitboard_x_diagr_h(struct bitboard *be, size_t row, size_t col, size_t x, struct col_move_t *mv1, struct col_move_t *mv2, struct col_move_t *hole)
{

    size_t m = min(row, be->size - col - 1);
    size_t start;
    size_t col_i;
    if (m > x - 1)
    {
        start = row - x;
        col_i = col + x;
    }
    else
    {
        start = row - m;
        col_i = col + m;
    }
    for (size_t i = start; i <= row && col_i >= col; i++)
    {
        hole->m.col = be->size;
        hole->m.row = be->size;
        size_t tmp_count = 0;
        size_t col_j = col_i;
        for (size_t j = i; j < i + x + 1 && j < be->size && col_j < be->size; j++)
        {
            if (bitboard_get(be, j, col_j))
                tmp_count++;
            else
            {
                hole->m.row = j;
                hole->m.col = col_j;
            }
            col_j--;
        }
        if (tmp_count >= x)
        {
            mv1->m.row = (hole->m.row < i + x) ? i + x + 1 : i + x;
            mv1->m.col = (hole->m.row < i + x) ? col_i - x - 1 : col_i - x;
            mv2->m.row = bitboard_get(be, i, col_i) ? i - 1 : i;
            mv2->m.col = bitboard_get(be, i, col_i) ? col_i + 1 : col_i;
            if (hole->m.row < i + x && hole->m.row > i && hole->m.col > col_i - x && hole->m.col < col_i)
                return 1;
        }
        col_i--;
    }
    return 0;
}



int bitboard_x_diagl_h(struct bitboard *be, size_t row, size_t col, size_t x, struct col_move_t *mv1, struct col_move_t *mv2, struct col_move_t *hole)
{
    size_t m = min(row, col);
    size_t start;
    size_t col_i;
    if (m > x - 1)
    {
        start = row - x;
        col_i = col - x;
    }
    else
    {
        start = row - m;
        col_i = col - m;
    }
    for (size_t i = start; i <= row && col_i <= col; i++)
    {
        hole->m.col = be->size;
        hole->m.row = be->size;
        size_t tmp_count = 0;
        size_t col_j = col_i;
        for (size_t j = i; j < i + x + 1 && j < be->size && col_j < be->size; j++)
        {
            if (bitboard_get(be, j, col_j))
                tmp_count++;
            else
            {
                hole->m.row = j;
                hole->m.col = col_j;
            }
            col_j++;
        }
        if (tmp_count >= x)
        {
            mv1->m.row = (hole->m.row < i + x) ? i + x + 1 : i + x;
            mv1->m.col = (hole->m.row < i + x) ? col_i + x + 1 : col_i + x;
            mv2->m.row = bitboard_get(be, i, col_i) ? i - 1 : i;
            mv2->m.col = bitboard_get(be, i, col_i) ? col_i - 1 : col_i;
            if (hole->m.row < i + x && hole->m.row > i && hole->m.col < col_i + x && hole->m.col > col_i)
                return 1;
        }
        col_i++;
    }
    return 0;
}



void bitboard_free(struct bitboard *be)
{
    free(be);
}
