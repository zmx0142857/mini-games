#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* 000 Not decided
 * 100 A wins by rr
 * 101 A wins by rl
 * 110 A wins by lr
 * 111 A wins by ll
 * 1000 B wins
 */
#define ARR 4
#define ARL 5
#define ALR 6
#define ALL 7
#define A_WINS 4
#define B_WINS 8
int N = 10;
char **table;
#define TBL table[al*(al+1)/2+ar-1][bl*(bl+1)/2+br-1]
int al, ar, bl, br;
int cnt = 0;
int collision = 0;
char buf[10];

#define status(AL,AR,BL,BR) al==AL && ar==AR && bl==BL && br==BR

void print()
{
	char ch;
	printf("A/B");
	for (al = 1; al < N; ++al)
	for (ar = 0; ar <= al; ++ar)
		printf(" %d/%d", al, ar);
	putchar('\n');
	for (al = 1; al < N; ++al)
	for (ar = 0; ar <= al; ++ar) {
		printf("%d/%d ", al, ar);
		for (bl = 1; bl < N; ++bl)
		for (br = 0; br <= bl; ++br) {
			ch = TBL;
			if (ch == 0)
				printf("    ");
			else if (ch == B_WINS)
				printf("B   ");
			else {
				putchar('A');
				putchar(ch & 2 ? 'l' : 'r');
				putchar(ch & 1 ? 'l' : 'r');
				putchar(' ');
			}
		}
		putchar('\n');
	}
}

char check(int al, int ar, int bl, int br)
{
	if (al >= N) al = 0;
	if (ar >= N) ar = 0;
	if (bl >= N) bl = 0;
	if (br >= N) br = 0;
	int tmp;
	if (al < ar) { tmp = al; al = ar; ar = tmp; }
	if (bl < br) { tmp = bl; bl = br; br = tmp; }
	tmp = TBL;
	return tmp;
}

// 四面有一 B 就获胜, 四面皆 A 则失败
#define try
#define try(BL,BR)\
	win = check(bl, br, BL, BR);\
	if (win == B_WINS) {\
		TBL = A_WINS | add | by;\
		return true;\
	} else if (win & A_WINS) {\
		++fail;\
	}

bool fill(int al, int ar, int bl, int br) {
	if (TBL) {
		//printf("%d %d %d %d already decided\n", al, ar, bl, br);
		++collision;
		return false;
	}
	char win = 0, fail = 0, add, by;
	if (al) {
		add = 2;
		if (bl) {
			by = 1;
			try(al+bl, ar);
		} else fail += 1;
		if (br) {
			by = 0;
			try(al+br, ar);
		} else fail += 1;
	} else fail += 2;
	if (ar) {
		add = 0;
		if (bl) {
			by = 1;
			try(al, ar+bl);
		} else fail += 1;
		if (br) {
			by = 0;
			try(al, ar+br);
		} else fail += 1;
	} else fail += 2;
	if (fail == 4) {
		TBL = B_WINS;
		return true;
	}
	return false; // 暂时不能判断
}

void init()
{
	int SIZE = N*(N+1)/2-1;
	table = (char**)malloc(SIZE*sizeof(char*));
	for (int i = 0; i < SIZE; ++i) {
		table[i] = (char*)malloc(SIZE);
		memset(table[i], 0, SIZE);
	}

	ar = 0;
	for (al = 1; al < N; ++al)
		for (bl = N-al; bl < N; ++bl)
			for (br = 0; br <= bl; ++br)
				TBL = ALL;
	cnt = (N*N-1)*N/3; // sum_(k=1)^(N-1) k(k+1)
}

void build_table()
{
	// init
	init();
	//puts("----init----");
	//print();
	
	// first round
	int bound = (N-1)*(N*(N+1)/2-1);
	bound = bound * 2 - (N-1)*(N-1);
	int prevcnt = 0;
	int loop = 0;
	while (cnt < bound && cnt > prevcnt) {
		++loop;
		prevcnt = cnt;
		br = 0;
		for (bl = N-1; bl > 0; --bl)
		for (al = N-1; al > 0; --al)
		for (ar = al; ar >= 0; --ar)
			if (fill(al, ar, bl, br)) ++cnt;

		ar = 0;
		for (al = N-1; al > 0; --al)
		for (bl = N-1; bl > 0; --bl)
		for (br = bl; br >= 0; --br)
			if (fill(al, ar, bl, br)) ++cnt;
	}
	//puts("\n----first round----");
	//print();
	//printf("collision: %d, loop: %d\n", collision, loop);
	//printf("bound: %d, cnt: %d, prevcnt: %d\n", bound, cnt, prevcnt);
	if (cnt < bound) return;

	bound = N*(N+1)/2-1; bound *= bound;
	loop = 0;
	while (cnt < bound && cnt > prevcnt) {
		++loop;
		prevcnt = cnt;
		for (int i = N-1; i > 0; --i)
		for (int j = i; j > 0; --j) {
			bl = i; br = j; al = i;
			for (ar = j; ar > 0; --ar)
				if (fill(al, ar, bl, br)) ++cnt;
			for (al = i-1; al > 0; --al)
			for (ar = al; ar > 0; --ar)
				if (fill(al, ar, bl, br)) ++cnt;

			al = i; ar = j; bl = i;
			for (br = j; br > 0; --br)
				if (fill(al, ar, bl, br)) ++cnt;
			for (bl = i-1; bl > 0; --bl)
			for (br = bl; br > 0; --br)
				if (fill(al, ar, bl, br)) ++cnt;
		}
	}
	//puts("\n----second round----");
	//print();
	//printf("collision: %d, loop: %d\n", collision, loop);
	//printf("bound: %d, cnt: %d, prevcnt: %d\n", bound, cnt, prevcnt);
	if (cnt < bound) return;
}

void play()
{
	srand(time(NULL));

	bool your_turn = true;
	printf("[ADD TO %d]\n"
	"2 players take turns to add a number from the other's hand.\n"
	"the one who get both numbers >= %d wins.\n"
	"you cannot operate on a number that is already >= %d.\n"
	"you have 4 choices: ll/lr/rl/rr\n"
	"will you go first? [y/n] ", N, N, N);

	al = ar = bl = br = 1;
	fgets(buf, 3, stdin);
	printf("you:      %d %d\n", al, ar);
	if (buf[0] == 'n' || buf[0] == 'N')
		your_turn = false;
	else 
		printf("computer: %d %d\n", bl, br);

	int flag = 0;
	while (!flag) {
		if (your_turn) {
			printf("> ");
			if (!fgets(buf, 4, stdin)) {
				break;
			}
			if (buf[0] == 'l') {
				if (al >= N) {
					puts("invalid input");
					continue;
				}
				if (buf[1] == 'l') {
					if (bl >= N) {
						puts("invalid input");
						continue;
					}
					al += bl;
				} else if (buf[1] == 'r') {
					if (br >= N) {
						puts("invalid input");
						continue;
					}
					al += br;
				}
			} else if (buf[0] == 'r') {
				if (ar >= N) {
					puts("invalid input");
					continue;
				}
				if (buf[1] == 'l') {
					if (bl >= N) {
						puts("invalid input");
						continue;
					}
					ar += bl;
				} else if (buf[1] == 'r') {
					if (br >= N) {
						puts("invalid input");
						continue;
					}
					ar += br;
				}
			} else {
				puts("invalid input");
				continue;
			}
			printf("you:     ");
			if (al >= N) { int tmp = al; al = ar; ar = tmp; }
			if (al < N) printf(" %d", al);
			else printf(" (%d)", al);
			if (ar < N) printf(" %d", ar);
			else printf(" (%d)", ar);
			putchar('\n');
			if (al >= N && ar >= N)
				flag = 1;
		} else {
			int solution = check(bl, br, al, ar);
			if (solution == B_WINS) {
				int choice = rand() % 4;
				while (true) {
					if (choice == 0 && bl < N && al < N) {
						bl += al;
						break;
					} else if (choice == 1 && bl < N && ar < N) {
						bl += ar;
						break;
					} else if (choice == 2 && br < N && al < N) {
						br += al;
						break;
					} else if (choice == 3 && br < N && ar < N) {
						br += ar;
						break;
					}
					choice = (choice+1) % 4;
				}
			} else {
				int *a1, *a2, *b1, *b2;
				if (ar >= N || al >= ar) {
					a1 = &al;
					a2 = &ar;
				} else {
					a1 = &ar;
					a2 = &al;
				}
				if (br >= N || bl >= br) {
					b1 = &bl;
					b2 = &br;
				} else {
					b1 = &br;
					b2 = &bl;
				}
				switch (solution) {
					case ALL: *b1 += *a1; break;
					case ALR: *b1 += *a2; break;
					case ARL: *b2 += *a1; break;
					case ARR: *b2 += *a2; break;
				}
			}
			printf("computer:");
			if (bl >= N) { int tmp = bl; bl = br; br = tmp; }
			if (bl < N) printf(" %d", bl);
			else printf(" (%d)", bl);
			if (br < N) printf(" %d", br);
			else printf(" (%d)", br);
			putchar('\n');
			if (bl >= N && br >= N)
				flag = 2;
		}
		your_turn = !your_turn;
	}
	if (flag == 1)
		puts("you win!");
	else if (flag == 2)
		puts("computer wins! you suck!");
	else
		puts("bye");
}

int main(int argc, char **argv)
{
	if (argc > 1) {
		N = atoi(argv[1]);
		if (N < 5 || N > 20) {
			puts("5 <= N <= 20 is required.");
			return 1;
		}
	}
	build_table();
	play();
	return 0;
}
