#ifndef _PATTERN_H_
#define _PATTERN_H_
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <set>
#include "board.h"

using namespace std;

set<unsigned short> patterns;//for all
set<unsigned short> patternsBlack;//only for black
set<unsigned short> patternsWhite;//only for white
set<unsigned short> cutDeny;


void initPatterns();

unsigned short bit(unsigned short orig, int num);

unsigned short clockwise(unsigned short orig);

unsigned short exchange(unsigned short orig);

unsigned short symmetryX(unsigned short orig);

unsigned short symmetryY(unsigned short orig);

#endif
