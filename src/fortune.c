#include "fortune.h"

/*
 * Initialise the datastructure for the Fortune algorithm
 */
FortuneStruct *initFortune(coord *points, int n)
{
  FortuneStruct *data = malloc(sizeof(FortuneStruct));
  data->Voronoid = InitEmptyPolygonMesh();
  // Sort
  qsort(points, n, sizeof(float) * 2, comparefloats);

  initFaces(data->Voronoid, points, n);
  data->Q = LoadSortedEventQueue(points, n, data->Voronoid->faces);
  data->beachLine = NULL;
  return data;
}

/*
 * Apply the fortune algorithm to the datastructure up to a yLine
 */
void fortuneAlgo(FortuneStruct *data, float yLine)
{
  while (data->Q->size > 0 && data->Q->es[0]->coordinates[1] < yLine)
  {
    //printf("New process \n");
    ProcessEvent(data);
  }
}

/*
 * Process the first event from the Queue
 */
void ProcessEvent(FortuneStruct *data)
{
  if (data->Q == NULL)
    return;
  if (data->Q->size == 0) // TODO update
    return;
  Event *e = popQueue(data->Q);

  if (e == NULL)
    return; // TODO should assert
  if (e->isValid)
  {
    if (e->type == SITE)
      ProcessSite(data, e);
    else
      ProcessCircle(data, e);
  }
  freeEvent(e);
}

/*
 * Process a site event
 * 
 * beachline: binaryTree representing the beachline
 * e: event to process
 * Q: queue of events
 */
void ProcessSite(FortuneStruct *data, Event *e)
{
  Node *newLeaf = malloc(sizeof(Node));
  newLeaf->Left = NULL;
  newLeaf->Right = NULL;
  newLeaf->isLeaf = true;
  newLeaf->ev = NULL;

  newLeaf->arcPoint[0] = e->coordinates[0];
  newLeaf->arcPoint[1] = e->coordinates[1];

  if (data->beachLine == NULL)
  {
    newLeaf->leftBP = NULL;
    newLeaf->rightBP = NULL;
    newLeaf->Root = NULL;
    data->beachLine = newLeaf;
    return;
  }

  Node *arc = getArc(data->beachLine, e->coordinates);
  //      old
  //       | -> size can be left or right
  //     Inner2
  //      /   \
  //   Inner1  arc
  //  /    \
  //arc  newLeaf
  //

  Node *Inner1 = malloc(sizeof(Node));
  Node *Inner2 = malloc(sizeof(Node));

  Node *Left = NULL;
  Node *Right = NULL;
  Node *var = arc;

  Inner1->isLeaf = false;
  Inner2->isLeaf = false;
  //Update Root
  if (arc->Root != NULL)
  {
    while ((Left == NULL || Right == NULL) && var->Root != NULL)
    {
      if (var->Root->Left == var)
      {
        if (Right == NULL)
        {
          Right = var->Root;
        }
      }
      else
      {
        if (Left == NULL)
        {
          Left = var->Root;
        }
      }
      var = var->Root;
    }

    if (arc->Root->Left == arc)
    {
      arc->Root->Left = Inner1;
    }
    else
    {
      arc->Root->Right = Inner1;
    }
  }
  else
  {
    data->beachLine = Inner1;
  }

  Inner1->leftHeight = 2;
  Inner1->rightHeight = 1;
  Inner2->leftHeight = 1;
  Inner2->rightHeight = 1;

  Inner1->Root = arc->Root;
  arc->Root = Inner2;
  newLeaf->Root = Inner2;
  Inner2->Root = Inner1;

  // Update Left Right
  Node *duplArc = duplicateLeaf(arc);
  Inner2->Left = arc;
  Inner2->Right = newLeaf;
  Inner1->Left = Inner2;
  Inner1->Right = duplArc;
  duplArc->Root = Inner1;

  // Update Arc and BP
  Node *LeftBP = arc->leftBP, *RightBp = arc->rightBP;

  arc->leftBP = LeftBP;
  arc->rightBP = Inner2;
  newLeaf->leftBP = Inner2;
  newLeaf->rightBP = Inner1;
  duplArc->leftBP = Inner1;
  duplArc->rightBP = RightBp;

  Inner2->leftArc = arc;
  Inner2->rightArc = newLeaf;
  Inner1->leftArc = newLeaf;
  Inner1->rightArc = Inner1->Right;
  if (LeftBP != NULL)
    LeftBP->rightArc = arc;
  if (RightBp != NULL)
    RightBp->leftArc = duplArc;

  // update Height
  Node *var2 = Inner1;
  while (var2->Root != NULL)
  {
    if (var2->Root->Left == var2)
    {
      int m = MAX(var2->leftHeight + 1, var2->rightHeight + 1);
      if (m == var2->Root->leftHeight)
        break;
      var2->Root->leftHeight = m;
      var2 = var2->Root;
    }
    else
    {
      int m = MAX(var2->leftHeight + 1, var2->rightHeight + 1);
      if (m == var2->Root->rightHeight)
        break;
      var2->Root->rightHeight = m;
      var2 = var2->Root;
    }

    int diff = var2->leftHeight - var2->rightHeight;

    // Not opti when weird shape, to FIX
    if (diff < -1)
    {
      Node *C = var2, *B = var2->Right, *oldRoot = var2->Root;
      C->Right = B->Left;
      if (C->Right != NULL)
      {
        C->Right->Root = C;

        if (C->Right->isLeaf)
          C->rightHeight = 1;
        else
          C->rightHeight = MAX(C->Right->leftHeight, C->Right->rightHeight) + 1;
      }
      else
      {
        C->rightHeight = 1;
      }
      if (C->Left->isLeaf)
        C->leftHeight = 1;
      else
        C->leftHeight = MAX(C->Left->leftHeight, C->Left->rightHeight) + 1;

      B->Left = C;
      C->Root = B;

      B->leftHeight = MAX(C->leftHeight, C->rightHeight) + 1;
      if (B->Right->isLeaf)
        B->rightHeight = 1;
      else
        B->rightHeight = MAX(B->Right->leftHeight, B->Right->rightHeight) + 1;

      if (oldRoot == NULL)
      {
        data->beachLine = B;
        B->Root = NULL;
      }
      else
      {
        if (oldRoot->Left == C)
        {
          oldRoot->Left = B;
        }
        else
        {
          oldRoot->Right = B;
        }
        B->Root = oldRoot;
      }

      var2 = B;
    }
    else if (diff > 1)
    {
      //Invert rotation
      Node *C = var2, *B = var2->Left, *oldRoot = var2->Root;
      C->Left = B->Right;
      if (C->Right != NULL)
      {
        C->Left->Root = C;
        if (C->Left->isLeaf)
          C->leftHeight = 1;
        else
          C->leftHeight = MAX(C->Left->leftHeight, C->Left->rightHeight) + 1;
      }
      else
      {
        C->leftHeight = 1;
      }
      if (C->Right->isLeaf)
        C->rightHeight = 1;
      else
        C->rightHeight = MAX(C->Right->leftHeight, C->Right->rightHeight) + 1;

      B->Right = C;
      C->Root = B;

      B->rightHeight = MAX(C->leftHeight, C->rightHeight) + 1;
      if (B->Left->isLeaf)
        B->leftHeight = 1;
      else
        B->leftHeight = MAX(B->Left->leftHeight, B->Left->rightHeight) + 1;

      if (oldRoot == NULL)
      {
        data->beachLine = B;
        B->Root = NULL;
      }
      else
      {
        if (oldRoot->Left == C)
        {
          oldRoot->Left = B;
        }
        else
        {
          oldRoot->Right = B;
        }
        B->Root = oldRoot;
      }

      var2 = B;
    }
  }

  // end of update Height

  //TODO: rebalance

  Circle *circle = NULL;
  if (Left != NULL)
  {
    circle = createCircle(arc->arcPoint, newLeaf->arcPoint, Left->leftArc->arcPoint);
  }

  /*
   * TODO check if working with double is better (epsilon machine 4 float) 
   * convert double -> float can lead to circle "valid" but y_c + r < e->y (for double but not for float)
   */

  if (circle != NULL && (circle->center[1] + circle->radius >= e->coordinates[1]))
  {
    if (arc->ev != NULL)
    {
      if (arc->ev->coordinates[1] > circle->center[1] + circle->radius)
      {
        arc->ev->isValid = false;
        arc->ev = AddPoint(data->Q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
        arc->ev->node = arc;
        arc->ev->circle = circle;
      }
      else
      {
        freeCircle(circle);
      }
    }
    else
    {
      arc->ev = AddPoint(data->Q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
      arc->ev->node = arc;
      arc->ev->circle = circle;
    }
  }
  else
  {
    freeCircle(circle);
  }

  if (Right != NULL)
  {
    circle = createCircle(newLeaf->arcPoint, arc->arcPoint, Right->rightArc->arcPoint);
  }
  else
  {
    circle = NULL;
  }

  if (circle != NULL && (circle->center[1] + circle->radius >= e->coordinates[1]))
  {
    if (duplArc->ev != NULL)
    {
      // if we need to replace the circle
      if (duplArc->ev->coordinates[1] > circle->center[1] + circle->radius)
      {
        duplArc->ev->isValid = false;
        duplArc->ev = AddPoint(data->Q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
        duplArc->ev->node = duplArc;
        duplArc->ev->circle = circle;
      }
      else
      {
        freeCircle(circle);
      }
    }
    else
    {
      duplArc->ev = AddPoint(data->Q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
      duplArc->ev->node = duplArc;
      duplArc->ev->circle = circle;
    }
  }
  else
  {
    freeCircle(circle);
  }

  Inner2->he = createHe();
  Inner1->he = createHe();
  oppositeHe(Inner2->he, Inner1->he);
  addHE(data->Voronoid, Inner2->he);
  addHE(data->Voronoid, Inner1->he);
  Inner1->he->f = e->f;
  Inner1->he->f->he = Inner1->he;
  Inner2->he->f = getFace(data->Voronoid, arc->arcPoint);
  if (Inner2->he->f->he == NULL)
  {
    Inner2->he->f->he = Inner2->he;
  }
}

/*
 * Process a circle event
 * 
 * beachline: binaryTree representing the beachline
 * e: event to process
 * q: List of event
 */
void ProcessCircle(FortuneStruct *data, Event *e)
{
  // Check if ok
  if (!e->isValid)
  {
    //printf("Invalid event \n");
    return;
  }
  // a, b, c, d are BP
  //  - LeftLeftarc - c - Leftarc - a - arc to delete - b - Rightarc - d - RightRightarc
  Node *a = NULL, *b = NULL, *c = NULL, *d = NULL;

  Node *leftArc = NULL;
  if (e->node->leftBP != NULL)
    leftArc = e->node->leftBP->leftArc;
  Node *rightArc = NULL;
  if (e->node->rightBP != NULL)
    rightArc = e->node->rightBP->rightArc;
  Node *var = e->node;
  bool is_right = true;
  bool replaceB = true;

  while ((a == NULL || b == NULL || c == NULL || d == NULL) && var->Root != NULL)
  {
    if (var->Root->Left == var)
    {
      if (b == NULL)
      {
        replaceB = false;
        b = var->Root;

        if (!var->Root->Right->isLeaf)
        {
          Node *subvar = var->Root->Right;
          while (!subvar->Left->isLeaf)
          {
            subvar = subvar->Left;
          }
          d = subvar;
        }
      }
      else if (d == NULL)
      {
        d = var->Root;
      }
    }
    else
    {
      if (a == NULL)
      {
        replaceB = true;
        a = var->Root;

        if (!var->Root->Left->isLeaf)
        {
          Node *subvar = var->Root->Left;
          while (!subvar->Right->isLeaf)
          {
            subvar = subvar->Right;
          }
          c = subvar;
        }
      }
      else if (c == NULL)
      {
        c = var->Root;
      }
    }
    var = var->Root;
  }

  // TODO improve data struct and fetch arc from BP instead of storing LeftSite and RightSite

  if (leftArc != NULL)
  {
    if (leftArc->ev != NULL)
    {
      leftArc->ev->isValid = false;
      leftArc->ev = NULL;
    }
  }

  if (rightArc != NULL)
  {
    if (rightArc->ev != NULL)
    {
      rightArc->ev->isValid = false;
      rightArc->ev = NULL;
    }
  }

  // Must do this before
  HalfEdge *hea1 = a->he;
  HalfEdge *heb1 = b->he;
  Node *MainRoot = e->node->Root->Root;
  Node *replace = NULL;
  Node *arc = NULL;

  Node *bpArc2; //the break point which is not replaced

  if (replaceB)
  {
    bpArc2 = a;
    replace = b->Right;
    a->rightArc = b->rightArc;
    arc = a->leftArc;
  }
  else
  {
    bpArc2 = b;
    replace = a->Left;
    b->leftArc = a->rightArc;
    is_right = false;
    arc = b->rightArc;
  }

  bpArc2->leftArc = leftArc;
  bpArc2->rightArc = rightArc;
  leftArc->rightBP = bpArc2;
  rightArc->leftBP = bpArc2;

  if (MainRoot->Left == e->node->Root)
  {
    MainRoot->Left = replace;
    if (replace->isLeaf)
    {
      MainRoot->leftHeight = 1;
    }
    else
    {
      MainRoot->leftHeight = MAX(replace->leftHeight, replace->rightHeight);
    }
  }
  else
  {
    MainRoot->Right = replace;

    if (replace->isLeaf)
    {
      MainRoot->rightHeight = 1;
    }
    else
    {
      MainRoot->rightHeight = MAX(replace->leftHeight, replace->rightHeight);
    }
  }

  // Update Height
  Node *var2 = MainRoot;
  while (var2->Root != NULL)
  {
    if (var2->Root->Left == var2)
    {
      int m = MAX(var2->leftHeight, var2->rightHeight) + 1;
      if (m == var2->Root->leftHeight)
        break;
      var2->Root->leftHeight = m;
      var2 = var2->Root;
    }
    else
    {
      int m = MAX(var2->leftHeight, var2->rightHeight) + 1;
      if (m == var2->Root->rightHeight)
        break;
      var2->Root->rightHeight = m;
      var2 = var2->Root;
    }
  }

  replace->Root = MainRoot;

  //Circle *circle = createMiddleCircle(arc);

  Circle *circle = NULL;
  if (c != NULL)
  {
    arc = leftArc;
    if (replaceB)
      circle = createCircle(c->leftArc->arcPoint, c->rightArc->arcPoint, a->rightArc->arcPoint);
    else
      circle = createCircle(c->leftArc->arcPoint, c->rightArc->arcPoint, b->rightArc->arcPoint);
  }

  //TODO improve this
  // should directly fetch the two arc

  if (circle != NULL && circle->center[1] + circle->radius >= e->coordinates[1])
  {
    if (arc->ev != NULL)
    {
      if (!arc->ev->isValid || (arc->ev->coordinates[1] > circle->center[1] + circle->radius))
      {
        arc->ev->isValid = false;
        arc->ev = AddPoint(data->Q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
        arc->ev->node = arc;
        arc->ev->circle = circle;
      }
      else
      {
        freeCircle(circle);
      }
    }
    else
    {
      arc->ev = AddPoint(data->Q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
      arc->ev->node = arc;
      arc->ev->circle = circle;
    }
  }
  else
  {
    freeCircle(circle);
  }

  if (d != NULL)
  {
    arc = rightArc;
    if (replaceB)
      circle = createCircle(d->leftArc->arcPoint, d->rightArc->arcPoint, a->leftArc->arcPoint);
    else
      circle = createCircle(d->leftArc->arcPoint, d->rightArc->arcPoint, b->leftArc->arcPoint);
  }
  else
  {
    circle = NULL;
  }

  if (circle != NULL && circle->center[1] + circle->radius >= e->coordinates[1])
  {
    if (arc->ev != NULL)
    {
      if (!arc->ev->isValid || (arc->ev->coordinates[1] > circle->center[1] + circle->radius))
      {
        arc->ev->isValid = false;
        arc->ev = AddPoint(data->Q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
        arc->ev->node = arc;
        arc->ev->circle = circle;
      }
      else
      {
        freeCircle(circle);
      }
    }
    else
    {
      arc->ev = AddPoint(data->Q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
      arc->ev->node = arc;
      arc->ev->circle = circle;
    }
  }
  else
  {
    freeCircle(circle);
  }

  // a: left edge of the closing arc
  // b: right edge of the closing arc
  // c: new edge
  // a1 -> b1 connection
  // b2 -> c1 connection
  // c2 -> a2 connection
  // Vertex to b1, c1 and a2
  //
  Vertex *v = createVertex(e->circle->center);
  addVertex(data->Voronoid, v);

  HalfEdge *hea2 = hea1->Opposite;
  HalfEdge *heb2 = heb1->Opposite;
  if (hea2->f == heb1->f || hea2->f == heb2->f)
  {
    HalfEdge *swap = hea1;
    hea1 = hea2;
    hea2 = swap;
  }

  if (hea1->f == heb2->f)
  {
    HalfEdge *swap = heb1;
    heb1 = heb2;
    heb2 = swap;
  }

  HalfEdge *hec1 = createHe();
  HalfEdge *hec2 = createHe();
  oppositeHe(hec1, hec2);
  bpArc2->he = hec2;
  addHE(data->Voronoid, hec1);
  addHE(data->Voronoid, hec2);

  linkHe(hea1, heb1);
  linkHe(heb2, hec1);
  hec1->f = heb2->f;
  linkHe(hec2, hea2);
  hec2->f = hea2->f;

  hea2->v = v;
  heb1->v = v;
  hec1->v = v;
  v->he = hea2;

  freeNode(e->node->Root);
  freeNode(e->node);
}

void freeFortuneStruct(FortuneStruct *data)
{
  freeTree(data->beachLine);
  freePolygonMesh(data->Voronoid);
  freeQueue(data->Q);
  free(data);
}