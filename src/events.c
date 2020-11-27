#include "events.h"

/*
 * Free an event
 */
void freeEvent(Event *e)
{
  freeCircle(e->circle);
  free(e);
}

void printEvent(Event *e)
{
  if (e == NULL)
  {
    printf("Event is NULL\n");
    return;
  }
  printf("Event %+2.3f %+2.3f %d    %p %p\n", e->coordinates[0], e->coordinates[1], e->type, e, e->next);
}
