#include "main.h"

enum Color color = BLACK;
enum Rule rule = RULE_GO;
bool is_ai = false;

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

enum Color flip(enum Color c)
{
    return c == BLACK ? WHITE : BLACK;
}

bool bounded(int x, int y)
{
	return x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT;
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
    if (argc >= 2) {
        // 启用五子棋规则
        if (strcmp(argv[1], "-5") == 0) {
            rule = RULE_GOMOKU;
        }
    }
    if (argc >= 3) {
        // 启用 AI
        if (strcmp(argv[2], "-ai") == 0) {
            is_ai = true;
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
