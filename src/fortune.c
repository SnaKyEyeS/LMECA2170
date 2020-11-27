#include "fortune.h"

/*
 * Process the first event from the Queue
 */
void ProcessEvent(Node **beachLine, Queue *Q)
{
  if (Q == NULL)
    return;
  if (Q->First == NULL)
    return;
  Event *e = popQueue(Q);

  if (e == NULL)
    return; // TODO should assert
  if (e->type == SITE)
    ProcessSite(beachLine, e, Q);
  else
    ProcessCircle(beachLine, e, Q);
  freeEvent(e);
}

/*
 * Process a site event
 * 
 * beachline: binaryTree representing the beachline
 * e: event to process
 * Q: queue of events
 */
void ProcessSite(Node **beachLine, Event *e, Queue *q)
{
  Node *newLeaf = malloc(sizeof(Node));
  newLeaf->Left = NULL;
  newLeaf->Right = NULL;
  newLeaf->isLeaf = true;
  newLeaf->ev = NULL;

  newLeaf->arcPoint[0] = e->coordinates[0];
  newLeaf->arcPoint[1] = e->coordinates[1];

  if ((*beachLine) == NULL)
  {
    newLeaf->Root = NULL;
    (*beachLine) = newLeaf;
    return;
  }

  Node *arc = getArc(*beachLine, e->coordinates);
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
    (*beachLine) = Inner1;
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
  HalfEdge *he = malloc(sizeof(HalfEdge));
  // TODO: construct voronoid diagram

  Circle *circle = createRightCircle(newLeaf);

  if (circle != NULL)
  {
    printf("Hello add circle \n");
    // TODO check if overwrite previous event
    if (Inner2->Left->ev != NULL)
    {
      if (Inner2->Left->ev->coordinates[1] > circle->center[1] + circle->radius)
      {
        deleteEvent(q, Inner2->Left->ev);
        Inner2->Left->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE);
        Inner2->Left->ev->node = Inner2->Left;
      }
    }
    else
    {
      Inner2->Left->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE);
      Inner2->Left->ev->node = Inner2->Left;
    }
  }
  free(circle);

  circle = createLeftCircle(newLeaf);
  if (circle != NULL)
  {
    if (Inner1->Right->ev != NULL)
    {
      // if we need to replace the circle
      if (Inner1->Right->ev->coordinates[1] > circle->center[1] + circle->radius)
      {
        deleteEvent(q, Inner1->Right->ev);
        Inner1->Right->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE);
        Inner1->Right->ev->node = Inner1->Right;
      }
    }
    else
    {
      Inner1->Right->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE);
      printEvent(Inner1->Right->ev);
      Inner1->Right->ev->node = Inner1->Right;
    }
  }
  free(circle);
}

/*
 * Process a circle event
 * 
 * beachline: binaryTree representing the beachline
 * e: event to process
 * q: List of event
 */
void ProcessCircle(Node **beachline, Event *e, Queue *q)
{
  // Check if ok
  Node *MainRoot = e->node->Root->Root;
  Node *replace = NULL;
  Node *arc = NULL;
  bool is_right = true;

  float x, y;
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

  replace->Root = MainRoot;

  // TODO voronoid

  Circle *circle = createMiddleCircle(arc);

  //TODO improve this
  // should directly fetch the two arc

  if (circle != NULL)
  {
    if (arc->ev != NULL)
    {
      if (arc->ev->coordinates[1] > circle->center[1] + circle->radius)
      {
        deleteEvent(q, arc->ev);
        arc->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE);
        arc->ev->node = arc;
      }
    }
    else
    {
      arc->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE);
      arc->ev->node = arc;
    }
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

  if (circle != NULL)
  {
    if (arc->ev != NULL)
    {
      if (arc->ev->coordinates[1] > circle->center[1] + circle->radius)
      {
        deleteEvent(q, arc->ev);
        arc->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE);
        arc->ev->node = arc;
      }
    }
    else
    {
      arc->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE);
      arc->ev->node = arc;
    }
  }

  printNode(e->node);
  printNode(e->node->Root);
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
    return createCircle(leaf->arcPoint, leftleftBp->leftSite, leftleftBp->rightSite);
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