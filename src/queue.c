#include "queue.h"

/*
 * Create a Q form a sorted list of point
 * 
 * points: list of all sorted points to load (x,y) - size min 2n
 * n: number of points to load
 */
Queue *LoadSortedEventQueue(coord *points, int a, int n, Face **f, Queue *root)
{
  Queue *Q = initQueueNode();
  if (n == 0)
  {
    return Q;
  }

  int b = n - 1;
  int bl = floor((a + 1 + b) / 2);
  int ar = bl + 1;

  Q->e = initEmptyEvent();
  Q->e->f = f[a];
  Q->e->coordinates[0] = points[a][0];
  Q->e->coordinates[1] = points[a][1];
  Q->Root = root;

  if (bl >= a + 1)
  {
    Q->Left = LoadSortedEventQueue(points, a + 1, ar, f, Q);
  }

  if (b >= ar)
  {
    Q->Right = LoadSortedEventQueue(points, ar, n, f, Q);
  }

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
  return LoadSortedEventQueue(points, 0, n, f, NULL);
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

  if (Q->e == NULL)
  {
    Q->e = initEmptyEvent();
    Q->e->coordinates[0] = x;
    Q->e->coordinates[1] = y;
    Q->e->f = f;
    Q->e->type = type;
    return Q->e;
  }

  Queue *add = initQueueNode();
  Event *e = initEmptyEvent();
  add->e = e;
  add->e->coordinates[0] = x;
  add->e->coordinates[1] = y;
  add->e->f = f;
  add->e->type = type;

  Queue *var = Q;

  while (var->Left != NULL && var->Right != NULL)
  {
    var = var->Right;
  }
  if (var->Left == NULL)
  {
    var->Left = add;
  }
  else
  {
    var->Right = add;
  }

  add->Root = var;

  int comp = comparefloats(add->e->coordinates, var->e->coordinates);

  while (comp < 0)
  {
    Event *tmp = add->e;
    add->e = var->e;
    var->e = tmp;
    add = var;
    var = add->Root;
    if (var == NULL)
    {
      comp = 1;
    }
    else
    {
      comp = comparefloats(add->e->coordinates, var->e->coordinates);
    }
  }

  return e;
}

/*
 * Delete an event from Q
 */
void deleteEvent(Queue *Q, Event *e)
{

  if (e == NULL)

    return;
  if (Q == NULL)
  {
    freeEvent(e);
    return;
  }
  if (Q->e == NULL)
  {
    freeEvent(e);
    return;
  }

  Queue *var = Q;
  int comp = comparefloats(e, var->e);
  while (var->Left != NULL && var->Right != NULL)
  {
    if (comp < 0)
    {
      var = var->Left;
    }
    else if (comp == 0)
    {
      freeEvent(popQueue(var));
    }
    else
    {
      var = var->Right;
    }
    comp = comparefloats(e, var->e);
  }

  if (var->e != e)
  {
    printf("ERROR ON THE TREE EVENT NOT FOUND \n");
    freeEvent(e);
    return;
  }
  freeQueue(var);
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

  // Empty Queue
  if (Q->e == NULL)
  {
    return NULL;
  }

  if (Q->Left == NULL && Q->Right == NULL)
  {
    Event *e = Q->e;
    Q->e = NULL;
    return e;
  }

  Event *e = Q->e;
  if (Q->Left == NULL)
  {
    Queue *toDel = Q->Right;
    Q->e = Q->Right->e;
    Q->Left = Q->Right->Left;
    Q->Right = Q->Right->Right;

    if (Q->Left != NULL)
      Q->Left->Root = Q;
    if (Q->Right != NULL)
      Q->Right->Root = Q;
    freeNodeQueue(toDel);
    return e;
  }
  if (Q->Right == NULL)
  {
    Queue *toDel = Q->Left;
    Q->e = Q->Left->e;
    Q->Right = Q->Left->Right;
    Q->Left = Q->Left->Left;
    if (Q->Left != NULL)
      Q->Left->Root = Q;
    if (Q->Right != NULL)
      Q->Right->Root = Q;
    freeNodeQueue(toDel);
    return e;
  }
  Queue *var = Q->Left;

  while (var->Right != NULL || var->Left != NULL)
  {
    if (var->Right != NULL)
      var = var->Right;
    else
      var = var->Left;
  }
  Q->e = var->e;

  if (var->Root->Left == var)
  {
    var->Root->Left = NULL;
  }
  else
  {
    var->Root->Right = NULL;
  }

  var = Q;
  while (true)
  {
    if (var->Left == NULL && var->Right == NULL)
    {
      return e;
    }

    if (var->Left == NULL)
    {
      int comb = comparefloats(var->e->coordinates, var->Right->e->coordinates);
      if (comb < 0)
      {
        return e;
      }
      Event *tmp = var->e;
      var->e = var->Right->e;
      var->Right->e = tmp;
      var = var->Right;
    }
    else if (var->Right == NULL)
    {
      int comb = comparefloats(var->e->coordinates, var->Left->e->coordinates);
      if (comb < 0)
      {
        return e;
      }
      Event *tmp = var->e;
      var->e = var->Left->e;
      var->Left->e = tmp;
      var = var->Left;
    }
    else
    {
      int subComp = comparefloats(var->Left->e, var->Right->e);
      if (subComp < 0)
      {
        int comb = comparefloats(var->e->coordinates, var->Left->e->coordinates);
        Event *tmp = var->e;
        var->e = var->Left->e;
        var->Left->e = tmp;
        var = var->Left;
      }
      else
      {
        int comb = comparefloats(var->e->coordinates, var->Right->e->coordinates);
        Event *tmp = var->e;
        var->e = var->Right->e;
        var->Right->e = tmp;
        var = var->Right;
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
  if (Q->e == NULL)
  {
    return start;
  }

  int a = getCircleEvent(Q->Left, points, start);
  if (Q->e->type == CIRCLE)
  {
    points[a][0] = Q->e->coordinates[0];
    points[a][1] = Q->e->coordinates[1];
    a++;
  }
  return getCircleEvent(Q->Right, points, a);
}

/*
 * Free just the node itSel
 */
void freeNodeQueue(Queue *Q)
{
  free(Q);
}

/*
 * Free everything inside the Queue and then the queue itself
 */
void freeQueue(Queue *Q)
{

  if (Q == NULL)
  {
    return;
  }

  if (Q->Left != NULL)
  {
    freeQueue(Q->Left);
  }

  if (Q->Right != NULL)
  {
    freeQueue(Q->Right);
  }

  freeEvent(Q->e);
  freeNodeQueue(Q);
}

/*
 * print a node of the queue
 */
void printNodeQueue(Queue *Q, int i)
{
  if (Q != NULL && Q->e == NULL)
  {
    printf("Queue is empty\n");
    return;
  }

  for (int a = 0; a < i; a++)
  {
    printf("  ");
  }

  if (Q == NULL)
  {
    printf("    | NULL (queue)\n");
    return;
  }

  printf("%3d | End Node %+f %+f  | %p | %p | %p | %p | Event %p\n", i, Q->e->coordinates[0], Q->e->coordinates[1], Q->Root, Q, Q->Left, Q->Right, Q->e);

  printNodeQueue(Q->Left, i + 1);

  printNodeQueue(Q->Right, i + 1);

  return;
}

/*
 * Print the Queue
 */
void printQueue(Queue *Q)
{
  printNodeQueue(Q, 0);
}

/*
 * Create an empty Queue node
 */
Queue *initQueueNode()
{
  Queue *Q = malloc(sizeof(Queue));
  Q->e = NULL;
  Q->Left = NULL;
  Q->Right = NULL;
  return Q;
}