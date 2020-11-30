#include "BOV.h"
#include <math.h>
#define EPSILON_PARABOLA 1e-10

#ifndef UTILS
#define UTILS

typedef double coord[2];
typedef float coordFloat[2];

int comparedoubles(const void *A, const void *B);
double parabola(double xArc, double yArc, double yLine, double x);

coord *linspace(double xmin, double xmax, int n);

int impulse(int old, int ne);

void convertFloat2Double(coordFloat *pointsFloat, coord *pointsDouble, int n);
void convertDouble2Float(coord *pointsDouble, coordFloat *pointsFloat, int n);

#endif