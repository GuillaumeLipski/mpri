
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double random(){
	int d = rand();
	d=(d%9)+1;
	double f=d/10.0;
	return f;
}

int main(void) {
	srand(time(NULL));
	double d=random();
	printf("%lf",d);
}