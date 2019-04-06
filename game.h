// game.h: header file for games written in C++.
#ifndef game_h
#define game_h

#include <iostream>
#include <cstring>
#include <sstream>

#ifdef __APPLE__
#error "Mac OS is currently not supported"
#endif

#ifdef __linux__
#error "linux is currently not supported"
#endif

#ifdef __MINGW32__
#include <windows.h>
#include <conio.h>

namespace zmx
{

typedef unsigned short Uint;
typedef short Sint;
enum color{	x000000, x000080, x008000, x008080,
			x800000, x800080, x808000, xc0c0c0,
			x808080, x0000ff, x00ff00, x00ffff,
			xff0000, xff00ff, xffff00, xffffff};

void keep(Sint *ptr, Sint inf, Sint sup); // keep *ptr in range [inf, sup]

class Game
{
	public:
		Game(): game_over(false) {}
		bool game_over;
};

class Screen
{
	public:
		Screen(Uint w, Uint h) { this->resize(w, h); }

		void resize(Uint w, Uint h);
		void set_cursor(Sint x, Sint y) const;
		void set_color(Uint bg, Uint fg=xffffff, bool cls=true) const;
		void loading(Uint loop=3);
		static void clear() { system("cls"); }

	private:
		Uint width;
		Uint height;
		HANDLE handle;
};

}; // namespace zmx

#endif // __MINGW32__

#endif // game_h


/* game.h example

#include "game.h"
using namespace std;
using namespace zmx;

int main()
{
    Game g;
    Screen sc(40, 20);
    sc.loading();
    sc.set_color(x008080, xc0c0c0);
    for (Uint i = 0; !g.game_over; ++i)
    {
        cout << i << endl;
        char key = getch();
        if (key == 27)
            g.game_over = true;
    }
    return 0;
}

*/
