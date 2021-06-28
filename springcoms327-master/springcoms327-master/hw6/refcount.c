#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct items{
	int val;
	int reach;
	struct items *next;
}items;

typedef struct vars{
	int val;
	struct items *link;
}vars;

int size=0;

int getVar_or_Src(char *input){
	char varnum[10];
	int index=0;
	while(*input!='-'){
		if(isdigit(*input) || *input=='-'){
			varnum[index]=*input;
			index++;
		}
		input++;
	}
	varnum[index]='\0';
	int result=atoi(varnum);

	return result;
}

int getNodedest(char *input){
	char nodedest[10];
	int index=0;
	while(*input!='>'){
		input++;
	}
	while(*input!='\0'){
		if(isdigit(*input) || *input=='-'){
			nodedest[index]=*input;
			index++;
		}
		input++;
	}
	nodedest[index]='\0';
	int result=atoi(nodedest);
	return result;
}

void listOutput(items **nodes){
	for(int i=0; i<size; i++){
		if(nodes[i]->reach){
			printf("%s %d %s %d %s\n", "Node", nodes[i]->val, "has", nodes[i]->reach, "incoming pointers");
		}
	}
}

int main(){
	//9 variables
	vars **vNine=malloc(9*sizeof(struct items));
	int nVars=1;
	for(int i=0; i<9; i++){
		vNine[i]=malloc(sizeof(items));
		vNine[i]->link=NULL;
		vNine[i]->val=nVars;
		nVars++;
	}
	char line[50];

	//gets N number of nodes
	size=atoi(fgets(line, 50, stdin));
	items **nodes=malloc(size*sizeof(struct items));
	int n=1;
	for(int i=0; i<size; i++){
		nodes[i]=malloc(sizeof(items));
		nodes[i]->reach=0;
		nodes[i]->val=n;
		n++;
	}
	while(fgets(line, 50, stdin)!=NULL){
		if(line[0]=='V'){
			int varnum=getVar_or_Src(line);
			int nodedest=getNodedest(line);
			if(varnum<1 || varnum>9 || nodedest>size || nodedest<0){
				fprintf(stderr, "%s\n", "Out of range!");
				return 1;
			}
			if(nodedest==0){
				vNine[varnum-1]->link=NULL;
			}
			else{
				vNine[varnum-1]->link=nodes[nodedest-1];
			}
		}
		else if(line[0]=='L'){
			int nodesrc=getVar_or_Src(line);
			int nodedest=getNodedest(line);
			if(nodesrc>size || nodesrc<1 || nodedest>size || nodedest<0){
				fprintf(stderr, "%s\n", "Out of range!");
				return 1;
			}
			nodes[nodesrc-1]->next=nodes[nodedest-1];
		}
	}

	for(int j=0; j<9; j++){
		if(vNine[j]->link){
			items *cur=vNine[j]->link;
			cur->reach++;
		}
	}

	for(int i=0; i<size; i++){
		if(nodes[i]->next){
			items *cur=nodes[i]->next;
			cur->reach++;
		}
	}
	listOutput(nodes);
	return 0;
}

