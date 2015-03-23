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
//void display(struct vector * objects[]);
//void applyforce(struct force * forces[], struct vector * objects[]);

struct vector {
	int x;
	int y;
};

struct force {
	struct vector cmpnt;
	int time;
};

int main(int argc, char *argv[]) {
	if (argc != 5) {
		printf("usage: %s mass-of-object num-of-frames x-force y-force\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int frames = atoi(argv[2]);
	int mass = atoi(argv[1]);
	int xacc = 0, yacc = 0, xvel = 0, yvel = 0, xpos = 0, ypos = 0;
	struct vector forces = {atoi(argv[3]), atoi(argv[4])};
	struct vector object = {0, 0};
	for (int i = 0; i < frames; i++) {
		object.x = xpos += xvel += xacc = round(forces.x / mass);
		object.y = ypos += yvel += yacc = forces.y / mass;
		printf("%d: Object\n  Acceleration:\n    x: %2d y: %2d\n  Velocity:\n    x: %2d y: %2d\n  Position:\n    x: %2d y: %2d\n\n", i, xacc, yacc, xvel, yvel, xpos, ypos);
	}
	puts("done");
	return 0;
}
