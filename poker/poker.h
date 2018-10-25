/*
    Name:        poker - 0.2
    Author:      Clarence Zhuo
    Description: simple card game program

    Use id:
        <back>  0
        ♥ A~K   1 ~ 13 heart
        ♦ A~K  14 ~ 26 diamond
        ♣ A~K  27 ~ 39 club
        ♠ A~K  40 ~ 52 spade
        Joker: 53 ~ 54

	Use margin:

		╓──────┐
		║A     │
		║♠     │
		║      │
		║      │
		╙──────┘
*/

#ifndef Poker_h
#define Poker_h

#include <iostream>
#include "../codepack/random.h"

class Poker;

class Card
{
friend class Poker;
friend std::wostream &operator<<(std::wostream &, const Card &);

public:
	enum Suit {NULL_SUIT=0, HEART, DIAMOND, CLUB, SPADE};
	enum Rank {BACK=0, ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT,
		NINE, TEN, JACK, QUEEN, KING, joker, JOKER};

	// constructor, destructor
	Card(int id=BACK, bool visible=true);
	Card(Rank r, Suit s=HEART, bool visible=true);
	~Card();

	// operators
	virtual bool operator==(const Card &) const;
	bool operator!=(const Card &) const;
	virtual bool operator<(const Card &) const;
	bool operator>(const Card &) const;
	bool operator<=(const Card &) const;
	bool operator>=(const Card &) const;

	// methods
	int rank() const;
	int suit() const;
	bool is_facecard() const;
	int count() const;
	std::string abbr() const;

	// test
	static void test();
	void dump() const;

private:
	// functions
	Card(const Card &);				// class Card is uncopyable
	Card &operator=(const Card &);

protected:
	int index() const;

	// data
	int id;			// constructor ensures 0 <= id <= 54
	bool visible;

	// static
	static int cnt[55]; 
	static const wchar_t img[16 * 9][11];
	static const wchar_t img_suit[5];
};

//--------Constructors etc--------

Card::Card(int i, bool v): id(0 <= i && i <= 54 ? i : 0), visible(v)
{
	++cnt[id];
}

Card::Card(Rank r, Suit s, bool v): id(0), visible(v)
{
	if (r == BACK)
		id = 0;
	else if (r > 13)
		id = 39 + r;
	else
		id = (s - HEART) * 13 + r;
	++cnt[id];
}

Card::~Card()
{
	--cnt[id];
}

//---------Operators-------------

bool Card::operator==(const Card &rhs) const
{
	return id == rhs.id;
}

bool Card::operator!=(const Card &rhs) const
{
	return !(*this == rhs);
}

bool Card::operator<(const Card &rhs) const
{
	return rank() < rhs.rank()
		|| (rank() == rhs.rank() && suit() < rhs.suit());
}

bool Card::operator>(const Card &rhs) const
{
	return (rhs < *this);
}

bool Card::operator<=(const Card &rhs) const
{
	return !(*this > rhs);
}

bool Card::operator>=(const Card &rhs) const
{
	return !(*this < rhs);
}

std::wostream& operator<<(std::wostream &os, const Card &rhs)
{
    const int begin = (rhs.visible ? 9 * rhs.rank() : 0);
    for (int i = begin; i < begin + 9; ++i)
    {
        for (int j = 0; j < 11; ++j)
        {
			wchar_t tmp = Card::img[i][j];
			os << (tmp != '$' ? tmp : rhs.img_suit[rhs.suit()]);
        }
        os << '\n';
    }
    return os;
}

//---------Methods--------------

int Card::rank() const
{
	if (id == 0)
		return BACK;
	if (id == 53 || id == 54)
		return id - 39;
	return (id - 1) % 13 + 1;
}

int Card::suit() const
{
	if (id == 0)
		return NULL_SUIT;
	if (id == 53)
		return SPADE; // ♠
	if (id == 54)
		return HEART; // ♥
	return (id - 1) / 13 + HEART;
}

int Card::count() const
{
	return cnt[id];
}

std::string Card::abbr() const
{
	if (!visible)
		return "??";
	static const char rank_abbr[] = "?A23456789TJQK";
	static const char suit_abbr[] = "?HDCS";
	int r = rank();
	int s = suit();
	if (r == BACK || s == NULL_SUIT)
		return "??";
	if (r == joker)
		return "jo";
	if (r == JOKER)
		return "JO";
	return std::string(1, rank_abbr[r]) + suit_abbr[s];
}

//--------------Test-------------------

void Card::test()
{
	using namespace std;
	cout << "---- test class Card ----\n";
	Card c1; cout << "c1: "; c1.dump();
	Card c2; cout << "c2: "; c2.dump();
	cout << "c1: "; c1.dump();
	{
		Card c3; cout << "c3: "; c3.dump();
	}
	cout << "c1: "; c1.dump();

	Card c4(Card::SIX, Card::HEART); cout << "c4: "; c4.dump();
	Card c5(Card::JOKER); cout << "c5: "; c5.dump();
}

void Card::dump() const
{
	std::cout << abbr() << " count: " << count() << '\n';
}

//--------Static Member------------

int Card::cnt[55] = {};
const wchar_t Card::img_suit[5] = {'*',L'♥',L'♦',L'♣',L'♠'};
const wchar_t Card::img[16*9][11] = { // 16 cards, height=9, width=11

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', '@', '@', '@', '@', '@', '@', '@', ' ', '|' },
{'|', ' ', '@', '@', '@', '@', '@', '@', '@', ' ', '|' },
{'|', ' ', '@', '@', '@', '@', '@', '@', '@', ' ', '|' },
{'|', ' ', '@', '@', '@', '@', '@', '@', '@', ' ', '|' },
{'|', ' ', '@', '@', '@', '@', '@', '@', '@', ' ', '|' },
{'|', ' ', '@', '@', '@', '@', '@', '@', '@', ' ', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '_', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ' },
{'|', 'A', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', 'A', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ' },
{'|', '2', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '2', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ' },
{'|', '3', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '3', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ' },
{'|', '4', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '4', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ' },
{'|', '5', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '5', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ' },
{'|', '6', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '6', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ' },
{'|', '7', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '7', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ' },
{'|', '8', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '8', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ' },
{'|', '9', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '9', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ' },
{'|', '1', '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', '$', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '$', ' ', ' ', ' ', '$', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '1', '0', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ' },
{'|', 'J', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', '|', ' ', '+', '+', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '|', '(', '\"',',', ')', ' ', ' ', '|' },
{'|', ' ', ' ', 'C', '/', ' ', ' ', '\\','7', ' ', '|' },
{'|', ' ', ' ', '/', '_', '_', '_', '_', '\\',' ', '|' },
{'|', ' ', ' ', '|', '|', ' ', ' ', '|', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', 'J', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ' },
{'|', 'Q', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', ' ', ' ', '@', '@', '@', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', '(', '"', ',', '@', '@', ' ', '|' },
{'|', ' ', ' ', '*', '/', ' ', ' ', '\\','7', ' ', '|' },
{'|', ' ', ' ', '/', '_', '_', '_', '_', '\\',' ', '|' },
{'|', ' ', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', 'Q', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ' },
{'|', 'K', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', '$', ' ', '|', ' ', 'w', 'w', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '|', '(', '"', ',', ')', ' ', ' ', '|' },
{'|', ' ', ' ', 'T', '/', ' ', ' ', '\\','7', ' ', '|' },
{'|', ' ', ' ', '/', '_', '_', '_', '_', '\\',' ', '|' },
{'|', ' ', ' ', ' ', '|', ' ', ' ', '|', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '$', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', 'K', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ' },
{'|', 'J', 'o', 'k', 'e', 'r', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', '\\','A', '/', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '_', '(', '&', ')', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', 'V', 'I', 'V', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', '/', ' ', '\\',' ', ' ', ' ', '|' },
{'|', ' ', ' ', 'J', ' ', ' ', ' ', 'L', ' ', ' ', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '_', '|' },

{' ', '_', '_', '_', '_', '_', '_', '_', '_', '_', ' ' },
{'|', 'J', 'o', 'k', 'e', 'r', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', '\\','A', '/', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', '_', '(', '&', ')', '$', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', 'V', 'I', 'V', ' ', ' ', ' ', '|' },
{'|', ' ', ' ', ' ', '/', ' ', '\\',' ', ' ', ' ', '|' },
{'|', ' ', ' ', 'J', ' ', ' ', ' ', 'L', ' ', ' ', '|' },
{'|', '_', '_', '_', '_', '_', '_', '_', '_', '_', '|' },

};

class Deck
{
friend std::ostream &operator<<(std::ostream &, const Deck &);
public:
    Deck(unsigned decks=1);
	void shuffle() const;
	static void test();

private:
	Card **card;			// array of pointers
	const int SIZE;
};

//--------Constructor & Destructor----------

Deck::Deck(unsigned decks): SIZE(54 * decks)
{
	card = new Card*[SIZE];
	for (int i = 0; i < SIZE; ++i)
		card[i] = new Card(i % 54 + 1);
}

//----------Method-----------

void Deck::shuffle() const
{
	for (int i = 0; i < SIZE; ++i)
	{
		int j = Random::integer(SIZE);
		std::swap(card[i], card[j]);
	}
}

//----------Friend-----------

std::ostream &operator<<(std::ostream &os, const Deck &rhs)
{
	for (int i = 0; i < rhs.SIZE; ++i)
		os << rhs.card[i]->abbr() << (i % 13 == 12 ? "\n" : " ");
	return os;
}

void Deck::test()
{
	using namespace std;
	cout << "---- test class Deck ----\n";
	Deck deck;
	cout << deck << endl;
	deck.shuffle();
	cout << deck << endl;
	deck.shuffle();
	cout << deck << endl;
}

#endif // Poker_h