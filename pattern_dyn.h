#ifndef _PATTERN_DYN_
#define _PATTERN_DYN_
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <set>
#include "board.h"

#define MAXDIAMETER 7
#define MAXPATTERN 30

using namespace std;

static int totalPattern3 = 0;
static int totalPattern5 = 0;
static int totalPattern7 = 0;

enum stoneType{ //type of stones in pattern
	notWhite=4,
	notBlack,
	arbitrary
};

enum patternType{ //type of patterns
	null = 0, //not a pattern
	hane,
	cut1,
	cut2
};

struct typeTrans{
	int x, y;
	int pat;
	int transx, transy;
	int ex;
	int clock;
};

bool onBoard(int centerx, int centery){//center:1-13
	return ((centerx>=0) && (centery>=0) && (centerx<=14) && (centery<=14));
}
//4 types of transformations, size:3-5
void exchangeDyn(int orig[], int size);
void clockwiseDyn(int orig[], int size);
void symmetryXDyn(int orig[], int size);
void symmetryYDyn(int orig[], int size);

void initPatternDyn(); // 待实现，需要手动建立

void insertPatternDyn(int pat[], int size, int type); 
// 手动建立pattern的时候使用这个函数，pat为size*size

bool samePattern(int x[], int existed[], int size, typeTrans *tt); 
//if is the same pattern, it will modify tt

bool equalStoneType(int x, int pat);
//需要检查逻辑是否正确，现在：任意、非黑、非白均可以匹配border

int matchPatternDyn(board *brd, int color, int x, int y, int size, int radius, typeTrans tt[]);
//未测试

void printPatternDyn(int pat[], int size);
void printPatternTypeDyn(int pat[], int size){printf("pattern type: %d\n",pat[size*size]);}
void printTrans(typeTrans tt){
	printf("pat: %d;\ntransx: %d; transy: %d;\nex: %d; clock:%d;\n",tt.pat,tt.transx,tt.transy,tt.ex,tt.clock);
}

#endif