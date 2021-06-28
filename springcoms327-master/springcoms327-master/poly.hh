#include <iostream>
using namespace std;

class poly{

    struct term{
        double coef;
        unsigned expo;
    };
public:
	term* arr=NULL;
    int size=0;
    
	poly(double coe, unsigned exp);
	poly(const poly&);
	~poly();

    poly operator= (const poly& p);
    poly operator+ (const poly& p);
    poly operator* (const poly& p);

    void evaluate(double x);
};

std::ostream& operator<<(std::ostream &s, const poly &p);
