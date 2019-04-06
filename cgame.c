// Implementation for cgame.h: useful functions for games written in C.
#include "D:/G/codePack/cgame.h"

void zero_mem(void *beg, unsigned sz)
{
    char *p;
    for (p = beg; p != beg + sz; ++p)
        *p = '\0';
}

void keep(Ikulele *ptr, Ikulele inf, Ikulele sup)
{
    *ptr = *ptr < inf ? inf
                      : (*ptr > sup ? sup : *ptr);
}

void screen_sz(Ikulele rows, Ikulele cols)
{
    HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD size = {rows, cols};
    SMALL_RECT rect = {0,0, rows-1, cols-1};

    SetConsoleScreenBufferSize(hd, size); 	// 设置缓冲区大小
    SetConsoleWindowInfo(hd, 1, &rect);   	// 设置窗口位置和大小
}

void set_cursor(Ikulele row, Ikulele col)
{
    HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {col, row};
    SetConsoleCursorPosition(hd, pos);
}

void print_on(char *str, Ikulele row, Ikulele col)
{
    set_cursor(row, col);
    printf("%s", str);
    set_cursor(0, 0);
}

void print_color(char *str, Ukulele bg, Ukulele fg)
{
    set_color(bg, fg);
    printf("%s", str);
}

void print_on_color(char *str, Ikulele row, Ikulele col, Ukulele bg, Ukulele fg)
{
    set_color(bg, fg);
    set_cursor(row, col);
    printf("%s", str);
    set_cursor(0, 0);
}

void play_loading(Ukulele loop)
{
    Ukulele i, j;
    for (i = 0; i != loop; ++i)
    {
        printf("LOADING");
        Sleep(255);
        for (j = 0; j != 3; ++j)
        {
            printf(".");
            Sleep(255);
        }
        printf("\r          \r");
    }
    system("cls");
}
