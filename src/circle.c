#include "circle.h"

/*
 *
 */
Circle *createCircle(double A[2], double B[2], double C[2])
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
  // Todo store same computation to be faster
  double p1 = A[0] * (B[1] - C[1]) - A[1] * (B[0] - C[0]) + B[0] * C[1] - B[1] * C[0];
  double p2 = (A[0] * A[0] + A[1] * A[1]) * (C[1] - B[1]) + (B[0] * B[0] + B[1] * B[1]) * (A[1] - C[1]) + (C[0] * C[0] + C[1] * C[1]) * (B[1] - A[1]);
  double p3 = (A[0] * A[0] + A[1] * A[1]) * (B[0] - C[0]) + (B[0] * B[0] + B[1] * B[1]) * (C[0] - A[0]) + (C[0] * C[0] + C[1] * C[1]) * (A[0] - B[0]);
  double p4 = (A[0] * A[0] + A[1] * A[1]) * (C[0] * B[1] - B[0] * C[1]) + (B[0] * B[0] + B[1] * B[1]) * (A[0] * C[1] - C[0] * A[1]) + (C[0] * C[0] + C[1] * C[1]) * (B[0] * A[1] - A[0] * B[1]);
}