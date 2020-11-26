#include "BOV.h"

#ifndef EVENTS
#define EVENTS

struct Event;
typedef struct Event Event;

enum TYPE_EVENT
{
  SITE,
  CIRCLE
};

#include "binaryTree.h"
#include "circle.h"
#include "queue.h"

struct Event
{
  double coordinates[2];
  Event *next;
  Event *previous;
  enum TYPE_EVENT type;
  Node *node;
};

void freeEvent(Event *e);

void printEvent(Event *e);
#endif