#include <ncurses.h>
#include <locale.h>		// setlocale()
#include <ctype.h>		// isspace()
#include <time.h>		// time()

#define MAX_LINES 50
#define MAX_COLS 300
char article[MAX_LINES][MAX_COLS];
int article_lines = 0;
int eol[MAX_LINES] = {0};
int len[MAX_LINES];

void init()
{
	setlocale(LC_ALL, "");	// 使用系统 locale
	initscr();				// 初始化 ncurses
	noecho();				// 不回显
	cbreak();				// 立即获得输入 (除了 ctrl-c, ctrl-z)
	keypad(stdscr, TRUE);	// 处理功能键
	start_color();			// 启用颜色
	init_pair(1, COLOR_RED, COLOR_BLACK);
}

int load_article(char *filename)
{
	FILE *fin = fopen(filename, "r");
	if (!fin) {
		endwin();
		printf("cannot open file '%s'. does this file exist?\n", filename);
		return 0;
	}
	int c;
	int i = 0;
	attron(A_DIM);
	while ((c = fgetc(fin)) != EOF) {
		if (c == '\n') {
			article[article_lines][i] = '\0';
			len[article_lines++] = i;
			i = 0;
		} else {
			article[article_lines][i++] = c;
		}
		addch(c);
	}
	attroff(A_DIM);
	move(0, 0);
	fclose(fin);
	return 1;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf(
			"Typing practice. Type your favorite article!\n"
			"usage: typing [filename]\n"
		);
		return 0;
	}

	init();

	if (!load_article(argv[1]))
		return 1;

	int y = 0, byte = 0;
	int correct_cnt = 0, error_cnt = 0, backspace_cnt = 0;
	int c = getch();
	time_t timer = time(NULL);
	while (c != 4) { // ctrl-d to exit
		switch (c) {
		case '\n':
			if (article[y][byte] == '\0') {
				getyx(stdscr, y, eol[y]);
				byte = 0;
				move(++y, 0);
				if (y == article_lines) {
					c = 4;	// exit
					continue;
				}
			}
			break;
		case KEY_BACKSPACE:
		case KEY_LEFT:
		case KEY_RIGHT:
			break;

		default:
			if (article[y][byte] == 0) {
				;
			} else if (c == article[y][byte]) { // 英文正确
				addch(c);
				++correct_cnt;
				++byte;
			} else if (c >= 128 && article[y][byte] < 0
					&& article[y][byte+1] < 0
					&& article[y][byte+2] < 0) { // 中文
				int c1 = c;
				int c2 = getch();
				int c3 = getch();
				if ((char)c1 == article[y][byte]
						&& (char)c2 == article[y][byte+1]
						&& (char)c3 == article[y][byte+2]) {
					addch(c1); addch(c2); addch(c3);
					++correct_cnt;
				} else {
					attron(COLOR_PAIR(1));
					addch(c1); addch(c2); addch(c3);
					attroff(COLOR_PAIR(1));
					++error_cnt;
				}
				byte += 3;
			} else if (c >= 128) {		// 误在英文处输入中文
				attron(COLOR_PAIR(1));
				do {
					addch(c);
					c = getch();
				} while (c >= 128);
				attroff(COLOR_PAIR(1));
				byte += 2;
				++error_cnt;
				continue;
			} else {					// 英文错误
				if (isspace(c))
					attron(A_UNDERLINE);
				attron(COLOR_PAIR(1));
				addch(c);
				attroff(A_UNDERLINE | COLOR_PAIR(1));
				++error_cnt;
				++byte;
				if (article[y][byte] < 0) {
					addch(' ');
					byte += 2;
				}
			}
		}
		c = getch();
	}
	int s = time(NULL) - timer;
	int mm = s / 60;
	int ss = s % 60;
	int typed_cnt = correct_cnt + error_cnt;

	printw( "\n--------------------\n"
			"time:      %dm %ds\n"
			"speed:     %.2fkpm\n"
			"accuracy:  %.2f%%\n"
			"typed:     %d\n"
			"correct:   %d\n"
			"backspace: %d",
		mm, ss, 60.0*typed_cnt/s, 100.0*correct_cnt/typed_cnt,
		typed_cnt, correct_cnt,
		backspace_cnt);
	getch();
	endwin();
	return 0;
}
