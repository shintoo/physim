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
	char outs[30];				// output file name storage
	int NumForces, NumObj, ObjInd;		// num of forces, objects, object index
	double mass;				// mass of object
	union vals data;			// integer or double (nforces, mass)
	struct vector time;			// time range
	struct vector window[2];		// window range
	
	struct vector initVel, initPos;

	ReadValue(fin, "forces", &data);		// read in number of forces
	rewind(fin);
	NumForces = data.intgr;
	
	ReadValue(fin, "mass", &data);		// read in object mass
	rewind(fin);
	mass = data.dbl;

	ReadTime(fin, &time);			// read in time range
	rewind(fin);

	ReadWindow(fin, window);		// read in window range
	rewind(fin);
	
	ReadInitials(fin, &initVel, &initPos);
	rewind(fin);

	printf("Initial velocity:\n\t<%f,%f>\nInitial position\n\t<%f,%f>\n",
		initVel.x, initVel.y, initPos.x, initPos.y);
	
	struct force forces[NumForces];		// build forces
	ReadForces(fin, NumForces, forces);
	rewind(fin);
	
	NumObj = (int) round(time.y - time.x);	// build object
	struct vector object[NumObj];
	
	ApplyForces(NumForces, NumObj,  object, // apply forces to object
		    forces, mass, &time,
		    &initVel, &initPos);

	strcpy(outs, argv[1]);			// open output file
	outs[22] = '\0';
	strcat(outs, "_physim");
	fout = fopen(outs, "w");

	WriteGraph(fout, NumObj, object,	// create and write
		   NumForces, forces,
		   window, &time);		// graph to output file
	fclose(fin);
	fclose(fout);	
	return 0;
}
