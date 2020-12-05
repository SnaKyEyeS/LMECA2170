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
  //Common
  Node *Root;
  bool isLeaf;

  // Leaf
  float arcPoint[2];
  Event *ev;
  Node *leftBP;
  Node *rightBP;

  // Node
  Node *rightArc;
  Node *leftArc;
  HalfEdge *he;
  Node *Left;
  Node *Right;
  int leftHeight;
  int rightHeight;
};

float getBpX(Node *node, float pointY);
Node *getArc(Node *node, float point[2]);
Node *duplicateLeaf(Node *leaf);

void boundingBoxBp(Node *root);

void freeNode(Node *node);
void freeTree(Node *node);

// Print

void printNode(Node *node);
void printAllTree(Node *root);
int printTree(Node *node, int depth, int id);

// Draw

void drawBeachLine(float y, Node *root, coord *points, int n, float minBox, float maxBox);

#endif