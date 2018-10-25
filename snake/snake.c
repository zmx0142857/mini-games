#include <stdio.h>
#include <stdlib.h>		// Sleep(), system()
#include <windows.h>	// HANDLE, BOOL, ...
#include <conio.h>		// getch()
#include <time.h>		// rand(), srand(), time()
#include <ctype.h>		// tolower()

// ����̨���
HANDLE handle;

// ��ͼ
#define ROWS 20
#define COLS 30

// ��һ���б���������Ϣ
// �����ʼ����Ϊ LEN, ��ʼλ��Ϊ (9,9), (9,8)
#define LEN 2
#define MAX_LEN ROWS * COLS
unsigned head = 0;					// ��ͷ
unsigned tail = LEN;				// β��λ��
unsigned body_r[MAX_LEN+1] = {9,9};
unsigned body_c[MAX_LEN+1] = {9,8};


// ʳ��
unsigned food_r = 0;
unsigned food_c = 0;
BOOL has_food = 0;

// ����
unsigned score = 0;

// �޸���Ļ�ߴ�
void screen_resize(unsigned rows, unsigned cols)
{
	char cmd[64];
	sprintf(cmd, "mode con: cols=%d lines=%d", cols, rows);
    system(cmd);
}

// ���ع��
void cursor_hide()
{
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &info);
}

// ��ָ��λ������ַ�
void render(char *wch, unsigned row, unsigned col)
{
    COORD pos = {2*col, row};
    SetConsoleCursorPosition(handle, pos);
    printf("%s", wch);
}

// ��ӡ����
void print_score()
{
    COORD pos = {8, ROWS+1};
    SetConsoleCursorPosition(handle, pos);
    printf("%d", score);
}

// �жϴ������Ƿ���ǽ

BOOL is_wall(unsigned row, unsigned col)
{
	return row <= 0 || row >= ROWS-1 || col <= 0 || col >= COLS-1;
}

// �жϴ������Ƿ���ʳ��
BOOL is_food(unsigned row, unsigned col)
{
	return has_food && row == food_r && col == food_c;
}

// �жϴ������Ƿ�������
BOOL is_body(unsigned row, unsigned col)
{
	unsigned i;
	for (i = head; i != tail; i = (i+1) % (MAX_LEN+1))
		if (row == body_r[i] && col == body_c[i])
			break;

	// i == tail ���ҽ��� (row, col) �������غ�
	return i != tail;
}

// ����ʳ��
void food_create()
{
    if (has_food) return;

	do {
		food_r = rand() % (ROWS-2) + 1;
		food_c = rand() % (COLS-2) + 1;
    } while (is_body(food_r, food_c));

	has_food = 1;
}

// ��ӡ��ͼ
void map_print()
{
    unsigned i, j;
    for (i = 0; i != ROWS; ++i)
    {
        for (j = 0; j != COLS; ++j)
        {
            if (is_wall(i, j))
            {
				printf("��");
			}
			else if (is_body(i, j))
			{
                printf("��");
			}
            else if (is_food(i, j))
			{
                printf("��");
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

// ��ʼ��
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

// ��ѭ��
void game_play()
{
    char key;
	char last_key = 'd';	// ��ͷ��ʼ����

    // �����̣�������
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

		if (is_wall(new_r, new_c))		// ײǽ����
		{
			break;
		}
		else if (is_food(new_r, new_c))	// �Ե�ʳ������
        {
            score += 10 + rand() % 11;
			has_food = 0;
            food_create();
            render("��", food_r, food_c);
        }
        else							// ��ȥβ��
        {
			tail = (tail == 0 ? MAX_LEN : tail-1);
            render("  ", body_r[tail], body_c[tail]);
			if (is_body(new_r, new_c))	// ҧ���Լ�����
				break;
        }

		// ���ڰ�ȫ�����Ի���ͷ��
		render("��", new_r, new_c);

		// �����������
		head = (head == 0 ? MAX_LEN : head-1);
		body_r[head] = new_r;
		body_c[head] = new_c;

        print_score();
        Sleep(32765/(score+200)+40);	// ����Խ�ߣ��ƶ�Խ��

        if (kbhit())					// �ж��Ƿ��°���
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
