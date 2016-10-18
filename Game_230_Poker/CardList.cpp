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

#include  "cardlist.h"
using namespace std;
using std::string;
#include <iostream>

CardList::CardList(void) {
	head = new card();
	head->val = ValDummy;
	head->suit = Dummy;
	head->f_Keep = false;
	head->next = NULL;
}

CardList::~CardList() {}

void CardList::AddCard(CardVal val_i, Suits suit_i) {
	//create newcard and assign values
	card* newCard = new card();
	newCard->val = val_i;
	newCard->suit = suit_i;
	newCard->f_Keep = false;
	newCard->next = NULL;

	card* prev = head;
	card* curr = head->next;
	
	while (curr != NULL) {
		
		//when input value is less than current value
		if (curr->val > val_i) {
			newCard->next = curr;
			break;
		}
		
		//when input value is the same as current value 
		//and input suit has lower order than current suit
		if (curr->val == val_i && curr->suit > suit_i) {
			newCard->next = curr;
			break;
		}

		prev = prev->next;
		curr = curr->next;
	}
	prev->next = newCard;

}

void CardList::AddCard(card* card_in) {
	card* prev = head;
	card* curr = head->next;

	while (curr != NULL) {

		//when input value is less than current value
		if (curr->val > card_in->val) {
			card_in->next = curr;
			break;
		}

		//when input value is the same as current value 
		//and input suit has lower order than current suit
		if (curr->val == card_in->val && curr->suit > card_in->suit) {
			card_in->next = curr;
			break;
		}

		prev = prev->next;
		curr = curr->next;
	}
	prev->next = card_in;
}

bool CardList::InCardList(CardVal val_i, Suits suit_i) {
	card* curr = head->next;

	while (curr != NULL) {
		if (curr->val == val_i && curr->suit == suit_i) {
			return true;
		}
		curr = curr->next;
	}

	return false;
}

card* CardList::RemoveCard(int selCnt) {
	card* prev = head;
	card* curr = head->next;
	int search_cnt = 0;
	
	//find selected card
	while (search_cnt < selCnt) {
		prev = prev->next;
		curr = curr->next;
		search_cnt++;
	}

	//remove selected card
	prev->next = curr->next;
	curr->next = NULL;

	return curr;
}

card* CardList::RemoveCard() {
	card* prev = head;
	card* curr = head->next;

	while (curr != NULL) {
		if (!curr->f_Keep) {
			prev->next = curr->next;
			return curr;
		}
		prev = prev->next;
		curr = curr->next;
	}

	return NULL;
}

card* CardList::RemoveCard(CardVal val_i, Suits suit_i) {
	card* prev = head;
	card* curr = head->next;

	while (curr != NULL) {
		if (curr->val == val_i && curr->suit == suit_i) {
			prev->next = curr->next;
			return curr;
		}
		prev = prev->next;
		curr = curr->next;
	}

	return NULL;
}

void CardList::GetCard(card* card_i) {
	this->AddCard(card_i);
}

void CardList::MarkCardKeep(int selCnt) {
	card* curr = head->next;
	int search_cnt = 0;

	while (search_cnt < selCnt) {
		curr = curr->next;
		search_cnt++;
	}

	curr->f_Keep = true;
}

bool CardList::IsEmpty() {
	if (head->next == NULL) {
		return true;
	}
	else {
		return false;
	}
}

string CardList::GetCardVal(CardVal val_i) {
	string val;
	switch (val_i)
	{
	case ValDummy:
		val = "valdummy";
		break;
	case Val2:
		val = "2";
		break;
	case Val3:
		val = "3";
		break;
	case Val4:
		val = "4";
		break;
	case Val5:
		val = "5";
		break;
	case Val6:
		val = "6";
		break;
	case Val7:
		val = "7";
		break;
	case Val8:
		val = "8";
		break;
	case Val9:
		val = "9";
		break;
	case Val10:
		val = "10";
		break;
	case Jack:
		val = "Jack";
		break;
	case Queen:
		val = "Queen";
		break;
	case King:
		val = "King";
		break;
	case Ace:
		val = "Ace";
		break;
	default:
		val = "valdefault";
		break;
	}
	return val;
}

string CardList::GetCardSuit(Suits suit_i) {
	string suit;
	switch (suit_i)
	{
	case Dummy:
		suit = "SuitDummy";
		break;
	case Clubs:
		suit = "Club";
		break;
	case Diamonds:
		suit = "Diamond";
		break;
	case Hearts:
		suit = "Heart";
		break;
	case Spades:
		suit = "Spade";
		break;
	default:
		break;
	}
	return suit;
}

void CardList::Deallocate (){
	card* prev = head;
	card* curr = head->next;

	while (curr != NULL) {
		delete prev;
		prev = curr;
		curr = curr->next;
	}
	delete prev;
}
