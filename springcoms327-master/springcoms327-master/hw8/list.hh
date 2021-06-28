#include <iostream>
using namespace std;

struct node{
	int val;
	struct node* next;
};

class list_builder{
	int size;
	node* list;
public:
	list_builder();
	~list_builder();
	void add_to_front(int value);
	void add_to_back(int value);
	void reset();
	int* copy();
	int getSize() { return size; }
	list_builder& operator=(const list_builder&) = delete;
};

class fixed_list{
	int *A;
	int size;
public:
	fixed_list(list_builder LB);
	~fixed_list();
	inline size_t Size() const { return size; }
	inline int operator[] (size_t sz);
	fixed_list& operator=(const fixed_list&) = delete;
};

std::ostream& operator<<(std::ostream &out, fixed_list &F);