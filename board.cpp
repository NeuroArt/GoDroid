#include "board.h"

void kaku::Union(kaku* a,kaku* b){
	kaku* pa = a->findparent();
	kaku* pb = b->findparent();
	if(pa!=pb){
		pa->parent=pb;
		pb->fakeliberty+=pa->fakeliberty;
	}
}

kaku* kaku::findparent(){
	if(this==this->parent)
		return this;
	return this->parent=this->parent->findparent();
}

short kaku::findliberty(){
	return this->findparent()->fakeliberty;
}

short kaku::get_final_status(){
	return this->findparent()->final_status;
}

board::board(){
	for(int i=0;i<SIZE+2;++i){
		goban[0][i].c = border;
		goban[14][i].c = border;
		goban[i][0].c = border;
		goban[i][14].c = border;
	}
	for(int i=0;i<SIZE+2;++i)
		for(int j=0;j<SIZE+2;++j){
			goban[i][j].parent = &goban[i][j];
			goban[i][j].fakeliberty = 0;
			goban[i][j].final_status = UNKNOWN;
		}
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
			goban[i][j].fakeliberty = b.goban[i][j].fakeliberty;
			goban[i][j].final_status = b.goban[i][j].final_status;
			goban[i][j].parent = &goban[0][0]+(b.goban[i][j].parent-&b.goban[0][0]);
			//printf("%d %d\n",&goban[0][0],&b.goban[0][0]);
		}
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
		}
	}
	for(int i=0;i<SIZE+2;++i)
		for(int j=0;j<SIZE+2;++j){
			goban[i][j].parent = &goban[i][j];
			goban[i][j].fakeliberty = 0;
		}
	ko_i = -1;
	ko_j = -1;
/*	emptycells->clear();*/
// 	for (int i=1;i<=SIZE*SIZE;++i)
// 		emptycells->insert(i);
}

void board::place(bool player, kaku*k){
	k->c = player?black:white;
	(E(k))->findparent()->fakeliberty --;
	(W(k))->findparent()->fakeliberty --;
	(S(k))->findparent()->fakeliberty --;
	(N(k))->findparent()->fakeliberty --;
/*	emptycells->erase(((k-&goban[0][0])/(SIZE+2)-1)*SIZE+(k-&goban[0][0])%(SIZE+2));*/
	//printf("%d\n",((k-&goban[0][0])/(SIZE+2)-1)*SIZE+(k-&goban[0][0])%(SIZE+2));
}

bool board::deathtest(kaku* k){
	if(k->findparent()->fakeliberty!=0)
		return false;
	return true;
}

void board::kill(kaku* k){
	k->c = empty;
	(E(k))->findparent()->fakeliberty ++;
	(W(k))->findparent()->fakeliberty ++;
	(S(k))->findparent()->fakeliberty ++;
	(N(k))->findparent()->fakeliberty ++;
/*	emptycells->insert(((k-&goban[0][0])/(SIZE+2)-1)*SIZE+(k-&goban[0][0])%(SIZE+2));*/
	//printf("%d\n",((k-&goban[0][0])/(SIZE+2)-1)*SIZE+(k-&goban[0][0])%(SIZE+2));
}

void board::killall(kaku* k, cell state,int &total){
	k->c = empty;
	k->parent = k;
	k->fakeliberty = 0;
	total += 1;
	ko_i = (k-&goban[0][0])/(SIZE+2);
	ko_j = (k-&goban[0][0])%(SIZE+2);
/*	emptycells->insert(((k-&goban[0][0])/(SIZE+2)-1)*SIZE+(k-&goban[0][0])%(SIZE+2));*/
	//printf("%d\n",((k-&goban[0][0])/(SIZE+2)-1)*SIZE+(k-&goban[0][0])%(SIZE+2));
	cell enemy = (state==white?black:white);
	if ((E(k))->c==enemy)
		(E(k))->findparent()->fakeliberty ++;
	if ((W(k))->c==enemy)
		(W(k))->findparent()->fakeliberty ++;
	if ((S(k))->c==enemy)
		(S(k))->findparent()->fakeliberty ++;
	if ((N(k))->c==enemy)
		(N(k))->findparent()->fakeliberty ++;
	if ((E(k))->c==state)
		killall(E(k),state,total);
	if ((W(k))->c==state)
		killall(W(k),state,total);
	if ((S(k))->c==state)
		killall(S(k),state,total);
	if ((N(k))->c==state)
		killall(N(k),state,total);
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
			if(goban[i][j].c==empty||goban[i][j].c==border)
				printf("%c",43);
			if(goban[i][j].c==black)
				printf("%c",'X');
			if(goban[i][j].c==white)
				printf("%c",'O');
		}
		printf("\n");
	}
}

void board::showboardtofile(){
	FILE *fp;
	fp=fopen("board.txt","w");
	if (fp!=NULL){
		fprintf(fp,"%c",' ');
		for(int i=1;i<SIZE+1;++i){
			fprintf(fp,"%c",'a'+i-1);
		}
		fprintf(fp,"\n");
		for(int i=1;i<SIZE+1;++i){
			fprintf(fp,"%c",'A'+i-1);
			for(int j=1;j<SIZE+1;++j){
				if(goban[i][j].c==empty||goban[i][j].c==border)
					fprintf(fp,"%c",43);
				if(goban[i][j].c==black)
					fprintf(fp,"%c",'X');
				if(goban[i][j].c==white)
					fprintf(fp,"%c",'O');
			}
			fprintf(fp,"\n");
		}
	}
	fclose(fp);
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
/*
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
*/
int board::black_raw(){
    int black = 0;
    for(int pos = 0; pos < SIZE*SIZE; ++pos){
        if(final_status[pos] == WHITE_TERRITORY || final_status[pos] == UNKNOWN) continue;
        if(final_status[pos] == BLACK_TERRITORY){black++; continue;}
        if((final_status[pos] == ALIVE) ^ (get_cell(I(pos), J(pos)) == white)) black++;
    }
    return black;
}

float board::judge(){
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
	return b-w-6.5;
}

bool board::play(bool player,int coordx, int coordy, bool simulation){
	kaku* target = &goban[coordx][coordy];
	cell enemy = player?white:black;
	cell alley = player?black:white;
	//printf("%d %d %d %d\n",coordx,coordy,ko_i,ko_j);
	if (coordx==0 && coordy==0){
		return true;
	}
	if (coordx<1||coordx>SIZE||coordy<1||coordy>SIZE||target->c!=empty||(coordx==ko_i&&coordy==ko_j)){ //ignoring the situation of "pass"
		return false;
	}
	if (simulation&&((E(target))->c==alley||(E(target))->c==border)&&((W(target))->c==alley||(W(target))->c==border)&&((S(target))->c==alley||(S(target))->c==border)&&((N(target))->c==alley||(N(target))->c==border))
		return false;

	place(player,target);
	int total = 0;
	if ((E(target))->c==enemy&&deathtest(E(target)))
		killall(E(target),enemy,total);
	if ((W(target))->c==enemy&&deathtest(W(target)))
		killall(W(target),enemy,total);
	if ((S(target))->c==enemy&&deathtest(S(target)))
		killall(S(target),enemy,total);
	if ((N(target))->c==enemy&&deathtest(N(target)))
		killall(N(target),enemy,total);
	if (total != 1){
		ko_i = -1;
		ko_j = -1;
	}
	//printf("%d %d %d\n",total,ko_i,ko_j);
	bool flag1 = (E(target))->c==empty||((E(target))->c==alley&&(E(target))->findliberty()!=0);
	bool flag2 = (W(target))->c==empty||((W(target))->c==alley&&(W(target))->findliberty()!=0);
	bool flag3 = (S(target))->c==empty||((S(target))->c==alley&&(S(target))->findliberty()!=0);
	bool flag4 = (N(target))->c==empty||((N(target))->c==alley&&(N(target))->findliberty()!=0);
	if (!flag1&&!flag2&&!flag3&&!flag4){
		kill(target);
		return false;
	}
	target->fakeliberty = ((E(target))->c==empty)+((W(target))->c==empty)+((S(target))->c==empty)+((N(target))->c==empty);
	if((E(target))->c==alley)
		kaku::Union(target,E(target));
	if((W(target))->c==alley)
		kaku::Union(target,W(target));
	if((S(target))->c==alley)
		kaku::Union(target,S(target));
	if((N(target))->c==alley)
		kaku::Union(target,N(target));
	return true;
}

board& board::operator=(const board& b){
	for(int i=0;i<SIZE+2;++i)
		for(int j=0;j<SIZE+2;++j){
			goban[i][j].c = b.goban[i][j].c;
			goban[i][j].fakeliberty = b.goban[i][j].fakeliberty;
			goban[i][j].final_status = b.goban[i][j].final_status;
			goban[i][j].parent = &goban[0][0]+(b.goban[i][j].parent-&b.goban[0][0]);
			//printf("%d %d\n",&goban[0][0],&b.goban[0][0]);
		}
		/*	emptycells = new std::set<int>(*b.emptycells);*/
		ko_i = b.ko_i;
		ko_j = b.ko_j;
	return *this;
}

int board::board_empty(){
	for (int i = 1; i <= board_size; ++i)
		for(int j = 1; j <= board_size; ++j)
			if (goban[i][j].c != empty)
				return 0;

	return 1;
}

void board::set_final_status(int i, int j, int status){
	goban[i][j].findparent()->final_status = status;
}

int board::get_final_status(int i, int j){
	return goban[i][j].get_final_status();
}

std::vector<int> board::get_valid_set(bool player){
	std::vector<int> validset;
	for(int coordx=1;coordx<SIZE+1;++coordx)
		for(int coordy=1;coordy<SIZE+1;++coordy){
			kaku* target = &goban[coordx][coordy];
			cell enemy = player?white:black;
			cell alley = player?black:white;
			if (target->c!=empty||(coordx==ko_i&&coordy==ko_j))
				continue;
			if (((E(target))->c==alley||(E(target))->c==border)&&((W(target))->c==alley||(W(target))->c==border)&&((S(target))->c==alley||(S(target))->c==border)&&((N(target))->c==alley||(N(target))->c==border))
				continue;
			(E(target))->findparent()->fakeliberty --;
			(W(target))->findparent()->fakeliberty --;
			(S(target))->findparent()->fakeliberty --;
			(N(target))->findparent()->fakeliberty --;
			if (((E(target))->c==enemy&&(E(target))->findliberty()==0)||((W(target))->c==enemy&&(W(target))->findliberty()==0)||((S(target))->c==enemy&&(S(target))->findliberty()==0)||((N(target))->c==enemy&&(N(target))->findliberty()==0)){
				validset.push_back((coordx-1)*13+coordy);
				(E(target))->findparent()->fakeliberty ++;
				(W(target))->findparent()->fakeliberty ++;
				(S(target))->findparent()->fakeliberty ++;
				(N(target))->findparent()->fakeliberty ++;
				continue;
			}
			//printf("%d %d %d\n",total,ko_i,ko_j);
			bool flag1 = (E(target))->c==empty||((E(target))->c==alley&&(E(target))->findliberty()!=0);
			bool flag2 = (W(target))->c==empty||((W(target))->c==alley&&(W(target))->findliberty()!=0);
			bool flag3 = (S(target))->c==empty||((S(target))->c==alley&&(S(target))->findliberty()!=0);
			bool flag4 = (N(target))->c==empty||((N(target))->c==alley&&(N(target))->findliberty()!=0);
			if (!flag1&&!flag2&&!flag3&&!flag4){
				(E(target))->findparent()->fakeliberty ++;
				(W(target))->findparent()->fakeliberty ++;
				(S(target))->findparent()->fakeliberty ++;
				(N(target))->findparent()->fakeliberty ++;
				continue;
			}
			(E(target))->findparent()->fakeliberty ++;
			(W(target))->findparent()->fakeliberty ++;
			(S(target))->findparent()->fakeliberty ++;
			(N(target))->findparent()->fakeliberty ++;
			validset.push_back((coordx-1)*13+coordy);
		}
	return validset;
}