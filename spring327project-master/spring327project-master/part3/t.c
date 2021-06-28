#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pt3utils.h"

//global variables
int m;
int turn;
int limit;
char suit[] = "cdhs";
char num[] = "_A23456789TJQK";
//moves: .(T cards to be turned over from the stock to the waste)   
//r (causing the waste pile to be reset back to the stock)
//src->dest
//src:w1-7
//dest:f, 1-7
//check tabl with foundation
//check within tableau
//check 

//now start with getAllPossibleMoves->validateMove(check whether move is valid or not)->executeMove->isWinnable(base case) if its winnablw within n moves return 1 else call getAllPossibleMoves
int isWinnable(int depth, Cards tabl[7][20], char foundation[4][2], Stock* stock, Waste* waste);

int validateMove(char src, char dest, Cards tabl[7][20], char foundation[4][2], Stock* stock, Waste* waste);//return 1 if its a valid move, 0 otherwise

Moves* executeMove(int depth, char src, char dest, Cards tabl[7][20], char foundation[4][2], Stock* stock, Waste* waste);

Moves* getAllPossibleMoves(int depth, Cards tabl[7][20], char foundation[4][2], Stock* stock, Waste* waste);

int foudRankCheck(char foud, char input);

int tablRankSuitCheck(char lRank, char lSuit, char rRank, char rSuit);


void printAll(Waste* wast, Stock* stok, char foundation[4][2], Cards tabl[7][20]){

	printf("Waste:\n");
	Waste* wptr=wast;
	while (wptr != NULL){
		Cards* temp = wptr->cards;
		printf("%c%c ", temp->rank, temp->suit);
		wptr = wptr->next;
	}
	printf("\nStock:\n");
	Stock* sptr=stok;
	while(sptr){
		printf("%c%c ", sptr->cards->rank, sptr->cards->suit);
		sptr=sptr->next;
	}
	printf("\nFoundation: \n");
	for(int i=0; i<4; i++){
		for(int j=0; j<2; j++){
			printf("%c", foundation[i][j]);
		}
		printf(" ");
	}
	printf("\nTableau: \n");
	for (int i = 0; i < 7; i ++){
		//printf("Column %d \n", 7-i);
		for (int j = 0; j < 20; j ++){
			printf("%c%c ", tabl[i][j].rank, tabl[i][j].suit);
		}
		printf("\n");
		
	}
	
}


int main(int argc, char* argv[]){
	FILE *fp;
	char *strmove=NULL;
	char *infile=NULL;
	int n=0;
	int check=0;
	int cache=0;
	int forced=0;
	int verbose=0;
	for(int i=1; i<argc; i++){
		if(strcmp(argv[i], "-m")==0){
			check=1;
			n=atoi(argv[++i]);
		}
		else if(strcmp(argv[i], "-c")==0){
			cache=1;
		}
		else if(strcmp(argv[i], "-f")==0){
			forced=1;
		}
		else if(strcmp(argv[i], "-v")==0){
			verbose=1;
		}
		else{
			if(!infile){
				infile=argv[i];
				fp=fopen(infile, "r");
			}
		}
	}

	if(!check) n=1000;

	if(!infile){
		fp=stdin;
		if(!fp){
			fprintf(stderr, "%s\n", "No such file or directory");
			return 2;
		}
	}

	m=n;
	int line=1;
	char foundation[4][2];
	Cards tabl[7][20];
	Stock* stok=NULL;
	Waste* wast = NULL;

	Moves* moveList=NULL;

	getAllData(fp, &line, &turn, &limit, foundation, tabl);
	wast=getW(fp, wast, &line);
	stok=getS(fp, stok, &line);
	moveList=getAllPossibleMoves(n, tabl, foundation, stok, wast);
	Moves* tp=moveList;
	while(tp){
		(tp->src, tp->dest, tabl, foundation, stok, wast);
		printf("%c %c\n", tp->src, tp->dest);
		tp=tp->next;
	}
	// printf("%d %d\n", turn, limit);
	// for(int i=0; i<4; i++){
	// 	for(int j=0; j<2; j++){
	// 		printf("%c", foundation[i][j]);
	// 	}
	// 	printf("\n");
	// }

	// for(int i=0; i<7; i++){
	// 	for(int j=0; j<20; j++){
	// 		printf("%c%c ", tabl[i][j].rank, tabl[i][j].suit);
	// 	}
	// 	printf("\n");
	// }

	// Waste* wptr=wast;
	// printf("%s\n", "WASTE:");
	// while(wptr){
	// 	printf("%c%c ", wptr->cards->rank, wptr->cards->suit);
	// 	wptr=wptr->next;
	// }
	// printf("\n");
	// Stock* sptr=stok;
	// printf("%s\n", "STOCK:");
	// while(sptr){
	// 	printf("%c%c ", sptr->cards->rank, sptr->cards->suit);
	// 	sptr=sptr->next;
	// }
	// printf("\n");
	// printf("%s %d\n", "N is:", n);
	// printf("%s %s\n", "filename is:", infile);
	// printf("%s %d\n", "cache is:", cache);
	// printf("%s %d\n", "forced is:", forced);
	// printf("%s %d\n", "verbose is:", verbose);
}

Moves* getAllPossibleMoves(int depth, Cards tabl[7][20], char foundation[4][2], Stock* stock, Waste* waste){
	char combo1[]="w1234567";
	char combo2[]="f1234567";
	Moves* possible=NULL;
	// Waste* wptr=waste;
	// printf("%s\n", "WASTE:");
	// while(wptr){
	// 	printf("%c%c ", wptr->cards->rank, wptr->cards->suit);
	// 	wptr=wptr->next;
	// }
	// printf("\n");
	// Stock* sptr=stock;
	// printf("%s\n", "STOCK:");
	// while(sptr){
	// 	printf("%c%c ", sptr->cards->rank, sptr->cards->suit);
	// 	sptr=sptr->next;
	// }
	// printf("\n");
	if(depth==0){
		fprintf(stderr, "%s\n", "not winnable");
		return possible;
	}
	// if(waste){

	// }
	if(!waste && validateMove('.', '0', tabl, foundation, stock, waste)){
		printf("%c %c\n", '.', '0');
		possible=executeMove(depth, '.', '0', tabl, foundation, stock, waste);
	} 
	// else if(!stock && validateMove('r', '0', tabl, foundation, stock, waste)){
	// 	printf("%c %c\n", 'r', '0');
	// 	possible=executeMove(depth, 'r', '0', tabl, foundation, stock, waste);
	// }
	else{
		int isMoved = 0;
		for(int i=0; i<strlen(combo1); i++){
			for(int j=0; j<strlen(combo2); j++){
				if (i != j){
					if(validateMove(combo1[i], combo2[j], tabl, foundation, stock, waste)){
						printf("%c %c\n", combo1[i], combo2[j]);
						isMoved = 1;
						possible=executeMove(depth, combo1[i], combo2[j], tabl, foundation, stock, waste);
					}	
				}
			}
		}
		if (!isMoved){
			if (stock && validateMove('.', '0', tabl, foundation, stock, waste)) {
				printf("%c %c\n", '.', '0');
				possible=executeMove(depth, '.', '0', tabl, foundation, stock, waste);
			}
		}
	}
	printAll(waste, stock, foundation, tabl);
	return possible;
}

int isWinnable(int depth, Cards tabl[7][20], char foundation[4][2], Stock* stock, Waste* waste){
	//base case
	//check tabl
	int check1=0;
	int iter=0;
	int count1=0;
	while(iter<7){
		if(tabl[iter][0].rank=='|'){
			count1+=1;
		}
		iter++;
	}
	if(count1==7){
		check1=1;
	}
	//check stock
	int check2=0;
	if(!stock){
		check2=1;
	}
	//check waste
	Waste* wptr=waste;
	int count2=0;
	while(wptr){
		count2++;
		wptr=wptr->next;
	}

	//check if winnable
	if(depth==0 && check1 && check2 && (count2==1 || count2==0)){
		return 1;
	}
	else{
		return 0;
	}
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
	if (lRank == '|' && rRank=='K'){
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

int validateMove(char src, char dest, Cards tabl[7][20], char foundation[4][2], Stock* stock, Waste* waste){
	Cards* tp;
	if (src == 'w'){								// src is from waste pile
		if (waste == NULL) {
			return 0;
			//return;
		} else {
			tp = waste->cards;
		}
		if (dest == 'f'){						// dest is foundation
			Cards* card = (Cards*) malloc (sizeof(card));
			for (int i = 0; i < 4; i ++){
				if (foundation[i][1] == tp-> suit){
					if (!foudRankCheck(foundation[i][0], tp->rank)){
						return 0;
					}
				}
			}
			return 1;
		}
		else {								// dest is number
			for (int i = 0; i < sizeof(tabl[0]); i ++){
				int des = dest - '0';
				if (tabl[7 - des][i].rank == '.'){
					if(tablRankSuitCheck(tabl[7 - des][i-1].rank, tabl[7 - des][i-1].suit, tp->rank, tp->suit) == 1){
						return 1;
					}
					else {
						return 0;
					}
				}
			}
		}	
	} 
	else if (src == 'r'){					// move all the waste card back to stock pile 
		if (waste == NULL || stock != NULL){
			return 0;
		}
		if (limit == -1){
			return 1;
		}	
	} 
	else if (src == '.'){						// take first stock card to waste pile
		if (stock != NULL){
			return 1;
		} 
		else if (stock == NULL){								// Stock pile has no card
			return 0;
		}
	} 
	else {	
		int sc = src - '0';							// src is number
		if (dest == 'f'){						// dest is foundation 
			Cards* card = (Cards*) malloc (sizeof(card));
			int i;

			if(tabl[7-sc][1].rank=='.' && tabl[7-sc][1].suit=='.'){
				return 0;
			}

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
						return 0;
					}
				}
			}
			return 0;
		} 
		else {								// dest is number
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
			if (tabl[7-des][tp].rank == '|' && tabl[7-sc][start].rank != 'K'){
				//printf("HERE---------------------%d%d\n", sc, des);
				return 0;
			}
			if (tabl[7-sc][start].rank == 'K' && tabl[7-sc][start-1].rank == '|'){
				//printf("hereeeeeee=======\n");
				return 0;
			}
			if (tablRankSuitCheck(tabl[7-des][add-1].rank, tabl[7-des][add-1].suit, tabl[7-sc][start].rank, tabl[7-sc][start].suit) == 1){
				//printf("HERE? not rught%c %c %d\n", tabl[7-sc][start].rank, tabl[7-sc][start-1].rank, add);
				return 1;
			} 
			else {
				return 0;
			}

		}
	}
}

Moves* executeMove(int depth, char src, char dest, Cards tabl[7][20], char foundation[4][2], Stock* stock, Waste* waste){
	depth--;
	Cards* tp;
	if (src == 'w'){								// src is from waste pile
		tp = waste->cards;		
		if (dest == 'f'){						// dest is foundation
			Cards* card = (Cards*) malloc (sizeof(card));
			for (int i = 0; i < 4; i ++){
				if (foundation[i][1] == tp-> suit){
					foundation[i][0] = tp->rank;
					Waste* newWast = (Waste*) malloc (sizeof(Waste));
					newWast=waste;
					waste = waste->next;
					free(newWast);
				}
			}
		} 
		else {								// dest is number
			for (int i = 0; i < sizeof(tabl[0]); i ++){
				int des = dest - '0';
				if (tabl[7 - des][i].rank == '.'){
					if (tablRankSuitCheck(tabl[7 - des][i-1].rank, tabl[7 - des][i-1].suit, tp->rank, tp->suit) == 1){
						tabl[7 - des][i].rank = tp->rank;
						tabl[7 - des][i].suit = tp->suit;
						Waste* newWast = (Waste*) malloc (sizeof(Waste));
						newWast=waste;
						waste = waste->next; 
						free(newWast);
						break;
					} 
				}
			}
		}	
	} 
	else if (src == 'r'){					// move all the waste card back to stock pile 
		if (limit == -1){
			while (waste != NULL){
				Stock* newStok = (Stock*) malloc (sizeof(Stock));
				Cards* card = (Cards*) malloc (sizeof(Cards));
				char rank=waste->cards->rank;
				char suit=waste->cards->suit;
				card->rank=rank;
				card->suit=suit;
				newStok->cards = card;
				newStok->next = stock; 
				stock = newStok; 
				Stock* newWast = waste->next; 
				waste = newWast;
			}
		}
		// } else if (limit > 0){
		// 	while (wast != NULL){
		// 		Stock* newStok = (Stock*) malloc (sizeof(Stock));
		// 		Cards* card = (Cards*) malloc (sizeof(Cards));
		// 		char rank=wast->cards->rank;
		// 		char suit=wast->cards->suit;
		// 		card->rank=rank;
		// 		card->suit=suit;
		// 		newStok->cards = card;
		// 		newStok->next = stok; 
		// 		stok = newStok; 
		// 		Stock* newWast = wast->next; 
		// 		wast = newWast;
		// 	}
		// 	limit --;
		// }		
	}
	else if (src == '.'){						// take first stock card to waste pile
		int turnTemp = turn;
		while (turnTemp){
			if (stock != NULL){
				Waste* newWast = (Waste*) malloc (sizeof(Waste));
				Cards* card = (Cards*) malloc (sizeof(Cards));
				char rank=stock->cards->rank;
				char suit=stock->cards->suit;
				card->rank=rank;
				card->suit=suit;
				newWast->cards = card;
				newWast->next = waste; 
				waste = newWast; 
				Stock* newStok = stock; 
				stock = stock->next;
				free(newStok);
			//printAll();
				turnTemp --;
			}
		}
	} 
	else {	
		int sc = src - '0';							// src is number
		if (dest == 'f'){						// dest is foundation 
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
		} 
		else {								// dest is number
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
			}
		}
	}
	if(isWinnable(depth, tabl, foundation, stock, waste)){
		// printf("%s\n", "winnable");
		Moves* possible = NULL;
		possible = getMoves(possible, src, dest);
		return possible;
	}
	else{
		printf("HERE?\n");
		getAllPossibleMoves(depth - 1, tabl, foundation, stock, waste);
	}
}