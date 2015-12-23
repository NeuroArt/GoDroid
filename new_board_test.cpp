#include <iostream>
#include <random>
#include <string>
#include <ctime>
using namespace std;
#include "board.h"
#include "UCT.h"
#define MAXINT 32767

int main(){
	clock_t start,finish;
	
	srand(time(NULL));
	//srand((unsigned)time(NULL));
// 	set<int> *es = new set<int>;
// 	for (int i=1;i<=SIZE*SIZE;++i)
// 		es->insert(i);
	board b;//全局唯一的board
	int fail = 0;
	for(int i = 0; i < 200; ++i){
		
		UCT tree(b);//当前board下新建UCT树
		start = clock();
		finish = clock();
		int cnt = 0;
		while(finish - start < 1000){ //一直模拟到2秒钟
			tree.playOneSequenceInMoGo();
			finish = clock();
			cnt++;
		}
		printf("cnt: %d\n", cnt);
		tree.showTotal();
		int move = tree.getNextMove();//获得模拟出的最佳下一步
		int positionX = move / SIZE + 1;
		int positionY = move % SIZE + 1;
		bool flag = b.getcurrentplayer();
		printf("The next move is %d, %d\n", positionX, positionY);
		if (b.play(flag, positionX, positionY)){//玩它

		}
		else
			fail++;
		printf("fail: %d\n", fail);
		finish = clock();
		cout << finish - start << endl;
		b.showboard();
	}
	/*start = clock();
	for(int i = 0; i < 10000; ++i) {
		montecarlo m(b);
	}
	finish = clock();
	printf("%d\n", finish - start);*/
	
	//printf("黑棋比白棋多%d\n",b.judge());
	system("pause");
}