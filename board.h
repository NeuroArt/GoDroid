#include <iostream>
#include <vector>
#include <set>
#include <stdio.h>

#ifndef _BOARD_H_
#define _BOARD_H_

#define BOARDSIZE 13

#define DEAD 0
#define ALIVE 1
#define SEKI 2
#define WHITE_TERRITORY 3
#define BLACK_TERRITORY 4
#define UNKNOWN 5

#define POS(i, j) ((i-1) * board_size + (j-1))
#define I(pos) (((pos) / board_size) + 1)
#define J(pos) (((pos) % board_size) + 1)

#define N(pos) (pos-(BOARDSIZE+2))
#define S(pos) (pos+(BOARDSIZE+2))
#define W(pos) (pos-1)
#define E(pos) (pos+1)

#define SURROUNDED(pos,cel) ((E(pos)->c==cel||E(pos)->c==border)&&(W(pos)->c==cel||W(pos)->c==border)&&(S(pos)->c==cel||S(pos)->c==border)&&(N(pos)->c==cel||N(pos)->c==border))

static int offset_x[4] = {-1, 1, 0, 0};
static int offset_y[4] = {0, 0, -1, 1};

enum cell{
	empty = 0, //0
	white,     //1
	black,   //2
	border  //3
};

struct kaku{
	cell c;
	kaku* parent;
	short position;
	short fakeliberty;
	short final_status;
	bool ataritested;
	kaku(int state=0, int l=0){
		parent = this;
		c = (cell)state;
		fakeliberty = l;
		final_status = UNKNOWN;
		ataritested = false;
	}
	static void Union(kaku* a,kaku* b);
	kaku* findparent();
	short findliberty();
	short get_final_status();
};

class board{
private:
	static const int board_size = BOARDSIZE;
/*	std::set<int> *emptycells;*/
	int ko_i;
	int ko_j;
	int atariposition;

	int final_status[BOARDSIZE*BOARDSIZE];

	void place(bool player, kaku*k);
	bool deathtest(kaku* k);
	bool bfs(kaku* k);
	void kill(kaku* k);
	void killall(kaku* k, cell state,int &total);
	void ataritest(kaku* k, cell state,int &total);
	void refreshtest(kaku*k);
public:
	kaku goban[BOARDSIZE+2][BOARDSIZE+2];
	board();
	board(const board& b);
	~board();
	board& operator=(const board& x);
	kaku* get_kaku(int position){return &goban[(position-1)/BOARDSIZE+1][(position-1)%BOARDSIZE+1];}
	int get_boardsize(){return board_size;}
	cell get_cell(int i, int j);
	void clear_board();
	int board_empty();
    int black_raw();//贴目前黑的总子数
	void showboard();
	void showboard(char *str);
	void showboardtofile();
	double judge();//目前用来凑数的判断函数，返回值为黑棋比白棋多多少子。
	//std::set<int>* getemptycells(); //function cancelled
	bool play(bool player,int coordx, int coordy, bool simulation=true);
	int get_final_status(int i, int j);
	void set_final_status(int i, int j, int status);
	short find_liberty(int i, int j){return goban[i][j].findliberty();}
	bool valid_test(kaku* target, bool player);
	bool kotest(kaku* target);

	std::set<short> validsetforblack;
	std::set<short> validsetforwhite;
	std::set<short> ataripositionforblack;
	std::set<short> ataripositionforwhite;
};

#endif
