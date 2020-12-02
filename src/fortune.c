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
  data->Q = LoadSortedEventQueue(points, 0, n, data->Voronoid->faces, NULL);
  data->beachLine = NULL;

  return data;
}

/*
 * Apply the fortune algorithm to the datastructure up to a yLine
 */
void fortuneAlgo(FortuneStruct *data, float yLine)
{
  while (data->Q->e != NULL && data->Q->e->coordinates[1] < yLine)
  {
    //printf("New process \n");
    //printQueue(data->Q);
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
  if (data->Q == NULL) // TODO update
    return;
  Event *e = popQueue(data->Q);
  if (e == NULL)
    return; // TODO should assert
  if (e->type == SITE)
    ProcessSite(data, e);
  else
    ProcessCircle(data, e);
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

  Inner1->isLeaf = false;
  Inner2->isLeaf = false;

  //Update Root
  if (arc->Root != NULL)
  {

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

  Inner1->Root = arc->Root;
  arc->Root = Inner2;
  newLeaf->Root = Inner2;
  Inner2->Root = Inner1;

  // Update Left Right
  Inner2->Left = arc;
  Inner2->Right = newLeaf;
  Inner1->Left = Inner2;
  Inner1->Right = duplicateLeaf(arc);
  Inner1->Right->Root = Inner1;

  // Update site position

  // TODO not working
  Inner2->leftSite[0] = arc->arcPoint[0];
  Inner2->leftSite[1] = arc->arcPoint[1];
  Inner2->rightSite[0] = e->coordinates[0];
  Inner2->rightSite[1] = e->coordinates[1];
  Inner1->rightSite[0] = arc->arcPoint[0];
  Inner1->rightSite[1] = arc->arcPoint[1];
  Inner1->leftSite[0] = e->coordinates[0];
  Inner1->leftSite[1] = e->coordinates[1];

  //TODO: rebalance

  Circle *circle = createRightCircle(newLeaf);

  if (circle != NULL && circle->center[1] + circle->radius > e->coordinates[1])
  {
    // TODO check if overwrite previous event
    if (Inner2->Left->ev != NULL)
    {
      if (Inner2->Left->ev->coordinates[1] > circle->center[1] + circle->radius)
      {
        Inner2->Left->ev->isValid = false;
        Inner2->Left->ev = AddPoint(data->Q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
        Inner2->Left->ev->node = Inner2->Left;
        Inner2->Left->ev->circle = circle;
      }
      else
      {
        freeCircle(circle);
      }
    }
    else
    {
      Inner2->Left->ev = AddPoint(data->Q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
      Inner2->Left->ev->node = Inner2->Left;
      Inner2->Left->ev->circle = circle;
    }
  }
  else
  {
    freeCircle(circle);
  }

  circle = createLeftCircle(newLeaf);
  if (circle != NULL && circle->center[1] + circle->radius > e->coordinates[1])
  {
    if (Inner1->Right->ev != NULL)
    {
      // if we need to replace the circle
      if (Inner1->Right->ev->coordinates[1] > circle->center[1] + circle->radius)
      {
        Inner1->Right->ev->isValid = false;
        Inner1->Right->ev = AddPoint(data->Q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
        Inner1->Right->ev->node = Inner1->Right;
        Inner1->Right->ev->circle = circle;
      }
      else
      {
        freeCircle(circle);
      }
    }
    else
    {
      Inner1->Right->ev = AddPoint(data->Q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
      Inner1->Right->ev->node = Inner1->Right;
      Inner1->Right->ev->circle = circle;
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

  Node *var = getLeftestArc(e->node);
  if (var->ev != NULL)
  {
    //printEvent(var->ev);
    var->ev->isValid = false;
    var->ev = NULL;
  }
  var = getRightestArc(e->node);
  if (var->ev != NULL)
  {
    var->ev->isValid = false;
    var->ev = NULL;
  }
  // Must do this before
  HalfEdge *hea1 = getLeftBpNode(e->node)->he;
  HalfEdge *heb1 = getRightBpNode(e->node)->he;
  Node *MainRoot = e->node->Root->Root;
  Node *replace = NULL;
  Node *arc = NULL;
  bool is_right = true;

  Node *bpArc2; //the break point which is not replaced
  if (e->node->Root->Left == e->node)
  {
    bpArc2 = getLeftBpNode(e->node->Root);
    replace = e->node->Root->Right;
    bpArc2->rightSite[0] = e->node->Root->rightSite[0];
    bpArc2->rightSite[1] = e->node->Root->rightSite[1];
    arc = getLeftArc(e->node->Root->Right);
  }
  else
  {
    bpArc2 = getRightBpNode(e->node->Root);
    replace = e->node->Root->Left;
    bpArc2->leftSite[0] = e->node->Root->leftSite[0];
    bpArc2->leftSite[1] = e->node->Root->leftSite[1];
    is_right = false;
    arc = getRightArc(e->node->Root->Left);
  }
  if (MainRoot->Left == e->node->Root)
  {
    MainRoot->Left = replace;
  }
  else
  {
    MainRoot->Right = replace;
  }

  replace->Root = MainRoot;

  Circle *circle = createMiddleCircle(arc);
  //TODO improve this
  // should directly fetch the two arc

  if (circle != NULL && circle->center[1] + circle->radius > e->coordinates[1])
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

  if (is_right)
  {
    circle = createRightCircle(arc);
    arc = getLeftestArc(arc);
  }
  else
  {
    circle = createLeftCircle(arc);
    arc = getRightestArc(arc);
  }
  if (circle != NULL && circle->center[1] + circle->radius > e->coordinates[1])
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

/*
 * Create the circle with the new arc at the left
 * 
 * Node: arc on the LEFT
 */
Circle *createLeftCircle(Node *leaf)
{
  Node *rightrightBp = getRightBpNode(getRightBpNode(leaf));
  if (rightrightBp != NULL)
  {
    return createCircle(leaf->arcPoint, rightrightBp->leftSite, rightrightBp->rightSite);
  }
}

/*
 * Create the circle with the new arc at the right
 * 
 * Node: arc on the right
 */
Circle *createRightCircle(Node *leaf)
{
  // TODO fix not working great also the next one (it does not verify if we have right bp)
  Node *leftleftBp = getLeftBpNode(getLeftBpNode(leaf));
  if (leftleftBp != NULL)
  {
    return createCircle(leftleftBp->leftSite, leftleftBp->rightSite, leaf->arcPoint);
  }
}

/*
 * Create the circle with the new arc at the middle
 * 
 * Node: arc on the middle
 */
Circle *createMiddleCircle(Node *leaf)
{
  Node *var = leaf;
  Node *left = NULL;
  Node *right = NULL;
  while (var->Root != NULL)
  {
    if (var->Root->Right == var && right == NULL)
    {
      right = var->Root;
    }

    if (var->Root->Left == var && left == NULL)
    {
      left = var->Root;
    }

    if (left != NULL & right != NULL)
    {
      return createCircle(right->leftSite, leaf->arcPoint, left->rightSite);
    }

    var = var->Root;
  }
  // no 2 Right branch
  return NULL;
}

void freeFortuneStruct(FortuneStruct *data)
{
  freeTree(data->beachLine);
  freePolygonMesh(data->Voronoid);
  freeQueue(data->Q);
  free(data);
}