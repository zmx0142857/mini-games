#define EXTERN_GAME_H
#include "main.h"
#include <assert.h>
#define INF 2147483647

// 棋型
enum GomokuType {
    GOMOKU_LIVE_1, GOMOKU_RUSH_1, GOMOKU_SLEEP_1, GOMOKU_DEAD_1, // 活1 冲1 眠1 死1
    GOMOKU_LIVE_2, GOMOKU_RUSH_2, GOMOKU_SLEEP_2, GOMOKU_DEAD_2,
    GOMOKU_LIVE_3, GOMOKU_RUSH_3, GOMOKU_SLEEP_3, GOMOKU_DEAD_3,
    GOMOKU_LIVE_4, GOMOKU_RUSH_4, GOMOKU_SLEEP_4, GOMOKU_DEAD_4,
    GOMOKU_WIN, // 5 子连珠
    GOMOKU_LONG // 长连
};

// 棋型对应的分数
int gomoku_score_map[] = {
    0, 0, 0,
    10, 5, 5,
    100, 50, 5,
    1000, 500, 50,
    10000,
    10000
};

// 五子棋: 获取指定方向上的棋型
// 假定 (x, y) 处未落子
enum GomokuType gomoku_line_type(int x, int y, int direction, enum Color color) {
    static int dx[8] = { 0, 1, 1, 1 };
    static int dy[8] = { 1, 1, 0, -1 };
    enum Color rival_color = flip(color);

    int block_l = 0, block_r = 0; // 阻挡物的位置
    bool block_blank_l = false, block_blank_r = false; // 与阻挡物间是否有空隙
    bool blank_l = false, blank_r = false; // 是否遇到空白
    int count_l = 1, count_r = 1; // 左右计数, 二者取大
    int offset, xx, yy;

    // 沿正方向看 (r)
    for (offset = 1; offset <= 5; ++offset) {
        xx = x + dx[direction] * offset;
        yy = y + dy[direction] * offset;
        if (!bounded(xx, yy) || board[xx][yy] == rival_color) {
            block_r = offset;
            break;
        } else if (board[xx][yy] == BLANK) {
            block_blank_r = true;
            if (blank_r) break;
            blank_r = true;
        } else { // board[xx][yy] == color
            block_blank_r = false;
            if (!blank_r) ++count_r;
            ++count_l;
        }
    }

    // 沿反方向看 (l)
    for (offset = -1; offset >= -5; --offset) {
        xx = x + dx[direction] * offset;
        yy = y + dy[direction] * offset;
        if (!bounded(xx, yy) || board[xx][yy] == rival_color) {
            block_l = offset;
            break;
        } else if (board[xx][yy] == BLANK) {
            block_blank_l = true;
            if (blank_l) break;
            blank_l = true;
        } else { // board[xx][yy] == color
            block_blank_l = false;
            if (!blank_l) ++count_l;
            ++count_r;
        }
    }

    bool dead_l = block_l && !block_blank_l;
    bool dead_r = block_r && !block_blank_r;
    if (block_l == 0) block_l = -INF;
    if (block_r == 0) block_r = INF;

    int type = 0;
    int count = count_l > count_r ? count_l : count_r;
    if (count <= 4) {
        type += (count - 1) * 4;
        if (block_r - block_l > 5) { // 有足够空隙
            if (dead_l || dead_r) { // 左右是否堵死
                type += 1; // RUSH
            } else {
                type += 0; // LIVE
            }
        } else { // 没有足够空隙
            if (block_blank_l || block_blank_r) { // 是否留有空白
                type += 2; // SLEEP
            } else {
                type += 3; // DEAD
            }
        }
    } else if (count == 5) {
        type = GOMOKU_WIN;
    } else { // count > 5
        type = GOMOKU_LONG;
    }
    return type;
}

// 五子棋: 获取指定方向上的连续棋子数
int gomoku_line_count(int x, int y, int direction, enum Color color) {
    static int dx[8] = { 0, 1, 1, 1 };
    static int dy[8] = { 1, 1, 0, -1 };
    int count = 1, offset, xx, yy;
    // 沿正方向看
    for (offset = 1; offset < 5; ++offset) {
        xx = x + dx[direction] * offset;
        yy = y + dy[direction] * offset;
        if (!bounded(xx, yy) || board[xx][yy] != color) break;
        ++count;
    }
    // 沿反方向看
    for (offset = -1; offset > -5; --offset) {
        xx = x + dx[direction] * offset;
        yy = y + dy[direction] * offset;
        if (!bounded(xx, yy) || board[xx][yy] != color) break;
        ++count;
    }
    return count;
}

// 五子棋: 获取位置 (x, y) 的分数
int gomoku_ai_score(int x, int y)
{
    int score = 0;
    for (int i = 0; i < 4; ++i) {
        score += gomoku_score_map[gomoku_line_type(x, y, i, color)];
        score += gomoku_score_map[gomoku_line_type(x, y, i, flip(color))];
    }
    return score;
}

// 五子棋: 遍历所有可能的落子位置, 计算最高分
void gomoku_ai_next_move()
{
    int best_score = -INF;
    int best_x = -INF;
    int best_y = -INF;

    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            if (board[x][y] == BLANK) {
                int score = gomoku_ai_score(x, y);
                // TODO: when score == best_score, make random choice
                if (score > best_score) {
                    best_score = score;
                    best_x = x;
                    best_y = y;
                }
            }
        }
    }

    int x = best_x, y = best_y;
    set_board(x, y, color);

    for (int i = 0; i < 4; ++i) {
        int type1 = gomoku_line_type(x, y, i, color);
        int type2 = gomoku_line_type(x, y, i, flip(color));
        mvprint(MSG_LINE + 1 + i, 0, "%d %d", type1, type2);
    }
}

// 五子棋: 检查各个方向, 若游戏结束, 返回 true
bool gomoku_check_directions(int x, int y)
{
    for (int i = 0; i < 4; ++i) {
        int count = gomoku_line_count(x, y, i, color);
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
    if (!is_ai) return false;

    gomoku_ai_next_move();
    if (gomoku_check_directions(x, y)) {
        MSG(color == BLACK ? "黑胜" : "白胜");
        return true;
    }
    color = flip(color);
    return false;
}
