#ifndef _UCT_H
#define _UCT_H
#include <math.h>
#include <string.h>
#include <stack>
#include <assert.h>
#include <queue>
#include <fstream>
#include "board.h"
#include "montecarlo.h"
#define largeFloat 1000000000000.0

using namespace std;

class UCT {
private:
	struct Node {
		int win, total;
		double value() {
			return double(win) / total;
		}
		Node *parent, *lchild, *sibling;
		//board currentBoard;
		int move;
		bool player;
		char color;
		double ucbValue;

		Node(bool _play) {
			win = 0;
			total = 0;
			parent = NULL;
			lchild = NULL;
			sibling = NULL;
			player = _play;
			move = -1;
			ucbValue = -1;
		}
		double V(double explore_coeff) {
			//printf("Tn: %d Total: %d ", Tn, total);
			return value() - value() * value() + sqrt(2 * explore_coeff / total);
		}
		double getUCBValue(double explore_coeff) {
			if (total == 0) return largeFloat;
			//printf("V: %f\n", V(explore_coeff));
			//return value() + sqrt(explore_coeff / total * min(0.25, V(explore_coeff)));
			return value() + sqrt(explore_coeff / total * 2);
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
			double explore_coeff = log(double(total));
			
			double bestUrgency = -largeFloat;
			
			Node *p = lchild;
			while (p != NULL) {
				double childUrgency = p->getUCBValue(explore_coeff);
				p->ucbValue = childUrgency;
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
			double worstUrgency = largeFloat;
			double explore_coeff = 2 * log(double(total));
			Node *p = lchild;
			while (p != NULL) {
				double childUrgency = p->getUCBValue(explore_coeff);
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
public:
	UCT(board &inBoard, bool player) {
		root = new Node(!player);
		rootBoard = inBoard;
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
		bool player = root->player;
		if (!back.empty())
			back.pop();
		while (!back.empty()) {
			int num = back.top();
			back.pop();
			int coordX = num / SIZE + 1;
			int coordY = num % SIZE + 1;
			player = !player;
			currentBoard.play(player, coordX, coordY);
		}
		return currentBoard;
	}
	void createAllChildrenIfNone(Node *p,bool player) {
		//assert(p != NULL);
		if (p->lchild == NULL) {
			int fail = 0;
			int a=0;
			board currentBoard(getBoard(p));
			std::vector<int>validset = getBoard(p).get_valid_set(player);
			std::vector<int>::iterator iter;
			for (iter=validset.begin();iter!=validset.end();iter++){
				bool tmpPlayer = !(p->player);
				Node *tmp= new Node(tmpPlayer);
				int pos = *iter;
				tmp->move = pos-1;
				p->addChild(tmp);
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
	void playOneSequenceInMoGo(bool player) {
		Node* p = root;
		createAllChildrenIfNone(p,player);
		do {
			p = p->findBestChild();
			//if (p == NULL) return;
			if (p != NULL && p->isLeaf()) {
				if (p->total == 0) break;
				createAllChildrenIfNone(p,player);
				p = p->findBestChild();
				break;
			}
		} while (1);
		if (p != NULL) {
			board currentBoard(getBoard(p));
			montecarlo m(currentBoard,player);
			update(p, m.getWinner());
		}
		else {
			board currentBoard(getBoard(p->parent));
			montecarlo m(currentBoard,player);
			update(p->parent, m.getWinner());
		}	
	}
	int getNextMove() {
		Node *tmp = root->findBestChild();
		ofstream out("E:\log2.txt", ios::app);
		int i = tmp->move / 13 + 1;
		int j = tmp->move % 13 + 1;
		out << "player" << tmp->player << ' ' << i << ' ' << j << " UCB: " << tmp->ucbValue << endl;
		return tmp->move;
	}
	void showTree(bool flag = 1) {
		rootBoard.showboardtofile();
	}
	void showTotal() {
		printf("%d/%d\n", root->win, root->total);
	}

};

#endif