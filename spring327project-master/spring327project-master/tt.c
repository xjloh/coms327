#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char tabl[15][100];
char tabl2[15][100];
char stok[5][100];
char wast[5][100];
char cardNum[13] = {'A', '2', '3', '4', '5' ,'6', '7', '8', '9', 'T', 'J', 'Q', 'K'};
char cardType[4] = {'c', 'd', 'h', 's'};

int checkTabl(int m, int n, char input[m][n]){
	char ** temp;
	for (int i = 0; i < m; i++){
		puts(input[i]);
		for (int j = 0; j < n; j++){
			if(input[i][j] == '#' || input[i][j] == '\n'){
				input[i][j] = '\0';
			}
		}
		//temp[i] = strtok(input[i], "|");
		//temp[i] = strtok(NULL, "|");
		
	}
	return 0;
}

int getCovered(int m, int n, char input[m][n]){		// return value of covered card
	int count = 0;
	for (int i = 1; i < m; i++){
		puts(input[i]);
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
		if(input[0][j] == '|'){
			input[0][j] = '\0';
		}
	}
	for (int i = 1; input[0][i] != '\0'; i++){
		if (input[0][i] != ' ' && input[0][i-1] == ' '){
			count ++;
		}
	}
	return count;
}

int getStock(int m, int n, char input[m][n]){		// return value of stock cards
	int count = 0; char temp[100]; char * tp;

	for (int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			if(input[i][j] == '#' || input[i][j] == '\n'){
				input[i][j] = '\0';
			}
		}
		strcat(temp, input[i]);
	}
	tp = strtok(temp, "|");
	tp = strtok(NULL, "|");

	for (int i = 1; tp[i] != '\0'; i++){
		if (tp[i] != ' ' && tp[i-1] == ' '){
			count ++;
		}
	}
	return count;
}

void printValid(int crvd, int stok, int wast){
	printf("Input file is valid\n%d covered cards \n%d stock cards \n%d waste cards\n", crvd, stok, wast);
}

void getData(FILE* fp){
	int yes = 0; int i = 0;
	int yes2 = 0;
	char temp[100];
	while(fgets(temp, 100, fp)){
		if (strstr(temp, "TABLEAU")){
			//fgets(temp, 100, fp);
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
		}
		if (yes == 1){
			strcpy(tabl[i], temp);
			//strcpy(tabl2[i], temp);
			i++;
		}
		if (yes2 == 1){
			strcpy(stok[i], temp);
			strcpy(wast[i], temp);
			i++;

		}

	}
}

int main(){
	FILE* fp = fopen("input1.txt", "r");


	getData(fp);

	int result = getCovered(15, 100, tabl);
	int res2 = getWaste(5, 100, wast);
	int res3 = getStock(5, 100, stok);

	//int res = checkTabl(15, 100, tabl2);

	printValid(result, res3, res2);

	
	fclose(fp); 
	return 0; 
}
