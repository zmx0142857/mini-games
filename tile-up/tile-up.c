#include "../game.h"

#define WIDTH 8
#define HEIGHT 8

void scene_play()
{
	screen_clear();
	for (int i = 0; i < HEIGHT; ++i) {
		mvprint(i << 1, WIDTH << 2, "|");
		mvprint((i << 1) + 1, WIDTH << 2, "|");
	}
	mvprint(HEIGHT << 1, 0, "--------------------------------+");
	getch();
}

void scene_high_score()
{
	screen_clear();
	printf("zmx: 10000\n"
		   "hyr: 12000\n"
		   "Any key to return\n");
	getch();
}

void scene_help()
{
	screen_clear();
	printf("You know how to play it!\n"
		   "Any key to return\n");
	getch();
}

void scene_welcome()
{
	while (true) {
		screen_clear();
		printf(
			"\n\n\n\n\n\n"
			"        Welcome to Tile-Up!\n\n\n"
			"            1. play\n"
			"            2. high score\n"
			"            3. help\n"
			"            4. exit\n");
		switch (getch()) {
			case '1': scene_play(); break;
			case '2': scene_high_score();  break;
			case '3': scene_help(); break;
			case '4': return;
		}
	}
}

int main()
{
	game_init();
	screen_size(20, 30);
	scene_welcome();
	game_over();
	return 0;
}
