#ifndef SOKOBAN_H
#define SOKOBAN_H

//库
#include<stdio.h>       //printf();scanf();
#include<conio.h>       //getche();
#include<windows.h>     //GetStdHandle();

//宏
#define MAX_LEVEL           50
#define MAP_W               17
#define MAP_H               13

#define NOTHING             0
#define WALL                1
#define HERO                2
#define BOX                 3
#define BOX_IN_PLACE        4
#define STAR                5
#define DOOR                6

#define Hero_x              Hero[0]
#define Hero_y              Hero[1]
#define Door_x              Door[0]
#define Door_y              Door[1]

#define TextColor(MyColor)  SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), MyColor )
#define BRIGHT              FOREGROUND_INTENSITY
#define RED                 FOREGROUND_RED
#define GREEN               FOREGROUND_GREEN
#define BLUE                FOREGROUND_BLUE

//声明全局变量
extern int Map[13][17];                        //地图
extern int MapCopy[13][17];                    //地图的副本，用于还原被抹去的星形记号
extern int Hero[2];                            //人的横纵坐标分别是Hero[0],Hero[1]
extern int Door[2];                            //门的横纵坐标分别是Door[0],Door[1]

extern int nLevel;                             //当前关卡
extern int nMove;                              //移动的次数
extern int nPush;                              //推箱子的次数

//声明全局函数
extern void GoToLevel(int);
extern void Play();
extern void InitializeAll();
extern void PrintBoard();
extern int Control();
extern int Finish();

extern void Lv1();
extern void Lv2();
extern void Lv3();
extern void Lv4();
extern void Lv5();
extern void Lv6();
extern void Lv7();
extern void Lv8();
extern void Lv9();
extern void Lv10();
extern void Lv11();
extern void Lv12();
extern void Lv13();
extern void Lv14();

#endif
