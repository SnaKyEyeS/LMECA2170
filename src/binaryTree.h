#include "BOV.h"

#include "halfEdge.h"
#include <assert.h>

#ifndef BINARYTREE
#define BINARYTREE

struct Node;
typedef struct Node Node;
typedef struct Parabola Parabola;

#include "events.h"
#include "circle.h"
#include <math.h>
#include <stdbool.h>
#include "utils.h"

// We are a leaf when Left = NULL and Right = NULL
struct Node
{
  Node *Left;
  Node *Right;
  Node *Root;
  float arcPoint[2];

  float leftSite[2];
  float rightSite[2];
  Event *ev;
  HalfEdge *he;

  bool isLeaf;
};

float getBpX(Node *node, float pointY);
Node *getArc(Node *node, float point[2]);
Node *getLeftArc(Node *node);
Node *getRightArc(Node *node);

Node *getRightestArc(Node *leaf);
Node *getLeftestArc(Node *leaf);

Node *getRightBpNode(Node *node);
Node *getLeftestBpNode(Node *node);

Node *duplicateLeaf(Node *leaf);

void freeNode(Node *node);

// Print

void printNode(Node *node);
void printAllTree(Node *root);
int printTree(Node *node, int depth, int id);

// Draw

void drawBeachLine(float y, Node *root, coord *points, int n);

#endif