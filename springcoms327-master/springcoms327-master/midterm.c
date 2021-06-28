#include <stdio.h>
#include <stdlib.h>

struct node{
	char variable;
	char* expression;
	struct node* next;
};

//this is my solution for Q4
struct node* remove_var(struct node* list, char v){
	struct node* ptr1=NULL;
	struct node* ptr2=NULL;
	struct node* ptr3=NULL;
	struct node* result=NULL;
	ptr1=list;
	while(ptr1!=NULL){
		if(ptr1->variable==v){
			if(ptr1==list){
				list=ptr1->next;
				ptr2=list;
				ptr1->next=NULL;
			}
			else{
				ptr2=list;
				while(ptr2->next!=ptr1){
					ptr2=ptr2->next;
				}
				ptr2->next=ptr1->next;
				ptr1->next=NULL;
			}
			if(!result){
				result=ptr3=ptr1;
			}
			else{
				ptr3->next=ptr1;
				ptr3=ptr1;
			}
			ptr1=ptr2;
		}
		else{
			ptr1=ptr1->next;
		}
	}
	//now printing list here will show {y, 2+x}->{z, x}
	//printing result will show {x, 0}->{x, x+1}
	return result;
}

int main(){

	return 0;
}
