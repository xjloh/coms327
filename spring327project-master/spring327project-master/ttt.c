#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// char tabl[15][100];
// char stok[5][100];
// char wast[5][100];
// char foud[5][20];
// int card[4][13];

// void getData(FILE* fp){
// 	int yes = 0; int i = 0;
// 	int yes2 = 0; int yes3 = 0;
// 	char temp[100];
// 	while(!feof(fp)){
// 		fgets(temp, 100, fp);
// 		if(strstr(temp, "FOUNDATIONS")){
// 			yes3 = 1;
// 		}

// 		if (strstr(temp, "TABLEAU")){
// 			fgets(temp, 100, fp);
// 			yes3 = 0;
// 			yes = 1;
// 			i = 0;
// 		}
// 		if (strstr(temp, "STOCK")){
// 			fgets(temp, 100, fp);
// 			yes = 0;
// 			yes2 = 1;
// 			i = 0;
// 		}
// 		if (strstr(temp, "MOVES")){
// 			yes2 = 0;
// 		}
// 		if (yes == 1){
// 			strcpy(tabl[i], temp);
// 			i++;
// 		}
// 		if (yes2 == 1){
// 			strcpy(stok[i], temp);
// 			strcpy(wast[i], temp);
// 			i++;

// 		}
// 		if (yes3 == 1){
// 			strcpy(foud[i], temp);
// 			//puts(foud[i]);
// 			i++;
// 		}

// 	}
// }

// int addFoud(int m, int n, char input[m][n]){
// 	char temp[100]; char * tp; char tpp[8];
// 	for (int i = 0; i < m; i ++){
// 		for (int j = 0; j < n; j ++){
// 			if (input[i][j] == '#'){
// 				input[i][j] = '\0';
// 			}
// 			if (input[i][j] ==  '\n'){
// 				input[i][j] = ' ';
// 			}
// 		}
// 		strcat(temp, input[i]);
// 	}
// 	tp = strtok(temp, ":");
// 	tp = strtok(NULL, ":");
// 	int j = 0;
// 	for(int i = 0; tp[i] != '\0'; i ++){
// 		if (tp[i] != ' ') {
// 			tpp[j] = tp[i];
// 			j++;
// 		}
// 	}
// 	tpp[8] = '\0';
// 	puts(tpp);
// 	// if (tpp[0] != '-'){
// 	// 	if (tpp[0] == 'T')  
// 	// }

// 	return 0;
// }

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

char suit[]="cdhs";
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

int main(){
	FILE* fp = fopen("input4.txt", "r");


	getFCards(fp);
	rewind(fp);
	getTCards(fp);
	rewind(fp);
	getSCards(fp);
	fcards[strlen(fcards)]='\0';
	tcards[strlen(tcards)]='\0';
	scards[strlen(scards)]='\0';
	// printf("%s\n", fcards);
	// printf("%s\n", tcards);
	// printf("%s\n", scards);
	findDup(fcards, tcards, scards);
	findMiss(fcards, tcards, scards);
	// getTableauCards(fp);
	// addFoud(5, 20, foud);
	fclose(fp); 
	return 0; 
}