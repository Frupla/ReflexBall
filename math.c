#include "sinLUT.h"

typedef struct{
	long x;
	long y;
}Tvector;

long sin(int x){
	if(x<0){
		return SIN[512 - (~x+1) & GRANDE];
	}	
	return SIN[x & GRANDE];
}

long cos(int x){
	if(x<0){
		return SIN[(640 - (~x+1)) & GRANDE];
	}
	return SIN[(x + 128)& GRANDE];
}

void rotate(TVector *v, int a){
	int temp = v->x;
	int tempie = sin(a);
	int tem = cos(a);
	v->x = temp * cos(a) - v->y * sin(a);
	v->y = temp * sin(a) + v->y * cos(a);
}