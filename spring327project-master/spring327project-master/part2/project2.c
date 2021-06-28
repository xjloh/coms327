#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "formatChecker.h"
#include "utils.h"

FILE *fp;
FILE *fpout;

int turn;
int limit;
int tpLimit;
char foundation[4][2];
Cards tabl[7][20];
Moves* moveList=NULL;
Stock* stok=NULL;
Waste* wast = NULL;

char *infile=NULL;
char *outfile=NULL;
char *strmove=NULL;

int count=0;
int nmoves=-1; //set N moves
int infinity=0; //if N moves is ommited
int x=0; //if -x is given

char suit[] = "cdhs";
char num[] = "_A23456789TJQK";
void newMove(char src, char dest, int count);
int foudRankCheck(char foud, char input);
int tablRankSuitCheck(char lRank, char lSuit, char rRank, char rSuit);

int main(int argc, char* argv[]){
	for(int i=1; i<argc; i++){
		if(strcmp(argv[i], "-m")==0){
			if(argv[i+1]!=NULL){
				strmove=argv[++i];
				char *ptr;
				nmoves=strtol(strmove, &ptr, 10);
				if(strcmp(ptr, "")!=0){
					if(!infile){
						infile=ptr;
						fp=fopen(infile, "r");
					}
					infinity=1;
				}
				if(nmoves<0){
					fprintf(stderr, "%s\n", "Invalid N moves");
					exit(1);
				}
			}
			else{
				infinity=1;
			}
		}
		else if(strcmp(argv[i], "-o")==0){
			outfile=argv[++i];
			fpout=fopen(outfile, "w");
			if(fpout==NULL){
				fprintf(stderr, "%s\n", "File can't be opened!");
				exit(1);
			}
		}
		else if(strcmp(argv[i], "-x")==0){
			x=1;
		}
		else{
			if(!infile){
				infile=argv[i];
				fp=fopen(infile, "r");
			}
		}
	}

	if(!infile){
		fp=stdin;
		if(!fp){
			fprintf(stderr, "%s\n", "No such file or directory");
			return 2;
		}
	}

	if(infile){
		//checks format for moves section
		checkFormat(fp);

		rewind(fp);
		turn=getTurn(fp);
		limit=getLimit(fp);
		tpLimit=limit;

		rewind(fp);
		getFoundation(fp, foundation);

		rewind(fp);
		moveList=addMoves(moveList, fp);

		rewind(fp);
		getTableau(fp, tabl);

		rewind(fp);
		stok=getStock(fp, stok);

		rewind(fp);
		wast=getWaste(fp, wast);

		Moves* mptr=moveList;
		if(infinity==0){
			while(mptr && count<nmoves){
				newMove(mptr->src, mptr->dest, count);
				count++;
				mptr=mptr->next;
			}
		}
		else if(infinity==1){
			while(mptr){
				newMove(mptr->src, mptr->dest, count);
				count++;
				mptr=mptr->next;
			}	
		}
	}
	else{ //use stdin such as cat or <
		int rule=stdinRule(fp);
		turn=stdinTurn(fp);
		// printf("%d\n", turn);
		limit=stdinLimit(fp);
		tpLimit=limit;
		// printf("\n%d\n", limit);
		getFandT(fp, foundation, tabl);
		wast=stdinWaste(fp, wast);
		// Waste* wptr=wast;
		// while(wptr){
		// 	printf("%c%c ", wptr->cards->rank, wptr->cards->suit);
		// 	wptr=wptr->next;
		// }
		stok=stdinStock(fp, stok);
		// Stock* sptr=stok;
		// while(sptr){
		// 	printf("%c%c ", sptr->cards->rank, sptr->cards->suit);
		// 	sptr=sptr->next;
		// }
		moveList=stdinMoves(fp, moveList);
		Moves* tp=moveList;
		// while(tp){
		// 	printf("%c%c ", tp->src, tp->dest);
		// 	tp=tp->next;
		// }
		Moves* mptr=moveList;
		if(nmoves>=0){
			while(mptr && count<nmoves){
				//execute move here
				newMove(mptr->src, mptr->dest, count);
				count++;
				mptr=mptr->next;
			}
		}
		else{
			while(mptr){
				newMove(mptr->src, mptr->dest, count);
				count++;
				mptr=mptr->next;
			}
		}
	}

	if(!outfile){
		if(x){
			exchangeFormat(1, count, turn, tpLimit, foundation, stok, tabl);
		}
		else{
			humanFormat(1, turn, count, foundation, wast, tabl);
		}
	}
	else{
		if(x){
			outFileE_Format(fpout, 1, count, turn, tpLimit, foundation, stok, tabl);
		}
		else{
			outFileH_Format(fpout, 1, turn, count, foundation, wast, tabl);
		}
	}
	return 0;
}

int foudRankCheck(char foud, char input){
	for (int i = 0; i < 14; i ++){
		if (foud == num[i]) {
			if (input == num[i+1]) return 1; 
			else return 0; 
		}
	}
	return 0;
}

int tablRankSuitCheck(char lRank, char lSuit, char rRank, char rSuit){
	int rk = 0; int st = 0; 




	if (lRank == '|'){
		return 1;
	}

	for (int i = 0; i < 14; i ++){
		if (lRank == num[i]) {
			if (rRank == num[i-1]) {
				rk = 1;
			} 
		}
	}
	int tp1 = 99; int tp2 = 99; 
	for (int j = 0; j < 4; j ++){
		if (lSuit == suit[j]) {
			tp1 = j;
		}
		if (rSuit == suit[j]) {
			tp2 = j;
		}
	}
	//printf("%c%c, %c%c -- %d %d\n", lRank, lSuit, rRank, rSuit, tp1, tp2);

	if (rk == 1){

		if (tp1 == 0 || tp1 == 3){
			if (tp2 == 1 || tp2 == 2) return 1;
		} else if (tp1 == 1 || tp1 == 2){
			if (tp2 == 0 || tp2 == 3) return 1;
		}
	} 
	//printAll();
	//printf("%c%c, %c%c -- %d %d\n", lRank, lSuit, rRank, rSuit, tp1, tp2);
	return 0;
}

void newMove(char src, char dest, int count){
	// printf("%c%c\n", src, dest);
	Cards* tp;
	if (src == 'w'){								// src is from waste pile
		if (wast == NULL) {
			fprintf(stderr, "Move %d is illegal: %c->%c\n", count+1, src, dest);
			if(!outfile){
				if(x){
					exchangeFormat(0, count, turn, tpLimit, foundation, stok, tabl);
				}
				else{
					humanFormat(0, turn, count, foundation, wast, tabl);
				}
			}
			else{
				if(x){
					outFileE_Format(fpout, 0, count, turn, tpLimit, foundation, stok, tabl);
				}
				else{
					outFileH_Format(fpout, 0, turn, count, foundation, wast, tabl);
				}
			}
			exit(1);
			//return;
		} else {
			tp = wast->cards;
		}				
		if (dest == 'f'){						// dest is foundation
			Cards* card = (Cards*) malloc (sizeof(card));

			for (int i = 0; i < 4; i ++){
				if (foundation[i][1] == tp-> suit){
					if (!foudRankCheck(foundation[i][0], tp->rank)){
						fprintf(stderr, "Move %d is illegal: %c->%c\n", count+1, src, dest);
						if(!outfile){
							if(x){
								exchangeFormat(0, count, turn, tpLimit, foundation, stok, tabl);
							}
							else{
								humanFormat(0, turn, count, foundation, wast, tabl);
							}
						}
						else{
							if(x){
								outFileE_Format(fpout, 0, count, turn, tpLimit, foundation, stok, tabl);
							}
							else{
								outFileH_Format(fpout, 0, turn, count, foundation, wast, tabl);
							}
						}
						exit(1);
					}
					foundation[i][0] = tp->rank;
					Waste* newWast = (Waste*) malloc (sizeof(Waste));
					newWast=wast;
					wast = wast->next;
					free(newWast);
				}
			}
		} else {								// dest is number
			for (int i = 0; i < sizeof(tabl[0]); i ++){
				int des = dest - '0';

				if (tabl[7 - des][i].rank == '.'){

					if (tablRankSuitCheck(tabl[7 - des][i-1].rank, tabl[7 - des][i-1].suit, tp->rank, tp->suit) == 1){
						tabl[7 - des][i].rank = tp->rank;
						tabl[7 - des][i].suit = tp->suit;
						Waste* newWast = (Waste*) malloc (sizeof(Waste));
						newWast=wast;
						wast = wast->next; 
						free(newWast);
						//printf("%d\n", des);
						//printAll();
						break;
					} else {
						fprintf(stderr, "Move %d is illegal: %c->%c\n", count+1, src, dest);
						if(!outfile){
							if(x){
								exchangeFormat(0, count, turn, tpLimit, foundation, stok, tabl);
							}
							else{
								humanFormat(0, turn, count, foundation, wast, tabl);
							}
						}
						else{
							if(x){
								outFileE_Format(fpout, 0, count, turn, tpLimit, foundation, stok, tabl);
							}
							else{
								outFileH_Format(fpout, 0, turn, count, foundation, wast, tabl);
							}
						}
						exit(1);
					}
				}
			}
		}	

	} else if (src == 'r'){					// move all the waste card back to stock pile 
		if (wast == NULL || stok != NULL){
			fprintf(stderr, "Move %d is illegal: %c->%c\n", count+1, src, dest);
			if(!outfile){
				if(x){
					exchangeFormat(0, count, turn, tpLimit, foundation, stok, tabl);
				}
				else{
					humanFormat(0, turn, count, foundation, wast, tabl);
				}
			}
			else{
				if(x){
					outFileE_Format(fpout, 0, count, turn, tpLimit, foundation, stok, tabl);
				}
				else{
					outFileH_Format(fpout, 0, turn, count, foundation, wast, tabl);
				}
			}
			exit(1);
		}
		if (limit == 0){
			fprintf(stderr, "Reset limit exceeded at move %d\n", count+1);
			if(!outfile){
				if(x){
					exchangeFormat(0, count, turn, tpLimit, foundation, stok, tabl);
				}
				else{
					humanFormat(0, turn, count, foundation, wast, tabl);
				}
			}
			else{
				if(x){
					outFileE_Format(fpout, 0, count, turn, tpLimit, foundation, stok, tabl);
				}
				else{
					outFileH_Format(fpout, 0, turn, count, foundation, wast, tabl);
				}
			}
			exit(1);
		} 
		if (limit == -1){
			while (wast != NULL){
				Stock* newStok = (Stock*) malloc (sizeof(Stock));
				Cards* card = (Cards*) malloc (sizeof(Cards));
				char rank=wast->cards->rank;
				char suit=wast->cards->suit;
				card->rank=rank;
				card->suit=suit;
				newStok->cards = card;
				newStok->next = stok; 
				stok = newStok; 
				Stock* newWast = wast->next; 
				wast = newWast;
			}
		} else if (limit > 0){
			while (wast != NULL){
				Stock* newStok = (Stock*) malloc (sizeof(Stock));
				Cards* card = (Cards*) malloc (sizeof(Cards));
				char rank=wast->cards->rank;
				char suit=wast->cards->suit;
				card->rank=rank;
				card->suit=suit;
				newStok->cards = card;
				newStok->next = stok; 
				stok = newStok; 
				Stock* newWast = wast->next; 
				wast = newWast;
			}
			limit --;
		}		
	} else if (src == '.'){						// take first stock card to waste pile
		int turnTemp = turn; 
		while (turnTemp){
			if (stok != NULL){
				Waste* newWast = (Waste*) malloc (sizeof(Waste));
				Cards* card = (Cards*) malloc (sizeof(Cards));
				char rank=stok->cards->rank;
				char suit=stok->cards->suit;
				card->rank=rank;
				card->suit=suit;
				newWast->cards = card;
				newWast->next = wast; 
				wast = newWast; 
				Stock* newStok = stok; 
				stok = stok->next;
				free(newStok);
			//printAll();
				turnTemp --;
			} else if (stok == NULL && turnTemp == turn){								// Stock pile has no card
				fprintf(stderr, "Move %d is illegal: '.'\n", count+1);
				if(!outfile){
					if(x){
						exchangeFormat(0, count, turn, tpLimit, foundation, stok, tabl);
					}
					else{
						humanFormat(0, turn, count, foundation, wast, tabl);
					}
				}
				else{
					if(x){
						outFileE_Format(fpout, 0, count, turn, tpLimit, foundation, stok, tabl);
					}
					else{
						outFileH_Format(fpout, 0, turn, count, foundation, wast, tabl);
					}
				}
				exit(1);
			}
		}
	} else {	
		//printf("HEREEEE\n");
		int sc = src - '0';							// src is number
		if (dest == 'f'){						// dest is foundation 
			//printf("ITS HERE\n");
			//printAll();
			Cards* card = (Cards*) malloc (sizeof(card));
			int i;
			for (i = 0; i < sizeof(tabl[0]); i ++){
				if (tabl[7-sc][i].rank == '.'){
					card->rank = tabl[7-sc][i-1].rank;
					card->suit = tabl[7-sc][i-1].suit;
					break;
				} 

			}
			for (int j = 0; j < 4; j ++){
				if (foundation[j][1] == card->suit){
					if (!foudRankCheck(foundation[j][0], card->rank)){
						fprintf(stderr, "Move %d is illegal: %c->%c\n", count+1, src, dest);
						if(!outfile){
							if(x){
								exchangeFormat(0, count, turn, tpLimit, foundation, stok, tabl);
							}
							else{
								humanFormat(0, turn, count, foundation, wast, tabl);
							}
						}
						else{
							if(x){
								outFileE_Format(fpout, 0, count, turn, tpLimit, foundation, stok, tabl);
							}
							else{
								outFileH_Format(fpout, 0, turn, count, foundation, wast, tabl);
							}
						}
						exit(1);
					}
					foundation[j][0] = card->rank;
					tabl[7-sc][i-1].rank = '.';
					tabl[7-sc][i-1].suit = '.';
					if (tabl[7-sc][i-2].rank == '|' && i > 2){
						tabl[7-sc][i-2].rank = tabl[7-sc][i-3].rank;
						tabl[7-sc][i-2].suit = tabl[7-sc][i-3].suit;
						tabl[7-sc][i-3].rank = '|';
						tabl[7-sc][i-3].suit = ' ';
					}
				}
			}
			//printAll();
		} else {								// dest is number
			int des = dest - '0';	
			int start = 0;
			int end = 0; 
			int add = 0; 
			int tp = 0;
			for (int i = 0; i < sizeof(tabl[0]); i ++){
				if (tabl[7-sc][i].rank == '|' && start == 0) start = i + 1;
				if (tabl[7-sc][i].rank == '.' && end == 0) end = i; 
				if (tabl[7-des][i].rank == '.' && add == 0) add = i;
				if (tabl[7-des][i].rank == '|' && tp == 0) tp = i;
				//printf("%d\n", add);
			}
			if (add == 1 && tabl[7-sc][start].rank != 'K'){
				fprintf(stderr, "Move %d is illegal: %c->%c\n", count+1, src, dest);
				if(!outfile){
					if(x){
						exchangeFormat(0, count, turn, tpLimit, foundation, stok, tabl);
					}
					else{
						humanFormat(0, turn, count, foundation, wast, tabl);
					}
				}
				else{
					if(x){
						outFileE_Format(fpout, 0, count, turn, tpLimit, foundation, stok, tabl);
					}
					else{
						outFileH_Format(fpout, 0, turn, count, foundation, wast, tabl);
					}
				}
				exit(1);
			}
			if (tablRankSuitCheck(tabl[7-des][add-1].rank, tabl[7-des][add-1].suit, tabl[7-sc][start].rank, tabl[7-sc][start].suit) == 1){
				for (int j = start; j <= end; j ++){
					tabl[7-des][add].rank = tabl[7-sc][j].rank;
					tabl[7-des][add].suit = tabl[7-sc][j].suit;
					tabl[7-sc][j].rank = '.';
					tabl[7-sc][j].suit = '.';
					add++; 
				}
				if (start >= 2){
					char rank = tabl[7-sc][start - 2].rank; 
					char suit = tabl[7-sc][start - 2].suit; 
				//printf("HERE?%c%c\n", rank, suit);
					tabl[7-sc][start - 2].rank = '|';
					tabl[7-sc][start - 2].suit = ' ';
					tabl[7-sc][start - 1].rank = rank;
					tabl[7-sc][start - 1].suit = suit;
				}
			} else {
				fprintf(stderr, "Move %d is illegal: %c->%c\n", count+1, src, dest);
				if(!outfile){
					if(x){
						exchangeFormat(0, count, turn, tpLimit, foundation, stok, tabl);
					}
					else{
						humanFormat(0, turn, count, foundation, wast, tabl);
					}
				}
				else{
					if(x){
						outFileE_Format(fpout, 0, count, turn, tpLimit, foundation, stok, tabl);
					}
					else{
						outFileH_Format(fpout, 0, turn, count, foundation, wast, tabl);
					}
				}
				exit(1);
			}

		}
		//printAll();
	}
	//printAll(tabl, stok, wast, foud);
}

