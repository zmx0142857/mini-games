#include "Sokoban.h"
//��ʾ������ʹ������鿴���ļ���

//����ȫ�ֱ���
int Map[MAP_H][MAP_W]     ={0};
int MapCopy[MAP_H][MAP_W] ={0};
int Hero[2]               ={0};
int Door[2]               ={0};
int nLevel                =0;
int nMove                 =0;
int nPush                 =0;
static int GateOpen       =0;   //�����Ƿ�һ�ȴ�?

//��ʼ��1: ֻ��յ�ͼ����
void InitializeCopy()
{
    int i,j;
    for(i=0; i<MAP_H; i++)
        for(j=0; j<MAP_W; j++)
            MapCopy[i][j] = 0;
}

//��ʼ��2: ����ƶ������������������ŵ�ͼ
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

//������
void Welcome()
{
    TextColor(BRIGHT|BLUE);//��ɫ

    printf("\n\n\n\n");
    printf("        ����    �� ��       ��      ��        ��������������    \n");
    printf("        ����   ��  ��      ������  ��������             ��      \n");
    printf("    ��������������������  �� ��  �� ��                ��        \n");
    printf("    ������������  ��        ��  ����������           ��         \n");
    printf("        ��������������  ������������������ ������������������   \n");
    printf("    ������������  ��        ��  ����������           ��         \n");
    printf("    ��������  ��������    ����������������           ��         \n");
    printf("      ������  ��  ��     �� ��  ����������           ��         \n");
    printf("      ������  ����������    ��  ����������       ������         \n");
    printf("                                                                \n");
    printf("                                                ���۾��� - 1.0 -\n\n");

    TextColor(BRIGHT|RED|GREEN|BLUE);//��ɫ

    printf("\t\t\t>>1  ��ʼ��Ϸ\t\t\t\t\n\n");
    printf("\t\t\t>>2  ѡ��ؿ�\t\t\t\t\n\n");
    printf("\t\t\t>>3  ��Ϸ����\t\t\t\t\n\n");
    printf("\t\t\t>>4  �˳���Ϸ\t\t\t\t\n\n");
}

//ͼ�������
void Instruction()
{
    char key;

    while(1)
    {
        system("cls");

    //����
        TextColor(BRIGHT|RED|GREEN|BLUE);

        printf("\t\t\t����������������������\n");
        printf("\t\t\t��    ͼ�������    ��\n");
        printf("\t\t\t�t�������������������s\n\n");

    //��
        TextColor(BRIGHT|GREEN|BLUE);//��ɫ

        printf("    ��        ");

        TextColor(BRIGHT|RED|GREEN|BLUE);

        printf("��������ˡ�\n\n\n");

    //ǽ
        TextColor(BRIGHT);//��ɫ

        printf("    ��        ");

        TextColor(BRIGHT|RED|GREEN|BLUE);

        printf("����ǽ��\n\n\n");

    //����
        TextColor(BRIGHT);

        printf("    ��        ");

        TextColor(BRIGHT|RED|GREEN|BLUE);

        printf("�������ӡ�ע�⣬���޷�ͬʱ�ƶ��������ӡ�\n\n\n");

    //Ŀ��λ��
        TextColor(BRIGHT|RED|GREEN);

        printf("    ��        ");

        TextColor(BRIGHT|RED|GREEN|BLUE);

        printf("���ӵ�Ŀ��λ�á������������Ƶ���Ӧλ���Խ������ڡ�\n\n\n");

    //�Ѿ�λ������
        TextColor(BRIGHT|RED|GREEN);

        printf("    ��        ");

        TextColor(BRIGHT|RED|GREEN|BLUE);

        printf("�Ѿ�����Ŀ��λ�õ����ӡ�\n\n\n");

    //����
        TextColor(BRIGHT|RED|BLUE);//�Ϻ�

        printf("    ��        ");

        TextColor(BRIGHT|RED|GREEN|BLUE);

        printf("���ڡ�\n\n\n");

    //����ָ��
        printf(" ��������     �ƶ���\n\n\n");
        printf("    R         ����������\n\n\n");
        printf("  Ctrl+C      ���������档\n");

        key = getch();
        if(key == 3)
        {
            system("cls");
            return;
        }
    }
}

//ѡ��
void SelectLevel()
{
    int level;
    int GoToMain=0;

    while(1)
    {
        system("cls");

        printf("\n\n");
        printf("      ������  ������  ������  ������  ������  ������\n");
        printf("      ��01��  ��02��  ��03��  ��04��  ��05��  ��06��\n");
        printf("      �t���s  �t���s  �t���s  �t���s  �t���s  �t���s\n\n");
        printf("      ������  ������  ������  ������  ������  ������\n");
        printf("      ��07��  ��08��  ��09��  ��10��  ��11��  ��12��\n");
        printf("      �t���s  �t���s  �t���s  �t���s  �t���s  �t���s\n\n");
        printf("      ������  ������   \n");
        printf("      ��13��  ��14��   \n");
        printf("      �t���s  �t���s   \n\n");
        printf("                         ��ѡ��ؿ�: ");
        fflush(stdin);                  //��ջ�����
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

//��Ϸ���̿���
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

//���̿���
int Control()
{
    int win;
    int moved;                          //���Ѿ��ƶ���һ��, moved=1
    char key;

    while(1)
    {
        key = getch();

        if(key == 'U' || key == 'u')    // U - ����
        {
        }

        if(key == 'R' || key == 'r')    // R - ��������
        {
            GoToLevel(nLevel);
        }

        if(key == 3)                    // Ctrl+C - ����������
        {
            return 1;
        }

        if(key == -32)
        {
            moved = 0;
            key = getch();

        //����
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

        //��������
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

        //�������䵽λ
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

        //����
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

        //��������
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

        //�������䵽λ
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

        //����
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

        //��������
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

        //�������䵽λ
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

        //����
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

        //��������
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

        //�������䵽λ
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
            Sleep(225);                     //�ѳ������255ms
            return 0;
        }
    }
    return 0;
}

//��ӡ
void PrintBoard()
{
    int i,j;
    for(i=0; i<MAP_H; i++)
        for(j=0; j<MAP_W; j++)
            if( MapCopy[i][j] == STAR && Map[i][j] == NOTHING )
                Map[i][j] = STAR;		//MapCopy���������⣺�ָ���Ĩȥ�ļǺ�

    for(i=0; i<MAP_H; i++)
    {
		for(j=0; j<MAP_W; j++)
        {
            switch(Map[i][j])
            {
                case HERO:
                    TextColor(BRIGHT|GREEN|BLUE);
                    printf("��");
                    break;

                case WALL:
                    TextColor(BRIGHT);
                    printf("��");
                    break;

                case BOX:
                    TextColor(BRIGHT);
                    printf("��");
                    break;

                case STAR:
                    TextColor(BRIGHT|RED|GREEN);
                    printf("��");
                    break;

                case BOX_IN_PLACE:
                    TextColor(BRIGHT|RED|GREEN);
                    printf("��");
                    break;

                case DOOR:
                    TextColor(BRIGHT|RED|BLUE);
                    printf("��");
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
    printf("    �ؿ�%3d    �ƶ�%3d   ����%3d\n\n",nLevel,nMove,nPush);
    printf("                     R - ��������\n");
    printf("                Ctrl+C - ��������\n");
}

//���ط�?
int IsFinish()
{
    int i,j;

    if(!GateOpen)                       //����δ��ʱ����
    {                                   //ͼ����δ��λ������ - δ��ʤ
        for(i=0; i<MAP_H; i++)
            for(j=0; j<MAP_W; j++)
                if(Map[i][j] == BOX)
                    return 0;
                                        //���� - ���Ŵ�
        Map[Door_x][Door_y] = DOOR;
        GateOpen=1;
    }

    if(Map[Door_x][Door_y] != HERO)     //�����ѿ�������δ�� - δ��ʤ
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
