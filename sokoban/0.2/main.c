#include "Sokoban.h"

// 提示：建议使用宋体查看本文件。

// 全局变量
Ukulele Map[MAP_H][MAP_W]     = {{0}};
Ukulele MapCopy[MAP_H][MAP_W] = {{0}};
Ukulele Hero[2]               = {0};
Ukulele Door[2]               = {0};
Ukulele nLevel                = 0;
Ukulele nMove                 = 0;
Ukulele nPush                 = 0;
BOOL gate_open       	  = 0;
BOOL game_over            = 0;

int main()
{
    while (1)
    {
        Scene_Welcome();

        switch (getch())
        {
            case '1':   Play(1);              break;
            case '2':   Scene_SelectLevel();  break;
            case '3':   Scene_Instruction();  break;
            case '4':   return 0;
            default :   system("cls");
        }
    }
    return 0;
}

void Init()
{
	zero_mem(Map, sizeof(Map));
	zero_mem(MapCopy, sizeof(MapCopy));
    nMove = 0;
    nPush = 0;
    gate_open = 0;
    game_over = 0;
}

void Scene_Welcome()
{
    // screen_sz(32, 70);
    system("mode con lines=32 cols=70");
    set_color(x000000, x0000ff); // 蓝色
    system("cls");

    printf("\n\n\n\n");
    printf("        ┏┓    ■ ■       ■      ■        ■■■■■■■    \n");
    printf("        ┃┃   ■  ■      ■■■  ■■■■             ■      \n");
    printf("    ┏━┛┗┓■■■■■  ■ ■  ■ ■                ■        \n");
    printf("    ┗━┓┏■■  ■        ■  ┏━━━┓           ■         \n");
    printf("        ┃┗┓■■■■  ■■■■┃┏━┓┃ ■■■■■■■■■   \n");
    printf("    ┏━┛┏┛■  ■        ■  ┃┏━┓┃           ■         \n");
    printf("    ┗━┓┃  ■■■■    ■■■┃┏━┓┃           ■         \n");
    printf("      ┏┛┃  ■  ■     ■ ■  ┃┗━┛┃           ■         \n");
    printf("      ┗━┛  ■■■■■    ■  ┗━━━┛       ■■■         \n");
    printf("                                                                \n");
    printf("                                                辣眼睛版 - 0.2 -\n\n");

    set_color(x000000, xffffff); // 白色
    printf("\t\t\t>>1  开始游戏\t\t\t\t\n\n");
    printf("\t\t\t>>2  选择关卡\t\t\t\t\n\n");
    printf("\t\t\t>>3  游戏帮助\t\t\t\t\n\n");
    printf("\t\t\t>>4  退出游戏\t\t\t\t\n\n");
}

void Scene_Instruction()
{
    system("cls");

    set_color(x000000, xffffff);
    printf("\t\t\t┌─────────┐\n");
    printf("\t\t\t│    图例与操作    │\n");
    printf("\t\t\t╰─────────╯\n\n");

    set_color(x000000, x00ffff); // 蓝绿色
    printf("    囧        ");

    set_color(x000000, xffffff);
    printf("这就是你了。\n\n\n");

    set_color(x000000, x808080); // 灰色
    printf("    █        ");

    set_color(x000000, xffffff);
    printf("这是墙。\n\n\n");

    set_color(x000000, x808080);
    printf("    田        ");

    set_color(x000000, xffffff);
    printf("这是箱子。注意，你无法同时推动两个箱子。\n\n\n");

    set_color(x000000, xffff00);
    printf("    ★        ");

    set_color(x000000, xffffff);
    printf("箱子的目标位置。将所有箱子推到对应位置以解锁出口。\n\n\n");

    set_color(x000000, xffff00);
    printf("    田        ");

    set_color(x000000, xffffff);
    printf("已经处于目标位置的箱子。\n\n\n");

    set_color(x000000, xff00ff); // 紫红
    printf("    П        ");

    set_color(x000000, xffffff);
    printf("出口。\n\n\n");

    printf(" ↑←↓→     移动。\n\n\n");
    printf("    R         重新来过。\n\n\n");
    printf("  Ctrl+C      返回主画面。\n");

    getch();
}

void Scene_SelectLevel()
{
    system("cls");

    printf("\n\n");
    printf("\t┌─┐  ┌─┐  ┌─┐  ┌─┐  ┌─┐  ┌─┐\n");
    printf("\t│01│  │02│  │03│  │04│  │05│  │06│\n");
    printf("\t╰─╯  ╰─╯  ╰─╯  ╰─╯  ╰─╯  ╰─╯\n\n");
    printf("\t┌─┐  ┌─┐  ┌─┐  ┌─┐  ┌─┐  ┌─┐\n");
    printf("\t│07│  │08│  │09│  │10│  │11│  │12│\n");
    printf("\t╰─╯  ╰─╯  ╰─╯  ╰─╯  ╰─╯  ╰─╯\n\n");
    printf("\t┌─┐  ┌─┐   \n");
    printf("\t│13│  │14│   \n");
    printf("\t╰─╯  ╰─╯   \n\n");

    printf("\t\t\t请选择关卡: ");

    fflush(stdin);                  // 清空缓冲区
    int i;
    scanf("%d", &i);
    Play(i);
}

void LoadLevel(int n)
{
    Init();

    // read level from file!!!-----------------------------------------------------------
    switch(n)
    {
        case 1: Lv1();    break;
        case 2: Lv2();    break;
        case 3: Lv3();    break;
        case 4: Lv4();    break;
        case 5: Lv5();    break;
        case 6: Lv6();    break;
        case 7: Lv7();    break;
        case 8: Lv8();    break;
        case 9: Lv9();    break;
        case 10:Lv10();   break;
        case 11:Lv11();   break;
        case 12:Lv12();   break;
        case 13:Lv13();   break;
        case 14:Lv14();   break;
        default:return;
    }

    nLevel = n;
    Map[Hero_r][Hero_c] = HERO;
    MapCopy[Door_r][Door_c] = DOOR;
    PrintBoard();
}

void Play(int i)
{
    while (i <= MAX_LEVEL)
    {
        LoadLevel(i);
        while (!game_over)
        {
            if (Control())
                return;
        }
        Sleep(255);
        ++i;
    }
}

// 若要回到主画面, 则返回1
BOOL Control()
{
    // 读键盘
    char key = getch();

    if (key == 'u' || key == 'U')         //  U - 撤消
    {
    }

    else if (key == 'r' || key == 'R')    //  R - 重新来过
    {
        LoadLevel(nLevel);
    }

    else if (key == 3)                    //  Ctrl+C - 返回主画面
    {
        return 1;
    }

    else if (key == -32)
    {
        key = getch();

        if (key != 'H' && key != 'P' && key != 'K' && key != 'M')
            return 0;

        int dr = key == 'H' ? -1 : (key == 'P' ? 1 : 0);
        int dc = key == 'K' ? -1 : (key == 'M' ? 1 : 0);
        int next_r = Hero_r + dr;
        int next_c = Hero_c + dc;

        BOOL move = Map[next_r][next_c] == SPACE
                 || Map[next_r][next_c] == STAR
                 || Map[next_r][next_c] == DOOR;

        BOOL push = Map[next_r][next_c] == BOX
                 ||	Map[next_r][next_c] == BOX_ON_STAR;

        BOOL has_space = Map[next_r + dr][next_c + dc] == SPACE;
        BOOL has_star = Map[next_r + dr][next_c + dc] == STAR;
        BOOL update_hero = 0;

        if (move)
        {
            update_hero = 1;
        }
        else if (push && (has_space || has_star))
        {
            update_hero = 1;
            Map[next_r + dr][next_c + dc] = has_star ? BOX_ON_STAR : BOX;

            print_on_color("田", next_r + dr, 2*(next_c + dc),
                           x000000, has_star ? xffff00 : x808080);
            ++nPush;
        }

        if (update_hero)
        {
            // MapCopy的真正用意：恢复被抹去的星记号
            BOOL star = MapCopy[Hero_r][Hero_c] == STAR;
            Map[Hero_r][Hero_c] = star ? STAR : SPACE;
            Map[next_r][next_c] = HERO;

            print_on_color(star ? "★" : "  ", Hero_r, 2*Hero_c,
                           x000000, xffff00);
            print_on_color("囧", next_r, 2*next_c,
                           x000000, x00ffff);
            Hero_r += dr;
            Hero_c += dc;
            ++nMove;
        }
    }

    // 判断获胜
    game_over = LvWon();

    // 更新移动与推箱次数
    set_color(x000000, xffffff);
    set_cursor(MAP_H + 2, 20);
    printf("%2d", nMove);
    set_cursor(MAP_H + 2, 30);
    printf("%2d", nPush);
    set_cursor(0, 0);

    return 0;
}

void PrintBoard()
{
    // screen_sz(20, 35);
    system("mode con lines=20 cols=35");
    system("cls");

    int i, j;
    for (i = 0; i < MAP_H; ++i, printf("\n"))
        for (j = 0; j < MAP_W; ++j)
            switch (Map[i][j])
            {
                case HERO: set_color(x000000, x00ffff); printf("囧"); break;
                case WALL: set_color(x000000, x808080); printf("█");  break;
                case BOX:  set_color(x000000, x808080); printf("田"); break;
                case BOX_ON_STAR:
						   set_color(x000000, xffff00); printf("田"); break;
                case STAR: set_color(x000000, xffff00); printf("★");  break;
                default:   printf("  ");
            }

    set_color(x000000, xffffff);
    printf("\n\n");
    printf("    关卡%3d    移动%3d   推箱%3d\n\n", nLevel, nMove, nPush);
    printf("                     R - 重新来过\n");
    printf("                Ctrl+C - 回主画面\n");
}

BOOL LvWon()
{
    if (!gate_open)
    {
		// 还有未就位的箱子吗
		int i, j;
        for (i = 0; i < MAP_H; ++i)
            for (j = 0; j < MAP_W; ++j)
                if (Map[i][j] == BOX)
                    return 0;

        Map[Door_r][Door_c] = DOOR;
        print_on_color("П", Door_r, 2*Door_c, x000000, xff00ff);
        gate_open = 1;
    }

    return Map[Door_r][Door_c] == HERO;
}
