#include <iostream>

class intbox{
public:
	intbox(){}
	~intbox(){}
	virtual bool contains(int a){}
	virtual void show(std::ostream &s){}
};

class singleton: public intbox{
int value;
public:
	singleton(int i);
	~singleton();
	virtual bool contains(int a);
	virtual void show(std::ostream &s);
};

class interval: public intbox{
	int low;
	int high;
public:
	interval(int l, int h);
	~interval();
	virtual bool contains(int a);
	virtual void show(std::ostream &s);
};

class collection: public intbox{
	unsigned int size;
	intbox* arr[];
public:
	collection(unsigned int i);
	~collection();
	void set_item(unsigned i, intbox* item);
	virtual bool contains(int a);
	virtual void show(std::ostream &s);
};