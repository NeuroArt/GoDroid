#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
#include <set>
#define SIZE 13

#define DEAD 0
#define ALIVE 1
#define SEKI 2
#define WHITE_TERRITORY 3
#define BLACK_TERRITORY 4
#define UNKNOWN 5

extern float komi;

#define POS(i, j) ((i-1) * board_size + (j-1))
#define I(pos) (((pos) / board_size) + 1)
#define J(pos) (((pos) % board_size) + 1)

static int offset_x[4] = {-1, 1, 0, 0};
static int offset_y[4] = {0, 0, -1, 1};

enum cell{
	empty = 0, //0
	white,   //1
	black,     //2
	border  //3
};

struct kaku{
	bool visited;
	cell c;
	int liberty;
	kaku(int state=0, int l=4){
		visited = false;
		c = (cell)state;
		liberty = l;
	}
};

class board{
private:
	static const int board_size = SIZE;

	kaku goban[SIZE+2][SIZE+2];
	bool currentplayer; //true=black; false=white
	std::set<int> *emptycells;
	int ko_i;
	int ko_j;

	int final_status[SIZE*SIZE];

	void place(bool player, kaku*k);
	bool deathtest(kaku* k);
	void kill(kaku* k);
	void killall(kaku* k, cell state,int &total);

public:
	board(std::set<int> *es=NULL);
	board(const board& b);
	~board();
	int get_boardsize(){return board_size;}
	cell get_cell(int i, int j);
	int board_empty();
	void clear_board();
	void compute_final_status();
	int suicide(int i, int j, int color);//string needed
	void set_final_status_string(int pos, int status);//需要棋串
	int has_additional_liberty(int i, int j, int libi, int libj);
        //需要棋串，(i,j)所在的棋串除(libi,libj)外是否还有其他的气
    int black_raw();//贴目前黑的总子数
	void showboard();
	bool getcurrentplayer(){return currentplayer;}
	std::set<int>* getemptycells();
	bool play(bool& player,int coordx, int coordy);
};

#endif
