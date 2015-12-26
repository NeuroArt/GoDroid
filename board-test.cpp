#include <iostream>
#include <random>
#include <string>
#include <ctime>
#include <set>
using namespace std;
#include "board.h"

int main(){
	clock_t start,finish;
	start = clock();
	srand((unsigned)time(NULL));
	set<int> *es = new set<int>;
	for (int i=1;i<=SIZE*SIZE;++i)
		es->insert(i);
	board b(es);
	b.showboard();
	while(true){
	//for (int t=0;t<20000;++t){
		char position1,position2;
		//b.showboard();
		//for(int n=0;n<600;++n){
			bool flag = b.getcurrentplayer();
			//position2 = rand()%13+1;
			//b.showboard();
			cout<<"现在轮到"<<(flag?"黑棋":"白棋")<<endl;
			cin >> position1 >> position2;
			b.play(flag, position1-'A'+1, position2-'a'+1); //如果play不合法的点会返回false
			b.showboard();
			//vector<int> empty = b.getemptycells();
		//}
	//}
	}
	finish = clock();
	cout << finish-start<<endl;
	system("pause");
}