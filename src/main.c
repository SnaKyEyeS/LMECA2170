#include "inputs.h"
#include <time.h>
#include "halfEdge.h"
#include "data_halfEdge.h"
#include "events.h"

int main()
{

	double test_points[10] = {
			0,
			0,
			1,
			1,
			-2,
			2,
			1.5,
			3,
			4,
			2,
	};

	Queue *Q = LoadEventQueue(test_points, 5);
	Node *BeachLine = NULL;
	Event *e;

	printQueue(Q);
	printf("\n");
	printAllTree(BeachLine);
	printf("\n");
	e = popQueue(Q);
	printEvent(e);
	ProcessSite(e);
	printf("\n");
	printAllTree(BeachLine);
	printf("\n");
	printQueue(Q);

	// give a bit of entropy for the seed of rand()
	// or it will always be the same sequence
	int seed = (int)time(NULL);
	srand(seed);
	/*
	bov_window_t *window = bov_window_new(800, 800, "Fortune's Algorithm - Devillez & Poncelet");
	bov_window_set_color(window, (GLfloat[]){0.9f, 0.85f, 0.8f, 1.0f});

	while (!bov_window_should_close(window))
	{

		bov_window_update(window);
	}

	bov_window_delete(window);
  */
	return EXIT_SUCCESS;
}