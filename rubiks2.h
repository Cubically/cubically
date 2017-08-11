#pragma once

enum { UP, LEFT, FRONT, RIGHT, BACK, DOWN };

//extern int cube[6][3][3];
extern int *cube;
extern int CUBESIZE;

#define CUBE(x,y,z) cube[((x)*CUBESIZE*CUBESIZE) + ((y)*CUBESIZE) + (z)]

extern void initcube(void), printcube(void), turncube(int, int, int), swap(int*, int*), rotateface(int, int);
extern char *color(int);
extern int issolved(void);

#ifdef VISUALIZER
# define dbg stdout
#else
# define dbg stderr
#endif
