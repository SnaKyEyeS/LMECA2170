#include "queue.h"

/*
 * Create a Q form a sorted list of point
 * 
 * points: list of all sorted points to load (x,y) - size min 2n
 * n: number of points to load
 */
Queue *LoadSortedEventQueue(coord *points, int n, Face **f)
{
  Queue *Q = malloc(sizeof(Queue));
  Q->max = 10 * n;
  Q->es = malloc(sizeof(Event *) * Q->max);
  for (int i = 0; i < n; i++)
  {
    Event *e = initEmptyEvent();
    e->coordinates[0] = points[i][0];
    e->coordinates[1] = points[i][1];
    e->f = f[i];
    Q->es[i] = e;
  }

  Q->size = n;

  return Q;
}

/*
 * Create a queue from an unsorted list of point
 * 
 * points: list of all points to load (x,y) - size min 2n
 * n: number of points to load
 */
Queue *LoadEventQueue(coord *points, int n, Face **f)
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
  Event *e = malloc(sizeof(Event)); // TODO impr
  e->coordinates[0] = x;
  e->coordinates[1] = y;
  e->f = f;
  e->type = type;
  e->circle = NULL;
  e->node = NULL;
  e->isValid = true;

  Q->es[Q->size] = e;
  Q->size++;
  int idx = Q->size;
  while (idx > 1)
  {
    int var = (idx >> 1) - 1;
    if (Q->es[var]->coordinates[1] < y)
    {
      return e;
    }
    else
    {
      Q->es[idx - 1] = Q->es[var];
      Q->es[var] = e; // TODO improve by affecting only once ?
    }
    idx = var + 1;
  }
  return e;
}

/*
 * Pop an element of the queue
 * 
 */
Event *popQueue(Queue *Q)
{
  //_improve with memory of the depth
  // NULL Queue
  if (Q == NULL)
  {
    return NULL;
  }

  if (Q->size == 0)
  {
    return NULL;
  }

  Event *e = Q->es[0];
  Q->size--;
  if (Q->size == 0)
    return e;
  Q->es[0] = Q->es[Q->size];

  int idx = 1;
  while (idx < Q->size)
  {
    int val = idx << 1;
    int idx_off = idx - 1;
    int val_off = val - 1;

    // End of heap
    if (val_off > Q->size)
      return e;

    // Left is smaller
    if (Q->es[idx_off]->coordinates[1] > Q->es[val_off]->coordinates[1])
    {
      if (val < Q->size && Q->es[idx_off]->coordinates[1] > Q->es[val]->coordinates[1]) // Right is smaller
      {
        if (Q->es[val_off]->coordinates[1] < Q->es[val]->coordinates[1])
        {
          // Left is less
          Event *swp = Q->es[idx_off];
          Q->es[idx_off] = Q->es[val_off];
          Q->es[val_off] = swp;
          idx = val;
        }
        else
        {
          // Right is less
          Event *swp = Q->es[idx_off];
          Q->es[idx_off] = Q->es[val];
          Q->es[val] = swp;
          idx = val + 1;
        }
      }
      else
      {
        // Left is less
        Event *swp = Q->es[idx_off];
        Q->es[idx_off] = Q->es[val_off];
        Q->es[val_off] = swp;
        idx = val;
      }
    }
    else
    {                                                                                   // Left is not smaller
      if (val < Q->size && Q->es[idx_off]->coordinates[1] > Q->es[val]->coordinates[1]) // Right is smaller
      {
        Event *swp = Q->es[idx_off];
        Q->es[idx_off] = Q->es[val];
        Q->es[val] = swp;
        idx = val + 1;
      }
      else
      {
        return e;
      }
    }
  }
  return e;
}

/*
 * Get the elements of Q which are circle events into points and return the number of elemens
 */
int getCircleEvent(Queue *Q, coord *points, int start)
{
  if (Q == NULL)
  {
    return start;
  }

  int a = 0;
  for (int i = 0; i < Q->size; i++)
  {
    if (Q->es[i]->type == CIRCLE)
    {
      points[a][0] = Q->es[i]->coordinates[0];
      points[a][1] = Q->es[i]->coordinates[1];
      a++;
    }
  }

  return a;
}

/*
 * Free just the node itSel
 */

/*
 * Free everything inside the Queue and then the queue itself
 */
void freeQueue(Queue *Q)
{
  if (Q == NULL)
  {
    return;
  }

  for (int i = 0; i < Q->size; i++)
  {
    freeEvent(Q->es[i]);
  }
  free(Q->es);
  free(Q);
}

/*
 * Print the Queue
 */
void printQueue(Queue *Q)
{
  if (Q == NULL)
  {
    printf("Queue is null\n");
    return;
  }

  if (Q->size == 0)
  {
    printf("Queue is empty\n");
    return;
  }

  printf("Start of Queue  | %d\n", Q->size);
  for (int i = 0; i < Q->size; i++)
  {
    printf("%3d | %f %f | %d | %p\n", i, Q->es[i]->coordinates[0], Q->es[i]->coordinates[1], Q->es[i]->type, Q->es[i]);
  }
  printf("End of Queue \n \n");
}
