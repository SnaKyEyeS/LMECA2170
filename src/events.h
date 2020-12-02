#include "BOV.h"
#include <stdbool.h>

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
  float coordinates[2];
  enum TYPE_EVENT type;
  Node *node;

  bool isValid;
  Circle *circle;
  Face *f;
};

Event *initEmptyEvent();

void freeEvent(Event *e);

void printEvent(Event *e);
#endif