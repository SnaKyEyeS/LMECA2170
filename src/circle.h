
#include "BOV.h"
#include <math.h>
#ifndef CIRCLE_H
#define CIRCLE_H

// TODO use always the same and deport in a single .h
#define EPSILON 1e-9

struct Circle;
typedef struct Circle Circle;

struct Circle
{
  float center[2];
  float radius;
};

Circle *createCircle(float A[2], float B[2], float C[2]);

void freeCircle(Circle *c);

#endif