#ifndef _MATH_H_
#define _MATH_H_
#define GRANDE 0x01ff

typedef struct{
	long x;
	long y;
}Tvector;

long sin(int x);
long cos(int x);
void rotate(Tvector *v, int a);
//void printfix(long i);
long expanded(long i);

long convert(int);


#endif /*_MATH_H_*/