#include "BOV.h"

#include "halfEdge.h"
#include <assert.h>

#ifndef BINARYTREE
#define BINARYTREE

struct Node;
typedef struct Node Node;

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
  double arcPoint[2];

  double leftSite[2];
  double rightSite[2];
  Event *ev;
  HalfEdge *he;

  bool isLeaf;
};

double getBpX(Node *node, double pointY);
Node *getArc(Node *node, double point[2]);
Node *getLeftArc(Node *node);
Node *getRightArc(Node *node);

Node *getRightestArc(Node *leaf);
Node *getLeftestArc(Node *leaf);

Node *getRightBpNode(Node *node);
Node *getLeftBpNode(Node *node);
Node *getRightestBpNode(Node *node);
Node *getLeftestBpNode(Node *node);

Node *duplicateLeaf(Node *leaf);

void boundingBoxBp(Node *root);

void freeNode(Node *node);
void freeTree(Node *node);

// Print

void printNode(Node *node);
void printAllTree(Node *root);
int printTree(Node *node, int depth, int id);

// Draw

void drawBeachLine(double y, Node *root, coord *points, int n);

#endif