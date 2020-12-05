#include "circle.h"

/*
 * Return the circle by the 3 points Af, Bf and C
 */
Circle *createCircle(float Af[2], float Bf[2], float Cf[2])
{

  // TODO check if faster to compare double or float
  if (fabs(Af[0] - Bf[0]) < EPSILON && fabs(Af[1] - Bf[1]) < EPSILON)
  {
    return NULL;
  }

  if (fabs(Cf[0] - Bf[0]) < EPSILON && fabs(Cf[1] - Bf[1]) < EPSILON)
  {
    return NULL;
  }

  if (fabs(Af[0] - Cf[0]) < EPSILON && fabs(Af[1] - Cf[1]) < EPSILON)
  {
    return NULL;
  }

  double A[2] = {Af[0], Af[1]};
  double B[2] = {Bf[0], Bf[1]};
  double C[2] = {Cf[0], Cf[1]};

  // TODO check colinearity use fast algo ?

  // http://www.ambrsoft.com/trigocalc/circle3d.htm
  double Amult = (A[0] * A[0] + A[1] * A[1]);
  double Bmult = (B[0] * B[0] + B[1] * B[1]);
  double Cmult = (C[0] * C[0] + C[1] * C[1]);
  double A0B1 = A[0] * B[1];
  double A1B0 = A[1] * B[0];
  double A0C1 = A[0] * C[1];
  double A1C0 = A[1] * C[0];
  double B0C1 = B[0] * C[1];
  double B1C0 = B[1] * C[0];
  double p1 = A0B1 - A0C1 - A1B0 + A1C0 + B0C1 - B1C0;
  double p2 = Amult * (C[1] - B[1]) + Bmult * (A[1] - C[1]) + Cmult * (B[1] - A[1]);
  double p3 = Amult * (B[0] - C[0]) + Bmult * (C[0] - A[0]) + Cmult * (A[0] - B[0]);
  double p4 = Amult * (B1C0 - B0C1) + Bmult * (A0C1 - A1C0) + Cmult * (A1B0 - A0B1);

  // p1 should be positive on the link given
  if (p1 < EPSILON)
  {
    return NULL;
  }

  double Delta = p2 * p2 + p3 * p3 - 4 * p1 * p4;

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