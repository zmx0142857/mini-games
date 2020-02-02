#include "game.h"
#include <time.h>	// time()
#include <string.h>	// memset()
#include <ctype.h>	// tolower()

#define WIDTH	10
#define HEIGHT	20
#define BUFFER_HEIGHT	4
bool map[HEIGHT+BUFFER_HEIGHT+1][WIDTH];

#define MOVE_LEFT -1
#define MOVE_DOWN 0
#define MOVE_RIGHT 1
#define ROTATE90 90
#define ROTATE180 180
#define ROTATE270 270
#define msglog(s...)\
	do {\
		mvprint(13,2*WIDTH+3, "          ");\
		mvprint(13, 2*WIDTH+3, s);\
	} while (false)

// debug only
void map_dump()
{
	for (int i = 0; cursor_goto(i, 40), i < HEIGHT+BUFFER_HEIGHT; ++i)
		for (int j = 0; j < WIDTH; ++j)
			putchar('0' + map[i][j]);
}

typedef struct {
	int x[4];
	int y[4];
} Tetris;

// coordinate y increases
Tetris tetris[7] = {
	// parenthesis indicates the pivot
	{ {-1, 0, 1, 2}, {0, 0, 0, 0} },	// []()[][]

	{ {0, -1, 0, 1}, {-1, 0, 0, 0} },	//   []
										// []()[]

	{ {0, 1, -1, 0}, {-1, -1, 0, 0} },	//   [][]
										// []()

	{ {-1, 0, 0, 1}, {-1, -1, 0, 0} },	// [][]
										//   ()[]

	{ {-1, 0, 1, 1}, {0, 0, 0, 1} },	// []()[]
										//     []

	{ {-1, 0, 1, 0}, {0, 0, 0, 1} },	// []()[]
										// []
	// this one doesn't rotate
	{ {0, 1, 0, 1}, {0, 0, 1, 1} }		// [][]
										// [][]
};

Tetris tetNext, tetCurrent;

int score = 0;
bool is_game_ready = false;
bool is_game_over = false;
bool need_new = true;
int tet_x, tet_y;

void score_print()
{
	mvprint(3, 2*WIDTH+4, "%6d", score);
}

void tet_eliminate()
{
	static int ys[4];
	int cnt = 0;
	bool ok;
	// find rows to emilinate
	for (int i = 0; i < 4; ++i) {
		ok = true;
		int y = ys[cnt] = tet_y + tetCurrent.y[i];
		for (int j = 0; j < cnt; ++j) {
			if (ys[j] == y) {
				ok = false;
				break;
			}
		}
		if (ok) {
			for (int x = 0; x < WIDTH; ++x) {
				if (!map[y][x]) {
					ok = false;
					break;
				}
			}
		}
		if (ok) ++cnt;
	}

	// insertion-sort the ys
	if (cnt) {
		for (int i = 1; i < cnt; ++i) {
			int j;
			int tmp = ys[i];
			for (j = i; j > 0 && ys[j-1] > tmp; --j)
				ys[j] = ys[j-1];
			ys[j] = tmp;
		}
	}
	
	// eliminate
	for (int i = 0; i < cnt; ++i) {
		int y = ys[i];
		while (y > 0) {
			for (int x = 0; x < WIDTH; ++x) {
				if (map[y][x] != map[y-1][x]) {
					map[y][x] = map[y-1][x];
					mvprint(y-BUFFER_HEIGHT, x*2+1,
						map[y][x] ? SYM_BLOCK_BRACK : SYM_BLANK);
				}
			}
			--y;
		}
		for (int x = 0; x < WIDTH; ++x) {
			map[0][x] = false;
			mvprint(0, x*2+1, SYM_BLANK);
		}
	}

	if (cnt) {
		score += cnt*(100+(cnt-1)*50);
		score_print();
	}
}

void tet_rotate(Tetris *tet, int angle)
{
	int tmp;
	for (int rotated = 0; rotated < angle; rotated += 90) {
		for (int i = 0; i < 4; ++i) {
			tmp = tet->y[i];
			tet->y[i] = tet->x[i];
			tet->x[i] = -tmp;
		}
	}
}

void tet_print(const Tetris *tet, int y, int x, bool visible)
{
	for (int i = 0; i < 4; ++i) {
		int new_x = x + tet->x[i];
		int new_y = y + tet->y[i];
		if (new_y >= 0)
			mvprint(new_y, new_x*2 + 1,
					visible ? SYM_BLOCK_BRACK : SYM_BLANK);
	}
}

void tet_copy(Tetris *dest, const Tetris *src)
{
	for (int i = 0; i < 4; ++i) {
		dest->x[i] = src->x[i];
		dest->y[i] = src->y[i];
	}
}

void tet_set(bool visible)
{
	for (int i = 0; i < 4; ++i) {
		int new_x = tet_x + tetCurrent.x[i];
		int new_y = tet_y + tetCurrent.y[i];
		map[new_y][new_x] = visible;
	}
	tet_print(&tetCurrent, tet_y-BUFFER_HEIGHT, tet_x, visible);
}

bool tet_chk(int movement)
{
	tet_set(false);
	if (movement == MOVE_LEFT || movement == MOVE_RIGHT) {
		for (int i = 0; i < 4; ++i) {
			int new_x = tet_x + tetCurrent.x[i] + movement;
			int new_y = tet_y + tetCurrent.y[i];
			if (new_x < 0 || new_x >= WIDTH || map[new_y][new_x] == true)
			{
				tet_set(true);
				return false;
			}
		}
	} else if (movement == MOVE_DOWN) {
		for (int i = 0; i < 4; ++i) {
			int new_x = tet_x + tetCurrent.x[i];
			int new_y = tet_y + tetCurrent.y[i] + 1;
			if (map[new_y][new_x] == true) {
				tet_set(true);
				tet_eliminate();
				for (int j = 0; j < 4; ++j) {
					if (tet_y + tetCurrent.y[j] < BUFFER_HEIGHT)
						is_game_over = true;
				}
				need_new = true;
				return false;
			}
		}
	} else { // rotate
		Tetris tmp;
		tet_copy(&tmp, &tetCurrent);
		tet_rotate(&tmp, movement);
		int tmp_x = tet_x, tmp_y = tet_y;
		for (int i = 0; i < 4; ++i) {
			int new_x = tmp_x + tmp.x[i];
			int new_y = tmp_y + tmp.y[i];
			if (new_x < 0)
				tmp_x += -new_x;
			else if (new_x >= WIDTH)
				tmp_x -= new_x - WIDTH + 1;
			else if (new_y >= HEIGHT+BUFFER_HEIGHT)
				tmp_y -= new_y - HEIGHT - BUFFER_HEIGHT + 1;
		}
		for (int i = 0; i < 4; ++i) {
			int new_x = tmp_x + tmp.x[i];
			int new_y = tmp_y + tmp.y[i];
			if (map[new_y][new_x] == true) {
				tet_set(true);
				return false;
			}
		}
		tet_x = tmp_x, tet_y = tmp_y;
	}
	return true;
}

void tet_generate()
{
	if (!is_game_ready) {
		srand(time(NULL));
	} else {
		tet_copy(&tetCurrent, &tetNext);
		tet_set(true);
		tet_print(&tetNext, 9, WIDTH+3, false);
	}
	tet_copy(&tetNext, tetris + rand() % 7);
	tet_rotate(&tetNext, rand() % 4 * 90);
	tet_print(&tetNext, 9, WIDTH+3, true);
	if (!is_game_ready) {
		is_game_ready = true;
		tet_generate();
	}
}

bool tet_move(int movement)
{
	if (tet_chk(movement)) {
		if (movement == MOVE_LEFT || movement == MOVE_RIGHT)
			tet_x += movement;
		else if (movement == MOVE_DOWN)
			++tet_y;
		else
			tet_rotate(&tetCurrent, movement);
		tet_set(true);
		return true;
	}
	return false;
}

void control()
{
	is_game_ready = false;
	is_game_over = false;
	need_new = true;
	while (!is_game_over) {
		if (need_new) {
			need_new = false;
			tet_x = 5;
			tet_y = 1;
			tet_generate();
		} else
			tet_move(MOVE_DOWN);

		while (!need_new && kbhit()) {
			char key = tolower(getch());
			switch (key) {
				case 'a': case '4':
					if (tet_move(MOVE_LEFT)) Sleep(30); break;
				case 'd': case '6':
					if (tet_move(MOVE_RIGHT)) Sleep(30); break;
				case 'w': case '2':
					if (tet_move(ROTATE90)) Sleep(30); break;
				case 's': case '5':
					while (tet_move(MOVE_DOWN)) {} break;
				case KEY_ESC:
					msglog("paused");
					key = getch();
					if (key == KEY_ESC) {
						return;
					} else {
						msglog(" ");
					}
			}
		}
		Sleep(300);
	}
}

void scene_play()
{
	screen_clear();
	for (int i = 0; i < HEIGHT; ++i) {
		mvprint(i, 0, "|");
		mvprint(i, 2*WIDTH+1, "|");
	}
	mvprint(HEIGHT, 0, "+--------------------+");
	mvprint(1, 2*WIDTH+4, " score");
	mvprint(5, 2*WIDTH+4, " next");
	score = 0;
	score_print();
	memset(map, 0, sizeof(map));
	memset(map+HEIGHT+BUFFER_HEIGHT, 1, WIDTH);

	control();

	mvprint(13, 2*WIDTH+3, "game over!");
	getch();
}

void scene_high_score()
{
	screen_clear();
	printf("zmx: 99999\n\n"
		   "any key to return\n");
	getch();
}

void scene_help()
{
	screen_clear();
	printf("w, 2 - rotate\n"
		   "a, 4 - move left\n"
		   "d, 6 - move right\n"
		   "s, 5 - drop\n"
		   "<esc> - pause; when paused,\n"
		   "  press <esc> again to exit\n\n"
		   "any key to return\n");
	getch();
}

void scene_welcome()
{
	while (true) {
		screen_clear();
		printf(
			"\n\n\n\n\n\n"
			"        Welcome to Tetris!\n\n\n"
			"            1. play\n"
			"            2. high score\n"
			"            3. help\n"
			"            4. exit\n");
		switch (getch()) {
			case '1': scene_play(); break;
			case '2': scene_high_score();  break;
			case '3': scene_help(); break;
			case '4': case KEY_ESC: return;
		}
	}
}

int main()
{
	game_init();
	screen_size(21, 32);
	scene_welcome();
	game_over();
	return 0;
}
