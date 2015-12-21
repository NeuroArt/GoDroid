#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
//#include <set>
#define SIZE 13
#define PROTOCOL_VERSION "2"
#define NAME "Neuroart"
#define VERSION "0.01"

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
	black,     //1
	white,   //2
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
/*	std::set<int> *emptycells;*/
	int ko_i;
	int ko_j;

	int final_status[SIZE*SIZE];

	void place(bool player, kaku*k);
	bool deathtest(kaku* k);
	void kill(kaku* k);
	void killall(kaku* k, cell state,int &total);

public:
	board();
	board(const board& b);
	~board();
	char* get_protocol_version(){return PROTOCOL_VERSION;}
	char* get_name(){return NAME;}
	char* get_version(){return VERSION;}
	void get_known_command(); //δʵ�֣���������δȷ��
	void quit(); //δʵ�֣���������δȷ��
	int get_boardsize(){return board_size;}
	cell get_cell(int i, int j);
	void clear_board();
	void get_komi(); //δʵ�֣���������δȷ��
	void get_fixed_handicap(); //δʵ�֣���������δȷ��
	void place_free_handicap(); //δʵ�֣���������δȷ��
	void set_free_handicap(); //δʵ�֣���������δȷ��
	void genmove(); //δʵ�֣���������δȷ��
	void final_score(); //δʵ�֣���������δȷ��
	void final_status_list(); //δʵ�֣���������δȷ��
	void compute_final_status();
<<<<<<< HEAD
	void set_final_status_string(int pos, int status){}//��Ҫ�崮
	int has_additional_liberty(int i, int j, int libi, int libj){}
=======
	void set_final_status_string(int pos, int status);//��Ҫ�崮
	int has_additional_liberty(int i, int j, int libi, int libj);
>>>>>>> origin/QianyangPeng's_branch
        //��Ҫ�崮��(i,j)���ڵ��崮��(libi,libj)���Ƿ�����������
    int black_raw();//��Ŀǰ�ڵ�������
	void showboard();
	bool getcurrentplayer(){return currentplayer;}
	//std::set<int>* getemptycells(); //function cancelled
	bool play(bool& player,int coordx, int coordy);
};

#endif
