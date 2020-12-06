#include "BOV.h"
#include <math.h>
#define EPSILON_PARABOLA 1e-10

#ifndef UTILS
#define UTILS

enum TYPE_ANIM
{
  NO_ANIM,
  MANUAL,
  STEP_ANIM,
  SWEEP_ANIM,
};

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef float coord[2];

int comparefloats(const void *A, const void *B);
float parabola(float xArc, float yArc, float yLine, float x);

coord *linspace(float xmin, float xmax, int n);
void linspaceRealloc(coord *points, float xmin, float xmax, int n);

int impulse(int old, int ne);

void key_callback_new(GLFWwindow *self, int key, int scancode, int action, int mods);

#endif