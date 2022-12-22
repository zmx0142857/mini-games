#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#define INF 999999

int board[] = { 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 2 };

// 问题: 1 1 0 0 1, 1 0 1 0 1 等也被算作活三？

bool bounded(int x)
{
    return x >= 0 && x <= 10;
}

int gomoku_line_type(int x) {
    int count = 1, offset, xx;
    int color = 1, rival_color = 2;
    bool blank_l = false, blank_r = false;
    int block_l = 0, block_r = 0;
    // 沿正方向看
    for (offset = 1; offset <= 5; ++offset) {
        xx = x + offset;
        if (!bounded(xx) || board[xx] == rival_color) {
            block_r = offset;
            break;
        } else if (board[xx] == 0) {
            blank_r = true;
        } else { // board[xx] == color
            blank_r = false;
            ++count;
        }
    }
    // 沿反方向看
    for (offset = -1; offset >= -5; --offset) {
        xx = x + offset;
        if (!bounded(xx) || board[xx] == rival_color) {
            block_l = offset;
            break;
        } else if (board[xx] == 0) {
            blank_l = true;
        } else { // board[xx] == color
            blank_l = false;
            ++count;
        }
    }

    bool dead_l = block_l && !blank_l;
    bool dead_r = block_r && !blank_r;
    if (block_l == 0) block_l = -INF;
    if (block_r == 0) block_r = INF;

    int type = 0;
    if (count <= 4) {
        type += (count - 1) * 4;
        if (block_r - block_l > 5) {
            if (dead_l || dead_r) {
                printf("rush %d\n", count);
                type += 1; // RUSH
            } else {
                printf("live %d\n", count);
                type += 0; // LIVE
            }
        } else {
            if (blank_l || blank_r) {
                printf("sleep %d\n", count);
                type += 2; // SLEEP
            } else {
                printf("dead %d\n", count);
                type += 3; // DEAD
            }
        }
    } else if (count == 5) {
        puts("= 5");
        type = 16;
    } else { // count > 5
        puts("> 5");
        type = 17;
    }
    return type;
}

int main()
{
    gomoku_line_type(5);
    return 0;
}
