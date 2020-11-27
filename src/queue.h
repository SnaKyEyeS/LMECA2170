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
Queue *LoadSortedEventQueue(float (*points)[2], int n, Face **faces);
Queue *LoadEventQueue(float (*points)[2], int n, Face **faces);

void deleteEvent(Queue *Q, Event *e);
Event *AddPoint(Queue *Q, float x, float y, enum TYPE_EVENT type, Face *f);
Event *popQueue(Queue *Q);

int getCircleEvent(Queue *Q, coord *points);

void printQueue(Queue *Q);

#endif