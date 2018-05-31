#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define MAP_R 20    // R: row
#define MAP_C 30    // C: column

char map[MAP_R][MAP_C];     // 游戏地图
unsigned score = 0;         // 当前分数

// 蛇身初始长度为2, 以下两列表示蛇的初始位置为(9,9),(9,8)
unsigned body_r[MAP_R * MAP_C] = {9,9};
unsigned body_c[MAP_R * MAP_C] = {9,8};
unsigned len = 2;

// 食物
unsigned food_r = 0;
unsigned food_c = 0;
unsigned foodCnt = 0;

// 函数声明
void initialize();
void createFood();
void render(char *wch, unsigned col, unsigned row);
void printScore();
void printMap();
void play();

// 主函数
int main()
{
    char key;

    // 改动 MAX_R 和 MAX_C 时要及时更新屏幕大小!
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

// 初始化
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

// 生成食物
void createFood()
{
    if (foodCnt != 0)
        return;

    food_r = 0;
    food_c = 0;

    unsigned fr = rand() % (MAP_R-2) + 1;
    unsigned fc = rand() % (MAP_C-2) + 1;
    BOOL overlapped = 0;   // 食物是否与蛇身重合

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

// 向指定位置输出字符
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

// 打印分数
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

// 打印地图
void printMap()
{
    unsigned i, j, k;
    for (i = 0; i != MAP_R; ++i)
    {
        for (j = 0; j != MAP_C; ++j)
        {
            // 砌墙
            if (i == 0 || i == MAP_R-1 || j == 0 || j == MAP_C-1)
                map[i][j] = 'w';
            else
                map[i][j] = ' ';

            // 当前位置是蛇身吗
            BOOL snake = 0;
            for (k = 0; k != len && snake == 0; ++k)
            {
                snake = (i == body_r[k] && j == body_c[k]);
            }

            // 当前位置是食物吗
            BOOL food = (foodCnt && food_r == i && food_c == j);

            // 当前位置有什么
            if (snake)
                printf("■");
            else if (food)
                printf("◆");
            else
            {
                switch (map[i][j])
                {
                    case 'w': printf("■");  break;
                    case ' ': printf("  "); break;
                    default : ; // 空语句
                }
            }
        } // for(j) 结束

        printf("\n");

    } // for(i) 结束

    printf("\nScore: 0\nUse wasd to play.\n");
}

// 主循环
void play()
{
    char key = 0, lastKey = 'd';
    BOOL beat = 0;

    // 取得一个键，不回显
    key = getch();

    while (1)
    {
        unsigned copy_x[MAP_R * MAP_C];
        unsigned copy_y[MAP_R * MAP_C];

        // 拷贝数组
        memcpy(copy_x, body_r, sizeof(unsigned)*len);
        memcpy(copy_y, body_c, sizeof(unsigned)*len);

        // body_r[0], body_c[0]: 头
        switch (key)
        {
            case 'W': case 'w': if (lastKey == 'S' || lastKey == 's') { key = lastKey; continue; } else --body_r[0]; break;
            case 'S': case 's': if (lastKey == 'W' || lastKey == 'w') { key = lastKey; continue; } else ++body_r[0]; break;
            case 'A': case 'a': if (lastKey == 'D' || lastKey == 'd') { key = lastKey; continue; } else --body_c[0]; break;
            case 'D': case 'd': if (lastKey == 'A' || lastKey == 'a') { key = lastKey; continue; } else ++body_c[0]; break;
            default : key = lastKey; continue;
        }
        lastKey = key;

        // 撞墙了吗
        if  (   body_r[0] <= 0 || body_r[0] >= MAP_R-1
            ||  body_c[0] <= 0 || body_c[0] >= MAP_C-1 )
            return;

        // 吃到食物了吗
        if (foodCnt > 0 && body_r[0] == food_r && body_c[0] == food_c)
        {
            memcpy(body_r+1, copy_x, sizeof(unsigned)*len);
            memcpy(body_c+1, copy_y, sizeof(unsigned)*len);
            render("■", body_c[0], body_r[0]);          // 绘制头部

            ++len;
            --foodCnt;
            score += 10 + rand() % 11;
            createFood();
            render("◆", food_c, food_r);                // 绘制食物
        }
        else
        {
            memcpy(body_r+1, copy_x, sizeof(unsigned)*(len-1));
            memcpy(body_c+1, copy_y, sizeof(unsigned)*(len-1));
            render("■", body_c[0], body_r[0]);          // 绘制头部
            render("  ", copy_y[len-1], copy_x[len-1]); // 消去尾部
        }

        printScore();
        Sleep(32765/(score+200)+40);    // 分数越高，移动越快

        // 咬到自己了吗
        unsigned i;
        for (i = 1; i != len; ++i)
        {
            beat = (body_r[0] == body_r[i] && body_c[0] == body_c[i]);
            if (beat)
                return;
        }

        if ( kbhit() )      // 判断是否按下按键
            key = getch();

    } // 死循环结束
}

#undef MAP_R
#undef MAP_C
