#include "montecarlo.h"
#include "board.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

montecarlo::montecarlo(board &inBoard) {
	getInitBoard(inBoard);
	run();
}

montecarlo::~montecarlo() {}

void montecarlo::getInitBoard(board &inBoard) {
	currentBoard = inBoard;
}

void montecarlo::run() {
	srand((unsigned)time(NULL));
	int step = 0;
	int fault = 0;
	while (fault <= 1000) {
		int randomNumber = rand() * 169 / (MAXINT + 1);
		int coordX = randomNumber / SIZE + 1;
		int coordY = randomNumber % SIZE + 1;
		bool flag = currentBoard.getcurrentplayer();
		if (currentBoard.play(flag, coordX, coordY)) {
			step++;
		}
		else {
			fault++;
		}
		if(fault >= 50) break;
	}
	winner = currentBoard.judge() > 0?1:0;
}

int montecarlo::getWinner() {
	return winner;
}
