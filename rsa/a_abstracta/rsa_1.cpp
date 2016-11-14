#include <NTL/ZZ.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/time.h>
#include <thread> 
#include <future>
#include "fback.cpp"
#include <stack>
#include "../func/func.cpp"
#include "../func/rsa_fun.cpp"
#include "../func/random.cpp"
#include "../func/criba.cpp"

static string alfa = "0123456789,abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ-.";
static int t_criba = 30;
static vector<unsigned> criba = criba_(t_criba);

using namespace std;
using namespace NTL;

class cesar{
private:
	int clave, n;
	FILE *t_ces, *t_dces;
public:

	cesar(ZZ _clave){
		this->n = alfa.size();
		unsigned ft;
		conv(ft, modulo(_clave, ZZ(n)));
		this->clave = ft;
	}
	void cifra(FILE *t_pl){
		char aux;
		int temp;
		t_ces = fopen("sws/t_ci_c.txt", "w");
		while((aux=getc(t_pl))!=EOF){
			temp = modulo_int(alfa.find(aux)+clave, n);
			fprintf(t_ces, "%c", alfa[temp]);
		}
		fclose(t_ces);
	}
	void descifra(FILE *t_dc){
		char aux;
		int temp;
		t_dces = fopen("sws/t_dc_c.txt", "r");
		while((aux=getc(t_dces))!=EOF){
			temp = modulo_int(alfa.find(aux)-clave, n);
			fprintf(t_dc, "%c", alfa[temp]);
		}
		fclose(t_dces);
	}
	~cesar(){	}
};

class rsa{
private:
	ZZ p, q ,c_pr, dh_kpr, dh_both;
	FILE *t_c, *t_dc;
	vector<ZZ> _p, _q, P, dpq;
	string sp, sq, scp, sap, saq, sacp;
	cesar *first;
public:
	int k = 1000;
	stack<ZZ> mat;
	double time_al, time_ope, time_pr;
	ZZ c_pu, N, dh_kpu, dh_p, dh_g;
	rsa(long int);
	ZZ resto_chino_rsa(ZZ);
	void cifrado(FILE*, ZZ, ZZ);
	void descifrado(FILE*);
	void diffie_hellman();
	void test_primalidad();
	bool miller(string, ZZ, string *);
	void pthr(string *, ZZ, string *);
	virtual ~rsa(){	};
};

rsa::rsa(long int t){
	mat = camera();

	struct timeval ti, tf;
	gettimeofday(&ti, NULL);

	unsigned i, j;
	ZZ fl = ZZ(criba.size()-1) >> 1;
	conv(i, modulo(_random(mat.top(), t_criba-2), fl) + fl-2);	mat.pop();
	conv(j, modulo(_random(mat.top(), t_criba-2), fl) + fl-2);	mat.pop();

	this->dh_p = criba[criba.size()-2];
	this->dh_g = criba[i];
	this->dh_kpr = criba[j];
	this->dh_kpu = exp_mod(this->dh_g, this->dh_kpr, this->dh_p);

	this->p = _random(mat.top(), t);	mat.pop();
	this->sp = zz_to_bin(this->p);

	this->q = _random(mat.top(), t-1);	mat.pop();
	this->sq = zz_to_bin(this->q);
	
	this->c_pu = _random(mat.top(), t-2);	mat.pop();
	this->scp = zz_to_bin(this->c_pu);
	
	this->sap = zz_to_bin(mat.top()); 	mat.pop();
	this->saq = zz_to_bin(mat.top()); 	mat.pop();
	this->sacp = zz_to_bin(mat.top()); 	mat.pop();

	gettimeofday(&tf, NULL);
	this->time_al = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
}

bool rsa::miller(string nu, ZZ k, string *aw){
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

void rsa::pthr(string *s, ZZ k, string *aw){
    bool t = true;
    while(t==true){
        if(miller(*s, k, aw))  t = false;
        else    dezpl(s);
    }
}

void rsa::diffie_hellman(){
	ZZ dh_bob;
	cout << "diffie_hellman bob:  "; cin >> dh_bob;
	this->dh_both = exp_mod(dh_bob, this->dh_kpr, this->dh_p);
	this->first = new cesar(this->dh_both);
}

void rsa::test_primalidad(){
	bool t=true, s=true, r=true;

	struct timeval ti, tf;
	gettimeofday(&ti, NULL);
	/*thread a(pthr, &sp, ZZ(k), &sap);
	thread b(pthr, &sq, ZZ(k), &saq);
	thread c(pthr, &scp, ZZ(k), &sacp);
	a.join();	b.join();	c.join();*/
	
	pthr(&sp, ZZ(k), &sap);
	this->p = bin_to_zz(sp);
	pthr(&sq, ZZ(k), &saq);
	this->q = bin_to_zz(sq);
	pthr(&scp, ZZ(k), &sacp);
	this->c_pu = bin_to_zz(scp);
	gettimeofday(&tf, NULL);

	this->time_pr = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
	ZZ ps = (this->p-1)*(this->q-1);
	this->c_pr = euc_ext(c_pu, ps);
	this->N = this->p*this->q;

	gettimeofday(&ti, NULL);
	_p.push_back(p);
	_p.push_back(q);

	_q.push_back(euc_ext(modulo(q,p),p));
	_q.push_back(euc_ext(modulo(p,q),q));
	
	P.push_back(q);
	P.push_back(p);

	dpq.push_back(modulo(c_pr, p-1));
	dpq.push_back(modulo(c_pr, q-1));

	gettimeofday(&tf, NULL);
	this->time_ope = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
}

void rsa::cifrado(FILE *t_pl, ZZ c_pu, ZZ N){
	vector<ZZ> v;
	ZZ t_cif, c1 = ZZ(0);
	string tb = to_string(alfa.size());
	int bq1 = tb.size(), p, c;
	ZZ bq2 = ZZ(zz_to_str(N).size() - 1);
	ZZ bq3 = ZZ(zz_to_str(N).size());

	/*this->first->cifra(t_pla);
	FILE *t_pl = fopen("sws/t_ci_c.txt", "r");*/

	struct timeval ti, tf;
	gettimeofday(&ti, NULL);

	string sms = "";
	while((c=getc(t_pl))!=EOF){
		if(c!=10){
			p = alfa.find(c);
			string aux = to_string(p);
			if(aux.size() == bq1) sms += aux;
			else sms += con_zero(aux, ZZ(bq1));
		}
	}

	string asd = "";
	ZZ smst = ZZ(sms.size())/bq2;
	long int a=0;
	t_c = fopen("sws/t_ci.txt", "w");
	for(long int i=0; i<sms.size(); i++){
		asd += sms[i];
		c1++;
		if(bq2>=sms.size() and (i+1)==sms.size()){
			ZZ num(INIT_VAL, asd.c_str());
			asd = zz_to_str(exp_mod(num, c_pu, N));
			load_bq_(t_c, asd, bq3);
		}
		else{
			if(c1 == bq2 and a!=smst){
				c1 = ZZ(0);
				ZZ num(INIT_VAL, asd.c_str());
				asd = zz_to_str(exp_mod(num, c_pu, N));
				load_bq_(t_c, asd, bq3);
				asd = "";
				a++;
			}
			else if((i+1)==sms.size()){
				while(asd.size()!=bq2) asd+="3";
				ZZ num(INIT_VAL, asd.c_str());
				asd = zz_to_str(exp_mod(num, c_pu, N));
				load_bq_(t_c, asd, bq3);
			}
		}
	}
	fclose(t_c);

	gettimeofday(&tf, NULL);
	double tim1 = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
	printf("time gen_aleatorio:  %.28f\n", tim1/1000);
}

ZZ rsa::resto_chino_rsa(ZZ w){
	ZZ x = ZZ(0), a, ad;
	for(long int j=0;j<_p.size();j++){
		ad = modulo(w, _p[j]); //saca modulo de t_cifr con p o q respe
		a = exp_mod(ad, dpq[j],_p[j]); //saca el a , para cada uno
		x+= a*P[j]*_q[j];
	}
	x = modulo(x, N);
	return x;
}

void rsa::descifrado(FILE *t_c){
	ZZ bq1 = ZZ((zz_to_str(ZZ(alfa.size()))).size());
	ZZ bq2 = ZZ(zz_to_str(this->N).size() - 1);
	ZZ bq3 = ZZ(zz_to_str(this->N).size());
	ZZ temp = ZZ(0);
	vector<ZZ> v;

	struct timeval ti, tf;
	gettimeofday(&ti, NULL);
	char c;
	string num = "";
	while((c=getc(t_c))!=EOF){
		temp++;
		num += c;
		if(temp == bq3){
			ZZ aux(INIT_VAL, num.c_str());
			v.push_back(aux);
			num = "";
			temp = ZZ(0);
		}
	}

	string sms = "";
	for(long int i=0; i<v.size(); i++){
		temp = resto_chino_rsa(v[i]);
		num = zz_to_str(temp);
		if(i == v.size()-1){
			sms += num;
		}
		else if(temp.size()==bq2)	sms += num;
		else	sms += con_zero(num, bq2);
	}

	num = "";
	FILE *t_dc = fopen("sws/t_dc.txt", "w");
	for(long int i=0; i<sms.size(); i++){
		num += sms[i];
		if(num.size() == bq1){
			int b = atoi(num.c_str());
			fprintf(t_dc,"%c",alfa[b]);
			num = "";
		}
	}
	fclose(t_dc);

	/*t_dc = fopen("sws/t_dc.txt", "w");
	this->first->descifra(t_dc);
	fclose(t_dc);*/

	gettimeofday(&tf, NULL);
	double tim2 = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
	printf("time gen_aleatorio:  %.28f\n", tim2/1000);
}

int main(int argc, char const *argv[]){
	vector<ZZ> u, v, z_cif;
	ZZ l_cif, c_pu, N, c_pu_b, N_b, ts = ZZ(1);
	bool t = true;
	int aux, temp, n_b;

	while(t){
		cout << "1. iniciar sesion" << endl;
		cout << "2. salir" << endl;
		cout << "opcion:  "; cin >> aux;
		cout << endl;
		switch(aux){
			case 1:{
				cout << "clave_bits:  "; cin >> n_b;
				//n_b = 10;
				rsa *alice = new rsa(n_b);
				bool t1 = true;
				while(t1){
					cout << endl;
					cout << "1. test_primalidad" << endl;
					cout << "2. alice_informacion" << endl;
					cout << "3. diffie_hellman" << endl;
					cout << "4. alice_encripta" << endl;
					cout << "5. alice_desencripta" << endl;
					cout << "6. cerrar sesion" << endl;
					cout << "opcion:  "; cin >> temp;
					cout << endl;
					switch(temp){
						case 1:	
							alice->test_primalidad();
							break;
						case 2:
							cout << "c_pu:  " << alice->c_pu << endl;
							cout << "N:  " << alice->N << endl;
							printf("time_al:  %.28f\n", alice->time_al);
							printf("time_pr:  %.28f\n", alice->time_pr);
							printf("time_ope:  %.28f\n", alice->time_ope);
							printf("time_tot:  %.28f\n", alice->time_ope+alice->time_al+alice->time_pr);
							cout << "mat_size:  " << alice->mat.size() << endl;
							cout << "dh_p:  " << alice->dh_p << endl;
							cout << "dh_g:  " << alice->dh_g << endl;
							cout << "dh_kpu:  " << alice->dh_kpu << endl;
							break;
						case 3:
							alice->diffie_hellman();
							break;
						case 4:{
							ZZ c_b, N_b;
							cout << "c_pu_bob:  ";	cin >> c_b;
							cout << "N_bob:  ";	cin >> N_b;
							FILE *t_pl = fopen("sws/t_pl.txt", "r");
							alice->cifrado(t_pl, c_b, N_b);
							fclose(t_pl);
							break;
						}
						case 5:{
							FILE *t_c = fopen("sws/t_ci.txt", "r");
							alice->descifrado(t_c);
							fclose(t_c);
							break;
						}
						case 6:
							delete alice;
							t1 = false;
							break;
					}
				}
				break;
			}
			case 2:
				t = false;
				break;
		}
	}
	return 0;
}