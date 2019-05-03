#include "display.h"

char *colors_str[] = {"BLACK", "WHITE", "RED", "BLUE"};
static int line_delete = 8;

void delete_after(FILE *src_file, FILE *tmp_file, const int line)
{
    char buffer[150];
    int count = 1;

    while ((fgets(buffer, 150, src_file)) != NULL)
    {
        /* If current line is not the line user wanted to remove */
        if (line > count)
            fputs(buffer, tmp_file);
        else
            break;
        count++;
    }
}
void display_move(struct move_t m, enum color_t c)
{
    FILE *f;
    FILE *tmp;
    char *path = "display/data.json";
    f = fopen(path, "r");
    tmp = fopen("display/data.tmp", "w");
    if (f == NULL)
    {
        printf("Error opening file f!\n");
        exit(1);
    }
    if (tmp == NULL)
    {
        printf("Error opening file tmp!\n");
        exit(1);
    }

    delete_after(f, tmp, line_delete);
    fclose(f);
    fclose(tmp);
    remove(path);
    rename("display/data.tmp", path);
    f = fopen(path, "a");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(f, "},\n{\n\"row\" : \"%zd\",\n\"column\" : \"%zd\",\n\"color\" : \"%s\"\n}],\n\"state\" : \"1\"\n}", m.row, m.col, colors_str[c]);
    fclose(f);
    line_delete += 5;
    #ifdef SLW 
        getchar();
    #endif
}
void init_display(size_t size, struct col_move_t *opening, char open, char *black, char *white)
{
    FILE *f;
    f = fopen("display/data.json", "w");

    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(f, "{\n\"size\" : \"%zd\",\n\"black\" : \"%s\",\n\"white\" : \"%s\",\n\"moves\" : [", size, black, white);
    fprintf(f, "\n{\n\"row\" : \"-1\",\n\"column\" : \"-1\",\n\"color\" : \"NOT\"\n}");
    if (open)
    {
        for (int i = 0; i < 2; i++)
        {
            fprintf(f, ",\n{\n\"row\" : \"%zd\",\n\"column\" : \"%zd\",\n\"color\" : \"%s\"\n}", opening[i].m.row,
                    opening[i].m.col, colors_str[opening[i].c]);
        }

        fprintf(f, ",\n{\n\"row\" : \"%zd\",\n\"column\" : \"%zd\",\n\"color\" : \"%s\"\n}", opening[2].m.row, opening[2].m.col,
                colors_str[opening[2].c]);
        line_delete += 17;
    }
    else
        line_delete = 10;
    fprintf(f, "],\n\"state\" : \"0\"\n}");
    fclose(f);
}

void display_board(struct board_game bg)
{
    FILE *f;
    char *path = "display/dataBrd.json";

    f = fopen(path, "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(f, "{\"size\" : \"%zd\",\n",bg.size);
    fprintf(f, "\"moves\" : [\n");
    char w[]="WHITE",b[]="BLACK";
    struct col_move_t m;
    for(size_t i = 0; i < bg.size; i++)
    {
        for(size_t j = 0; j < bg.size; j++)
        {
            m.m.col = j;
            m.m.row = i;
            if(!board_game_legalm(&bg, m))
            {
                fprintf(f, "\n{\n\"row\" : \"%zd\",\n\"column\" : \"%zd\",\n\"color\" : \"%s\"\n},", m.m.row, m.m.col, (board_game_get_color(&bg,m.m) == 0)?b:w);
            }
        }
    }
    fprintf(f, "\n{\n\"row\" : \"-1\",\n\"column\" : \"-1\",\n\"color\" : \"NOT\"\n}]}");
    fclose(f);
}
void final_display(char win, char *state, struct col_move_t *m)
{
    FILE *f;
    FILE *tmp;
    char *path = "display/data.json";
    f = fopen(path, "r");
    tmp = fopen("display/data.tmp", "w");
    if (f == NULL || tmp == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    delete_after(f, tmp, line_delete);
    fclose(f);
    fclose(tmp);
    remove(path);
    rename("display/data.tmp", path);
    f = fopen(path, "a");
    fprintf(f, "}],\n\"state\" : \"%s\",\n", state);
    if (win == 1)
    {
        fprintf(f, "\"win_move\" : {\n\"row\" : \"%zd\",\n\"column\" : \"%zd\",\n\"color\" : \"%s\"\n}", m->m.row, m->m.col, colors_str[m->c]);
    }
    else if (win == 2)
    {
        fprintf(f, "\"stupid_move\" : {\n\"row\" : \"%zd\",\n\"column\" : \"%zd\",\n\"color\" : \"%s\"\n}", m->m.row, m->m.col, colors_str[m->c]);
    }
    else
    {
        fprintf(f, "\"last_move\" : {\n\"row\" : \"%zd\",\n\"column\" : \"%zd\",\n\"color\" : \"%s\"\n}", m->m.row, m->m.col, colors_str[m->c]);
    }
    fprintf(f, "}");
    fclose(f);
}