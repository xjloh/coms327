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
int cache=0;
int forced=0;
int verbose=0;
int win=0;
int temp = 0;

int isWinnable(int depth, Cards tabl[7][20], char foundation[4][2], Stock* stock, Waste* waste);

int validateMove(char src, char dest, Cards tabl[7][20], char foundation[4][2], Stock* stock, Waste* waste);//return 1 if its a valid move, 0 otherwise

Config executeMove(char src, char dest, Config config);

Moves* getAllOriginMoves(Config config);

Moves* getNextMove(Config config);

int foudRankCheck(char foud, char input);

int tablRankSuitCheck(char lRank, char lSuit, char rRank, char rSuit);

Config findWinnable(int depth, Config config, char src, char dest, int print);

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

int show=0;
int iter=1;
int main(int argc, char* argv[]){
	FILE *fp;
	char *strmove=NULL;
	char *infile=NULL;
	int n=0;
	int check=0;
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
	Config config;
	config.stock=NULL;
	config.waste=NULL;
	getAllData(fp, &line, &turn, &limit, config.foundation, config.tabl);
	config.waste=getW(fp, config.waste, &line);
	config.stock=getS(fp, config.stock, &line);
	Moves* ori=getAllOriginMoves(config);
	Moves* mptr=ori;
	char wSrc;
	char wDest;
	if(verbose && cache==0){
		printf("%s\n", "Using DFS search");
	}
	while(mptr){
		wSrc=mptr->src;
		wDest=mptr->dest;
		findWinnable(n, config, mptr->src, mptr->dest, 0);
		if(win) break;
		mptr=mptr->next;
	}
	if(verbose && cache==0){
		char result[50]; 
		sprintf(result, "%d", show);
		// if(strlen(result)>9){

		// }if (strlen(result)>6){
		// 	printf("%s\n", "hi2");
		// }if (strlen(result)>3){
		// 	printf("%s\n", "hi");
		// }
		// printf("%s\n", result + strlen(result) - 3);

		for (int i = 0; i < strlen(result); i++){
			if (strlen(result)-4 == i){
				printf("%c,", result[i]);
				continue;
			}if (strlen(result)-7 == i){
				printf("%c,", result[i]);
				continue;
			}if (strlen(result)-10 == i){
				printf("%c,", result[i]);
				continue;
			}
			printf("%c", result[i]);
		}
		printf(" configurations checked.\n");

		//printf("%s %d %s\n", result, strlen(result), "configurations checked.");
	}
	if(win){
		printf("%s %d %s\n", "# Winnable in",n, "moves");
		findWinnable(n, config, wSrc, wDest, 1);
	}
	else{
		printf("%s %d %s\n", "# Game is not winnable within",n, "moves");
	}
}

Config findWinnable(int depth, Config config, char src, char dest, int print){
	show++;
	if(verbose && cache==0){
		while(iter*1000000<show){
			printf("%d,000,000 %s\n", iter, "configurations checked so far");
			iter+=1;
		}
	}
	Config cur=config;
	if(depth==0){
		if(isWinnable(depth, cur.tabl, cur.foundation, cur.stock, cur.waste)){
			win=1;
			return cur;
		}
		else{
			return cur;
		}
	}
	cur=executeMove(src, dest, cur);
	if(print){
		if(src=='.' || src=='r') printf("%c\n", src);
		else{
			printf("%c%s%c\n", src, "->", dest);
		}
	}
	Moves* ori=getAllOriginMoves(cur);
	depth--;
	Moves* mptr=ori;
	while(mptr){
		findWinnable(depth, cur, mptr->src, mptr->dest, print);
		if(win) return cur;
		mptr=mptr->next;
	}
}

int isWinnable(int depth, Cards tabl[7][20], char foundation[4][2], Stock* stock, Waste* waste){
	//base case
	//check tabl
	int check1=1;
	int iter=0;
	while(iter<7){
		if(tabl[iter][0].rank!='|'){
			check1=0;
		}
		iter++;
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
	if(depth==0 && check1 && check2 && count2==1){
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
	int rk = 0;
	// if (lRank == '|' && rRank=='K'){
	// 	return 1;
	// }

	for (int i = 0; i < 14; i ++){
		if (lRank == num[i]) {
			if (rRank == num[i-1]) {
				rk = 1;
			} 
		}
	}

	if (rk == 1){
		if((lSuit=='c' || lSuit=='s') && (rSuit=='d' || rSuit=='h')) return 1;
		if((lSuit=='d' || lSuit=='h') && (rSuit=='c' || rSuit=='s')) return 1;
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
		}
		Waste* wptr=waste;
		int count2=0;
		while(wptr){
			count2++;
			wptr=wptr->next;
		}
		if(count2==1 && !stock) return 0;

		tp = waste->cards;
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
					if(tablRankSuitCheck(tabl[7 - des][i-1].rank, tabl[7 - des][i-1].suit, tp->rank, tp->suit)){
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

			for (i = 0; i < 20; i ++){
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
			return 1;
		} 
		else {								// dest is number
			int des = dest - '0';
			int start = 0;
			int end = 0; 
			int add = 0; 
			int tp = 0;
			for (int i = 0; i < 20; i ++){
				if (tabl[7-sc][i].rank == '|' && start == 0) start = i + 1;
				if (tabl[7-sc][i].rank == '.' && end == 0) end = i; 
				if (tabl[7-des][i].rank == '.' && add == 0) add = i;
				if (tabl[7-des][i].rank == '|' && tp == 0) tp = i;
				//printf("%d\n", add);
			}
			//avoid moving row with just King
			if(tp==0 && add==1 && tabl[7-sc][start].rank=='K' && tabl[7-sc][start-1].rank=='|'){
				return 0;
			}
			//check if only moving specific row of cards from src to dest
			for(int i=start; i<end; i++){
				if(tablRankSuitCheck(tabl[7-des][add-1].rank, tabl[7-des][add-1].suit, tabl[7-sc][i].rank, tabl[7-sc][i].suit)){
					return 1;
				}
			}
			return 0;
		}
	}
}

Config executeMove(char src, char dest, Config config){
	Cards* tp;
	if (src == 'w'){								// src is from waste pile
		tp = config.waste->cards;		
		if (dest == 'f'){						// dest is foundation
			Cards* card = (Cards*) malloc (sizeof(card));
			for (int i = 0; i < 4; i ++){
				if (config.foundation[i][1] == tp-> suit){
					config.foundation[i][0] = tp->rank;
					Waste* newWast = (Waste*) malloc (sizeof(Waste));
					newWast=config.waste;
					config.waste = config.waste->next;
					// free(newWast);
				}
			}
		} 
		else {								// dest is number
			for (int i = 0; i < sizeof(config.tabl[0]); i ++){
				int des = dest - '0';
				if (config.tabl[7 - des][i].rank == '.'){
					if (tablRankSuitCheck(config.tabl[7 - des][i-1].rank, config.tabl[7 - des][i-1].suit, tp->rank, tp->suit)){
						config.tabl[7 - des][i].rank = tp->rank;
						config.tabl[7 - des][i].suit = tp->suit;
						Waste* newWast = (Waste*) malloc (sizeof(Waste));
						newWast=config.waste;
						config.waste = config.waste->next; 
						// free(newWast);
						break;
					} 
				}
			}
		}	
	} 
	else if (src == 'r'){					// move all the waste card back to stock pile 
		if (limit == -1){
			while (config.waste != NULL){
				Stock* newStok = (Stock*) malloc (sizeof(Stock));
				Cards* card = (Cards*) malloc (sizeof(Cards));
				char rank=config.waste->cards->rank;
				char suit=config.waste->cards->suit;
				card->rank=rank;
				card->suit=suit;
				newStok->cards = card;
				newStok->next = config.stock; 
				config.stock = newStok; 
				Stock* newWast = config.waste->next; 
				config.waste = newWast;
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
			if (config.stock != NULL){
				Waste* newWast = (Waste*) malloc (sizeof(Waste));
				Cards* card = (Cards*) malloc (sizeof(Cards));
				char rank=config.stock->cards->rank;
				char suit=config.stock->cards->suit;
				card->rank=rank;
				card->suit=suit;
				newWast->cards = card;
				newWast->next = config.waste; 
				config.waste = newWast;
				Stock* newStok = config.stock;
				config.stock = config.stock->next;
				// free(newStok);
				turnTemp --;
			}
		}
	} 
	else {	
		int sc = src - '0';							// src is number
		if (dest == 'f'){						// dest is foundation 
			Cards* card = (Cards*) malloc (sizeof(card));
			int i;
			for (i = 0; i < sizeof(config.tabl[0]); i ++){
				if (config.tabl[7-sc][i].rank == '.'){
					card->rank = config.tabl[7-sc][i-1].rank;
					card->suit = config.tabl[7-sc][i-1].suit;
					break;
				} 

			}
			for (int j = 0; j < 4; j ++){
				if (config.foundation[j][1] == card->suit){
					config.foundation[j][0] = card->rank;
					config.tabl[7-sc][i-1].rank = '.';
					config.tabl[7-sc][i-1].suit = '.';
					if (config.tabl[7-sc][i-2].rank == '|' && i > 2){
						config.tabl[7-sc][i-2].rank = config.tabl[7-sc][i-3].rank;
						config.tabl[7-sc][i-2].suit = config.tabl[7-sc][i-3].suit;
						config.tabl[7-sc][i-3].rank = '|';
						config.tabl[7-sc][i-3].suit = ' ';
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
			for (int i = 0; i < sizeof(config.tabl[0]); i ++){
				if (config.tabl[7-sc][i].rank == '|' && start == 0) start = i + 1;
				if (config.tabl[7-sc][i].rank == '.' && end == 0) end = i; 
				if (config.tabl[7-des][i].rank == '.' && add == 0) add = i;
				if (config.tabl[7-des][i].rank == '|' && tp == 0) tp = i;
				//printf("%d\n", add);
			}
			//iterate the card on src column and compare with cards on dest column
			//so that it moves more than 1 card from src to dest
			for(int i=start; i<=end; i++){
				if(tablRankSuitCheck(config.tabl[7-des][add-1].rank, config.tabl[7-des][add-1].suit, config.tabl[7-sc][i].rank, config.tabl[7-sc][i].suit)){
					int iter=i;
					while(config.tabl[7-sc][iter].rank!='.'){
						config.tabl[7-des][add].rank=config.tabl[7-sc][iter].rank;
						config.tabl[7-des][add].suit=config.tabl[7-sc][iter].suit;
						config.tabl[7-sc][iter].rank='.';
						config.tabl[7-sc][iter].suit='.';
						add++;
						iter++;
					}

					if (start >= 2 && config.tabl[7-sc][start].rank == '.'){
						char rank = config.tabl[7-sc][start - 2].rank; 
						char suit = config.tabl[7-sc][start - 2].suit; 
						//printf("HERE?%c%c\n", rank, suit);
						config.tabl[7-sc][start - 2].rank = '|';
						config.tabl[7-sc][start - 2].suit = ' ';
						config.tabl[7-sc][start - 1].rank = rank;
						config.tabl[7-sc][start - 1].suit = suit;
					}
					break;
				}
			}
		}
	}
	//printf("\n\n\n\n%c->%c     %d", src, dest, temp++);
	//printAll(config.waste, config.stock, config.foundation, config.tabl);
	return config;
}

Moves* getAllOriginMoves(Config config){
	Moves* possible=NULL;
	// int next=findNextMove(config);
	char tbl[]="7654321";
	//waste to tabl
	for(int i=0; i<strlen(tbl); i++){
		if(validateMove('w', tbl[i], config.tabl, config.foundation, config.stock, config.waste)){
			possible=getMoves(possible, 'w', tbl[i]);
		}
	}
	//waste to foundation
	if(validateMove('w', 'f', config.tabl, config.foundation, config.stock, config.waste)){
		possible=getMoves(possible, 'w', 'f');
	}
	//draw
	if(validateMove('.', '0', config.tabl, config.foundation, config.stock, config.waste)){
		possible=getMoves(possible, '.', '0');
	}
	//tableau to foundation
	for(int i=0; i<strlen(tbl); i++){
		if(validateMove(tbl[i], 'f', config.tabl, config.foundation, config.stock, config.waste)){
			possible=getMoves(possible, tbl[i], 'f');
		}
	}
	//tabl to tabl
	for(int i=0; i<strlen(tbl); i++){
		for(int j=0; j<strlen(tbl); j++){
			if(i != j){
				if(validateMove(tbl[i], tbl[j], config.tabl, config.foundation, config.stock, config.waste)){
					possible=getMoves(possible, tbl[i], tbl[j]);
				}
			}
		}
	}
	//reset
	if(validateMove('r', '0', config.tabl, config.foundation, config.stock, config.waste)){
		possible=getMoves(possible, 'r', '0');
	}
	return possible;
}