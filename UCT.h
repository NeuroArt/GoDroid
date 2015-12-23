#ifndef _UCT_H
#define _UCT_H
#include <math.h>
#include <string.h>
#include <stack>
#include <assert.h>
#include "board.h"
#include "montecarlo.h"
#define largeFloat 1000000000000.0

class UCT {
private:
	struct Node {
		int win, total;
		float value() {
			return float(win) / total;
		}
		Node *parent, *lchild, *sibling;
		//board currentBoard;
		int move;
		bool player;
		char color;

		Node() {
			win = 0;
			total = 0;
			parent = NULL;
			lchild = NULL;
			sibling = NULL;
			player = 1;
			move = 0;
		}
		//explore_coeff = 2 * ln(Total Simulations)
		float getUCBValue(float explore_coeff) {
			if (total == 0) return largeFloat;
			return value() + sqrt(explore_coeff / total);
		}
		void addChild(Node *newChild) {
			if (lchild == NULL) {
				lchild = newChild;
				lchild->parent = this;
			}
			else {
				newChild->sibling = lchild;
				lchild = newChild;
				lchild->parent = this;
			}
		}
		void removeChild(Node *delChild) {
			if (lchild == delChild) {
				lchild = lchild->sibling;
				return;
			}
			Node *actChild;
			actChild = lchild;
			while (actChild->sibling != NULL) {
				if (actChild->sibling == delChild) {
					actChild->sibling = actChild->sibling->sibling;
					return;
				}
				actChild = actChild->sibling;
			}
		}
		void freeSubtree(Node *p) {
			if (p == NULL) return;
			p = p->lchild;
			Node *tmp;
			while (p != NULL) {
				freeSubtree(p);
				tmp = p;
				p = p->sibling;
				delete tmp;
			}
		}
		void updateWin() {
			win++;
			total++;
		}
		void updateLoss() {
			total++;
		}
		bool isLeaf() {
			if (lchild == NULL) return 1;
			return 0;
		}
		Node* findBestChild() {
			Node* bestChild = NULL;
			float explore_coeff = log(total) * 2;
			float bestUrgency = -largeFloat;
			Node *p = lchild;
			while (p != NULL) {
				float childUrgency = p->getUCBValue(explore_coeff);
				if (childUrgency > bestUrgency) {
					bestUrgency = childUrgency;
					bestChild = p;
				}
				p = p->sibling;
			}
			return bestChild;
		}
		bool removeWorstChild() {
			Node *worstChild = NULL;
			float worstUrgency = largeFloat;
			float explore_coeff = 2 * log(total);
			Node *p = lchild;
			while (p != NULL) {
				float childUrgency = p->getUCBValue(explore_coeff);
				if (childUrgency < worstUrgency) {
					worstUrgency = childUrgency;
					worstChild = p;
				}
				p = p->sibling;
			}
			if (worstChild == lchild && worstChild->sibling == NULL) return 0;
			removeChild(worstChild);
			freeSubtree(worstChild);
			delete worstChild;
		}
	};
	Node *root;
	board rootBoard;
	Node *garbage;
public:
	UCT(board &inBoard) {
		root = new Node;
		rootBoard = inBoard;
		garbage = new Node;
	}
	~UCT() {
		root->freeSubtree(root);
		delete root;
		/*while (garbage != NULL) {
			Node *p = garbage;
			garbage = garbage->sibling;
			delete p;
		}*/
	}
	board getBoard(Node *p) {
		board currentBoard(rootBoard);
		stack<int> back;
		Node *q = p;
		while (q->parent != NULL) {
			back.push(q->move);
			q = q->parent;
		}
		if (!back.empty())
			back.pop();
		while (!back.empty()) {
			bool player = currentBoard.getcurrentplayer();
			int num = back.top();
			back.pop();
			int coordX = num / SIZE + 1;
			int coordY = num % SIZE + 1;
			currentBoard.play(player, coordX, coordY);
		}
		return currentBoard;
	}
	void createAllChildrenIfNone(Node *p) {
		//assert(p != NULL);
		if (p->lchild == NULL) {
			int fail = 0;
			board currentBoard(getBoard(p));
			for (int randomNumber = 0; randomNumber < 169; ++randomNumber) {
				Node *tmp = new Node();
				board tmpBoard(currentBoard);
				int coordX = randomNumber / SIZE + 1;
				int coordY = randomNumber % SIZE + 1;
				tmp->move = randomNumber;
				bool flag = tmpBoard.getcurrentplayer();
				tmp->player = flag;
				if (tmpBoard.play(flag, coordX, coordY)) {
					p->addChild(tmp);
				}
				else {
					//printf("~~~~~~~~~~~~~~~~~~~~~\n");
					//printf("%d\n", tmp->move);
					//printf("~~~~~~~~~~~~~~~~~~~~~\n");
					delete tmp;
					//fail++;
					//tmp->sibling = garbage;
					//garbage = tmp;
				}
			}
		}
	}
	void update(Node *s, bool winner) {
		while (s != NULL) {
			if (s->player == winner) s->updateWin();
			else s->updateLoss();
			s = s->parent;
		}
	}
	void playOneSequenceInMoGo() {
		Node* p = root;
		createAllChildrenIfNone(p);
		//showTree(0);
		do {
			p = p->findBestChild();
			//if (p == NULL) return;
			if (p != NULL && p->isLeaf()) {
				if (p->total == 0) break;
				createAllChildrenIfNone(p);
				p = p->findBestChild();
				break;
			}
		} while (1);
		if (p != NULL) {
			board currentBoard(getBoard(p));
			montecarlo m(currentBoard);
			update(p, m.getWinner());
		}
		else {
			assert(0);
			board currentBoard(getBoard(p->parent));
			montecarlo m(currentBoard);
			update(p->parent, m.getWinner());
		}	
	}
	int getNextMove() {
		Node *tmp = root->findBestChild();
		return tmp->move;
	}
	void showTree(bool flag = 1) {
		Node *p = root;
		int level = 0;
		while (p != NULL) {
			printf("level-%d ", level++);
			Node* sib = p;
			int levelNum = 0;
			while (sib != NULL) {
				if (flag)
					printf("%d/%d ", sib->win, sib->total);
				else {
					if (sib->total == 0) {
						sib = sib->sibling;
						continue;
					}
					levelNum++;
					printf("%d/%d ", sib->win, sib->total);
				}
				sib = sib->sibling;
			}
			printf("%d\n", levelNum);
			p = p->lchild;
		}
	}
	void showTotal() {
		printf("%d/%d\n", root->win, root->total);
	}

};

#endif