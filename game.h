/* header file for c games played on terminal */
#ifndef Game_h
#define Game_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>		// sleep()
#include <stdbool.h>	// bool
#include <signal.h>		// signal()
#include <fcntl.h>		// fcntl()

/******** windows ********/

#ifdef __MINGW32__

#include <conio.h>
#include <windows.h>

HANDLE cmd;
#define toggle_flush()

#define screen_clear()		system("cls")

#define set_color(fg)		SetConsoleTextAttribute(cmd, fg)
#define set_colors(fg, bg)	SetConsoleTextAttribute(cmd, (bg<<4)|fg)

#define BG_BLACK	0x0
#define BG_BLUE		0x1
#define BG_GREEN	0x2
#define BG_CYAN		0x3
#define BG_RED		0x4
#define BG_MAGENTA	0x5
#define BG_YELLOW	0x6
#define BG_WHITE	0x7
#define FG_BLACK	0x8
#define FG_BLUE		0x9
#define FG_GREEN	0xa
#define FG_CYAN		0xb
#define FG_RED		0xc
#define FG_MAGENTA	0xd
#define FG_YELLOW	0xe
#define FG_WHITE	0xf

/*
void screen_size(int y, int x)
{
    COORD size = {x, y};
    SetConsoleScreenBufferSize(cmd, size); 	// set buffer size
    SMALL_RECT rect = {0,0, x-1, y-1};
    SetConsoleWindowInfo(cmd, 1, &rect);   	// set window size
}
*/

void screen_size(unsigned height, unsigned width)
{
	char buf[100];
	sprintf(buf, "mode con lines=%d cols=%d", height, width);
	system(buf);
	screen_clear();
}

void cursor_hide()
{
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(cmd, &info);
}

#define cursor_show()

#define cursor_goto(y,x)\
	do {\
		COORD pos = {x, y};\
		SetConsoleCursorPosition(cmd, pos);\
	} while (0)

#define game_init()\
	do {\
		cmd = GetStdHandle(STD_OUTPUT_HANDLE);\
		signal(SIGINT, SIG_IGN);\
		system("chcp 65001");\
		cursor_hide();\
	} while (0)

#define game_over()\
	do {\
		system("chcp 936");\
		cursor_show();\
	} while (0)

#endif // __MINGW32__

/******** linux ********/

#ifdef __linux__

#include <sys/ioctl.h>	// winsize, ioctl()
#include <termios.h>	// termios

struct winsize tty;
#define get_ttysize()		ioctl(0, TIOCGWINSZ, &tty)

// enable/disable buffered I/O
void toggle_flush()
{
	static struct termios on, off;
	static int ready = 0;
	if (!ready) {
		tcgetattr(STDIN_FILENO, &on);
		off = on;
		off.c_lflag &= ~(ICANON | ECHO);	// 禁用 I/O 缓冲, 无回显
		ready = 1;
	}
	static int flush_on = 1;
	if (flush_on) {
		tcsetattr(STDIN_FILENO, TCSANOW, &off);
	} else {
		tcsetattr(STDIN_FILENO, TCSANOW, &on);
	}
	flush_on = !flush_on;
}

int getch()
{
	toggle_flush();
	int ret = getchar();
	toggle_flush();
	return ret;
}

int kbhit() {
	int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	int ch = getch();
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if (ch == EOF)
		return 0;
	ungetc(ch, stdin);
	return ch;
}

#define Sleep(ms)\
	do {\
		toggle_flush();\
		usleep((ms)*1000);\
		toggle_flush();\
	} while (0)

// POSIX escape sequences

#define STYLE_RESET			"\033[0m"
#define STYLE_BRIGHT		"\033[1m"
#define STYLE_DIM			"\033[2m"
#define STYLE_UNDERLINE		"\033[4m"
#define STYLE_BLINK			"\033[5m"
#define STYLE_REVERSE		"\033[7m"
#define STYLE_HIDDEN		"\033[8m"

#define FG_BLACK			"30"
#define FG_RED				"31"
#define FG_GREEN			"32"
#define FG_YELLOW			"33"
#define FG_BLUE				"34"
#define FG_MAGENTA			"35"
#define FG_CYAN				"36"
#define FG_WHITE			"37"
#define BG_BLACK			"40"
#define BG_RED				"41"
#define BG_GREEN			"42"
#define BG_YELLOW			"43"
#define BG_BLUE				"44"
#define BG_MAGENTA			"45"
#define BG_CYAN				"46"
#define BG_WHITE			"47"

#define COLOR(fg)			"\033[" fg "m"
#define COLORS(fg, bg)		"\033[" fg ";" bg "m"
#define set_color(fg)		printf("\033[%sm", (fg))
#define set_colors(fg, bg)	printf("\033[%s;%sm", (fg), (bg))

#define screen_clear()		printf("\033[2J\033[1;1H")
#define line_clear()		printf("\033[K")

#define cursor_goto(y,x)	printf("\033[%d;%dH", (y)+1, (x)+1)
#define cursor_cr()			pritnf("\r")
#define cursor_up(n)		printf("\033[%dA", (n))
#define cursor_down(n)		printf("\033[%dB", (n))
#define cursor_right(n)		printf("\033[%dC", (n))
#define cursor_left(n)		printf("\033[%dD", (n))
#define cursor_hide()		printf("\033[?25l");
#define cursor_show()		printf("\033[?25h");

void screen_size(unsigned height, unsigned width)
{
	char buf[100];
	sprintf(buf, "resize -s %d %d", height, width);
	system(buf);
	screen_clear();
}

#define game_init()\
	do {\
		signal(SIGINT, SIG_IGN);\
		cursor_hide();\
	} while (0)

#define game_over()			cursor_show()

#endif // __linux__

#define mvprint(y, x, args...)\
	do {\
		cursor_goto(y, x);\
		printf(args);\
	} while (0)

#define color_print(fg, args...)\
	do {\
		set_color(fg);\
		printf(args);\
	} while (0)

#define colors_print(fg, bg, args...)\
	do {\
		set_colors(fg, bg);\
		printf(args);\
	} while (0)

#define color_mvprint(fg, y, x, args...)\
	do{\
		set_color(fg);\
		mvprint(y, x, args);\
	} while (0)

#define colors_mvprint(fg, bg, y, x, args...)\
	do{\
		set_colors(fg, bg);\
		mvprint(y, x, args);\
	} while (0)

#define keep_between(value, inf, sup)\
		  (value < inf ? (value = inf)\
		: (value > sup ? (value = sup)\
		: value))

void play_loading(unsigned loop)
{
    unsigned i, j;
    for (i = 0; i != loop; ++i) {
        printf("LOADING");
        Sleep(255);
        for (j = 0; j != 3; ++j) {
            printf(".");
            Sleep(255);
        }
        printf("\r          \r");
    }
	screen_clear();
}

/******** symbols ********/

#define SYM_BLANK "  "
#define SYM_BLOCK_BRACK "[]"
#define SYM_CIRCLE_PAREN "()"

#ifdef __linux__
	#define SYM_FILL " "
	#define SYM_BLOCK "██"
#else
	#define SYM_FILL
	#define SYM_BLOCK "█"
#endif

#define SYM_SQUARE "■" SYM_FILL
#define SYM_DIAMOND "◆" SYM_FILL
#define SYM_STAR "★" SYM_FILL
#define SYM_ARROW_UP "↑" SYM_FILL
#define SYM_ARROW_LEFT "←" SYM_FILL
#define SYM_ARROW_DOWN "↓" SYM_FILL
#define SYM_ARROW_RIGHT "→" SYM_FILL
#define SYM_ARROWS SYM_ARROW_LEFT SYM_ARROW_DOWN SYM_ARROW_UP SYM_ARROW_RIGHT

#endif // Game_h
