#include "poker.h"
using namespace std;

int main()
{
	// Card::test();
	// Deck::test();
	int i;
	while (cin >> i)
		wcout << Card(i) << '\n';
	return 0;
}
