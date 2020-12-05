#include "utils.h"

/*
 * Compare the float representation of a point (x,y)
 * 
 * A: first point
 * B: second point
 */
int comparefloats(const void *A, const void *B)
{
  // TODO replace comparefloat by direct comparision to go faster
  float a = ((float *)A)[1];
  float b = ((float *)B)[1];

  if (a > b)
  {
    return 1;
  }
  else if (b > a)
  {
    return -1;
  }
  return 0;
}

/*
 * Get the y value of the parabola represented by xArc, yArc
 * 
 * xArc: x coordinate of the site point
 * yArc: y coordinate of the site point
 * yLine: Height of the line 
 * x: to where compute the y
 * 
 */
float parabola(float xArc, float yArc, float yLine, float x)
{
  // if the line is to close to the site point
  if (fabs(yArc - yLine) < EPSILON_PARABOLA)
  {
    return yLine;
  }
  else
  {
    double doublexArc = (double)xArc;
    double doubleyArc = (double)yArc;
    double doubleyLine = (double)yLine;
    double doublex = (double)x;
    double res = (doublex * doublex - 2 * doublexArc * doublex + doublexArc * doublexArc + doubleyArc * doubleyArc - doubleyLine * doubleyLine) / (2 * (doubleyArc - doubleyLine));
    return (float)res;
  }
}

/*
 * Return a pointer to a an array of 2*n elements of equaly spaces elements
 * xmin: lower value of the interval
 * xman: higher value of the interval
 * n: number of points
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

/*
 * Convert a continuous signal (key pressed down) to an impulse
 * 
 * old: previous value of the key
 * new: new value fetched by the key
 */
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
