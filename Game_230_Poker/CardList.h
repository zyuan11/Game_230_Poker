#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

#ifndef CARDLIST_H_
#define CARDLIST_H_
using namespace std;
#include <iostream>

const enum Suits : uint8_t { Dummy = 0, Clubs = 1, Diamonds = 2, Hearts = 3, Spades = 4 };
const enum CardVal : uint8_t { ValDummy = 1, Val2 = 2, Val3 = 3, Val4 = 4, Val5 = 5, Val6 = 6, Val7 = 7, Val8 = 8, Val9 = 9, Val10 = 10, Jack = 11, Queen = 12, King = 13, Ace = 14 };
const enum HandIdx {a = 0, b = 1, c = 2, d = 3, e = 4, EMPTY = -1 };

typedef struct card {
	CardVal val;
	Suits suit;
	bool f_Keep;
	card* next;
} card;

class CardList {
public:
	card* head;
	CardList();
	~CardList();
	void AddCard(CardVal val_i, Suits suit_i);
	void AddCard(card* card_in);
	bool InCardList(CardVal val_i, Suits suit_i);
	card* RemoveCard(int sel_cnt);
	card* RemoveCard();
	card* RemoveCard(CardVal val_i, Suits suit_i);
	void GetCard(card* card_i);
	void MarkCardKeep(int selCnt);
	string GetCardVal(CardVal val_i);
	string GetCardSuit(Suits suit_i);
	void Deallocate();
	bool IsEmpty();

};
#endif // !CARDLIST_H_