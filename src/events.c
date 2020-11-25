#include "events.h"
/*
 * Create a Q form a sorted list of point
 * 
 * points: list of all sorted points to load (x,y) - size min 2n
 * n: number of points to load
 */
Queue *LoadSortedEventQueue(double *points, int n)
{
  Queue *Q = malloc(sizeof(Queue));

  Q->First = NULL;
  Q->Last = NULL;

  for (int i = 0; i < n; i++)
  {
    Event *E = malloc(sizeof(Event));
    E->coordinates[0] = points[i * 2];
    E->coordinates[1] = points[i * 2 + 1];
    E->type = SITE;

    if (Q->First == NULL)
    {
      Q->First = E;
    }
    else
    {
      Q->Last->next = E;
    }
    Q->Last = E;
  }
  Q->Last->next = NULL;
  return Q;
}

/*
 * Process a site event
 * 
 * beachline: binaryTree representing the beachline
 * e: event to process
 * Q: queue of events
 */
void ProcessSite(Node *beachLine, Event *e, Queue *q)
{

  Node *newLeaf = malloc(sizeof(Node));
  newLeaf->Left = NULL;
  newLeaf->Right = NULL;
  newLeaf->isLeaf = true;

  newLeaf->ev = e; // useful ?
  newLeaf->arcPoint[0] = e->coordinates[0];
  newLeaf->arcPoint[1] = e->coordinates[1];

  if (beachLine == NULL)
  {
    newLeaf->Root = NULL;
    beachLine = newLeaf;
    return;
  }

  Node *arc = getArc(beachLine, e->coordinates);

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
    beachLine = Inner1;
  }
  Inner1->Root = arc->Root;
  arc->Root = Inner2;
  newLeaf->Root = Inner2;

  // Update Left Right
  Inner2->Left = arc;
  Inner2->Right = newLeaf;
  Inner2->Root = Inner2;
  Inner1->Left = Inner2;
  Inner1->Right = duplicateLeaf(arc);
  Inner1->Right->Root = Inner1;

  // Update site position
  Inner2->leftSite[0] = arc->ev->coordinates[0];
  Inner2->leftSite[1] = arc->ev->coordinates[1];
  Inner2->rightSite[0] = e->coordinates[0];
  Inner2->rightSite[1] = e->coordinates[1];
  Inner1->rightSite[0] = arc->ev->coordinates[0];
  Inner1->rightSite[1] = arc->ev->coordinates[1];
  Inner1->leftSite[0] = e->coordinates[0];
  Inner1->leftSite[1] = e->coordinates[1];

  //TODO: rebalance

  HalfEdge *he = malloc(sizeof(HalfEdge));
  // TODO: construct voronoid diagram

  Circle *circle = createLeftCircle(newLeaf);
  if (circle != NULL)
  {
    // TODO check if overwrite previous event
    Inner2->Left->ev = AddPoint(q, circle->center[0], circle->center[1] - circle->radius, CIRCLE);
    Inner2->Left->ev->node = Inner2->Left;
  }

  // todo free circle

  circle = createRightCircle(newLeaf);
  if (circle != NULL)
  {
    Inner1->Right->ev = AddPoint(q, circle->center[0], circle->center[1] - circle->radius, CIRCLE);
    Inner2->Right->ev->node = Inner1->Right;
  }

  //todo free circle

  //TODO free event
}

/*
 * Process a circle event
 * 
 * beachline: binaryTree representing the beachline
 * e: event to process
 * q: List of event
 */
void ProcessCircle(Node *beachline, Event *e, Queue *q)
{
  // Check if ok
  Node *MainRoot = e->node->Root->Root;
  Node *replace = NULL;
  Node *arc = NULL;
  bool is_right = true;

  double x, y;
  if (e->node->Root->Left == e->node)
  {
    replace = e->node->Root->Right;
    x = e->node->Root->rightSite[0];
    y = e->node->Root->rightSite[1];
    arc = getLeftArc(e->node->Root->Right);
  }
  else
  {
    replace = e->node->Root->Left;
    x = e->node->Root->leftSite[0];
    y = e->node->Root->leftSite[1];
    is_right = false;
    arc = getRightArc(e->node->Root->Left);
  }
  if (MainRoot->Left == e->node->Root)
  {
    MainRoot->Left = replace;
    MainRoot->leftSite[0] = x;
    MainRoot->leftSite[1] = y;
  }
  else
  {
    MainRoot->Right = replace;
    MainRoot->rightSite[0] = x;
    MainRoot->leftSite[1] = y;
  }

  // TODO voronoid

  Circle *circle = createMiddleCircle(arc);

  //TODO improve this
  // should directly fetch the two arc

  if (circle != NULL)
  {
    arc->ev = AddPoint(q, circle->center[0], circle->center[1] - circle->radius, CIRCLE);
    arc->ev->node = arc;
  }

  if (is_right)
  {
    arc = getRightestArc(arc);
    circle = createRightCircle(arc);
  }
  else
  {
    arc = getLeftestArc(arc);
    circle = createLeftCircle(arc);
  }

  arc->ev = AddPoint(q, circle->center[0], circle->center[1] - circle->radius, CIRCLE);
  arc->ev->node = arc;

  freeNode(e->node->Root);
  freeNode(e->node);
  freeEvent(e);
}

/*
 * Create the circle with the new arc at the left
 * 
 * Node: arc on the LEFT
 */
Circle *createLeftCircle(Node *leaf)
{
  Node *var = leaf;
  int left_depl = 0;
  while (var->Root != NULL)
  {

    if (var->Root->Left == var)
    {
      left_depl += 1;
    }
    if (left_depl == 2)
    {
      return createCircle(leaf->arcPoint, var->leftSite, var->rightSite);
    }

    var = var->Root;
  }
  // no 2 Left branch
  return NULL;
}

/*
 * Create the circle with the new arc at the right
 * 
 * Node: arc on the right
 */
Circle *createRightCircle(Node *leaf)
{
  Node *var = leaf;
  int right_depl = 0;
  while (var->Root != NULL)
  {

    if (var->Root->Right == var)
    {
      right_depl += 1;
    }
    if (right_depl == 2)
    {
      return createCircle(leaf->arcPoint, var->leftSite, var->rightSite);
    }

    var = var->Root;
  }
  // no 2 Right branch
  return NULL;
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

    if (var->Root->Right == var && right != NULL)
    {
      right = var->Root;
    }

    if (var->Root->Left == var && left != NULL)
    {
      left = var->Root;
    }

    if (left != NULL & right != NULL)
    {
      return createCircle(leaf->arcPoint, right->rightSite, left->leftSite);
    }

    var = var->Root;
  }
  // no 2 Right branch
  return NULL;
}

/*
 * Compare the double representation of a point (x,y)
 * 
 * A: first point
 * B: second point
 */
int compareDoubles(const void *A, const void *B)
{
  double *a = (double *)A;
  double *b = (double *)B;
  if (a[1] > b[1])
  {
    return 1;
  }
  else if (b[1] > a[1])
  {
    return -1;
  }
  return 0;
}

/*
 * Create a queue from an unsorted list of point
 * 
 * points: list of all points to load (x,y) - size min 2n
 * n: number of points to load
 */
Queue *LoadEventQueue(double *points, int n)
{
  qsort(points, n, sizeof(double) * 2, compareDoubles);
  return LoadSortedEventQueue(points, n);
}
/*
 * Add a new point to the Queue
 * 
 * Q: Queue
 * x: x coordinate of the new point
 * y: y coordinate of the new point
 */

Event *AddPoint(Queue *Q, double x, double y, enum TYPE_EVENT type)
{
  Event *E = malloc(sizeof(Event));
  E->coordinates[0] = x;
  E->coordinates[1] = y;
  E->type = type;

  Event *P = Q->First;

  if (P == NULL)
  {
    Q->First = E;
    E->next = NULL;
    Q->Last = E;
    return E;
  }

  if (E->coordinates[1] < P->coordinates[1])
  {
    E->next = P;
    Q->First = E;
    return E;
  }

  while (P != NULL)
  {
    if (P->next == NULL)
    {
      P->next = E;
      E->next = NULL;
      Q->Last = E;
      return E;
    }
    else
    {

      if (E->coordinates[1] < P->next->coordinates[1])
      {
        E->next = P->next;
        P->next = E;
        return E;
      }
      P = P->next;
    }
  }
  E->next = NULL;
  Q->Last = E;
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
}

/*
 * Pop an element of the queue
 * 
 */
Event *popQueue(Queue *Q)
{
  if (Q == NULL)
  {
    return NULL;
  }

  if (Q->First == NULL)
  {
    return NULL;
  }

  Event *e = Q->First;
  Q->First = e->next;
  return e;
}

/*
 * Free an event
 */
void freeEvent(Event *e)
{
  free(e);
}

void printEvent(Event *e)
{
  if (e == NULL)
  {
    printf("Event is NULL\n");
    return;
  }
  printf("Event %+2.3f %+2.3f %d    %p %p\n", e->coordinates[0], e->coordinates[1], e->type, e, e->next);
}

void printQueue(Queue *Q)
{
  Event *E = Q->First;
  int i = 0;
  printf("QUEUE & FIRST & LAST\n");
  printf("%p %p\n", Q->First, Q->Last);
  printf("EVENTS \n%*s X      Y      TYPE &EVENT         &NEXT\n", 4, "id");
  while (E != NULL)
  {
    printf("%*d %+2.3f %+2.3f %d    %p %p\n", 4, i, E->coordinates[0], E->coordinates[1], E->type, E, E->next);
    E = E->next;
    i++;
  }
  printf("End of Queue \n");
}