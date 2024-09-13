#include <stdio.h>
#include <stdlib.h>

#define MAXN 99
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

int N = 9;
int board[MAXN*MAXN]; // i 行 j 列: board[i*N+j]
int clue[MAXN*4]; // 上方第 j 条线索: clue[UP*N+j]
int stack[MAXN*MAXN];
int max_depth = 0;
int count = 0;

/**
 * @param pos 开始位置 pos = i*N+j
 * @param step 1: 从左到右, -1: 从右到左, N: 从上到下, -N: 从下到上
 * @returns 能看到的高楼数
 */
int see(int pos, int step) {
    int max = 0, cnt = 0;
    for (int j = 0; j < N; j++) {
        if (board[pos] > max) {
            max = board[pos];
            ++cnt;
        }
        pos += step;
    }
    return cnt;
}

// 打印 clue
int print_clue() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", clue[i*N+j]);
        }
        puts("");
    }
}

// 打印 board
int print_board() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", board[i*N+j]);
        }
        puts("");
    }
}

// 数字 num 在位置 pos 能否放置
int fit(int pos, int num) {
    int i = pos / N, j = pos % N;
    // 每行、每列的数字不重复
    for (int k = 0; k < N; ++k) {
        if (board[i*N+k] == num || board[k*N+j] == num) return 0;
    }

    // sudoku rule
    // i -= i % 3;
    // j -= j % 3;
    // for (int k = i; k < i + 3; ++k) {
    //    for (int l = j; l < j + 3; ++l) {
    //        if (board[k*N+l] == num) return 0;
    //    }
    // }

    // skyscraper rule
    board[pos] = num;
    i = pos / N;
    j = pos % N;
    if (i == N-1 || (i == N-2 && board[pos + N] != 0)) {
        if (clue[DOWN*N+j]) {
            int see_down = see((N-1)*N+j, -N);
            if (see_down != clue[DOWN*N+j]) {
                board[pos] = 0;
                return 0;
            }
        }
        if (clue[UP*N+j]) {
            int see_up = see(0*N+j, N);
            if (see_up != clue[UP*N+j]) {
                board[pos] = 0;
                return 0;
            }
        }
    }
    if (j == N-1 || (j == N-2 && board[pos+1] != 0)) {
        if (clue[RIGHT*N+i]) {
            int see_right = see(i*N+(N-1), -1);
            if (see_right != clue[RIGHT*N+i]) {
                board[pos] = 0;
                return 0;
            }
        }
        if (clue[LEFT*N+i]) {
            int see_left = see(i*N+0, 1);
            if (see_left != clue[LEFT*N+i]) {
                board[pos] = 0;
                return 0;
            }
        }
    }

    return 1;
}

int solve_recur(int depth) {
    if (depth == max_depth) {
        puts("");
        print_board();
        ++count;
    } else {
        int pos = stack[depth];
        for (int num = N; num >= 1; num--) {
            if (fit(pos, num)) {
                board[pos] = num;
                solve_recur(depth + 1);
                board[pos] = 0;
            }
        }
    }
}

int solve() {
    for (int pos = 0; pos < N*N; pos++) {
        if (board[pos] == 0) {
            stack[max_depth++] = pos;
        }
    }
    printf("%d blanks to solve\n", max_depth);
    solve_recur(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        puts("usage: ./skyscraper input.txt");
        return 1;
    }
    FILE *fp = fopen(argv[1], "r");

    // 第一个数字是棋盘大小 N
    fscanf(fp, "%d", &N);

    // 后面 4 行依次是上下左右的线索, 每行 N 个数
    // 上、下的线索从左往右读取
    // 左、右的线索从上往下读取
    for (int pos = 0; pos < 4 * N; pos++) {
        fscanf(fp, "%d", &clue[pos]);
    }

    // 填充 N*N 的 board
    for (int pos = 0; pos < N * N; pos++) {
        fscanf(fp, "%d", &board[pos]);
    }

    print_clue();
    puts("");
    print_board();

    solve();
    printf("%d solution(s)\n", count);
    return 0;
}
