#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
#include <vector>
#include <stdio.h>
//#include <set>

#define SIZE 13

#define DEAD 0
#define ALIVE 1
#define SEKI 2
#define WHITE_TERRITORY 3
#define BLACK_TERRITORY 4
#define UNKNOWN 5

#define POS(i, j) ((i-1) * board_size + (j-1))
#define I(pos) (((pos) / board_size) + 1)
#define J(pos) (((pos) % board_size) + 1)

#define N(pos) pos-(SIZE+2)
#define S(pos) pos+(SIZE+2)
#define W(pos) pos-1
#define E(pos) pos+1

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
	short fakeliberty;
	short final_status;
	kaku(int state=0, int l=0){
		parent = this;
		c = (cell)state;
		fakeliberty = l;
		final_status = UNKNOWN;
	}
	static void Union(kaku* a,kaku* b);
	kaku* findparent();
	short findliberty();
	short get_final_status();
};

class board{
private:
	static const int board_size = SIZE;
	kaku goban[SIZE+2][SIZE+2];
/*	std::set<int> *emptycells;*/
	int ko_i;
	int ko_j;

	int final_status[SIZE*SIZE];

	void place(bool player, kaku*k);
	bool deathtest(kaku* k);
	bool bfs(kaku* k);
	void kill(kaku* k);
	void killall(kaku* k, cell state,int &total);

public:
	board();
	board(const board& b);
	~board();
	board& operator=(const board& x);
	int get_boardsize(){return board_size;}
	cell get_cell(int i, int j);
	void clear_board();
	int board_empty();
	//void final_score(); 未实现，返回类型未确定
	//void compute_final_status();
	//int has_additional_liberty(int i, int j, int libi, int libj){return 0;}
        //需要棋串，(i,j)所在的棋串除(libi,libj)外是否还有其他的气
    int black_raw();//贴目前黑的总子数
	void showboard();
	void showboard(char *str);
	void showboardtofile();
	float judge();//目前用来凑数的判断函数，返回值为黑棋比白棋多多少子。
	//std::set<int>* getemptycells(); //function cancelled
	bool play(bool player,int coordx, int coordy, bool simulation=true);
	int get_final_status(int i, int j);
	void set_final_status(int i, int j, int status);
	short find_liberty(int i, int j){return goban[i][j].findliberty();}
	std::vector<int> get_valid_set(bool player);//返回值范围为1-169，不是0-168，望注意。
};

#endif
