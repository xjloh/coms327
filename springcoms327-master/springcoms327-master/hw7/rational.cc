#include <iostream>
#include <cstdlib>
#include "rational.hh"

using namespace std;

rational:: rational(){
	this->denom=1;
	this->numer=0;
}

rational::rational(long int val){
	this->numer=val;
	this->denom=1;
}

rational::rational(long int n, long int d){
	long int gcd=1;
	long int max=1;
	long int nnn=abs(n);
	long int ddd=abs(d);
	while(gcd<=nnn && gcd<=ddd){
		if(nnn%gcd==0 && ddd%gcd==0){
			if(gcd>max){
				max=gcd;
			}
		}
		gcd++;
	}

	long int nn=n/max;
	long int dd=d/max;

	if((n>0 && d<0) || (n<0 && d<0)){
		nn=-nn;
		dd=-dd;
	}
	this->numer=nn;
	this->denom=dd;
	
}

rational operator+ (const rational& val1, const rational& val2){
	long int n=val1.numer*val2.denom + val1.denom*val2.numer;
	long int d=val1.denom*val2.denom;
	rational res(n, d);
	return res;
}

rational operator- (const rational& val1, const rational& val2){
	long int n=val1.numer*val2.denom - val1.denom*val2.numer;
	long int d=val1.denom*val2.denom;
	rational res(n, d);
	return res;
}

rational operator* (const rational& val1, const rational& val2){
	long int n=val1.numer*val2.numer;
	long int d=val1.denom*val2.denom;
	rational res(n, d);
	return res;
}

rational operator/ (const rational& val1, const rational& val2){
	long int n=val1.numer*val2.denom;
	long int d=val1.denom*val2.numer;
	rational res(n, d);
	return res;
}

ostream& operator<< (ostream& str, const rational& val){
	if(val.numer==0){
		str<<val.numer;
	}
	else{
		str<<val.numer<<'/'<<val.denom;
	}
	return str;
}

// int main(){
// 	return 0;
// }
