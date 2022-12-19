#ifndef MAIN_H
#define MAIN_H

#include "game.h"
#include <string.h>

#define STAR "◎" // @ ╋ ⍿ ⌖ ·
#define WIDTH 19
#define HEIGHT 19
#define SIZE (WIDTH*HEIGHT)
#define BOARD_Y (HEIGHT-y-1)
#define BOARD_X (2*(x+1))
#define MSG_LINE (HEIGHT+1)
#define MSG(args...)\
	do { mvprint(MSG_LINE, 0, args); line_clear(); } while (0)

char board[WIDTH][HEIGHT];
char visited[WIDTH][HEIGHT];
int qx[SIZE], qy[SIZE]; // 理论上循环队列空间需要加一, 但这里不可能用尽
int sx[SIZE], sy[SIZE];
enum Color { BLANK, BLACK, WHITE };
enum Rule { RULE_GO, RULE_GOMOKU }; // 围棋规则, 五子棋规则

enum Color color;
enum Rule rule;

bool bounded(int x, int y);
void set_board(int x, int y, int color);
enum Color flip(enum Color c);
bool rule_go(int x, int y);
bool rule_gomoku(int x, int y);

#endif // MAIN_H
