#include <string.h>
#include <ctype.h>
#include "physim.h"


//finds string str in file fp, returns location
int findstr(FILE *fp, char *str, fpos_t *strloc) {
	char c;
	int len = 0, i = 0, ret_val = 1;	// ret_val: 1 = notfound, 2 = found

	len = strlen(str);
	while ((c = fgetc(fp)) != EOF) {
		if (c == '#') {			// ignore comments
			while (c != '\n')
				c = fgetc(fp);
			c = fgetc(fp);
		}
		if (c == str[0]) {
			for (i = 1; i < len; i++) {
				if ((c = fgetc(fp)) == str[i]) {
					if (i == len - 1) {
						fgetpos(fp, strloc);
						strloc->__pos -= len;
						ret_val = 0;
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
	while (c != EOF)		// eat rest of file after previous readval use
		c = fgetc(fp);
	fsetpos(fp, &strloc);
	while (c != '\n') {		// read chars into temp char array
		if (isdigit((c = getc(fp))) || c == '.')
			temp[i++] = c;
	}
	temp[i + 1] = '\0';		// make temp a string to convert
	if (str[0] == 'f')		// for nforces
		var->intgr = atoi(temp);
	if (str[0] == 'm')		// for mass
		var->dbl =  atof(temp);
	rewind(fp);
}

//reads numeric values on line line in file fp into vector structure vect
void vectorread(FILE *fp, fpos_t loc, struct vector *vect) {
	char c, lf, val1[32], *val2[32];
	double x, y;
	int i = 0, 1negflag, 2negflag;

	fsetpos(fp, loc);
	i = 0;
	while ((c = getc(fp)) != EOF) {
		if (c == '<')
			break;
	}
	if (c != '<') {
		puts("Error: No vector found: use format <x,y>");
		exit (EXIT_FAILURE);
	}
	if ((c = getc(fp)) == '-')
		1negflag = 1;
	else
		val1[i++] = c;
	while (isdigit(c = getc(fp)) || c == '.')
		val1[i++] = c;
	if (c != ',') {
		puts("Error reading vector: expecting ',' between values.");
		exit(EXIT_FAILURE);
	}
	val1[++i] = '\0';
	i = 0;
	if ((c = getc(fp)) == '-')
		2negflag = 1;
	else
		val2[i++] = c;
	while (isdigit(c = getc(fp)) || c == '.')
		val2[i++] = c;
	if (c != '>') {
		puts("Error reading vector: expecting '>'. use format <x,y>");
		exit(EXIT_FAILURE);
	}
	val2[++i] = '\0';
	vect->cmpnt.x = atof(val1);
	vect->cmpnt.y = atof(val2);
	//force times?
}

//reads nforces amount of forces into array of force forces[] from file fp
void sforces(FILE *fp, const int nforces, force forces[]) {
	fpos_t forcesloc = findstr("forces: %s\n	");
	char time[16], c;

	for (int i = 0; i < nforces, i++) {
		vectorread(fp, forcesloc, forces[i]);
		c = getc(fp);		// move forward 1/skip space
		while ((c = getc(fp)) != '\n' && i < 16)
			temp[i++] = c;
		forces[i].time = atoi(time);
		forcesloc = ftell(fp);
	}
}
//may be replaced by two vectorreads in main()
void swindow(FILE *fp, vector window[2]) {
	fpos_t winloc;

	findstr(fp, "window", &winloc);
	for (int i = 0; i < 2; i++)
		vectorread(fp, winloc, window[i]);
}
//may be replaced by a vectorread in main()
void stime(FILE *fp, vector *time) {
	fpos_t tloc;
	findstr(fp, "time", &tloc);
	vectorread(fp, tloc, time);
}
//applies nforces amount of forces in array of force forces[] onto vectors in objects[], turning a force into a position using the newton's second law
void applyforces(const unsigned int nforces, const force forces[], vector objects[], const double mass, const vector *time) {
	unsigned int frcct = 0;
	vector acc, vel;
	
	for (int i = time->x; i <= time->y; i++) {
		if (i == forces[frcct].time) {
			acc.x += forces[frcct].cmpnt.x / mass;
			acc.y += forces[frcct].cmpnt.y / mass;
			++frcct;
		}
		object[i].x += vel.x += acc.x;
		object[i].y += vel.y += acc.y;
	}
}
//creates a visual 2d array of size window plotting the path of objects[] over time period time
void writegraph(const vector objects[], const vector window[2], const vector *time) {

}
