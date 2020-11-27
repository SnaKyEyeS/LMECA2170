#include "queue.h"

/*
 * Create a Q form a sorted list of point
 * 
 * points: list of all sorted points to load (x,y) - size min 2n
 * n: number of points to load
 */
Queue *LoadSortedEventQueue(float (*points)[2], int n, Face **f)
{
  Queue *Q = malloc(sizeof(Queue));

  Q->First = NULL;
  Event *last = NULL;

  for (int i = 0; i < n; i++)
  {
    Event *E = malloc(sizeof(Event));
    E->coordinates[0] = points[i][0];
    E->coordinates[1] = points[i][1];
    E->type = SITE;
    E->isValid = true;
    E->circle = NULL;
    E->f = f[i];

    if (Q->First == NULL)
    {
      E->previous = NULL;
      E->next = NULL;
      Q->First = E;
      last = E;
    }
    else
    {
      last->next = E;
      E->previous = last;
      printEvent(last);
      E->next = NULL;
    }
    last = E;
  }
  return Q;
}

/*
 * Create a queue from an unsorted list of point
 * 
 * points: list of all points to load (x,y) - size min 2n
 * n: number of points to load
 */
Queue *LoadEventQueue(float (*points)[2], int n, Face **f)
{
  qsort(points, n, sizeof(float) * 2, comparefloats);
  return LoadSortedEventQueue(points, n, f);
}
/*
 * Add a new point to the Queue
 * 
 * Q: Queue
 * x: x coordinate of the new point
 * y: y coordinate of the new point
 */

Event *AddPoint(Queue *Q, float x, float y, enum TYPE_EVENT type, Face *f)
{
  Event *E = malloc(sizeof(Event));
  E->coordinates[0] = x;
  E->coordinates[1] = y;
  E->type = type;
  E->isValid = true;
  E->circle = NULL;

  Event *P = Q->First;

  if (P == NULL)
  {
    Q->First = E;
    E->next = NULL;
    E->previous = NULL;
    return E;
  }

  if (E->coordinates[1] < P->coordinates[1])
  {
    E->next = P;
    E->next->previous = E;
    Q->First = E;
    return E;
  }

  while (P != NULL)
  {
    if (P->next == NULL)
    {
      P->next = E;
      E->next = NULL;
      E->previous = P;
      return E;
    }
    else
    {

      if (E->coordinates[1] < P->next->coordinates[1])
      {
        E->next = P->next;
        E->previous = P;
        E->next->previous = E;
        P->next = E;
        return E;
      }
      P = P->next;
    }
  }
  E->next = NULL;
}

/*
 * Delete an event from Q
 */
void deleteEvent(Queue *Q, Event *e)
{
  if (Q == NULL)
    return;
  if (e == NULL)
    return;

  if (Q->First == e)
  {
    Q->First = e->next;
    Q->First->previous = NULL;
    freeEvent(e);
    return;
  }

  if (e->next != NULL)
  {
    e->next->previous = e->previous;
  }
  e->previous->next = e->next;
  freeEvent(e);
}

/*
 * Pop an element of the queue
 * 
 */
Event *popQueue(Queue *Q)
{
  if (Q == NULL)
  {
    return NULL;
  }

  if (Q->First == NULL)
  {
    return NULL;
  }

  Event *e = Q->First;
  Q->First = e->next;
  if (Q->First != NULL)
  {
    Q->First->previous = NULL;
  }
  return e;
}

/*
 * Get the elements of Q which are circle events into points and return the number of elemens
 */
int getCircleEvent(Queue *Q, coord *points)
{
  int nCircle = 0;
  Event *var = Q->First;
  while (var != NULL)
  {
    if (var->type == CIRCLE)
    {
      points[nCircle][0] = var->coordinates[0];
      points[nCircle][1] = var->coordinates[1];
      nCircle++;
    }
    var = var->next;
  }
  return nCircle;
}

void printQueue(Queue *Q)
{
  Event *E = Q->First;
  int i = 0;
  printf("QUEUE &FIRST\n");
  printf("      %p \n", Q->First);
  printf("EVENTS \n%*s X      Y      TYPE ISVALID &EVENT         &NEXT          &PREVIOUS     &NODE\n", 4, "id");
  while (E != NULL)
  {
    printf("%*d %+2.3f %+2.3f %d    %d      %14p %14p %14p %p\n", 4, i, E->coordinates[0], E->coordinates[1], E->type, E->isValid, E, E->next, E->previous, E->node);
    E = E->next;
    i++;
  }
  printf("End of Queue \n");
}