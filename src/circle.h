
#include "BOV.h"

#ifndef CIRCLE_H
#define CIRCLE_H

// TODO use always the same and deport in a single .h
#define EPSILON 0.0001

struct Circle;
typedef struct Circle Circle;

struct Circle
{
  double center[2];
  double radius;
};

Circle *createCircle(double A[2], double B[2], double C[2]);

#endif