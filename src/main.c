#include "inputs.h"
#include <time.h>
#include "halfEdge.h"
#include "data_halfEdge.h"
#include "fortune.h"

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
			2,
			4};

	Queue *Q = LoadEventQueue(test_points, 5);
	Node **beachLine = malloc(sizeof(Node *));
	Event *e;

	printQueue(Q);
	printf("\n");
	printAllTree(*beachLine);
	printf("\n");
	for (int i = 0; i < 1; i++)
	{
		ProcessEvent(beachLine, Q);
		printf("\n");
		printAllTree(*beachLine);
		printf("\n");
		printQueue(Q);
		printf("\n");
	}

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