#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pt4utils.h"



int checkRank(char c){
	if(c=='A' || c=='T' || c=='J' || c=='Q' || c=='K'){
		return 1;
	}
	else if(c>='2' && c<='9'){
		return 1;
	}
	else if(c=='_'){
		return 1;
	}
	return 0;
}

Waste* addWasteCard(Waste* waste, char rank, char suit){
	Waste* newWaste=malloc(sizeof(Waste));
	Cards* card=malloc(sizeof(Cards));
	card->rank=rank;
	card->suit=suit;
	newWaste->cards=card;
	newWaste->next=waste;
	waste=newWaste;
	return waste;
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

void getAllData(FILE *fp, int *line, int *turn, int *limit, char foundation[4][2], Cards tabl[7][20]){
	char c;
	int cp1=0;
	int cp2=0;
	int cp3=0;
	char suit[]="cdhs";
	int scount=0;
	int rk=0;
	for(;;){
		c=fgetc(fp);
		if(c==EOF) break;

		if(c=='#'){
			while(c!='\n') c=fgetc(fp);
		}

		if(c=='\n') *line+=1;

		if(c=='R' && cp1==0){
			c=fgetc(fp);
			if(c=='U'){
				char rules[]="ULES:";
				for(int i=0; i<strlen(rules); i++){
					if(c!=rules[i]){
						fprintf(stderr, "%s %d%s\n", "Error near line", *line, ": expecting 'RULES:'");
						exit(1);
					}
					c=fgetc(fp);
				}
				while(c!='F'){
					if(c=='\n') *line+=1;
					if(c=='#'){
						while(c!='\n') c=fgetc(fp);
					}
					if(c=='t'){
						c=fgetc(fp);
						if(c=='u'){
							c=fgetc(fp);
							int index=0;
							char turn1[]="rn 1";
							char turn3[]="rn 3";
							while(turn1[index]!='\0' && turn3[index]!='\0'){
								if(c!=turn1[index] && c!=turn3[index]){
									fprintf(stderr, "%s %d%s\n", "Error near line",*line,": expecting 'turn 1' or 'turn 3'");
									exit(1);
								}
								else if(c=='1'){ 
									*turn=1; 
									break;
								}
								else if(c=='3'){ 
									*turn=3; 
									break;
								}
								c=fgetc(fp);
								index++;
							}
						}
					}//end of turn
					if(c=='u'){
						c=fgetc(fp);
						if(c=='n'){
							int index=0;
							char unlimit[]="nlimited";
							while(unlimit[index]!='\0'){
								if(c!=unlimit[index]){
									fprintf(stderr, "%s %d%s\n", "Error near line",*line,": expecting 'unlimited or limit N'");	
									exit(1);
								}
								c=fgetc(fp);
								index++;
							}
							*limit=-1;
							break;
						}
					}//end of unlimited
					if(c=='l'){
						c=fgetc(fp);
						if(c=='i'){
							int index=0;
							char lt[]="imit";
							while(lt[index]!='\0'){
								if(c!=lt[index]){
									fprintf(stderr, "%s %d%s\n", "Error near line",*line,": expecting 'unlimited or limit N'");	
									exit(1);
								}
								c=fgetc(fp);
								index++;
							}
							if(c==' '){
								c=fgetc(fp);
								if(c=='-'){
									fprintf(stderr, "%s %d%s\n", "Error near line",*line,": expecting 'limit N' where N is one of {0, 1, 2, . . . , 9}");	
									exit(1);
								}
								else if(c<='9' && c>='0'){
									char tmp[1];
									tmp[0]=c;
									*limit=atoi(tmp);
									c=fgetc(fp);
									if(c<='9' && c>='0'){
										fprintf(stderr, "%s %d%s\n", "Error near line",*line,": expecting 'limit N' where N is one of {0, 1, 2, . . . , 9}");	
										exit(1);
									}
									break;
								}
								else{
									fprintf(stderr, "%s %d%s\n", "Error near line",*line,": expecting 'unlimited or limit N'");	
									exit(1);
								}
							}
						}
					}//end of limit
					c=fgetc(fp);
				}//end of while
				cp1=1;
			}
		}//end of rules

		if(c=='F' && cp2==0){
			c=fgetc(fp);
			if(c=='O'){
				int index=0;
				char foundation[]="OUNDATIONS:";
				while(foundation[index]!='\0'){
					if(c!=foundation[index]){
						fprintf(stderr, "%s %d%s\n", "Error near line", *line,": expecting 'FOUNDATIONS:'");
						exit(1);
					}
					c=fgetc(fp);
					index++;
				}
				cp2=1;
			}
		}//end of foundation

		//check foundation cards
		if(cp2==1 && scount<4 && checkRank(c)){
			char r=c;
			char s=fgetc(fp);
			if(checkRank(r) && s==suit[scount]){
				foundation[rk][0]=r;
				foundation[rk][1]=s;
				rk++;
				scount++;
			}
			else{
				fprintf(stderr, "%s %d%s\n", "Error near line", *line,": format error at foundations card");
				exit(1);
			}
		}//end of checking foundation cards

		//check for tableau
		if(c=='T' && cp3==0){
			c=fgetc(fp);
			if(c=='A'){
				if(cp2==0){
					fprintf(stderr, "%s %d%s\n", "Error near line", *line,": expecting 'FOUNDATIONS:'");
					exit(1);
				}
				int index=0;
				char tblu[]="ABLEAU:";
				while(tblu[index]!='\0'){
					if(c!=tblu[index]){
						fprintf(stderr, "%s %d%s\n", "Error near line", *line,": expecting 'TABLEAU:'");
						exit(1);
					}
					c=fgetc(fp);
					index++;
				}
				int row=0;
				int col=0;
				int ck=0;
				while(c!='S'){
					if(c=='\n'){
						*line+=1;
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
							// printf("\n");
						}
					}
					if(c=='#'){
						while(c!='\n') c=fgetc(fp);
					}

					if(c!='\n' && c!='#' && c!=' '){
						tabl[row][col].rank=c;
						if(c=='|'){
							tabl[row][col].suit=' ';
							col++;
						}
						else{
							c=fgetc(fp);
							tabl[row][col].suit=c;
							col++;
						}
					}
					c=fgetc(fp);
				}
				cp3=1;
				break;
			}
		}//end of tableau
	}
}

Waste* getW(FILE* fp, Waste* waste, int *line){
	char c;
	int read=0;
	for(;;){
		c=fgetc(fp);
		if(c=='\n') *line+=1;
		if(c=='#'){
			while(c!='\n') c=fgetc(fp);
		}

		//check for STOCK
		if(c=='T' && read==0){
			c=fgetc(fp);
			if(c=='O'){
				int index=0;
				char stok[]="OCK:";
				while(stok[index]!='\0'){
					if(c!=stok[index]){
						fprintf(stderr, "%s %d%s\n", "Error near line", *line,": expecting 'STOCK:'");
						exit(1);
					}
					c=fgetc(fp);
					index++;
				}
				read=1;
			}
		}
		if(c=='|' && read==1) break;

		if(c!=' ' && c!='\n' && c!='#' && read==1){
			char rank=c;
			c=fgetc(fp);
			char suit=c;
			waste=addWasteCard(waste, rank, suit);
		}
	}
	return waste;
}

Stock* getS(FILE* fp, Stock* stock, int *line){
	char c;
	int read=0;
	for(;;){
		if(c=='\n') *line+=1;
		if(c==EOF) break;
		if(c=='M') return stock;

		if(c=='#'){
			while(c!='\n'){
				c=fgetc(fp);
			}
		}
		if(c=='|' && !read){
			read=1;
		}

		if(read && c!=' ' && c!='\n' && c!='#' && c!='|'){
			char rank=c;
			c=fgetc(fp);
			char suit=c;
			stock=addStockCard(stock, rank, suit);
		}
		c=fgetc(fp);
	}
	return stock;
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

int foudRankCheck(char foud, char input){
	char num[] = "_A23456789TJQK";
	for (int i = 0; i < 14; i ++){
		if (foud == num[i]) {
			if (input == num[i+1]) return 1; 
			else return 0; 
		}
	}
	return 0;
}

int tablRankSuitCheck(char lRank, char lSuit, char rRank, char rSuit){
	char num[] = "_A23456789TJQK";
	int rk = 0;
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

	if (rk == 1){
		if((lSuit=='c' || lSuit=='s') && (rSuit=='d' || rSuit=='h')) return 1;
		if((lSuit=='d' || lSuit=='h') && (rSuit=='c' || rSuit=='s')) return 1;
	} 
	//printAll();
	//printf("%c%c, %c%c -- %d %d\n", lRank, lSuit, rRank, rSuit, tp1, tp2);
	return 0;
}

int validateMove(int limit, char src, char dest, Cards tabl[7][20], char foundation[4][2], Stock* stock, Waste* waste){
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
			for (int i = 0; i < sizeof(tabl[0]); i++){
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
				return 1;
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

Config executeMove(char src, char dest, int limit, int turn, Config config){
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
						free(newWast);
						break;
					} 
				}
			}
		}	
	} 
	else if (src == 'r'){					// move all the waste card back to stock pile 
		// if (limit == -1){
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
		// }
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
		while (turnTemp && config.stock){
			// if (config.stock != NULL){
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
			// }
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
	return config;
}



