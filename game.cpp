// Implementation for game.h: useful classes for games written in C++.
#include "game.h"

namespace zmx {

void keep(Sint *ptr, Sint inf, Sint sup)
{
    *ptr = *ptr < inf ? inf
                      : (*ptr > sup ? sup : *ptr);
}

void Screen::resize(Uint w, Uint h)
{
	width = w;
	height = h;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);

	std::ostringstream os("mode con");
	os << " lines=" << height << " cols=" << width;
	system(os.str().c_str());
	clear();
}

void Screen::set_cursor(Sint x, Sint y) const
{
	COORD pos = {x, y};
    SetConsoleCursorPosition(handle, pos);
}

void Screen::set_color(Uint bg, Uint fg, bool cls) const
{
	SetConsoleTextAttribute(handle, bg * 0x10 + fg);
	if (cls)
        clear();
}

void Screen::loading(Uint loop)
{
    for (Uint i = 0; i != loop; ++i)
    {
        std::cout << "LOADING";
        Sleep(255);
        for (Uint j = 0; j != 3; ++j)
        {
            std::cout<< '.';
            Sleep(255);
        }
        std::cout << "\r          \r";
    }
    clear();
}

}; // namespace zmx

