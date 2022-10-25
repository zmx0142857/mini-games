/*
    Name:        Poker_0.1
    Author:      zmx0142857
    Copyright:
    Link:
    Description: simple card game program

    Use index:
        <back>  0
        ♥ A~K:  1 ~ 13
        ♦ A~K: 14 ~ 26
        ♣ A~K: 27 ~ 39
        ♠ A~K: 40 ~ 52
        Joker: 53 ~ 54

	Use margin:

		╓──────┐
		║A     │
		║♠     │
		║      │
		║      │
		╙──────┘
*/

#include <iostream>
#include <stdexcept>
#include <cstdlib>
using namespace std;

class Poker;
class Card
{
    friend class Poker;
	friend ostream& operator<<(ostream &, const Card &);

	public:
		Card(char d, bool v = true): data(d), visible(v)
            { if (d < 1 || d > 54) throw invalid_argument("No such card!"); }

		char color() const;

	private:
	    // function member
	    Card(const Card &);     // class Card is uncopyable
	    Card& operator=(const Card &);
		char index() const;

		// static member
		static const char style_ascii[15*9][11];

        // data member
		char data;
		bool visible;
};

class Poker
{
	typedef Card Deck[54];

	friend bool operator==(const Card &, const Card &);
	friend bool operator!=(const Card &, const Card &);
	friend bool operator<(const Card &, const Card &);
	friend bool operator>(const Card &, const Card &);
	friend bool operator<=(const Card &, const Card &);
	friend bool operator>=(const Card &, const Card &);

public:
    Poker()

private:
    char value(Card) const;

    Deck data;

};

//----------Initialize Static Const Member------------

const char Card::style_ascii[15*9][11] = { // 15 cards, height = 9, width = 11

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', '\0'},
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', '@', '@', '@', '@', '@', '@', '@', ' ', '|' },
{'|', ' ', '@', '@', '@', '@', '@', '@', '@', ' ', '|' },
{'|', ' ', '@', '@', '@', '@', '@', '@', '@', ' ', '|' },
{'|', ' ', '@', '@', '@', '@', '@', '@', '@', ' ', '|' },
{'|', ' ', '@', '@', '@', '@', '@', '@', '@', ' ', '|' },
{'|', ' ', '@', '@', '@', '@', '@', '@', '@', ' ', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '_', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', '\0'},
{'|', 'A', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', 'A', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', '\0'},
{'|', '2', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '2', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', '\0'},
{'|', '3', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '3', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', '\0'},
{'|', '4', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '4', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', '\0'},
{'|', '5', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '5', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', '\0'},
{'|', '6', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '6', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', '\0'},
{'|', '7', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '7', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', '\0'},
{'|', '8', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '8', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', '\0'},
{'|', '9', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '9', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', '\0'},
{'|', '1', '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '1', '0', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', '\0'},
{'|', 'J', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', '|', ' ', '+', '+', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '|', '(', '\"',',', ')', ' ', ' ', '|' },
{'|', ' ', ' ', 'C', '/', ' ', ' ', '\\','7', ' ', '|' },
{'|', ' ', ' ', '/', '_', '_', '_', '_', '\\',' ', '|' },
{'|', ' ', ' ', '|', '|', ' ', ' ', '|', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', 'J', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', '\0'},
{'|', 'Q', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', ' ', ' ', '@', '@', '@', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', '(', '"', ',', '@', '@', ' ', '|' },
{'|', ' ', ' ', '*', '/', ' ', ' ', '\\','7', ' ', '|' },
{'|', ' ', ' ', '/', '_', '_', '_', '_', '\\',' ', '|' },
{'|', ' ', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', 'Q', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', '\0'},
{'|', 'K', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', '|', ' ', 'w', 'w', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '|', '(', '"', ',', ')', ' ', ' ', '|' },
{'|', ' ', ' ', 'T', '/', ' ', ' ', '\\','7', ' ', '|' },
{'|', ' ', ' ', '/', '_', '_', '_', '_', '\\',' ', '|' },
{'|', ' ', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', 'K', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', '\0'},
{'|', 'J', 'o', 'k', 'e', 'r', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', '\\','A', '/', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '_', '(','\x81',')', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', 'V', 'I', 'V', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', '/', ' ', '\\',' ', ' ', ' ', '|' },
{'|', ' ', ' ', 'J', ' ', ' ', ' ', 'L', ' ', ' ', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '_', '|' },

};

//---------Methods--------------

char Card::color() const
{
    switch (data)
    {
    case 53:
        return 6; // ♠
    case 54:
        return 3; // ♥
    default:
        return (data - 1) / 13 + 3;
    }
}

char Card::index() const
{
    switch (data)
    {
    case 53: case 54:
        return 14;
    default:
        return (data - 1) % 13 + 1;
    }
}

//---------------Friends------------------

ostream& operator<<(ostream &os, const Card &cd)
{
    const char val = (cd.visible ? 9 * cd.index() : 0);
    for (size_t i = 0; i != 9; ++i)
    {
        for (size_t j = 0; j != 11; ++j)
        {
            if (Card::style_ascii[val + i][j] == '$')
                os << cd.color();
            else
                os << Card::style_ascii[val + i][j];
        }
        os << '\n';
    }
    return os;
}

int main()
{
    system("mode con cp select=437");
    system("cls");

    int demand;
    while (cin >> demand)
        try
        {
            cout << Card(demand);
        }
        catch (invalid_argument err)
        {
            cerr << err.what() << endl;
        }

    return 0;
}