#include "BOV.h"
#include <math.h>
#include <stdbool.h>
#define EPSILON_PARABOLA 1e-10
#define TEST_EPS 1e-3
#define PI 3.141592653

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

bool inBox(float x, float y, float box[2][2]);
bool lineCrossBox(float pointA[2], float pointB[2], float box[2][2]);

void key_callback_new(GLFWwindow *self, int key, int scancode, int action, int mods);

#endif