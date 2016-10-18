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

#ifndef JUDGE_H_
#define JUDGE_H_
#include "Player.h"


const enum WinPatterns { RoyalFlush = 800, StraightFlush = 50, FourKind = 25, FullHouse = 9, Flush = 6, Straight = 4, ThreeKind = 3, TwoPair = 2, OnePair = 1, LOSE = 0 };

class Judge {
private:
	Player player;
	CardList* deck;
	CardList* discard;
	int deckSize;
	int discardSize;
	bool f_EndGame;
	
	void DeckInit();
	void DiscardInit();
	void DisplayDeck();
	void DisplayDiscard();
	
	int SelectCard();
	void DealDeckEmpty();
	void GiveNewHand();
	void ClearHand();
	void ClearF_Kept();
	void DiscardCards();
	void Replace();
	void ReplaceAll();
	int SwapLetterToNum(char letter);
	Suits SwapLetterToSuits(char suit_i);
	bool SwapIdxValidityChk(char idx_i);
	bool SwapCardValValidityChk(int val_i);
	bool SwapSuitValidityChk(char suit_i);
	void Swap();
	void Op();

	void DebugGiveNewHand();

	WinPatterns CheckWin();
#pragma region ChkWinMethods
	bool IsRoyalFlush();
	bool IsStraightFlush();
	bool IsFourKind();
	bool IsFullHouse();
	bool IsFlush();
	bool IsStraight();
	bool IsThreeKind();
	bool IsTwoPair();
	bool IsOnePair();
#pragma endregion
	
	void UpdateMoneyBeforeNewTurn();
	
	void GameInit();
	void WaitNewTurn();
	void StartTurn();		
	void EndTurn();
	void EndGame();
	void Finalization();
	
public:
	void StartGame();
};

#endif // !JUDGE_H_#pragma once
