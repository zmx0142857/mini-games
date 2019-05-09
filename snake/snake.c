#include "../game.h"
#include <time.h>		// rand(), srand(), time()
#include <ctype.h>		// tolower()

// 地图
#define ROWS 20
#define COLS 30

// 分数
unsigned score;

// 用一队列保存蛇身信息
// 蛇身初始长度为 2, 初始位置为 (9,9), (9,8)
#define MAX_LEN ROWS * COLS
struct {
	unsigned head;					// 队头
	unsigned tail;					// 尾后位置
	struct {
		unsigned y[MAX_LEN+1];
		unsigned x[MAX_LEN+1];
	} body;
} snake;

// 食物
struct {
	unsigned y;
	unsigned x;
	bool exist;
} food;

// 判断此坐标是否是墙
bool is_wall(unsigned y, unsigned x)
{
	return y <= 0 || y >= ROWS-1 || x <= 0 || x >= COLS-1;
}

// 判断此坐标是否是食物
bool is_food(unsigned y, unsigned x)
{
	return food.exist && y == food.y && x == food.x;
}

// 判断此坐标是否是蛇身
bool is_body(unsigned y, unsigned x)
{
	unsigned i;
	for (i = snake.head; i != snake.tail; i = (i+1) % (MAX_LEN+1))
		if (y == snake.body.y[i] && x == snake.body.x[i])
			break;
	// i == snake.tail 当且仅当 (y, x) 与蛇身不重合
	return i != snake.tail;
}

// 生成食物
void food_create()
{
    if (!food.exist) {
		do {
			food.y = rand() % (ROWS-2) + 1;
			food.x = rand() % (COLS-2) + 1;
		} while (is_body(food.y, food.x));
		food.exist = true;
	}
}

// 打印地图
void map_print()
{
    unsigned y, x;
    for (y = 0; y != ROWS; ++y) {
        for (x = 0; x != COLS; ++x) {
            if (is_wall(y, x))
				printf(SQUARE);
			else if (is_body(y, x))
				printf(SQUARE);
            else if (is_food(y, x))
                printf(DIAMOND);
			else
				printf(BLANK);
        }
		putchar('\n');
    }
    printf("\nScores: 0\nUse wasd to play.\n");
}

// 初始化
void init()
{
	screen_clear();
	srand(time(NULL));

    score = 0;

	snake.head = 0;
	snake.tail = 2;
    snake.body.y[0] = 9;
    snake.body.x[0] = 9;
    snake.body.y[1] = 9;
    snake.body.x[1] = 8;

    food.y = 0;
    food.x = 0;
    food.exist = false;

	food_create();
	map_print();
}

// 主循环
void play()
{
    char key;
	char last_key = 'd';	// 蛇头初始向右

    // 读键盘，不回显
    key = tolower(getch());
    while (true) {
		bool valid = ( (key == 'w' && last_key != 's')
					|| (key == 's' && last_key != 'w')
					|| (key == 'a' && last_key != 'd')
					|| (key == 'd' && last_key != 'a') );
		if (valid)
			last_key = key;
		else
			key = last_key;

		unsigned new_y = snake.body.y[snake.head];
		unsigned new_x = snake.body.x[snake.head];
		switch (key) {
			case 'w': --new_y; break;
			case 's': ++new_y; break;
			case 'a': --new_x; break;
			case 'd': ++new_x; break;
		}

		if (is_wall(new_y, new_x)) {		// 撞墙了吗
			break;
		} else if (is_food(new_y, new_x)) {	// 吃到食物了吗
            score += 10 + rand() % 11;
			mvprint(ROWS+1, 8, "%d", score);
			food.exist = false;
            food_create();
            mvprint(food.y, 2*food.x, DIAMOND);
        } else {							// 消去尾部
			snake.tail = (snake.tail == 0 ? MAX_LEN : snake.tail-1);
            mvprint(snake.body.y[snake.tail], 2*snake.body.x[snake.tail], BLANK);
			if (is_body(new_y, new_x))		// 咬到自己了吗
				break;
        }

		// 现在安全，可以绘制头部
		mvprint(new_y, 2*new_x, SQUARE);

		// 将新坐标入队
		snake.head = (snake.head == 0 ? MAX_LEN : snake.head-1);
		snake.body.y[snake.head] = new_y;
		snake.body.x[snake.head] = new_x;

		Sleep(32765/(score+200)+40);	// 分数越高，移动越快

		// 判断是否按下按键
		if (kbhit())
			key = tolower(getch());

    }
}

bool new_game()
{
	mvprint(ROWS+2, 0, "Game over! <ESC> to quit, any other key to restart\n");
	return getch() != 27;
}

int main()
{
	game_init();
	screen_size(ROWS+4, COLS*2+1);

	do {
		init();
		play();
	} while (new_game());

	game_over();
    return 0;
}
