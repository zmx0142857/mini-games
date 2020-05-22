#include "game.h"
#include "string.h"

#define STAR "◎" // @ ╋ ⍿ ⌖ ·
#define WIDTH 19
#define HEIGHT 19
#define SIZE (WIDTH*HEIGHT)
#define BLANK 0
#define BLACK 1
#define WHITE 2
#define BOARD_Y (HEIGHT-y-1)
#define BOARD_X (2*(x+1))
#define MSG_LINE (HEIGHT+1)
#define MSG(args...)\
	do { mvprint(MSG_LINE, 0, args); line_clear(); } while (0)

char board[WIDTH][HEIGHT];
char color = BLACK;
char visited[WIDTH][HEIGHT];
int qx[SIZE], qy[SIZE]; // 理论上循环队列空间需要加一, 但这里不可能用尽
int qfront = 0, qrear = 0;
int sx[SIZE], sy[SIZE];
int top = 0;

void print_board()
{
	int lineno = HEIGHT;
	printf("%2d┌", lineno--);
	for (int j = 2; j < WIDTH; ++j)
		printf("─┬");
	puts("─┐");
	for (int i = 2; i < HEIGHT; ++i) {
		printf("%2d├", lineno--);
		for (int j = 2; j < WIDTH; ++j)
			printf("─┼");
		puts("─┤");
	}
	printf("%2d└", lineno--);
	for (int j = 2; j < WIDTH; ++j)
		printf("─┴");
	puts("─┘");
	printf(" ");
	for (int j = 0; j < WIDTH; ++j)
		printf("%2c", j + 'A');
	putchar('\n');

	int star[] = {3, 9, 15};
	int x, y;
	for (int i = 0; i < 3; ++i) {
		y = star[i];
		for (int j = 0; j < 3; ++j) {
			x = star[j];
			mvprint(BOARD_Y, BOARD_X, STAR);
		}
	}
}

void set_board(int x, int y, int color)
{
	board[x][y] = color;
	if (color == BLANK) {
		cursor_goto(BOARD_Y, BOARD_X);
		if (x == 0) {
			if (y == 0)
				printf("└");
			else if (y == HEIGHT-1)
				printf("┌");
			else
				printf("├");
		} else if (x == WIDTH-1) {
			if (y == 0)
				printf("┘");
			else if (y == HEIGHT-1)
				printf("┐");
			else
				printf("┤");
		} else {
			if (y == 0)
				printf("┴");
			else if (y == HEIGHT-1)
				printf("┬");
			else if (x % 6 == 3 && y % 6 == 3)
				printf(STAR);
			else
				printf("┼");
		}
	} else {
		mvprint(BOARD_Y, BOARD_X,
				color == BLACK ? SYM_CIRCLE_BLACK : SYM_CIRCLE_WHITE);
	}
}

char flip(char c)
{
	return !(c-1) + 1;
}

bool bounded(int x, int y)
{
	return x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT;
}

// 判断 (x,y) 所在的连通分量是否被围死
int isdead(int x, int y)
{
	top = 0;
	if (!bounded(x, y) || board[x][y] != color)
		return 0;
	int dx[] = {0, 0, -1, 1};
	int dy[] = {1, -1, 0, 0};

	// 广度优先
	memset(visited, false, sizeof(visited));
	qfront = qrear = 0;
	qx[qrear] = x;
	qy[qrear] = y;
	qrear = (qrear+1) % SIZE;
	visited[x][y] = true;
	while (qfront != qrear) {
		x = qx[qfront];
		y = qy[qfront];
		qfront = (qfront+1) % SIZE;

		// 要移除的棋子入栈
		sx[top] = x;
		sy[top++] = y;

		for (int i = 0; i < 4; ++i) {
			x += dx[i];
			y += dy[i];
			if (bounded(x, y) && !visited[x][y]) {
				if (board[x][y] == color) {
					qx[qrear] = x;
					qy[qrear] = y;
					qrear = (qrear+1) % SIZE;
					visited[x][y] = true;
				} else if (board[x][y] == BLANK) {
					return 0;
				}
			}
			x -= dx[i];
			y -= dy[i];
		}
	}
	return top;
}

// 吃掉含 (x,y) 的一个连通分量
int do_eat(int x, int y)
{
	int ret = top = isdead(x, y);
	while (top--) {
		x = sx[top];
		y = sy[top];
		set_board(x, y, BLANK);
	}
	return ret;
}

// 落子于 (x,y), 返回吃子数
int eat(int x, int y)
{
	color = flip(color);
	int ret = do_eat(x, y+1) + do_eat(x, y-1) + do_eat(x-1, y) + do_eat(x+1, y);
	if (ret)
		MSG("%d eaten", ret);
	color = flip(color);
	return ret;
}

// 试图在 (x,y) 落子
void move(int x, int y)
{
	if (!bounded(x, y) || board[x][y]) {
		MSG("invalid move");
	} else {
		board[x][y] = color; // try
		int flag_dead = isdead(x, y);
		int flag_eat = eat(x, y);
		if (flag_dead && !flag_eat) {
			board[x][y] = BLANK; // restore
			MSG("dead position");
		} else {
			set_board(x, y, color);
			color = flip(color);
		}
	}
}

void play()
{
	memset(board, BLANK, sizeof(board));
	print_board();
	int x, y;
	while (
		cursor_goto(MSG_LINE+1, 0),
		line_clear(),
		scanf("%d %d", &x, &y) == 2
	) {
		move(x-1, y-1);
	}
}

int main()
{
	screen_size(HEIGHT + 3, WIDTH*2 + 10);
	bool is_game_over = false;
    while (!is_game_over) {
		play();
		is_game_over = true;
    }
	return 0;
}
