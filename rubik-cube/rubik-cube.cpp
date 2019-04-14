#include <cstdio>
#include <stack>
using namespace std;

class Cube {
	typedef unsigned char Color;
	static const Color WHITE=0, BLUE=1, RED=2, YELLOW=3, GREEN=4,
				 MAGENTA=5, COLORS=6;
	static const int DIM = 9;
	Color data[COLORS][DIM];
	typedef unsigned char Move;
	stack<Move> undo, redo;

public:
	Cube()
	{
		for (Color color = WHITE; color != COLORS; ++color)
			for (int i = 0; i != DIM; ++i)
				data[color][i] = color;
	}

	void cycle(Color &a, Color &b, Color &c, Color &d, bool clockwise)
	{
		Color tmp = a;
		if (clockwise) {
			a = b; b = c; c = d; d = tmp;
		} else {
			a = d; d = c; c = b; b = tmp;
		}
	}

	void face_rotate(int face, bool clockwise)
	{
		
		cycle(data[face][7], data[face][5], data[face][3], data[face][1], clockwise);
		cycle(data[face][8], data[face][6], data[face][4], data[face][2], clockwise);
	}

	void rotate(Move group, bool clockwise)
	{
		/* group:
		 * 1 = up,    2 = mid, 3 = bottom,
		 * 4 = left,  5 = mid, 6 = right,
		 * 7 = front, 8 = mid, 9 = rear.
		 */
		switch (group) {
			case 1: face_rotate(0, clockwise);
					cycle(data[1][2], data[2][4], data[4][8], data[5][6], clockwise);
					cycle(data[1][3], data[2][5], data[4][1], data[5][7], clockwise);
					cycle(data[1][4], data[2][6], data[4][2], data[5][8], clockwise); break;
			case 3: face_rotate(3, clockwise);
					cycle(data[5][2], data[4][4], data[2][8], data[1][6], clockwise);
					cycle(data[5][3], data[4][5], data[2][1], data[1][7], clockwise);
					cycle(data[5][4], data[4][6], data[2][2], data[1][8], clockwise); break;
			case 4: face_rotate(5, clockwise);
					cycle(data[4][2], data[3][4], data[1][8], data[0][6], clockwise);
					cycle(data[4][3], data[3][5], data[1][1], data[0][7], clockwise);
					cycle(data[4][4], data[3][6], data[1][2], data[0][8], clockwise); break;
			case 6: face_rotate(2, clockwise);
					cycle(data[0][2], data[1][4], data[3][8], data[4][6], clockwise);
					cycle(data[0][3], data[1][5], data[3][1], data[4][7], clockwise);
					cycle(data[0][4], data[1][6], data[3][2], data[4][8], clockwise); break;
			case 7: face_rotate(1, clockwise);
					cycle(data[2][2], data[0][4], data[5][8], data[3][6], clockwise);
					cycle(data[2][3], data[0][5], data[5][1], data[3][7], clockwise);
					cycle(data[2][4], data[0][6], data[5][2], data[3][8], clockwise); break;
			case 9: face_rotate(4, clockwise);
					cycle(data[3][2], data[5][4], data[0][8], data[2][6], clockwise);
					cycle(data[3][3], data[5][5], data[0][1], data[2][7], clockwise);
					cycle(data[3][4], data[5][6], data[0][2], data[2][8], clockwise); break;
			case 2: cycle(data[1][1], data[2][3], data[4][7], data[5][5], clockwise);
					cycle(data[1][0], data[2][0], data[4][0], data[5][0], clockwise);
					cycle(data[1][5], data[2][7], data[4][3], data[5][1], clockwise); break;
			case 5: cycle(data[0][1], data[1][3], data[3][7], data[4][5], clockwise);
					cycle(data[0][0], data[1][0], data[3][0], data[4][0], clockwise);
					cycle(data[0][5], data[1][7], data[3][3], data[4][1], clockwise); break;
			case 8: cycle(data[2][1], data[0][3], data[5][7], data[3][5], clockwise);
					cycle(data[2][0], data[0][0], data[5][0], data[3][0], clockwise);
					cycle(data[2][5], data[0][7], data[5][3], data[3][1], clockwise); break;
		}
	}

	void print() const
	{
		/*
		      ____________           	      ____________          
		     / 8 / 1 / 2 /\          	     /\ 2 \ 1 \ 8 \
	        /___/___/___/6 \                /6 \___\___\___\
           / 7 /(0)/ 3 /\  /\              /\  /\ 3 \(4)\ 7 \
		  /___/___/___/5 \/7 \       	  /7 \/5 \___\___\___\
		 / 6 / 5 / 4 /\  /\  /\      	 /\  /\  /\ 4 \ 5 \ 6 \
		/___/___/___/4 \/2)\/8 \     	/8 \/5)\/4 \___\___\___\
		\ 2 \ 3 \ 4 \  /\  /\  /     	\  /\  /\  / 4 / 3 / 2 /
		 \___\___\___\/3 \/1 \/      	 \/1 \/3 \/___/___/___/
		  \ 1 \(1)\ 5 \  /\  /       	  \  /\  / 5 /(3)/ 1 /
		   \___\___\___\/2 \/        	   \/2 \/___/___/___/
		    \ 8 \ 7 \ 6 \  /         	    \  / 6 / 7 / 8 /
			 \___\___\___\/          		 \/___/___/___/

		printf(
			"      ____________\n"
			"     /   /   /   /\\\n"
			"    /___/___/___/  \\\n"
			"   /   /   /   /\\  /\\\n"
			"  /___/___/___/  \\/  \\\n"
			" /   /   /   /\\  /\\  /\\\n"
			"/___/___/___/  \\/  \\/  \\\n"
			"\\   \\   \\   \\  /\\  /\\  /\n"
			" \\___\\___\\___\\/  \\/  \\/\n"
			"  \\   \\   \\   \\  /\\  /\n"
			"   \\___\\___\\___\\/  \\/\n"
			"    \\   \\   \\   \\  /\n"
			"     \\___\\___\\___\\/\n");
		*/
		static const char *ones[] = {
			"\033[47m \033[0m", // white
            "\033[44m \033[0m", // blue
            "\033[41m \033[0m", // red
            "\033[43m \033[0m", // yellow
            "\033[42m \033[0m", // green
            "\033[45m \033[0m"  // magenta
		};
		static const char *threes[] = {
			"\033[47m   \033[0m",
            "\033[44m   \033[0m",
            "\033[41m   \033[0m",
            "\033[43m   \033[0m",
            "\033[42m   \033[0m",
            "\033[45m   \033[0m"
		};
		printf("\033[0;0H");	// go to 0,0
#define QQQ(i,j)	threes[data[i][j]]
#define Q(i,j)		ones[data[i][j]]
		printf("       %s %s %s \n"
			   "      %s %s %s %s \n"
			   "     %s %s %s %s \n"
			   "    %s %s %s %s %s %s \n"
			   "   %s %s %s %s %s \n"
			   "  %s %s %s %s %s %s %s %s \n"
			   " %s %s %s %s %s %s\n"
			   "e %s %s %s %s %s %s %s %s o\n"
			   "   %s %s %s %s %s\n"
			   "  d %s %s %s %s %s %s k\n"
			   "     %s %s %s %s\n"
			   "    c %s %s %s %s m\n"
			   "       v   b   n\n\n"
		, QQQ(0,8), QQQ(0,1), QQQ(0,2)
		, QQQ(0,8), QQQ(0,1), QQQ(0,2),                  Q(2,6)
		, QQQ(0,7), QQQ(0,0), QQQ(0,3),                 QQQ(2,6)
		, QQQ(0,7), QQQ(0,0), QQQ(0,3),          Q(2,5), Q(2,6), Q(2,7)
		, QQQ(0,6), QQQ(0,5), QQQ(0,4),         QQQ(2,5),       QQQ(2,7)
		, QQQ(0,6), QQQ(0,5), QQQ(0,4),  Q(2,4), Q(2,5), Q(2,0), Q(2,7), Q(2,8)
		, QQQ(1,2), QQQ(1,3), QQQ(1,4), QQQ(2,4),       QQQ(2,0),       QQQ(2,8)
		, QQQ(1,2), QQQ(1,3), QQQ(1,4),  Q(2,4), Q(2,3), Q(2,0), Q(2,1), Q(2,8)
		, QQQ(1,1), QQQ(1,0), QQQ(1,5),         QQQ(2,3)      , QQQ(2,1)
		, QQQ(1,1), QQQ(1,0), QQQ(1,5),          Q(2,3), Q(2,2), Q(2,1)
		, QQQ(1,8), QQQ(1,7), QQQ(1,6),                 QQQ(2,2)
		, QQQ(1,8), QQQ(1,7), QQQ(1,6),                  Q(2,2));
#undef QQQ
#undef Q
	}

	void play()
	{
		printf("\033[2J");		// clear screen
		char c = 0;
		Move m;
		print();
		printf("\033[1A");		// move up cursor
		while ((c = getchar()) != EOF) {
			switch (c) {
				case 'e': rotate(1, true); undo.push(~1); break;
				case 'd': rotate(2, true); undo.push(~2); break;
				case 'c': rotate(3, false); undo.push(3); break;
				case 'v': rotate(4, true); undo.push(~4); break;
				case 'b': rotate(5, false); undo.push(5); break;
				case 'n': rotate(6, false); undo.push(6); break;
				case 'm': rotate(7, true); undo.push(~7); break;
				case 'k': rotate(8, true); undo.push(~8); break;
				case 'o': rotate(9, false); undo.push(9); break;

				case 'E': rotate(1, false); undo.push(1); break;
				case 'D': rotate(2, false); undo.push(2); break;
				case 'C': rotate(3, true); undo.push(~3); break;
				case 'V': rotate(4, false); undo.push(4); break;
				case 'B': rotate(5, true); undo.push(~5); break;
				case 'N': rotate(6, true); undo.push(~6); break;
				case 'M': rotate(7, false); undo.push(7); break;
				case 'K': rotate(8, false); undo.push(8); break;
				case 'O': rotate(9, true); undo.push(~9); break;
				case '\n': 
					// move up cursor
					printf("\033[1A");
					printf("\033[K");
					continue;
				case 'u': 
					if (!undo.empty()) {
						m = undo.top();
						undo.pop();
						redo.push(m);
						if (1 <= m && m <= 9)
							rotate(m, true);
						else
							rotate(~m, false);
						break;
					} else {
						continue;
					}
				case 'r':
					if (!redo.empty()) {
						m = redo.top();
						redo.pop();
						if (1 <= m && m <= 9)
							rotate(m, false);
						else
							rotate(~m, true);
						break;
					} else {
						continue;
					}
				default:
					continue;
			}
			print();
		}
	}
};

int main()
{
	Cube cube;
	cube.play();
	return 0;
}
