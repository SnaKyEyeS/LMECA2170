#include "BOV.h"
#include "utils.h"

#ifndef QUEUE_H
#define QUEUE_H

struct Queue;
typedef struct Queue Queue;

#include "circle.h"
#include "binaryTree.h"
#include "events.h"

struct Queue
{
  Event *First;
};

// Create a Q
Queue *LoadSortedEventQueue(double *points, int n);
Queue *LoadEventQueue(double *points, int n);

void deleteEvent(Queue *Q, Event *e);
Event *AddPoint(Queue *Q, double x, double y, enum TYPE_EVENT type);
Event *popQueue(Queue *Q);

void printQueue(Queue *Q);

#endif