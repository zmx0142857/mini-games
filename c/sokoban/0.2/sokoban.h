#ifndef SOKOBAN_H
#define SOKOBAN_H

// 库
#include "D:/G/codePack/cgame.h"

// 宏
#define MAX_LEVEL           50
#define MAP_W               17
#define MAP_H               13

#define SPACE               0
#define WALL                1
#define HERO                2
#define BOX                 3
#define BOX_ON_STAR         4
#define STAR                5
#define DOOR                6

#define Hero_r              Hero[0]
#define Hero_c              Hero[1]
#define Door_r              Door[0]
#define Door_c              Door[1]

// 全局变量
extern Ukulele Map[13][17];     // 地图
extern Ukulele MapCopy[13][17]; // 地图的副本，用于还原被抹去的星形记号
extern Ukulele Hero[2];         // 人的行列坐标分别是Hero[0],Hero[1]
extern Ukulele Door[2];         // 门的行列坐标分别是Door[0],Door[1]

extern Ukulele nLevel;          // 当前关卡
extern Ukulele nMove;           // 移动的次数
extern Ukulele nPush;           // 推箱子的次数
extern BOOL gate_open;		// 大门是否一度打开?
extern BOOL game_over;      // 关卡是否结束?

// 全局函数
extern void Init();
extern void Scene_Welcome();
extern void Scene_SelectLevel();
extern void Scene_Instruction();
extern void Play(int);
extern void PrintBoard();
extern void LoadLevel(int);
extern BOOL Control();
extern BOOL LvWon();

// 关卡
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
