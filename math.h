#ifndef _MATH_H_
#define _MATH_H_
#define GRANDE 0x01ff

typedef struct what{
	long x; // Usually in 18.14 encoding
	long y; // Usually in 18.14 encoding
}Tvector;

long sin(int x);
long cos(int x);
void rotate(Tvector *v, int a);
//void printfix(long i);
long expanded(long i);


#endif /*_MATH_H_*/