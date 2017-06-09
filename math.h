#ifndef _MATH_H_
#define _MATH_H_

typedef struct{
	long x;
	long y;
}Tvector;

long sin(int x);
long cos(int x);
void rotate(struct TVector *v, int a);

#endif /*_MATH_H_*/