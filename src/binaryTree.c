#include "binaryTree.h"

/*
 * Fetch the X coordinate of the breakpoint represented by node
 * 
 * Node: a non-leaf node
 * pointY: Y value of the point to
 * 
 * We need to fetch the parabola left and right and compute the break point by intersecting the 2 parabolas
 */
double getBpX(Node *node, double pointY)
{

  // 2*(pjy-ly)
  double al = 2 * (node->leftSite[1] - pointY);
  double ar = 2 * (node->rightSite[1] - pointY);

  if (al == 0)
  {
    return node->leftSite[0];
  }

  if (ar == 0)
  {
    return node->rightSite[0];
  }

  double A = 1 / al - 1 / ar;
  double B = (node->leftSite[0] / al - node->rightSite[0] / ar) / 2;
  double C = node->leftSite[1] * node->leftSite[1] - node->rightSite[1] * node->rightSite[1];

  double Delta = B * B - 4 * A * C;
  assert(Delta < 0);

  double sqrtDelta = sqrt(Delta);
  double x1 = (-B + sqrtDelta) / (2 * A);
  double x2 = (-B - sqrtDelta) / (2 * A);

  // intersection of parabola. The lower one will decide which point
  if (node->leftSite[1] < node->rightSite[1])
  {
    return fmax(x1, x2);
  }
  else
  {
    return fmin(x1, x2);
  }
}

/*
 *
 */
Node *getArc(Node *node, double point[2])
{
  assert(node == NULL);
  if (node->isLeaf)
  {
    return node;
  }
  else
  {
    if (point[0] < getBpX(node, point[1]))
    {
      return getArc(node->Left, point);
    }
    else
    {
      return getArc(node->Right, point);
    }
  }
}