#ifndef _PATTERN_H_
#define _PATTERN_H_
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <set>
#include "board.h"

using namespace std;

void initPatterns();

unsigned short toShort(int pattern[8]);

unsigned short word(unsigned short orig, int num);

unsigned short clockwise(unsigned short orig);

unsigned short exchange(unsigned short orig);

unsigned short symmetryX(unsigned short orig);

unsigned short symmetryY(unsigned short orig);

void insert(unsigned short pat);

void insertWhite(unsigned short pat);

void insertBlack(unsigned short pat);

void erase(unsigned short pat);

bool matchPattern(unsigned short pat, int color);

int findPattern(board *brd, int color, int lastx, int lasty);

#endif
