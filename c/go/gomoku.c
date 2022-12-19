#define EXTERN_GAME_H
#include "main.h"

// 五子棋: 检查各个方向, 若游戏结束, 返回 true
bool gomoku_check_directions(int x, int y)
{
    int dx[8] = { 0, 1, 1, 1 };
    int dy[8] = { 1, 1, 0, -1 };
    for (int i = 0; i < 4; ++i) {
        int offset;
        int count = 1;
        // 沿正方向看
        for (offset = 1; offset < 5; ++offset) {
            int xx = x + dx[i] * offset;
            int yy = y + dy[i] * offset;
            if (!bounded(xx, yy) || board[xx][yy] != color) break;
            ++count;
        }
        // 沿反方向看
        for (offset = -1; offset > -5; --offset) {
            int xx = x + dx[i] * offset;
            int yy = y + dy[i] * offset;
            if (!bounded(xx, yy) || board[xx][yy] != color) break;
            ++count;
        }
        if (count >= 5) return true;
    }
    return false;
}

// 五子棋: 若游戏结束, 返回 true
bool rule_gomoku(int x, int y)
{
    set_board(x, y, color);
    if (gomoku_check_directions(x, y)) {
        MSG(color == BLACK ? "黑胜" : "白胜");
        return true;
    }
    color = flip(color);
    return false;
}
