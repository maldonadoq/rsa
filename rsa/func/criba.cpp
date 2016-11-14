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

using namespace std;
using namespace NTL;

vector<unsigned> criba_(int bt){
	
	int64_t t = 1;
	t = (t<<bt)-1;
	int64_t sqr = sqrt(t);
	int64_t cache = 32768;
	unsigned segment = max(sqr, cache);
	int64_t s = 3, n = 3, tem = 0;

	int sqr_ = sqrt(sqr);
	vector<bool> b (sqr+1, 1);
	vector<unsigned> v;
	vector<bool> criba(segment);
	vector<int> primo;
	vector<int> next;
	ZZ co = ZZ(0);

	for(int i=2; i<sqr_; i++)
		if(b[i])
			for(int j=i*i; j<=sqr; j+=i)
				b[j] = false;

	for(int64_t bg=0; bg<=t; bg+=segment){
		fill(criba.begin(), criba.end(), true);
		int64_t en = min(bg+segment-1, t);
		for(;s*s<=en; s+=2){
			if(b[s]){
				primo.push_back(s);
				next.push_back(s*s-bg);
			}
		}
		for(int i=0; i<primo.size(); i++){
			int j = next[i];
			for(int k=primo[i]*2; j<segment; j+=k)	criba[j] = false;
			next[i] = j-segment;
		}
		for(; n<=en; n+=2)
			if(criba[n-bg]) v.push_back(n);
	}
	return v;
}
/*
int main(int argc, char const *argv[]){
	int t = 30;
	vector<unsigned> v;
	struct timeval ti, tf;
    double tiempo;
    gettimeofday(&ti, NULL);
   	v = criba_(t);
   	cout << v.size() << endl;
    gettimeofday(&tf, NULL);
    tiempo = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
   	printf("Tiempo demorado:  %.28f segundos\n", tiempo/1000);

	return 0;
}*/