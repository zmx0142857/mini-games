// 关灯问题
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int rows;
int cols;
char press[100][100]; // 按下的开关
char puzzle[100][100]; // 题面

bool guess() {
    int i, j;
    for (i = 2; i <= rows; i++) {
        for (j = 1; j <= cols; j++) {
            // 根据同列的上一行灯的最后状态，来决定是否按按钮
            press[i][j] = ( press[i-1][j]
                            + puzzle[i-1][j]
                            + press[i-1][j-1]
                            + press[i-2][j]
                            + press[i-1][j+1] ) % 2;
        }
    }

    for (j = 1; j <= cols; j++) {
        // 逐一判断最后一行的灯是否都熄灭               
        if (puzzle[rows][j] != (press[rows][j] + press[rows][j-1] + press[rows][j+1] + press[rows-1][j]) % 2)
            return false;
    }

    return true;
}

void process() {
    int c;
    // 清空第一行
    for (c = 1; c <= cols; c++)
        press[1][c] = 0;

    while (!guess()) {
        // 采用二进制进位的算法，从000000 - 111111枚举第一行按钮的方式
        press[1][1]++;
        c = 1;                
        while (press[1][c] > 1) {
            press[1][c] = 0;
            c++;
            press[1][c]++;
        }
    }
}

void init() {
    // 在矩阵的第一行前加入一个空行
    for (int i = 0; i < cols + 2; i++)
        press[0][i] = puzzle[0][i] = 0;
    // 在第一列前、最后一列后分别加入一个空列
    for (int i = 1; i <= rows; i++)
        press[i][0] = puzzle[i][0] = press[i][cols+1] = puzzle[i][cols+1] = 0;
    // 读取输入
    for (int i = 1; i <= rows; i++)
        for (int j = 1; j <= cols; j++)
            scanf("%d", (int*)&puzzle[i][j]);
}

void print() {
    for (int i = 1; i <= rows; i++){
        for (int j = 1; j <= cols; j++) {
            printf("%d ", press[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    if (argc < 3) {
        puts("usage: lightoff rows cols");
        return 1;
    }
    rows = atoi(argv[1]);
    cols = atoi(argv[2]);
    init();
    process();
    print();
    return 0;
}
