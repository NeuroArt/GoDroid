#ifndef _PATTERN_H_
#define _PATTERN_H_
#include <math.h>
#include <stdio.h>
#include <set>

using namespace std;

set<unsigned short> patterns;
set<unsigned short> patternsBlack;
set<unsigned short> patternsWhite;
set<unsigned short> cutDeny;


void initPatterns();

unsigned short bit(unsigned short orig, int num);

unsigned short clockwise(unsigned short orig);

unsigned short exchange(unsigned short orig);

unsigned short symmetryX(unsigned short orig);

unsigned short symmetryY(unsigned short orig);

#endif
