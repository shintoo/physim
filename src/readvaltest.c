#include <stdio.h>
#include <stdlib.h>
#include "physim.c"


int main(int argc, char *argv[]) {
	FILE *fin = fopen(argv[1], "r");
	char c;
	int nforces = 0;
	double mass = 0;
	vals nfms;

	readval(fin, "forces", &nfms);
	nforces = nfms.intgr;
	readval(fin, "mass", &nfms);
	mass = nfms.dbl;
	printf("mass: %f\nnforces: %d\n", mass, nforces);
	return 0;
}
