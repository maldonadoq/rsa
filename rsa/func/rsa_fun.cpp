#include <stdio.h>
#include <NTL/ZZ.h>
#include <vector>
#include <sstream>
#include <string>
#include <math.h>

using namespace NTL;
using namespace std;


string zz_to_str(const ZZ &z) {
    stringstream buffer;
    buffer << z;
    return buffer.str();
}

string con_zero(string x, ZZ t){
    while(x.size() != t)
        x = "0" + x;                        
    return x;
}

void load_bq(FILE *a, string bq){
    int c;
    for(int i=0;bq[i]!='\0';i++){
        fprintf(a, "%c",bq[i]);
    }
}

void load_bq_(FILE *a, string sm, ZZ bq){
    string aux = sm;
    if(sm.size() != bq) aux = con_zero(aux, bq);
    for(int i=0;aux[i]!='\0';i++){
        fprintf(a, "%c",aux[i]);
    }
}

ZZ thread_random(int a){ 
    return RandomPrime_ZZ(a);
}
