#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <NTL/ZZ.h>
#include <unistd.h>
#include <sys/time.h>
#include <stack>

string zz_to_bin(ZZ n){
    string bt = "";
    int b;
    while(n>0){
        conv(b, modulo(n, ZZ(2)));
        bt = (char)(b+'0') + bt;
        n = n>>1; 
    }
    return bt;
}

ZZ bin_to_zz(string nb){
    unsigned t = nb.size()-1;
    ZZ n = ZZ(0), aux = ZZ(1);
    int temp = 0;
    for(unsigned i=0; i<=t; i++){
        if(nb[t-i] == '1'){
            aux = aux << temp;
            n += aux;
            temp = 1;
        }
        else temp++;
    }
    return n;
}

string sum(string a){
    int t = a.size()-1;
    string b = "";
    int aux;
    for(int i=0; i<t; i++){
        conv(aux, modulo(ZZ((a[i]-'0')+(a[i+1]-'0')),ZZ(2)));
        b += char(aux+'0');
    }
    return b;
}

string dez(string r){
    int t = r.size()/2;
    string a1="", b1="";
    int p, aux;

    conv(p, modulo(bin_to_zz(r), ZZ(r.size())));
    for(int i=1; i<t; i++) a1+=r[i];
    conv(aux, modulo(ZZ((r[p]-'0')+(r[0]-'0')),ZZ(2)));
    a1 += char(aux+'0');
    a1[0] = '1';

    conv(p, modulo(bin_to_zz(r), ZZ(r.size())));
    for(int i=t; i<r.size()-1; i++) b1+=r[i];
    conv(aux, modulo(ZZ((r[p]-'0')+(r[r.size()-1]-'0')),ZZ(2)));
    b1 = char(aux+'0') + b1;
    return (a1+b1);
}

void dezpl(string *rp){
    string r = *rp;
    *rp = dez(r);
}


ZZ _random(ZZ seed, int t){
    string se = zz_to_bin(seed);
    while(se.size()<t){
        se += sum(se);
        se = dez(se);
    }

    string se1 = "";
    for(int i=0; i<t; i++)  se1 += se[i];
    se1 = dez(se1);
    if(se1[t-1]=='0')  se1[t-1] = '1';
    return bin_to_zz(se1);
}
/*
bool miller(string nu, ZZ k, string *aw){
    ZZ n = bin_to_zz(nu);
    if(n<2) return false;
    ZZ r = n-1;
    int s;
    for(s=0; (r&1)==0; s++) r = r>>1;
    for(int i=0; i<k; i++){
        ZZ a = modulo(bin_to_zz(*aw), n-3) + 2;
        dezpl(aw);
        ZZ y = exp_mod(a, r, n);
        if((y!=ZZ(1)) and (y!=(n-1))){
            ZZ j=ZZ(1);
            while((j<=(s-1)) and (y!=(n-1))){
                y = exp_mod(y, ZZ(2), n);
                if(y==1) return false;
                j = j+1;
            }
            if(y!=(n-1))    return false;
        }
    }
    return true;
}

void pthr(string *s, ZZ k, string *aw){
    bool t = true;
    while(t==true){
        if(miller(*s, k, aw))  t = false;
        else    dezpl(s);
    }
}*/