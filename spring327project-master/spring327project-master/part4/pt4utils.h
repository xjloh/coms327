#include <stdlib.h>
#include <stdio.h>

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
	struct config* next;
};
typedef struct config Config;

int checkRank(char c);

void getAllData(FILE *fp, int *line, int *turn, int *limit, char foundation[4][2], Cards tabl[7][20]);

Waste* getW(FILE* fp, Waste* waste, int *line);

Stock* getS(FILE* fp, Stock* stock, int *line);

Waste* addWasteCard(Waste* waste, char rank, char suit);

Stock* addStockCard(Stock* stock, char rank, char suit);

Moves* getMoves(Moves *list, char src, char dest);

int foudRankCheck(char foud, char input);

int tablRankSuitCheck(char lRank, char lSuit, char rRank, char rSuit);

int validateMove(int limit, char src, char dest, Cards tabl[7][20], char foundation[4][2], Stock* stock, Waste* waste);

Config executeMove(char src, char dest, int limit, int turn, Config config);
