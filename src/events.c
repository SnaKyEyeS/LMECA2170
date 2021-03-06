#include "events.h"

/*
 * Return an empty event
 */
Event *initEmptyEvent()
{
  Event *e = malloc(sizeof(Event));
  e->type = SITE;
  e->node = NULL;

  e->isValid = true;
  e->circle = NULL;
  e->f = NULL;

  return e;
}

/*
 * Free an event
 */
void freeEvent(Event *e)
{
  freeCircle(e->circle);
  free(e);
}

/*
 * Print an event
 */
void printEvent(Event *e)
{
  if (e == NULL)
  {
    printf("Event is NULL\n");
    return;
  }
  printf("Event %+2.7f %+2.7f %d    %p circle: %p face: %p\n", e->coordinates[0], e->coordinates[1], e->type, e, e->circle, e->f);
}
