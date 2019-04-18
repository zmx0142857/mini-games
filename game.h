#ifndef Game_h
#define Game_h

#include <stdio.h>
#include <sys/ioctl.h>	// winsize, ioctl()
#include <termios.h>	// termios

struct winsize tty;
#define get_ttysize()	ioctl(0, TIOCGWINSZ, &tty);

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

#define STYLE_RESET			"\033[0m"
#define STYLE_BRIGHT		"\033[1m"
#define STYLE_DIM			"\033[2m"
#define STYLE_UNDERLINE		"\033[4m"
#define STYLE_BLINK			"\033[5m"
#define STYLE_REVERSE		"\033[7m"
#define STYLE_HIDDEN		"\033[8m"

#define BLACK				"0"
#define RED					"1"
#define GREEN				"2"
#define YELLOW				"3"
#define BLUE				"4"
#define MAGENTA				"5"
#define CYAN				"6"
#define WHITE				"7"
#define COLOR(color)		"\033[3" color "m"
#define COLORS(fg, bg)		"\033[3" fg ";4" bg "m"

#define screen_clear()		printf("\033[2J\033[0;0H")
#define line_clear()		printf("\033[K")

#define cursor_goto(y,x)	printf("\033[%d;%dH", y, x)
#define cursor_cr()			pritnf("\r")
#define cursor_up(n)		printf("\033[%dA", n)
#define cursor_down(n)		printf("\033[%dB", n)
#define cursor_right(n)		printf("\033[%dC", n)
#define cursor_left(n)		printf("\033[%dD", n)

#endif // Game_h
