
#ifndef _PARALLELIZATION_H
#define _PARALLELIZATION_H

#include "UCT.h"
#include <thread>
#include <mutex>
#include <ctime>
#include <iostream>
#include <vector>
using namespace std;

mutex m;

struct situation {
	board _board;
	bool _player;
	UCT *finalTree;
	situation(board &inboard, bool player, UCT *_tree) {
		_board = inboard;
		//cout << &inboard << ' ' << &_board << endl;
		_player = player;
		finalTree = _tree;
	}
};

class Parallelization {
	friend void runUCT(situation *s);
private:
	int move;
public:
	Parallelization(board &inboard, bool player, int threadNumber = 4) {
		vector<thread*> runningStack;
		runningStack.reserve(threadNumber);
		bool tmpPlayer = player;
		UCT *finalTree = new UCT(inboard, tmpPlayer);
		finalTree->createAllChildrenIfNone(finalTree->getRoot());
		for (int i = 0; i < threadNumber; ++i) {
			bool tmpPlayer = player;
			situation *s = new situation(inboard, tmpPlayer, finalTree);
			//thread t(runUCT, s);
			runningStack.push_back(new thread(runUCT, s));
		}
		for (int i = 0; i < runningStack.size(); ++i) {
			runningStack[i]->join();
			delete runningStack[i];
		}
		(*finalTree).showTree(0);
		//move = 10;
		move = finalTree->getNextMove();
	}
	int getMove() {
		return move;
	}
};

void runUCT(situation *s) {
	bool player = s->_player;
	UCT tree(s->_board, player);
	int startTime = clock();
	int finishTime = clock();
	int cnt = 0;
	while (finishTime - startTime <= 2500) {
		tree.playOneSequenceInMoGo(player);
		finishTime = clock();
		cnt++;
	}
	printf("thread cnt: %d\n", cnt);
	cout << s->finalTree << endl;
	while (!m.try_lock());
	int flag = tree.addFirstLevel(*(s->finalTree));
	m.unlock();
}

#endif