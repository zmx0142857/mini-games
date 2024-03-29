#include <string.h> // memset(), strcpy()
#include <ctype.h> // isdigit()
#include "game.h"

// 常量
#define MAP_W       17
#define MAP_H       13

#define SPACE           ' '
#define WALL            '#'
#define HERO            '@'
#define BOX             '$'
#define STAR            '.'
#define BOX_ON_STAR     '*'
#define HERO_ON_STAR    '!'

// 全局变量
char map[MAP_H][MAP_W] = {{0}};         // 地图
int hero[2] = {0};                      // 人的行列坐标分别是 hero[0], hero[1]
#define hero_y hero[0]
#define hero_x hero[1]

char level_name[256];                   // 关卡名
char file_name[256];                    // 关卡文件
int move_cnt = 0;                       // 移动的次数
int push_cnt = 0;                       // 推箱子的次数
int misplaced_box_cnt = 0;              // 未归位的箱子数

// 函数

void map_print()
{
    for (int i = 0; i < MAP_H; ++i, putchar('\n'))
        for (int j = 0; j < MAP_W; ++j)
            switch (map[i][j]) {
                case HERO: case HERO_ON_STAR:
                    color_print(FG_CYAN, "囧"); break;
                case WALL: color_print(FG_WHITE, SYM_BLOCK);  break;
                case BOX:  color_print(FG_WHITE, "田"); break;
                case BOX_ON_STAR: color_print(FG_YELLOW, "田"); break;
                case STAR: color_print(FG_YELLOW, SYM_STAR);  break;
                default: printf("  ");
            }
    color_print(FG_WHITE,
        "\n\n"
        "    关卡 %s    移动%3d   推箱%3d\n\n"
        "                     R - 重新来过\n"
        "                 <ESC> - 回主画面\n",
        level_name, move_cnt, push_cnt
    );
    cursor_goto(0, 0);
}

void scene_welcome()
{
    screen_clear();

    set_color(FG_BLUE);
    char *buf[] = {
        "   #  #  #    #     #      #####",
        " #######  ### ###         #",
        "   #  #  #   # #  # #         #",
        "   ######  ### ### #######",
        " ##  #  #      #   #  #       #",
        " ##  ####  ### ###       #",
        "   #  ####    #   ###     ##"
    };
    for (int i = 0; i < sizeof(buf)/sizeof(char*); ++i) {
        for (char *p = buf[i]; *p != '\0'; ++p) {
            if (*p == WALL)
                printf("田");
            else
                putchar(*p);
        }
        putchar('\n');
    }
    color_print(FG_WHITE, "\n\n\n"
        "                1  开始游戏\n\n"
        "                2  选择关卡\n\n"
        "                3  游戏帮助\n\n"
        "                4  退出游戏\n");
}

void scene_help()
{
    screen_clear();

    printf("\n\n");
    color_print(FG_CYAN,  "    囧    ");
    color_print(FG_WHITE, "    你\n\n");

    color_print(FG_WHITE, "    " SYM_BLOCK "    ");
    color_print(FG_WHITE, "    墙\n\n");

    color_print(FG_WHITE, "    田    ");
    color_print(FG_WHITE, "    箱子 你无法同时推动两个箱子！\n\n");

    color_print(FG_YELLOW,"    " SYM_STAR "    ");
    color_print(FG_WHITE, "    目标 将所有箱子推到目标上！\n\n");

    color_print(FG_YELLOW,"    田    ");
    color_print(FG_WHITE, "    就位的箱子\n\n");

    printf("    " SYM_ARROWS "  移动\n\n"
           "    R         重新来过\n\n"
           "    <ESC>     回主画面\n");

    cursor_goto(0, 0);
    getch();
}

bool level_load()
{
    FILE *fin = fopen(file_name, "r");
    if (!fin)
        return 0;

    memset(map, SPACE, sizeof(map));
    misplaced_box_cnt = 0;
    move_cnt = 0;
    push_cnt = 0;

    int ch, x = 0, y = 0;
    while ((ch = fgetc(fin)) != EOF) {
        switch (ch) {
            case '\n':
                x = 0;
                ++y;
                continue;
            case HERO: case HERO_ON_STAR:
                hero_y = y;
                hero_x = x;
                break;
            case STAR: case WALL: case BOX_ON_STAR:
                break;
            case BOX:
                ++misplaced_box_cnt;
                break;
            default:
                ch = SPACE;
        }
        map[y][x++] = ch;
    }
    fclose(fin);

    Sleep(255);
    screen_clear();
    map_print();
    return 1;
}

bool is_game_won()
{
    return misplaced_box_cnt == 0;
}

// 若要回到主画面, 则返回 1
bool control()
{
    char key = getch();
    if (key == 'u' || key == 'U') {         //  U - 撤消
        ;
    } else if (key == 'r' || key == 'R') {  //  R - 重新来过
        level_load();
#ifdef __MINGW32__
    } else if (key == 27) {                 //  <ESC> - 回主画面
        return 1;
    } else if (key == -32) {                // 方向键
        key = getch();
        if (key != 'H' && key != 'P' && key != 'K' && key != 'M')
            return 0;

        int dy = key == 'H' ? -1 : (key == 'P' ? 1 : 0);
        int dx = key == 'K' ? -1 : (key == 'M' ? 1 : 0);
#endif
#ifdef __linux__
    } else if (key == 27) {                 // 方向键
        key = getch();
        if (key != '[')
            return 1;
        key = getch();
        if (key != 'A' && key != 'B' && key != 'C' && key != 'D')
            return 0;

        int dy = key == 'A' ? -1 : (key == 'B' ? 1 : 0);
        int dx = key == 'D' ? -1 : (key == 'C' ? 1 : 0);
#endif
        int next_y = hero_y + dy;
        int next_x = hero_x + dx;

        bool hero_on_star = map[hero_y][hero_x] == HERO_ON_STAR;
        bool next_space = map[next_y][next_x] == SPACE;
        bool next_star = map[next_y][next_x] == STAR;
        bool push_space = map[next_y][next_x] == BOX;
        bool push_star = map[next_y][next_x] == BOX_ON_STAR;
        bool push_to_space = map[next_y + dy][next_x + dx] == SPACE;
        bool push_to_star = map[next_y + dy][next_x + dx] == STAR;
        bool update_hero = 0;

        if (next_space || next_star) {
            update_hero = 1;
        } else if ((push_space || push_star)
                && (push_to_space || push_to_star)) {
            update_hero = 1;
            map[next_y + dy][next_x + dx] = push_to_star ? BOX_ON_STAR : BOX;
            color_mvprint(
                (push_to_star ? FG_YELLOW : FG_WHITE),
                next_y + dy, 2*(next_x + dx),
                "田"
            );

            if (push_star)
                ++misplaced_box_cnt;
            if (push_to_star)
                --misplaced_box_cnt;
            color_mvprint(FG_WHITE, MAP_H + 2, 30, "%2d", ++push_cnt);
        }

        if (update_hero) {
            map[hero_y][hero_x] = hero_on_star ? STAR : SPACE;
            map[next_y][next_x] = next_star || push_star ? HERO_ON_STAR : HERO;
            color_mvprint(
                FG_YELLOW,
                hero_y, 2*hero_x,
                hero_on_star ? "★" : "  "
            );
            color_mvprint(
                FG_CYAN,
                next_y, 2*next_x,
                "囧"
            );
            hero_y += dy;
            hero_x += dx;
            color_mvprint(FG_WHITE, MAP_H + 2, 20, "%2d", ++move_cnt);
        }
    }
    return 0;
}

// max level number is 999
void play(const char *levelname)
{
    strcpy(level_name, levelname);
    // check if levelname is number
    bool is_number = 1;
    for (const char *p = levelname; *p != '\0'; ++p) {
        if (!isdigit(*p)) {
            is_number = 0;
            break;
        }
    }
    if (is_number) {
        int i = atoi(levelname);
        sprintf(file_name, "level/%03d.level", i);
        while (level_load()) {
            do {
                if (control())
                    return;
            } while (!is_game_won());
            sprintf(file_name, "level/%03d.level", ++i);
            sprintf(level_name, "%d", i);
        }
    } else {
        sprintf(file_name, "level/%s.level", levelname);
        if (level_load()) {
            do {
                if (control())
                    return;
            } while (!is_game_won());
        }
    }
}

void scene_level_select()
{
    screen_clear();
    printf("\n\n\n\n\n\n\n\n\t\t选择关卡: ");
    char buf[256];
    scanf("%s", buf);
    play(buf);
}

int main()
{
    game_init();
    screen_size(20, 48);
    bool is_game_over = false;
    while (!is_game_over) {
        scene_welcome();
        switch (getch()) {
            case '1': play("1"); break;
            case '2': scene_level_select();  break;
            case '3': scene_help(); break;
            case '4': is_game_over = true; break;
            default: continue;
        }
    }
    game_over();
    return 0;
}
