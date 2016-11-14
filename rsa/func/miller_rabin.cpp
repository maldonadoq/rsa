#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <NTL/ZZ.h>
#include "func.cpp"

using namespace NTL;
using namespace std;

bool test_miller_rabin(ZZ n, ZZ k){
    if(n<2) return false;
    ZZ r = n-1;
    int s;
    for(s=0; (r&1)==0; s++)	r = r>>1;
    for(int i=0; i<k; i++){
    	ZZ a = modulo(ZZ(rand()), n-3) + 2;
    	ZZ y = exp_mod(a, r, n);
    	if((y!=ZZ(1)) and (y!=(n-1))){
    		ZZ j=ZZ(1);
    		while((j<=(s-1)) and (y!=(n-1))){
    			y = exp_mod(y, ZZ(2), n);
    			if(y==1) return false;
    			j = j+1;
    		}
    		if(y!=(n-1))	return false;
    	}
    }
    return true;
}

class asd{
private:
	int b;
public:
	int a;
	asd(int);
	int pro(int, int);
	void cam();
	void print();
};


asd::asd(int a){
	this->a = a;
	this->b = 2*a;
}

int asd::pro(int a, int b){
	return a*b;
}

void asd::cam(){
	this->a = 2*a;
	this->b = 2*b;
}

void asd::print(){
	cout << this->a << endl;
	cout << this->b << endl;
}

int main(int argc, char const *argv[]){
	asd A(4);
	A.print();
	A.cam();
	A.print();
	return 0;
}