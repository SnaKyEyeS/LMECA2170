#include "events.h"
/*
 * Create a Q form a sorted list of point
 * 
 * points: list of all sorted points to load (x,y) - size min 2n
 * n: number of points to load
 */
Queue *LoadSortedEventQueue(double *points, int n)
{
  Queue *Q = malloc(sizeof(Queue));

  Q->First = NULL;
  Q->Last = NULL;

  for (int i = 0; i < n; i++)
  {
    Event *E = malloc(sizeof(Event));
    E->coordinates[0] = points[i * 2];
    E->coordinates[1] = points[i * 2 + 1];
    E->type = SITE;

    if (Q->First == NULL)
    {
      Q->First = E;
    }
    else
    {
      Q->Last->next = E;
    }
    Q->Last = E;
  }
  Q->Last->next = NULL;
  return Q;
}

void ProcessSite(Node *beachLine, Event *e)
{
  if (beachLine == NULL)
  {
    beachLine = malloc(sizeof(Node));
    beachLine->Left = NULL;
    beachLine->Right = NULL;
    beachLine->bp[0] = e->coordinates[0];
    beachLine->bp[1] = e->coordinates[1];
  }
  else
  {
    Node *var = beachLine;
    // Get the arc (2)
    while (var->Left != NULL || var->Right != NULL)
    {
      if (var->Left == NULL || var->Right == NULL)
      {
        // ERROR
        return;
      }
      if (e->coordinates[0] > var->bp[0])
      {
        var = var->Right;
      }
      else
      {
        var = var->Left;
      }
    }
  }
}

/*
 * Compare the double representation of a point (x,y)
 * 
 * A: first point
 * B: second point
 */
int compareDoubles(const void *A, const void *B)
{
  double *a = (double *)A;
  double *b = (double *)B;
  if (a[1] > b[1])
  {
    return 1;
  }
  else if (b[1] > a[1])
  {
    return -1;
  }
  return 0;
}

/*
 * Create a queue from an unsorted list of point
 * 
 * points: list of all points to load (x,y) - size min 2n
 * n: number of points to load
 */
Queue *LoadEventQueue(double *points, int n)
{
  qsort(points, n, sizeof(double) * 2, compareDoubles);
  return LoadSortedEventQueue(points, n);
}
/*
 * Add a new point to the Queue
 * 
 * Q: Queue
 * x: x coordinate of the new point
 * y: y coordinate of the new point
 */

void AddPoint(Queue *Q, double x, double y, enum TYPE_EVENT type)
{
  Event *E = malloc(sizeof(Event));
  E->coordinates[0] = x;
  E->coordinates[1] = y;
  E->type = type;

  Event *P = Q->First;

  if (P == NULL)
  {
    Q->First = E;
    E->next = NULL;
    Q->Last = E;
    return;
  }

  if (E->coordinates[1] < P->coordinates[1])
  {
    E->next = P;
    Q->First = E;
    return;
  }

  while (P != NULL)
  {
    if (P->next == NULL)
    {
      P->next = E;
      E->next = NULL;
      Q->Last = E;
      return;
    }
    else
    {

      if (E->coordinates[1] < P->next->coordinates[1])
      {
        E->next = P->next;
        P->next = E;
        return;
      }
      P = P->next;
    }
  }
  E->next = NULL;
  Q->Last = E;
}

void printQueue(Queue *Q)
{
  Event *E = Q->First;
  int i = 0;
  printf("QUEUE & FIRST & LAST\n");
  printf("%p %p\n", Q->First, Q->Last);
  printf("EVENTS \n%*s X      Y      TYPE &EVENT         &NEXT\n", 4, "id");
  while (E != NULL)
  {
    printf("%*d %+2.3f %+2.3f %d    %p %p\n", 4, i, E->coordinates[0], E->coordinates[1], E->type, E, E->next);
    E = E->next;
    i++;
  }
  printf("End of Queue \n");
}