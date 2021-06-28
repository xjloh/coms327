#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "formatChecker.h"

void getFoundation(FILE* fp, char foundation[4][2]){
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

Stock* getStock(FILE* fp, Stock* stock){
	char line[7];
	while(fgets(line, 7, fp)){
		if(strstr(line, "STOCK:")){
			break;
		}
	}
	char c;
	int read=0;
	for(;;){
		c=fgetc(fp);
		if(c==EOF) break;
		if(c=='M') break;
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
	}
	return stock;
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

Waste* getWaste(FILE* fp, Waste* waste){
	char line[7];
	while(fgets(line, 7, fp)){
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
		if(c=='|') break;

		if(c!=' ' && c!='\n' && c!='#'){
			char rank=c;
			c=fgetc(fp);
			char suit=c;
			waste=addWasteCard(waste, rank, suit);
		}
	}
	return waste;
}

void getTableau(FILE* fp, Cards tabl[7][20]){
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
	}
	// for(int i=0; i<7; i++){
	// 	for(int j=0; j<20; j++){
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
	char line[7];

	while(fgets(line, 7, fp)){
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

void humanFormat(int valid, int turn, int numMoves, char foundation[4][2], Waste* waste, Cards tabl[7][20]){
	if(valid==1){
		printf("%s %d %s\n", "Processed", numMoves, "moves, all valid");
	}
	printf("%s\n", "Foundations");
	for(int i=0; i<4; i++){
		for(int j=0; j<2; j++){
			printf("%c", foundation[i][j]);
		}
		printf(" ");
	}
	printf("\n");
	printf("%s", "Tableau");
	int col1=0; int col2=0; int col3=0;
	int col4=0; int col5=0; int col6=0;
	int col7=0;
	for(int i=0; i<20; i++){
		int count=0;
		//if all cards are missing
		for(int j=6; j>=0; j--){
			if(tabl[j][i].rank=='.'){
				count++;
			}
		}
		if(count==7){
				break;
		}
		//set non-hiding cards
		for(int j=6; j>=0; j--){
			if(col1 && col2 && col3 && col4 && col5 && col6 && col7){
				break;
			}
			if(tabl[j][i].rank=='|'){
				if(j==6) col1=1;
				if(j==5) col2=1; 
				if(j==4) col3=1;
				if(j==3) col4=1;
				if(j==2) col5=1;
				if(j==1) col6=1;
				if(j==0) col7=1;
			}
		}
		//print cards
		for(int j=6; j>=0; j--){
			if(col1 && j==6 && tabl[j][i].rank!='|'){
				printf("%c%c ", tabl[j][i].rank, tabl[j][i].suit);
			}
			else if(col2 && j==5 && tabl[j][i].rank!='|'){
				printf("%c%c ", tabl[j][i].rank, tabl[j][i].suit);
			}
			else if(col3 && j==4 && tabl[j][i].rank!='|'){
				printf("%c%c ", tabl[j][i].rank, tabl[j][i].suit);
			}
			else if(col4 && j==3 && tabl[j][i].rank!='|'){
				printf("%c%c ", tabl[j][i].rank, tabl[j][i].suit);
			}
			else if(col5 && j==2 && tabl[j][i].rank!='|'){
				printf("%c%c ", tabl[j][i].rank, tabl[j][i].suit);
			}
			else if(col6 && j==1 && tabl[j][i].rank!='|'){
				printf("%c%c ", tabl[j][i].rank, tabl[j][i].suit);
			}
			else if(col7 && j==0 && tabl[j][i].rank!='|'){
				printf("%c%c ", tabl[j][i].rank, tabl[j][i].suit);
			}
			else{
				if(i>0){
					printf("%s ", "##");
				}
			}
		}
		printf("\n");
	}

	printf("%s\n", "Waste top");
	if(!waste){
		printf("%s\n", "(empty)");
	}
	else{
		Waste *ptr=waste;
		int tmp=0;
		while(tmp<turn && ptr){
			printf("%c%c ", ptr->cards->rank, ptr->cards->suit);
			tmp++;
			ptr=ptr->next;
		}
		printf("\n");
	}
}

int getTurn(FILE *fp){
	int turn=0;
	char rules[7];
	while(fgets(rules, 7, fp)){
		if(strstr(rules, "RULES:")){
			break;
		}
	}
	//get turn
	char c;
	for(;;){
		c=fgetc(fp);
		// printf("%c", c);
		if(c=='#'){
			while(c!='\n') c=fgetc(fp);
		}
		if(c!='\n' && c!='#' && c!=' '){
			if(c=='1'){
				turn=1;
				break;
			}
			else if(c=='3'){
				turn=3;
				break;
			}
		}
	}
	// printf("%c\n", c);
	return turn;
}

int getLimit(FILE* fp){
	int limit;
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

void exchangeFormat(int valid, int numMoves, int turn, int limit, char foundation[4][2], Stock* stock, Cards tabl[7][20]){
	if(valid==1){
		printf("%s %d %s\n", "Processed", numMoves, "moves, all valid");
	}
	printf("%s\n", "RULES:");
	if(turn==1) printf("  %s\n", "turn 1");
	if(turn==3) printf("  %s\n", "turn 3");
	if(limit==-1) printf("  %s\n", "unlimited");
	if(limit>-1) printf("  %s %d\n", "limit", limit);
	printf("%s\n", "FOUNDATIONS:");
	for(int i=0; i<4; i++){
		printf("  ");
		for(int j=0; j<2; j++){
			printf("%c", foundation[i][j]);
		}
		printf("\n");
	}
	printf("%s\n", "TABLEAU:");
	for(int i=0; i<7; i++){
		printf("  ");
		for(int j=0; j<11; j++){
			if(tabl[i][j].rank=='.') continue;

			printf("%c%c ", tabl[i][j].rank, tabl[i][j].suit);
		}
		printf("\n");
	}
	printf("%s\n|  ", "STOCK:");
	while(stock){
		printf("%c%c ", stock->cards->rank, stock->cards->suit);
		stock=stock->next;
	}
	printf("\n");
	printf("%s\n", "MOVES:");
}

void outFileH_Format(FILE* fp, int valid, int turn, int numMoves, char foundation[4][2], Waste* waste, Cards tabl[7][20]){
	if(valid==1){
		printf("%s %d %s\n", "Processed", numMoves, "moves, all valid");
	}
	fprintf(fp, "%s\n", "Foundations");
	for(int i=0; i<4; i++){
		for(int j=0; j<2; j++){
			fprintf(fp, "%c", foundation[i][j]);
		}
		fprintf(fp, " ");
	}
	fprintf(fp, "\n");
	fprintf(fp, "%s", "Tableau");
	int col1=0; int col2=0; int col3=0;
	int col4=0; int col5=0; int col6=0;
	int col7=0;
	for(int i=0; i<20; i++){
		int count=0;
		//if all cards are missing
		for(int j=6; j>=0; j--){
			if(tabl[j][i].rank=='.'){
				count++;
			}
		}
		if(count==7){
				break;
		}
		//set non-hiding cards
		for(int j=6; j>=0; j--){
			if(col1 && col2 && col3 && col4 && col5 && col6 && col7){
				break;
			}
			if(tabl[j][i].rank=='|'){
				if(j==6) col1=1;
				if(j==5) col2=1; 
				if(j==4) col3=1;
				if(j==3) col4=1;
				if(j==2) col5=1;
				if(j==1) col6=1;
				if(j==0) col7=1;
			}
		}
		//print cards
		for(int j=6; j>=0; j--){
			if(col1 && j==6 && tabl[j][i].rank!='|'){
				fprintf(fp, "%c%c ", tabl[j][i].rank, tabl[j][i].suit);
			}
			else if(col2 && j==5 && tabl[j][i].rank!='|'){
				fprintf(fp, "%c%c ", tabl[j][i].rank, tabl[j][i].suit);
			}
			else if(col3 && j==4 && tabl[j][i].rank!='|'){
				fprintf(fp, "%c%c ", tabl[j][i].rank, tabl[j][i].suit);
			}
			else if(col4 && j==3 && tabl[j][i].rank!='|'){
				fprintf(fp, "%c%c ", tabl[j][i].rank, tabl[j][i].suit);
			}
			else if(col5 && j==2 && tabl[j][i].rank!='|'){
				fprintf(fp, "%c%c ", tabl[j][i].rank, tabl[j][i].suit);
			}
			else if(col6 && j==1 && tabl[j][i].rank!='|'){
				fprintf(fp, "%c%c ", tabl[j][i].rank, tabl[j][i].suit);
			}
			else if(col7 && j==0 && tabl[j][i].rank!='|'){
				fprintf(fp, "%c%c ", tabl[j][i].rank, tabl[j][i].suit);
			}
			else{
				if(i>0){
					fprintf(fp, "%s ", "##");
				}
			}
		}
		fprintf(fp, "\n");
	}

	fprintf(fp, "%s\n", "Waste top");
	if(!waste){
		fprintf(fp, "%s\n", "(empty)");
	}
	else{
		Waste *ptr=waste;
		int tmp=0;
		while(tmp<turn && ptr){
			fprintf(fp, "%c%c ", ptr->cards->rank, ptr->cards->suit);
			tmp++;
			ptr=ptr->next;
		}
		printf("\n");
	}
}

void outFileE_Format(FILE* fp, int valid, int numMoves, int turn, int limit, char foundation[4][2], Stock* stock, Cards tabl[7][20]){
	if(valid==1){
		printf("%s %d %s\n", "Processed", numMoves, "moves, all valid");
	}
	fprintf(fp, "%s\n", "RULES:");
	if(turn==1) fprintf(fp, "  %s\n", "turn 1");
	if(turn==3) fprintf(fp, "  %s\n", "turn 3");
	if(limit==-1) fprintf(fp, "  %s\n", "unlimited");
	if(limit>-1) fprintf(fp, "  %s %d\n", "limit", limit);
	fprintf(fp, "%s\n", "FOUNDATIONS:");
	for(int i=0; i<4; i++){
		fprintf(fp, "  ");
		for(int j=0; j<2; j++){
			fprintf(fp, "%c", foundation[i][j]);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "%s\n", "TABLEAU:");
	for(int i=0; i<7; i++){
		fprintf(fp, "  ");
		for(int j=0; j<11; j++){
			if(tabl[i][j].rank=='.') continue;

			fprintf(fp, "%c%c ", tabl[i][j].rank, tabl[i][j].suit);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "%s\n|  ", "STOCK:");
	while(stock){
		fprintf(fp, "%c%c ", stock->cards->rank, stock->cards->suit);
		stock=stock->next;
	}
	fprintf(fp, "\n");
	fprintf(fp, "%s\n", "MOVES:");
}

int line=1;

int stdinRule(FILE* fp){
	char c;
	for(;;){
		c=fgetc(fp);
		if(c=='#'){
			while(c!='\n') c=fgetc(fp);
		}
		if(c=='\n'){
			line++;
		}
		//check rules
		if(c=='R'){
			c=fgetc(fp);
			if(c=='U'){
				char rule[]="ULES:";
				int index1=0;
				while(rule[index1]!='\0'){
					if(c!=rule[index1]){
						fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'RULES:'");
						exit(1);
					}
					c=fgetc(fp);
					index1++;
				}
				return 1;
			}
		}
	}
	return 0;
}

int stdinTurn(FILE* fp){
	//get turn
	int turn;
	char c;
	for(;;){
		if(c=='\n') line++;

		if(c=='#'){
			while(c!='\n'){
				c=fgetc(fp);
			}
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
						fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'turn 1' or 'turn 3'");
						exit(1);
					}
					else if(c=='1'){ 
						turn=1; 
						return turn;
					}
					else if(c=='3'){ 
						turn=3; 
						return turn;
					}
					c=fgetc(fp);
					index++;
				}
			}
		}
		c=fgetc(fp);
	}
}

int stdinLimit(FILE* fp){
	char c;
	int limit=-2;
	for(;;){
		if(c=='\n') line++;

		if(c=='#'){
			while(c!='\n') c=fgetc(fp);
		}
		//check unlimited
		if(c=='u'){
			c=fgetc(fp);
			if(c=='n'){
				int index=0;
				char unlimit[]="nlimited";
				while(unlimit[index]!='\0'){
					if(c!=unlimit[index]){
						fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'unlimited or limit N'");	
						exit(1);
					}
					c=fgetc(fp);
					index++;
				}
				return limit=-1;
			}
		}//end of unlimited

		//check limit N
		if(c=='l'){
			c=fgetc(fp);
			if(c=='i'){
				int index=0;
				char lt[]="imit";
				while(lt[index]!='\0'){
					if(c!=lt[index]){
						fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'unlimited or limit N'");	
						exit(1);
					}
					c=fgetc(fp);
					index++;
				}
				if(c==' '){
					c=fgetc(fp);
					if(c=='-'){
						fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'limit N' where N is one of {0, 1, 2, . . . , 9}");	
						exit(1);
					}
					else if(c<='9' && c>='0'){
						char tmp[1];
						tmp[0]=c;
						limit=atoi(tmp);
						c=fgetc(fp);
						if(c<='9' && c>='0'){
							fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'limit N' where N is one of {0, 1, 2, . . . , 9}");	
							exit(1);
						}
						return limit;
					}
				}
				else{
					fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'unlimited or limit N'");	
					exit(1);
				}
			}
		}//end of limit
		c=fgetc(fp);
	}
	return limit;
}

void getFandT(FILE* fp, char foundation[4][2], Cards tabl[7][20]){
	char c;
	char suit[]="cdhs";
	int line=0;
	int found=0;
	int scount=0;
	int tbl=0;
	int i=0;
	for(;;){
		c=fgetc(fp);
		if(c==EOF) break;
		if(c=='\n') line++;

		if(c=='#'){
			while(c!='\n'){
				c=fgetc(fp);
			}
		}
		//check foundation
		if(c=='F' && found==0){
			c=fgetc(fp);
			if(c=='O'){
				int index=0;
				char foundation[]="OUNDATIONS:";
				while(foundation[index]!='\0'){
					if(c!=foundation[index]){
						fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'FOUNDATIONS:'");
						exit(1);
					}
					c=fgetc(fp);
					index++;
				}
				found=1;
			}
		}//end of foundation

		//check foundation cards
		if(found==1 && scount<4 && checkRank(c)){
			char r=c;
			char s=fgetc(fp);
			if(checkRank(r) && s==suit[scount]){
				// printf("%c%c\n", r, s);
				foundation[i][0]=r;
				foundation[i][1]=s;
				i++;
				scount++;
			}
			else{
				fprintf(stderr, "%s %d%s\n", "Error near line",line,": format error at foundations card");
				exit(1);
			}
		}//end of checking foundation cards

		//check for tableau
		if(c=='T' && tbl==0){
			c=fgetc(fp);
			if(c=='A'){
				if(found==0){
					fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'FOUNDATIONS:'");
					exit(1);
				}
				int index=0;
				char tblu[]="ABLEAU:";
				while(tblu[index]!='\0'){
					if(c!=tblu[index]){
						fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'TABLEAU:'");
						exit(1);
					}
					c=fgetc(fp);
					index++;
				}
				tbl=1;
				break;
			}
		}//end of tableau
	}

	int row=0;
	int col=0;
	int ck=0;
	for(;;){
		
		if(c=='S') break;
		if(c=='\n'){
			line++;
			
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
}

Waste* stdinWaste(FILE* fp, Waste* waste){
	char c;
	for(;;){
		if(c=='\n') line++;

		if(c=='#'){
			while(c!='\n') c=fgetc(fp);
		}
		//check for STOCK
		if(c=='S'){
			c=fgetc(fp);
			if(c=='T'){
				int index=0;
				char stok[]="TOCK:";
				while(stok[index]!='\0'){
					if(c!=stok[index]){
						fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'STOCK:'");
						exit(1);
					}
					c=fgetc(fp);
					index++;
				}
				int read=0;
				for(;;){
					if(c=='\n') line++;
					if(c=='M') return waste;
					if(c=='#'){
						while(c!='\n'){
							c=fgetc(fp);
						}
					}
					if(c=='|') return waste;

					if(c!=' ' && c!='\n' && c!='#'){
						char rank=c;
						c=fgetc(fp);
						char suit=c;
						waste=addWasteCard(waste, rank, suit);
					}
					c=fgetc(fp);
				}
				return waste;
			}
		}
		c=fgetc(fp);
	}//end
}

Stock* stdinStock(FILE* fp, Stock* stock){
	char c;
	int read=0;
	for(;;){
		if(c=='\n') line++;

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

Moves* stdinMoves(FILE* fp, Moves* moves){
	int mv=0;
	int mvf=0;
	char c;
	for(;;){
		if(c=='\n') line++;

		if(c=='#'){
			while(c!='\n') c=fgetc(fp);
		}
		if(c=='M'){
			c=fgetc(fp);
			if(c=='O'){
				int index=0;
				char mov[]="OVES:";
				while(mov[index]!='\0'){
					if(c!=mov[index]){
						fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'MOVES:'");
						exit(1);
					}
					c=fgetc(fp);
					index++;
				}
				mv=1;
				break;
			}
		}
		c=fgetc(fp);
	}//end of moves

	//check format for moves
	char src[]="w1234567";
	char dest[]="f1234567";
	char arrow[]="->";
	for(;;){
		// printf("%c", c);
		if(c==EOF) break;
		if(c=='\n') line++;
		if(c=='#'){
			while(c!='\n'){
				c=fgetc(fp);
			}
		}
		if(c!=' ' && c!='\n' && c!='#'){
			char sc;
			char dt;
			if(c=='.' || c=='r'){
				sc=c;
				dt='0';
				moves=getMoves(moves, sc, dt);
			}
			else{
				int s=0;
				int d=0;
				int i=0;
				while(src[i]!='\0'){
					if(c==src[i]){
						s=1;
						break;
					}
					i++;
				}
				if(!s){
					fprintf(stderr, "%s %d%s\n", "Error near line", line,": src error");
					exit(1);
				}
				sc=c;
				c=fgetc(fp);
				if(c!=arrow[0]){
					fprintf(stderr, "%s %d%s\n", "Error near line", line,": '->' error");
					exit(1);
				}
				c=fgetc(fp);
				if(c!=arrow[1]){
					fprintf(stderr, "%s %d%s\n", "Error near line", line,": '->' error");
					exit(1);
				}
				c=fgetc(fp);
				i=0;
				while(dest[i]!='\0'){
					if(c==dest[i]){
						d=1;
						break;
					}
					i++;
				}
				if(!d){
					fprintf(stderr, "%s %d%s\n", "Error near line", line,": dest error");
					exit(1);
				}
				dt=c;
				moves=getMoves(moves, sc, dt);
			}
		}
		c=fgetc(fp);
	}
	if(mv==0){
		fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'MOVES:'");
		exit(1);
	}
	return moves;
}