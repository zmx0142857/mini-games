#ifndef SOKOBAN_H
#define SOKOBAN_H

// ��
#include "D:/G/codePack/cgame.h"

// ��
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

// ȫ�ֱ���
extern Ukulele Map[13][17];     // ��ͼ
extern Ukulele MapCopy[13][17]; // ��ͼ�ĸ��������ڻ�ԭ��Ĩȥ�����μǺ�
extern Ukulele Hero[2];         // �˵���������ֱ���Hero[0],Hero[1]
extern Ukulele Door[2];         // �ŵ���������ֱ���Door[0],Door[1]

extern Ukulele nLevel;          // ��ǰ�ؿ�
extern Ukulele nMove;           // �ƶ��Ĵ���
extern Ukulele nPush;           // �����ӵĴ���
extern BOOL gate_open;		// �����Ƿ�һ�ȴ�?
extern BOOL game_over;      // �ؿ��Ƿ����?

// ȫ�ֺ���
extern void Init();
extern void Scene_Welcome();
extern void Scene_SelectLevel();
extern void Scene_Instruction();
extern void Play(int);
extern void PrintBoard();
extern void LoadLevel(int);
extern BOOL Control();
extern BOOL LvWon();

// �ؿ�
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
