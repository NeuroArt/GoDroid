#include "montecarlo.h"
#include "board.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

montecarlo::montecarlo(board &inBoard,bool bplayer) {
	getInitBoard(inBoard);
	player = !bplayer;
	run();
}

montecarlo::~montecarlo() {}

void montecarlo::getInitBoard(board &inBoard) {
	currentBoard = inBoard;
}

void montecarlo::run() {
	int step = 0;
	int fault = 0;
	//printf("random: ");
	while (fault <= 1000) {
		int randomNumber = rand() * (SIZE*SIZE) / (RAND_MAX + 1);
		//printf("%d ", randomNumber);
		int coordX = randomNumber / SIZE + 1;
		int coordY = randomNumber % SIZE + 1;
		if (currentBoard.play(player, coordX, coordY)) {
			player = !player;
		}
		else {
			fault++;
		}
	}
	//printf("\n");
	winner = currentBoard.judge() > 0?1:0;
}

bool montecarlo::getWinner() {
	return winner;
}
