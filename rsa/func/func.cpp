#include <stdio.h>
#include <vector>
#include <sstream>
#include <string>
#include <math.h>
#include <NTL/ZZ.h>

using namespace NTL;
using namespace std;


int modulo_int(int a,int n){
    int q = a/n;
    int r = a-(n*q);
    if(r<0) return r+n;
    return r;
}

ZZ modulo(ZZ a,ZZ n){
    ZZ q = a/n;
    ZZ r = a-(n*q);
    if(r<0) return r+n;
    return r;
}

ZZ euc(ZZ x, ZZ y){
    ZZ may ,men,r;
    if(x>=y){   may = x;    men = y;    }
    else{   may = y;    men = x;    }
    long int i=0;
    while(men!=0){
        r = modulo(may,men);
        may = men;
        men = r;
        //cout << r << "  " << may << "  " << men << endl;
        i++;
    }
    cout << "vueltas " << i << endl;
    return may;
}

ZZ euc_bin(ZZ x, ZZ y){
    int i;
    if (x == 0) return y;
    else if (y == 0) return x;
    else if(x==y) return x;
    for (i = 0; ((x&1)==0)and((y&1)==0); i++) {
        x = x>>1;
        y = y>>1;
    }
    
    while ((x&1)==0)    x = x>>1;
    long int j=0;
    while(y != 0){
        while((y&1)==0){y = y>>1;}
        if (x > y) {
            ZZ t = y;
            y = x;
            x = t;
        }
        y = y - x;
       j++;
    }
    cout << "vueltas " << j << endl;
    return x << i;
}

ZZ euc_ext(ZZ a, ZZ b){
    ZZ q, r, r1, r2, s, s1, s2, t, t1, t2;
    r1 = a; r2 = b;
    s1 = ZZ(1); s2 = ZZ(0);
    t1 = ZZ(0); t2 = ZZ(1);
    long int i=0;
    while(r2>0){
        q = r1/r2;
        r = r1 -q*r2;
        r1 = r2;
        r2 = r;

        s = s1 - q*s2;
        s1 = s2;
        s2 = s; 
        
        t = t1 - q*t2;
        t1 = t2;
        t2 = t;

        i++;
    }
    return modulo(s1,b);

}


ZZ euc_ext_bin(ZZ a, ZZ b){
    if(a==0) return b;
    else if(b==0) return a;
    else if(a==b) return a;
    long int i;
    for(i=0;(a&1)==0 and (b&1)==0;i++){
        a=a>>1;
        b=b>>1;
    }
    ZZ u=a, v=b, A=ZZ(1), B=ZZ(0), C=ZZ(0), D=ZZ(1);
    while((u&1)==0){
        u = u<<1;
        if(modulo(A,ZZ(2)) == modulo(B,ZZ(2)) == 0){
            A = A>>1;
            B = B>>1;
        }
    }
    while((v&1)==0){
        v = v>>1;
        if(modulo(C,ZZ(2)) == modulo(D,ZZ(2)) == 0){
            C = C>>1;
            D = D>>1;
        }
    }
    if(u>=v){
        u = u-v;
        A = A-C;
        B = B-D;
    }
    if(v==0){
        a = C;
        b = D;
    }

    cout << a << endl << endl;
    cout << b << endl << endl;

    return (v<<i);
}


ZZ min_res(ZZ a, ZZ b){
    ZZ r=ZZ(1), aux;
    //long int i=0;
    while(r>0){
        r = modulo(a,b);
        aux = b>>1;
        if(r>aux){  a=b; b=b-r;}
        else{   a=b; b=r;   }
        //i++;
    }
    //cout << "vueltas: " << i << endl;
    return a;
}

ZZ min(ZZ x, ZZ y){
    if(x<y) return x;
    return y;
}

ZZ lsb_gcd(ZZ a, ZZ b){
    ZZ x = a, y = b;
    if(x==0) return y;
    else if(y==0) return x;
    else if(x==y) return y;
    ZZ t, temp;
    long int i, j, vu=0;
    while(y!=0){
        i=0;
        while((y<<i)<=x)  i++;
        j = i-1;
        t = min(x-(y<<j),(y<<i)-x);
        x = y;  y = t;
        if(x<y){
            temp = x;
            x = y;
            y = temp;
        }
        //vu++
    }
    //cout << vu << endl;
    return x;
}


ZZ exp_mod(ZZ x, ZZ y, ZZ mod){
    ZZ res = ZZ(1), aux;
    while(y>0){
        if((y&1)==1)    res = modulo((x*res),mod);
        y = y>>1;
        aux = modulo(x,mod)*modulo(x,mod);
        x = modulo(aux,mod);
    }
    return res;
}

ZZ exp(ZZ x, ZZ y){
    ZZ res = ZZ(1);
    while(y>0){
        if((y&1)==1) res = res*x;
        y = y>>1;
    }
    return res;
}

ZZ potencia(long int x, long int y){
    ZZ pot = ZZ(1);
    for(long int i=0;i<y;i++){
        pot = pot*x;
    }
    return pot;
}


ZZ lehmer(ZZ x, ZZ y, long int pot, long int sys){
    ZZ w = potencia(sys,pot);
    ZZ a0, a1, u0, u1, v0, v1, q0, q1, r, q, R, t;
    cout << w << endl;

    long int h, vu=0;
    while(y>=w){
        h = log(y) +2 - pot;
        a0 = x>>h;
        a1 = y>>h;
        u0 = ZZ(1); u1 = ZZ(0);
        v0 = ZZ(0); v1 = ZZ(1);
        while((a1+u1!=0) and (a1+v1!=0)){
            q0 = (a0+u0)/(a1+u1);
            q1 = (a0+v0)/(a1+v1);
            if(q0!=q1)  break;
            r = a0-(q0*a1);
            a0 = a1; a1 = r;
            r = u0-(q0*u1);
            u0 = u1; u1 = r;
            r = v0-(q0*v1);
            v0 = v1; v1 = r;
            vu++;
        }
        if(v0==0){
            q = x/y;
            R = x-(q*y);
            x = y; y = R;
        }
        else{
            R = u0*x + v0*y;
            t = u1*x + v1*y;
            x = R; y = t;
        }
        if(y==0) return x;   
    }
    x = euc_bin(x,y);

    return x;
}


ZZ resto_chino(vector<ZZ> a, vector <ZZ> p){
    vector<ZZ> P, q;
    ZZ N = ZZ(1), x=ZZ(0);
    for(long int i=0;i<p.size()-1;i++)
        if(min_res(p[i], p[i+1])!=1) return ZZ(-1);
    for(long int i=0;i<p.size();i++)    N*=p[i];
    for(long int i=0;i<a.size();i++){
        P.push_back(N/p[i]);
        q.push_back(euc_ext(modulo(P[i],p[i]),p[i]));
        x+=a[i]*P[i]*q[i];
    }
    return modulo(x,N);

}