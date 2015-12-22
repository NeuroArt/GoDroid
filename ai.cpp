#include "ai.h"

ai::ai(){
	brd = new board();
	//get_komi();
}

int ai::on_board(int i, int j)
{
  return i >= 1 && i <= board_size && j >= 1 && j <= board_size;
}

int ai::pass_move(int i, int j)
{
  return i == -1 && j == -1;
}

int ai::legal_move(int i, int j, int color)
{
  int other = OTHER_COLOR(color);
  
  /* Pass is always legal. */
  if (pass_move(i, j))
    return 1;

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

int ai::valid_fixed_handicap(int handicap)
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

void ai::place_fixed_handicap(int handicap)
{
  int low = board_size >= 13 ? 4 : 3;
  int mid = board_size / 2 + 1;
  int high = board_size - low;
  
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

void ai::place_free_handicap(int handicap)
{
  int k;
  int i, j;
  
  for (k = 0; k < handicap; k++) {
    generate_move(&i, &j, black);
    play_move(i, j, black);
  }
}

void ai::generate_move(int *i, int *j, int color)
{
	int moves[SIZE * SIZE];
	int num_moves = 0;
	int move;
	int ai, aj;
	int k;

	memset(moves, 0, sizeof(moves));
	for (ai = 0; ai < board_size; ai++)
		for (aj = 0; aj < board_size; aj++) {
			/* Consider moving at (ai, aj) if it is legal and not suicide. */
			if (legal_move(ai, aj, color) && !brd->suicide(ai, aj, color)) {
				/* Further require the move not to be suicide for the opponent... */
				if (!brd->suicide(ai, aj, OTHER_COLOR(color)))
					moves[num_moves++] = POS(ai, aj);
				else {
					/* ...however, if the move captures at least one stone,
					* consider it anyway.
					*/
					for (k = 0; k < 4; k++) {
						int bi = ai + offset_x[k];
						int bj = aj + offset_y[k];
						if (on_board(bi, bj) && brd->get_cell(bi, bj) == OTHER_COLOR(color)) {
							moves[num_moves++] = POS(ai, aj);
							break;
						}
					}
				}
			}
		}

	/* Choose one of the considered moves randomly with uniform
	* distribution. (Strictly speaking the moves with smaller 1D
	* coordinates tend to have a very slightly higher probability to be
	* chosen, but for all practical purposes we get a uniform
	* distribution.)
	*/
	if (num_moves > 0) {
		move = moves[rand() % num_moves];
		*i = I(move);
		*j = J(move);
	}
	else {
		/* But pass if no move was considered. */
		*i = -1;
		*j = -1;
	}
}

void ai::play_move(int i, int j, int color)
{
	int pos = POS(i, j);
	int captured_stones = 0;
	int k;

	/* Reset the ko point. */
	ko_i = -1;
	ko_j = -1;

	/* Nothing more happens if the move was a pass. */
	if (pass_move(i, j))
	return;
	bool clr = (color == BLACK);
	brd->play(clr, i, j);

	/* If the move is a suicide we only need to remove the adjacent
	* friendly stones.
	if (suicide(i, j, color)) {
		for (k = 0; k < 4; k++) {
			int ai = i + deltai[k];
			int aj = j + deltaj[k];
			if (on_board(ai, aj)
			&& get_board(ai, aj) == color)
			remove_string(ai, aj);
		}
		return;
	}
	*/

	/* Not suicide. Remove captured opponent strings. 
	for (k = 0; k < 4; k++) {
		int ai = i + deltai[k];
		int aj = j + deltaj[k];
		if (on_board(ai, aj) && get_board(ai, aj) == OTHER_COLOR(color) && !has_additional_liberty(ai, aj, i, j))
			captured_stones += remove_string(ai, aj);
	}
	*/

	/* Put down the new stone. Initially build a single stone string by
	* setting next_stone[pos] pointing to itself.
	board[pos] = color;
	next_stone[pos] = pos;
	*/

	/* If we have friendly neighbor strings we need to link the strings
	* together.
	for (k = 0; k < 4; k++) {
		int ai = i + deltai[k];
		int aj = j + deltaj[k];
		int pos2 = POS(ai, aj);
	*/
		/* Make sure that the stones are not already linked together. This
		* may happen if the same string neighbors the new stone in more
		* than one direction.
		if (on_board(ai, aj) && board[pos2] == color && !same_string(pos, pos2)) {
		*/
			/* The strings are linked together simply by swapping the the
			* next_stone pointers.
			int tmp = next_stone[pos2];
			next_stone[pos2] = next_stone[pos];
			next_stone[pos] = tmp;
		}
	}
			*/

	/* If we have captured exactly one stone and the new string is a
	* single stone it may have been a ko capture.
	if (captured_stones == 1 && next_stone[pos] == pos) {
		int ai, aj;
	*/
		/* Check whether the new string has exactly one liberty. If so it
		* would be an illegal ko capture to play there immediately. We
		* know that there must be a liberty immediately adjacent to the
		* new stone since we captured one stone.
		for (k = 0; k < 4; k++) {
			ai = i + deltai[k];
			aj = j + deltaj[k];
			if (on_board(ai, aj) && get_board(ai, aj) == EMPTY)
			break;
		}
    
		if (!has_additional_liberty(i, j, ai, aj)) {
			ko_i = ai;
			ko_j = aj;
		}
	}
		*/
}