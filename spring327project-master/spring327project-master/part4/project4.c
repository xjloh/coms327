#include <stdio.h>
#include <stdlib.h>
#include <termbox.h>
#include <string.h>
#include "pt4utils.h"

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

unsigned long seed;


void showInterface(char *filename);
void showConfig(Config config, int turn, int limit);
Config getAllSeedData(Config config_in);
void randomize ( int arr[], int n );
void setToBlank();
Cards intToCard(int a);
void swap (int *a, int *b);
int checkUncovered(Cards in[20]);
void saveFile(Config config_in, int turn, int limit);

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

int main(int argc, char *argv[]){
	FILE *fp;
	char *infile=NULL;
	int turn;
	int limit;
	int checker1=0; //check whether -1 or -3 is given
	int checker2=0; //check whether -l is given
	int inputType = 0; // 0 is default, 1 is -f, 2 is -s
	for(int i=1; i<argc; i++){
		if(strcmp(argv[i], "-f")==0){
			infile=argv[++i];
			fp=fopen(infile, "r");
			if(!fp){
				fprintf(stderr, "%s\n", "Invalid file!");
				exit(1);
			}
			inputType = 1;
		}
		else if(strcmp(argv[i], "-s")==0){
			seed=atoi(argv[++i]);
			if(seed<0){
				fprintf(stderr, "%s\n", "Seed should be positive integer!");
				exit(1);
			}
			inputType = 2;
		}
		else if(strcmp(argv[i], "-1")==0){
			checker1=1;
			turn=1;
		}
		else if(strcmp(argv[i], "-3")==0){
			checker1=1;
			turn=3;
		}
		else if(strcmp(argv[i], "-l")==0){
			checker2=1;
			limit=atoi(argv[++i]);
			if(limit<0){
				fprintf(stderr, "%s\n", "Limit N should be positive integer!");
				exit(1);
			}
		}
	}

	Config config;
	char title[100] = "";
	if (inputType == 1){
		strcat(title, "File: ");
		strcat(title, infile);
		int line=1;
		config.stock=NULL;
		config.waste=NULL;
		getAllData(fp, &line, &turn, &limit, config.foundation, config.tabl);
		config.waste=getW(fp, config.waste, &line);
		config.stock=getS(fp, config.stock, &line);
	}
	if (inputType == 2){
		if(checker1==0) turn=1;
		if(checker2==0) limit=-1;
		strcat(title, "Seed");
		randomize(a, 52);
		config.stock=NULL;
		config.waste=NULL;
		config = getAllSeedData(config);
	}

	Config restart=config;
	Config *undoConfig=NULL;

	tb_init();
	showInterface(title);
	showConfig(config, turn, limit);
	char src=' ';
	char dest=' ';
	for(;;){
		tb_present();
		struct tb_event event;
		tb_poll_event(&event);

		if (event.ch == 'Q') break;
		else if (event.ch == '.'){
			src='.';
			dest='0';
		}
		else if(event.ch=='>'){
			if(limit==0) break;
			if(limit!=-1) limit--;
			src='r';
			dest='0';
		}
		else if(event.ch=='w' || event.ch=='f' || (event.ch>='1' && event.ch<='7')){
			if(src==' ') src=event.ch;
			else{
				if(src!=' ' && dest==' ') dest=event.ch;
			}
		}
		else if(event.ch=='R'){
			config=restart;
		}
		else if(event.ch=='N'){
			turn=1;
			limit=-1;
			randomize(a, 52);
			Config newgame;
			newgame.stock=NULL;
			newgame.waste=NULL;
			newgame = getAllSeedData(newgame);
			config.stock=NULL;
			config.waste=NULL;
			config=newgame;
			restart=newgame;
		}
		else if(event.ch=='S'){
			saveFile(config, turn, limit); 
		}
		else if(event.ch=='U'){
			if(!undoConfig) continue;

			config=*undoConfig;
			undoConfig=undoConfig->next;
		}

		if(src!=' ' && dest!=' '){
			if(validateMove(limit, src, dest, config.tabl, config.foundation, config.stock, config.waste)){
				Config *curConfig=&config;
				config=executeMove(src, dest, limit, turn, config);
				src=' ';
				dest=' ';
				
				curConfig->next=undoConfig;
				undoConfig=curConfig;
			}
			else{
					// char *invalid="Invalid move. Please try again"
				break;
			}
		}
		setToBlank();
		showInterface(title);
		showConfig(config, turn, limit);
	}
	tb_shutdown();
	return 0;
}

void randomize ( int arr[], int n ) { 
	srand (seed); 
	for (int i = n-1; i > 0; i--)  { 
		int j = rand() % (i+1); 
		swap(&arr[i], &arr[j]); 
	} 
} 

void saveFile(Config config_in, int turn, int limit){
	FILE *fptr;
	fptr = fopen("output.txt","w");

	if(fptr == NULL)
	{
		printf("Error!");   
		exit(1);             
	}
	fprintf(fptr,"RULES: \n turn %d \n", turn);
	if (limit == -1) fprintf(fptr," unlimited");
	else fprintf(fptr," limit %d", limit);

	fprintf(fptr, "\n\nFOUNDATION:\n");
	fprintf(fptr, " %cc\n %cd\n %ch\n %cs\n",config_in.foundation[0][0], 
		config_in.foundation[1][0],config_in.foundation[2][0],config_in.foundation[3][0]);

	fprintf(fptr, "\nTABLEAU:\n");
	for (int i = 0; i < 7; i ++){
		for (int j = 0; config_in.tabl[i][j].rank != '.'; j++){
			fprintf(fptr, " %c%c", config_in.tabl[i][j].rank, config_in.tabl[i][j].suit);
		}
		fprintf(fptr, "\n");
	}

	fprintf(fptr, "\nSTOCK:\n");
	Waste* wptr = config_in.waste;
	while (wptr != NULL){
		fprintf(fptr, "%c%c ", wptr->cards->rank, wptr->cards->suit);
		wptr = wptr->next;
	} 
	fprintf(fptr, "| ");
	Stock* sptr = config_in.stock;
	while (sptr != NULL){
		fprintf(fptr, "%c%c ", sptr->cards->rank, sptr->cards->suit);
		sptr = sptr->next;
	} 
	fclose(fptr);
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
	for (int i = 28; i < 52; i ++){
		Cards card = intToCard(a[i]);
		tempStock = addStockCard(tempStock, card.rank, card.suit);
	}
	config_in.stock = tempStock;


    // filling up foundation 
	for (int i = 0; i < 4; i ++){
		config_in.foundation[i][1] = suit[i];
		config_in.foundation[i][0] = '_';
	}

    // filling up tableau 
	for (int i = 0; i < 28; i ++){
		Cards card = intToCard(a[i]);
		Cards card2; 
		card2.rank = '|';
		card2.suit = ' ';
        if (i>=0 && i<6) config_in.tabl[0][i] = card;		//7
        if (i == 6) {
        	config_in.tabl[0][i] = card2;
        	config_in.tabl[0][i+1] = card;
        }
        if (i>=7 && i<12) config_in.tabl[1][i-7] = card;	//6
        if (i == 12) {
        	config_in.tabl[1][i-7] = card2;
        	config_in.tabl[1][i-6] = card;
        }
        if (i>=13 && i<17) config_in.tabl[2][i-13] = card;	//5
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
        if (i == 25) config_in.tabl[5][0] = card;
        if (i == 26) {
        	config_in.tabl[5][1] = card2;
        	config_in.tabl[5][2] = card;
        }
        if (i == 27) {
        	config_in.tabl[6][0] = card2;
        	config_in.tabl[6][1] = card;
        }
    }
    return config_in;
}

void showInterface(char *filename){
	//game commands
	char *quit="Q: quit";
	char *restart="R: restart";
	char *newgame="N: new game";
	char *save="S: save to file";
	char *undo="U: undo move";
	char *draw=".: next card";
	char *reset=">: reset stock";
	char *text="To move, press the source and";
	char *text2="then destination (one of W, F,";
	char *text3="1, 2, 3, 4, 5, 6, 7)";

	struct tb_cell cell;
	int cx=0;
	int cy=0;
	//create the green box 34x10
	cell.bg = TB_GREEN;
	cell.fg=TB_GREEN;
	cell.ch= ' ';
	for(int i=0; i<10; i++){
		cy=i;
		if(i==0 || i+1==10){
			int j=0;
			while(j<=34){
				tb_put_cell(j, cy, &cell);
				j++;
			}
		}
		else{
			cx=0;
			tb_put_cell(cx, cy, &cell);
			cx=34;
			tb_put_cell(cx, cy, &cell);
		}
	}

	cy=0;
	cx=10;
	cell.bg = TB_GREEN;
	cell.fg = TB_WHITE;
	for(int i=0; i<strlen(filename) && i<34; i++){
		cell.ch=filename[i];
		tb_put_cell(cx, cy, &cell);
		cx++;
	}

	cx=2;
	cy++;
	cell.bg=TB_CYAN;
	cell.fg=TB_BLACK;
	for(int i = 0; i<strlen(quit); i++){
		if(quit[i]==':'){
			cell.bg=TB_DEFAULT;
			cell.fg=TB_WHITE;
		}
		cell.ch=quit[i];
		tb_put_cell(cx, cy, &cell);
		cx++;
	}

	cx=2;
	cy++;
	cell.bg=TB_CYAN;
	cell.fg=TB_BLACK;
	for(int i = 0; i<strlen(restart); i++){
		if(restart[i]==':'){
			cell.bg=TB_DEFAULT;
			cell.fg=TB_WHITE;
		}
		cell.ch=restart[i];
		tb_put_cell(cx, cy, &cell);
		cx++;
	}

	cx=2;
	cy++;
	cell.bg=TB_CYAN;
	cell.fg=TB_BLACK;
	for(int i = 0; i<strlen(newgame); i++){
		if(newgame[i]==':'){
			cell.bg=TB_DEFAULT;
			cell.fg=TB_WHITE;
		}
		cell.ch=newgame[i];
		tb_put_cell(cx, cy, &cell);
		cx++;
	}

	cx=2;
	cy+=2;
	cell.bg=TB_CYAN;
	cell.fg=TB_BLACK;
	for(int i = 0; i<strlen(draw); i++){
		if(draw[i]==':'){
			cell.bg=TB_DEFAULT;
			cell.fg=TB_WHITE;
		}
		cell.ch=draw[i];
		tb_put_cell(cx, cy, &cell);
		cx++;
	}

	cx=2;
	cy++;
	cell.bg=TB_DEFAULT;
	cell.fg=TB_WHITE;
	for(int i = 0; i<strlen(text); i++){
		cell.ch=text[i];
		tb_put_cell(cx, cy, &cell);
		cx++;
	}

	cx=2;
	cy++;
	for(int i = 0; i<strlen(text2); i++){
		cell.ch=text2[i];
		tb_put_cell(cx, cy, &cell);
		cx++;
	}

	cx=2;
	cy++;
	for(int i = 0; i<strlen(text3); i++){
		cell.ch=text3[i];
		tb_put_cell(cx, cy, &cell);
		cx++;
	}

	cx=18;
	cy=1;
	cell.bg=TB_CYAN;
	cell.fg=TB_BLACK;
	for(int i = 0; i<strlen(save); i++){
		if(save[i]==':'){
			cell.bg=TB_DEFAULT;
			cell.fg=TB_WHITE;
		}
		cell.ch=save[i];
		tb_put_cell(cx, cy, &cell);
		cx++;
	}

	cx=18;
	cy+=2;
	cell.bg=TB_CYAN;
	cell.fg=TB_BLACK;
	for(int i = 0; i<strlen(undo); i++){
		if(undo[i]==':'){
			cell.bg=TB_DEFAULT;
			cell.fg=TB_WHITE;
		}
		cell.ch=undo[i];
		tb_put_cell(cx, cy, &cell);
		cx++;
	}

	cx=18;
	cy+=2;
	cell.bg=TB_CYAN;
	cell.fg=TB_BLACK;
	for(int i = 0; i<strlen(reset); i++){
		if(reset[i]==':'){
			cell.bg=TB_DEFAULT;
			cell.fg=TB_WHITE;
		}
		cell.ch=reset[i];
		tb_put_cell(cx, cy, &cell);
		cx++;
	}
}

void showConfig(Config config, int turn, int limit){
	// setToBlank();
	//show foundation
	char *f="Foundations";
	int cy=11;
	int cx=10;
	struct tb_cell cell;
	cell.bg=TB_DEFAULT;
	cell.fg=TB_WHITE;
	for(int i=0; i<strlen(f); i++){
		cell.ch=f[i];
		tb_put_cell(cx, cy, &cell);
		cx++;
	}

	cy+=2;
	cx=4;
	int tpY=cy;
	int tpX=cx;
	int i=0;
	int check=1;
	int j=cy;
	int k=cx;
	while(i<4){
		cell.fg=TB_BLACK;
		if(config.foundation[i][0]=='_'){
			cell.bg=TB_GREEN;
			check=0;
		}
		else{
			if(config.foundation[i][1]=='d' || config.foundation[i][1]=='h'){
				cell.fg=TB_RED;
			}
			cell.bg=TB_WHITE;
		}

		for(int j=0; j<4; j++){
			for(int k=0; k<4; k++){
				if(check){
					if((j==0 && k==0) || j==3 && k==3){
						cell.ch=config.foundation[i][0];
					}
					else if((j==0 && k==1) || (j==3 && k==2)){
						cell.ch=config.foundation[i][1];
					}
					else{
						cell.ch=' ';
					}
				}
				else{
					cell.ch=config.foundation[i][1];
				}
				tb_put_cell(cx, cy, &cell);
				cx++;
			}
			cx=tpX;
			cy++;
		}
		check=1;
		tpX+=6;
		cx=tpX;
		cy=tpY;
		i++;
	}
	//show waste
	int wx=3;
	int wy=18;
	struct tb_cell wcell;
	wcell.bg=TB_DEFAULT;
	wcell.fg=TB_WHITE;
	char *waste="Waste";
	for(int i=0; i<strlen(waste); i++){
		wcell.ch=waste[i];
		tb_put_cell(wx, wy, &wcell);
		wx++;
	}

	wx+=2;
	char lim[50];
	if(limit==-1){
		strcpy(lim, "(Unlimited resets)");
	}
	else if(limit>=0 && limit<=9){
		if(limit==0) strcat(lim, "(0 resets remaining)");
		else if(limit==1) strcpy(lim, "(1 resets remaining)");
		else if(limit==2) strcpy(lim, "(2 resets remaining)");
		else if(limit==3) strcpy(lim, "(3 resets remaining)");
		else if(limit==4) strcpy(lim, "(4 resets remaining)");
		else if(limit==5) strcpy(lim, "(5 resets remaining)");
		else if(limit==6) strcpy(lim, "(6 resets remaining)");
		else if(limit==7) strcpy(lim, "(7 resets remaining)");
		else if(limit==8) strcpy(lim, "(8 resets remaining)");
		else if(limit==9) strcpy(lim, "(9 resets remaining)");
	}

	for(int i=0; i<strlen(lim); i++){
		wcell.ch=lim[i];
		tb_put_cell(wx, wy, &wcell);
		wx++;
	}

	// wx=4;
	wy=20;
	int tpWX=wx;
	int tpWY=wy;
	wy=tpWY;
	wx=8;
	tpWX=wx;
	Waste *wptr=config.waste;
	int num=0;
	if(config.waste){
		wcell.bg=TB_WHITE;
		while(wptr && num<turn){
			if(wptr->cards->suit=='c' || wptr->cards->suit=='s') wcell.fg=TB_BLACK;
			else if(wptr->cards->suit=='d' || wptr->cards->suit=='h') wcell.fg=TB_RED;
			for(int i=0; i<4; i++){
				for(int j=0; j<4; j++){
					if(i==0 && j==0) wcell.ch=wptr->cards->rank;
					else if(i==0 && j==1) wcell.ch=wptr->cards->suit;
					else if(i==3 && j==2 && num==0) wcell.ch=wptr->cards->suit;
					else if(i==3 && j==3 && num==0) wcell.ch=wptr->cards->rank;
					else{
						if(turn==3 && num>0){
							if(i==0 && (j==2 || j==3)) continue;
							else{
								wcell.ch=' ';
							}
						}
						else{
							wcell.ch=' ';
						}
					}
					tb_put_cell(wx, wy, &wcell);
					wx++;
				}
				wx=tpWX;
				wy++;
			}
			tpWX-=2;
			wx=tpWX;
			wy=tpWY;
			num++;
			wptr=wptr->next;
		}
	}

	//show turn
	int tx=6;
	int ty=24;
	struct tb_cell tcell;
	tcell.bg=TB_DEFAULT;
	tcell.fg=TB_WHITE;
	if(turn==1){
		tcell.ch='1';
	}
	else if(turn==3){
		tcell.ch='3';
	}
	tb_put_cell(tx, ty, &tcell);

	//show stock
	int sx=16;
	int tpSX=sx;
	int sy=20;
	int count=0;
	struct tb_cell scell;
	Stock *sptr=config.stock;
	while(sptr){
		count++;
		sptr=sptr->next;
	}
	scell.bg=TB_MAGENTA;
	scell.fg=TB_BLACK;
	scell.ch='#';
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			tb_put_cell(sx, sy, &scell);
			sx++;
		}
		sx=tpSX;
		sy++;
	}

	int sc=17;
	scell.bg=TB_DEFAULT;
	scell.fg=TB_WHITE;
	char val[100];
	sprintf(val, "%d", count);
	for(int i=0; i<strlen(val); i++){
		scell.ch=val[i];
		tb_put_cell(sc, sy, &scell);
		sc++;
	}
	scell.ch=' ';
	tb_put_cell(sc, sy, &scell);

	//show tableau
	cell.bg = TB_RED;
	cell.fg = TB_BLACK;
	cell.ch='1';
	tb_put_cell(39, 0, &cell);
	cell.ch='2';
	tb_put_cell(44, 0, &cell);
	cell.ch='3';
	tb_put_cell(49, 0, &cell);
	cell.ch='4';
	tb_put_cell(54, 0, &cell);
	cell.ch='5';
	tb_put_cell(59, 0, &cell);
	cell.ch='6';
	tb_put_cell(64, 0, &cell);
	cell.ch='7';
	tb_put_cell(69, 0, &cell);

	for (int i = 0; i < 7; i ++){
		int t = 38;
		int temp = checkUncovered(config.tabl[i]);
		cell.bg = TB_CYAN;
		cell.fg = TB_BLACK;
		cell.ch='#';
		// put all the uncovered cards first
		if (temp > 0){
			for (int j = 0; j < temp; j ++){
				tb_put_cell(t+(5*(6-i)), j+1, &cell);
				tb_put_cell(t+1+(5*(6-i)), j+1, &cell);
				tb_put_cell(t+2+(5*(6-i)), j+1, &cell);
				tb_put_cell(t+3+(5*(6-i)), j+1, &cell);
			}
		}

		// put the covered card if there is any
		for (int k = temp + 1; k < 20; k ++){
			if (config.tabl[i][k].rank != '.'){
				cell.bg = TB_WHITE;
				cell.fg = TB_BLACK;
				if (config.tabl[i][k].suit == 'd' || config.tabl[i][k].suit == 'h') cell.fg = TB_RED;

				// putting the rank and suit and the whole line
				cell.ch=config.tabl[i][k].rank;
				tb_put_cell(t+(5*(6-i)), k, &cell);
				cell.ch=config.tabl[i][k].suit;
				tb_put_cell(t+(5*(6-i))+1, k, &cell);
				cell.ch=' ';
				tb_put_cell(t+(5*(6-i))+2, k, &cell);
				tb_put_cell(t+(5*(6-i))+3, k, &cell);

				// make it 4x4 for the card
				if (config.tabl[i][k+1].rank == '.'){
					for (int a = 1; a < 4; a++){
						cell.ch=' ';
						tb_put_cell(t+(5*(6-i)), k+a, &cell);
						tb_put_cell(t+(5*(6-i))+1, k+a, &cell);
						if (a == 3){
							cell.ch=config.tabl[i][k].rank;
							tb_put_cell(t+(5*(6-i))+3, k+a, &cell);
							cell.ch=config.tabl[i][k].suit;
							tb_put_cell(t+(5*(6-i))+2, k+a, &cell);
						} else {
							tb_put_cell(t+(5*(6-i))+2, k+a, &cell);
							tb_put_cell(t+(5*(6-i))+3, k+a, &cell);
						}
					}
				}
			} else {
				break;
			}
		}
	}
}

void setToBlank(){
	struct tb_cell bcell;
	bcell.bg=TB_DEFAULT;
	bcell.fg=TB_DEFAULT;
	bcell.ch=' ';

	//set everything to blank
	for(int i=0; i<25; i++){
		for(int j=0; j<80; j++){
			tb_put_cell(j, i, &bcell);
		}
	}
	// //clear waste
	// int wx=4;
	// int wy=20;
	// int tpWX=wx;
	// int tpWY=wy;
	// bcell.bg=TB_DEFAULT;
	// bcell.ch=' ';
	// for(int i=20; i<24; i++){
	// 	for(int j=4; j<12; j++){
	// 		tb_put_cell(j, i, &bcell);
	// 	}
	// }

	// //clear tabl
	// for(int i=1; i<25; i++){
	// 	for(int j=38; j<80; j++){
	// 		tb_put_cell(j, i, &bcell);
	// 	}
	// }
}

Cards intToCard(int a){
	int tp1 = a/4;
	int tp2 = a%4;
	Cards card;
	card.rank=num[tp1+1];
	card.suit=suit[tp2];
	return card;
}

void swap (int *a, int *b) { 
	int temp = *a; 
	*a = *b; 
	*b = temp; 
} 

int checkUncovered(Cards in[20]){
	int i = 0;
	while (in[i].rank != '|'){
		i++;
	}
	return i;
}
