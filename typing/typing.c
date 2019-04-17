#include <stdio.h>
#include <termios.h>	// termios()
#include <locale.h>		// setlocale()
#include <ctype.h>		// isspace()
#include <time.h>		// time()
#include "../game.h"

#define MAX_LINES 1000
#define MAX_COLS 500
struct {
	char text[MAX_LINES][MAX_COLS];
	int lines;
	int col[MAX_LINES];
	int len[MAX_LINES];
} article;

struct {
	int correct;
	int error;
	int backspace;
} cnt;

void toggle_flush()
{
	static struct termios on, off;
	static int ready = 0;
	if (!ready) {
		tcgetattr(0, &on);
		off = on;
		off.c_lflag &= ~ICANON;	// 禁用 I/O 缓冲
		off.c_lflag |= ~ECHO;	// 无回显
		ready = 1;
	}
	static int flush_on = 1;
	if (flush_on) {
		tcsetattr(0, TCSANOW, &off);
	} else {
		tcsetattr(0, TCSANOW, &on);
	}
	flush_on = !flush_on;
}

int load_article(char *filename)
{
	FILE *fin = fopen(filename, "r");
	if (!fin) {
		printf("error: cannot open file '%s'. does this file exist?\n", filename);
		return 1;
	}

	int c;
	article.lines = 0;
	int i = 0;
	printf(STYLE_DIM);
	while ((c = fgetc(fin)) != EOF) {
		if (article.lines == MAX_LINES) {
			printf("\nerror: max lines (%d) reached. abort.\n", MAX_LINES);
			fclose(fin);
			return 2;
		}
		if (c == '\n') {
			article.text[article.lines][i] = '\0';
			article.len[article.lines++] = i;
			i = 0;
		} else {
			if (i == MAX_COLS) {
				printf("\nerror: max cols (%d) reached. abort.\n", MAX_COLS);
				fclose(fin);
				return 3;
			}
			article.text[article.lines][i++] = c;
		}
		putchar(c);
	}

	// no eol
	if (i != 0) {
		article.text[article.lines][i] = '\0';
		article.len[article.lines++] = i;
		putchar('\n');
	}
	printf(STYLE_RESET);

	cursor_up(article.lines);
	fclose(fin);
	return 0;
}

void play(int c)
{
	int line = 0, len = 0, col = 0;
	cnt.correct = cnt.error = cnt.backspace = 0;
	while (c != EOF) { // ctrl-d to exit
		if (article.text[line][len] == '\0') {
			if (c == '\r') {
				++line;
				len = 0;
				col = 0;
				putchar('\n');
				if (line == article.lines) {
					c = EOF;	// exit
					continue;
				}
			}
		} else if (c == '\r' || c == 127) { // newline or backspace
			;
		} else if (c == article.text[line][len]) { // 英文正确
			putchar(c);
			++cnt.correct;
			++len;
			++col;
		} else if (c >= 128 && article.text[line][len] < 0
				&& article.text[line][len+1] < 0
				&& article.text[line][len+2] < 0) { // 中文
			int c1 = c;
			int c2 = getchar();
			int c3 = getchar();
			if ((char)c1 == article.text[line][len]
					&& (char)c2 == article.text[line][len+1]
					&& (char)c3 == article.text[line][len+2]) {
				putchar(c1); putchar(c2); putchar(c3);
				++cnt.correct;
			} else {
				printf(COLOR(RED));
				putchar(c1); putchar(c2); putchar(c3);
				printf(STYLE_RESET);
				++cnt.error;
			}
			len += 3;
			col += 2;
		} else if (c >= 128) {		// 误在英文处输入中文
			printf(COLOR(RED));
			do {
				putchar(c);
				c = getchar();
			} while (c >= 128);
			printf(STYLE_RESET);
			++cnt.error;
			len += 2;
			col += 2;
			continue;
		} else {					// 英文错误
			if (isspace(c))
				printf(STYLE_UNDERLINE);
			printf(COLOR(RED));
			putchar(c);
			printf(STYLE_RESET);
			++cnt.error;
			if (article.text[line][len] < 0) {
				putchar(' ');
				len += 3;
				col += 2;
			} else {
				++len;
				++col;
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
	toggle_flush();

	int c = getchar();
	time_t begin = time(NULL);
	play(c);
	summary(begin);

	toggle_flush();
	return 0;
}

