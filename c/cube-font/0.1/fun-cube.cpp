/*
    Name:       Funcube_1.0
    Author:     Zhuo Mingxin
    Version:    1.0
    Copyright:
    Link:
    Description:Input 2D graphics, output 3D.
                                                  _____
                                                 /\    \
                                                /::\____\
                                               /:::/    /
                                              /:::/    /
                                             /:::/    /
                  00......00                /:::/____/
                  00......00               /::::\    \
                  00......00              /::::::\    \   _____
                  00......00             /:::/\:::\    \ /\    \
                  0000000000      ->    /:::/  \:::\    /::\____\
                  0000000000            \::/    \:::\  /:::/    /
                  00......00             \/____/ \:::\/:::/    /
                  00......00                      \::::::/    /
                  00......00                       \::::/    /
                  00......00                       /:::/    /
                                                  /:::/    /
                                                 /:::/    /
                                                /:::/    /
                                                \::/    /
                                                 \/____/




                                              _____________
                 .../000\..                  /::::::::\    \
                 ../00000\.                 /::::::::::\    \
                 ./0000000\                /:::/~~\:::::\____\
                 /0/...\000               /::::\~~ \:::::|    |
                 00.....000              /::::::\   \::::|    |
                 0000000000      ->     /:::/\:::\   |:::|    |
                 0000000000             \::/  \:::\  |:::|    |
                 00.....000              \/____\:::\/::::|    |
                 00.....000                     \::::::::|____|
                 00.....000                      \::::::/    /
                                                 /:::::/    /
                                                /:::::/    /
                                               /:::::/    /
                                               \::::/    /
                                                \::/    /
                                                 \/____/
00......00
00......00
00......00
00......00
0000000000
0000000000
00......00
00......00
00......00
00......00

.../000\..
../00000\.
./0000000\
/0/...\000
00.....000
0000000000
0000000000
00.....000
00.....000
00.....000

00000000\.
000000000.
00....\00.
00..../0/.
00000000\.
000000000\
00.....\00
00...../00
000000000/
00000000/.
*/
#include<iostream>
#include<stdlib.h>
using namespace std;

#define iHEIGHT 10
#define iWIDTH 10
#define THICKNESS 5
#define oHEIGHT iHEIGHT+iWIDTH+2
#define oWIDTH iHEIGHT+iWIDTH+THICKNESS

char input[iHEIGHT][iWIDTH]={'\0'};
char output[oHEIGHT][oWIDTH]={'\0'};
char unit[5][2][THICKNESS+3]={'\0'};
bool hasup,hasdown,hasleft,hasright;    //hasleft=false 意味着左一格是'.'或没有左一格了
bool isconcave;                         //isconcave=true意味着此处是凹形，要画底线
/*
choice      0           1             2            3      4

            default                   default             default

write       :\    \     ::            _______      |:     :|    |
            :/    /     ~~~~~~~       ::\    \     |:     :|    |
THICKNESS     ^^^^^
*/
void writeUnit(int row,int col,int choice);

int main()
{
//definition
    int i,j,row,col;

    unit[0][0][0]=unit[0][1][0]=':';
    unit[0][0][1]=unit[0][0][THICKNESS+1]='\\';
    unit[0][1][1]=unit[0][1][THICKNESS+1]='/';

    unit[1][0][0]=unit[1][0][1]=':';
		for(i=0; i<THICKNESS+2; i++)
			unit[1][1][i]='~';

    unit[2][1][0]=unit[2][1][1]=':';
    unit[2][1][2]=unit[2][1][THICKNESS+2]='\\';
    for(i=0; i<THICKNESS+2; i++)
        unit[2][0][i]='_';

    unit[3][0][0]=unit[3][1][0]='|';
    unit[3][0][1]=unit[3][1][1]=':';

    unit[4][0][0]=unit[4][1][0]=':';
    unit[4][0][1]=unit[4][0][THICKNESS+1]
   =unit[4][1][1]=unit[4][1][THICKNESS+1]='|';
//input
    cout<<"Input a 2D description, using '/', '\\', '0' and '.':\n";
    for(i=0; i<iHEIGHT; i++)
        for(j=0; j<iWIDTH; j++)
            cin>>input[i][j];
//write
    for(i=iHEIGHT-1; i>=0; i--)
        for(j=0; j<iWIDTH; j++)
        {
            //防止数组溢出，做了些“多余”的工作！
            hasup   = i!=0 && input[i-1][j]!='.' && i!=0;
            hasdown = i!=iHEIGHT-1 && input[i+1][j]!='.' && i!=iHEIGHT-1;
            hasleft = j!=0 && input[i][j-1]!='.' && j!=0;
            hasright= j!=iWIDTH-1 && input[i][j+1]!='.' && j!=iWIDTH-1;

            isconcave=i!=iHEIGHT-1 && j!=iWIDTH-1 && input[i+1][j+1]!='.'
                    && (input[i][j]!='\\' || input[i+1][j+1]!='\\')
                    && i!=iHEIGHT-1 && j!=iWIDTH-1;

            row=1+i+j;
            col=9-i+j;
            /*
               从input到output的坐标变换公式是:
                    row'=1+row+col
                    col'=9-row+col
                    row=(row'-col')/2 + 4
                    col=(row'+col')/2 - 5
            */
            switch(input[i][j])
            {
                case '0':
                    writeUnit(row,col,0);
                    break;
                case '/':
                    writeUnit(row,col, 1+ (!hasup || !hasleft));
                    break;
                case '\\':
                    writeUnit(row,col, 3+ (!hasup || !hasright));
                    break;
                default:
                    break;
            }
        }
//output
    for(i=0; i<oHEIGHT; i++)
    {
        for(j=0; j<oWIDTH; j++)
            cout<<output[i][j];
        cout<<endl;
    }
//pause
    system("pause");
    return 0;
}

void writeUnit(int row,int col,int choice)
{
    int i,j,k;

    if(choice==0 || choice==4)
    {
        if(!hasleft)
            unit[choice][0][0]='/';         //左边
        else
            unit[choice][0][0]=':';

        if(!hasleft && !hasup)
            for(k=2; k<=THICKNESS; k++)
                unit[choice][0][k]='~';     //画上线
        else if(choice==0 &&
                    (
                        (!hasright && !hasup)
                    ||
                        input[(row-col)/2 + 4][(row+col)/2 - 4]=='\\'   //右是'\'
                    )
               )
            for(k=2; k<=THICKNESS; k++)
                unit[choice][0][k]='_';     //画中线
        else
            for(k=2; k<=THICKNESS; k++)
                unit[choice][0][k]=' ';
    }

    if(choice==0 || choice==2 || choice==4)
    {
        if(!hasdown)
            unit[choice][1][0]='\\';        //底边
        else
            unit[choice][1][0]=':';

        if(
            isconcave
        ||
            (choice==0 && !hasdown)
        ||
            (
                choice==4
            &&
                (
                    (row-col)/2 + 5==iHEIGHT
                ||
                    (row+col)/2 - 4==iWIDTH                         //右下出界
                ||
                    input[(row-col)/2 + 5][(row+col)/2 - 4]!='\\'   //右下不是'\'
                )
            )
        )
            for(k=2; k<=THICKNESS; k++)
                unit[choice][1][k]='_';     //画底线
        else
            for(k=2; k<=THICKNESS; k++)
                unit[choice][1][k]=' ';
    }

//打印单位
    for(i=0; i<2; i++)
        for(j=0; j<THICKNESS+3; j++)
            if(row+i<oHEIGHT && col+j<oWIDTH)
            {
                output[row+i][col+j]=unit[choice][i][j];
            }
}

#undef iHEIGHT
#undef iWIDTH
#undef THICKNESS
#undef oHEIGHT
#undef oWIDTH
