/** Physim: 2D single-object physics simulator
*	The purpose of this program is to create a
*	graph in which x(t) and y(t) are based off
*	of forces applied to an object with mass,
*	reading the mass, forces, time range, and
*	window range from an easily written file.
*
*-------------------------------------------------
*
* TODO:
*	- fix writegraph()
*	- allow for initial position and velocity
*	- allow for "auto" option for window
*	- allow for time-interval force application, e.g.
*		- <10,21> 0-30
*		- <0,-20> always
*	- thorough error handling
*/

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
	char outs[20];				// output file name storage
	int nforces, nobj, obji;		// num of forces, objects, object index
	double mass;				// mass of object
	union vals nfms;			// integer or double (nforces, mass)
	struct vector time;			// time range
	struct vector window[2];		// window range

	readval(fin, "forces", &nfms);		// read in number of forces
	rewind(fin);
	nforces = nfms.intgr;
	
	readval(fin, "mass", &nfms);		// read in object mass
	rewind(fin);
	mass = nfms.dbl;

	stime(fin, &time);			// read in time range
	rewind(fin);

	swindow(fin, window);			// read in window range
	rewind(fin);
	
	struct force forces[nforces];		// build forces
	sforces(fin, nforces, forces);
	rewind(fin);

	nobj = (int) round(time.y - time.x);	// build object
	struct vector object[nobj];

	applyforces(nforces, nobj,  object, 	// apply forces to object
		    forces, mass, &time);

	strcpy(outs, argv[1]);			// open output file
	outs[15] = '\0';
	strcat(outs, ".ps");
	fout = fopen(outs, "w");

	writegraph(fout, nobj, object, 		// create and write
		   window, &time);		// graph to output file
	
	return 0;
}
