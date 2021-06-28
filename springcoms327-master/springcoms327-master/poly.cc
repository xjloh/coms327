#include "poly.hh"

poly::poly(double coe, unsigned exp){

	int counter=0;
	for(int i=0; i<size; i++){
		if(arr[i].expo==exp) counter++;
	}

	if(counter==0){
		size++;
		term *tmp=new term();
		if(coe==0){
			tmp->coef=0;
			tmp->expo=0;
		}
		else{
			tmp->coef=coe;
			tmp->expo=exp;
		}
		arr=(term*)realloc(arr, (size)*sizeof(term));
		arr[size-1]=*tmp;
	}
}

poly::poly(const poly& p){
	size=p.size;
	arr=(term*)realloc(arr, (size)*sizeof(term));
	for(int i=0; i<size; i++){
		arr[i]=p.arr[i];
	}
}

poly::~poly(){
	free(arr);
}

poly poly::operator+ (const poly &p){
	poly res(*this);

	// res+=p;
	return res;
}

std::ostream& operator<<(std::ostream &s, const poly &p){
	for(int i=0; i<p.size; i++){
		if(i>0 && p.arr[i].coef > 0) s << '+';

		if(p.arr[i].expo>0){
			if(p.arr[i].coef==1) s << 'x';
			else if(p.arr[i].coef==-1) s << "-x";
			else if(p.arr[i].coef==0) s << p.arr[i].coef;
			else{
				s << p.arr[i].coef << 'x';
			}

			if(p.arr[i].expo>1) s << '^' << p.arr[i].expo;
		}
		else if(p.arr[i].expo==0){
			s << p.arr[i].coef;
		}
	}

	return s;
}

int main(){
	poly p1(1.0, 5);
	poly p2(2.5, 1);
	poly p3(-0.2, 0);
	cout << p1 << p2 << p3 << '\n';
	poly p4=p1+p2+p3;
	cout << p4 << '\n';
	return 0;
}