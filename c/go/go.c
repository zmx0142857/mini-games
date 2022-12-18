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
int qfront = 0, qrear = 0;
int sx[SIZE], sy[SIZE];
int top = 0;

enum Color { BLANK, BLACK, WHITE };
enum Color color = BLACK;
enum Rule { RULE_GO, RULE_GOMOKU }; // 围棋规则, 五子棋规则
enum Rule rule = RULE_GO;

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
    return c == BLACK ? WHITE : BLACK;
}

bool bounded(int x, int y)
{
	return x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT;
}

// 围棋: 判断 (x,y) 所在的连通分量是否被围死
int go_isdead(int x, int y)
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

// 围棋: 吃掉含 (x,y) 的一个连通分量
int go_do_eat(int x, int y)
{
	int ret = top = go_isdead(x, y);
	while (top--) {
		x = sx[top];
		y = sy[top];
		set_board(x, y, BLANK);
	}
	return ret;
}

// 围棋: 落子于 (x,y), 返回吃子数
int go_eat(int x, int y)
{
	color = flip(color);
	int ret
        = go_do_eat(x, y+1)
        + go_do_eat(x, y-1)
        + go_do_eat(x-1, y)
        + go_do_eat(x+1, y);
	if (ret)
		MSG("提子: %d", ret);
	color = flip(color);
	return ret;
}

// 五子棋: 检查各个方向, 若游戏结束, 返回 true
bool gomoku_check_directions(int x, int y) {
    int dx[8] = { 0, 1, 1, 1 };
    int dy[8] = { 1, 1, 0, -1 };
    for (int i = 0; i < 4; ++i) {
        int offset;
        int count = 1;
        // 沿正方向看
        for (offset = 1; offset < 5; ++offset) {
            int xx = x + dx[i] * offset;
            int yy = y + dy[i] * offset;
            if (!bounded(xx, yy) || board[xx][yy] != color) break;
            ++count;
        }
        // 沿反方向看
        for (offset = -1; offset > -5; --offset) {
            int xx = x + dx[i] * offset;
            int yy = y + dy[i] * offset;
            if (!bounded(xx, yy) || board[xx][yy] != color) break;
            ++count;
        }
        if (count >= 5) return true;
    }
    return false;
}

// 五子棋: 若游戏结束, 返回 true
bool rule_gomoku(int x, int y) {
    set_board(x, y, color);
    if (gomoku_check_directions(x, y)) {
        MSG(color == BLACK ? "黑胜" : "白胜");
        return true;
    }
    color = flip(color);
    return false;
}

// 围棋: 若游戏结束, 返回 true
bool rule_go(int x, int y) {
    board[x][y] = color; // try
    int flag_dead = go_isdead(x, y);
    int flag_eat = go_eat(x, y);
    if (flag_dead && !flag_eat) {
        board[x][y] = BLANK; // restore
        MSG("此处不可落子");
    } else {
        set_board(x, y, color);
        color = flip(color);
	}
    return false;
}

// 试图在 (x,y) 落子
// 若游戏结束, 返回 true
bool move(int x, int y)
{
    if (!bounded(x, y)) {
        MSG("超出棋盘范围");
        return false;
    }
	if (board[x][y]) {
        MSG("此处已有落子");
        return false;
    }
    if (rule == RULE_GO) {
        return rule_go(x, y);
    } else if (rule == RULE_GOMOKU) {
        return rule_gomoku(x, y);
    }
}

void play()
{
	memset(board, BLANK, sizeof(board));
	print_board();
    MSG("%s: %s",
        (rule == RULE_GO ? "围棋" : "五子棋"),
        "方向键或 WASD 移动, 空格落子"
    );
	int x = WIDTH / 2;
    int y = HEIGHT / 2;
    cursor_goto(BOARD_Y, BOARD_X);

    int key = getch();
    while (key != -1) {
        if (key >= 'A' && key <= 'Z') key += 32;
        key = arrow_key(key);
        switch (key) {
            case 'w': if (bounded(x, y+1)) ++y; break;
            case 's': if (bounded(x, y-1)) --y; break;
            case 'a': if (bounded(x-1, y)) --x; break;
            case 'd': if (bounded(x+1, y)) ++x; break;
            case ' ': if (move(x, y)) return; break;
        }
		// MSG("%c%d", x+'A', y+1);
        cursor_goto(BOARD_Y, BOARD_X);
        key = getch();
    }
}

int main(int argc, char **argv)
{
    if (argc == 2) {
        // 启用五子棋规则
        if (strcmp(argv[1], "-5") == 0) {
            rule = RULE_GOMOKU;
        }
    }

	screen_size(HEIGHT + 3, WIDTH*2 + 10);
	bool is_game_over = false;
    while (!is_game_over) {
		play();
		is_game_over = true;
    }
    cursor_goto(MSG_LINE+1, 0);
	return 0;
}
