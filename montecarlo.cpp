#include "montecarlo.h"
#include "testBoard.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

montecarlo::montecarlo(board inBoard) {
	getInitBoard(inBoard);
	run();
}

montecarlo::~montecarlo() {}

void montecarlo::getInitBoard(board inBoard) {
	currentBoard = inBoard;
}

void montecarlo::run() {
	srand((unsigned)time(NULL));
	int step = 0;
	int fault = 0;
	while (step <= 300) {
		vector<int> legalPos = currentBoard.getemptycells();
		int randomNumber = rand() * legalPos.length() / (MAXINT + 1);
		int coordX = legalPos[randomNumber] / SIZE;
		int coordY = legalPos[randomNumber] % SIZE;
		if(currentBoard.play(currentBoard.currentPlayer, coordX, coordY)) {
			step++;
			fault = 0;
		}
		else {
			fault++;
		}
		if(fault >= 50) break;
	}
	winner = currentBoard.winner;
}

int montecarlo::getWinner() {
	return winner;
}

int main() {
	board tmp;
	montecarlo m(tmp);
	int _winner = m.getWinner();
	printf("%d\n", _winner);
}