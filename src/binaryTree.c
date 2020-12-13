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
  double doublepointY = (double)pointY;
  double doubleLx = (double)node->leftArc->arcPoint[0];
  double doubleLy = (double)node->leftArc->arcPoint[1];
  double doubleRx = (double)node->rightArc->arcPoint[0];
  double doubleRy = (double)node->rightArc->arcPoint[1];
  // 2*(pjy-ly)
  double al = 2 * (doubleLy - doublepointY);
  double ar = 2 * (doubleRy - doublepointY);

  if (fabs(al) < EPSILON)
  {
    return node->leftArc->arcPoint[0];
  }

  if (fabs(ar) < EPSILON)
  {
    return node->rightArc->arcPoint[0];
  }

  double A = 1.0 / al - 1.0 / ar;
  double B = 2 * (doubleRx / ar - doubleLx / al);
  double C = (doubleLx * doubleLx + doubleLy * doubleLy - doublepointY * doublepointY) / al - (doubleRx * doubleRx + doubleRy * doubleRy - doublepointY * doublepointY) / ar;

  double Delta = B * B - 4 * A * C;
  if (Delta < EPSILON)
  {
    return 0; //TODO check what to do
  }

  double sqrtDelta = sqrt(Delta);
  double x1 = (-B + sqrtDelta) / (2 * A);
  double x2 = (-B - sqrtDelta) / (2 * A);
  // intersection of parabola. The lower one will decide which point
  if (node->leftArc->arcPoint[0] < x1 && x1 < node->rightArc->arcPoint[0])
  {
    return (float)x1;
  }
  else
  {
    return (float)x2;
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
 * Free a recursively a Node and all its sub-node/leaf
 */
void freeTree(Node *node)
{
  if (node == NULL)
  {
    return;
  }

  if (!node->isLeaf)
  {
    freeTree(node->Left);
    freeTree(node->Right);
  }

  freeNode(node);
}

/*
 * Print a Node
 */
void printNode(Node *node)
{
  if (node == NULL)
    printf("Node is NULL\n");
  else
  {
    if (node->isLeaf)
      printf("LEAF - (%p) (Root: %p) (arcPoint %f,%f) \n", node, node->Root, node->arcPoint[0], node->arcPoint[1]);
    else
      printf("NODE - (%p) (Left: %p, Right: %p) (Root: %p) (IsLeaf %d) (leftSite %f, %f) (rightSite %f,%f)\n", node, node->Left, node->Right, node->Root, node->isLeaf, node->leftArc->arcPoint[0], node->leftArc->arcPoint[1], node->rightArc->arcPoint[0], node->rightArc->arcPoint[1]);
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
bool drawBeachLine(float y, Node *root, coord *points, int n, float minBox, float maxBox)
{

  if (root == NULL)
  {
    for (int i = 0; i < n; i++)
    {
      points[i][1] = minBox;
    }
    return false;
  }

  Node *var = root;
  bool beachlineBehindBox = true;
  while (!var->isLeaf)
  {
    var = var->Left;
  }
  float xArc = var->arcPoint[0];
  float yArc = var->arcPoint[1];
  float bpX = 0;

  if (var->rightBP != NULL)
  {
    bpX = getBpX(var->rightBP, y);
    var = var->rightBP->rightArc; // First BP can be null
    for (int i = 0; i < n; i++)
    {
      if (var->rightBP != NULL)
      {
        while (points[i][0] > bpX && var->rightBP != NULL)
        {
          xArc = var->arcPoint[0];
          yArc = var->arcPoint[1];
          if (var->rightBP != NULL)
          {
            bpX = getBpX(var->rightBP, y);
          }
          var = var->rightBP->rightArc;
        }
      }
      else if (points[i][0] > bpX)
      {
        xArc = var->arcPoint[0];
        yArc = var->arcPoint[1];
      }

      points[i][1] = parabola(xArc, yArc, y, points[i][0]);

      if (points[i][1] > maxBox)
      {
        points[i][1] = maxBox;
      }
      else
        beachlineBehindBox = false;

      if (points[i][1] < minBox)
      {
        points[i][1] = minBox;
      }
    }
  }

  else
  {
    for (int i = 0; i < n; i++)
    {
      points[i][1] = parabola(xArc, yArc, y, points[i][0]);
      if (points[i][1] > maxBox)
        points[i][1] = maxBox;
      else
        beachlineBehindBox = false;

      if (points[i][1] < minBox)
        points[i][1] = minBox;
    }
  }
  return beachlineBehindBox;
}

void boundingBoxBp(Node *root, float box[2][2])
{
  if (root == NULL)
    return;
  if (root->isLeaf)
    return;

  Node *var = root;
  while (!var->isLeaf)
  {
    var = var->Left;
  }

  var = var->rightBP;
  while (var != NULL)
  {
    float x = getBpX(var, 10 * box[1][1]);
    float y = parabola(var->leftArc->arcPoint[0], var->leftArc->arcPoint[1], 10 * box[1][1], x);

    if (var->he->v != NULL || var->he->Opposite->v != NULL)
    {
      float refX = 0;
      float refY = 0;
      if (var->he->v != NULL)
      {
        refX = var->he->v->coordinates[0];
        refY = var->he->v->coordinates[1];
      }
      else
      {
        refX = var->he->Opposite->v->coordinates[0];
        refY = var->he->Opposite->v->coordinates[1];
      }
      float point[2];
      point[0] = x;
      point[1] = y;
      Vertex *v = createVertex(point);

      if (var->he->v == NULL)
      {
        var->he->v = v;
        v->he = var->he;
      }
      else
      {
        var->he->Opposite->v = v;
        var->he->Opposite->v->he = var->he->Opposite;
      }

      /*
      if (inBox(refX, refY, box))
      {

        double angle = atan2(y - refY, x - refX);
        float r;

        if (angle > 0)
        {
          r = (box[1][1] - refY) / sin(angle);
        }
        else
        {
          r = (box[0][1] - refY) / sin(angle);
        }

        if (fabs(angle) < PI / 2)
        {
          float testr = (box[1][0] - refX) / cos(angle);
          if (testr < r && testr > 0)
            r = testr;
        }
        else
        {
          float testr = (box[0][0] - refX) / cos(angle);
          if (testr < r && testr > 0)
            r = testr;
        }

        float point[2];
        point[0] = refX + r * cos(angle);
        point[1] = refY + r * sin(angle);
        Vertex *v = createVertex(point);

        if (var->he->v == NULL)
        {
          var->he->v = v;
          v->he = var->he;
        }
        else if (var->he->Opposite->v == NULL)
        {
          var->he->Opposite->v = v;
          var->he->Opposite->v->he = var->he->Opposite;
        }
      }*/
    }
    else
    {
      Node *subVar = var;
      while (subVar != NULL)
      {
        if (subVar->he == var->he->Opposite)
        {
          break;
        }
        subVar = subVar->rightArc->rightBP;
      }
      if (subVar != NULL)
      {
        float points[2][2];
        points[0][0] = x;
        points[0][1] = y;
        points[1][0] = getBpX(subVar, 10 * box[1][1]);
        points[1][1] = parabola(subVar->rightArc->arcPoint[0], subVar->rightArc->arcPoint[1], y, 10 * box[1][1]);

        Vertex *v1 = createVertex(points[0]);
        Vertex *v2 = createVertex(points[1]);
        var->he->v = v1;
        v1->he = var->he;
        subVar->he->v = v2;
        v2->he = subVar->he;
      }
      else
      {
      }
    }

    var = var->rightArc->rightBP;
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
    printf("|- LEAVE (%.3f,%.3f) (%d) %p %p\n", node->arcPoint[0], node->arcPoint[1], id, node, node->Root);
  }
  else
  {
    printf("|- BREAKPOINT (%.3f, %.3f), (%.3f, %.3f) (%d) (Node: %p, Root: %p) (Left: %p, Right: %p) (lH:%d, rH:%d)\n", node->leftArc->arcPoint[0], node->leftArc->arcPoint[1], node->rightArc->arcPoint[0], node->rightArc->arcPoint[1], id, node, node->Root, node->Left, node->Right, node->leftHeight, node->rightHeight);
  }

  return 1 + printTree(node->Left, depth + 1, id * 10) + printTree(node->Right, depth + 1, id * 10 + 1);
}

/* 
 * Fetch the consturctiong He and store into points for drawing
 */
int drawConstructingHe(float y, Node *root, coord *points, float box[2][2])
{
  if (root == NULL)
  {
    return 0;
  }

  if (root->isLeaf)
  {
    return 0;
  }

  Node *var = root;
  while (!var->Left->isLeaf)
  {
    var = var->Left;
  }

  int n = 0;

  while (var != NULL)
  {
    if (var->he->v != NULL || var->he->Opposite->v != NULL)
    {
      points[n][0] = getBpX(var, y);
      points[n][1] = parabola(var->rightArc->arcPoint[0], var->rightArc->arcPoint[1], y, points[n][0]);
      if (var->he->v != NULL)
      {
        points[n + 1][0] = var->he->v->coordinates[0];
        points[n + 1][1] = var->he->v->coordinates[1];
      }
      else
      {
        points[n + 1][0] = var->he->Opposite->v->coordinates[0];
        points[n + 1][1] = var->he->Opposite->v->coordinates[1];
      }

      if (inBox(points[n][0], points[n][1], box) && inBox(points[n + 1][0], points[n + 1][1], box))
        n += 2;
      else if (inBox(points[n][0], points[n][1], box))
      {
        float refX = points[n][0];
        float refY = points[n][1];
        float x = points[n + 1][0];
        float y = points[n + 1][1];
        double angle = atan2(y - refY, x - refX);
        float r;

        if (angle > 0)
        {
          r = (box[1][1] - refY) / sin(angle);
        }
        else
        {
          r = (box[0][1] - refY) / sin(angle);
        }

        if (fabs(angle) < PI / 2)
        {
          float testr = (box[1][0] - refX) / cos(angle);
          if (testr < r)
            r = testr;
        }
        else
        {
          float testr = (box[0][0] - refX) / cos(angle);
          if (testr < r)
            r = testr;
        }
        points[n + 1][0] = refX + r * cos(angle);
        points[n + 1][1] = refY + r * sin(angle);
        n += 2;
      }
      else if (inBox(points[n + 1][0], points[n + 1][1], box))
      {
        float refX = points[n + 1][0];
        float refY = points[n + 1][1];
        float x = points[n][0];
        float y = points[n][1];
        double angle = atan2(y - refY, x - refX);
        float r;

        if (angle > 0)
        {
          r = (box[1][1] - refY) / sin(angle);
        }
        else
        {
          r = (box[0][1] - refY) / sin(angle);
        }

        if (fabs(angle) < PI / 2)
        {
          float testr = (box[1][0] - refX) / cos(angle);
          if (testr < r)
            r = testr;
        }
        else
        {
          float testr = (box[0][0] - refX) / cos(angle);
          if (testr < r)
            r = testr;
        }
        points[n][0] = refX + r * cos(angle);
        points[n][1] = refY + r * sin(angle);
        n += 2;
      }
    }
    else
    {
      Node *subVar = var;
      while (subVar != NULL)
      {
        if (subVar->he == var->he->Opposite)
        {
          break;
        }
        subVar = subVar->rightArc->rightBP;
      }
      if (subVar != NULL)
      {
        points[n][0] = getBpX(var, y);
        points[n][1] = parabola(var->rightArc->arcPoint[0], var->rightArc->arcPoint[1], y, points[n][0]);
        points[n + 1][0] = getBpX(subVar, y);
        points[n + 1][1] = parabola(subVar->rightArc->arcPoint[0], subVar->rightArc->arcPoint[1], y, points[n + 1][0]);

        if (inBox(points[n][0], points[n][1], box) && inBox(points[n + 1][0], points[n + 1][1], box))
        {

          n += 2;
        }
        else if (inBox(points[n][0], points[n][1], box))
        {
          float refX = points[n][0];
          float refY = points[n][1];
          float x = points[n + 1][0];
          float y = points[n + 1][1];
          double angle = atan2(y - refY, x - refX);
          float r;

          if (angle > 0)
          {
            r = (box[1][1] - refY) / sin(angle);
          }
          else
          {
            r = (box[0][1] - refY) / sin(angle);
          }

          if (fabs(angle) < PI / 2)
          {
            float testr = (box[1][0] - refX) / cos(angle);
            if (testr < r)
              r = testr;
          }
          else
          {
            float testr = (box[0][0] - refX) / cos(angle);
            if (testr < r)
              r = testr;
          }
          points[n + 1][0] = refX + r * cos(angle);
          points[n + 1][1] = refY + r * sin(angle);
          n += 2;
        }
        else if (inBox(points[n + 1][0], points[n + 1][1], box))
        {
          float refX = points[n + 1][0];
          float refY = points[n + 1][1];
          float x = points[n][0];
          float y = points[n][1];
          double angle = atan2(y - refY, x - refX);
          float r;

          if (angle > 0)
          {
            r = (box[1][1] - refY) / sin(angle);
          }
          else
          {
            r = (box[0][1] - refY) / sin(angle);
          }

          if (fabs(angle) < PI / 2)
          {
            float testr = (box[1][0] - refX) / cos(angle);
            if (testr < r)
              r = testr;
          }
          else
          {
            float testr = (box[0][0] - refX) / cos(angle);
            if (testr < r)
              r = testr;
          }
          points[n][0] = refX + r * cos(angle);
          points[n][1] = refY + r * sin(angle);
          n += 2;
        }
      }
      else
      {
      }
    }

    var = var->rightArc->rightBP;
  }

  return n;
}