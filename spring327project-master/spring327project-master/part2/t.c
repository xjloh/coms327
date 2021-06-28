#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct cards{
	char rank;
	char suit;
	struct cards* next;
};
typedef struct cards Cards;

struct waste{
	Cards* cards;
	struct waste* next;
};
typedef struct waste Waste;


int main (){
	printf("HI\n");
	Cards* newCard = NULL;
	Cards* newnew=malloc(sizeof(Cards));
	newnew->rank = 'A';
	newnew->suit = 's';
	newCard=newnew;

	//printf("%c%c\n", newCard->rank, newCard->suit);



	Cards* newnew2=malloc(sizeof(Cards));
	newnew2->rank = '2';
	newnew2->suit = 's';

	newnew2->next = newCard; 
	newCard = newnew2; 

	while (newCard != NULL){
		printf("%c%c\n", newCard->rank, newCard->suit);
		newCard = newCard->next;
	}	

}