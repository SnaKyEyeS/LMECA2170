#include "BOV.h"
#include <math.h>
#define EPSILON_PARABOLA 1e-5
#define STEP1 0.1
#define step2 0.05

#ifndef UTILS
#define UTILS

typedef float coord[2];

int comparefloats(const void *A, const void *B);
float parabola(float xArc, float yArc, float yLine, float x);

coord *linspace(float xmin, float xmax, int n);

int impulse(int old, int ne);

#endif