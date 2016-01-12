#define _CRT_SECURE_NO_WARNINGS
#include "board.h"
#include <fstream>
using namespace std;

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
	for(int i=0;i<BOARDSIZE+2;++i){
		goban[0][i].c = border;
		goban[14][i].c = border;
		goban[i][0].c = border;
		goban[i][14].c = border;
	}
	for(int i=0;i<BOARDSIZE+2;++i)
		for(int j=0;j<BOARDSIZE+2;++j){
			goban[i][j].parent = &goban[i][j];
			goban[i][j].fakeliberty = 0;
			goban[i][j].final_status = UNKNOWN;
		}
	ko_i = -1;
	ko_j = -1;

	validsetforblack.clear();
	validsetforwhite.clear();
	for (int i=1;i<=BOARDSIZE*BOARDSIZE;++i){
		validsetforblack.insert(i);
		validsetforwhite.insert(i);
		goban[(i-1)/BOARDSIZE+1][(i-1)%BOARDSIZE+1].position=i;
	}
	ataripositionforblack.clear();
	ataripositionforwhite.clear();
}

board::board(const board& b){
	for(int i=0;i<BOARDSIZE+2;++i)
		for(int j=0;j<BOARDSIZE+2;++j){
			goban[i][j].c = b.goban[i][j].c;
			goban[i][j].fakeliberty = b.goban[i][j].fakeliberty;
			goban[i][j].final_status = b.goban[i][j].final_status;
			goban[i][j].parent = &goban[0][0]+(b.goban[i][j].parent-&b.goban[0][0]);
			//printf("%d %d\n",&goban[0][0],&b.goban[0][0]);
		}
/*	emptycells = new std::set<int>(*b.emptycells);*/
	validsetforblack = b.validsetforblack;
	validsetforwhite = b.validsetforwhite;
	for (int i=1;i<=BOARDSIZE*BOARDSIZE;++i){
		goban[(i-1)/BOARDSIZE+1][(i-1)%BOARDSIZE+1].position=i;
	}
	ataripositionforblack = b.ataripositionforblack;
	ataripositionforwhite = b.ataripositionforwhite;
	ko_i = b.ko_i;
	ko_j = b.ko_j;
}

board::~board(){
}

void board::clear_board(){
	for(int i=1;i<BOARDSIZE+1;++i){
		for(int j=1;j<BOARDSIZE+1;++j){
			goban[i][j].c = empty;
		}
	}
	for(int i=0;i<BOARDSIZE+2;++i)
		for(int j=0;j<BOARDSIZE+2;++j){
			goban[i][j].parent = &goban[i][j];
			goban[i][j].fakeliberty = 0;
		}
	ko_i = -1;
	ko_j = -1;
/*	emptycells->clear();*/
// 	for (int i=1;i<=BOARDSIZE*BOARDSIZE;++i)
// 		emptycells->insert(i);
	validsetforblack.clear();
	validsetforwhite.clear();
	for (int i=1;i<=BOARDSIZE*BOARDSIZE;++i){
		validsetforblack.insert(i);
		validsetforwhite.insert(i);
	}
	ataripositionforblack.clear();
	ataripositionforwhite.clear();
}

void board::place(bool player, kaku*k){
	k->c = player?black:white;
	(E(k))->findparent()->fakeliberty --;
	(W(k))->findparent()->fakeliberty --;
	(S(k))->findparent()->fakeliberty --;
	(N(k))->findparent()->fakeliberty --;
	validsetforblack.erase(k->position);
	validsetforwhite.erase(k->position);
	ataripositionforblack.erase(k->position);
	ataripositionforwhite.erase(k->position);

	if (E(k)->c==empty){
		if (!valid_test(E(k),1))
			validsetforblack.erase(E(k)->position);
		if (!valid_test(E(k),0))
			validsetforwhite.erase(E(k)->position);
		if (SURROUNDED(E(k),k->c)){
			if(k->c==black)
				validsetforblack.erase(E(k)->position);
			else
				validsetforwhite.erase(E(k)->position);
		}
	}
	if (S(k)->c==empty){
		if (!valid_test(S(k),1))
			validsetforblack.erase(S(k)->position);
		if (!valid_test(S(k),0))
			validsetforwhite.erase(S(k)->position);
		if (SURROUNDED(S(k),k->c)){
			if(k->c==black)
				validsetforblack.erase(S(k)->position);
			else
				validsetforwhite.erase(S(k)->position);
		}
	}
	if (W(k)->c==empty){
		if (!valid_test(W(k),1))
			validsetforblack.erase(W(k)->position);
		if (!valid_test(W(k),0))
			validsetforwhite.erase(W(k)->position);
		if (SURROUNDED(W(k),k->c)){
			if(k->c==black)
				validsetforblack.erase(W(k)->position);
			else
				validsetforwhite.erase(W(k)->position);
		}
	}
	if (N(k)->c==empty){
		if (!valid_test(N(k),1))
			validsetforblack.erase(N(k)->position);
		if (!valid_test(N(k),0))
			validsetforwhite.erase(N(k)->position);
		if (SURROUNDED(N(k),k->c)){
			if(k->c==black)
				validsetforblack.erase(N(k)->position);
			else
				validsetforwhite.erase(N(k)->position);
		}
	}
}

bool board::deathtest(kaku* k){
	if(k->findparent()->fakeliberty!=0)
		return false;
	return true;
}

void board::kill(kaku* k){ //逻辑正确的话这个函数是永远不可能执行的
	k->c = empty;
	(E(k))->findparent()->fakeliberty ++;
	(W(k))->findparent()->fakeliberty ++;
	(S(k))->findparent()->fakeliberty ++;
	(N(k))->findparent()->fakeliberty ++;
/*	emptycells->insert(((k-&goban[0][0])/(BOARDSIZE+2)-1)*BOARDSIZE+(k-&goban[0][0])%(BOARDSIZE+2));*/
	//printf("%d\n",((k-&goban[0][0])/(BOARDSIZE+2)-1)*BOARDSIZE+(k-&goban[0][0])%(BOARDSIZE+2));
}

void board::killall(kaku* k, cell state,int &total){
	k->c = empty;
	k->parent = k;
	k->fakeliberty = 0;
	total += 1;
	ko_i = (k-&goban[0][0])/(BOARDSIZE+2);
	ko_j = (k-&goban[0][0])%(BOARDSIZE+2);
	validsetforblack.insert(k->position);
	validsetforwhite.insert(k->position);

/*	emptycells->insert(((k-&goban[0][0])/(BOARDSIZE+2)-1)*BOARDSIZE+(k-&goban[0][0])%(BOARDSIZE+2));*/
	//printf("%d\n",((k-&goban[0][0])/(BOARDSIZE+2)-1)*BOARDSIZE+(k-&goban[0][0])%(BOARDSIZE+2));
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

void board::ataritest(kaku* k, cell state,int &total){
	if (total>1)
		return;
	k->ataritested = true;
	if ((E(k))->c==empty&&!E(k)->ataritested){
		if (state==black)
			ataripositionforblack.erase(E(k)->position);
		else
			ataripositionforwhite.erase(E(k)->position);
		atariposition = E(k)->position;
		E(k)->ataritested = true;
		total++;
	}
	if ((W(k))->c==empty&&!W(k)->ataritested){
		if (state==black)
			ataripositionforblack.erase(W(k)->position);
		else
			ataripositionforwhite.erase(W(k)->position);
		atariposition = W(k)->position;
		W(k)->ataritested = true;
		total++;
	}
	if ((S(k))->c==empty&&!S(k)->ataritested){
		if (state==black)
			ataripositionforblack.erase(S(k)->position);
		else
			ataripositionforwhite.erase(S(k)->position);
		atariposition = S(k)->position;
		S(k)->ataritested = true;
		total++;
	}
	if ((N(k))->c==empty&&!N(k)->ataritested){
		if (state==black)
			ataripositionforblack.erase(N(k)->position);
		else
			ataripositionforwhite.erase(N(k)->position);
		atariposition = N(k)->position;
		N(k)->ataritested = true;
		total++;
	}
	if ((E(k))->c==state&&!E(k)->ataritested)
		ataritest(E(k),state,total);
	if ((W(k))->c==state&&!W(k)->ataritested)
		ataritest(W(k),state,total);
	if ((S(k))->c==state&&!S(k)->ataritested)
		ataritest(S(k),state,total);
	if ((N(k))->c==state&&!N(k)->ataritested)
		ataritest(N(k),state,total);
}

void board::refreshtest(kaku*k){
	k->ataritested=false;
	if ((E(k))->ataritested==true)
		refreshtest(E(k));
	if ((W(k))->ataritested==true)
		refreshtest(W(k));
	if ((S(k))->ataritested==true)
		refreshtest(S(k));
	if ((N(k))->ataritested==true)
		refreshtest(N(k));
}

void board::showboard(){
// 	ofstream fout("log3.txt", ios::app);
// 	//printf("%c",' ');
// 	fout << ' ';
// 	for(int i=1;i<BOARDSIZE+1;++i){
// 		//printf("%c",'a'+i-1);
// 		fout << char('a' + i - 1) << ' ';
// 	}
// 	//printf("\n");
// 	fout << endl;
// 	for(int i=1;i<BOARDSIZE+1;++i){
// 		//printf("%c",'A'+i-1);
// 		fout << char('A' + i - 1) << ' ';
// 		for(int j=1;j<BOARDSIZE+1;++j){
// 			if (goban[i][j].c == empty || goban[i][j].c == border)
// 				fout << ". ";
// 			if (goban[i][j].c == black)
// 				fout << "X ";
// 			if (goban[i][j].c == white)
// 				fout << "O ";
// 		}
// 		fout << endl;
// 	}

	printf("%c",' ');
	for(int i=1;i<BOARDSIZE+1;++i){
		printf("%c",'a'+i-1);
	}
	printf("\n");
	for(int i=1;i<BOARDSIZE+1;++i){
		printf("%c",'A'+i-1);
		for(int j=1;j<BOARDSIZE+1;++j){
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
		for(int i=1;i<BOARDSIZE+1;++i){
			fprintf(fp,"%c",'a'+i-1);
		}
		fprintf(fp,"\n");
		for(int i=1;i<BOARDSIZE+1;++i){
			fprintf(fp,"%c",'A'+i-1);
			for(int j=1;j<BOARDSIZE+1;++j){
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

cell board::get_cell(int i, int j){
    return goban[i][j].c;
}
/*
void board::compute_final_status(){
    for(int pos = 0; pos < BOARDSIZE*BOARDSIZE; ++pos)
        final_status[pos] = UNKNOWN;

    for(int i = 1; i < BOARDSIZE+1; ++i){
        for(int j = 1; j < BOARDSIZE+1; ++j){
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
    for(int pos = 0; pos < BOARDSIZE*BOARDSIZE; ++pos){
        if(final_status[pos] == WHITE_TERRITORY || final_status[pos] == UNKNOWN) continue;
        if(final_status[pos] == BLACK_TERRITORY){black++; continue;}
        if((final_status[pos] == ALIVE) ^ (get_cell(I(pos), J(pos)) == white)) black++;
    }
    return black;
}

double board::judge(){
	int b=0;
	int w=0;
	for(int i=1;i<BOARDSIZE+1;++i){
		for(int j=1;j<BOARDSIZE+1;++j){
			if (goban[i][j].c == black)
				b++;
			else if (goban[i][j].c == white)
				w++;
			else{
				b += ((E(&goban[i][j]))->c==black)+((W(&goban[i][j]))->c==black)+((S(&goban[i][j]))->c==black)+((N(&goban[i][j]))->c==black)+((N(&goban[i][j])-1)->c==black)+((N(&goban[i][j])+1)->c==black)+((S(&goban[i][j])-1)->c==black)+((S(&goban[i][j])+1)->c==black);
				w += ((E(&goban[i][j]))->c==white)+((W(&goban[i][j]))->c==white)+((S(&goban[i][j]))->c==white)+((N(&goban[i][j]))->c==white)+((N(&goban[i][j])-1)->c==white)+((N(&goban[i][j])+1)->c==white)+((S(&goban[i][j])-1)->c==white)+((S(&goban[i][j])+1)->c==white);
			}
		}
	}
	return b-w-6.5;
}


double board::njudge(){
	int b = 0;
	int w = 0;
	for(int i = 1; i <= BOARDSIZE; ++i)
		for(int j = 1; j <= BOARDSIZE; ++j)
			if(get_cell(i,j) == empty)
				for(int k = 0; j < 4; ++k){
					int ai = i + offset_x[k];
					int aj = j + offset_y[k];
					if(ai<1 || ai>BOARDSIZE || aj<1 || aj>BOARDSIZE)
						continue;
					if(get_final_status(ai,aj) == UNKNOWN){
						if(get_cell(ai,aj) == empty){
							if(find_liberty(ai,aj) > 1)
								set_final_status(ai,aj,ALIVE);
							else
								set_final_status(ai,aj,DEAD);
						}
					}
					if(get_final_status(i,j) == UNKNOWN){
						if((get_final_status(ai,aj) == ALIVE) ^ (get_cell(ai,aj) == white))
							set_final_status(i,j,BLACK_TERRITORY);
						else
							set_final_status(i,j,WHITE_TERRITORY);
					}
				}
	for(int i = 1; i <= BOARDSIZE; ++i)
		for(int j = 1; j <= BOARDSIZE; ++j)
			if(get_final_status(i,j) == WHITE_TERRITORY || (get_final_status(i,j) == ALIVE ^ get_cell(i,j) == black))
				w++;
			else if(get_final_status(i,j) == BLACK_TERRITORY || (get_final_status(i,j) == ALIVE ^ get_cell(i,j) == white))
				b++;
	return b-w-6.5

}

bool board::play(bool player,int coordx, int coordy, bool simulation){
	kaku* target = &goban[coordx][coordy];
	cell enemy = player?white:black;
	cell alley = player?black:white;
	//printf("%d %d %d %d\n",coordx,coordy,ko_i,ko_j);
	if (coordx==0 && coordy==0){
		ko_i=-1;
		ko_j=-1;
		return true;
	}
	if (coordx<1||coordx>BOARDSIZE||coordy<1||coordy>BOARDSIZE||target->c!=empty){
		return false;
	}
	if (coordx==ko_i&&coordy==ko_j&&E(target)->c==enemy&&W(target)->c==enemy&&S(target)->c==enemy&&N(target)->c==enemy)
		if(E(target)->findliberty()==1||W(target)->findliberty()==1||S(target)->findliberty()==1||N(target)->findliberty()==1){
			ko_i = -1;
			ko_j = -1;
			return false;
		}
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
	else if(total==1){
		if(player&&!valid_test(&goban[ko_i][ko_j],0))
			validsetforwhite.erase(goban[ko_i][ko_j].position);
		else if(!player&&!valid_test(&goban[ko_i][ko_j],1))
			validsetforblack.erase(goban[ko_i][ko_j].position);
		if(player)
			validsetforblack.erase(goban[ko_i][ko_j].position);
		else
			validsetforwhite.erase(goban[ko_i][ko_j].position);
	}

	//printf("%d %d %d\n",total,ko_i,ko_j);
// 	bool flag1 = (E(target))->c==empty||((E(target))->c==alley&&(E(target))->findliberty()!=0);
// 	bool flag2 = (W(target))->c==empty||((W(target))->c==alley&&(W(target))->findliberty()!=0);
// 	bool flag3 = (S(target))->c==empty||((S(target))->c==alley&&(S(target))->findliberty()!=0);
// 	bool flag4 = (N(target))->c==empty||((N(target))->c==alley&&(N(target))->findliberty()!=0);
// 	if (!flag1&&!flag2&&!flag3&&!flag4){
// 		kill(target); // 按道理来说，逻辑正确的话这行指令是永远不可能执行的，为求保险写上此行//自杀就自杀吧，谁也保不了你
// 		return false;
// 	}

	if (E(target)->c==enemy){ //维护atari点
		total = 0;
		ataritest(E(target),enemy,total);
		if (total==1){
			if (enemy==black)
				ataripositionforblack.insert(atariposition);
			else
				ataripositionforwhite.insert(atariposition);
		}
		refreshtest(E(target));
	}
	if (W(target)->c==enemy)
		if ((W(target)->findparent())!=(E(target)->findparent())){
			total = 0;
			ataritest(W(target),enemy,total);
			if (total==1){
				if (enemy==black)
					ataripositionforblack.insert(atariposition);
				else
					ataripositionforwhite.insert(atariposition);
			}
			refreshtest(W(target));

		}
	if (S(target)->c==enemy)
		if (((S(target)->findparent())!=(E(target)->findparent()))&&((S(target)->findparent())!=(W(target)->findparent()))){
			total = 0;
			ataritest(S(target),enemy,total);
			if (total==1){
				if (enemy==black)
					ataripositionforblack.insert(atariposition);
				else
					ataripositionforwhite.insert(atariposition);
			}
			refreshtest(S(target));

		}
	if (N(target)->c==enemy)
		if (((N(target)->findparent())!=(E(target)->findparent()))&&((N(target)->findparent())!=(W(target)->findparent()))&&((N(target)->findparent())!=(S(target)->findparent()))){
			total = 0;
			ataritest(N(target),enemy,total);
			if (total==1){
				if (enemy==black)
					ataripositionforblack.insert(atariposition);
				else
					ataripositionforwhite.insert(atariposition);
			}
			refreshtest(N(target));
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
	if (deathtest(target)){
		killall(target,alley,total);
		return true;
	}
	total = 0;
	ataritest(target,alley,total);
	if (total==1){
		if (alley==black)
			ataripositionforblack.insert(atariposition);
		else
			ataripositionforwhite.insert(atariposition);
	}
	refreshtest(target);
	return true;
}

board& board::operator=(const board& b){
	for(int i=0;i<BOARDSIZE+2;++i)
		for(int j=0;j<BOARDSIZE+2;++j){
			goban[i][j].c = b.goban[i][j].c;
			goban[i][j].fakeliberty = b.goban[i][j].fakeliberty;
			goban[i][j].final_status = b.goban[i][j].final_status;
			goban[i][j].parent = &goban[0][0]+(b.goban[i][j].parent-&b.goban[0][0]);
			//printf("%d %d\n",&goban[0][0],&b.goban[0][0]);
		}
		/*	emptycells = new std::set<int>(*b.emptycells);*/
	ko_i = b.ko_i;
	ko_j = b.ko_j;
	validsetforblack = b.validsetforblack;
	validsetforwhite = b.validsetforwhite;
	for (int i=1;i<=BOARDSIZE*BOARDSIZE;++i){
		goban[(i-1)/BOARDSIZE+1][(i-1)%BOARDSIZE+1].position=i;
	}
	ataripositionforblack = b.ataripositionforblack;
	ataripositionforwhite = b.ataripositionforwhite;
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

bool board::valid_test(kaku* target, bool player){
	cell enemy = player?white:black;
	cell alley = player?black:white;
	short coordx = (target->position-1)/BOARDSIZE+1;
	short coordy = (target->position-1)%BOARDSIZE+1;
	if (coordx<1||coordx>BOARDSIZE||coordy<1||coordy>BOARDSIZE||target->c!=empty||(coordx==ko_i&&coordy==ko_j)){
		return false;
	}
	(E(target))->findparent()->fakeliberty --;
	(W(target))->findparent()->fakeliberty --;
	(S(target))->findparent()->fakeliberty --;
	(N(target))->findparent()->fakeliberty --;
	if (((E(target))->c==enemy&&(E(target))->findliberty()==0)||((W(target))->c==enemy&&(W(target))->findliberty()==0)||((S(target))->c==enemy&&(S(target))->findliberty()==0)||((N(target))->c==enemy&&(N(target))->findliberty()==0)){
		(E(target))->findparent()->fakeliberty ++;
		(W(target))->findparent()->fakeliberty ++;
		(S(target))->findparent()->fakeliberty ++;
		(N(target))->findparent()->fakeliberty ++;
		return true;
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
		return false;
	}
	(E(target))->findparent()->fakeliberty ++;
	(W(target))->findparent()->fakeliberty ++;
	(S(target))->findparent()->fakeliberty ++;
	(N(target))->findparent()->fakeliberty ++;
	return true;
}

bool board::emptytest(int target){
	int coordx = target / BOARDSIZE + 1;
	int coordy = target % BOARDSIZE + 1;
	kaku* t = &goban[coordx][coordy];
	return (t->c == empty) && (E(t)->c == empty) && (W(t)->c == empty) && (S(t)->c == empty) && (N(t)->c == empty) && ((S(t)-1)->c == empty) && ((S(t)+1)->c == empty) && ((N(t)-1)->c == empty) && ((N(t)+1)->c == empty);
}