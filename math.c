#include "sinLUT.h"
#define GRANDE 0x01ff

typedef struct{
	long x;
	long y;
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
}

long convert(int number) {
    if (number & 0x8000) {
        number = ~number + 1;
    }
    return ~(((long) number) << 14) + 1;
}
