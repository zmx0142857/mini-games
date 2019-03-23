#include <stdio.h>
#include <stdlib.h>		// Sleep(), system()
#include <windows.h>	// HANDLE, BOOL, ...
#include <conio.h>		// getch()
#include <time.h>		// rand(), srand(), time()
#include <ctype.h>		// tolower()

// 控制台句柄
HANDLE handle;

// 地图
#define ROWS 20
#define COLS 30

// 用一队列保存蛇身信息
// 蛇身初始长度为 LEN, 初始位置为 (9,9), (9,8)
#define LEN 2
#define MAX_LEN ROWS * COLS
unsigned head = 0;					// 队头
unsigned tail = LEN;				// 尾后位置
unsigned body_r[MAX_LEN+1] = {9,9};
unsigned body_c[MAX_LEN+1] = {9,8};


// 食物
unsigned food_r = 0;
unsigned food_c = 0;
BOOL has_food = 0;

// 分数
unsigned score = 0;

// 修改屏幕尺寸
void screen_resize(unsigned rows, unsigned cols)
{
	char cmd[64];
	sprintf(cmd, "mode con: cols=%d lines=%d", cols, rows);
    system(cmd);
}

// 隐藏光标
void cursor_hide()
{
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &info);
}

// 向指定位置输出字符
void render(char *wch, unsigned row, unsigned col)
{
    COORD pos = {2*col, row};
    SetConsoleCursorPosition(handle, pos);
    printf("%s", wch);
}

// 打印分数
void print_score()
{
    COORD pos = {8, ROWS+1};
    SetConsoleCursorPosition(handle, pos);
    printf("%d", score);
}

// 判断此坐标是否是墙

BOOL is_wall(unsigned row, unsigned col)
{
	return row <= 0 || row >= ROWS-1 || col <= 0 || col >= COLS-1;
}

// 判断此坐标是否是食物
BOOL is_food(unsigned row, unsigned col)
{
	return has_food && row == food_r && col == food_c;
}

// 判断此坐标是否是蛇身
BOOL is_body(unsigned row, unsigned col)
{
	unsigned i;
	for (i = head; i != tail; i = (i+1) % (MAX_LEN+1))
		if (row == body_r[i] && col == body_c[i])
			break;

	// i == tail 当且仅当 (row, col) 与蛇身不重合
	return i != tail;
}

// 生成食物
void food_create()
{
    if (has_food) return;

	do {
		food_r = rand() % (ROWS-2) + 1;
		food_c = rand() % (COLS-2) + 1;
    } while (is_body(food_r, food_c));

	has_food = 1;
}

// 打印地图
void map_print()
{
    unsigned i, j;
    for (i = 0; i != ROWS; ++i)
    {
        for (j = 0; j != COLS; ++j)
        {
            if (is_wall(i, j))
            {
				printf("■");
			}
			else if (is_body(i, j))
			{
                printf("■");
			}
            else if (is_food(i, j))
			{
                printf("◆");
			}
			else
			{
				printf("  ");
            }
        }
        printf("\n");
    }
    printf("\nScores: 0\nUse wasd to play.\n");
}

// 初始化
void game_init()
{
    system("cls");
	srand(time(NULL));

    score = 0;

	head = 0;
	tail = LEN;
    body_r[0] = 9;
    body_c[0] = 9;
    body_r[1] = 9;
    body_c[1] = 8;

    food_r = 0;
    food_c = 0;
    has_food = 0;

	food_create();
	map_print();
}

// 主循环
void game_play()
{
    char key;
	char last_key = 'd';	// 蛇头初始向右

    // 读键盘，不回显
    key = tolower(getch());
    while (1)
    {
		BOOL valid = ( (key == 'w' && last_key != 's')
					|| (key == 's' && last_key != 'w')
					|| (key == 'a' && last_key != 'd')
					|| (key == 'd' && last_key != 'a') );
		if (valid)
		{
			last_key = key;
		}
		else
		{
			key = last_key;
		}

		unsigned new_r = body_r[head];
		unsigned new_c = body_c[head];
		switch (key)
		{
			case 'w': --new_r; break;
			case 's': ++new_r; break;
			case 'a': --new_c; break;
			case 'd': ++new_c; break;
		}

		if (is_wall(new_r, new_c))		// 撞墙了吗
		{
			break;
		}
		else if (is_food(new_r, new_c))	// 吃到食物了吗
        {
            score += 10 + rand() % 11;
			has_food = 0;
            food_create();
            render("◆", food_r, food_c);
        }
        else							// 消去尾部
        {
			tail = (tail == 0 ? MAX_LEN : tail-1);
            render("  ", body_r[tail], body_c[tail]);
			if (is_body(new_r, new_c))	// 咬到自己了吗
				break;
        }

		// 现在安全，可以绘制头部
		render("■", new_r, new_c);

		// 将新坐标入队
		head = (head == 0 ? MAX_LEN : head-1);
		body_r[head] = new_r;
		body_c[head] = new_c;

        print_score();
        Sleep(32765/(score+200)+40);	// 分数越高，移动越快

        if (kbhit())					// 判断是否按下按键
            key = tolower(getch());
    }
}

BOOL game_over()
{
	render("Game over! <Esc> to quit, any other key to restart.", ROWS+2, 0);
	Sleep(800);
	return getch() == 27; // 'Esc'
}

int main()
{
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	screen_resize(ROWS+4, COLS*2+1);
	//system("chcp 936"); // change codepage to gbk
	system("chcp 65001"); // change codepage to utf-8
	cursor_hide();

    do {
		game_init();
        game_play();
    } while (!game_over());

    return 0;
}

#undef ROWS
#undef COLS
#undef LEN
#undef MAX_LEN
