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

#ifndef PLAYER_H_
#define PLAYER_H_
#include "CardList.h"

const int INITMONEY = 10;
const int ANTE = 1;
const int HANDSIZE = 5;

const enum Options { OPERROR = 0, OpDeck = 1, OpNone = 2, OpAll = 3, OpExit = 4, OpSwap = 5, OpReplace = 6 };

class Player {
	
private:
	int myMoney;
public:
	CardList* myHand;
	Options opCode;
	HandIdx IndToKeep[HANDSIZE];
	bool f_NoMoney;

	void PlayerInit();
	string GetCardIdx(int ind_i);
	void MyHandDisplay();
	void InputOp();
	void IndToKeepInit();
	bool OpCodeValidityChk(string op);
	int GetCurrMoney();
	void PayAnte();
	void UpdateMoney(int moneyWon);
	void PlayerFinalization();
};

#endif // !PLAYER_H_