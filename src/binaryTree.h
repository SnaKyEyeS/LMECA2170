#include "BOV.h"

#include "halfEdge.h"

#ifndef BINARYTREE
#define BINARYTREE

struct Node;
typedef struct Node Node;
typedef struct Parabola Parabola;

#include "events.h"

// We are a leaf when Left = NULL and Right = NULL
struct Node
{
  Node *Left;
  Node *Right;
  double bp[2]; // Break point
  Parabola *Lparabola;
  Parabola *Rparabola;
  Event *ev;
  HalfEdge *he;
};

struct Parabola
{
};

#endif