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
 * return the node representing the leaf representing the arc where point is above in the beachline
 * 
 * node: beachLine
 * point: point position
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

/*
 * Retrieve the leftest arc from a Node
 *
 */
Node *getLeftArc(Node *node)
{
  if (node->isLeaf)
  {
    return node;
  }
  return getLeftArc(node->Left);
}

/*
 * Retrieve the rightest arc from a Node
 *
 */
Node *getRightArc(Node *node)
{
  if (node->isLeaf)
  {
    return node;
  }
  return getRightArc(node->Right);
}

/*
 * Return the next leaf representing the arc to the left of the actual leaf
 */
Node *getLeftestArc(Node *leaf)
{
  if (leaf->Root == NULL)
  {
    return NULL;
  }
  Node *var = leaf;
  while (var != NULL)
  {
    if (var->Root->Right == var)
    {
      return getRightArc(var->Root->Left);
    }
    var = var->Root;
  }
  return NULL;
}

/*
 * Return the next leaf representing the arc to the right of the actual leaf
 */
Node *getRightestArc(Node *leaf)
{
  if (leaf->Root == NULL)
  {
    return NULL;
  }
  Node *var = leaf;
  while (var != NULL)
  {
    if (var->Root->Left == var)
    {
      return getLeftArc(var->Root->Right);
    }
    var = var->Root;
  }
  return NULL;
}

/*
 * Free a node/leaf
 */
void freeNode(Node *node)
{
  free(node);
}

/*
 * Print a tree from its root
 */
void printAllTree(Node *root)
{
  if (root == NULL)
  {
    printf("Tree is empty");
  }
  else
  {
    int nbElem = printTree(root, 0, 1);
    printf("The tree has %d elements", nbElem);
  }
}

/*
 * Print a tree from a depth
 */
int printTree(Node *node, int depth, int id)
{
  if (node == NULL)
  {
    return 0;
  }
  for (int i = 0; i < depth; i++)
  {
    printf("  ");
  }

  if (node->isLeaf)
  {
    printf("|- LEAVE (%f,%f)", node->arcPoint[0], node->arcPoint[1]);
  }
  else
  {
    printf("|- BREAKPOINT (%f, %f), (%f, %f) (%d)", node->leftSite[0], node->leftSite[1], node->rightSite[0], node->rightSite[1], id);
  }

  return 1 + printTree(node->Left, depth + 1, id << 1 + 1) + printTree(node->Right, depth + 1, id << 1);
}