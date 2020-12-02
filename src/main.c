#include <sys/time.h>
#include <time.h>
#include <stdbool.h>

#include "inputs.h"
#include "halfEdge.h"
#include "data_halfEdge.h"
#include "fortune.h"
#include "utils.h"

int main(int argc, char *argv[])
{
	int nPoints = 0;
	bool benchmark = false;

	// Handle flags
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--benchmark"))
		{
			benchmark = true;
			if (i + 1 >= argc)
			{
				printf("You should precise the numbers of points after -b or --benchmark\n");
				return EXIT_FAILURE;
			}

			// Return 0 if not a number
			nPoints = atoi(argv[i + 1]);
			if (nPoints <= 0)
			{
				printf("The number of point should be higher than 0\n");
				return EXIT_FAILURE;
			}

			i += 1;
		}
	}

	/*
	int seed = (int)time(NULL);
	srand(seed);*/

	if (benchmark)
	{
		coord *points = malloc(sizeof(coord) * nPoints);
		GLfloat min[2] = {-1, -1};
		GLfloat max[2] = {1, 1};
		random_uniform_points(points, nPoints, min, max);
		struct timeval stop, start;

		gettimeofday(&start, NULL);
		FortuneStruct *data = initFortune(points, nPoints);
		fortuneAlgo(data, 100);
		gettimeofday(&stop, NULL);

		printf("%lu\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

		freeFortuneStruct(data);
		free(points);
		return EXIT_SUCCESS;
	}

	int p = 5;

	/*float test_points[5][2] = {
			{0, 0},
			{1, 1},
			{-2, 2},
			{1.5, 3},
			{2, 4}};*/
	/*
	float test_points[11][2] = {
			{0.1, -0.6},
			{-0.11, -0.55},
			{0.5, -0.5},
			{-0.2, -0.4},
			{0.5, 0},
			{0.3, 0.001},
			{-0.5, 0.002},
			{-0.4, 0.2},
			{0.55, 0.375},
			{0.55, 0.425},
			{0, 0.6}};
*/
	coord *test_points = malloc(sizeof(coord) * p);
	GLfloat min[2] = {-0.8, -0.8};
	GLfloat max[2] = {0.8, 0.8};
	random_uniform_points(test_points, p, min, max);
	FortuneStruct *data = initFortune(test_points, p);

	int resPoints = 1000000;

	// Only for print
	int maxCircleEvent = 10000;
	coord *pointsCircleEvent = malloc(sizeof(coord) * maxCircleEvent);
	int nCircleEvent = 0;

	int maxHE = 1000000 * 2;
	coord *pointsHe = malloc(sizeof(coord) * maxHE);
	int nHe = 0;

	float sweeplineHeight = -1;
	float xmin = -20;
	float xmax = 20;

	float pointsSweepLine[2][2] = {
			{xmin, sweeplineHeight},
			{xmax, sweeplineHeight},
	};

	coord *points = linspace(xmin, xmax, resPoints);
	//ProcessEvent(beachLine, Q);
	drawBeachLine(sweeplineHeight, data->beachLine, points, resPoints);

	// give a bit of entropy for the seed of rand()
	// or it will always be the same sequence

	bov_window_t *window = bov_window_new(800, 800, "Fortune's Algorithm - Devillez & Poncelet");
	bov_window_set_color(window, (GLfloat[]){0.9f, 0.85f, 0.8f, 1.0f});

	float nextEvent[1][2] = {
			{0, 0}};

	bov_points_t *ptsHard = bov_points_new(test_points, p, GL_STATIC_DRAW);
	bov_points_t *ptsBeachline = bov_points_new(points, resPoints, GL_DYNAMIC_DRAW);
	bov_points_t *ptsSweepline = bov_points_new(pointsSweepLine, 2, GL_DYNAMIC_DRAW);
	bov_points_t *ptnextEvent = bov_points_new(nextEvent, 1, GL_DYNAMIC_DRAW);
	bov_points_t *circleEvent = bov_points_new(pointsCircleEvent, nCircleEvent, GL_DYNAMIC_DRAW);
	bov_points_t *ptsHe = bov_points_new(pointsHe, nHe, GL_STATIC_DRAW);

	bov_points_draw(window, ptsHard, 0, p);
	bov_points_set_width(ptsHard, 0.02);
	bov_points_set_width(circleEvent, 0.02);
	bov_points_set_width(ptnextEvent, 0.01);
	bov_points_set_width(ptsBeachline, 0.001);
	bov_points_set_width(ptsHe, 0.008);
	bov_points_set_width(ptsSweepline, 0.008);
	bov_points_set_color(ptnextEvent, (float[4]){0.1, 0.8, 0.1, 1});
	bov_points_set_color(ptsHard, (float[4]){1, 0, 0, 1});
	bov_points_set_color(ptsSweepline, (float[4]){0, 0, 1, 1});
	bov_points_set_color(circleEvent, (float[4]){1, 0, 1, 1});
	bov_points_set_color(ptsHe, (float[4]){1, 1, 0, 1});

	int aKey = 0, sKey = 0, dKey = 0, fKey = 0, eKey = 0, wKey = 0, qKey = 0;
	float oldSweepLine = sweeplineHeight - 1;
	int k = 1;
	while (!bov_window_should_close(window))
	{
		int intTime = (int)bov_window_get_time(window) * 1000;
		if (intTime > k * 3)
		{
			k += 1;
			sweeplineHeight += 0.018;
			pointsSweepLine[0][1] = sweeplineHeight;
			pointsSweepLine[1][1] = sweeplineHeight;
			bov_points_update(ptsSweepline, pointsSweepLine, 2);
			/*
			char str[80];
			sprintf(str, "vid/V0-image-%8d.ppm", k);
			puts(str);
			bov_window_screenshot(window, str);*/
		}
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
			sweeplineHeight += 0.0000001;
			pointsSweepLine[0][1] = sweeplineHeight;
			pointsSweepLine[1][1] = sweeplineHeight;
			bov_points_update(ptsSweepline, pointsSweepLine, 2);
		}
		dKey = impulse(dKey, glfwGetKey(window->self, GLFW_KEY_D));
		if (dKey == 1)
		{
			sweeplineHeight += 1;
			pointsSweepLine[0][1] = sweeplineHeight;
			pointsSweepLine[1][1] = sweeplineHeight;
			bov_points_update(ptsSweepline, pointsSweepLine, 2);
		}
		fKey = impulse(fKey, glfwGetKey(window->self, GLFW_KEY_F));
		if (fKey == 1)
		{
			sweeplineHeight += 50;
			pointsSweepLine[0][1] = sweeplineHeight;
			pointsSweepLine[1][1] = sweeplineHeight;
			bov_points_update(ptsSweepline, pointsSweepLine, 2);
		}
		eKey = impulse(eKey, glfwGetKey(window->self, GLFW_KEY_E));
		if (eKey == 1)
		{
			printAllTree(data->beachLine);
		}
		wKey = impulse(wKey, glfwGetKey(window->self, GLFW_KEY_W));
		if (wKey == 1)
		{
			printQueue(data->Q);
		}
		qKey = impulse(qKey, glfwGetKey(window->self, GLFW_KEY_Q));
		if (qKey == 1)
		{
			printf("\n");
			printPolygonMesh(data->Voronoid);
			printf("\n");
		}

		if (sweeplineHeight > oldSweepLine)
		{
			fortuneAlgo(data, sweeplineHeight);
			nCircleEvent = getCircleEvent(data->Q, pointsCircleEvent, 0);
			bov_points_update(circleEvent, pointsCircleEvent, nCircleEvent);
			if (data->Q->e != NULL)
			{
				nextEvent[0][0] = data->Q->e->coordinates[0];
				nextEvent[0][1] = data->Q->e->coordinates[1];
				bov_points_update(ptnextEvent, nextEvent, 1);
			}
			else
			{
				boundingBoxBp(data->beachLine);
			}
			nHe = getHePoints(data->Voronoid, pointsHe);
			bov_points_update(ptsHe, pointsHe, nHe);
			drawBeachLine(sweeplineHeight, data->beachLine, points, resPoints);
			bov_points_update(ptsBeachline, points, resPoints);
		}

		bov_points_draw(window, circleEvent, 0, nCircleEvent);
		bov_line_strip_draw(window, ptsSweepline, 0, 2);
		bov_points_draw(window, ptsHard, 0, p);
		bov_points_draw(window, ptnextEvent, 0, 2);
		bov_line_strip_draw(window, ptsBeachline, 0, resPoints);
		bov_lines_draw(window, ptsHe, 0, nHe);
		bov_window_update(window);

		oldSweepLine = sweeplineHeight;
	}

	bov_window_delete(window);

	bov_points_delete(ptsHard);
	bov_points_delete(ptsBeachline);
	bov_points_delete(ptsSweepline);
	bov_points_delete(ptnextEvent);
	bov_points_delete(circleEvent);
	bov_points_delete(ptsHe);

	freeFortuneStruct(data);

	free(pointsCircleEvent);
	free(pointsHe);
	free(points);

	return EXIT_SUCCESS;
}