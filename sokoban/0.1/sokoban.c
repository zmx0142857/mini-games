#include "Sokoban.h"
//提示：建议使用宋体查看本文件。

//定义全局变量
int Map[MAP_H][MAP_W]     ={0};
int MapCopy[MAP_H][MAP_W] ={0};
int Hero[2]               ={0};
int Door[2]               ={0};
int nLevel                =0;
int nMove                 =0;
int nPush                 =0;
static int GateOpen       =0;   //大门是否一度打开?

//初始化1: 只清空地图副本
void InitializeCopy()
{
    int i,j;
    for(i=0; i<MAP_H; i++)
        for(j=0; j<MAP_W; j++)
            MapCopy[i][j] = 0;
}

//初始化2: 清空移动数、推箱子数与两张地图
void InitializeAll()
{
    nMove = 0;
    nPush = 0;

    int i,j;
    for(i=0; i<MAP_H; i++)
    {
        for(j=0; j<MAP_W; j++)
        {
            Map[i][j] = 0;
            MapCopy[i][j] = 0;
        }
    }
}

//主画面
void Welcome()
{
    TextColor(BRIGHT|BLUE);//蓝色

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
    printf("                                                辣眼睛版 - 1.0 -\n\n");

    TextColor(BRIGHT|RED|GREEN|BLUE);//白色

    printf("\t\t\t>>1  开始游戏\t\t\t\t\n\n");
    printf("\t\t\t>>2  选择关卡\t\t\t\t\n\n");
    printf("\t\t\t>>3  游戏帮助\t\t\t\t\n\n");
    printf("\t\t\t>>4  退出游戏\t\t\t\t\n\n");
}

//图例与操作
void Instruction()
{
    char key;

    while(1)
    {
        system("cls");

    //标题
        TextColor(BRIGHT|RED|GREEN|BLUE);

        printf("\t\t\t┌─────────┐\n");
        printf("\t\t\t│    图例与操作    │\n");
        printf("\t\t\t╰─────────╯\n\n");

    //你
        TextColor(BRIGHT|GREEN|BLUE);//青色

        printf("    囧        ");

        TextColor(BRIGHT|RED|GREEN|BLUE);

        printf("这就是你了。\n\n\n");

    //墙
        TextColor(BRIGHT);//灰色

        printf("    █        ");

        TextColor(BRIGHT|RED|GREEN|BLUE);

        printf("这是墙。\n\n\n");

    //箱子
        TextColor(BRIGHT);

        printf("    田        ");

        TextColor(BRIGHT|RED|GREEN|BLUE);

        printf("这是箱子。注意，你无法同时推动两个箱子。\n\n\n");

    //目标位置
        TextColor(BRIGHT|RED|GREEN);

        printf("    ★        ");

        TextColor(BRIGHT|RED|GREEN|BLUE);

        printf("箱子的目标位置。将所有箱子推到对应位置以解锁出口。\n\n\n");

    //已就位的箱子
        TextColor(BRIGHT|RED|GREEN);

        printf("    田        ");

        TextColor(BRIGHT|RED|GREEN|BLUE);

        printf("已经处于目标位置的箱子。\n\n\n");

    //出口
        TextColor(BRIGHT|RED|BLUE);//紫红

        printf("    П        ");

        TextColor(BRIGHT|RED|GREEN|BLUE);

        printf("出口。\n\n\n");

    //操作指南
        printf(" ↑←↓→     移动。\n\n\n");
        printf("    R         重新来过。\n\n\n");
        printf("  Ctrl+C      返回主画面。\n");

        key = getch();
        if(key == 3)
        {
            system("cls");
            return;
        }
    }
}

//选关
void SelectLevel()
{
    int level;
    int GoToMain=0;

    while(1)
    {
        system("cls");

        printf("\n\n");
        printf("      ┌─┐  ┌─┐  ┌─┐  ┌─┐  ┌─┐  ┌─┐\n");
        printf("      │01│  │02│  │03│  │04│  │05│  │06│\n");
        printf("      ╰─╯  ╰─╯  ╰─╯  ╰─╯  ╰─╯  ╰─╯\n\n");
        printf("      ┌─┐  ┌─┐  ┌─┐  ┌─┐  ┌─┐  ┌─┐\n");
        printf("      │07│  │08│  │09│  │10│  │11│  │12│\n");
        printf("      ╰─╯  ╰─╯  ╰─╯  ╰─╯  ╰─╯  ╰─╯\n\n");
        printf("      ┌─┐  ┌─┐   \n");
        printf("      │13│  │14│   \n");
        printf("      ╰─╯  ╰─╯   \n\n");
        printf("                         请选择关卡: ");
        fflush(stdin);                  //清空缓冲区
        scanf("%d",&level);
        system("cls");
        for(;level<=MAX_LEVEL; level++)
        {
            GoToLevel(level);

            GoToMain = Control();
            if(GoToMain)
            {
                system("cls");
                return;
            }
        }
    }
}

void GoToLevel(int n)
{
    system("cls");
    InitializeAll();

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
    }

	nLevel = n;
    Map[Hero_x][Hero_y] = HERO;
    MapCopy[Door_x][Door_y] = DOOR;
	GateOpen = 0;
    PrintBoard();
}

//游戏流程控制
void Play()
{
    int n;
    int GoToMain=0;
    for(n=1; n<=MAX_LEVEL; n++)
    {
        GoToLevel(n);

        GoToMain = Control();
        if(GoToMain)
        {
            system("cls");
            return;
        }
    }
}

//键盘控制
int Control()
{
    int win;
    int moved;                          //若已经移动过一次, moved=1
    char key;

    while(1)
    {
        key = getch();

        if(key == 'U' || key == 'u')    // U - 撤消
        {
        }

        if(key == 'R' || key == 'r')    // R - 重新来过
        {
            GoToLevel(nLevel);
        }

        if(key == 3)                    // Ctrl+C - 返回主画面
        {
            return 1;
        }

        if(key == -32)
        {
            moved = 0;
            key = getch();

        //上移
            if	(	!moved
				&&  key == 'H'
				&&  (	Map[Hero_x-1][Hero_y] == NOTHING
					||  Map[Hero_x-1][Hero_y] == STAR
					||  Map[Hero_x-1][Hero_y] == DOOR
					)
				)
            {
                Map[Hero_x][Hero_y] = NOTHING;
                Map[Hero_x-1][Hero_y] = HERO;
                Hero_x--;
                nMove++;
                moved=1;
            }

        //上移推箱
            if	(	!moved
				&&  key == 'H'
				&&  ( Map[Hero_x-1][Hero_y] == BOX || Map[Hero_x-1][Hero_y] == BOX_IN_PLACE )
				&&  Map[Hero_x-2][Hero_y] == NOTHING
				)
            {
                Map[Hero_x][Hero_y] = NOTHING;
                Map[Hero_x-1][Hero_y] = HERO;
                Map[Hero_x-2][Hero_y] = BOX;
                Hero_x--;
                nMove++;
                nPush++;
                moved=1;
            }

        //上移推箱到位
            if	(   !moved
                &&  key == 'H'
                &&  ( Map[Hero_x-1][Hero_y] == BOX || Map[Hero_x-1][Hero_y] == BOX_IN_PLACE )
                &&  Map[Hero_x-2][Hero_y] == STAR
				)
            {
                Map[Hero_x][Hero_y] = NOTHING;
                Map[Hero_x-1][Hero_y] = HERO;
                Map[Hero_x-2][Hero_y] = BOX_IN_PLACE;
                Hero_x--;
                nMove++;
                nPush++;
                moved=1;
            }

        //下移
            if	(	!moved
				&&	key == 'P'
				&&  (	Map[Hero_x+1][Hero_y] == NOTHING
					||	Map[Hero_x+1][Hero_y] == STAR
					||  Map[Hero_x+1][Hero_y] == DOOR
					)
				)
            {
                Map[Hero_x][Hero_y] = NOTHING;
                Map[Hero_x+1][Hero_y] = HERO;
                Hero_x++;
                nMove++;
				moved=1;
            }

        //下移推箱
            if	(	!moved
				&&	key == 'P'
				&&	( Map[Hero_x+1][Hero_y] == BOX || Map[Hero_x+1][Hero_y] == BOX_IN_PLACE )
				&&  Map[Hero_x+2][Hero_y] == NOTHING
				)
            {
                Map[Hero_x][Hero_y] = NOTHING;
                Map[Hero_x+1][Hero_y] = HERO;
                Map[Hero_x+2][Hero_y] = BOX;
                Hero_x++;
                nMove++;
                nPush++;
				moved=1;
            }

        //下移推箱到位
            if	(   !moved
                &&  key == 'P'
                &&  ( Map[Hero_x+1][Hero_y] == BOX || Map[Hero_x+1][Hero_y] == BOX_IN_PLACE )
                &&  Map[Hero_x+2][Hero_y] == STAR
                )
            {
                Map[Hero_x][Hero_y] = NOTHING;
                Map[Hero_x+1][Hero_y] = HERO;
                Map[Hero_x+2][Hero_y] = BOX_IN_PLACE;
                Hero_x++;
                nMove++;
                nPush++;
                moved=1;
            }

        //左移
            if	(	!moved
				&&  key == 'K'
				&&  (	Map[Hero_x][Hero_y-1] == 0
					||	Map[Hero_x][Hero_y-1] == STAR
					||	Map[Hero_x][Hero_y-1] == DOOR
					)
				)
            {
                Map[Hero_x][Hero_y]=NOTHING;
                Map[Hero_x][Hero_y-1] = HERO;
                Hero_y--;
                nMove++;
				moved=1;
            }

        //左移推箱
            if	(	!moved
				&&  key == 'K'
				&&  ( Map[Hero_x][Hero_y-1] == BOX || Map[Hero_x][Hero_y-1] == BOX_IN_PLACE )
				&&  Map[Hero_x][Hero_y-2] == NOTHING
				)
            {
                Map[Hero_x][Hero_y] = NOTHING;
                Map[Hero_x][Hero_y-1] = HERO;
                Map[Hero_x][Hero_y-2] = BOX;
                Hero_y--;
                nMove++;
                nPush++;
				moved=1;
            }

        //左移推箱到位
            if	(	!moved
				&&	key == 'K'
				&&  ( Map[Hero_x][Hero_y-1] == BOX || Map[Hero_x][Hero_y-1] == BOX_IN_PLACE )
				&&  Map[Hero_x][Hero_y-2] == STAR
				)
            {
                Map[Hero_x][Hero_y] = NOTHING;
                Map[Hero_x][Hero_y-1] = HERO;
                Map[Hero_x][Hero_y-2] = BOX_IN_PLACE;
                Hero_y--;
                nMove++;
                nPush++;
                moved=1;
            }

        //右移
            if	(	!moved
				&&  key == 'M'
				&& 	(	Map[Hero_x][Hero_y+1] == NOTHING
					||	Map[Hero_x][Hero_y+1] == STAR
					||	Map[Hero_x][Hero_y+1] == DOOR
					)
				)
            {
                Map[Hero_x][Hero_y+1] = HERO;
                Map[Hero_x][Hero_y] = NOTHING;
                Hero_y++;
                nMove++;
				moved=1;
            }

        //右移推箱
            if	(	!moved
				&&	key == 'M'
				&&	( Map[Hero_x][Hero_y+1] == BOX || Map[Hero_x][Hero_y+1] == BOX_IN_PLACE )
				&&  Map[Hero_x][Hero_y+2] == NOTHING
				)
            {
                Map[Hero_x][Hero_y+1] = HERO;
                Map[Hero_x][Hero_y] = NOTHING;
                Map[Hero_x][Hero_y+2] = BOX;
                Hero_y++;
                nMove++;
                nPush++;
				moved=1;
            }

        //右移推箱到位
            if	(	!moved
				&&	key == 'M'
				&&  ( Map[Hero_x][Hero_y+1] == BOX || Map[Hero_x][Hero_y+1] == BOX_IN_PLACE )
				&&  Map[Hero_x][Hero_y+2] == STAR
				)
            {
                Map[Hero_x][Hero_y] = NOTHING;
                Map[Hero_x][Hero_y+1] = HERO;
                Map[Hero_x][Hero_y+2] = BOX_IN_PLACE;
                Hero_y++;
                nMove++;
                nPush++;
                moved=1;
            }
        }

        win=IsFinish();
        system("cls");
        PrintBoard();

        if(win)
        {
            Sleep(225);                     //把程序挂起255ms
            return 0;
        }
    }
    return 0;
}

//打印
void PrintBoard()
{
    int i,j;
    for(i=0; i<MAP_H; i++)
        for(j=0; j<MAP_W; j++)
            if( MapCopy[i][j] == STAR && Map[i][j] == NOTHING )
                Map[i][j] = STAR;		//MapCopy的真正用意：恢复被抹去的记号

    for(i=0; i<MAP_H; i++)
    {
		for(j=0; j<MAP_W; j++)
        {
            switch(Map[i][j])
            {
                case HERO:
                    TextColor(BRIGHT|GREEN|BLUE);
                    printf("囧");
                    break;

                case WALL:
                    TextColor(BRIGHT);
                    printf("█");
                    break;

                case BOX:
                    TextColor(BRIGHT);
                    printf("田");
                    break;

                case STAR:
                    TextColor(BRIGHT|RED|GREEN);
                    printf("★");
                    break;

                case BOX_IN_PLACE:
                    TextColor(BRIGHT|RED|GREEN);
                    printf("田");
                    break;

                case DOOR:
                    TextColor(BRIGHT|RED|BLUE);
                    printf("П");
                    break;

                default:
                    TextColor(BRIGHT|RED|GREEN|BLUE);
                    printf("  ");
            }
        }
    printf("\n");
    }

    TextColor(BRIGHT|RED|GREEN|BLUE);
    printf("\n\n");
    printf("    关卡%3d    移动%3d   推箱%3d\n\n",nLevel,nMove,nPush);
    printf("                     R - 重新来过\n");
    printf("                Ctrl+C - 回主画面\n");
}

//过关否?
int IsFinish()
{
    int i,j;

    if(!GateOpen)                       //大门未开时……
    {                                   //图中有未就位的箱子 - 未获胜
        for(i=0; i<MAP_H; i++)
            for(j=0; j<MAP_W; j++)
                if(Map[i][j] == BOX)
                    return 0;
                                        //否则 - 大门打开
        Map[Door_x][Door_y] = DOOR;
        GateOpen=1;
    }

    if(Map[Door_x][Door_y] != HERO)     //大门已开，但人未到 - 未获胜
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
