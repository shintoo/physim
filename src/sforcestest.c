#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "physim.c"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("usage: %s <file>", argv[0]);
		exit(EXIT_FAILURE);
	}
	FILE *fin, *fout;
	if ((fin = fopen(argv[1], "r")) == NULL) {
		printf("%s: error opening file %s", argv[0], argv[1]);
		exit(EXIT_FAILURE);
	}
	char outs[20];
	int nforces;
	double mass;
	union vals nfms;
	struct vector time;
	struct vector window[2];
	readval(fin, "forces", &nfms);
	rewind(fin);
	nforces = nfms.intgr;
	readval(fin, "mass", &nfms);
	rewind(fin);
	mass = nfms.dbl;
	struct force forces[nforces];
	stime(fin, &time);
	swindow(fin, window);
	printf("time: <%f,%f>\nmass: %f\nwindow:\n\t<%f,%f>\n\t<%f,%f>\n",
		 time.x, time.y, mass, window[0].x, window[0].y, window[1].x, window[1].y);
	sforces(fin, nforces, forces);	//reads forces from file into array of force structures
	for (int i = 0; i < nforces; i++)
		printf("%d: <%f,%f>\n", i, forces[i].cmpnt.x, forces[i].cmpnt.y);
	
	return 0;
}
