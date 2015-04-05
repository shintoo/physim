#include "physim.h"

//finds string str in file fp, returns location
int FindString(FILE *fp, char *str, fpos_t *strloc) {
	char c;
	int len = 0, i = 0, ret_val = 1;			// ret_val: 0 = notfound, 1 = found

	len = strlen(str);
	while ((c = fgetc(fp)) != EOF) {
		if (c == '#') {					// ignore comments
			while (c != '\n' && c != EOF)
				c = fgetc(fp);
			c = fgetc(fp);
		}
		if (c == str[0]) {				// Start comparing if first char is found
			for (i = 1; i < len; i++) {
				if ((c = fgetc(fp)) == str[i]) {
					if (i == len - 1) {
						fgetpos(fp, strloc);	// set loc to
						strloc->__pos -= len;	// BEGINNING
						ret_val = 0;		// of string
						break;
					}
				} else {
					ret_val = 1;
					break;
				}
			}
			if (ret_val = 0)
				break;
		}
	}
	return ret_val;
}

//reads either double type for mass or integer type for nforces from file fp
void ReadValue(FILE *fp, char *str, vals *var) {
	int i = 0;
	char temp[64], c;
	fpos_t strloc;
	if (FindString(fp, str, &strloc) != 0) {
		printf("Error: missing argument: %s\n", str);
		exit(EXIT_FAILURE);
	}
	while (c != EOF)					// eat rest of file after previous readval use
		c = fgetc(fp);
	fsetpos(fp, &strloc);
	while (c != '\n' && c != '#') {				// read chars into temp char array
		if (isdigit((c = getc(fp))) || c == '.')
			temp[i++] = c;
	}
	temp[i + 1] = '\0';					// make temp a string to convert
	if (str[0] == 'f')					// for forces
		var->intgr = atoi(temp);
	if (str[0] == 'm')					// for mass
		var->dbl =  atof(temp);
	rewind(fp);
}

//reads numeric values on line line in file fp into vector structure vect
void ReadVector(FILE *fp, fpos_t *loc, Vector *vect) {
	char c, val1[32], val2[32];
	int i = 0, nlct = 0, decct = 0;				// newline count, decimal count

	fsetpos(fp, loc);
	i = 0;							// *****value 1*****
	while ((c = fgetc(fp)) != EOF) {
		if (c == '#') {					// ignore comments
			while (c != '\n' && c != EOF)
				c = fgetc(fp);
		}
		if (c == '<')
			break;
	}
	if (c != '<') {
		puts("Error: No vector found: use format <x,y>\n"
		"Error ID: 74\n");
		exit (EXIT_FAILURE);
	}
	c = fgetc(fp);						// c may be '-' for negative
	val1[i++] = c;
	while (isdigit(c = fgetc(fp)) || c == '.')		// read numbers and decimal
		val1[i++] = c;
	if (c != ',') {
		puts("\nError reading vector: expecting ',' between values.\n"
		"Error ID: 83\n");
		exit(EXIT_FAILURE);
	}
	val1[i] = '\0';
	i = 0;							// *****value 2*****
	c = fgetc(fp);
		val2[i++] = c;
	while (isdigit(c = fgetc(fp)) || c == '.') {
		if (c == '.') {
			decct++;
			if (decct > 1) {
				puts("Error: reading value: multiple decimals");
				exit(EXIT_FAILURE);
			}
		}
		val2[i++] = c;
	}
	if (c != '>') {
		puts("\nError reading vector: expecting '>'. use format <x,y>\n"
		"Error ID: 96\n");
		exit(EXIT_FAILURE);
	}
	val2[i] = '\0';
	vect->x = atof(val1);					// convert and store
	vect->y = atof(val2);
}

//reads nforces amount of forces into array of force forces[] from file fp
void ReadForces(FILE *fp, const int NumForces, Force forces[]) {
	fpos_t floc;
	char temp[16], time[16], c;
	int tempct, tempi;

	FindString(fp, "forces", &floc);
	fsetpos(fp, &floc);					// start reading at "forces"
	for (int i = 0; i < NumForces; i++) {			// do for each force
		tempct = 0;
		ReadVector(fp, &floc, &(forces[i].cmpnt));
		c = fgetc(fp);
		
		if (c == ' ' || c == '\t') {			// read time applied
			while ((c = fgetc(fp)) != '\n' && c != '#') {
				if (isdigit(c)) {
					temp[tempct++] = c;
				}
			}
			for (tempi = 0; tempi < tempct; tempi++)
				time[tempi] = temp[tempi];
		} else {
			printf("Error: expecting space or tab before time: use format <x,y> t\n");
			exit(EXIT_FAILURE);
		}
		time[tempi + 1] = '\0';
		forces[i].time = atoi(time);
	
		while (c != '\n')
			c = fgetc(fp);
		c = fgetc(fp);
		fgetpos(fp, &floc);
	}
}

// reads in the window range
void ReadWindow(FILE *fp, Vector window[2]) {
	fpos_t winloc;
	char c;

	FindString(fp, "window", &winloc);
	fsetpos(fp, &winloc);

	for (int i = 0; i < 2; i++) {				// read bottom left & top right
		ReadVector(fp, &winloc, &(window[i]));
		while (c != '\n')
			c = fgetc(fp);
		c = fgetc(fp);
		fgetpos(fp, &winloc);
	}
	if (window[0].x > window[1].x || window[0].y > window[1].y) {
		printf("Error: invalid window range\n");
		exit(EXIT_FAILURE);
	}
}

// reads in the time range
void ReadTime(FILE *fp, Vector *time) {
	fpos_t tloc;
	
	FindString(fp, "time", &tloc);
	ReadVector(fp, &tloc, time);
}

// reads in the initial velocity and initial acceleration
void ReadInitials(FILE *fp, Vector *velocity, Vector *position) {
	fpos_t vel, pos;

	if (FindString(fp, "pos", &pos) == 0) {
		ReadVector(fp, &pos, position);
	}
	rewind(fp);
	if (FindString(fp, "vel", &vel) == 0) {
		ReadVector(fp, &vel, velocity);
	}
}

//applies nforces amount of forces in array of force forces[] onto vectors in objects[], turning a force into a position using the newton's second law
void ApplyForces(const int NumForces, const int NumObj, Vector object[NumObj], const Force forces[NumForces], const double mass, const Vector *time, const Vector *initVel, const Vector *initPos) {
	int frcct = 0;						// force in queue
	struct vector acc = {0,0};
//	struct vector vel = {initVel->x, initVel->y};		// initial vel - not yet working
	struct vector vel = {0,0};
	int obji = 0;						// object index

	object[obji].y = initPos->y;
	object[obji].x = initPos->x;
	for (int i = time->x; i < time->y; i++) {
		obji = i - time->x + 1;				// start at time->x, increment	
		object[obji].y = initPos->y;
		object[obji].x = initPos->x;
		if (i == forces[frcct].time) {
			acc.y += forces[frcct].cmpnt.y / mass;	// apply force to acc
			acc.x += forces[frcct].cmpnt.x / mass;	//
			++frcct;				// load next force
		}
		object[obji].y += vel.y += acc.y;		// apply acc to object
		object[obji].x += vel.x += acc.x;		//
	}
}

//creates a visual 2d array of size window plotting the path of objects[] over time period time
void WriteGraph(FILE *fp, const int NumObj, const Vector object[NumObj], const int NumForces, const Force forces[NumForces], const Vector window[2], const Vector *time) {
	int xsize, ysize;					// size of graph
	int x, y, t;						// position in x, y, time
	int obji, frcct = 0;					// index for object
	char c;

	xsize = abs(round(window[1].x - window[0].x)) + 1;	// +1 is for null character
	ysize = abs(round(window[1].y - window[0].y));	
	char graph[ysize][xsize];
	
	for (y = 0; y < ysize; y++) {				// initialize graph to empty
		for (x = 0; x < xsize; x++)
			graph[y][x] = '-';
		graph[y][xsize - 1] = '\0';
	}
	y = 0;
	for (t = time->x; t < time->y; t++) {
		obji = t - time->x;
		x = (int) object[obji].x;
		y = (int) object[obji].y;
		if (!((y < window[0].y || y > window[1].y) || (x < window[0].x || x > window[1].x))) {
			if (t == forces[frcct].time) {
				c = frcct + '0';
				graph[y][x] = c;
				frcct++;
			} else {
				if (graph[y][x] == '-') {
					graph[y][x] = 'X';
				}
			}
		}
		obji++;
	}
	for (y = ysize - 1; y >= 0; y--) {			// y - axis
		fputs(graph[y], fp);
		fprintf(fp, "%d\n", y);
	}	
/*	for (int i = 0; i < xsize; i++) {			// x - axis
		fprintf(fp, "%d", i % 10);
	}
	fputc('\n', fp);
	for (int i = 0; i < xsize; i++) {
		fprintf(fp, "%d", (i % 10) / 10);
	}
*/
}

