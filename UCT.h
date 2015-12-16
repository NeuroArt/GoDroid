#ifndef _UCT_H
#define _UCT_H
#include <math.h>
#include "testBoard.h"
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
		board currentBoard;
		char color;

		Node(board inBoard) {
			win = 0;
			total = 0;
			parent = NULL;
			lchild = NULL;
			sibling = NULL;
			currentBoard = inBoard;
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
			if(lchild == delChild) {
				lchild = lchild->sibling;
				return;
			}
			Node *actChild;
			actChild = lchild;
			while(actChild->sibling != NULL) {
				if(actChild->sibling == delChild) {
					actChild->sibling = actChild->sibling->sibling;
					return;
				}
				actChild = actChild->sibling;
			}
		}
		void freeSubtree(Node *p) {
			if(p == NULL) return;
			p = p->lchild;
			Node *tmp;
			while(p != NULL) {
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
/*		char oppositeColor() {
			if(color == 'B') return 'W';
			else return 'B';
		}*/
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
			while(p != NULL) {
				float childUrgency = p->getUCBValue(explore_coeff);
				if(childUrgency < worstUrgency) {
					worstUrgency = childUrgency;
					worstChild = p;
				}
				p = p->sibling;
			}
			if(worstChild == lchild && worstChild->sibling = NULL) return 0;
			removeChild(worstChild);
			freeSubtree(worstChild);
			delete worstChild;
		}
	};
	Node *root;
public:
	UCT(board inBoard, char startColor) {
		root = new Node(inBoard);
	}
	void createAllChildrenIfNone(Node *p) {
		if (p->lchild == NULL) {
			vector<int> legalPos = p->currentBoard.getemptycells();
			for (int i = 0; i < legalPos.length(); ++i) {
				Node *tmp = new Node(p->currentBoard);
				int coordX = legalPos[i] / SIZE;
				int coordY = legalPos[i] % SIZE;
				tmp->currentBoard.play(currentBoard.currentPlayer, coordX, coordY);
				p->addChild(tmp);
			}
		}
	}
	void update(Node *s, char winner) {
		while(s != NULL) {
			if(s->currentBoard->currentPlayer == winner) s->updateWin();
			else s->updateLoss();
			s = s->parent;
		}
	}
	void playOneSequenceInMoGo() {
		Node* p = root;
		createAllChildrenIfNone(p);
		do {
			p = p->findBestChild();
			if(p->isLeaf()) {
				if(p->total == 0) break;
				createAllChildrenIfNone(p);
				p = p->findBestChild();
				break;
			}
		} while (1);
		montecarlo m(p->currentBoard);
		update(p, m.getWinner());
	}

};

#endif