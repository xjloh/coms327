#include "intbox.hh"

singleton::singleton(int i){
	value=i;
}

singleton::~singleton(){

}

bool singleton::contains(int a){
	if(value==a) return true;
	return false;
}

void singleton::show(std::ostream &s){
	s << value;
}

interval::interval(int l, int h){
	if(l>h) throw "Error: Low is greater than high!";
	low=l;
	high=h;
}

interval::~interval(){

}

bool interval::contains(int a){
	if(a>=low && a<=high) return true;
	return false;
}

void interval::show(std::ostream &s){
	s << '[' << low << ", " << high << ']';
}

collection::collection(unsigned int i){
	size=i;
	arr[size]=new intbox();
}

collection::~collection(){
	free(arr);
}

void collection::set_item(unsigned i, intbox* item){
	arr[i]=item;
}

bool collection::contains(int a){
	for(int i=0; i<size; i++){
		if(arr[i]->contains(a)) return true;
	}
	return false;
}

void collection::show(std::ostream &s){
	s << '{';
	for(int i=0; i<size; i++){
		arr[i]->show(s);
		if(i+1<size) s << ", ";
	}
	s << '}';
}