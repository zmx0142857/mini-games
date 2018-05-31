#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define MAP_R 20    // R: row
#define MAP_C 30    // C: column

char map[MAP_R][MAP_C];     // ��Ϸ��ͼ
unsigned score = 0;         // ��ǰ����

// �����ʼ����Ϊ2, �������б�ʾ�ߵĳ�ʼλ��Ϊ(9,9),(9,8)
unsigned body_r[MAP_R * MAP_C] = {9,9};
unsigned body_c[MAP_R * MAP_C] = {9,8};
unsigned len = 2;

// ʳ��
unsigned food_r = 0;
unsigned food_c = 0;
unsigned foodCnt = 0;

// ��������
void initialize();
void createFood();
void render(char *wch, unsigned col, unsigned row);
void printScore();
void printMap();
void play();

// ������
int main()
{
    char key;

    // �Ķ� MAX_R �� MAX_C ʱҪ��ʱ������Ļ��С!
    system("MODE CON: COLS=61 LINES=25");

    while (1)
    {
        srand(time(NULL));
        createFood();
        printMap();

        play();

        render("Game over! <Esc> to quit, any other key to restart.", 0, MAP_R+2);
        if ( (key = getch()) == 27) // 'Esc'
            break;

        initialize();
    }
    return 0;
}

// ��ʼ��
void initialize()
{
    system("cls");
    score = 0;
    body_r[0] = 9;
    body_c[0] = 9;
    body_r[1] = 9;
    body_c[1] = 8;
    len = 2;
    food_r = 0;
    food_c = 0;
    foodCnt = 0;
}

// ����ʳ��
void createFood()
{
    if (foodCnt != 0)
        return;

    food_r = 0;
    food_c = 0;

    unsigned fr = rand() % (MAP_R-2) + 1;
    unsigned fc = rand() % (MAP_C-2) + 1;
    BOOL overlapped = 0;   // ʳ���Ƿ��������غ�

    do {
        unsigned i;
        for (i = 0; i != len; ++i)
        {
            overlapped = (body_r[i] == food_r && body_c[i] == food_c);
            if (overlapped)
            {
                fr = rand() % (MAP_R-2) + 1;
                fc = rand() % (MAP_C-2) + 1;
                break;
            }
        }
    } while (overlapped);

    ++foodCnt;
    food_r = fr;
    food_c = fc;
}

// ��ָ��λ������ַ�
void render(char *wch, unsigned col, unsigned row)
{
    HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {2*col, row};
    SetConsoleCursorPosition(hd, pos);
    printf("%s", wch);
    pos.X = 0;
    pos.Y = MAP_R + 3;
    SetConsoleCursorPosition(hd, pos);
}

// ��ӡ����
void printScore()
{
    HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {7, MAP_R+1};
    SetConsoleCursorPosition(hd, pos);
    printf("%d", score);
    pos.X = 0;
    pos.Y = MAP_R + 3;
    SetConsoleCursorPosition(hd, pos);
}

// ��ӡ��ͼ
void printMap()
{
    unsigned i, j, k;
    for (i = 0; i != MAP_R; ++i)
    {
        for (j = 0; j != MAP_C; ++j)
        {
            // ��ǽ
            if (i == 0 || i == MAP_R-1 || j == 0 || j == MAP_C-1)
                map[i][j] = 'w';
            else
                map[i][j] = ' ';

            // ��ǰλ����������
            BOOL snake = 0;
            for (k = 0; k != len && snake == 0; ++k)
            {
                snake = (i == body_r[k] && j == body_c[k]);
            }

            // ��ǰλ����ʳ����
            BOOL food = (foodCnt && food_r == i && food_c == j);

            // ��ǰλ����ʲô
            if (snake)
                printf("��");
            else if (food)
                printf("��");
            else
            {
                switch (map[i][j])
                {
                    case 'w': printf("��");  break;
                    case ' ': printf("  "); break;
                    default : ; // �����
                }
            }
        } // for(j) ����

        printf("\n");

    } // for(i) ����

    printf("\nScore: 0\nUse wasd to play.\n");
}

// ��ѭ��
void play()
{
    char key = 0, lastKey = 'd';
    BOOL beat = 0;

    // ȡ��һ������������
    key = getch();

    while (1)
    {
        unsigned copy_x[MAP_R * MAP_C];
        unsigned copy_y[MAP_R * MAP_C];

        // ��������
        memcpy(copy_x, body_r, sizeof(unsigned)*len);
        memcpy(copy_y, body_c, sizeof(unsigned)*len);

        // body_r[0], body_c[0]: ͷ
        switch (key)
        {
            case 'W': case 'w': if (lastKey == 'S' || lastKey == 's') { key = lastKey; continue; } else --body_r[0]; break;
            case 'S': case 's': if (lastKey == 'W' || lastKey == 'w') { key = lastKey; continue; } else ++body_r[0]; break;
            case 'A': case 'a': if (lastKey == 'D' || lastKey == 'd') { key = lastKey; continue; } else --body_c[0]; break;
            case 'D': case 'd': if (lastKey == 'A' || lastKey == 'a') { key = lastKey; continue; } else ++body_c[0]; break;
            default : key = lastKey; continue;
        }
        lastKey = key;

        // ײǽ����
        if  (   body_r[0] <= 0 || body_r[0] >= MAP_R-1
            ||  body_c[0] <= 0 || body_c[0] >= MAP_C-1 )
            return;

        // �Ե�ʳ������
        if (foodCnt > 0 && body_r[0] == food_r && body_c[0] == food_c)
        {
            memcpy(body_r+1, copy_x, sizeof(unsigned)*len);
            memcpy(body_c+1, copy_y, sizeof(unsigned)*len);
            render("��", body_c[0], body_r[0]);          // ����ͷ��

            ++len;
            --foodCnt;
            score += 10 + rand() % 11;
            createFood();
            render("��", food_c, food_r);                // ����ʳ��
        }
        else
        {
            memcpy(body_r+1, copy_x, sizeof(unsigned)*(len-1));
            memcpy(body_c+1, copy_y, sizeof(unsigned)*(len-1));
            render("��", body_c[0], body_r[0]);          // ����ͷ��
            render("  ", copy_y[len-1], copy_x[len-1]); // ��ȥβ��
        }

        printScore();
        Sleep(32765/(score+200)+40);    // ����Խ�ߣ��ƶ�Խ��

        // ҧ���Լ�����
        unsigned i;
        for (i = 1; i != len; ++i)
        {
            beat = (body_r[0] == body_r[i] && body_c[0] == body_c[i]);
            if (beat)
                return;
        }

        if ( kbhit() )      // �ж��Ƿ��°���
            key = getch();

    } // ��ѭ������
}

#undef MAP_R
#undef MAP_C
