#include "inputs.h"
#include <time.h>
#include "halfEdge.h"
#include "data_halfEdge.h"
#include "fortune.h"
#include "utils.h"

int main()
{

	float test_points[5][2] = {
			{0, 0},
			{1, 1},
			{-2, 2},
			{1.5, 3},
			{2, 4}};

	int nPoints = 400;

	float sweeplineHeight = 1;
	float xmin = -5;
	float xmax = 5;

	float pointsSweepLine[2][2] = {
			{xmin, sweeplineHeight},
			{xmax, sweeplineHeight},
	};

	Queue *Q = LoadEventQueue(test_points, 5);
	Node **beachLine = malloc(sizeof(Node *));
	Event *e;

	for (int i = 0; i < 0; i++)
	{
		ProcessEvent(beachLine, Q);
		printf("\n");
		printAllTree(*beachLine);
		printf("\n");
		printQueue(Q);
		printf("\n");
	}

	coord *points = linspace(-5, 5, nPoints);
	//ProcessEvent(beachLine, Q);
	drawBeachLine(sweeplineHeight, *beachLine, points, nPoints);

	// give a bit of entropy for the seed of rand()
	// or it will always be the same sequence
	int seed = (int)time(NULL);
	srand(seed);
	bov_window_t *window = bov_window_new(800, 800, "Fortune's Algorithm - Devillez & Poncelet");
	bov_window_set_color(window, (GLfloat[]){0.9f, 0.85f, 0.8f, 1.0f});

	float nextEvent[1][2] = {
			{0, 0}};

	bov_points_t *ptsHard = bov_points_new(test_points, 5, GL_STATIC_DRAW);
	bov_points_t *ptsBeachline = bov_points_new(points, nPoints, GL_DYNAMIC_DRAW);
	bov_points_t *ptsSweepline = bov_points_new(pointsSweepLine, 2, GL_DYNAMIC_DRAW);
	bov_points_t *ptnextEvent = bov_points_new(nextEvent, 1, GL_DYNAMIC_DRAW);

	bov_points_draw(window, ptsHard, 0, 5);
	bov_points_set_width(ptsHard, 0.1);
	bov_points_set_width(ptnextEvent, 0.05);
	bov_points_set_color(ptnextEvent, (float[4]){0.1, 0.8, 0.1, 1});
	bov_points_set_color(ptsHard, (float[4]){1, 0, 0, 1});
	bov_points_set_color(ptsSweepline, (float[4]){0, 0, 1, 1});

	int aKey = 0, sKey = 0, eKey = 0, wKey = 0;

	while (!bov_window_should_close(window))
	{
		float oldSweepLine = sweeplineHeight;
		aKey = impulse(aKey, glfwGetKey(window->self, GLFW_KEY_A));
		if (aKey == 1)
		{
			sweeplineHeight += 0.1;
			pointsSweepLine[0][1] = sweeplineHeight;
			pointsSweepLine[1][1] = sweeplineHeight;
			bov_points_update(ptsSweepline, pointsSweepLine, 2);
		}
		sKey = impulse(sKey, glfwGetKey(window->self, GLFW_KEY_S));
		if (sKey == 1)
		{
			sweeplineHeight += 0.05;
			pointsSweepLine[0][1] = sweeplineHeight;
			pointsSweepLine[1][1] = sweeplineHeight;
			bov_points_update(ptsSweepline, pointsSweepLine, 2);
		}
		eKey = impulse(eKey, glfwGetKey(window->self, GLFW_KEY_E));
		if (eKey == 1)
		{
			printAllTree(*beachLine);
		}
		wKey = impulse(wKey, glfwGetKey(window->self, GLFW_KEY_W));
		if (wKey == 1)
		{
			printQueue(Q);
		}

		if (sweeplineHeight > oldSweepLine)
		{
			while (Q != NULL && Q->First->coordinates[1] < sweeplineHeight)
			{
				printf("Main loop \n");
				ProcessEvent(beachLine, Q);
				printAllTree(*beachLine);
				printQueue(Q);
				printf("\n SPACER \n ");
				if (Q->First != NULL)
				{
					nextEvent[0][0] = Q->First->coordinates[0];
					nextEvent[0][1] = Q->First->coordinates[1];
					bov_points_update(ptnextEvent, nextEvent, 1);
				}
				printf("End of main loop \n \n");
			}
			drawBeachLine(sweeplineHeight, *beachLine, points, nPoints);
			bov_points_update(ptsBeachline, points, nPoints);
		}

		bov_line_strip_draw(window, ptsSweepline, 0, 2);
		bov_points_draw(window, ptsHard, 0, 5);
		bov_points_draw(window, ptnextEvent, 0, 2);
		bov_line_strip_draw(window, ptsBeachline, 0, nPoints);
		bov_window_update(window);
	}

	bov_window_delete(window);

	return EXIT_SUCCESS;
}