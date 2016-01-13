#include "pattern_dyn.h"
#include <iostream>
#include <bitset>

int pattern[MAXPATTERN][MAXDIAMETER*MAXDIAMETER+1];

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
			res[i*size+j] = orig[j*size+(size-1-i)];
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
				for(int l = sizer; l <= sizer; ++l){
					patx[tmp] = brd->get_cell(centerx+k, centery+l);
					++tmp;
				}
			}
			for(int k = 0; k < totalPattern; ++k){
				if(samePattern(patx,pattern[k],size,tt[length])){
					tt[length].x = centerx;
					tt[length].y = centery;
					length++; break;
				}
			}
		}
	}
	return length;
}