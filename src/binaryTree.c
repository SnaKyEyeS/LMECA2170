#include "binaryTree.h"

/*
 * Fetch the X coordinate of the breakpoint represented by node
 * 
 * Node: a non-leaf node
 * pointY: Y value of the point to
 * 
 * We need to fetch the parabola left and right and compute the break point by intersecting the 2 parabolas
 */
float getBpX(Node *node, float pointY)
{
  // 2*(pjy-ly)
  float al = 2 * (node->leftSite[1] - pointY);
  float ar = 2 * (node->rightSite[1] - pointY);

  if (al == 0)
  {
    return node->leftSite[0];
  }

  if (ar == 0)
  {
    return node->rightSite[0];
  }

  float A = 1.0 / al - 1.0 / ar;
  float B = -2 * (node->leftSite[0] / al - node->rightSite[0] / ar);
  float C = (node->leftSite[0] * node->leftSite[0] + node->leftSite[1] * node->leftSite[1] - pointY * pointY) / al - (node->rightSite[0] * node->rightSite[0] + node->rightSite[1] * node->rightSite[1] - pointY * pointY) / ar;

  float Delta = B * B - 4 * A * C;
  if (Delta < EPSILON)
  {
    return 0; //TODO check what to do
  }

  float sqrtDelta = sqrt(Delta);
  float x1 = (-B + sqrtDelta) / (2 * A);
  float x2 = (-B - sqrtDelta) / (2 * A);

  // intersection of parabola. The lower one will decide which point
  if (node->leftSite[0] < x1 && x1 < node->rightSite[0])
  {
    return x1;
  }
  else
  {
    return x2;
  }
}

/*
 * return the node representing the leaf representing the arc where point is above in the beachline
 * 
 * node: beachLine
 * point: point position
 */
Node *getArc(Node *node, float point[2])
{
  assert(node != NULL);
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
 * Fetch the Bp to the right to this node
 */
Node *getRightBpNode(Node *node)
{
  if (node == NULL)
  {
    return NULL;
  }

  if (node->Right != NULL && !node->Right->isLeaf)
  {
    Node *var = getLeftestBpNode(node->Right);
    if (var != NULL)
    {
      return var;
    }
  }

  Node *var = node;
  while (var->Root != NULL)
  {
    if (var->Root->Left == var)
      return var->Root;

    var = var->Root;
  }
  return NULL;
}

/*
 * Fetch the Bp to the Left to this node
 */
Node *getLeftBpNode(Node *node)
{
  if (node == NULL)
  {
    return NULL;
  }

  if (node->Left != NULL && !node->Left->isLeaf)
  {
    Node *var = getRightestBpNode(node->Left);
    if (var != NULL)
    {
      return var;
    }
  }

  Node *var = node;
  while (var->Root != NULL)
  {
    if (var->Root->Right == var)
      return var->Root;

    var = var->Root;
  }
  return NULL;
}

/*
 * Fetch the leftest bp Node included in node
 */
Node *getLeftestBpNode(Node *node)
{
  if (node == NULL)
    return NULL;
  // It's a leave
  if (node->Left == NULL)
    return NULL;
  if (node->Left->isLeaf)
    return node;

  Node *var = node->Left;
  while (!var->Left->isLeaf)
  {
    var = var->Left;
  }
  return var;
}

/*
 * Fetch the rightest bp Node included in node
 */
Node *getRightestBpNode(Node *node)
{
  if (node == NULL)
    return NULL;
  // It's a leave
  if (node->Right == NULL)
    return NULL;
  if (node->Right->isLeaf)
    return node;

  Node *var = node->Right;
  while (!var->Right->isLeaf)
  {
    var = var->Right;
  }
  return var;
}

/*
 * Duplicate the leaf
 */
Node *duplicateLeaf(Node *leaf)
{
  if (leaf == NULL)
  {
    return NULL;
  }
  if (!leaf->isLeaf)
  {
    // TODO: assert ?
    return NULL;
  }
  Node *n = malloc(sizeof(Node));
  n->isLeaf = true;
  n->arcPoint[0] = leaf->arcPoint[0];
  n->arcPoint[1] = leaf->arcPoint[1];
  n->Left = NULL;
  n->Right = NULL;
  n->ev = NULL;
  return n;
}

/*
 * Free a node/leaf
 */
void freeNode(Node *node)
{
  free(node);
}

/*
 *
 */
void printNode(Node *node)
{
  if (node == NULL)
    printf("Node is NULL\n");
  else
  {
    printf("NODE - (%p) (Left: %p, Right: %p) (Root: %p) (IsLeaf %d) (leftSite %f, %f) (rightSite %f,%f) (arcPoint %f,%f) \n", node, node->Left, node->Right, node->Root, node->isLeaf, node->leftSite[0], node->leftSite[1], node->rightSite[0], node->rightSite[1], node->arcPoint[0], node->arcPoint[1]);
  }
}

/*
 * Print a tree from its root
 */
void printAllTree(Node *root)
{
  if (root == NULL)
  {
    printf("Tree is empty \n");
  }
  else
  {
    printf("TREE\n");
    int nbElem = printTree(root, 0, 1);
    printf("The tree has %d elements\n", nbElem);
  }
}

/*
 * Will fill points[i*2+1] with the value of the beachline at point
 *  points[i*2] for i < n
 * 
 * Considering the sweep line at height y
 */
void drawBeachLine(float y, Node *root, coord *points, int n)
{
  if (root == NULL)
  {
    for (int i = 0; i < n; i++)
    {
      points[i][1] = 0;
    }
    return;
  }

  Node *var = getLeftArc(root);
  float xArc = var->arcPoint[0];
  float yArc = var->arcPoint[1];

  float bpX = 0;
  if (var != NULL)
  {
    if (var->Root != NULL)
    {
      bpX = getBpX(var->Root, y);
      var = var->Root; // First BP can be nulla
    }
    else
    {
      var = NULL;
    }
  }

  for (int i = 0; i < n; i++)
  {
    if (var != NULL)
    {
      if (points[i][0] > bpX)
      {
        xArc = var->rightSite[0];
        yArc = var->rightSite[1];
        var = getRightBpNode(var);
        if (var != NULL)
        {
          bpX = getBpX(var, y);
        }
      }
    }
    points[i][1] = parabola(xArc, yArc, y, points[i][0]);
  }
}

/*
 * Print a tree from a depth
 */
int printTree(Node *node, int depth, int id)
{
  // TODO print from depth to have the real tree but should check max depth first (quite slow)
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
    printf("|- LEAVE (%f,%f) (%d) %p %p\n", node->arcPoint[0], node->arcPoint[1], id, node, node->Root);
  }
  else
  {
    printf("|- BREAKPOINT (%f, %f), (%f, %f) (%d) %p %p\n", node->leftSite[0], node->leftSite[1], node->rightSite[0], node->rightSite[1], id, node, node->Root);
  }

  return 1 + printTree(node->Left, depth + 1, id * 10) + printTree(node->Right, depth + 1, id * 10 + 1);
}