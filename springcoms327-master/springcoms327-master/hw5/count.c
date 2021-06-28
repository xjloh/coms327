#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

struct node{
	char *data;
	unsigned int count;
	struct node *next;
};

struct hashtable{
	unsigned int tsize;
	struct node **htable;
};

void init_table(struct hashtable* T, unsigned size);
void add_to_table(struct hashtable* T, char* str);
void show_table(struct hashtable* T);

int main(int argc, char* argv[]){
	FILE *fp;
	fp=stdin;

	if(!fp){
		fprintf(stderr, "%s\n", "Invalid file!");
		exit(1);
	}

	char *words=malloc(sizeof(char));
	char c;
	int clength=0;
	int sz=0;
	fscanf(fp, "%d", &sz);
	struct hashtable *table= (struct hashtable *)malloc(sizeof(struct hashtable));
	init_table(table, sz);

	for(;;){
		if(clength>32){
			fprintf(stderr, "%s\n", "More than 32 characters long!");
			exit(1);
		}
		c=fgetc(fp);
		if(c==EOF){
			break;
		}
		c=tolower(c);
		if((c>='a' && c<='z') || c=='\''){
			words=realloc(words, (clength+1)*sizeof(char));
			words[clength]=c;
			clength++;
		}
		else{
			if(!words[0]){
				continue;
			}
			words[clength]='\0';
			add_to_table(table, words);
			words=malloc(sizeof(char));
			clength=0;
		}
	}
	show_table(table);
	fclose(fp);
	return 0;
}

void init_table(struct hashtable* T, unsigned size){
	if(size<1){
		fprintf(stderr, "%s\n", "Invalid size!");
		exit(1);
	}

	T->htable= (struct node **) malloc(sizeof(struct node *)*size);
	int i;
	for(i=0; i<size; i++){
		T->htable[i]=0;
	}
	T->tsize=size;
}

unsigned long 
hash(char *str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
	    hash = ((hash << 5) + hash) + c; 

	return hash;
}

void add_to_table(struct hashtable* T, char* str){
	unsigned long h=hash(str);
	int index=h % T->tsize;
	struct node *cur=T->htable[index];
	int exist=0;
	while(cur!=NULL){
		if(strcmp(str, cur->data)==0){
			cur->count ++;
			exist=1;
			break;
		}
		cur=cur->next;
	}
	if(exist==0){
		struct node *new=malloc(sizeof(struct node));
		new->data=str;
		new->count=1;
		new->next=T->htable[index];
		T->htable[index]=new;
	}
}

void show_table(struct hashtable* T){
	struct node *cur=NULL;
	int i;
	for(i=0; i<T->tsize; i++){
		cur=T->htable[i];
		while(cur!=NULL){
			printf("%d %s\n", cur->count, cur->data);
			cur=cur->next;
		}
	}
}