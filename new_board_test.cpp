#include <iostream>
#include <random>
#include <string>
#include <ctime>
using namespace std;
#include "board.h"

int main(){
	clock_t start,finish;
	start = clock();
	srand(time(NULL));
	//srand((unsigned)time(NULL));
// 	set<int> *es = new set<int>;
// 	for (int i=1;i<=SIZE*SIZE;++i)
// 		es->insert(i);
	board b;
	for (int t=0;t<100000;++t){
		b.clear_board();
		//b.showboard();
		int position1,position2;
		int fail=0;
		while(fail<300){
			bool flag = b.getcurrentplayer();
			position1 = rand()%13+1;
			position2 = rand()%13+1;
			if(b.play(flag, position1, position2)){ //如果play不合法的点会返回false

			}
			else
				fail++;
		}
		b.showboard();
		printf("黑棋比白棋多%d\n",b.judge());
		system("pause");
	}
	finish = clock();
	cout << finish-start<<endl;
	system("pause");
}