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

#include "Player.h"
#include "CardList.h"
#include "Judge.h"
#include <iostream>
#include <string>
using std::string;
using namespace std;

//Initialize player's money and create player's hand
void Player::PlayerInit() {
	myMoney = INITMONEY;
	f_NoMoney = false;
	cout << "You have start with $" << myMoney << endl;
	myHand = new CardList();
}

string Player::GetCardIdx(int ind_i) {
	switch (ind_i)
	{
	case 0:
		return "a";
	case 1:
		return "b";
	case 2:
		return "c";
	case 3:
		return "d";
	case 4:
		return "e";
	default:
		return "Error";
	}
}

void Player::MyHandDisplay() {
	card* curr = myHand->head;
	string val;
	string suit;
	string idx;
	
	//cards in my hand printing title
	cout << "\nCards in my Hand: " << endl;

	//increment from dummy
	if (curr->val == ValDummy && curr->next != NULL) {
		curr = curr->next;
	}

	//walk through cards in my hand to print all cards within deck
	for (int i = 0; i < 5; i++) {
		val = myHand->GetCardVal(curr->val);
		suit = myHand->GetCardSuit(curr->suit);
		idx = GetCardIdx(i);
		
		if (curr->f_Keep) {
			cout << idx << ": " << val << " of " << suit << " (kept)" << endl;
		}
		else {
			cout << idx << ": " << val << " of " << suit << endl;
		}

		curr = curr->next;
	}	
}

void Player::InputOp() {
	string op;
	cout << "\n\nOPTIONS..." << endl;
	cout << "- Type the letters for the cards you wish to keep. \(i.e., \"acd\"\)" << endl;
	cout << "- Type \"deck\" to view the cards remaining in the deck." << endl;
	cout << "- Type \"none\" to discard all cards in your hand." << endl;
	cout << "- Type \"all\" to keep all cards in your hand." << endl;
	cout << "- Type \"exit\" to exit the game." << endl;
	cout << "- Type \"swap\" to CHEAT and swap a card in your hand with one in the deck." << endl;
	cout << "YOUR CHOICE: ";
	cin >> op;
	
	if (op == "deck") {
		opCode = OpDeck;
	}
	else if (op == "none") {
		opCode = OpNone;
	}
	else if (op == "all") {
		opCode = OpAll;
	}
	else if (op == "exit") {
		opCode = OpExit;
	}
	else if (op == "swap") {
		opCode = OpSwap;
	}
	else {
		while (!OpCodeValidityChk(op)) {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "Your input is not valid." << endl;
			this->InputOp();
			return;
		}
		opCode = OpReplace;
	}
}

bool Player::OpCodeValidityChk(string op) {
	this->IndToKeepInit();
	for (std::string::size_type i = 0; i < op.size(); ++i) {
		if (op[i] == 'a') {
			IndToKeep[i] = a;
		}
		else if (op[i] == 'b') {
			IndToKeep[i] = b;
		}
		else if (op[i] == 'c') {
			IndToKeep[i] = c;
		}
		else if (op[i] == 'd') {
			IndToKeep[i] = d;
		}
		else if (op[i] == 'e') {
			IndToKeep[i] = e;
		}
		else {
			this->IndToKeepInit();
			return false;
		}
	}
	return true;
}

void Player::IndToKeepInit() {
	for (int i = 0; i < 5; i++) {
		IndToKeep[i] = EMPTY;
	}
}

int Player::GetCurrMoney() {
	return myMoney;
}

void Player::PayAnte() {
	myMoney--;
}

void Player::UpdateMoney(int moneyWon) {

	cout << "You earned $" << moneyWon << endl;
	myMoney += moneyWon;
	
	if (myMoney == 0) {
		f_NoMoney = true;
	}
}

void Player::PlayerFinalization() {
	myHand->Deallocate();
	delete myHand;
}