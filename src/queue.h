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
  Event *e;
  Queue *Left;
  Queue *Right;
  Queue *Root;
};

// Create a Q
Queue *LoadSortedEventQueue(coord *points, int a, int n, Face **faces, Queue *root);
Queue *LoadEventQueue(coord *points, int n, Face **faces);

void deleteEvent(Queue *Q, Event *e);
Event *AddPoint(Queue *Q, float x, float y, enum TYPE_EVENT type, Face *f);
Event *popQueue(Queue *Q);

int getCircleEvent(Queue *Q, coord *points, int start);

void freeQueue(Queue *Q);
void freeNodeQueue(Queue *Q);

void printNodeQueue(Queue *Q, int i);
void printQueue(Queue *Q);

Queue *initQueueNode();

#endif