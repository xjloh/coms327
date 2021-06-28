#include <stdlib.h>
#include <stdio.h>
#include "formatChecker.h"

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

void checkFormat(FILE *fp){
	int rl=0;
	int trn=0;
	int unl=0;
	int found=0;
	int scount=0;
	int tbl=0;
	int stk=0;
	int mv=0;
	int mvf=0;
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
			}
		}//end of moves

		//check format for moves
		if(mv && !mvf){
			char src[]="w1234567";
			char dest[]="f1234567";
			char arrow[]="->";
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

				if(c!=' ' && c!='\n' && c!='#'){
					if(c=='.' || c=='r'){
						continue;
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

						c=fgetc(fp);
						if(c!=arrow[0]){
							fprintf(stderr, "%s %d%s\n", "Error near line", line,": '->'' error");
							exit(1);
						}
						c=fgetc(fp);
						if(c!=arrow[1]){
							fprintf(stderr, "%s %d%s\n", "Error near line", line,": '->'' error");
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
					}
				}
			}
			mvf=1;
			break;
		}
	}
	if(mv==0){
		fprintf(stderr, "%s %d%s\n", "Error near line",line,": expecting 'MOVES:'");
		exit(1);
	}
}
