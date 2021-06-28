#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "formatChecker.h"

FILE *fpin;
FILE *fpout;

int main(int argc, char* argv[]){
	char *infile=NULL;
	char *outfile=NULL;
	char *strmove=NULL;

	int nmoves; //set N moves
	int infinity=0; //if N moves is ommited
	int x=0; //if -x is given
	int file=0;
	int cat=1;

	for(int i=1; i<argc; i++){
		if(strcmp(argv[i], "-m")==0){
			if(argv[i+1]!=NULL){
				strmove=argv[++i];
				char *ptr;
				nmoves=strtol(strmove, &ptr, 10);
				if(strcmp(ptr, "")!=0){
					infinity=1;
				}
				if(nmoves<0){
					fprintf(stderr, "%s\n", "Invalid N moves");
					exit(1);
				}
			}
			else{
				infinity=1;
			}
		}
		else if(strcmp(argv[i], "-o")==0){
			outfile=argv[++i];
			printf("%s\n", outfile);
			fpout=fopen(outfile, "w");
			if(fpout==NULL){
				fprintf(stderr, "%s\n", "File can't be opened!");
				exit(1);
			}
		}
		else if(strcmp())
		else{
			if(file==0 && atoi(argv[i])==0){
				fp=fopen(argv[i], "r");
				file=1;
				cat=0;
			}
		}
	}

	if(cat==1){
		fp=stdin;
	}

	if(fp==NULL){
		fprintf(stderr, "%s\n", "No such file or directory");
		return 2;
	}

	if(infinity==0){
		printf("%s %d\n","N is", nmoves);
	}
	else{
		printf("%s\n", "No N moves");
	}
	// printf("%s %s\n","filename", filename);
	// printf("%s %d\n","X is", x);
	// printf("%s %s\n", "outfile is", outfile);

	// char c;
	// for(;;){
	// 	c=fgetc(fp);
	// 	if(c==EOF){
	// 		break;
	// 	}
	// 	printf("%c", c);
	// }
	return 0;
}
