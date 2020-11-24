#include "BOV.h"

#include "halfEdge.h"
#include <math.h>
#include <assert.h>

#ifndef BINARYTREE
#define BINARYTREE

struct Node;
typedef struct Node Node;
typedef struct Parabola Parabola;

#include "events.h"
#include <stdbool.h>

// We are a leaf when Left = NULL and Right = NULL
struct Node
{
  Node *Left;
  Node *Right;
  Node *Root;
  double arcPoint[2];

  double leftSite[2];
  double rightSite[2];
  Event *ev;
  HalfEdge *he;

  bool isLeaf;
};

double getBpX(Node *node, double pointY);
Node *getArc(Node *node, double point[2]);

#endif