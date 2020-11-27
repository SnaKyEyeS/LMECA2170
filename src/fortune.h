#include "BOV.h"

#ifndef FORTUNE
#define FORTUNE

#include "binaryTree.h"
#include "circle.h"
#include "queue.h"
#include "events.h"
#include "halfEdge.h"

void ProcessEvent(Node **beachLine, Queue *Q, PolygonMesh *PM);
void ProcessSite(Node **beachLine, Event *e, Queue *q, PolygonMesh *PM);
void ProcessCircle(Node **beachLine, Event *e, Queue *q, PolygonMesh *PM);

Circle *createLeftCircle(Node *leaf);
Circle *createRightCircle(Node *leaf);
Circle *createMiddleCircle(Node *leaf);

#endif