#include "circle.h"

/*
 *
 */
Circle *createCircle(float A[2], float B[2], float C[2])
{
  if (fabs(A[0] - B[0]) < EPSILON && fabs(A[1] - B[1]) < EPSILON)
  {
    return NULL;
  }

  if (fabs(C[0] - B[0]) < EPSILON && fabs(C[1] - B[1]) < EPSILON)
  {
    return NULL;
  }

  if (fabs(A[0] - C[0]) < EPSILON && fabs(A[1] - C[1]) < EPSILON)
  {
    return NULL;
  }

  // TODO check colinearity use fast algo ?

  // http://www.ambrsoft.com/trigocalc/circle3d.htm
  // Todo store same computation to be faster and check
  float p1 = A[0] * (B[1] - C[1]) - A[1] * (B[0] - C[0]) + B[0] * C[1] - B[1] * C[0];
  float p2 = (A[0] * A[0] + A[1] * A[1]) * (C[1] - B[1]) + (B[0] * B[0] + B[1] * B[1]) * (A[1] - C[1]) + (C[0] * C[0] + C[1] * C[1]) * (B[1] - A[1]);
  float p3 = (A[0] * A[0] + A[1] * A[1]) * (B[0] - C[0]) + (B[0] * B[0] + B[1] * B[1]) * (C[0] - A[0]) + (C[0] * C[0] + C[1] * C[1]) * (A[0] - B[0]);
  float p4 = (A[0] * A[0] + A[1] * A[1]) * (C[0] * B[1] - B[0] * C[1]) + (B[0] * B[0] + B[1] * B[1]) * (A[0] * C[1] - C[0] * A[1]) + (C[0] * C[0] + C[1] * C[1]) * (B[0] * A[1] - A[0] * B[1]);

  float Delta = p2 * p2 + p3 * p3 - 4 * p1 * p4;

  if (Delta < EPSILON)
  {
    return NULL;
  }

  Circle *c = malloc(sizeof(Circle));
  c->radius = sqrt(Delta / (4 * p1 * p1));
  c->center[0] = -p2 / (2 * p1);
  c->center[1] = -p3 / (2 * p1);
  return c;
}

/*
 *
 */
void freeCircle(Circle *c)
{
  if (c != NULL)
    free(c);
}