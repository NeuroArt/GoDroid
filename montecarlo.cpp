#include "montecarlo.h"
#include "board.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "pattern.h"
#include "pattern_dyn.h"

using namespace std;

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
	int fault = 0;
	int last = 0; // last enemy move, [0,13]
	bool flag=true;
	int triedtimes=0;
	while (flag) {
		bool walked = false;
		bool flag1 = false;
		bool flag2 = false;
		bool flag3 = false;
		bool flagP = false;

		set<short>* ataripositionalley = player ? (&currentBoard.ataripositionforblack) : (&currentBoard.ataripositionforwhite);
		set<short>* validsetalley = player ? (&currentBoard.validsetforblack) : (&currentBoard.validsetforwhite);
		set<short>* ataripositionenemy = (!player) ? (&currentBoard.ataripositionforblack) : (&currentBoard.ataripositionforwhite);
		set<short>* validsetenemy = !player ? (&currentBoard.validsetforblack) : (&currentBoard.validsetforwhite);
		set<short>::iterator iter;
		int patmove;
		while (triedtimes <= 300 && !ataripositionalley->empty()){
			triedtimes++;
			iter = ataripositionalley->begin();
			int randomNumber = rand() % ataripositionalley->size();
			for (int i = 0; i < randomNumber; i++)
				iter++;
			int coordX = (*iter - 1) / BOARDSIZE + 1;
			int coordY = (*iter - 1) % BOARDSIZE + 1;
			last = *iter;
			if (currentBoard.play(player, coordX, coordY)) {
				player = !player;
				walked = true;
				flag1 = true;
				break;
			}
			ataripositionalley->erase(*iter);//����û�п�����ȫ
		}
		for (int k = 0; k < 9; ++k){
			int target = rand() % (BOARDSIZE*BOARDSIZE);
			if (currentBoard.emptytest(target))
				if (currentBoard.play(player, (target - 1) / BOARDSIZE + 1, (target - 1) % BOARDSIZE + 1)) {
					player = !player;
					walked = true;
					flagP = true;
					break;
				}
		}
		while (triedtimes <= 300 && !walked && last != 0){
			/*pattern_dyn用法是这个样子的：
			typeTrans tt[10];
			int cnt = matchPatternDyn(&currentBoard, player, (last-1)/BOARDSIZE+1, (last-1)%BOARDSIZE+1, 3, 3, tt);
			if(cnt > 0) printf("dynamic hit\n");
			for(int i = 0; i < cnt; ++i) printTrans(tt[i]);
			*/
			patmove = findPattern(&currentBoard, player, (last-1)/BOARDSIZE+1, (last-1)%BOARDSIZE+1);
			if (patmove){
				//printf("pattern hit\n");
				triedtimes++;
				int coordX = (patmove - 1) / BOARDSIZE + 1;
				int coordY = (patmove - 1) % BOARDSIZE + 1;
				last = patmove;
				if (currentBoard.play(player, coordX, coordY)) {
					player = !player;
					walked = true;
					flagP = true;
					break;
				}
			}
			else break;
		}
		while (triedtimes <= 300 && !walked&&!ataripositionenemy->empty()){
			triedtimes++;
			iter = ataripositionenemy->begin();
			int randomNumber = rand() % ataripositionenemy->size();
			for (int i = 0; i < randomNumber; i++)
				iter++;
			int coordX = (*iter - 1) / BOARDSIZE + 1;
			int coordY = (*iter - 1) % BOARDSIZE + 1;
			last = *iter;
			if (currentBoard.play(player, coordX, coordY)) {
				player = !player;
				walked = true;
				flag2 = true;
				break;
			}
			ataripositionenemy->erase(*iter);
		}
		while (triedtimes <= 300 && !walked&&!validsetalley->empty()){
			triedtimes++;
			iter = validsetalley->begin();
			int randomNumber = rand() % validsetalley->size();
			for (int i = 0; i < randomNumber; i++)
				iter++;
			int coordX = (*iter - 1) / BOARDSIZE + 1;
			int coordY = (*iter - 1) % BOARDSIZE + 1;
			last = *iter;
			if (currentBoard.play(player, coordX, coordY)) {
				player = !player;
				walked = true;
				flag3 = true;
				break;
			}
			validsetalley->erase(*iter);
		}
		if (!flag1&&!flag2&&!flag3 && !flagP){
			player = !player;
			walked = true;
		}
		if (triedtimes > 300)
			flag = false;
		if (validsetalley->empty() && validsetenemy->empty() && ataripositionalley->empty() && ataripositionenemy->empty()){
			flag = false;
		}
		/*currentBoard.showboard();
		printf("%d\n", last);
		system("pause");
		for (iter=ataripositionalley->begin();iter!=ataripositionalley->end();iter++)
		 	printf("%d ",*iter);
		printf("\n");
		for (iter=ataripositionenemy->begin();iter!=ataripositionenemy->end();iter++)
		 	printf("%d ",*iter);
		printf("\n");
		for (iter=validsetalley->begin();iter!=validsetalley->end();iter++)
		 	printf("%d ",*iter);
		printf("\n");
		for (iter=validsetenemy->begin();iter!=validsetenemy->end();iter++)
		 	printf("%d ",*iter);
		printf("\n");*/
	}
	/*currentBoard.showboard();
	system("pause");
	printf("\n");*/
	winner = currentBoard.judge()>0?1:0;
}

bool montecarlo::getWinner() {
	return winner;
}

bool montecarlo::checkposition(int move, bool player){
	cell target = player ? black : white;
	int coordx = move / BOARDSIZE + 1;
	int coordy = move % BOARDSIZE + 1;
	return currentBoard.goban[coordx][coordy].c == target;
}