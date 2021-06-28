#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pt3utils.h"

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