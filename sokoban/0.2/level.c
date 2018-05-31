#include "Sokoban.h"

//关卡1~14
void Lv1()
{
	int i;
	for(i=3; i<=14; i++)	Map[3][i] = Map[11][i] = WALL;
	for(i=4; i<=10; i++)	Map[i][3] = Map[i][14] = WALL;

	Hero_r = 7;
	Hero_c = 6;

	Map[7][8] = BOX;

	Map[7][10] = MapCopy[7][10] = STAR;

	Door_r = 5;
	Door_c = 11;
}

void Lv2()
{
	int i;
	for(i=4; i<=13; i++)	Map[2][i] = WALL;
	for(i=4; i<=9; i++) 	Map[11][i] = WALL;
	for(i=9; i<=13; i++)	Map[8][i] = WALL;
	for(i=3; i<=11; i++)	Map[i][4] = WALL;
	for(i=8; i<=11; i++)	Map[i][9] = WALL;
	for(i=2; i<=8; i++)		Map[i][13] = WALL;

	Hero_r = 9;
	Hero_c = 7;

	Map[6][6] = BOX;
	Map[7][9] = BOX;

	Map[4][6] = MapCopy[4][6] = STAR;
	Map[4][11] = MapCopy[4][11] = STAR;

	Door_r = 9;
	Door_c = 6;
}

void Lv3()
{
	int i;
	for(i=6; i<=9; i++)		Map[3][i] = WALL;
	for(i=9; i<=11; i++)	Map[8][i] = WALL;
	for(i=6; i<=9; i++)		Map[9][i] = WALL;
	for(i=3; i<=9; i++)		Map[i][6] = WALL;
	for(i=5; i<=8; i++)		Map[i][11] = WALL;

	Map[4][9] = WALL;
	Map[5][9] = WALL;
	Map[5][10] = WALL;

	Hero_r = 6;
	Hero_c = 8;

	Map[6][7] = BOX_ON_STAR;
	Map[7][9] = BOX;

	Map[4][8] = MapCopy[4][8] = STAR;
	MapCopy[6][7] = STAR;

	Door_r = 8;
	Door_c = 7;
}

void Lv4()
{
	int i;
	for(i=6; i<=9; i++)	    Map[3][i] = WALL;
	for(i=4; i<=6; i++)	    Map[4][i] = WALL;
	for(i=9; i<=12; i++)	Map[4][i] = WALL;
	for(i=4; i<=12; i++)	Map[8][i] = WALL;
	for(i=5; i<=8; i++)	    Map[i][4] = WALL;
	for(i=5; i<=8; i++)	    Map[i][12] = WALL;
	Map[6][6] = WALL;
	Map[6][9] = WALL;
	Map[7][9] = WALL;

	Hero_r = 7;
	Hero_c = 10;

	Map[6][10] = BOX;
	Map[5][10] = BOX;

	Map[7][6] = MapCopy[7][6] = STAR;
	Map[7][8] = MapCopy[7][8] = STAR;

	Door_r = 4;
	Door_c = 7;
}

void Lv5()
{
	int i;
	for(i=3; i<=14; i++)            Map[4][i] = (i!=9);
	for(i=8; i<=10; i++)            Map[5][i] = WALL;
	for(i=7; i<=14; i++)	        Map[8][i] = WALL;
	for(i=3; i<=7; i++)		        Map[9][i] = WALL;
	for(i=4; i<=9; i++)		        Map[i][3] = WALL;
	for(i=4; i<=8; i++)		        Map[i][14] = WALL;
	Map[6][12] = WALL;
	Map[7][7] = WALL;

	Hero_r = 6;
	Hero_c = 13;

	for(i=8; i<=10; i++)    Map[7][i] = MapCopy[7][i] = STAR;

	Map[6][5] = BOX;
	Map[6][6] = BOX;
	Map[7][5] = BOX;

	Door_r = 6;
	Door_c = 5;
}

void Lv6()
{
	int i;
	for(i=6; i<=9; i++)     Map[2][i] = WALL;
	for(i=10; i<=12; i++)	Map[6][i] = Map[9][i] = WALL;
	for(i=7; i<=10; i++)    Map[10][i] = WALL;
	for(i=2; i<=6; i++)     Map[i][6] = WALL;
	for(i=6; i<=10; i++)    Map[i][7] = WALL;
	for(i=6; i<=9; i++)     Map[i][12] = WALL;
	Map[3][9] = WALL;
	Map[3][10] = WALL;
	Map[4][10] = WALL;
	Map[5][10] = WALL;

	Hero_r = 4;
	Hero_c = 8;

	Map[6][8] = BOX;
	Map[5][9] = BOX;
	Map[7][9] = BOX;

	for(i=3; i<=5; i++)     Map[i][7] = MapCopy[i][7] = STAR;

	Door_r = 8;
	Door_c = 10;
}

void Lv7()
{
	int i;
	for(i=2; i<=5; i++)     Map[4][i] = WALL;
	for(i=5; i<=8; i++)		Map[i][2] = Map[i][16] = WALL;
	for(i=2; i<=16; i++)    Map[8][i] = WALL;
	for(i=5; i<=16; i++)    Map[5][i] = WALL;

	Hero_r=6;
	Hero_c=14;

	for(i=4; i<=12; i+=2)   Map[6][i] = BOX;

	for(i=4; i<=8; i++)     Map[7][i] = MapCopy[7][i] = STAR;

	Door_r=5;
	Door_c=3;
}

void Lv8()
{
	int i,j;
	for(i=5; i<=8; i++) 	Map[3][i] = WALL;
	for(i=4; i<=8; i++){	Map[6][i] = (i!=7);
							Map[9][i] = WALL;
	}
	for(i=3; i<=5; i++)		Map[i][5] = WALL;
	Map[7][4] = WALL;
	Map[8][4] = WALL;

	Hero_r = 4;
	Hero_c = 8;

	Map[5][7] = BOX;
	Map[7][7] = BOX;

	//对称处理
	for(i=3; i<=9; i++)
		for(j=4; j<=8; j++)
			Map[i][17-j] = Map[i][j];

	Map[7][8] = MapCopy[7][8] = STAR;
	Map[7][9] = MapCopy[7][9] = STAR;
	Map[8][8] = MapCopy[8][8] = STAR;
	Map[8][9] = MapCopy[8][9] = STAR;

	Door_r = 4;
	Door_c = 9;
}

void Lv9()
{
	int i,j;
	for(i=5; i<=8; i++)		Map[3][i] = WALL;
	for(i=4; i<=6; i++)		Map[i][5] = WALL;

	Hero_r = 4;
	Hero_c = 6;

	Map[5][8] = BOX;
	Map[6][7] = BOX;

	Map[5][7] = MapCopy[5][7] = STAR;
	Map[6][8] = MapCopy[6][8] = STAR;

	//对称处理
	for(i=3; i<=6; i++)
	{
		for(j=5; j<=8; j++)
		{
			Map[i][17-j] = Map[13-i][j] = Map[13-i][17-j] = Map[i][j];
			if(Map[i][j] == STAR)
				MapCopy[i][17-j] = MapCopy[13-i][j] = MapCopy[13-i][17-j] = MapCopy[i][j];
		}
	}

	Door_r=9;
	Door_c=11;
}

void Lv10()
{
	int i;
	for(i=6; i<=9; i++) 	Map[3][i] = WALL;
	for(i=2; i<=6; i++) 	Map[4][i] = WALL;
	for(i=9; i<=15; i++)	Map[5][i] = WALL;
	for(i=1; i<=14; i++)	Map[9][i] = (i!=8 && i!=9 && i!=11);
	for(i=7; i<=10; i++)	Map[10][i] = WALL;
	for(i=5; i<=9; i++) 	Map[i][15] = WALL;
	Map[4][9]=WALL;
	Map[5][2]=WALL;
	Map[6][1]=WALL;
	Map[6][2]=WALL;
	Map[6][4]=WALL;
	Map[6][5]=WALL;
	Map[6][9]=WALL;
	Map[7][1]=WALL;
	Map[7][7]=WALL;
	Map[8][1]=WALL;
	Map[8][10]=WALL;
	Map[8][11]=WALL;
	Map[8][12]=WALL;

	Hero_r=7;
	Hero_c=11;

	Map[7][3]=BOX;
	Map[7][5]=BOX;
	Map[7][6]=BOX;
	Map[7][8]=BOX;
	Map[5][8]=BOX;

	Map[6][7] = MapCopy[6][7] = STAR;
	Map[6][8] = MapCopy[6][8] = STAR;
	Map[6][12]= MapCopy[6][12] = STAR;
	Map[6][13]= MapCopy[6][13] = STAR;
	Map[6][14]= MapCopy[6][14] = STAR;

	Door_r=4;
	Door_c=8;
}

void Lv11()
{
	int i,j;
	for(i=3; i<=8; i++)		Map[1][i]=WALL;
	for(i=1; i<=3; i++) 	Map[2][i]=WALL;
	for(i=5; i<=7; i++)		Map[8][i]=WALL;
	for(i=4; i<=6; i++)		Map[11][i]=WALL;
	for(i=6; i<=8; i++) 	Map[12][i]=WALL;
	Map[3][2]=WALL;
	Map[3][3]=WALL;
	Map[4][3]=WALL;
	Map[4][4]=WALL;
	Map[5][4]=WALL;
	Map[5][5]=WALL;
	Map[5][7]=WALL;
	Map[6][5]=WALL;
	Map[7][5]=WALL;
	Map[9][4]=WALL;
	Map[9][5]=WALL;
	Map[10][4]=WALL;

	Hero_r=7;
	Hero_c=8;

	for(i=2; i<=5; i++)		Map[i][8]=BOX;
	Map[3][5]=BOX;
	Map[5][6]=BOX;
	Map[6][7]=BOX;
	Map[9][7]=BOX;
	Map[10][8]=BOX;

	Map[2][4] = MapCopy[2][4] = STAR;
	Map[2][7] = MapCopy[2][7] = STAR;
	Map[4][6] = MapCopy[4][6] = STAR;
	Map[4][7] = MapCopy[4][7] = STAR;
	Map[6][6] = MapCopy[6][6] = STAR;
	Map[10][5]= MapCopy[10][5] = STAR;
	Map[11][8]= MapCopy[11][8] = STAR;

	//对称处理
	for(i=1; i<=12; i++)
	{
		for(j=1; j<=7; j++)
		{
			Map[i][16-j] = Map[i][j];
			MapCopy[i][16-j] = MapCopy[i][j];
		}
	}

	Door_r=8;
	Door_c=8;
}

void Lv12()
{
	int i;
	for(i=4; i<=12; i++) 	Map[2][i] = WALL;
	for(i=7; i<=9; i++)		Map[5][i] = WALL;
	for(i=8; i<=10; i++)	Map[9][i] = WALL;
	for(i=5; i<=8; i++)		Map[10][i]= WALL;
	for(i=3; i<=5; i++)		Map[i][4] = Map[i][12] = WALL;
	for(i=5; i<=9; i++)		Map[i][5] = WALL;
	Map[6][11] = WALL;
	Map[6][12] = WALL;
	Map[7][10] = WALL;
	Map[7][11] = WALL;
	Map[8][10] = WALL;

	Hero_r = 5;
	Hero_c = 10;

	Map[4][6] = BOX;
	Map[6][8] = BOX;
	Map[7][8] = BOX;
	Map[8][8] = BOX;
	Map[7][7] = BOX;

	Map[3][5] = MapCopy[3][5] = STAR;
	Map[4][5] = MapCopy[4][5] = STAR;
	Map[3][7] = MapCopy[3][7] = STAR;
	Map[4][7] = MapCopy[4][7] = STAR;
	Map[4][8] = MapCopy[4][8] = STAR;

	Door_r = 6;
	Door_c = 7;
}

void Lv13()
{
	int i;
	for(i=5; i<=11; i++)	Map[3][i] = WALL;
	for(i=4; i<=9; i++) 	Map[9][i] = WALL;
	for(i=9; i<=13; i++)	Map[10][i]= WALL;
	for(i=4; i<=9; i++) 	Map[i][4] = WALL;
	for(i=6; i<=10; i++)	Map[i][13]= WALL;
	for(i=3; i<=6; i++) 	Map[i][11]= WALL;
	Map[4][5] = WALL;
	Map[6][9] = WALL;
	Map[6][12]= WALL;
	Map[7][6] = WALL;
	Map[8][8] = WALL;
	Map[8][9] = WALL;
	Map[8][11]= WALL;

	Hero_r = 7;
	Hero_c = 8;

	Map[5][7] = BOX_ON_STAR;
	Map[6][7] = BOX_ON_STAR;
	Map[7][7] = BOX;
	Map[7][9] = BOX;
	Map[7][10]= BOX;
	Map[5][9] = BOX;

	Map[6][8] = MapCopy[6][8] = STAR;
	Map[5][8] = MapCopy[5][8] = STAR;
	Map[5][10]= MapCopy[5][10]= STAR;
	Map[6][10]= MapCopy[6][10]= STAR;
	MapCopy[6][7] = STAR;
	MapCopy[5][7] = STAR;

	Door_r = 4;
	Door_c = 6;
}

void Lv14()
{
	int i;
	for(i=5; i<=8; i++)		Map[2][i] = WALL;
	for(i=3; i<=5; i++)		Map[3][i] = WALL;
	for(i=8; i<=11;i++)		Map[3][i] = WALL;
	for(i=3; i<=7; i++)		Map[i][3] = WALL;
	for(i=11;i<=13;i++)		Map[5][i] = WALL;
	for(i=5; i<=9; i++)		Map[i][13]= WALL;
	for(i=5; i<=8; i++)		Map[9][i] = WALL;
	for(i=11;i<=13;i++)		Map[9][i] = WALL;
	for(i=8; i<=11;i++)		Map[10][i]= WALL;
	Map[4][3] = WALL;
	Map[4][11]= WALL;
	Map[5][5] = WALL;
	Map[5][9] = WALL;
	Map[7][4] = WALL;
	Map[7][5] = WALL;
	Map[7][7] = WALL;
	Map[7][9] = WALL;
	Map[7][11]= WALL;
	Map[8][5] = WALL;

	Hero_r = 4;
	Hero_c = 7;

	for(i=5; i<=11; i++)
	{
		if(i==8)
			continue;
		Map[6][i] = BOX;
	}

	Map[5][6] = MapCopy[5][6] = STAR;
	Map[5][8] = MapCopy[5][8] = STAR;
	Map[5][10]= MapCopy[5][10]= STAR;
	Map[7][6] = MapCopy[7][6] = STAR;
	Map[7][8] = MapCopy[7][8] = STAR;
	Map[7][10]= MapCopy[7][10]= STAR;

	Door_r=9;
	Door_c=10;
}
