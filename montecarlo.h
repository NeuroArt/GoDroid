#ifndef _MONTECARLO_H
#define _MONTECARLO_H

#include "testBoard.h"

#define MAXINT 32767

class montecarlo {
private:
	board currentBoard;
	int winner;
public:
	montecarlo(board inBoard);
	~montecarlo();
	void getInitBoard(board inBoard);
	void run();
	int getWinner();
};

#endif