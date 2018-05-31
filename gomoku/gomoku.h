#include <iostream>
#include <vector>
#include "game.h"
using namespace zmx;

class Gomoku: public Game
{
	public:
		Gomoku();

	protected:
		static const Uint ROAD = 15;
		static const std::string token[11];
		Uint board[ROAD][ROAD];
		std::vector<COORD> history;
}

const std::string Gomoku::token[11] = {
	"┌─","┬─","┐","├─","┼─","┤","└─","┴─","┘","@","O"
};
