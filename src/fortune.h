#include "BOV.h"

#ifndef FORTUNE
#define FORTUNE

#include "binaryTree.h"
#include "circle.h"
#include "queue.h"
#include "events.h"
#include "halfEdge.h"

struct FortuneStruct;
typedef struct FortuneStruct FortuneStruct;

struct FortuneStruct
{
  PolygonMesh *Voronoid;
  Node *beachLine;
  Queue *Q;
};

FortuneStruct *initFortune(coord *points, int n);
void fortuneAlgo(FortuneStruct *data, double yLine);

void ProcessEvent(FortuneStruct *data);
void ProcessSite(FortuneStruct *data, Event *e);
void ProcessCircle(FortuneStruct *data, Event *e);

Circle *createLeftCircle(Node *leaf);
Circle *createRightCircle(Node *leaf);
Circle *createMiddleCircle(Node *leaf);

void freeFortuneStruct(FortuneStruct *data);

#endif