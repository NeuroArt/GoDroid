#ifndef _AI_H
#define _AI_H

#include "board.h"

//i,j belongs to [0,12] and needs transformations when communicating with board.

#define PROTOCOL_VERSION "2"
#define NAME "Neuroart"
#define VERSION "0.01"

#define MIN_BOARD 2
#define MAX_BOARD 23

#define EMPTY 0
#define WHITE 1
#define BLACK 2

#define OTHER_COLOR(color) (WHITE + BLACK - (color))

int pass_move(int i, int j);

void init_ai();
void clear_board();
int board_empty();
int get_board(int i, int j);
int get_boardsize();
int get_string(int i, int j, int *stonei, int *stonej);//未实现，返回类型未确定

int legal_move(int i, int j, int color);//board?
void play_move(int i, int j, int color);
void generate_move(int *i, int *j, int color); //未实现，返回类型未确定
int valid_fixed_handicap(int handicap);
void place_fixed_handicap(int handicap);
void place_free_handicap(int handicap);

void compute_final_status(void);
void set_final_status(int i, int j, int status);
int get_final_status(int i, int j);
char* get_protocol_version();
char* get_name();
char* get_version();

short find_liberty(int i, int j);

#endif