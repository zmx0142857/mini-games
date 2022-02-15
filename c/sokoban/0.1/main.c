#include "Sokoban.h"

int main()
{
    void Welcome();
    void Instruction();
    void SelectLevel();
    void Initialize1();

    char key;

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD size = {70, 32};
    SMALL_RECT rc = {0,0, 70-1, 32-1};
	
    SetConsoleScreenBufferSize(hOut, size); //���û�������С
    SetConsoleWindowInfo(hOut, 1, &rc);   	//���ô���λ�úʹ�С

    InitializeCopy();
	Welcome();
	system("cls");
    while(1)
    {
        Welcome();
        key = getch();

        switch(key)
        {
            case '1':   Play();         break;
            case '2':   SelectLevel();  break;
            case '3':   Instruction();  break;
            case '4':   return 0;
            default :   system("cls");
        }
    }
    return 0;
}
