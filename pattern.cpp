#include "pattern.h"
#include <iostream>
#include <bitset>

set<unsigned short> patterns;//for all
set<unsigned short> patternsBlack;//only for black
set<unsigned short> patternsWhite;//only for white
set<unsigned short> cutDeny;

//需要查找的集合有三个，一个是通用pattern，一个是仅当轮到黑落子时才可行的pattern，一个是白的pattern

//数组表示的pattern转换成short
//数组表示的pattern依次为左上，上，右上，左，右，左下，下，右下
//分别对应4进制表示时pattern的第0到7位
//总共需要8位4进制，对应16位2进制，可以用unsigned short表示
unsigned short toShort(int pattern[8]){
    unsigned short res = 0;
    for(int i = 0; i < 8; ++i){
        res += (pattern[i]*pow(4,i));
    }
    return res;
}

//取short表示的pattern的第num位
unsigned short word(unsigned short orig, int num){
    unsigned short res = orig/pow(4,num);
    return res % 4;
}

//short表示的pattern转换成数组
void toArray(unsigned short pat, int pattern[8]){
	//printf("Array: ");
	for(int i = 0; i < 8; ++i){
		pattern[i] = pat%4;
		pat = pat / 4;
		//printf("%d",pattern[i]);
	}//printf("\n");
}

//对short表示的pattern做旋转操作，顺时针90度
unsigned short clockwise(unsigned short orig){
    int trans[8] = {2,4,7,1,6,0,3,5};
    unsigned short res = 0;
    for(int i = 0; i < 8; ++i)
        res += (word(orig,trans[i])*pow(4,i));
    return res;
}

//对short表示的pattern做反色操作，黑白颠倒
unsigned short exchange(unsigned short orig){
    int patt[8];
	toArray(orig,patt);
	for(int i = 0; i < 8; ++i){
		int color = patt[i];
		patt[i] = (color==black?white:patt[i]);
		patt[i] = (color==white?black:patt[i]);
	}
	return toShort(patt);
}

//对short表示的pattern做对称操作，沿竖直轴对称
unsigned short symmetryY(unsigned short orig){
    int trans[8] = {2,1,0,4,3,7,6,5};
    unsigned short res = 0;
    for(int i = 0; i < 8; ++i)
        res += (word(orig,trans[i])*pow(4,i));
    return res;
}

//对short表示的pattern做对称操作，沿水平轴对称
unsigned short symmetryX(unsigned short orig){
    int trans[8] = {5,6,7,3,4,0,1,2};
    unsigned short res = 0;
    for(int i = 0; i < 8; ++i)
        res += (word(orig,trans[i])*pow(4,i));
    return res;
}

//将pattern及其所有可能的等价变换插入集合中
void insert(unsigned short pat){
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 2; ++j){
            patterns.insert(pat);
            patterns.insert(symmetryX(pat));
            patterns.insert(symmetryY(pat));
            patterns.insert(symmetryX(symmetryY(pat)));
            pat = exchange(pat);
        }
		pat = clockwise(pat);
    }
}

//将白色子的pattern及所有可能的等价变换插入集合中（反色后需插入黑集合）
void insertWhite(unsigned short pat){
    for(int i = 0; i < 4; ++i){
        patternsWhite.insert(pat);
        patternsWhite.insert(symmetryX(pat));
        patternsWhite.insert(symmetryY(pat));
        patternsWhite.insert(symmetryX(symmetryY(pat)));
		pat = clockwise(pat);
    }
	pat = exchange(pat);
    for(int i = 0; i < 4; ++i){
        patternsBlack.insert(pat);
        patternsBlack.insert(symmetryX(pat));
        patternsBlack.insert(symmetryY(pat));
        patternsBlack.insert(symmetryX(symmetryY(pat)));
		pat = clockwise(pat);
    }
}

//将黑色子的pattern及所有可能的等价变换插入集合中（反色后需插入白集合）
void insertBlack(unsigned short pat){
    for(int i = 0; i < 4; ++i){
        patternsBlack.insert(pat);
        patternsBlack.insert(symmetryX(pat));
        patternsBlack.insert(symmetryY(pat));
        patternsBlack.insert(symmetryX(symmetryY(pat)));
		pat = clockwise(pat);
    }
	pat = exchange(pat);
    for(int i = 0; i < 4; ++i){
        patternsWhite.insert(pat);
        patternsWhite.insert(symmetryX(pat));
        patternsWhite.insert(symmetryY(pat));
        patternsWhite.insert(symmetryX(symmetryY(pat)));
		pat = clockwise(pat);
    }
}

//将一个pattern及其所有等价变换从集合中删除
void erase(unsigned short pat){
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 2; ++j){
            patterns.erase(pat);
            patterns.erase(symmetryX(pat));
            patterns.erase(symmetryY(pat));
            patterns.erase(symmetryX(symmetryY(pat)));
            pat = exchange(pat);
        }
    }
}

//初始化所有pattern集合
void initPatterns(){
    unsigned short pat;
    //hane
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            for(int k = 0; k < 3; ++k){
                int pattern[8] = {2,1,2,0,0,i,j,k};
                pat = toShort(pattern);
                insert(pat);
                int pattern1[8] = {2,1,i,2,0,j,0,k};
                pat = toShort(pattern1);
                insert(pat);
            }
            int pattern2[8] = {2,1,0,0,0,i,0,j};
            pat = toShort(pattern2);
            insert(pat);
            int pattern3[8] = {2,1,1,0,0,i,0,j};
            pat = toShort(pattern3);
            insertBlack(pat);
        }
    }

    //cut1
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            for(int k = 0; k < 3; ++k){
                for(int x = 0; x < 3; ++x){
                    for(int y = 0; y < 3; ++y){
                        int pattern[8] = {2,1,i,1,j,k,x,y};
                        pat = toShort(pattern);
                        insert(pat);
                    }
                }
            }
        }
    }
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            for(int k = 0; k < 3; ++k){
                int pattern[8] = {2,1,i,1,1,j,0,k};
                pat = toShort(pattern);
                erase(pat);
                int pattern1[8] = {2,1,i,1,0,j,1,k};
                pat = toShort(pattern1);
                erase(pat);
                int pattern2[8] = {2,1,i,1,1,j,1,k};
                pat = toShort(pattern2);
                erase(pat);
            }
        }
    }

    //cut2
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            for(int k = 0; k < 2; ++k){
                for(int x = 0; x < 2; ++x){
                    for(int y = 0; y < 2; ++y){
                        int pattern[8] = {i,2,j,1,1,k*2,x*2,y*2};
                        pat = toShort(pattern);
                        insert(pat);
                    }
                }
            }
        }
    }

    //boarder
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            int pattern[8] = {2,0,i,1,j,3,3,3};
            pat = toShort(pattern);
            insertBlack(pat);
        }
    }
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            for(int k = 0; k < 2; ++k){
                int pattern[8] = {i,2,j,k,1,3,3,3};
                pat = toShort(pattern);
                insert(pat);
            }
        }
    }
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            for(int k = 0; k < 3; ++k){
                int pattern[8] = {i,2,1,j,k,3,3,3};
                pat = toShort(pattern);
                insertBlack(pat);
            }
        }
    }
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            for(int k = 0; k < 2; ++k){
                int pattern[8] = {i,2,1,j,k,3,3,3};
                pat = toShort(pattern);
                insertWhite(pat);
            }
        }
    }
    for(int i = 0; i < 3; ++i){
        int pattern[8] = {i,2,1,1,2,3,3,3};
        pat = toShort(pattern);
        insertWhite(pat);
    }
}

//判断short表示的pattern是否在集合中，有则返回1
bool matchPattern(unsigned short pat, int color){
	return ((patterns.find(pat)!=patterns.end()) || ((color == 1) && (patternsWhite.find(pat)!=patternsWhite.end())) || ((color == 2) && (patternsBlack.find(pat)!=patternsBlack.end())));
}
 
int findPattern(board *brd, int color, int lastx, int lasty){ 
	//lastxy:[1,13]; 在lastx,lasty周围8个点处匹配pattern，有则返回匹配到pattern点的*一维*坐标，否则返回0
	if(lastx*lasty == 0) return 0;
	int deltay[8] = {-1,0,1,-1,1,-1,0,1};
	int deltax[8] = {-1,-1,-1,0,0,1,1,1};
	int x[8], y[8];
	int cnt = 0;
	for(int i = 0; i < 8; ++i){
		int centerx = lastx + deltax[i];
		int centery = lasty + deltay[i];
		if(brd->get_cell(centerx,centery) != empty) continue;
		int pattern[8];
		if(centerx > 0 && centerx <= BOARDSIZE && centery > 0 && centery <= BOARDSIZE){ //centerxy:[1,13];
			//printf("patternj: \n");
			for(int j = 0; j < 8; ++j){
				pattern[j] = brd->get_cell(centerx+deltax[j],centery+deltay[j]);
				//printf("%d ", pattern[j]);
			}//printf("\n");
			short patt = toShort(pattern);
			//printShort(patt);
			if(matchPattern(patt, color)){
				//printf("i=%d\n", i);
				//printf("center: %d %d\n", centerx, centery);
				//printShort(patt);
				//printf("%d", patt);
				//printf("\n");
				x[cnt] = centerx;
				y[cnt] = centery;
				cnt++;
			}
		}
	}
	if(cnt){
		//printf("cnt = %d\n", cnt);
		int r = rand() % cnt;
		//转换成一维坐标
		return (x[r] - 1) * BOARDSIZE + y[r];
	}
	return 0;
}
//打印short表示的pattern
void printShort(unsigned short pat){
	int pattern[8];
	toArray(pat, pattern);
	printPattern(pattern);
}
//打印数组表示的pattern
void printPattern(int pattern[8]){
	printf("Pattern: \n");
	for(int i = 0; i < 3; ++i){
		if(pattern[i] == empty) printf("+");
		else if(pattern[i] == black) printf("X");
		else if(pattern[i] == white) printf("0");
		else if(pattern[i] == border) printf("=");
	}printf("\n");
	if(pattern[3] == empty) printf("+");
	else if(pattern[3] == black) printf("X");
	else if(pattern[3] == white) printf("0");
	else if(pattern[3] == border) printf("=");
	printf("-");
	if(pattern[4] == empty) printf("+");
	else if(pattern[4] == black) printf("X");
	else if(pattern[4] == white) printf("0");
	else if(pattern[4] == border) printf("=");
	printf("\n");
	for(int i = 5; i < 8; ++i){
		if(pattern[i] == empty) printf("+");
		else if(pattern[i] == black) printf("X");
		else if(pattern[i] == white) printf("0");
		else if(pattern[i] == border) printf("=");
	}printf("\n\n");
}

/*
int main(){
    int pattern[8] = {3,3,3,3,3,3,3,3};
    cout << "hello" << endl;
    unsigned short pat = toShort(pattern);
    cout << pat << endl;
    cout << bitset<sizeof(int)*8>(pat) << endl;
}
*/