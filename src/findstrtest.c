#include <stdlib.h>
#include <stdio.h>
#include "physim.c"

int main(int argc, char *argv[]) {
	FILE *fp = fopen(argv[1], "r");
	fpos_t strloc;
	char c;
	if ((findstr(fp, argv[2], &strloc)) != 0) {
		puts("\nnotfound");
		exit(EXIT_FAILURE);
	}
	fsetpos(fp, &strloc);
	c = fgetc(fp);
	do {
		if (c == '#') {
			while (c != '\n')
				c = fgetc(fp);
		}
			
		putchar(c);
	} while ((c = fgetc(fp)) != EOF);
	return 0;
}
