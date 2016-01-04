#include "board.h"

#ifndef _MONTECARLO_H
#define _MONTECARLO_H

class montecarlo {
private:
	board currentBoard;
	bool player;
	bool winner;
public:
	montecarlo(board &inBoard,bool bplayer);
	~montecarlo();
	void getInitBoard(board &inBoard);
	void run();
	bool getWinner();
};

#endif