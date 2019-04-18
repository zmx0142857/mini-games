#include <locale.h>		// setlocale()
#include <ctype.h>		// isspace()
#include <time.h>		// time()
#include "../game.h"

#define MAX_LINES 1000
#define MAX_COLS 500
struct {
	char text[MAX_LINES][MAX_COLS];
	int lines;					// in byte
	int cols[MAX_LINES];		// in byte, useful when undoing
	int x[MAX_LINES];			// of display
} article;


// correct 1~2, error 1~4, carrage return
enum Record {C1, C2, E1, E2, E3, E4, CR};
struct {
	enum Record stack[MAX_LINES*MAX_COLS];
	int top;
} undo;

struct {
	int correct;
	int error;
	int backspace;
} cnt;

int load_article(char *filename)
{
	FILE *fin = fopen(filename, "r");
	if (!fin) {
		fprintf(stderr, "error: cannot open file '%s'. "
				"does this file exist?\n", filename);
		return 1;
	}

	article.lines = 0;
	int c;
	int i = 0;
	int sum = 0;
	while ((c = fgetc(fin)) != EOF) {
		if (article.lines == MAX_LINES) {
			fprintf(stderr, "error: max lines (%d) reached. "
					"abort.\n", MAX_LINES);
			fclose(fin);
			return 2;
		}
		if (c == '\n') {
			article.text[article.lines][i] = '\0';
			article.cols[article.lines] = i;
			++article.lines;
			i = 0;
		} else {
			if (i == MAX_COLS) {
				fprintf(stderr, "\nerror: max cols (%d) reached. "
						"abort.\n", MAX_COLS);
				fclose(fin);
				return 3;
			}
			article.text[article.lines][i++] = c;
			if (c < 128) { // 英文
				article.x[article.lines] += 1;
			} else if (++sum == 3) { // 中文
				article.x[article.lines] += 2;
				sum = 0;
			}
		}
	}
	fclose(fin);

	// no eol
	if (i != 0) {
		article.text[article.lines][i] = '\0';
		article.cols[article.lines++] = i;
	}
	return 0;
}

void print_article(int line)
{
	static int i = 0;
	if (i < line + 4) { // load on reaching last 3 lines
		get_ttysize();
		int j = article.x[i] / tty.ws_col + 1;
		printf(STYLE_DIM);
		if (i == 0) { // first call
			while (i < article.lines && j < tty.ws_row) {
				puts(article.text[i++]);
				j += article.x[i] / tty.ws_col + 1;
			}
			cursor_up(j - article.x[i]/tty.ws_col - 1);
		} else {
			int k = 0;
			while (k < 3)
				cursor_down(article.x[line+(k++)]/tty.ws_col+1);
			puts(article.text[i++]);
			cursor_up(j);
			while (k)
				cursor_up(article.x[line+(k--)]/tty.ws_col+1);
		}
		printf(STYLE_RESET);
	}
}

void play(int c)
{
	int line = 0, col = 0;
	//int x = 0; // on display, useful for undoing
	cnt.correct = cnt.error = cnt.backspace = 0;
	undo.top = 0;
	while (c != EOF) { // ctrl-d to exit
		if (c == 127) { // backspace
			if (undo.top > 0) {
			enum Record record = undo.stack[--undo.top];
			switch (record) {
				case C1: case E1:
					if (record == C1)
						--cnt.correct;
					else
						--cnt.error;
					--col;
					//--x;
					cursor_left(1);
					printf(STYLE_DIM);
					putchar(article.text[line][col]);
					printf(STYLE_RESET);
					cursor_left(1);
					break;
				case C2: case E2:
					if (record == C2)
						--cnt.correct;
					else
						--cnt.error;
					col -= 3;
					//x -= 2;
					cursor_left(2);
					printf(STYLE_DIM);
					putchar((int)article.text[line][col]);
					putchar((int)article.text[line][col+1]);
					putchar((int)article.text[line][col+2]);
					printf(STYLE_RESET);
					cursor_left(2);
					break;
				case E3: // 误在英文处输入中文
					--cnt.error;
					col -= 2;
					//x -= 2;
					cursor_left(2);
					printf(STYLE_DIM);
					putchar(article.text[line][col]);
					putchar(article.text[line][col+1]);
					printf(STYLE_RESET);
					cursor_left(2);
					break;
				case E4: // 误在中文处输入英文
					--cnt.error;
					col -= 3;
					//x -= 2;
					cursor_left(2);
					printf(STYLE_DIM);
					putchar((int)article.text[line][col]);
					putchar((int)article.text[line][col+1]);
					putchar((int)article.text[line][col+2]);
					printf(STYLE_RESET);
					cursor_left(2);
					break;
				case CR:
					get_ttysize();
					col = article.cols[--line];
					//x = article.x[line];
					cursor_up(1);
					cursor_right(article.x[line] % tty.ws_col);
					break;
				default:
					break;
			}
			}
		} else if (c == '\r') { // carrage return
			if (article.text[line][col] == '\0') {
				undo.stack[undo.top++] = CR;
				++line;
				col = 0;
				//x = 0;
				putchar('\n');
				print_article(line);
				if (line == article.lines) {
					c = EOF;	// exit
					continue;
				}
			}
		} else if (article.text[line][col] == '\0') {
			;
		} else if (c == article.text[line][col]) { // 英文正确
			undo.stack[undo.top++] = C1;
			putchar(c);
			++cnt.correct;
			++col;
			//++x;
		} else if (c >= 128 && article.text[line][col] < 0
				&& article.text[line][col+1] < 0
				&& article.text[line][col+2] < 0) { // 中文
			int c1 = c;
			int c2 = getchar();
			int c3 = getchar();
			if ((char)c1 == article.text[line][col]
			 && (char)c2 == article.text[line][col+1]
			 && (char)c3 == article.text[line][col+2]) {
				undo.stack[undo.top++] = C2;
				putchar(c1); putchar(c2); putchar(c3);
				++cnt.correct;
			} else {
				undo.stack[undo.top++] = E2;
				printf(COLOR(RED));
				putchar(c1); putchar(c2); putchar(c3);
				printf(STYLE_RESET);
				++cnt.error;
			}
			col += 3;
			//x += 2;
		} else if (c >= 128) {		// 误在英文处输入中文
			undo.stack[undo.top++] = E3;
			printf(COLOR(RED));
			do {
				putchar(c);
				c = getchar();
			} while (c >= 128);
			printf(STYLE_RESET);
			++cnt.error;
			col += 2;
			//x += 2;
			continue;
		} else {					// 英文错误
			if (isspace(c))
				printf(STYLE_UNDERLINE);
			printf(COLOR(RED));
			putchar(c);
			printf(STYLE_RESET);
			++cnt.error;
			if (article.text[line][col] < 0) { // 误在中文处输入英文
				undo.stack[undo.top++] = E4;
				putchar(' ');
				col += 3;
				//x += 2;
			} else {
				undo.stack[undo.top++] = E1;
				++col;
				//++x;
			}
		}
		c = getchar();
	}
	article.lines -= line;
}

void summary(time_t begin)
{
	int s = time(NULL) - begin;
	int mm = s / 60;
	int ss = s % 60;
	int typed = cnt.correct + cnt.error;

	if (article.lines > 0)
		cursor_down(article.lines);
	printf( "\r--------------------\n"
			"time:      %dm %ds\n"
			"speed:     %.2fkpm\n"
			"accuracy:  %.2f%%\n"
			"typed:     %d\n"
			"correct:   %d\n"
			"backspace: %d\n",
		mm, ss, 60.0*typed/s, 100.0*cnt.correct/typed,
		typed, cnt.correct,
		cnt.backspace);
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

	setlocale(LC_ALL, "");		// 使用系统 locale
	int err = load_article(argv[1]);
	if (err)
		return err;
	screen_clear();
	print_article(0);
	toggle_flush();

	int c = getchar();
	time_t begin = time(NULL);
	play(c);
	summary(begin);

	toggle_flush();
	return 0;
}

