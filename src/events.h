#include "BOV.h"

#ifndef EVENTS
#define EVENTS

struct Event;
typedef struct Event Event;
typedef struct Queue Queue;

#include "binaryTree.h"

enum TYPE_EVENT
{
  SITE,
  CIRCLE
};

struct Event
{
  double coordinates[2];
  Event *next;
  enum TYPE_EVENT type;
  Node *node;
};

struct Queue
{
  Event *First;
  Event *Last;
};

// Create a Q
Queue *LoadSortedEventQueue(double *points, int n);
Queue *LoadEventQueue(double *points, int n);

void ProcessSite(Node *beachLine, Event *e);
void ProcessCircle(Node *beachLine, Event *e);

// Utils
void AddPoint(Queue *Q, double x, double y, enum TYPE_EVENT type);
Queue *PopQueue(Queue *Q);

void printQueue(Queue *Q);

#endif