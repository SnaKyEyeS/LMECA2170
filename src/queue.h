#include "BOV.h"
#include <math.h>
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
  Event **es;
  int max;
  int size;
};

// Create a Q
Queue *LoadSortedEventQueue(coord *points, int n, Face **faces);
Queue *LoadEventQueue(coord *points, int n, Face **faces);

Event *AddPoint(Queue *Q, float x, float y, enum TYPE_EVENT type, Face *f);
Event *popQueue(Queue *Q);

int getCircleEvent(Queue *Q, coord *points, int start);

void freeQueue(Queue *Q);
void freeNodeQueue(Queue *Q);

void printQueue(Queue *Q);

#endif