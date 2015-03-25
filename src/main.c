/* TODO:
*	- Create an array of objects, one for each frame
*	- Change forces to the force structure, create array
*	- read force components and application time from file
*	- read object mass, and initial position from same file
*	- Write the display function
*	- Write apply force function
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "physim.c"

int main(int argc, int *argv[]) {
	if (argc != 2) {
		printf("usage: %s <file>", argv[0]);
		exit(EXIT_FAILURE);
	}
	FILE *fp;
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("%s: error opening file %s", argv[0], argv[1]);
		exit(EXIT_FAILURE);
	}
	unsigned double mass;		// mass of object read from fp
	unsigned int nforces;		// number of forces
	// nforces = [from file: find line that begins with "forces: ", get the digits until '\n'
	// into a string, convert to integer
	force forces[nforces];
	vector time;			//.x = begin time, .y = end time
	vector window[2];		// bottom left and top right window values <x,y>
	stime(fp, &time);		// reads time from file into time structure (begin, end)
	vector object[time.y - time.x]	// declares array of object structures with an amount of members
					// equal to range of time
	swindow(fp, window);		// reads window size (botleft, topright) from file into window
	sforces(fp, nforces, forces[]);	//reads forces from file into array of force structures
	applyforces(forces[], object[], &time);
	display(object[], window, &time);
	fclose(fp);
	return 0;
}
