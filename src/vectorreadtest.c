#include <stdio.h>
#include "physim.c"

int main(int argc, char *argv[]) {
	FILE *fin = fopen(argv[1], "r");
	puts("open");	
	struct vector time;
	puts("struct time");
	fpos_t tloc;
	findstr(fin, "time", &tloc);
	puts("findstr");
	vectorread(fin, &tloc, &time);
	printf("<%f,%f>\n", time.x, time.y);
	return 0;
}	
