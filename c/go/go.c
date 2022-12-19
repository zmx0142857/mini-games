#include "main.h"

int qfront = 0, qrear = 0, top = 0;

// 围棋: 判断 (x,y) 所在的连通分量是否被围死
int go_isdead(int x, int y)
{
	top = 0;
	if (!bounded(x, y) || board[x][y] != color)
		return 0;
	int dx[] = {0, 0, -1, 1};
	int dy[] = {1, -1, 0, 0};

	// 广度优先
	memset(visited, false, sizeof(visited));
	qfront = qrear = 0;
	qx[qrear] = x;
	qy[qrear] = y;
	qrear = (qrear+1) % SIZE;
	visited[x][y] = true;
	while (qfront != qrear) {
		x = qx[qfront];
		y = qy[qfront];
		qfront = (qfront+1) % SIZE;

		// 要移除的棋子入栈
		sx[top] = x;
		sy[top++] = y;

		for (int i = 0; i < 4; ++i) {
			x += dx[i];
			y += dy[i];
			if (bounded(x, y) && !visited[x][y]) {
				if (board[x][y] == color) {
					qx[qrear] = x;
					qy[qrear] = y;
					qrear = (qrear+1) % SIZE;
					visited[x][y] = true;
				} else if (board[x][y] == BLANK) {
					return 0;
				}
			}
			x -= dx[i];
			y -= dy[i];
		}
	}
	return top;
}

// 围棋: 吃掉含 (x,y) 的一个连通分量
int go_do_eat(int x, int y)
{
	int ret = top = go_isdead(x, y);
	while (top--) {
		x = sx[top];
		y = sy[top];
		set_board(x, y, BLANK);
	}
	return ret;
}

// 围棋: 落子于 (x,y), 返回吃子数
int go_eat(int x, int y)
{
	color = flip(color);
	int ret
        = go_do_eat(x, y+1)
        + go_do_eat(x, y-1)
        + go_do_eat(x-1, y)
        + go_do_eat(x+1, y);
	if (ret)
		MSG("提子: %d", ret);
	color = flip(color);
	return ret;
}

// 围棋: 若游戏结束, 返回 true
bool rule_go(int x, int y)
{
    board[x][y] = color; // try
    int flag_dead = go_isdead(x, y);
    int flag_eat = go_eat(x, y);
    if (flag_dead && !flag_eat) {
        board[x][y] = BLANK; // restore
        MSG("此处不可落子");
    } else {
        set_board(x, y, color);
        color = flip(color);
	}
    return false;
}
