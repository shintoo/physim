/* Physim: 2D vector-force physics simulator
*	The purpose of this program is to create a
*	parametric graph in which x(t) and y(t)
*	are based off of forces applied to an
*	object with mass, reading the mass,
*	forces, time range, and window range
*	from an easily written file.
* ------------------------------------------------
* TODO:
*	- fix what doesnt work
*		- vectorread()
*		- sforces(), swindow(), stime(), applyforces
*	- write writegraph()
*	- include initial position and velocity options
*	- include "auto" option for window
*	- include time-interval forces, e.g. "<10,21> 0-200" or
*					     "<0,-20> always"
*	- thorough error handling and reporting
*/


#include <stdio.h>
#include <stdlib.h>
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
	rewind(fin)l
	nforces = nfms.intgr;
	readval(fin, "mass", &nfms);
	rewind(fin);
	mass = nfms.mass;
	
	struct force forces[nforces];
	
	stime(fp, &time)
	struct vector object[time.y - time.x]	// declares array of object structures with an amount of members
						// equal to range of time
	
	swindow(fin, window)		// reads window size (botleft, topright) from file into window
	sforces(fin, nforces, forces);	//reads forces from file into array of force structures
	
	applyforces(forces, object, &time);
	
	strncpy(outs, argv[1]);
	outs[15] = '\0';
	strcat(outs, ".phs");
	fopen(outs, "w");
	
	writegraph(fout, object, window, &time);
	
	fclose(fin);
	fclose(fout);
	return 0;
}
