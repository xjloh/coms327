#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "utils.h"

struct cards{
	char rank;
	char suit;
};
typedef struct cards Cards;

// struct foundation{
// 	Cards* cards;
// 	struct foundation* next;
// };
// typedef struct foundation Foundation;

struct stock{
	Cards* cards;
	struct stock* next;
};
typedef struct stock Stock;

struct waste{
	Cards* cards;
	struct waste* next;
};
typedef struct stock Waste;

struct moves{
	char src;
	char dest;
	struct moves* next;
};
typedef struct moves Moves;

// void executeMove(Moves* list, Tableau* tabl);

// char tabl[30][100];
// char stok[100];
// char wast[100];
// char foud[5][20];
// int card[4][13];

Cards tabl[7][20];
char foundation[4][2];
Moves* moveList=NULL;
Stock* stok=NULL;
Waste* wast = NULL;
int limit = 0; 

char suit[] = "cdhs";
char num[] = "_A23456789TJQK";

void printAll(){

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

int getLimit(FILE* fp){
	int limit=0;
	char c1;
	int cont=0;
	for(;;){
		c1=fgetc(fp);
		if(c1=='#'){
			while(c1!='\n') c1=fgetc(fp);
		}
		if(c1!=' ' && c1!='\n' && c1!='#'){
			if(c1=='u'){
				limit=-1;
				return limit;
			}
			else if(c1=='l'){
				cont=1;
			}

			if(cont && c1>='0' && c1<='9'){
				char tmp[1];
				tmp[0]=c1;
				limit=atoi(tmp);
				return limit;
			}
		}
	}
	return limit;
}


void getFoundation(FILE* fp){
	char line[13];
	while(fgets(line, 13, fp)){
		if(strstr(line, "FOUNDATIONS:")){
			break;
		}
	}
	char c;
	for(;;){
		c=fgetc(fp);
		if(c==EOF) break;
		if(c=='#') while(c!='\n'){c=fgetc(fp);}

		if(c!=' ' && c!='\n' && c!='#'){//c is the rank
			char rank=c;
			char suit=fgetc(fp);
			if(c=='T'){
				if(suit=='A'){
					break;
				}
			}
			if(suit=='c'){ foundation[0][0]=rank; foundation[0][1]=suit;}

			if(suit=='d'){ foundation[1][0]=rank; foundation[1][1]=suit;} 	

			if(suit=='h'){ foundation[2][0]=rank; foundation[2][1]=suit;}

			if(suit=='s'){ foundation[3][0]=rank; foundation[3][1]=suit;}
		}
	}
}

Stock* addStockCard(Stock* stock, char rank, char suit, char divider){
	Stock* last =stock;
	Stock* newStock=malloc(sizeof(Stock));

	Cards* card=malloc(sizeof(Cards));
	card->rank=rank;
	card->suit=suit;
	newStock->cards=card;
	newStock->next=NULL;
	if(!stock){
		stock=newStock;
		return stock;
	}
	while(last->next){
		last=last->next;
	}
	last->next=newStock;
	return stock;
}

Stock* getStock(FILE* fp, Stock* stock){
	char line[100];
	while(fgets(line, 100, fp)){
		if(strstr(line, "STOCK:")){
			break;
		}
	}
	char c;
	for(;;){
		c=fgetc(fp);
		if(c==EOF) break;
		if(c=='M') break;
		if(c=='#'){
			while(c!='\n'){
				c=fgetc(fp);
			}
		}
		if(c!=' ' && c!='\n' && c!='#' && c!='|'){
			char rank=c;
			c=fgetc(fp);
			char suit=c;
			stock=addStockCard(stock, rank, suit, '0');
		}
	}
	return stock;
}

void getTableau(FILE* fp){
	char line[9];
	while(fgets(line, 9, fp)){
		if(strstr(line, "TABLEAU:")){
			break;
		}
	}
	char c;
	int row=0;
	int col=0;
	int ck=0;
	for(;;){
		c=fgetc(fp);
		if(c=='S') break;
		if(c=='\n'){
			if(!ck){
				ck=1;
			}
			else{
				while(col<20){
					tabl[row][col].rank='.';
					tabl[row][col].suit='.';
					col++;
				}
				row++;
				col=0;
			}
		}
		if(c=='#'){
			while(c!='\n'){
				c=fgetc(fp);
			}
		}
		if(c!='\n' && c!='#' && c!=' '){
			// printf("%d %d| ", row, col);
			tabl[row][col].rank=c;
			c=fgetc(fp);
			tabl[row][col].suit=c;
			// printf("%c%c ", tabl[row][col].rank, tabl[row][col].suit);
			col++;
		}
	}
	// for(int i=0; i<7; i++){
	// 	for(int j=0; j<11; j++){
	// 		if(tabl[i][j].rank){
	// 			printf("%c%c ", tabl[i][j].rank, tabl[i][j].suit);
	// 		}
	// 	}
	// 	printf("\n");
	// }
}

Moves* getMoves(Moves *list, char src, char dest){
	Moves *last=list;

	Moves *newMove=malloc(sizeof(Moves));
	newMove->src=src;
	newMove->dest=dest;
	newMove->next=NULL;
	if(!list){
		list=newMove;
		return list;
	}
	while(last->next){
		last=last->next;
	}
	last->next=newMove;
	return list;
}

Moves* addMoves(Moves* list, FILE *fp){
	char source[]="w1234567";
	char destination[]="f1234567";
	char line[100];

	while(fgets(line, 100, fp)){
		if(strstr(line, "MOVES:")){
			break;
		}
	}
	char c;
	for(;;){
		c=fgetc(fp);
		if(c==EOF){
			break;
		}
		if(c=='#'){
			while(c!='\n'){
				c=fgetc(fp);
			}
		}
		if(c!=' ' && c!='\n' && c!='#'){
			if(c=='.' || c=='r'){
				char src=c;
				char dest='0';
				list=getMoves(list, src, dest);
				// printf("%c %c\n", src, dest);
			}
			else{
				int i=0;
				char src;
				char dest;
				while(source[i]!='\0'){
					if(c==source[i]){
						src=c;
						break;
					}
					i++;
				}
				c=fgetc(fp);
				c=fgetc(fp);
				c=fgetc(fp);
				i=0;
				while(destination[i]!='\0'){
					if(c==destination[i]){
						dest=c;
						break;
					}
					i++;
				}
				list=getMoves(list, src, dest);
				// printf("%c %c\n", src, dest);
			}
		}
	}
	return list;
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
			fprintf(stderr, "Move %d is illegal: %c->%c\n", count, src, dest);
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
						fprintf(stderr, "Move %d is illegal: %c->%c\n", count, src, dest);
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
						printf("%d\n", des);
						printAll();
						break;
					} else {
						fprintf(stderr, "Move %d is illegal: %c->%c\n", count, src, dest);
						exit(1);
					}
				}
			}
		}	

	} else if (src == 'r'){						// move all the waste card back to stock pile 
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
			//printAll();

		}
	} else if (src == '.'){						// take first stock card to waste pile
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
			return;
		} else {								// Stock pile has no card
			printf("Stok empty\n"); // change to error
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
						fprintf(stderr, "Move %d is illegal: %c->%c\n", count, src, dest);
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
				fprintf(stderr, "Move %d is illegal: %c->%c\n", count, src, dest);
				exit(1);
			}

		}
		//printAll();
	}
	//printAll(tabl, stok, wast, foud);
}



/*
#
# Start of a game
#
RULES:
  turn 1
  unlimited
FOUNDATIONS:
  _c
  _d
  _h
  _s
TABLEAU:
  3h 7h 8h Th Jc Ad | Qs
  7c 6c 6s 7s 5h | Tc
  7d As Ks 8c | Js
  Kd 9s 6h | 3c
  Td Qc | 9d
  2h | Qh
  | 4d
STOCK:
| Jd 3d Ac Kc 4h 2s Qd 6d Kh 5s 4s Ah Ts 4c 5d 
  2d 9c 9h 2c Jh 8s 3s 8d 5c
MOVES:
#
# Lots of moves
#
  4->1  5->2  5->3  2->5  2->1  5->2  5->f  5->3  .     w->7
  6->7  .     .     w->f  .     .     .     .     .     .
  .     w->4  1->4  w->1  7->1  7->f  3->1  .     w->6  .
  w->f  .     .     .     .     w->f  .     .     .     w->f
  .     w->3  .     .     .     .     r     .     w->f  .  
  w->5  .     .     w->f  4->f  4->f  .     w->5  7->5  7->5
  .     .     w->3  .     w->f  .     .     w->5  7->5  .
  w->3  .     w->3  7->3  7->f  .     w->f  6->f  .     .
  w->f  r     .     w->f  6->f  6->5  6->3  6->1  4->6


*/


int main(){
	FILE *fp;
	fp=stdin;
	getFoundation(fp);
	rewind(fp);
	moveList=addMoves(moveList, fp);
	Moves* cur=moveList;
	rewind(fp);
	getTableau(fp);
	rewind(fp);
	stok=getStock(fp, stok);
	rewind(fp);
	limit = getLimit(fp);

	printAll();
	//Moves* cur=moveList;
	int count=0;
	while(cur){
		count++;
		printf("\n=====================================================\n");
		printf("%c->%c Move: %d\n", cur->src, cur->dest, count);
		//newMove('.',' ');
		newMove(cur->src, cur->dest, count);

		//printAll();
		cur=cur->next;
	}
	printAll();

	fclose(fp);
	return 0; 
}