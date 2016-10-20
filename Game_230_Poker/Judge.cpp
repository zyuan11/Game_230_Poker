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

#include "Judge.h"
#include "CardList.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;
using std::string;

void Judge::DeckInit() {
	//allocate deck memory
	deck = new CardList();
	
	Suits arr[5] = { Dummy, Clubs, Diamonds, Hearts, Spades };
	Suits cardSuit;
	CardVal arr2[14] = { ValDummy, Val2, Val3, Val4, Val5, Val6, Val7, Val8, Val9, Val10, Jack, Queen, King, Ace };
	CardVal cardVal;
	
	deckSize = 0;
	//initialize deck with all 52 cards
	for (int i = 1; i < 5; i++) {
		cardSuit = arr[i];
		
		for (int j = 1; j < 14; j++) {
			cardVal = arr2[j];
			deck->AddCard(cardVal, cardSuit);
			deckSize++;
		}
	}
}

void Judge::DisplayDeck()
{
	card* curr = deck->head;
	string val;
	string suit;
	
	DealDeckEmpty();

	//deck printing title
	cout << "Cards in deck: " << endl;
	
	//increment from dummy
	if (curr->val == ValDummy && curr->next != NULL) {
		curr = curr->next;
	}
	
	//walk through deck to print all cards within deck
	while (curr != NULL ) {
		val = deck->GetCardVal(curr->val);
		suit = deck->GetCardSuit(curr->suit);
		cout << val << " of " << suit << " | ";
		curr = curr->next;
	}
	cout << endl;

	cout << "There are " << deckSize << " cards in deck." << endl;
}

void Judge::DisplayDiscard() {
	card* curr = discard->head;
	string val;
	string suit;

	//discard printing title
	cout << "Cards in discard: " << endl;

	//increment from dummy
	if (curr->val == ValDummy && curr->next != NULL) {
		curr = curr->next;
	}

	//walk through deck to print all cards within deck
	while (curr != NULL) {
		val = deck->GetCardVal(curr->val);
		suit = deck->GetCardSuit(curr->suit);
		cout << val << " of " << suit << " | ";
		curr = curr->next;
	}
	cout << endl;
}

void Judge::DiscardInit() {
	discard = new CardList();
	discardSize = 0;
}

//return a randomly selected index number of deck
int Judge::SelectCard() {
	return (rand() % deckSize);
}

//move cards in discard to deck when deck is empty
void Judge::DealDeckEmpty() {
	if (deck->IsEmpty()) {
		deck->head->next = discard->head->next;
		discard->head->next = NULL;
		deckSize = discardSize;
		discardSize = 0;
	}
}

//give hand five new cards
void Judge::GiveNewHand() {
	srand(time(NULL));
	for (int i = 1; i <= 5; i++) {
		DealDeckEmpty();
		player.myHand->GetCard(deck->RemoveCard(SelectCard()));
		deckSize--;
	}
}

//remove all five cards in hand to discard
void Judge::ClearHand() {
	while (!player.myHand->IsEmpty()) {
		discard->GetCard(player.myHand->RemoveCard(0));
		discardSize++;
	}
}

//clear flag of keep of every card in hand
void Judge::ClearF_Kept() {
	card* curr = player.myHand->head->next;

	while (curr != NULL) {
		curr->f_Keep = false;
		curr = curr->next;
	}
}

void Judge::DiscardCards() {
	card* cardToDiscard;

	cardToDiscard = player.myHand->RemoveCard();
	while (cardToDiscard != NULL) {
		discard->GetCard(cardToDiscard);
		discardSize++;
		cardToDiscard = player.myHand->RemoveCard();
	}
}

//replace selected cards from hand with newly randomly selected cards from deck
void Judge::Replace() {
	int cnt = 0;

	//remove selected cards from hand to discard
	while (player.IndToKeep[cnt] != EMPTY && cnt < HANDSIZE) {
		player.myHand->MarkCardKeep((int)player.IndToKeep[cnt]);
		cnt++;
	}

	DiscardCards();

	//randomly get new cards from deck to hand (make sure hand has five cards in total)
	cnt = HANDSIZE - cnt;
	srand(time(NULL));
	while (cnt > 0) {
		DealDeckEmpty();
		player.myHand->GetCard(deck->RemoveCard(SelectCard()));
		deckSize--;
		cnt--;
	}
}

//replace all five cards from hand with newly randomly selected cards from deck
void Judge::ReplaceAll() {
	ClearHand();
	GiveNewHand();
}

void Judge::DebugGiveNewHand() {
	player.myHand->AddCard(Ace, Clubs);
	player.myHand->AddCard(Ace, Spades);
	player.myHand->AddCard(Val4, Clubs);
	player.myHand->AddCard(Val6, Hearts);
	player.myHand->AddCard(Val6, Clubs);
}

//check if cards in hand match any patterns
WinPatterns Judge::CheckWin() {
	if (IsRoyalFlush()) {
		return RoyalFlush;
	}
	else if (IsStraightFlush()) {
		return StraightFlush;
	}
	else if (IsFourKind()) {
		return FourKind;
	}
	else if (IsFullHouse()) {
		return FullHouse;
	}
	else if (IsFlush()) {
		return Flush;
	}
	else if (IsStraight()) {
		return Straight;
	}
	else if (IsThreeKind()) {
		return ThreeKind;
	}
	else if (IsTwoPair()) {
		return TwoPair;
	}
	else if (IsOnePair()) {
		return OnePair;
	}

	cout << "\nNo Poker hand scored! " << endl;
	return LOSE;
}
#pragma region CheckWin

bool Judge::IsRoyalFlush() {
	card* prev = player.myHand->head;
	card* curr = player.myHand->head->next;

	while (curr->next != NULL) {
		if (curr->next->val != prev->next->val + 1 ||
			curr->next->suit != prev->next->suit) {
			return false;
		}
		curr = curr->next;
		prev = prev->next;
	}

	if (curr->val != Ace) {
		return false;
	}

	cout << "Congratulations! You got RoyalFlush and earned $800!" << endl;
	return true;
}

bool Judge::IsStraightFlush() {
	card* prev = player.myHand->head;
	card* curr = player.myHand->head->next;

	while (curr->next != NULL) {
		if (curr->next->val != prev->next->val + 1 ||
			curr->next->suit != prev->next->suit) {
			return false;
		}
		curr = curr->next;
		prev = prev->next;
	}

	cout << "Congratulations! You got Straight Flush and earned $50!" << endl;
	return true;

}

bool Judge::IsFourKind() {
	card* prev = player.myHand->head;
	card* curr = player.myHand->head->next;
	int chkCnt = 0;

	while (curr->next != NULL) {
		if (curr->next->val == prev->next->val) {
			chkCnt++;
		}
		else {
			if (chkCnt != 0 && chkCnt != 3) {
				return false;
			}
		}
		curr = curr->next;
		prev = prev->next;
	}

	if (chkCnt == 3) {
		cout << "Congratulations! You got Four of a Kind and earned $25!" << endl;
		return true;
	}
	else {
		return false;
	}
}

bool Judge::IsFullHouse() {
	card* prev = player.myHand->head;
	card* curr = player.myHand->head->next;
	bool flag1 = false;
	bool flag2 = false;
	int cnt = 0;

	while (curr->next != NULL) {
		if (curr->next->val == prev->next->val) {
			cnt++;
		}
		else if (curr->next->val != prev->next->val) {
			if (cnt == 1 && !flag1) {
				flag1 = true;
			}
			else if (cnt == 1 && flag1) {
				return false;
			}
			else if (cnt == 2) {
				flag2 = true;
			}
			cnt = 0;
		}

		curr = curr->next;
		prev = prev->next;
	}

	if ((flag1 && (cnt == 2)) || (flag2 && (cnt == 1))) {
		cout << "Congratulations! You got Full House and earned $9!" << endl;
		return true;
	}

	return false;
}

bool Judge::IsFlush() {
	card* prev = player.myHand->head;
	card* curr = player.myHand->head->next;

	while (curr->next != NULL) {
		if (curr->next->suit != prev->next->suit) {
			return false;
		}
		curr = curr->next;
		prev = prev->next;
	}
	cout << "Congratulations! You got Flush and earned $6!" << endl;
	return  true;
}

bool Judge::IsStraight() {
	card* prev = player.myHand->head;
	card* curr = player.myHand->head->next;

	while (curr->next != NULL) {
		if (curr->next->val != prev->next->val + 1) {
			return false;
		}
		curr = curr->next;
		prev = prev->next;
	}

	cout << "Congratulations! You got Straight and earned $4!" << endl;
	return true;
}

bool Judge::IsThreeKind() {
	card* prev = player.myHand->head;
	card* curr = player.myHand->head->next;
	int cnt = 0;

	while (curr->next != NULL) {
		if (curr->next->val == prev->next->val) {
			cnt++;
		}
		else {
			if (cnt == 2) {
				cout << "Congratulations! You got Three of a Kind and earned $3!" << endl;
				return true;
			}
			cnt = 0;
		}
		curr = curr->next;
		prev = prev->next;
	}

	if (cnt == 2) {
		cout << "Congratulations! You got Three of a Kind and earned $3!" << endl;
		return true;
	}
	return  false;
}

bool Judge::IsTwoPair() {
	card* prev = player.myHand->head;
	card* curr = player.myHand->head->next;
	bool flag = false;
	int cnt = 0;

	while (curr->next != NULL) {
		if (curr->next->val == prev->next->val) {
			cnt++;
		}
		else {
			if (cnt == 1 && !flag) {
				flag = true;
			}
			else if (cnt == 1 && flag) {
				cout << "Congratulations! You got Tow Pair and earned $2!" << endl;
				return true;
			}
			cnt = 0;
		}
		curr = curr->next;
		prev = prev->next;
	}

	if (flag && cnt == 1) {
		cout << "Congratulations! You got Tow Pair and earned $2!" << endl;
		return true;
	}

	return  false;
}

bool Judge::IsOnePair() {
	card* prev = player.myHand->head;
	card* curr = player.myHand->head->next;

	while (curr->next != NULL) {
		if ((curr->next->val == prev->next->val) && (curr->next->val >= Jack)) {
			cout << "Congratulations! You got One Pair (Jacks or better) and earned $1!" << endl;
			return true;
		}
		curr = curr->next;
		prev = prev->next;
	}
	return  false;
}

#pragma endregion

void Judge::Swap() {
	char idx;
	int val;
	char suit;
	Suits suit_i;
	bool f_IdxValid = false;
	bool f_CardValValid = false;
	bool f_SuitValid = false;


	while (!f_IdxValid) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Enter the letter of the card in hand: ";
		cin >> idx;
		cout << endl;
		f_IdxValid = SwapIdxValidityChk(idx);
		if (!f_IdxValid) {
			cout << "Invalid Input ! " << endl;
		}
	}

	while (!f_CardValValid) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Enter the value (2-10, 11 for Jack, 12 for Queen, 13 for King, 14 for Ace) of the card in the dec to swap with: ";
		cin >> val;
		cout << endl;
		f_CardValValid = SwapCardValValidityChk(val);
		if (!f_CardValValid) {
			cout << "Invalid Input ! " << endl;
		}
	}

	while (!f_SuitValid) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Enter the suit (c for Club, d for Diamond, h for heart, s for Spade) of the card in the deck to swap with: ";
		cin >> suit;
		cout << endl;
		f_SuitValid = SwapSuitValidityChk(suit);
		if (!f_SuitValid) {
			cout << "Invalid Input ! " << endl;
		}
	}

	suit_i = SwapLetterToSuits(suit);
	if (!deck->InCardList((CardVal)val, suit_i)) {
		cout << "Invalid deck card." << endl;
		player.MyHandDisplay();
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		Op();
		return;
	}
	else {
		deck->GetCard(player.myHand->RemoveCard(SwapLetterToNum(idx)));
		player.myHand->GetCard(deck->RemoveCard((CardVal)val, suit_i));
		player.MyHandDisplay();
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		Op();
		return;
	}
}
#pragma region SwapMethods
int Judge::SwapLetterToNum(char letter) {
	int returnval;

	switch (letter)
	{
	case 'a':
		returnval = 0;
		break;

	case 'b':
		returnval = 1;
		break;

	case 'c':
		returnval = 2;
		break;

	case 'd':
		returnval = 3;
		break;

	case 'e':
		returnval = 4;
		break;

	default:
		returnval = -1;
		break;
	}
	return returnval;
}
Suits Judge::SwapLetterToSuits(char suit_i) {
	switch (suit_i)
	{
	case 'c':
		return Clubs;

	case 'd':
		return Diamonds;

	case 'h':
		return Hearts;

	case 's':
		return Spades;

	default:
		return Dummy;
	}
}
bool Judge::SwapIdxValidityChk(char idx_i) {
	if (idx_i != 'a' && idx_i != 'b' && idx_i != 'c' && idx_i != 'd' && idx_i != 'e') {
		return false;
	}
	else {
		return true;
	}
}
bool Judge::SwapCardValValidityChk(int val_i) {
	if (val_i <= 14 && val_i >= 2) {
		return true;
	}
	else {
		return false;
	}
}
bool Judge::SwapSuitValidityChk(char suit_i) {
	if (suit_i != 'c' && suit_i != 'd' && suit_i != 'h' && suit_i != 's') {
		return false;
	}
	else {
		return true;
	}
}
#pragma endregion


//get input command from player and operate
void Judge::Op() {
	player.InputOp();
	switch (player.opCode)
	{
	case OpDeck:
		DisplayDeck();
		player.MyHandDisplay();
		Op();
		break;

	case OpNone:
		ReplaceAll();
		player.MyHandDisplay();
		break;
	
	case OpAll:
		player.MyHandDisplay();
		break;

	case OpExit:
		EndGame();
		break;
	
	case OpSwap:
		Swap();
		break;

	case OpReplace:
		Replace();
		player.MyHandDisplay();
		break;

	case OPERROR:
		cout << "Judge::Op()::OPERROR error" << endl;
		break;
	
	default:
		cout << "Judge::Op()::Default error" << endl;
		break;
	}
}

void Judge::UpdateMoneyBeforeNewTurn() {
	player.PayAnte();
	cout << "\nYou pay a $1 ante and now have $" << player.GetCurrMoney() << endl;
}

void Judge::WaitNewTurn() {
	cout << "\n-------------------Press ENTER to start a new turn-------------------";
	do {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
	} while (cin.get() != '\n');
}

//start new turn
void Judge::StartTurn() {
	UpdateMoneyBeforeNewTurn();
	GiveNewHand();
	player.MyHandDisplay();
	Op();
	if (!f_EndGame) {
		player.UpdateMoney((int)CheckWin());
		cout << "You now have $" << player.GetCurrMoney() << endl;
	}
}

void Judge::EndTurn() {
	ClearF_Kept();
	ClearHand();
}

//initialize deck, discard, player
void Judge::GameInit() {
	DeckInit();
	DiscardInit();
	player.PlayerInit();
}

void Judge::Finalization() {
	deck->Deallocate();
	delete deck;
	discard->Deallocate();
	delete discard;
	player.PlayerFinalization();
}

void Judge::EndGame() {
	f_EndGame = true;
}

void Judge::StartGame()  {
	f_EndGame = false;
	GameInit();

	while (!f_EndGame && !player.f_NoMoney) {
		StartTurn();
		EndTurn();
		if (!f_EndGame) {
			WaitNewTurn();
		}
	}

	if (player.f_NoMoney) {
		cout << "You have no money to pay for ante. Thanks for your playing!" << endl;
	}

	Finalization();
}


