#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
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
	bool currentplayer; //true=black; false=white
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
	int get_boardsize(){return board_size;}
	cell get_cell(int i, int j);
	void clear_board();
	int board_empty();
	void genmove(); //δʵ�֣���������δȷ��
	void final_score(); //δʵ�֣���������δȷ��
	void compute_final_status();
	int has_additional_liberty(int i, int j, int libi, int libj){}
        //��Ҫ�崮��(i,j)���ڵ��崮��(libi,libj)���Ƿ�����������
    int black_raw();//��Ŀǰ�ڵ�������
	void showboard();
	int judge();//Ŀǰ�����������жϺ���������ֵΪ����Ȱ��������ӡ�
	bool getcurrentplayer(){return currentplayer;}
	//std::set<int>* getemptycells(); //function cancelled
	bool play(bool& player,int coordx, int coordy);
	int get_final_status(int i, int j);
	short find_liberty(int i, int j){return goban[i][j].findliberty();}

	board& operator=(const board& x);
};

#endif
