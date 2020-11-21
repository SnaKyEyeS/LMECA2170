#include "BOV.h"

enum TYPE_EVENT
{
  SITE,
  CIRCLE
};

typedef struct Event Event;
typedef struct Queue Queue;

struct Event
{
  double coordinates[2];
  Event *next;
  enum TYPE_EVENT type;
};

struct Queue
{
  Event *First;
  Event *Last;
};

// Create a Q
Queue *LoadSortedEventQueue(double *points, int n);
Queue *LoadEventQueue(double *points, int n);

// Utils
void AddPoint(Queue *Q, double x, double y, enum TYPE_EVENT type);
Queue *PopQueue(Queue *Q);

void printQueue(Queue *Q);