#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FILE *fp;
char suit[]="cdhs";
char num[] = "A23456789TJQK";
char tabl[30][100];
char tabl2[30][100];
char stok[5][100];
char wast[5][100];

int getCovered(int m, int n, char input[m][n]){		// return value of covered card
	int count = 0;
	for (int i = 0; i < m; i++){
		for (int j = 0; j < n; j++){
			if(input[i][j] == '#' || input[i][j] == '|'){
				input[i][j] = '\0';
			}
		}
	}
	for (int k = 0; k < m; k++){
		for (int l = 0; input[k][l] != '\0'; l++){
			if (input[k][l] != ' ' && input[k][l+1] == ' '){
				count ++;
			}
		}
	}
	return count;
}

int getWaste(int m, int n, char input[m][n]){		// return value of waste card
	int count = 0;  char temp[10];
	for (int j = 0; j < n; j++){
		if (input[0][j] == 'S' && input[0][j+1] == 'T'){
			input[0][j] = ' ';input[0][j+1] = ' ';input[0][j+2] = ' ';input[0][j+3] = ' ';input[0][j+4] = ' ';input[0][j+5] = ' ';
		}

		if(input[0][j] == '|'){
			input[0][j] = '\0';
		}
	}
	//printf("%s\n", input[0]);
	for (int i = 0; input[0][i] != '\0'; i++){
		if (input[0][i] != ' ' && input[0][i+1] == ' '){
			count ++;
		}
	}
	return count;
}

int getStock(int m, int n, char input[m][n]){		// return value of stock cards
	int count = 0; 
	char temp[100]; 
	char *tp;

	for (int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			if(input[i][j] == '#' || input[i][j] == '\n'){
				input[i][j] = '\0';
			}
		}
		strcat(temp, input[i]);
	}
	temp[strlen(temp)]='\0';
	tp = strtok(temp, "|");
	tp = strtok(NULL, "|");
	
	if(tp==NULL){
		return 0;
	}

	for (int i = 1; tp[i] != '\0'; i++){
		if (tp[i] != ' ' && tp[i-1] == ' '){
			count ++;
		}
	}
	return count;
}

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

// check tableau part to make sure the pile is appropriate
void checkTabl(int m, int n, char input[m][n]){
	char temp[100]; char * tp; int columnct = 7;
	for (int i = 0; i < m; i ++){
		for (int j = 0; j < n; j ++){
			if (input[i][j] == '#'){
				input[i][j] = '\0';
			}
			if (input[i][j] ==  '\n'){
				input[i][j] = ' ';
			}
		}
		if (strstr(input[i], "|")){								// check for |, remove non need stuffs
			strcat(temp, input[i]);
			if (strtok(temp, "|") != NULL){
				tp = strtok(NULL, "|");
				int tmp = 99; int tmp2 = 99;
				for (int k = 1; tp[k] != '\0'; k ++){
					if (tp[k] == ' ' && tp[k - 1] == ' ') break;
					for (int x = 0; x < 13; x ++){
						if (tp[k] == num[x]){
							if (tmp == 99) tmp = x;
							else {
								if (tmp <= x) {
									//printf("Error card number on tableau pile column \n");	// if the card rank is wrong
									fprintf(stderr, "Error card number on tableau column %d\n", columnct);
									exit(1);	// if the card rank is wrong
								}
								else tmp = x;
							}
						}
					}

					for (int x = 0; x < 4; x ++){					// check from the suit array 
						if (tp[k] == suit[x]){
							//printf("%d\n", x);
							if (tmp2 == 99) tmp2 = x;
							else {
								if (tmp2 == 1 || tmp2 == 2){
									if (x == 1 || x == 2) {								
									//printf("Error suit on tableau pile column \n");		// if the card suit is wrong
									fprintf(stderr, "Error suit on tableau column %d\n", columnct);
									exit(1);
									}
								}
							
								if (tmp2 == 0 || tmp2 == 3){
									if (x == 0 || x == 3) {								
									//printf("Error suit on tableau pile column \n");		// if the card suit is wrong
									fprintf(stderr, "Error suit on tableau pile column %d\n", columnct);
									exit(1);
									}
								}
								tmp2 = x;
							}
						}
					}
					
				}
				columnct --;
				//printf("%s\n", tp);
			}
		}
	}
}

void checkFormat(FILE *fp){
	int rl=0;
	int trn=0;
	int unl=0;
	int found=0;
	int scount=0;
	int tbl=0;
	int stk=0;
	int mv=0;
	char c;
	char suit[]="cdhs";
	int line=0;
	for(;;){
		c=fgetc(fp);

		if(c==EOF){
			break;
		}
		if(c=='\n'){
			line++;
		}
		if(c=='#'){
			while(c!='\n'){
				c=fgetc(fp);
			}
		}

		//check rules
		if(c=='R' && rl==0){
			c=fgetc(fp);
			if(c=='U'){
				char rule[]="ULES:";
				int index1=0;
				while(rule[index1]!='\0'){
					if(c!=rule[index1]){
						fprintf(stderr, "%s %d%s\n", "Error near line",line-1,": expecting 'RULES:'");
						exit(1);
					}
					c=fgetc(fp);
					index1++;
				}
				rl=1;
			}
		}

		//check turn
		if(c=='t' && trn==0){
			c=fgetc(fp);
			if(c=='u'){
				if(rl==0){
					fprintf(stderr, "%s %d%s\n", "Error near line",line-1,": expecting 'RULES:'");
					exit(1);
				}
				c=fgetc(fp);
				int index=0;
				char turn1[]="rn 1";
				char turn3[]="rn 3";
				while(turn1[index]!='\0' && turn3[index]!='\0'){
					if(c!=turn1[index] && c!=turn3[index]){
						fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'turn 1' or 'turn 3'");
						exit(1);
					}
					c=fgetc(fp);
					index++;
				}
				trn=1;
			}
		}

		//check unlimited
		if(c=='u' && unl==0){
			c=fgetc(fp);
			if(c=='n'){
				if(rl==0){
					fprintf(stderr, "%s %d%s\n", "Error near line",line-1,": expecting 'RULES:'");
					exit(1);
				}
				else if(trn==0){
					fprintf(stderr, "%s %d%s\n", "Error near line",line-1,": expecting 'turn 1' or 'turn 3'");
					exit(1);
				}
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
				unl=1;
			}
		}//end of unlimited

		//check limit N
		if(c=='l' && unl==0){
			c=fgetc(fp);
			if(c=='i'){
				if(rl==0){
					// printf("%s\n", "lol");
					fprintf(stderr, "%s %d%s\n", "Error near line",line-1,": expecting 'RULES:'");
					exit(1);
				}
				else if(trn==0){
					fprintf(stderr, "%s %d%s\n", "Error near line",line-1,": expecting 'turn 1' or 'turn 3'");
					exit(1);
				}
				int index=0;
				char limit[]="imit";
				while(limit[index]!='\0'){
					if(c!=limit[index]){
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
						c=fgetc(fp);
						if(c<='9' && c>='0'){
							fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'limit N' where N is one of {0, 1, 2, . . . , 9}");	
							exit(1);
						}
					}
				}
				else{
					fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'unlimited or limit N'");	
					exit(1);
				}
				unl=1;
			}
		}//end of limit

		//check foundation
		if(c=='F' && found==0){
			c=fgetc(fp);
			if(c=='O'){
				if(rl==0){
					// printf("%s\n", "lol");
					fprintf(stderr, "%s %d%s\n", "Error near line",line-1,": expecting 'RULES:'");
					exit(1);
				}
				else if(unl==0){
					fprintf(stderr, "%s %d%s\n", "Error near line",line-1,": expecting 'unlimited or limit N'");	
					exit(1);
				}
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
			}
		}//end of tableau

		//check for stock
		if(c=='S' && stk==0){
			c=fgetc(fp);
			if(c=='T'){
				if(tbl==0){
					fprintf(stderr, "%s %d%s\n", "Error near line",line-1,": expecting 'TABLEAU:'");
					exit(1);
				}
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
				stk=1;
			}
		}//end of stock

		//check for moves
		if(c=='M' && mv==0){
			c=fgetc(fp);
			if(c=='O'){
				if(stk==0){
					fprintf(stderr, "%s %d%s\n", "Error near line",line-1,": expecting 'STOCK:'");
					exit(1);
				}
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
		}//end of moves
	}
	if(mv==0){
		fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'MOVES:'");
		exit(1);
	}
}

void getData(FILE* fp){
	int yes = 0; int i = 0;
	int yes2 = 0;
	char temp[100];
	while(!feof(fp)){
		fgets(temp, 100, fp);
		if (strstr(temp, "TABLEAU")){
			fgets(temp, 100, fp);
			yes = 1;
		}
		if (strstr(temp, "STOCK")){
			fgets(temp, 100, fp);
			yes = 0;
			yes2 = 1;
			i = 0;
		}
		if (strstr(temp, "MOVES")){
			yes2 = 0;
			yes = 0;
		}
		if (yes == 1){
			strcpy(tabl[i], temp);
			strcpy(tabl2[i], temp);
			i++;
		}
		if (yes2 == 1){
			strcpy(stok[i], temp);
			strcpy(wast[i], temp);
			i++;

		}

	}
}

void printValid(int crvd, int stok, int wast){
	printf("Input file is valid\n%d covered cards \n%d stock cards \n%d waste cards\n", crvd, stok, wast);
}

//get cards from foundation, tableau, stock
char fcards[8];
void getFCards(FILE* fp){
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
		if(c=='F'){
			char q=fgetc(fp);
			if(q=='O'){
				int index=0;
				int count=0;
				while(c!='T' || index<4){
					if(c=='#'){
						while(c!='\n'){
							c=fgetc(fp);
						}
					}
					if(checkRank(c)){
						char f=fgetc(fp);
						if(f==suit[index]){
							fcards[count]=c;
							count++;
							fcards[count]=f;
							count++;
							index++;
						}
					}
					c=fgetc(fp);
				}
			}
		}
	}
}

char tcards[104];
void getTCards(FILE *fp){
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
		if(c=='T'){
			char f=fgetc(fp);
			if(f=='A'){
				while(c!=':'){
					c=fgetc(fp);
				}
				c=fgetc(fp); //skip ':'
				int index=0;
				while(c!='S'){
					if(c=='#'){
						while(c!='\n'){
							c=fgetc(fp);
						}
					}
					if(c!='\n' && c!=' ' && c!='|'){
						tcards[index]=c;
						index++;
					}
					c=fgetc(fp);
				}
			}
		}
	}
}

char scards[104];
void getSCards(FILE *fp){
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
		if(c=='S'){
			char f=fgetc(fp);
			if(f=='T'){
				while(c!=':'){
					c=fgetc(fp);
				}
				c=fgetc(fp); //skip ':'
				int index=0;
				while(c!='M'){
					if(c=='#'){
						while(c!='\n'){
							c=fgetc(fp);
						}
					}
					if(c!='\n' && c!=' ' && c!='|'){
						scards[index]=c;
						index++;
					}
					c=fgetc(fp);
				}
			}
		}
	}
}

int isSuit(char c){
	if(c=='c' || c=='d' || c=='h' || c=='s'){
		return 1;
	}
	return 0;
}

int rankChecker(char fc, char tc){
	char rank[]="A23456789TJQK";
	int fnum=0;
	int tnum=0;
	if(fc=='_'){
		return 0;
	}
	for(int i=0; i<13; i++){
		if(rank[i]==fc){
			break;
		}
		fnum++;
	}
	for(int i=0; i<13; i++){
		if(rank[i]==tc){
			break;
		}
		tnum++;
	}
	if(fnum>tnum || fnum==tnum){
		return 1;
	}
	return 0;
}

char findNextCard(char fc){
	char rank[]="A23456789TJQKe";
	char n;
	if(fc=='_'){
		return 'e';
	}
	for(int i=0; i<14; i++){
		if(rank[i]==fc){
			n=rank[i+1];
		}
	}
	return n;
}


void findDup(char fcards[], char tcards[], char scards[]){
	//compare foundation with tableau
	int count=0;
	char dup[104];
	for(int i=strlen(tcards); i>0; i--){
		for(int j=strlen(fcards); j>0; j--){
			if(tcards[i]==fcards[j] && isSuit(tcards[i]) && isSuit(fcards[j])){//if have the same suit
				if(rankChecker(fcards[j-1], tcards[i-1])){//compare ranking
					// printf("%c%c\n", tcards[i-1],tcards[i]);
					dup[count]=tcards[i-1];//rank
					count++;
					dup[count]=tcards[i];//suit
					count++;
					dup[count]=' ';//space
					count++;
				}
			}
		}
	}
	//compare foundation with stock
	for(int i=strlen(scards); i>0; i--){
		for(int j=strlen(fcards); j>0; j--){
			if(scards[i]==fcards[j] && isSuit(scards[i]) && isSuit(fcards[j])){//if have the same suit
				if(rankChecker(fcards[j-1], scards[i-1])){//compare ranking
					// printf("%c%c\n", scards[i-1],scards[i]);
					dup[count]=scards[i-1];//rank
					count++;
					dup[count]=scards[i];//suit
					count++;
					dup[count]=' ';//space
					count++;
				}
			}
		}
	}
	dup[count]='\0';
	if(count>0){
		fprintf(stderr, "%s %s\n", "Duplicated cards:", dup);
		exit(1);
	}
}

void findMiss(char fcards[], char tcards[], char scards[]){
	char miss[104];
	int count=0;
	int found=0;
	//compare foundation with tableau and stock
	for(int i=strlen(fcards); i>0; i--){
		if(checkRank(fcards[i-1])){
			char next=findNextCard(fcards[i-1]);
			while(next!='e'){
				//search in tableau
				for(int j=strlen(tcards); j>0; j--){
					if(fcards[i]==tcards[j] && isSuit(fcards[i]) && isSuit(tcards[j])){
						if(tcards[j-1]==next){
							found=1;
						}
					}
				}
				//search in stock
				for(int k=strlen(scards); k>0; k--){
					if(fcards[i]==scards[k] && isSuit(fcards[i]) && isSuit(scards[k])){
						if(scards[k-1]==next){
							found=1;
						}
					}
				}
				if(found==0){
					miss[count]=next;
					count++;
					miss[count]=fcards[i];
					count++;
					miss[count]=' ';
					count++;
				}
				found=0;
				next=findNextCard(next);
			}
		}
	}
	miss[count]='\0';
	if(count>0){
		fprintf(stderr, "%s %s\n", "Missing cards:", miss);
		exit(1);
	}
}

int main(int argc, char const *argv[])
{
	if(argc == 2){
		fp=fopen(argv[1], "r");
		if(fp==NULL){
			fprintf(stderr, "%s\n", "File does not exist!");
			return 2;
		}
	}
	else{
		fp=stdin;
	}

	checkFormat(fp);
	rewind(fp);
	getData(fp);
	checkTabl(30, 100, tabl2);

	//check for dup and miss
	rewind(fp);
	getFCards(fp);
	rewind(fp);
	getTCards(fp);
	rewind(fp);
	getSCards(fp);
	fcards[strlen(fcards)]='\0';
	tcards[strlen(tcards)]='\0';
	scards[strlen(scards)]='\0';
	findDup(fcards, tcards, scards);
	findMiss(fcards, tcards, scards);

	int result = getCovered(30, 100, tabl);
	int res2 = getWaste(5, 100, wast);
	int res3 = getStock(5, 100, stok);

	printValid(result, res3, res2);
	fclose(fp);
	return 0;
}

