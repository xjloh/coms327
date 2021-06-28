#include <stdlib.h>
#include <stdio.h>

unsigned* build_factors(unsigned N);
void show_array(unsigned* A);
unsigned* build_common(unsigned* A, unsigned* B);

unsigned* build_factors(unsigned N){
	unsigned* result=malloc(256*sizeof(unsigned));
	int prime;
	unsigned index=0;
	unsigned i=2;
	while(i<=N){
		if(N%i==0){
			N=N/i;
			prime=1;
			for(int j=2; j<=i/2; j++){
				if(i%j==0){
					prime=0;
					break;
				}
			}
			if(prime){
				result[index]=i;
				index++;
			}
		}
		else{
			i++;
		}
	}
	result[index]=0;
	return result;
}

void show_array(unsigned* A){
	int iter=0;
	while(*(A+iter)!=0){
		printf("%u ", *(A+iter));
		iter++;
	}
	printf("\n");
}

int min(int a, int b){
	if(a>0 && b>0){
		if(a<b){
			return a;
		}
		else if(a>b){
			return b;
		}
		else if(a==b){
			return a;
		}
	}
	return 0;
}

int max(int a, int b){
	if(a<b){
		return b;
	}
	else if(a>b){
		return a;
	}
	else if(a==b){
		return a;
	}
	return 0;
}

unsigned* build_common(unsigned* A, unsigned* B){
	int sA=0;
	int sB=0;
	for(int i=0; A[i]!=0; i++){
		sA=A[i];
	}
	for(int j=0; B[j]!=0; j++){
		sB=B[j];
	}
	int mx=max(sA, sB);
	unsigned *result=malloc(256*sizeof(unsigned));
	unsigned pf=2;
	int iter=0;
	int stop=0;
	for(;;){
		if( stop>mx){
			break;
		}
		int ct1=0;
		int ct2=0;
		for(int i=0; A[i]!=0; i++){
			if(A[i]==pf){
				ct1++;
			}
		}
		for(int j=0; B[j]!=0; j++){
			if(B[j]==pf){
				ct2++;
			}
		}
		int c=min(ct1, ct2);
		if(c>0){
			int x=0;
			while(x<c){
				result[iter]=pf;
				iter++;
				x++;
			}
		}
		stop++;
		pf++;
	}
	result[iter]=0;
	return result;
}

int main(){
	unsigned m, n;
	printf("%s\n", "Enter two integers M, N:");
	scanf("%u, %u", &m, &n);
	unsigned *mf;
	mf=build_factors(m);
	unsigned *nf;
	nf=build_factors(n);
	printf("%d %s ", m, "has factors:");
	show_array(mf);
	printf("%d %s ", n, "has factors:");
	show_array(nf);
	unsigned* common;
	common=build_common(mf, nf);
	printf("%d %s %d %s ", m, "and", n, "have common factors:");
	show_array(common);
	return 0;
}
