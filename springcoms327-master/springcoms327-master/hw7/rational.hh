#include <iostream>
using namespace std;

class rational{
private:
	long int numer;
	long int denom;
public:
	rational();
	rational(long int val);
	rational(long int n, long int d);

	//overloading operators
	friend rational operator+ (const rational&, const rational&);
	friend rational operator- (const rational&, const rational&);
	friend rational operator* (const rational&, const rational&);
	friend rational operator/ (const rational&, const rational&);

	//overloading stream output
	friend ostream& operator<< (ostream&, const rational&);
};
