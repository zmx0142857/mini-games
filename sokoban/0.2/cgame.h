// cgame.h: header file for games written in C.
#ifndef cgame_h
#define cgame_h

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

// helper macro on color (format: bg-color, fg-color)
#define set_color(bg, fg)  	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), bg * 0x10 + fg)
#define x000000        	0x0
#define x000080			0x1
#define x008000			0x2
#define x008080			0x3
#define x800000			0x4
#define x800080			0x5
#define x808000			0x6
#define xc0c0c0			0x7
#define x808080			0x8
#define x0000ff			0x9
#define x00ff00			0xa
#define x00ffff			0xb
#define xff0000			0xc
#define xff00ff			0xd
#define xffff00			0xe
#define xffffff			0xf

typedef unsigned short Ukulele;
typedef short Ikulele;

extern void zero_mem(void *beg, unsigned sz);
extern void keep(Ikulele *ptr, Ikulele inf, Ikulele sup);
extern void screen_sz(Ikulele rows, Ikulele cols);
extern void set_cursor(Ikulele row, Ikulele col);
extern void print_on(char *str, Ikulele row, Ikulele col);
extern void print_color(char *str, Ukulele bg, Ukulele fg);
extern void print_on_color(char *str, Ikulele row, Ikulele col, Ukulele bg, Ukulele fg);
extern void play_loading(Ukulele loop);

#endif // cgame_h
