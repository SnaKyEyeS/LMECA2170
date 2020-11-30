#include "utils.h"

/*
 * Compare the double representation of a point (x,y)
 * 
 * A: first point
 * B: second point
 */
int comparedoubles(const void *A, const void *B)
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

/*
 * get the y value of the parabola
 * 
 */
double parabola(double xArc, double yArc, double yLine, double x)
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
coord *linspace(double xmin, double xmax, int n)
{
  coord *points = malloc(sizeof(coord) * n);

  double step = (xmax - xmin) / (n - 1);
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

/*
 * Convert an array of float to double
 */
void convertFloat2Double(coordFloat *pointsFloat, coord *pointsDouble, int n)
{
  for (int i = 0; i < n; i++)
  {
    char buf[42];
    sprintf(buf, "%.7g", pointsFloat[i][0]); // round to 7 decimal digits
    pointsDouble[i][0] = atof(buf);
    sprintf(buf, "%.7g", pointsFloat[i][1]); // round to 7 decimal digits
    pointsDouble[i][1] = atof(buf);
  }
}

/*
 * Convert an arry of double to float (7digit)
 */
void convertDouble2Float(coord *pointsDouble, coordFloat *pointsFloat, int n)
{
  for (int i = 0; i < n; i++)
  {
    pointsFloat[i][0] = (float)pointsDouble[i][0];
    pointsFloat[i][1] = (float)pointsDouble[i][1];
  }
}