#include "physim.h"

//finds string str in file fp, returns location
fpos_t findstr(FILE *fp, char *str) {
	fpos_t strloc = NULL;
	char c;
	int nlct = 0, len = 0;

	while ((c = str[len++]) != '\0');
	while ((c = getc(fp)) != EOF) {
		if (c == '\n')
			nlct++;
		if (c == str[0]) {
			strloc = ftell(fp);
			for (int i = 1; i < len; i++) {
				if ((c = getc(fp)) == str[i])
					continue;
				else {
					strloc = NULL;
					break;
				}
			}
			if (i = len - 1 && strloc != NULL)
				break;
		}
	}
	return strloc;
}
//reads either double type for mass or integer type for nforces from file fp
void readval(FILE *fp, char *str, void *var) {
	int i = 0;
	char temp[64], c;
	fpos_t strloc = findstr(fp, str);
	if (strloc == NULL) {
		printf("Error: missing argument: %s", str);
		exit(EXIT_FAILURE);
	}
	fsetpos(fp, strloc);
	while (isdigit(c = getc(fp) || c == '.')
		 temp[i++] = c;
	temp[i + 1] = '\0';
	if (str[0] == 'm')
		*var = atof(temp);
	else if (str[0] == 'f')
		*var = atoi(temp);
}
//reads numeric values on line line in file fp into vector structure vect
void vectorread(FILE *fp, fpos_t loc, vector *vect) {
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
	fpos_t winloc = findstr(fp, "window: ");
	for (int i = 0; i < 2; i++)
		vectorread(fp, winloc, window[i]);
}
//may be replaced by a vectorread in main()
void stime(FILE *fp, vector *time) {
	fpos_t tloc = findstr(fp, "time: ");
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
