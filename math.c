#include "sinLUT.h"
#define GRANDE 0x01ff

typedef struct what{
	long x; // Usually in 18.14 encoding
	long y; // Usually in 18.14 encoding
}Tvector;

long sin(int x){
	if(x<0){
		return SIN[(512 - (~x+1)) & GRANDE];
	}	
	return SIN[x & GRANDE];
}

//void printfix(long i){
//	if((i & 0x80000000) != 0){
//		printf("-");
//		i = ~i + 1;
//	}
//	printf("%ld.%04ld", i >> 16, 10000 * (unsigned long) (i & 0xffff) >> 16);
//}

long expanded(long i){
	return i << 2;
}

long cos(int x){
	if(x<0){
		return SIN[(640 - (~x+1)) & GRANDE];
	}
	return SIN[(x + 128)& GRANDE];
}

void rotate(Tvector *v, int a){
	int temp = v->x;
	v->x = temp * cos(a) - v->y * sin(a);
	v->y = temp * sin(a) + v->y * cos(a);
	v->x = v->x >> 14;
	v->y = v->y >> 14;
}

long convert(int number) {
	if ((number & 0x8000) != 0) {
		number = ~number + 1;
		return ~(((long) number) << 14) + 1;
	}
	return ((long) number) << 14;
}
