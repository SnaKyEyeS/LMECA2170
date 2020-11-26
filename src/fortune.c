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

  newLeaf->arcPoint[0] = e->coordinates[0];
  newLeaf->arcPoint[1] = e->coordinates[1];

  if ((*beachLine) == NULL)
  {
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

  //Update Root
  printf("Update root\n");
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

  printf("Update Left Right\n");
  // Update Left Right
  Inner2->Left = arc;
  Inner2->Right = newLeaf;
  Inner2->Root = Inner1;
  Inner1->Left = Inner2;
  Inner1->Right = duplicateLeaf(arc);
  Inner1->Right->Root = Inner1;

  printf("Update site position \n");
  // Update site position

  // TODO not working
  Inner2->leftSite[0] = arc->arcPoint[0];
  Inner2->leftSite[1] = arc->arcPoint[1];
  printf("Sub step 1 \n");
  Inner2->rightSite[0] = e->coordinates[0];
  Inner2->rightSite[1] = e->coordinates[1];
  printf("Sub step 2 \n");
  Inner1->rightSite[0] = arc->arcPoint[0];
  Inner1->rightSite[1] = arc->arcPoint[1];
  printf("Sub step 3 \n");
  Inner1->leftSite[0] = e->coordinates[0];
  Inner1->leftSite[1] = e->coordinates[1];

  //TODO: rebalance
  printf("Half edge \n");
  HalfEdge *he = malloc(sizeof(HalfEdge));
  // TODO: construct voronoid diagram

  printf("Circle LEFT \n");
  Circle *circle = createLeftCircle(newLeaf);
  if (circle != NULL)
  {
    // TODO check if overwrite previous event
    if (Inner2->Left->ev != NULL)
    {
      // if we need to repalce the circle
      if (Inner2->Left->ev->coordinates[1] > circle->center[1] + circle->radius)
      {
      }
    }
    else
    {
      Inner2->Left->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE);
      Inner2->Left->ev->node = Inner2->Left;
    }
  }

  free(circle);

  // todo free circle
  printf("Circle right \n");
  circle = createRightCircle(newLeaf);
  if (circle != NULL)
  {
    Inner1->Right->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE);
    Inner2->Right->ev->node = Inner1->Right;
  }
  free(circle);

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
void ProcessCircle(Node **beachline, Event *e, Queue *q)
{
  // Check if ok
  Node *MainRoot = e->node->Root->Root;
  Node *replace = NULL;
  Node *arc = NULL;
  bool is_right = true;

  double x, y;
  printf("Fetch good replace arc and coord \n");
  printAllTree(*beachline);
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

  printf("Fetch good mainRoot \n");
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

  printAllTree(*beachline);

  // TODO voronoid

  Circle *circle = createMiddleCircle(arc);

  //TODO improve this
  // should directly fetch the two arc

  printf("First circle \n");
  if (circle != NULL)
  {
    arc->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE);
    arc->ev->node = arc;
  }

  printf("Second circle choice \n");
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

  printf("Add second circle \n");
  if (circle != NULL)
  {
    arc->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE);
    arc->ev->node = arc;
  }

  printf("FREE \n");
  printf("Sup ?\n");
  printNode(e->node);
  printf("Bruh \n ");
  printNode(e->node->Root);
  freeNode(e->node->Root);
  printf("Second Free \n");
  freeNode(e->node);

  printf("End process circle \n");
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