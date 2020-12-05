#include "circle.h"

/*
 *
 */
Circle *createCircle(float Af[2], float Bf[2], float Cf[2])
{

  double A[2] = {Af[0], Af[1]};
  double B[2] = {Bf[0], Bf[1]};
  double C[2] = {Cf[0], Cf[1]};

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
  double p1 = A[0] * (B[1] - C[1]) - A[1] * (B[0] - C[0]) + B[0] * C[1] - B[1] * C[0];
  double p2 = (A[0] * A[0] + A[1] * A[1]) * (C[1] - B[1]) + (B[0] * B[0] + B[1] * B[1]) * (A[1] - C[1]) + (C[0] * C[0] + C[1] * C[1]) * (B[1] - A[1]);
  double p3 = (A[0] * A[0] + A[1] * A[1]) * (B[0] - C[0]) + (B[0] * B[0] + B[1] * B[1]) * (C[0] - A[0]) + (C[0] * C[0] + C[1] * C[1]) * (A[0] - B[0]);
  double p4 = (A[0] * A[0] + A[1] * A[1]) * (C[0] * B[1] - B[0] * C[1]) + (B[0] * B[0] + B[1] * B[1]) * (A[0] * C[1] - C[0] * A[1]) + (C[0] * C[0] + C[1] * C[1]) * (B[0] * A[1] - A[0] * B[1]);

  double Delta = p2 * p2 + p3 * p3 - 4 * p1 * p4;

  if (p1 < EPSILON)
  {
    return NULL;
  }
  if (Delta < EPSILON)
  {
    return NULL;
  }

  Circle *c = malloc(sizeof(Circle));
  c->radius = (float)(sqrt(Delta) / (2 * p1));
  c->center[0] = (float)(-p2 / (2 * p1));
  c->center[1] = (float)(-p3 / (2 * p1));
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

/*
 *
 */
void printCircle(Circle *c)
{
  if (c == NULL)
  {
    printf("Circle is NULL \n");
    return;
  }
  printf("Circle cx: %f | cy: %f | radius: %f \n", c->center[0], c->center[1], c->radius);
}