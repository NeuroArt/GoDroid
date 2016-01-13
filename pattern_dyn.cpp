#include "pattern_dyn.h"
#include "board.h"
#include <iostream>
#include <bitset>

using namespace std;

int pattern_3[MAXPATTERN][11];
int pattern_5[MAXPATTERN][27];
int pattern_7[MAXPATTERN][51];

bool validCoord(int centerx, int centery){//center:1-13
	return ((centerx>=0) && (centery>=0) && (centerx<=14) && (centery<=14));
}

void exchangeDyn(int orig[], int size){
	for(int i = 0; i < size*size; ++i){
		int color = orig[i];
		if(color == white) orig[i] = black;
		else if(color == black) orig[i] = white;
		else if(color == notWhite) orig[i] = notBlack;
		else if(color == notBlack) orig[i] = notWhite;
	}
}

void clockwiseDyn(int orig[], int size){
	int res[MAXDIAMETER*MAXDIAMETER];
	for(int i = 0; i < size; ++i){
		for(int j = 0; j < size; ++j){
			res[i*size+(size-1-j)] = orig[j*size+i];
		}
	}
	for(int i = 0; i < size*size; ++i){
		orig[i] = res[i];
	}
}

void symmetryXDyn(int orig[], int size){
	int res[MAXDIAMETER*MAXDIAMETER];
	for(int i = 0; i < size; ++i){
		for(int j = 0; j < size; ++j){
			res[i*size+j] = orig[(size-1-i)*size+j];
		}
	}
	for(int i = 0; i < size*size; ++i){
		orig[i] = res[i];
	}
}

void symmetryYDyn(int orig[], int size){
	int res[MAXDIAMETER*MAXDIAMETER];
	for(int i = 0; i < size; ++i){
		for(int j = 0; j < size; ++j){
			res[i*size+j] = orig[i*size+(size-1-j)];
		}
	}
	for(int i = 0; i < size*size; ++i){
		orig[i] = res[i];
	}
}

void initPatternDyn(){
	int pattern0[9] = {2,1,2,0,6,0,6,6,6};
    int pattern1[9] = {2,1,6,2,6,0,6,0,6};
    int pattern2[9] = {2,1,0,0,6,0,6,0,6};
    int pattern3[9] = {2,1,1,0,black,0,6,0,6};
	insertPatternDyn(pattern0, 3, hane,1);
	insertPatternDyn(pattern1, 3, hane,2);
	insertPatternDyn(pattern2, 3, hane,3);
	insertPatternDyn(pattern3, 3, hane,4);
	int pattern4[9] = {arbitrary,black,arbitrary,white,arbitrary,white,notWhite,notWhite,notWhite};
	insertPatternDyn(pattern4, 3, cut2,1);
}

void insertPatternDyn(int pat[], int size, int type, int smallType){
	if(size == 3){
		for(int i = 0; i < size*size; ++i){
			pattern_3[totalPattern3][i] = pat[i];
		}pattern_3[totalPattern3][size*size] = type;
		pattern_3[totalPattern3][size*size+1] = smallType;
		totalPattern3++;
	}
	if(size == 5){
		for(int i = 0; i < size*size; ++i){
			pattern_5[totalPattern5][i] = pat[i];
		}pattern_5[totalPattern5][size*size] = type;
		pattern_5[totalPattern5][size*size+1] = smallType;
		totalPattern5++;
	}
	if(size == 7){
		for(int i = 0; i < size*size; ++i){
			pattern_7[totalPattern7][i] = pat[i];
		}pattern_7[totalPattern7][size*size] = type;
		pattern_7[totalPattern7][size*size+1] = smallType;
		totalPattern7++;
	}
}

bool equalStoneType(int x, int pat){// x:0-3; pat:0-6
	if(x == pat) return true;
	if(pat == arbitrary || x == arbitrary) return true;

	if(pat == notWhite) return (x != white);
	if(pat == notBlack) return (x != black);

	if(x == notWhite) return (pat != white);
	if(x == notBlack) return (pat != black);

	return false;
}

bool samePattern(int x[], int existed[], int size, typeTrans &tt){
	//attention! center point of x[] must be current_color!
	tt.pat = 0;
	for(tt.ex = 0; tt.ex < 2; ++tt.ex){
		for(tt.clock = 0; tt.clock < 4; ++tt.clock){
			for(tt.transx = 0; tt.transx < 2; ++tt.transx){
				for(tt.transy = 0; tt.transy < 2; ++tt.transy){
					bool flag = true;
					for(int i = 0; i < size*size; ++i){
						if(!equalStoneType(x[i], existed[i])) flag = false;
					}
					if(flag){
						tt.pat = existed[size*size];
						tt.smallType = existed[size*size+1];
						return true;
					}
					symmetryYDyn(x, size);
				}
				symmetryXDyn(x, size);
			}
			clockwiseDyn(x, size);
		}
		exchangeDyn(x, size);
	}
	return false;
}

int matchPatternDyn(board *brd, int color, int x, int y, int size, int radius, typeTrans tt[]){
	int length = 0;
	int sizer = (size-1)/2;
	for(int i = -radius; i <= radius; ++i){
		for(int j = -radius; j <= radius; ++j){
			if(brd->get_cell(x+i, y+j) != empty) continue;
			int patx[MAXDIAMETER*MAXDIAMETER];
			int tmp = 0;
			int centerx = x+i; int centery = y+j;
			for(int k = -sizer; k <= sizer; ++k){
				for(int l = -sizer; l <= sizer; ++l){
					if(!validCoord(centerx+k, centery+l)) patx[tmp] = border;
					else patx[tmp] = brd->get_cell(centerx+k, centery+l);
					++tmp;
				}
			}
			patx[(size*size-1)/2] = color;
			//printPatternDyn(patx, 3);
			if(size == 3){
				for(int k = 0; k < totalPattern3; ++k){
					if(samePattern(patx,pattern_3[k],size,tt[length])){
						tt[length].x = centerx;
						tt[length].y = centery;
						length++; break;
					}
				}
			}
			if(size == 5){
				for(int k = 0; k < totalPattern5; ++k){
					if(samePattern(patx,pattern_5[k],size,tt[length])){
						tt[length].x = centerx;
						tt[length].y = centery;
						length++; break;
					}
				}
			}
			if(size == 7){
				for(int k = 0; k < totalPattern7; ++k){
					if(samePattern(patx,pattern_7[k],size,tt[length])){
						tt[length].x = centerx;
						tt[length].y = centery;
						length++; break;
					}
				}
			}
		}
	}
	return length;
}

void printPatternDyn(int pat[], int size){
	printf("patternDyn:\n");
	for(int i = 0; i < size; ++i){
		for(int j = 0; j < size; ++j){
			int p = pat[i*size+j];
			if(p == 0) printf("+");
			if(p == white) printf("0");
			if(p == black) printf("X");
			if(p == border) printf("=");
			if(p == notWhite) printf("B");
			if(p == notBlack) printf("W");
			if(p == arbitrary) printf("A");
		}printf("\n");
	}printf("\n");
}

void printPatternTypeDyn(int pat[], int size){
	printf("pattern type: %d\n",pat[size*size]);
}

void printTrans(typeTrans tt){
	printf("\npat: %d;\ntransx: %d; transy: %d;\nex: %d; clock:%d;\n",tt.pat,tt.transx,tt.transy,tt.ex,tt.clock);
	printf("x: %d; y: %d;\n",tt.x, tt.y);
}

/*
int main(){
    int p[50],q[50];
	typeTrans tt;
	int size = 3;
	for(int i = 0; i < 50; ++i){
		p[i] = 0; q[i] = 0;
	}
	p[0] = arbitrary; p[3] = black; p[5] = white; p[8] = black;
	p[9] = hane;
	//p[13] = arbitrary; p[20] = notBlack;
	//p[28] = black; p[42] = notBlack;
	q[0] = white; q[3] = black; q[5] = white; q[8] = black;
	printPatternDyn(q,size);
	printPatternDyn(p,size);
	exchangeDyn(p,size);
	symmetryXDyn(p,size);
	symmetryYDyn(p,size);
	clockwiseDyn(p,size);
	printPatternDyn(p,size);
	if(samePattern(q,p,size,tt)) printTrans(tt);

	system("pause");
}
*/