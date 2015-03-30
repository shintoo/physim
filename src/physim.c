#include "physim.h"


//finds string str in file fp, returns location
int findstr(FILE *fp, char *str, fpos_t *strloc) {
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
void readval(FILE *fp, char *str, vals *var) {
	int i = 0;
	char temp[64], c;
	fpos_t strloc;
	if (findstr(fp, str, &strloc) != 0) {
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
	if (str[0] == 'f')					// for nforces
		var->intgr = atoi(temp);
	if (str[0] == 'm')					// for mass
		var->dbl =  atof(temp);
	rewind(fp);
}

//reads numeric values on line line in file fp into vector structure vect
void vectorread(FILE *fp, fpos_t *loc, struct vector *vect) {
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
void sforces(FILE *fp, const int nforces, struct force forces[]) {
	fpos_t floc;
	char temp[16], time[16], c;
	int tempct, tempi;

	findstr(fp, "forces", &floc);
	fsetpos(fp, &floc);					// start reading at "forces"
	for (int i = 0; i < nforces; i++) {			// do for each force
		tempct = 0;
		vectorread(fp, &floc, &(forces[i].cmpnt));
		c = fgetc(fp);
		
		if (c == ' ' || c == '\t') {			// read time applied
			while ((c = fgetc(fp)) != '\n' && c != '#') {
				if (isdigit(c))
					temp[tempct++] = c;
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

//may be replaced by two vectorreads in main()
void swindow(FILE *fp, struct vector window[2]) {
	fpos_t winloc;
	char c;

	findstr(fp, "window", &winloc);
	fsetpos(fp, &winloc);

	for (int i = 0; i < 2; i++) {				// read bottom left & top right
		vectorread(fp, &winloc, &(window[i]));
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

//may be replaced by a vectorread in main()
void stime(FILE *fp, struct vector *time) {
	fpos_t tloc;
	findstr(fp, "time", &tloc);
	vectorread(fp, &tloc, time);
}

//applies nforces amount of forces in array of force forces[] onto vectors in objects[], turning a force into a position using the newton's second law
void applyforces(const int nforces, const int numob, struct vector object[numob], const struct force forces[nforces], const double mass, const struct vector *time) {
	int frcct = 0;						// force in queue
	struct vector acc = {0,0}, vel = {0,0};			// initial values
	int obji = 0;						// object index

	for (int i = time->x; i < time->y; i++) {
		obji = i - ((int) time->x);			// start at time->x, increment
		if (i == forces[frcct].time) {
			acc.x += forces[frcct].cmpnt.x / mass;	// apply force to acc
			acc.y += forces[frcct].cmpnt.y / mass;	//
			++frcct;				// load next force
		}
		object[obji].x += vel.x += acc.x;		// apply acc to object
		object[obji].y += vel.y += acc.y;		//
	}
}

//creates a visual 2d array of size window plotting the path of objects[] over time period time
void writegraph(FILE *fp, const int nobj, const struct vector object[nobj], const struct vector window[2], const struct vector *time) {
	int xsize, ysize;
	int x, y, t;
	int obji;

	xsize = abs(round(window[1].x - window[0].x)) + 1;
	ysize = abs(round(window[1].y - window[0].x));
	printf("ysize: %d\nxsize: %d\n", ysize, xsize);	
	char graph[ysize][xsize];
	
	for (y = 0; y < ysize; y++) {
		for (x = 0; x < xsize; x++)
			graph[y][x] = '.';
		graph[y][61] = '\0';
	}
	for (t = time->x; t < time->y; t++) {
		obji = t - time->x;
		x = (int) object[obji].x;
		y = (int) object[obji].y;
		graph[y][x] = 'x';
		obji++;
	}

	puts("------------");
	puts(graph[ysize - 1]);
	fputs(graph[0], fp);
	fputc('\n', fp);
	fputs(graph[25], fp);
	fputc('\n', fp);
	fputs(graph[59], fp);
	printf("\n%f %f\n", time->x, time->y);
	
//	for (y = ysize - 1; y > 0; y++) {
//		fputs(graph[y], stdout);
//		fputc('\n', stdout);
//	}
}
