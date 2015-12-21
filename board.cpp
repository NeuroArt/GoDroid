#include "board.h"

board::board(){
	for(int i=0;i<SIZE+2;++i){
		goban[0][i].c = border;
		goban[14][i].c = border;
		goban[i][0].c = border;
		goban[i][14].c = border;

		goban[0][i].liberty = 0;
		goban[14][i].liberty = 0;
		goban[i][0].liberty = 0;
		goban[i][14].liberty = 0;

		if (goban[1][i].liberty>0)
			goban[1][i].liberty--;
		if (goban[13][i].liberty>0)
			goban[13][i].liberty--;
		if (goban[i][1].liberty>0)
			goban[i][1].liberty--;
		if (goban[i][13].liberty>0)
			goban[i][13].liberty--;
	}
	currentplayer = true;
	ko_i = -1;
	ko_j = -1;
// 	if (es==NULL){
// 		emptycells = new std::set<int>;
// 		for (int i=1;i<=SIZE*SIZE;++i)
// 			emptycells->insert(i);
// 	}
// 	else
// 		emptycells = new std::set<int>(*es);
}

board::board(const board& b){
	for(int i=0;i<SIZE+2;++i)
		for(int j=0;j<SIZE+2;++j){
			goban[i][j].c = b.goban[i][j].c;
			goban[i][j].liberty = b.goban[i][j].liberty;
		}
	currentplayer = b.currentplayer;
/*	emptycells = new std::set<int>(*b.emptycells);*/
	ko_i = b.ko_i;
	ko_j = b.ko_j;
}

board::~board(){
/*	delete emptycells;*/
}

void board::clear_board(){
	for(int i=1;i<SIZE+1;++i){
		for(int j=1;j<SIZE+1;++j){
			goban[i][j].c = empty;
			goban[i][j].liberty=4;
		}
	}
	for(int i=0;i<SIZE+2;++i){
		goban[0][i].c = border;
		goban[14][i].c = border;
		goban[i][0].c = border;
		goban[i][14].c = border;

		goban[0][i].liberty = 0;
		goban[14][i].liberty = 0;
		goban[i][0].liberty = 0;
		goban[i][14].liberty = 0;

		if (goban[1][i].liberty>0)
			goban[1][i].liberty--;
		if (goban[13][i].liberty>0)
			goban[13][i].liberty--;
		if (goban[i][1].liberty>0)
			goban[i][1].liberty--;
		if (goban[i][13].liberty>0)
			goban[i][13].liberty--;
	}
	currentplayer = true;
	ko_i = -1;
	ko_j = -1;
/*	emptycells->clear();*/
// 	for (int i=1;i<=SIZE*SIZE;++i)
// 		emptycells->insert(i);
}

void board::place(bool player, kaku*k){
	k->c = player?black:white;
	if ((k+1)->c!=border)
		(k+1)->liberty --;
	if ((k-1)->c!=border)
		(k-1)->liberty --;
	if ((k+SIZE+2)->c!=border)
		(k+SIZE+2)->liberty --;
	if ((k-(SIZE+2))->c!=border)
		(k-(SIZE+2))->liberty --;
/*	emptycells->erase(((k-&goban[0][0])/(SIZE+2)-1)*SIZE+(k-&goban[0][0])%(SIZE+2));*/
	//printf("%d\n",((k-&goban[0][0])/(SIZE+2)-1)*SIZE+(k-&goban[0][0])%(SIZE+2));
}

bool board::deathtest(kaku* k){
	k->visited = true;
	if(k->liberty!=0){
		k->visited = false;
		return false;
	}
	bool flag1=true,flag2=true,flag3=true,flag4=true;
	if ((k+1)->c == k->c&&!(k+1)->visited)
		flag1 = deathtest(k+1);
	if ((k-1)->c == k->c&&!(k-1)->visited)
		flag2 = deathtest(k-1);
	if ((k+SIZE+2)->c == k->c&&!(k+SIZE+2)->visited)
		flag3 = deathtest(k+SIZE+2);
	if ((k-(SIZE+2))->c == k->c&&!(k-(SIZE+2))->visited)
		flag4 = deathtest(k-(SIZE+2));
	k->visited = false;
	return flag1&&flag2&&flag3&&flag4;
}

void board::kill(kaku* k){
	k->c = empty;
	if ((k+1)->c!=border)
		(k+1)->liberty ++;
	if ((k-1)->c!=border)
		(k-1)->liberty ++;
	if ((k+SIZE+2)->c!=border)
		(k+SIZE+2)->liberty ++;
	if ((k-(SIZE+2))->c!=border)
		(k-(SIZE+2))->liberty ++;
/*	emptycells->insert(((k-&goban[0][0])/(SIZE+2)-1)*SIZE+(k-&goban[0][0])%(SIZE+2));*/
	//printf("%d\n",((k-&goban[0][0])/(SIZE+2)-1)*SIZE+(k-&goban[0][0])%(SIZE+2));
}

void board::killall(kaku* k, cell state,int &total){
	k->c = empty;
	total += 1;
	ko_i = (k-&goban[0][0])/(SIZE+2);
	ko_j = (k-&goban[0][0])%(SIZE+2);
/*	emptycells->insert(((k-&goban[0][0])/(SIZE+2)-1)*SIZE+(k-&goban[0][0])%(SIZE+2));*/
	//printf("%d\n",((k-&goban[0][0])/(SIZE+2)-1)*SIZE+(k-&goban[0][0])%(SIZE+2));
	if ((k+1)->c!=border)
		(k+1)->liberty ++;
	if ((k-1)->c!=border)
		(k-1)->liberty ++;
	if ((k+SIZE+2)->c!=border)
		(k+SIZE+2)->liberty ++;
	if ((k-(SIZE+2))->c!=border)
		(k-(SIZE+2))->liberty ++;
	if ((k+1)->c==state)
		killall(k+1,state,total);
	if ((k-1)->c==state)
		killall(k-1,state,total);
	if ((k+SIZE+2)->c==state)
		killall(k+SIZE+2,state,total);
	if ((k-(SIZE+2))->c==state)
		killall(k-(SIZE+2),state,total);
}

void board::showboard(){
	printf("%c",' ');
	for(int i=1;i<SIZE+1;++i){
		printf("%c",'a'+i-1);
	}
	printf("\n");
	for(int i=1;i<SIZE+1;++i){
		printf("%c",'A'+i-1);
		for(int j=1;j<SIZE+1;++j){
			if(goban[i][j].c==0||goban[i][j].c==4)
				printf("%c",43);
			if(goban[i][j].c==1)
				printf("%c",'X');
			if(goban[i][j].c==2)
				printf("%c",'O');
		}
		printf("\n");
	}
}

// std::set<int>* board::getemptycells(){
// 	//std::vector<int> result;
// 	//result.clear();
// 	//std::set<int>::iterator it;
// 	//for(it=emptycells.begin();it!=emptycells.end();it++){
// 	//	result.push_back(*it);
// 	//}
// 	//return result;
// 	return emptycells;
// }

cell board::get_cell(int i, int j){
    return goban[i][j].c;
}

void board::compute_final_status(){
    for(int pos = 0; pos < SIZE*SIZE; ++pos)
        final_status[pos] = UNKNOWN;

    for(int i = 1; i < SIZE+1; ++i){
        for(int j = 1; j < SIZE+1; ++j){
            if(get_cell(i, j) == empty){
                for(int k = 0; k < 4; k++){
                    int ai = i + offset_x[k];
                    int aj = j + offset_y[k];
                    if(get_cell(ai, aj) == border)
                        continue;
                    int pos = POS(ai,aj);
                    if (final_status[pos] == UNKNOWN) {
                        if (get_cell(ai, aj) != empty) {
                            if (has_additional_liberty(ai, aj, i, j))
                                set_final_status_string(pos, ALIVE);
                            else
                                set_final_status_string(pos, DEAD);
                        }
                    }
                    if (final_status[POS(i, j)] == UNKNOWN) {
						if ((final_status[pos] == ALIVE) ^ (get_cell(ai, aj) == white))
                            final_status[POS(i, j)] = BLACK_TERRITORY;
                        else
                            final_status[POS(i, j)] = WHITE_TERRITORY;
                    }
                }
            }
        }
    }
}

int board::black_raw(){
    int black = 0;
    for(int pos = 0; pos < SIZE*SIZE; ++pos){
        if(final_status[pos] == WHITE_TERRITORY || final_status[pos] == UNKNOWN) continue;
        if(final_status[pos] == BLACK_TERRITORY){black++; continue;}
        if((final_status[pos] == ALIVE) ^ (get_cell(I(pos), J(pos)) == white)) black++;
    }
    return black;
}

int board::judge(){
	int b=0;
	int w=0;
	for(int i=1;i<SIZE+1;++i){
		for(int j=1;j<SIZE+1;++j){
			if (goban[i][j].c == black)
				b++;
			if (goban[i][j].c == white)
				w++;
		}
	}
	return b-w;
}

bool board::play(bool& player,int coordx, int coordy){
	kaku* target = &goban[coordx][coordy];
	//printf("%d %d %d %d\n",coordx,coordy,ko_i,ko_j);
	if (player != currentplayer||coordx<1||coordx>13||coordy<1||coordy>13||target->c!=empty||(coordx==ko_i&&coordy==ko_j)){ //ignoring the situation of "pass"
		return false;
	}
	bool flag = false; //flag indicates has any enemy been eliminated.
	cell enemy = player?white:black;
	place(player,target);
	int total = 0;
	if ((target+1)->c==enemy&&deathtest(target+1))
		killall(target+1,enemy,total);
	if ((target-1)->c==enemy&&deathtest(target-1))
		killall(target-1,enemy,total);
	if ((target+SIZE+2)->c==enemy&&deathtest(target+SIZE+2))
		killall(target+SIZE+2,enemy,total);
	if ((target-(SIZE+2))->c==enemy&&deathtest(target-(SIZE+2)))
		killall(target-(SIZE+2),enemy,total);
	if (total != 1){
		ko_i = -1;
		ko_j = -1;
	}
	//printf("%d %d %d\n",total,ko_i,ko_j);
	if (deathtest(target)){
		kill(target);
		return false;
	}
	currentplayer = !currentplayer;
	return true;
}
