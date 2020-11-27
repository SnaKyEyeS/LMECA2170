#include "utils.h"

/*
 * Compare the float representation of a point (x,y)
 * 
 * A: first point
 * B: second point
 */
int comparefloats(const void *A, const void *B)
{
  float *a = (float *)A;
  float *b = (float *)B;
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

/*
 * get the y value of the parabola
 * 
 */
float parabola(float xArc, float yArc, float yLine, float x)
{
  if (fabs(yArc - yLine) < EPSILON_PARABOLA)
  {
    return yLine;
  }
  else
  {
    return (x * x - 2 * xArc * x + xArc * xArc + yArc * yArc - yLine * yLine) / (2 * (yArc - yLine));
  }
}

/*
 * Retrun a pointer to a an array of 2*n elements
 * the x elements will be linpsace between xmin and x_max
 * 
 */
coord *linspace(float xmin, float xmax, int n)
{
  coord *points = malloc(sizeof(coord) * n);

  float step = (xmax - xmin) / (n - 1);
  for (int i = 0; i < n; i++)
  {
    points[i][0] = xmin + i * step;
    points[i][1] = 0;
  }

  points[n - 1][0] = xmax;

  return points;
}

int impulse(int old, int ne)
{
  if (old == 1)
  {
    return 0;
  }
  if (ne == 0)
  {
    return -1;
  }
  if (ne == 1 && old == -1)
  {
    return 1;
  }
  return 0;
}
