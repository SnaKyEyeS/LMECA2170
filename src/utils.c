#include "utils.h"

/*
 * Compare the double representation of a point (x,y)
 * 
 * A: first point
 * B: second point
 */
int compareDoubles(const void *A, const void *B)
{
  double *a = (double *)A;
  double *b = (double *)B;
  if (a[1] > b[1])
  {
    return 1;
  }
  else if (b[1] > a[1])
  {
    return -1;
  }
  return 0;
}
