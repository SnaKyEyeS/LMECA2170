#include "BOV.h"

#ifndef FORTUNE
#define FORTUNE

#include "binaryTree.h"
#include "circle.h"
#include "queue.h"
#include "events.h"

void ProcessEvent(Node **beachLine, Queue *Q);
void ProcessSite(Node **beachLine, Event *e, Queue *q);
void ProcessCircle(Node **beachLine, Event *e, Queue *q);

Circle *createLeftCircle(Node *leaf);
Circle *createRightCircle(Node *leaf);
Circle *createMiddleCircle(Node *leaf);

#endif