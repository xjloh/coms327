#include <stdio.h>
#include <stdlib.h>
//#include <termbox.h>
#include <string.h>
#include <time.h> 

int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 
               9, 10, 11, 12, 13, 14, 15, 
               16, 17, 18, 19, 20, 21, 22, 
               23, 24, 25, 26, 27, 28, 29, 
               30, 31, 32, 33, 34, 35, 36, 
               37, 38, 39, 40, 41, 42, 43, 
               44, 45, 46, 47, 48, 49, 50, 
               51}; 

char suit[] = "cdhs";
char num[] = "_A23456789TJQK"; 

unsigned long RNG_seed = 10000;

struct cards{
	char rank;
	char suit;
};
typedef struct cards Cards;

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

struct config{
	char foundation[4][2];
	Cards tabl[7][20];
	Waste *waste;
	Stock *stock;
	int winnable;
};
typedef struct config Config;              

void swap (int *a, int *b) 
{ 
    int temp = *a; 
    *a = *b; 
    *b = temp; 
} 

Cards intToCard(int a){
	int tp1 = a/4;
	int tp2 = a%4;
	Cards card;
	card.rank=num[tp1+1];
	card.suit=suit[tp2];
	return card;
}

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


// A utility function to print an array 
void printArray (int arr[], int n) 
{ 
    for (int i = 0; i < n; i++) 
        intToCard(arr[i]);
    printf("\n"); 
} 
  
void randomize ( int arr[], int n ) { 
    srand (RNG_seed); 
    for (int i = n-1; i > 0; i--)  { 
        int j = rand() % (i+1); 
        swap(&arr[i], &arr[j]); 
    } 
} 

Stock* addStockCard(Stock* stock, char rank, char suit){
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


Config getAllSeedData(Config config_in){
	Cards card3; 
		card3.rank = '.';
		card3.suit = '.';
	for (int i = 0; i < 7; i ++){
		for (int j = 0; j < 20; j ++){
			config_in.tabl[i][j] = card3;
		}
	}

	// filling up stock
	Stock* tempStock; 
	for (int i = 29; i < 52; i ++){
		Cards card = intToCard(a[i]);
		tempStock = addStockCard(tempStock, card.rank, card.suit);
	}
	config_in.stock = tempStock;


	// filling up foundation 
	for (int i = 0; i < 4; i ++){
		config_in.foundation[i][0] = suit[i];
		config_in.foundation[i][1] = '_';
	}

	// filling up tableau 
	for (int i = 0; i < 29; i ++){
		Cards card = intToCard(a[i]);
		Cards card2; 
		card2.rank = '|';
		card2.suit = ' ';
		if (i>=0 && i<6) config_in.tabl[0][i] = card;
		if (i == 6) {
			config_in.tabl[0][i] = card2;
			config_in.tabl[0][i+1] = card;
		}
		if (i>=7 && i<12) config_in.tabl[1][i-7] = card;
		if (i == 12) {
			config_in.tabl[1][i-7] = card2;
			config_in.tabl[1][i-6] = card;
		}
		if (i>=13 && i<17) config_in.tabl[2][i-13] = card;
		if (i == 17) {
			config_in.tabl[2][i-13] = card2;
			config_in.tabl[2][i-12] = card;
		}
		if (i>=18 && i<21) config_in.tabl[3][i-18] = card;
		if (i == 21) {
			config_in.tabl[3][i-18] = card2;
			config_in.tabl[3][i-17] = card;
		}
		if (i>=22 && i<24) config_in.tabl[4][i-22] = card;
		if (i == 24) {
			config_in.tabl[4][i-22] = card2;
			config_in.tabl[4][i-21] = card;
		}
		if (i == 26) config_in.tabl[5][0] = card;
		if (i == 27) {
			config_in.tabl[5][1] = card2;
			config_in.tabl[5][2] = card;
		}
		if (i == 28) {
			config_in.tabl[6][0] = card2;
			config_in.tabl[6][1] = card;
		}
	}
	return config_in;
}

int checkUncovered(Cards in[20]){
	int i = 0;
	while (in[i].rank != '|'){
		i++;
	}
	return i;
}

int main(){
	//printf("HELLO\n");
	Config config;
	randomize(a, 52);
	config = getAllSeedData(config);
	// printf("\nStock:\n");
	// Stock* sptr=config.stock;
	// while(sptr){
	// 	printf("%c%c ", sptr->cards->rank, sptr->cards->suit);
	// 	sptr=sptr->next;
	// }
	//printAll(config.waste, config.stock, config.foundation, config.tabl);
	printf("\nTableau: \n");
	for (int i = 0; i < 7; i ++){
		//printf("Column %d \n", 7-i);
		for (int j = 0; j < 20; j ++){
			printf("%c%c ", config.tabl[i][j].rank, config.tabl[i][j].suit);
		}
		printf("\n");
		
	}

	printf("%d\n", checkUncovered(config.tabl[2]));
  
    return 0; 
}