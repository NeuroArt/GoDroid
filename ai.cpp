
#include <ctime>
#include "ai.h"
#include "UCT.h"
#include "Parallelization.h"
#include "pattern.h"

board *brd;
int board_size = BOARDSIZE;
double komi = 3.14;
int ko_i, ko_j;//illeagal ko point

static int on_board(int i, int j){return i >= 0 && i < board_size && j >= 0 && j < board_size;};

void init_ai(){
	brd = new board();
	initPatterns();
	//get_komi();
}

void clear_board(){brd->clear_board();}
int board_empty(){return brd->board_empty();}
int get_board(int i, int j){return brd->get_cell(i+1,j+1);}
int get_boardsize(){return board_size;}
board* get_brd(){return brd;}

int pass_move(int i, int j)
{
  return i == -1 && j == -1;
}

int legal_move(int i, int j, int color)
{
  int other = OTHER_COLOR(color);
  
  /* Pass is always legal. */
  if (pass_move(i, j))
    return 1;
  ++i; ++j;
  /* Already occupied. */
  if (brd->get_cell(i, j) != empty)
    return 0;

  /* Illegal ko recapture. It is not illegal to fill the ko so we must
   * check the color of at least one neighbor.
   */
  if (i == ko_i && j == ko_j
      && ((on_board(i - 1, j) && brd->get_cell(i - 1, j) == other)
	  || (on_board(i + 1, j) && brd->get_cell(i + 1, j) == other)))
    return 0;

  return 1;
}

int valid_fixed_handicap(int handicap)
{
  if (handicap < 2 || handicap > 9)
    return 0;
  if (board_size % 2 == 0 && handicap > 4)
    return 0;
  if (board_size == 7 && handicap > 4)
    return 0;
  if (board_size < 7 && handicap > 0)
    return 0;
  
  return 1;
}

void place_fixed_handicap(int handicap)
{
  int low = board_size >= BOARDSIZE ? 3 : 2;
  int mid = board_size / 2;
  int high = board_size - 1 - low;

  ++low;++mid;++high;
  
  if (handicap >= 2) {
    play_move(high, low, black);   /* bottom left corner */
    play_move(low, high, black);   /* top right corner */
  }
  
  if (handicap >= 3)
    play_move(low, low, black);    /* top left corner */
  
  if (handicap >= 4)
    play_move(high, high, black);  /* bottom right corner */
  
  if (handicap >= 5 && handicap % 2 == 1)
    play_move(mid, mid, black);    /* tengen */
  
  if (handicap >= 6) {
    play_move(mid, low, black);    /* left edge */
    play_move(mid, high, black);   /* right edge */
  }
  
  if (handicap >= 8) {
    play_move(low, mid, black);    /* top edge */
    play_move(high, mid, black);   /* bottom edge */
  }
}

void place_free_handicap(int handicap)
{
  int k;
  int i, j;
  
  for (k = 0; k < handicap; k++) {
    generate_move(&i, &j, black);
    play_move(i, j, black);
  }
}

void compute_final_status(void){
	int i, j;
	int k;
	for (i = 0; i < board_size; i++)
		for (j = 0; j < board_size; j++)
			set_final_status(i,j,UNKNOWN);
  
	for (i = 0; i < board_size; i++)
		for (j = 0; j < board_size; j++)
			if (get_board(i, j) == EMPTY)
				for (k = 0; k < 4; k++) {
					int ai = i + offset_x[k];
					int aj = j + offset_y[k];
					if (!on_board(ai, aj))
						continue;
					/* When the game is finished, we know for sure that (ai, aj)
					* contains a stone. The move generation algorithm would
					* never leave two adjacent empty vertices. Check the number
					* of liberties to decide its status, unless it's known
					* already.
					*
					* If we should be called in a non-final position, just make
					* sure we don't call set_final_status_string() on an empty
					* vertex.
					*/
					if (get_final_status(ai, aj) == UNKNOWN) {
						if (get_board(ai, aj) != EMPTY) {
							if (find_liberty(ai, aj) >= 2)
								set_final_status(ai, aj, ALIVE);
							else
								set_final_status(ai, aj, DEAD);
						}
					}
					/* Set the final status of the (i, j) vertex to either black
					* or white territory.
					*/
					if (get_final_status(i, j) == UNKNOWN) {
						if ((get_final_status(ai,aj) == ALIVE) ^ (get_board(ai, aj) == WHITE))
							set_final_status(i, j, BLACK_TERRITORY);
						else
							set_final_status(i, j, WHITE_TERRITORY);
					}
				}
}

void set_final_status(int i, int j, int status){
	brd->set_final_status(i+1, j+1, status);
}

//void generate_move(int *i, int *j, int color) { //老的generate_move
//	bool player = color==black?1:0;
//	//brd->showboard();
//	UCT tree(*brd, player);
//	int starttime = clock();
//	int finishtime = clock();
//	int cnt = 0;
//	while(finishtime - starttime < 2500) {
//		tree.playOneSequenceInMoGo(player);
//		finishtime = clock();
//		cnt++;
//	}
//	printf("cnt: %d\n", cnt);
//	int mov = tree.getNextMove();
//	tree.showTree(0);
//	*i = mov/BOARDSIZE;
//	*j = mov%BOARDSIZE;
//}

void generate_move(int *i, int *j, int color) { //新的generate_move 使用了并行
	bool player = color == BLACK ? 1 : 0;
	Parallelization para(*brd, player, 4);//并行类，还未测试
	int mov = para.getMove();
	*i = mov / BOARDSIZE;
	*j = mov % BOARDSIZE;
}

void play_move(int i, int j, int color){
	int mov = i*BOARDSIZE + j;
	int posX = mov / BOARDSIZE + 1;
	int posY = mov % BOARDSIZE + 1;
	bool flag = color==BLACK?true:false;
	brd->play(flag, posX, posY, false);
}

int get_final_status(int i, int j){return brd->get_final_status(i+1, j+1);}
char* get_protocol_version(){return PROTOCOL_VERSION;}
char* get_name(){return NAME;}
char* get_version(){return VERSION;}
short find_liberty(int i, int j){return brd->find_liberty(i+1, j+1);}