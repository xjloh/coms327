#include "list.hh"

list_builder::list_builder(){
	list=new node();
	size=0;
}

list_builder::~list_builder(){
	delete list;
	size=0;
}

void list_builder::add_to_front(int value){
	size++;
	node *newNode=new node();
	newNode->val=value;
	newNode->next=list;
	list=newNode;
}

void list_builder::add_to_back(int value){
	size++;
	node *newNode=new node();
	newNode->val=value;
	newNode->next=NULL;
	if(!list){
		list=newNode;
	}
	else{
		node *last=list;
		while(last->next){
			last=last->next;
		}
		last->next=newNode;
	}
}

void list_builder::reset(){
	list=new node();
	size=0;
}

int* list_builder::copy(){
	int* arr=(int*) malloc(size*sizeof(int));
	int iter=0;
	while(list){
		if(list->val!=0){
			arr[iter]=list->val;
			iter++;
		}
		list=list->next;
	}
	return arr;
}

fixed_list::fixed_list(list_builder LB){
	size=LB.getSize();
	A=(int*) malloc(size*sizeof(int));
	A=LB.copy();
}

fixed_list::~fixed_list(){
	A=(int*)realloc(A, 0*sizeof(int));
	size=0;
}

inline int fixed_list::operator[] (size_t sz){
	if(A==0) throw "Null array!";
	if(sz>=size) throw "Index out of bounds!";
	return A[sz];
}

std::ostream& operator<<(std::ostream &out, fixed_list &F){
	out<<"[";
	for(size_t i=0; i<F.Size(); i++){
		if(i) out << ", ";
		out<< F[i];
	}
	return out << "]";
}
