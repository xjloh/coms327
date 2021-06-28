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

int getTurn(FILE *fp);
int getLimit(FILE *fp);

Moves* addMoves(Moves* list, FILE *fp);

Moves* getMoves(Moves *list, char src, char dest);

void getTableau(FILE* fp, Cards tabl[7][20]);

Stock* getStock(FILE* fp, Stock* stock);

Waste* getWaste(FILE* fp, Waste* waste);

void getFoundation(FILE* fp, char foundation[4][2]);

void humanFormat(int valid, int turn, int numMoves, char foundation[4][2], Waste* waste, Cards tabl[7][20]);

void exchangeFormat(int valid, int numMoves, int turn, int limit, char foundation[4][2], Stock* stock, Cards tabl[7][20]);

void outFileH_Format(FILE* fp, int valid, int turn, int numMoves, char foundation[4][2], Waste* waste, Cards tabl[7][20]);

void outFileE_Format(FILE* fp, int valid, int numMoves, int turn, int limit, char foundation[4][2], Stock* stock, Cards tabl[7][20]);

void getFandT(FILE* fp, char foundation[4][2], Cards tabl[7][20]);

Moves* stdinMoves(FILE* fp, Moves* moves);

Stock* stdinStock(FILE* fp, Stock* stock);

Waste* stdinWaste(FILE* fp, Waste* waste);

int stdinLimit(FILE* fp);

int stdinTurn(FILE* fp);

int stdinRule(FILE* fp);