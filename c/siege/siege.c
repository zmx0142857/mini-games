/* honeycomb with 121 cells:
 * each cell can be determined with a pair of numbers (column,row),
 * the origin (0,0) is marked by % and (0,4) is marked by #
             __                      __
            /  \__                __/  \
          . \__/  \__          __/  \__/
       .    /  \__/  \__    __/  \__/  \
    .       \__/  \__/  \__/  \__/  \__/
 %          /  \__/  \__/  \__/  \__/  \
            \__/  \__/  \__/  \__/  \__/
 .          /  \__/  \__/  \__/  \__/  \
            \__/  \__/  \__/  \__/  \__/
 .        __/  \__/  \__/  \__/  \__/  \__
       __/  \__/  \__/  \__/  \__/  \__/  \__
 .  __/  \__/  \__/  \__/  \__/  \__/  \__/  \__
 __/  \__/  \__/  \__/  \__/  \__/  \__/  \__/  \__
/# \__/  \__/  \__/  \__/  \__/  \__/  \__/  \__/  \
\__/  \__/  \__/  \__/  \__/  \__/  \__/  \__/  \__/
   \__/  \__/  \__/  \__/  \__/  \__/  \__/  \__/
      \__/  \__/  \__/  \__/  \__/  \__/  \__/
         \__/  \__/  \__/  \__/  \__/  \__/
            \__/  \__/  \__/  \__/  \__/
            /  \__/  \__/  \__/  \__/  \
            \__/  \__/  \__/  \__/  \__/
            /  \__/  \__/  \__/  \__/  \
            \__/  \__/  \__/  \__/  \__/
            /  \__/  \__/  \__/  \__/  \
            \__/  \__/        \__/  \__/
            /  \__/              \__/  \
            \__/                    \__/
*/

#include "game.h"
#include "string.h"

#define LEN 122

// cells
#define BLANK 0
#define WALL 1
#define BALL 2
#define CELL 3
#define CURSOR 4
#define QUESTION_MARK 5
#define EXCLAMATION_MARK 6

// directions 8 - 13
#define VISITED 8

// status
#define PLAYING 0
#define WIN 1
#define LOSE 2
#define QUIT 3

int cur_x, cur_y, ball_x, ball_y;
int honeycomb[17][17];

#define BUFLEN 256
char title[BUFLEN];

#ifdef DEBUG
FILE *logfile;
#endif

void help()
{
	printf("siege: try to trap the ball!\n"
		   "controls:\n"
		   "    u: go upper left\n"
		   "    i: go up\n"
		   "    o: go upper right\n"
		   "    j: go lower left\n"
		   "    k: go down\n"
		   "    l: go lower right\n"
		   "    hold down shift to move item\n");
}

bool isvalid(int x, int y)
{
	return (x <= 12 && y >= 4 && y-x <= 4)
		|| (y <= 12 && x >= 4 && x-y <= 4);
}

void print_cell(int x, int y, int type)
{
	int a = 5-x+2*y, b = 3*x+1;
	switch (type) {
		case BALL: mvprint(a, b, "\033[34m()\033[0m"); break;
		case QUESTION_MARK: mvprint(a, b, "\033[34m??\033[0m"); break;
		case EXCLAMATION_MARK: mvprint(a, b, "\033[34m!!\033[0m"); break;
		case WALL: mvprint(a, b, "\033[31m##\033[0m"); break;
		case BLANK: mvprint(a, b, "  "); break;
		case CELL:
			mvprint(a-1, b, "__");
			mvprint(a, b-1, "/  \\");
			mvprint(a+1, b-1, "\\__/");
			break;
		default: break; // CURSOR
	}
	a = 5 - cur_x + 2 * cur_y;
	b = 3 * cur_x + 1;
	cursor_goto(a, b);
}

#ifdef DEBUG
void dump()
{
	for (int y = 0; y < 17; ++y) {
		for (int x = 0; x < 17; ++x) {
			if (honeycomb[x][y] == 0)
				fputc(' ', logfile);
			else
				fprintf(logfile, "%d", honeycomb[x][y]);
		}
		fputc('\n', logfile);
	}
	fputc('\n', logfile);
	fflush(logfile);
}
#endif

int escape()
{
	Sleep(300);

	// bfs
	int qx[LEN], qy[LEN]; // queue
	int front = 0, rear = 0;
	int next_x, next_y;
	const int dxs[6] = {-1, -1, 0, 0, 1, 1};
	const int dys[6] = {-1, 0, -1, 1, 0, 1};
	bool found = false;
	// enqueue
	qx[rear] = ball_x;
	qy[rear] = ball_y;
	rear = (rear+1) % LEN;
	while (!found && front != rear) {
		// dequeue
		int x = qx[front];
		int y = qy[front];
		front = (front+1) % LEN;
		for (int i = 0; i < 6; ++i) {
			x += dxs[i];
			y += dys[i];
			if (!isvalid(x, y)) {
				x -= dxs[i];
				y -= dys[i];
				if (x == ball_x && y == ball_y) {
					print_cell(x, y, EXCLAMATION_MARK);
					return LOSE;
				}
				found = true;
				while (honeycomb[x][y] != BALL) {
					int j = honeycomb[x][y]-VISITED;
					next_x = x;
					next_y = y;
					x += dxs[j];
					y += dys[j];
				}
				break;
			} else if (honeycomb[x][y] == BLANK) {
				// reverse direction
				honeycomb[x][y] = VISITED+5-i;
				// enqueue
				qx[rear] = x;
				qy[rear] = y;
				rear = (rear+1) % LEN;
			}
			x -= dxs[i];
			y -= dys[i];
		}
	}
	for (int x = 0; x < 17; ++x)
		for (int y = 0; y < 17; ++y)
			if (honeycomb[x][y] >= VISITED)
				honeycomb[x][y] = BLANK;
	if (found) {
		honeycomb[ball_x][ball_y] = BLANK;
		print_cell(ball_x, ball_y, BLANK);
		ball_x = next_x; ball_y = next_y;
		honeycomb[ball_x][ball_y] = BALL;
		print_cell(ball_x, ball_y, BALL);
		return PLAYING;
	} else {
		print_cell(ball_x, ball_y, QUESTION_MARK);
		return WIN;
	}
}

void move_cursor(int dx, int dy)
{
	if (isvalid(cur_x + dx, cur_y + dy)) {
		cur_x += dx, cur_y += dy;
		print_cell(cur_x, cur_y, CURSOR);
	}
}

int move_wall(int dx, int dy)
{
	int *from = &honeycomb[cur_x][cur_y];
	int *to = &honeycomb[cur_x + dx][cur_y + dy];
	if (isvalid(cur_x + dx, cur_y + dy)
			&& *from == WALL && *to == BLANK) {
		print_cell(cur_x, cur_y, *from = BLANK);
		cur_x += dx, cur_y += dy;
		print_cell(cur_x, cur_y, *to = WALL);
		return escape();
	}
	return PLAYING;
}

bool init(char *filename)
{
	FILE *f = fopen(filename, "r");
	if (!f)
		return false;
	// set title
	fgets(title, BUFLEN, f);

	// set global varibles
	cur_x = cur_y = 8;
	memset(honeycomb, BLANK, sizeof(int)*17*17);

	// the ball
	fscanf(f, "%d %d", &ball_x, &ball_y);
	honeycomb[ball_x][ball_y] = BALL;

	// walls
	int x, y;
	while (fscanf(f, "%d %d", &x, &y) == 2)
		honeycomb[x][y] = WALL;

	fclose(f);
	return true;
}

void print_map()
{
	for (int x = 0; x < 17; ++x)
		for (int y = 0; y < 17; ++y)
			if (isvalid(x, y)) {
				int type = honeycomb[x][y];
				print_cell(x, y, CELL);
				if (type != BLANK)
					print_cell(x, y, type);
			}
	print_cell(cur_x, cur_y, CURSOR);
}

int play()
{
	int status = PLAYING;
	int ch;
	while (status == PLAYING) {
		ch = getch();
		switch (ch) {
			case 'u': move_cursor(-1, -1); break;
			case 'i': move_cursor(0, -1); break;
			case 'o': move_cursor(1, 0); break;
			case 'j': move_cursor(-1, 0); break;
			case 'k': move_cursor(0, 1); break;
			case 'l': move_cursor(1, 1); break;

			case 'U': status = move_wall(-1, -1); break;
			case 'I': status = move_wall(0, -1); break;
			case 'O': status = move_wall(1, 0); break;
			case 'J': status = move_wall(-1, 0); break;
			case 'K': status = move_wall(0, 1); break;
			case 'L': status = move_wall(1, 1); break;

			case EOF: case 4: status = QUIT; break; // ctrl-d
			case 'r': status = LOSE; break; // restart
		}
	}
	return status;
}

int main(int argc, char **argv)
{
#ifdef DEBUG
	// logfile
	logfile = fopen("log", "w");
	if (!logfile) return 0;
#endif

	int i = 1;
	if (argc == 2) {
		if (strcmp(argv[1], "-h") == 0) {
			help();
			return 0;
		} else if ((i = atoi(argv[1])) <= 0) {
			i = 1;
		}
	}
	screen_clear();
	char buf[BUFLEN];
	while (true) {
		sprintf(buf, "level/%02d.level", i);
		if (!init(buf))
			break;
		cursor_goto(27, 0);
		line_clear();
		printf("Lv %d: %s", i, title);
		print_map();
		int status = play();
		if (status == WIN) {
			fflush(stdout);
			Sleep(500);
			++i;
		} else if (status == LOSE) {
			fflush(stdout);
			Sleep(500);
		} else if (status == QUIT) {
			break;
		}
	}
	cursor_goto(28, 0); 

#ifdef DEBUG
	fclose(logfile);
#endif
	return 0;
}
