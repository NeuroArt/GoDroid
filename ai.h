#include "board.h"

#define PROTOCOL_VERSION "2"
#define NAME "Neuroart"
#define VERSION "0.01"

#define MIN_BOARD 2
#define MAX_BOARD 23

#define EMPTY 0
#define WHITE 1
#define BLACK 2

#define OTHER_COLOR(color) (WHITE + BLACK - (color))

class ai{
private:
	board *brd;
	static const int board_size = SIZE;
	float komi;
	int ko_i, ko_j;//illeagal ko point
	int on_board(int i, int j);
	int pass_move(int i, int j);
public:
	ai();
	~ai();
	void clear_board(){brd->clear_board();}
	int board_empty(){return brd->board_empty();}
	int get_board(int i, int j){return brd->get_cell(i,j);}
	int get_string(int i, int j, int *stonei, int *stonej);//未实现，返回类型未确定
	int get_boardsize(){return board_size;}

	int legal_move(int i, int j, int color);//board?
	void play_move(int i, int j, int color);
	void generate_move(int *i, int *j, int color); //未实现，返回类型未确定
	int valid_fixed_handicap(int handicap);
	void place_fixed_handicap(int handicap);
	void place_free_handicap(int handicap);

	void compute_final_status(void);//未实现，返回类型未确定
	int get_final_status(int i, int j);//未实现，返回类型未确定
	void set_final_status(int i, int j, int status);//未实现，返回类型未确定

	char* get_protocol_version(){return PROTOCOL_VERSION;}
	char* get_name(){return NAME;}
	char* get_version(){return VERSION;}
	void get_komi(); //未实现，返回类型未确定
	void get_known_command(); //未实现，返回类型未确定
	void final_score(); //未实现，返回类型未确定
	void final_status_list(); //未实现，返回类型未确定
};