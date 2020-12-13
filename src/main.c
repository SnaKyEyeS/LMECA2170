#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "inputs.h"
#include "halfEdge.h"
#include "data_halfEdge.h"
#include "fortune.h"
#include "utils.h"
#include "color.h"

int main(int argc, char *argv[])
{
	int nPoints = 40;
	bool benchmark = false;
	bool shouldSaveVid = false;
	float fps = 10;
	enum TYPE_ANIM typeAnim = SWEEP_ANIM;
	int nBeachLine = 300;

	bool showSweepLine = true;
	bool showBeachLine = true;
	bool showSiteEvents = true;
	bool showCircleEvents = true;
	bool showNextEvent = true;
	bool showConstructingHe = true;
	bool showHe = true;
	bool showBox = true;
	bool showHelp = true;
	bool showTriang = true;

	struct stat st = {0};

	// Handle flags
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
		{
			printf("Fortune's algorithm and visualisation by Devillez & Poncelet \n \n");
			printf("Flags: \n");
			printf("-b, --benchmark [n]: output the time taken to apply the algorithm on the [n] points\n");
			printf("-a, --animation [mode]: specify the level of animation\n");
			printf(" * 0: No animation \n");
			printf(" * 1: Manual mode \n");
			printf(" * 2: Step mode \n");
			printf(" * 3: Sweep mode \n");
			printf("-r, --resolution [n]: specify the max number of points [n] to draw the beachline\n");
			printf("-p, --points [n]: specify the number of points [n] to use (default: 20) \n");
			printf("-s, --save: specify to save the animation\n");
			printf("-f, --fps [n]: specify the number of fps to save (default: 10)\n");
			printf("\n\n");
			return 0;
		}

		// Benchmark -> No plot | see if everything is allright
		if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--benchmark") == 0)
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

		else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--animation") == 0)
		{
			if (i + 1 >= argc)
			{
				printf("You should select the correct type of animation\n");
				printf("- 0: No animation, will only output the final result\n");
				printf("- 1: Give you manual control of the Line\n");
				printf("- 2: The animation will only go by the Event point\n");
				printf("- 3: The animation will a moving line\n");
				printf("Default: 3\n");
				return EXIT_FAILURE;
			}

			// Return 0 if not a number
			int anim = atoi(argv[i + 1]);
			if (anim < 0 || anim > SWEEP_ANIM)
			{
				printf("You should select the correct type of animation\n");
				printf("- 0: No animation, will only output the final result\n");
				printf("- 1: Give you manual control of the Line\n");
				printf("- 2: The animation will only go by the Event point\n");
				printf("- 3: The animation will a moving line\n");
				printf("Default: 3\n");
				return EXIT_FAILURE;
			}

			typeAnim = anim;

			i += 1;
		}

		else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--resolution") == 0)
		{

			if (i + 1 >= argc)
			{
				printf("You should precise the numbers of points after -r or --resolution\n");
				return EXIT_FAILURE;
			}

			// Return 0 if not a number
			nBeachLine = atoi(argv[i + 1]);
			if (nBeachLine <= 0)
			{
				printf("The number of point should be higher than 0\n");
				return EXIT_FAILURE;
			}

			i += 1;
		}

		else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--points") == 0)
		{

			if (i + 1 >= argc)
			{
				printf("You should precise the numbers of points after -p or --points\n");
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

		else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--save") == 0)
		{
			showHelp = false;
			shouldSaveVid = true;
		}

		else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fps") == 0)
		{

			if (i + 1 >= argc)
			{
				printf("You should precise the numbers of fps after -f or --fps\n");
				return EXIT_FAILURE;
			}

			// Return 0 if not a number
			fps = atof(argv[i + 1]);
			if (fps <= 0)
			{
				printf("The number of fps should be higher than 0\n");
				return EXIT_FAILURE;
			}

			i += 1;
		}
	}

	int seed = (int)time(NULL);
	srand(seed);

	if (benchmark)
	{
		coord *points = malloc(sizeof(coord) * nPoints);
		GLfloat min[2] = {-100, -100};
		GLfloat max[2] = {100, 100};
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
	/*
	float test_points[10][2] = {
			{0, 0},
			{0.1, 0.1},
			{-0.2, 0.2},
			{0.15, 0.3},
			{0.2, 0.4},
			{0.85, 0.85}};*/

	coord *test_points = malloc(sizeof(coord) * nPoints);
	GLfloat min[2] = {-0.8, -0.8};
	GLfloat max[2] = {0.8, 0.8};
	random_uniform_points(test_points, nPoints, min, max);
	qsort(test_points, nPoints, sizeof(float) * 2, comparefloats);

	FortuneStruct *data = initFortune(test_points, nPoints);

	// Only for print
	int maxCircleEvent = 10000;
	coord *pointsCircleEvent = malloc(sizeof(coord) * maxCircleEvent);
	int nCircleEvent = 0;

	int maxHE = 1000000 * 2;
	coord *pointsHe = malloc(sizeof(coord) * maxHE);
	int nHe = 0;

	int maxTriang = maxHE;
	coord *pointsTriang = malloc(sizeof(coord) * maxTriang);
	int nTriang = 0;

	coord *pointsHeConstruct = malloc(sizeof(coord) * nPoints * 10 * 2);
	int nHeConstruct = nPoints * 10 * 2;

	float xmin = -0.9;
	float xmax = 0.9;

	float sweeplineHeight = -0.9;

	float pointsSweepLine[2][2] = {
			{xmin, sweeplineHeight},
			{xmax, sweeplineHeight},
	};

	float boxPoints[5][2] = {
			{-0.9, -0.9},
			{-0.9, 0.9},
			{0.9, 0.9},
			{0.9, -0.9},
			{-0.9, -0.9}};

	float box[2][2] = {
			{-0.9, -0.9},
			{0.9, 0.9}};

	int nMaxBeachLine = nBeachLine * 10;

	coord *points = linspace(xmin, xmax, nMaxBeachLine);
	if (nBeachLine <= 0)
	{
		nBeachLine = 2;
	}
	linspaceRealloc(points, xmin, xmax, nBeachLine);

	bov_window_t *window = bov_window_new(1500, 1000, "Fortune's Algorithm - Devillez & Poncelet");
	glfwSetKeyCallback(window->self, key_callback_new);
	bov_window_set_color(window, (GLfloat[])nord0);

	float nextEvent[1][2] = {
			{0, 0}};

	bov_points_t *ptsHard = bov_points_new(test_points, nPoints, GL_STATIC_DRAW);
	bov_points_t *ptsBox = bov_points_new(boxPoints, 5, GL_STATIC_DRAW);
	bov_points_t *ptsBeachline = bov_points_new(points, nBeachLine, GL_DYNAMIC_DRAW);
	bov_points_t *ptsSweepline = bov_points_new(pointsSweepLine, 2, GL_DYNAMIC_DRAW);
	bov_points_t *ptnextEvent = bov_points_new(nextEvent, 1, GL_DYNAMIC_DRAW);
	bov_points_t *circleEvent = bov_points_new(pointsCircleEvent, nCircleEvent, GL_DYNAMIC_DRAW);
	bov_points_t *ptsHe = bov_points_new(pointsHe, nHe, GL_STATIC_DRAW);
	bov_points_t *ptsTriang = bov_points_new(pointsTriang, nTriang, GL_STATIC_DRAW);
	bov_points_t *ptsHeConstruct = bov_points_new(pointsHe, nHeConstruct, GL_STATIC_DRAW);

	bov_text_param_t parameters = {
			.outlineColor = nord3,
			.fillColor = nord4, // completely transparent
			.fontSize = 25,
			.boldness = 1,
			.spaceType = PIXEL_SPACE,
			.pos = {10, 900},
			.outlineWidth = 2};

	GLubyte helperText[] = {"Shortcuts \n- 1 finish the animation\n- 2 switch to manual mode\n- 3 switch to step mode\n- 4 switch to sweep mode\n\n- E go to next Event (MANUAL)\n- A decelerate (STEP, SWEEP)\n- B accelerate (STEP, SWEEP)\n- O Reduce resolution of the beachline\n- P augment resolution of the beachLine\n\n- Q small step of the beachline\n- S medium step of the beachline\n- D big step of the beachline\n\n- G show/hide the beachline\n- H show/hide the sweepline\n- J show/hide the next Event\n- K show/hide the constructing half-edges\n- L show/hide the triangulation\n- V show/hide the site points\n- B show/hide the half-edges\n- N show/hide the circle vents\n\n- U decrease width of elements\n- I increase width of elements\n\n- T show/hide this help"};
	bov_text_t *textHelp = bov_text_new(helperText, GL_STATIC_DRAW);
	bov_text_set_param(textHelp, parameters);

	float wHard = 0.02;
	float wCEvent = 0.02;
	float wNEvent = 0.01;
	float wBeachLine = 0.004;
	float wHe = 0.008;
	float wHeConstruct = 0.002;
	float wSweepline = 0.008;
	float wTriang = 0.002;

	float wFactor = 1;

	bov_points_set_width(ptsHard, wHard * wFactor);
	bov_points_set_width(circleEvent, wCEvent * wFactor);
	bov_points_set_width(ptnextEvent, wNEvent * wFactor);
	bov_points_set_width(ptsBeachline, wBeachLine * wFactor);
	bov_points_set_width(ptsHe, wHe * wFactor);
	bov_points_set_width(ptsHeConstruct, wHeConstruct * wFactor);
	bov_points_set_width(ptsSweepline, wSweepline * wFactor);
	bov_points_set_width(ptsTriang, wTriang * wFactor);

	bov_points_set_color(ptnextEvent, (float[4])nord15);
	bov_points_set_color(ptsHard, (float[4])nord14);
	bov_points_set_color(ptsSweepline, (float[4])nord10);
	bov_points_set_color(ptsBeachline, (float[4])nord13);
	bov_points_set_color(circleEvent, (float[4])nord5);
	bov_points_set_color(ptsHe, (float[4])nord11);
	bov_points_set_color(ptsTriang, (float[4])nord7);
	bov_points_set_color(ptsHeConstruct, (float[4])nord12);

	char screenshot_dir[64] = "";

	if (shouldSaveVid)
	{
		if (stat("Video", &st) == -1)
		{
			mkdir("Video", 0700);
		}

		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		sprintf(screenshot_dir, "Video/SCR_%d-%02d-%02dT%02d:%02d:%02d/", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		if (stat(screenshot_dir, &st) == -1)
		{
			mkdir(screenshot_dir, 0700);
		}
	}

	int nImage = 0;

	int aKey = 0, sKey = 0, dKey = 0, fKey = 0, eKey = 0, wKey = 0, qKey = 0, oKey = 0, pKey = 0, oneKey = 0, twoKey = 0, threeKey = 0, fourKey = 0;

	// Hide|Show elements
	int gKey = 0, hKey = 0, jKey = 0, kKey = 0, vKey = 0, bKey = 0, nKey = 0, mKey = 0, tKey, lKey = 0;

	int iKey = 0, uKey = 0;

	float kStep = 10000;
	int kContinuous = 1;
	int nNEvent = 2, nSweepLine = 2;

	float stepSpeed = 7000;
	float continuousSpeed = 0.004;

	bool updateDrawing = true;
	bool shouldUpdateStep = false;
	bool shouldUpdateContinuous = false;
	bool shouldUpdateManual = false;
	bool shouldComputeBB = false;
	bool BBiscomputed = false;

	if (typeAnim == NO_ANIM)
	{
		if (data->Q != NULL && data->Q->es != NULL)
		{
			fortuneAlgo(data, data->Q->es[data->Q->size - 1]->coordinates[1] + 1);
			boundingBoxBp(data->beachLine, box);

			shouldUpdateContinuous = false;
		}
		showBeachLine = false;
		showSweepLine = false;
		showNextEvent = false;
		showConstructingHe = false;
	}
	else if (typeAnim == MANUAL)
	{
		updateDrawing = true;

		sweeplineHeight = box[0][1];
		drawBeachLine(sweeplineHeight, data->beachLine, points, nBeachLine, box[0][1], box[1][1]);
	}
	else if (typeAnim == STEP_ANIM)
	{
		sweeplineHeight = box[0][1];
		drawBeachLine(sweeplineHeight, data->beachLine, points, nBeachLine, box[0][1], box[1][1]);
	}

	while (!bov_window_should_close(window))
	{
		int intTime = (int)(bov_window_get_time(window) * 10000);
		// SPEED of step
		if (intTime > kStep)
		{
			kStep += stepSpeed;
			if (typeAnim == STEP_ANIM)
			{
				shouldUpdateStep = true;
			}
		}

		//Speed of sweep line
		if (intTime > kContinuous * 30)
		{
			kContinuous += 1;
			if (typeAnim == SWEEP_ANIM)
			{
				shouldUpdateContinuous = true;
				sweeplineHeight += continuousSpeed;
			}
		}

		switch (typeAnim)
		{
		case NO_ANIM:
			// Do nothing here
			break;
		case MANUAL:
			// E key to process 1 event
			eKey = impulse(eKey, glfwGetKey(window->self, GLFW_KEY_E));
			if (eKey == 1)
			{
				if (data->Q != NULL && data->Q->es != NULL && data->Q->size > 0)
				{
					ProcessEvent(data);
					updateDrawing = true;

					if (data->Q->size == 0)
					{
						if (shouldComputeBB)
						{
							boundingBoxBp(data->beachLine, box);
							shouldComputeBB = false;
						}

						// Hide this if not needed
						showBeachLine = false;
						showSweepLine = false;
						showNextEvent = false;
						showConstructingHe = false;
					}
					else
					{
						sweeplineHeight = data->Q->es[0]->coordinates[1];
					}

					if (shouldComputeBB)
					{
						// Process all event outside the box
						while (data->Q->size != 0)
							fortuneAlgo(data, data->Q->es[data->Q->size - 1]->coordinates[1] + 1);

						boundingBoxBp(data->beachLine, box);
						shouldComputeBB = false;

						// Hide this if not needed
						showBeachLine = false;
						showSweepLine = false;
						showNextEvent = false;
						showConstructingHe = false;
					}

					if (sweeplineHeight > box[1][1])
					{
						showSweepLine = false;
					}
				}
			}

			if (shouldUpdateManual)
			{
				fortuneAlgo(data, sweeplineHeight);
				if (data->Q->size == 0)
				{
					if (shouldComputeBB)
					{
						boundingBoxBp(data->beachLine, box);
						shouldComputeBB = false;
					}

					showBeachLine = false;
					showSweepLine = false;
					showNextEvent = false;
					showConstructingHe = false;
				}
				else if (shouldComputeBB)
				{
					// Process all event outside the box
					while (data->Q->size != 0)
						fortuneAlgo(data, data->Q->es[data->Q->size - 1]->coordinates[1] + 1);

					boundingBoxBp(data->beachLine, box);
					shouldComputeBB = false;

					showBeachLine = false;
					showSweepLine = false;
					showNextEvent = false;
					showConstructingHe = false;
				}
				else if (sweeplineHeight > box[1][1])
				{
					showSweepLine = false;
				}
				updateDrawing = true;
			}
			shouldUpdateManual = false;

			break;
		case STEP_ANIM:

			// Speed effect
			wKey = impulse(wKey, glfwGetKey(window->self, GLFW_KEY_W));
			if (wKey == 1)
			{
				stepSpeed *= 0.71;
				if (stepSpeed == 0)
					stepSpeed = 1;
			}
			qKey = impulse(qKey, glfwGetKey(window->self, GLFW_KEY_Q));
			if (qKey == 1)
			{
				stepSpeed *= 1.4;
			}

			// Update
			if (shouldUpdateStep)
			{
				if (data->Q != NULL && data->Q->es != NULL && data->Q->size > 0)
				{
					ProcessEvent(data);
					printAllTree(data->beachLine);
					drawBeachLine(sweeplineHeight, data->beachLine, points, nBeachLine, box[0][1], box[1][1]);

					updateDrawing = true;
					if (data->Q->size == 0)
					{
						if (shouldComputeBB)
						{
							boundingBoxBp(data->beachLine, box);
							shouldComputeBB = false;
						}
						showBeachLine = false;
						showSweepLine = false;
						showNextEvent = false;
						showConstructingHe = false;
					}
					else if (shouldComputeBB)
					{
						// Process all event outside the box
						while (data->Q->size != 0)
							fortuneAlgo(data, data->Q->es[data->Q->size - 1]->coordinates[1] + 1);

						boundingBoxBp(data->beachLine, box);
						shouldComputeBB = false;

						showBeachLine = false;
						showSweepLine = false;
						showNextEvent = false;
						showConstructingHe = false;
					}
					else
					{
						sweeplineHeight = data->Q->es[0]->coordinates[1];
					}

					if (sweeplineHeight > box[1][1])
					{
						showSweepLine = false;
					}
				}
			}

			shouldUpdateStep = false;
			break;
		case SWEEP_ANIM:

			// Speed effect
			wKey = impulse(wKey, glfwGetKey(window->self, GLFW_KEY_W));
			if (wKey == 1)
			{
				continuousSpeed *= 1.40;
			}
			qKey = impulse(qKey, glfwGetKey(window->self, GLFW_KEY_Q));
			if (qKey == 1)
			{
				continuousSpeed *= 0.71;
			}

			//Update
			if (shouldUpdateContinuous)
			{
				fortuneAlgo(data, sweeplineHeight);
				if (data->Q->size == 0)
				{
					if (shouldComputeBB)
					{
						boundingBoxBp(data->beachLine, box);
						shouldComputeBB = false;
					}
					// Hide this if not needed
					showBeachLine = false;
					showSweepLine = false;
					showNextEvent = false;
					showConstructingHe = false;
				}
				else if (shouldComputeBB)
				{
					// Process all event outside the box
					while (data->Q->size != 0)
						fortuneAlgo(data, data->Q->es[data->Q->size - 1]->coordinates[1] + 1);

					if (shouldComputeBB)
					{
						boundingBoxBp(data->beachLine, box);
						shouldComputeBB = false;
					}
					// Hide this if not needed
					showBeachLine = false;
					showSweepLine = false;
					showNextEvent = false;
					showConstructingHe = false;
				}
				else if (sweeplineHeight > box[1][1])
				{
					showSweepLine = false;
				}

				updateDrawing = true;
			}

			shouldUpdateContinuous = false;
			break;
		}

		if (updateDrawing)
		{
			// Wait for next update
			updateDrawing = false;

			if (showHelp)
			{
				bov_text_update(textHelp, helperText);
			}
			else
			{
				bov_text_update(textHelp, (GLubyte[]){""});
			}

			if (showTriang)
			{
				nTriang = getTriangPoints(data->Voronoid, pointsTriang);
				bov_points_update(ptsTriang, pointsTriang, nTriang);
			}
			else
			{
				bov_points_update(ptsTriang, pointsTriang, 0);
			}

			if (showBeachLine)
			{
				shouldComputeBB = drawBeachLine(sweeplineHeight, data->beachLine, points, nBeachLine, box[0][1], box[1][1]); // TODO update
				bov_points_update(ptsBeachline, points, nBeachLine);
			}
			else
			{
				bov_points_update(ptsBeachline, points, 0);
			}

			if (showSweepLine)
			{
				// Update of the sweep line
				pointsSweepLine[0][1] = sweeplineHeight;
				pointsSweepLine[1][1] = sweeplineHeight;
				bov_points_update(ptsSweepline, pointsSweepLine, nSweepLine);
			}
			else
			{

				bov_points_update(ptsSweepline, pointsSweepLine, 0);
			}

			if (showNextEvent)
			{
				//update of next event
				if (data->Q->size > 0)
				{
					nextEvent[0][0] = data->Q->es[0]->coordinates[0];
					nextEvent[0][1] = data->Q->es[0]->coordinates[1];
					if (inBox(nextEvent[0][0], nextEvent[0][1], box))
						bov_points_update(ptnextEvent, nextEvent, 1);
					else
						bov_points_update(ptnextEvent, nextEvent, 0);
				}
			}
			else
			{
				bov_points_update(ptnextEvent, nextEvent, 0);
			}

			if (showConstructingHe)
			{
				nHeConstruct = drawConstructingHe(sweeplineHeight, data->beachLine, pointsHeConstruct, box);
				// TODO should assert if go over max
				bov_points_update(ptsHeConstruct, pointsHeConstruct, nHeConstruct);
			}
			else
			{
				bov_points_update(ptsHeConstruct, pointsHeConstruct, 0);
			}

			if (showCircleEvents)
			{
				// Update of circle events
				nCircleEvent = getCircleEvent(data->Q, pointsCircleEvent, 0, box);
				bov_points_update(circleEvent, pointsCircleEvent, nCircleEvent);
			}
			else
			{
				bov_points_update(circleEvent, pointsCircleEvent, 0);
			}

			if (showHe)
			{
				nHe = getHePoints(data->Voronoid, pointsHe, box);
				bov_points_update(ptsHe, pointsHe, nHe);
			}
			else
			{
				bov_points_update(ptsHe, pointsHe, 0);
			}

			if (showSiteEvents)
			{
				bov_points_update(ptsHard, test_points, nPoints);
			}
			else
			{
				bov_points_update(ptsHard, test_points, 0);
			}

			if (showBox)
			{
				bov_points_update(ptsBox, boxPoints, 5);
			}
			else
			{
				bov_points_update(ptsBox, boxPoints, 0);
			}
		}

		// General Key binding

		// switch between anim mode
		oneKey = impulse(oneKey, glfwGetKey(window->self, GLFW_KEY_1));
		if (oneKey == 1)
		{
			typeAnim = NO_ANIM;
			if (data->Q != NULL && data->Q->es != NULL)
			{
				while (data->Q->size > 0)
				{
					fortuneAlgo(data, data->Q->es[data->Q->size - 1]->coordinates[1] + 1);
				}
				if (!BBiscomputed)
				{
					boundingBoxBp(data->beachLine, box);
					BBiscomputed = true;
				}
			}
			showBeachLine = false;
			showSweepLine = false;
			showNextEvent = false;
			showConstructingHe = false;
			updateDrawing = true;
		}
		twoKey = impulse(twoKey, glfwGetKey(window->self, GLFW_KEY_2));
		if (twoKey == 1)
		{
			typeAnim = MANUAL;
		}
		threeKey = impulse(threeKey, glfwGetKey(window->self, GLFW_KEY_3));
		if (threeKey == 1)
		{
			typeAnim = STEP_ANIM;
		}
		fourKey = impulse(fourKey, glfwGetKey(window->self, GLFW_KEY_4));
		if (fourKey == 1)
		{
			typeAnim = SWEEP_ANIM;
		}

		// BeachLine resolution
		pKey = impulse(pKey, glfwGetKey(window->self, GLFW_KEY_P));
		if (pKey == 1)
		{
			nBeachLine = nBeachLine * 1.4;
			if (nBeachLine > nMaxBeachLine)
				nBeachLine = nMaxBeachLine;
			linspaceRealloc(points, xmin, xmax, nBeachLine);
			updateDrawing = true;
		}
		oKey = impulse(oKey, glfwGetKey(window->self, GLFW_KEY_O));
		if (oKey == 1)
		{
			nBeachLine = nBeachLine * 0.71;
			if (nBeachLine <= 2)
				nBeachLine = 2;
			linspaceRealloc(points, xmin, xmax, nBeachLine);
			updateDrawing = true;
		}

		// Move event
		aKey = impulse(aKey, glfwGetKey(window->self, GLFW_KEY_A));
		if (aKey == 1)
		{
			sweeplineHeight += 0.01;
			shouldUpdateManual = true;
		}
		sKey = impulse(sKey, glfwGetKey(window->self, GLFW_KEY_S));
		if (sKey == 1)
		{
			sweeplineHeight += 0.05;
			shouldUpdateManual = true;
		}
		dKey = impulse(dKey, glfwGetKey(window->self, GLFW_KEY_D));
		if (dKey == 1)
		{
			sweeplineHeight += 0.25;
			shouldUpdateManual = true;
		}

		// Hide or show elems
		gKey = impulse(gKey, glfwGetKey(window->self, GLFW_KEY_G));
		if (gKey == 1)
		{
			showBeachLine = !showBeachLine;
			updateDrawing = true;
		}

		hKey = impulse(hKey, glfwGetKey(window->self, GLFW_KEY_H));
		if (hKey == 1)
		{
			showSweepLine = !showSweepLine;
			updateDrawing = true;
		}

		jKey = impulse(jKey, glfwGetKey(window->self, GLFW_KEY_J));
		if (jKey == 1)
		{
			showNextEvent = !showNextEvent;
			updateDrawing = true;
		}

		kKey = impulse(kKey, glfwGetKey(window->self, GLFW_KEY_K));
		if (kKey == 1)
		{
			showConstructingHe = !showConstructingHe;
			updateDrawing = true;
		}

		vKey = impulse(vKey, glfwGetKey(window->self, GLFW_KEY_V));
		if (vKey == 1)
		{
			showSiteEvents = !showSiteEvents;
			updateDrawing = true;
		}

		bKey = impulse(bKey, glfwGetKey(window->self, GLFW_KEY_B));
		if (bKey == 1)
		{
			showHe = !showHe;
			updateDrawing = true;
		}

		nKey = impulse(nKey, glfwGetKey(window->self, GLFW_KEY_N));
		if (nKey == 1)
		{
			showCircleEvents = !showCircleEvents;
			updateDrawing = true;
		}

		mKey = impulse(mKey, glfwGetKey(window->self, GLFW_KEY_M));
		if (mKey == 1)
		{
			showBox = !showBox;
			updateDrawing = true;
		}

		tKey = impulse(tKey, glfwGetKey(window->self, GLFW_KEY_T));
		if (tKey == 1)
		{
			showHelp = !showHelp;
			updateDrawing = true;
		}

		lKey = impulse(lKey, glfwGetKey(window->self, GLFW_KEY_L));
		if (lKey == 1)
		{
			showTriang = !showTriang;
			updateDrawing = true;
		}

		// Change width
		uKey = impulse(uKey, glfwGetKey(window->self, GLFW_KEY_U));
		if (uKey == 1)
		{
			wFactor -= 0.1;
			bov_points_set_width(ptsHard, wHard * wFactor);
			bov_points_set_width(circleEvent, wCEvent * wFactor);
			bov_points_set_width(ptnextEvent, wNEvent * wFactor);
			bov_points_set_width(ptsBeachline, wBeachLine * wFactor);
			bov_points_set_width(ptsHe, wHe * wFactor);
			bov_points_set_width(ptsHeConstruct, wHeConstruct * wFactor);
			bov_points_set_width(ptsSweepline, wSweepline * wFactor);
			bov_points_set_width(ptsTriang, wTriang * wFactor);
			updateDrawing = true;
		}

		iKey = impulse(iKey, glfwGetKey(window->self, GLFW_KEY_I));
		if (iKey == 1)
		{
			wFactor += 0.1;
			bov_points_set_width(ptsHard, wHard * wFactor);
			bov_points_set_width(circleEvent, wCEvent * wFactor);
			bov_points_set_width(ptnextEvent, wNEvent * wFactor);
			bov_points_set_width(ptsBeachline, wBeachLine * wFactor);
			bov_points_set_width(ptsHe, wHe * wFactor);
			bov_points_set_width(ptsHeConstruct, wHeConstruct * wFactor);
			bov_points_set_width(ptsSweepline, wSweepline * wFactor);
			bov_points_set_width(ptsTriang, wTriang * wFactor);
			updateDrawing = true;
		}

		//Update of the window
		bov_points_draw(window, circleEvent, 0, nCircleEvent); // TODO not print outside the box ? or directly process them
		bov_line_strip_draw(window, ptsSweepline, 0, nSweepLine);
		bov_points_draw(window, ptnextEvent, 0, nNEvent);
		bov_line_strip_draw(window, ptsBeachline, 0, nBeachLine);
		bov_lines_draw(window, ptsTriang, 0, nTriang);
		bov_lines_draw(window, ptsHe, 0, nHe);
		bov_lines_draw(window, ptsHeConstruct, 0, nHeConstruct);
		bov_line_strip_draw(window, ptsBox, 0, 5);

		bov_points_draw(window, ptsHard, 0, nPoints);
		bov_text_draw(window, textHelp);
		bov_window_update(window);

		if (shouldSaveVid)
		{
			int t = (nImage * 10000 / fps);
			if (intTime > t)
			{
				nImage += 1;
				char scr[64] = "";
				char ext[10] = "";
				strcpy(scr, screenshot_dir);
				sprintf(ext, "%05d.ppm", nImage);
				strcat(scr, ext);
				bov_window_screenshot(window, scr);
			}
		}
	}

	bov_points_delete(ptsBox);
	bov_points_delete(ptsHard);
	bov_points_delete(ptsBeachline);
	bov_points_delete(ptsSweepline);
	bov_points_delete(ptnextEvent);
	bov_points_delete(circleEvent);
	bov_points_delete(ptsHe);
	bov_points_delete(ptsTriang);
	bov_points_delete(ptsHeConstruct);
	bov_text_delete(textHelp);

	bov_window_delete(window);

	freeFortuneStruct(data);

	free(pointsCircleEvent);
	free(pointsHe);
	free(pointsHeConstruct);
	free(points);

	return EXIT_SUCCESS;
}
