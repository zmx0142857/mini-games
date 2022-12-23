#define EXTERN_GAME_H
#include "main.h"
#include <assert.h>
#define INF 99999

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
    2, 1, 1, 0,
    10, 2, 2, 0,
    100, 50, 5, 0,
    1000, 500, 50, 0,
    10000,
    10000
};

// 五子棋: 获取指定方向上的棋型分数
// 假定 (x, y) 处未落子
int gomoku_line_score(int x, int y, int direction, enum Color color, int debug) {
    static int dx[8] = { 0, 1, 1, 1 };
    static int dy[8] = { 1, 1, 0, -1 };
    enum Color rival_color = flip(color);

    int block_l = 0, block_r = 0; // 阻挡物的位置
    bool block_blank_l = false, block_blank_r = false; // 与阻挡物间是否有空隙
    int blank_l = false, blank_r = false; // 是否遇到空白
    bool skip_l = false, skip_r = false; // 己方棋子是否断开
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
            ++blank_r;
        } else { // board[xx][yy] == color
            block_blank_r = false;
            if (blank_r == 0) {
                ++count_r;
                ++count_l;
            }
            if (blank_r == 1) {
                skip_r = true;
                ++count_r;
            }
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
            ++blank_l;
        } else { // board[xx][yy] == color
            block_blank_l = false;
            if (blank_l == 0) {
                ++count_l;
                ++count_r;
            }
            if (blank_l == 1) {
                skip_l = true;
                ++count_l;
            }
        }
    }

    bool dead_l = block_l && !block_blank_l;
    bool dead_r = block_r && !block_blank_r;
    if (block_l == 0) block_l = -INF;
    if (block_r == 0) block_r = INF;

    int type = 0;
    int count = count_l > count_r ? count_l : count_r;
    int skip = count_l > count_r ? skip_l
        : count_l < count_r ? skip_r
        : skip_l < skip_r ? skip_l
        : skip_r;
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

    int score = gomoku_score_map[type];
    if (skip == 0) {
        score += 10;
    }
    if (debug > 0) {
        mvprint(MSG_LINE + debug + direction, 0,
            "type: %d, skip: %d, skip_l: %d, skip_r: %d, count_l: %d, count_r: %d, score: %d", type, skip, skip_l, skip_r, count_l, count_r, score
        );
    }
    return score;
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
        score += gomoku_line_score(x, y, i, color, 0);
        score += gomoku_line_score(x, y, i, flip(color), 0);
    }
    return score;
}

// 五子棋: 遍历所有可能的落子位置, 计算最高分
void gomoku_ai_next_move(int *ret_x, int *ret_y)
{
    int best_score = -INF;
    int best_x = -INF;
    int best_y = -INF;
    int best_count = 0;

    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            if (board[x][y] == BLANK) {
                int score = gomoku_ai_score(x, y);
                if (score > best_score) {
                    best_score = score;
                    best_count = 1;
                    best_x = x;
                    best_y = y;
                }
                if (score == best_score) {
                    ++best_count;
                    // 第 n 个最优解以 1/n 的概率取代当前最优解
                    if (rand() % best_count == 0) {
                        best_x = x;
                        best_y = y;
                    }
                }
            }
        }
    }
    
    assert(best_score != -INF);

    *ret_x = best_x;
    *ret_y = best_y;

//     for (int i = 0; i < 4; ++i) {
//         gomoku_line_score(best_x, best_y, i, color, 1);
//         gomoku_line_score(best_x, best_y, i, flip(color), 5);
//     }
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

// 五子棋: 下一步棋: 若游戏结束, 返回 true
bool gomoku_step(int x, int y)
{
    set_board(x, y, color);
    if (gomoku_check_directions(x, y)) {
        MSG(color == BLACK ? "黑胜" : "白胜");
        return true;
    }
    color = flip(color);
    return false;
}

// 五子棋: 若游戏结束, 返回 true
bool rule_gomoku(int x, int y)
{
    if (gomoku_step(x, y)) return true;
    if (!is_ai) return false;
    gomoku_ai_next_move(&x, &y);
    if (gomoku_step(x, y)) return true;
    return false;
}
