#include "fortune.h"

/*
 * Process the first event from the Queue
 */
void ProcessEvent(Node **beachLine, Queue *Q, PolygonMesh *PM)
{
  if (Q == NULL)
    return;
  if (Q->First == NULL)
    return;
  Event *e = popQueue(Q);

  if (e == NULL)
    return; // TODO should assert
  if (e->type == SITE)
    ProcessSite(beachLine, e, Q, PM);
  else
    ProcessCircle(beachLine, e, Q, PM);
  freeEvent(e);
}

/*
 * Process a site event
 * 
 * beachline: binaryTree representing the beachline
 * e: event to process
 * Q: queue of events
 */
void ProcessSite(Node **beachLine, Event *e, Queue *q, PolygonMesh *PM)
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

  Circle *circle = createRightCircle(newLeaf);

  if (circle != NULL && circle->center[1] + circle->radius > e->coordinates[1])
  {
    // TODO check if overwrite previous event
    if (Inner2->Left->ev != NULL)
    {
      if (Inner2->Left->ev->coordinates[1] > circle->center[1] + circle->radius)
      {
        deleteEvent(q, Inner2->Left->ev);
        Inner2->Left->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
        Inner2->Left->ev->node = Inner2->Left;
        freeCircle(Inner2->Left->ev->circle);
        Inner2->Left->ev->circle = circle;
      }
    }
    else
    {
      Inner2->Left->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
      Inner2->Left->ev->node = Inner2->Left;
      Inner2->Left->ev->circle = circle;
    }
  }

  circle = createLeftCircle(newLeaf);
  if (circle != NULL && circle->center[1] + circle->radius > e->coordinates[1])
  {
    if (Inner1->Right->ev != NULL)
    {
      // if we need to replace the circle
      if (Inner1->Right->ev->coordinates[1] > circle->center[1] + circle->radius)
      {
        deleteEvent(q, Inner1->Right->ev);
        Inner1->Right->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
        Inner1->Right->ev->node = Inner1->Right;
        freeCircle(Inner1->Right->ev->circle);
        Inner1->Right->ev->circle = circle;
      }
    }
    else
    {
      Inner1->Right->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
      Inner1->Right->ev->node = Inner1->Right;
      Inner1->Right->ev->circle = circle;
    }
  }

  Inner2->he = createHe();
  Inner2->he->Opposite = createHe

      addHE(PM, Inner2->he);
}

/*
 * Process a circle event
 * 
 * beachline: binaryTree representing the beachline
 * e: event to process
 * q: List of event
 */
void ProcessCircle(Node **beachline, Event *e, Queue *q, PolygonMesh *PM)
{
  // Check if ok
  if (!e->isValid)
  {
    printf("Invalid event \n");
    return;
  }

  Node *var = getLeftestArc(e->node);
  if (var->ev != NULL)
  {
    var->ev->isValid = false;
  }
  var = getRightestArc(e->node);
  if (var->ev != NULL)
  {
    var->ev->isValid = false;
  }

  Node *MainRoot = e->node->Root->Root;
  Node *replace = NULL;
  Node *arc = NULL;
  bool is_right = true;

  if (e->node->Root->Left == e->node)
  {
    Node *var = getLeftBpNode(e->node->Root);
    replace = e->node->Root->Right;
    var->rightSite[0] = e->node->Root->rightSite[0];
    var->rightSite[1] = e->node->Root->rightSite[1];
    arc = getLeftArc(e->node->Root->Right);
  }
  else
  {
    Node *var = getRightBpNode(e->node->Root);
    replace = e->node->Root->Left;
    var->leftSite[0] = e->node->Root->leftSite[0];
    var->leftSite[1] = e->node->Root->leftSite[1];
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

  // TODO voronoid

  Circle *circle = createMiddleCircle(arc);
  //TODO improve this
  // should directly fetch the two arc

  if (circle != NULL && circle->center[1] + circle->radius > e->coordinates[1])
  {
    if (arc->ev != NULL)
    {
      if (!arc->ev->isValid || (arc->ev->coordinates[1] > circle->center[1] + circle->radius))
      {
        deleteEvent(q, arc->ev);
        arc->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
        arc->ev->node = arc;
        freeCircle(arc->ev->circle);
        arc->ev->circle = circle;
      }
    }
    else
    {
      arc->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
      arc->ev->node = arc;
      arc->ev->circle = circle;
    }
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
        printQueue(q);
        printEvent(arc->ev);
        deleteEvent(q, arc->ev);
        arc->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
        arc->ev->node = arc;
        freeCircle(arc->ev->circle);
        arc->ev->circle = circle;
      }
    }
    else
    {
      arc->ev = AddPoint(q, circle->center[0], circle->center[1] + circle->radius, CIRCLE, (Face *)NULL);
      arc->ev->node = arc;
      arc->ev->circle = circle;
    }
  }

  Vertex *v = createVertex(e->circle->center);
  addVertex(PM, v);

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
      printNode(left);
      printNode(leaf);
      printNode(right);
      return createCircle(right->leftSite, leaf->arcPoint, left->rightSite);
    }

    var = var->Root;
  }
  // no 2 Right branch
  return NULL;
}