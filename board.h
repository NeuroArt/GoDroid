#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
#include <set>
#include <vector>
#define SIZE 13

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
	std::set<int> *emptycells;
	int ko_i;
	int ko_j;

	void place(bool player, kaku*k);
	bool deathtest(kaku* k);
	void kill(kaku* k);
	void killall(kaku* k, cell state,int &total);

public:
	board(std::set<int> *es=NULL);
	board(const board& b);
	~board();
	void get_protocol_version(); //δʵ�֣���������δȷ��
	void get_name(); //δʵ�֣���������δȷ��
	void get_version(); //δʵ�֣���������δȷ��
	void get_known_command(); //δʵ�֣���������δȷ��
	void quit(); //δʵ�֣���������δȷ��
	int get_boardsize(); //δʵ�֣���������δȷ��
	void clear_board(); //δʵ�֣���������δȷ��
	void get_komi(); //δʵ�֣���������δȷ��
	void get_fixed_handicap(); //δʵ�֣���������δȷ��
	void place_free_handicap(); //δʵ�֣���������δȷ��
	void set_free_handicap(); //δʵ�֣���������δȷ��
	void genmove(); //δʵ�֣���������δȷ��
	void final_score(); //δʵ�֣���������δȷ��
	void final_status_list(); //δʵ�֣���������δȷ��
	void showboard();
	bool getcurrentplayer(){return currentplayer;}
	std::vector<int> getemptycells();
	bool play(bool& player,int coordx, int coordy);
};

#endif