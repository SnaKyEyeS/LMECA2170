#include "inputs.h"
#include <time.h>
#include "halfEdge.h"
#include "data_halfEdge.h"

int main()
{

	// Test 4 half-edges

	PolygonMesh *Test = LoadPolygonMesh(two_square_vertices, 6, two_square_hEdges, 8, 2);
	printPolygonMesh(Test);

	// give a bit of entropy for the seed of rand()
	// or it will always be the same sequence
	int seed = (int)time(NULL);
	srand(seed);

	// we print the seed so you can get the distribution of points back
	printf("seed=%d\n", seed);

	bov_window_t *window = bov_window_new(800, 800, "My first BOV program");
	bov_window_set_color(window, (GLfloat[]){0.9f, 0.85f, 0.8f, 1.0f});

	const GLsizei nPoints = 16;
	GLfloat(*coord)[2] = malloc(sizeof(coord[0]) * nPoints);
#if 0 // put 1 for random polygon
	random_polygon(coord, nPoints, 4);
#else
	random_points(coord, nPoints);
#endif

	bov_points_t *coordDraw = getHalfEdgesBOVPolygonMesh(Test, 0.0);
	bov_points_t *coordDraw2 = getHalfEdgesBOVPolygonMesh(Test, 0.03);
	bov_points_set_color(coordDraw, (GLfloat[4]){0.0, 0.0, 0.0, 1.0});
	bov_points_set_color(coordDraw2, (GLfloat[4]){0.3, 0.0, 0.0, 1.0});
	bov_points_set_outline_color(coordDraw, (GLfloat[4]){0.3, 0.12, 0.0, 0.25});

	while (!bov_window_should_close(window))
	{
		bov_points_set_width(coordDraw, 0.003);
		bov_points_set_outline_width(coordDraw, 0.002);
		bov_points_set_width(coordDraw2, 0.004);
		bov_lines_draw(window, coordDraw, 0, nPoints);

		// points_set_width(coordDraw, 0.003);
		bov_points_set_outline_width(coordDraw, -1.);
		bov_points_draw(window, coordDraw, 0, nPoints);

		bov_lines_draw(window, coordDraw2, 0, 8 * 2);

		bov_window_update(window);
	}

	bov_points_delete(coordDraw);
	bov_points_delete(coordDraw2);
	free(coord);
	bov_window_delete(window);

	FreePolygonMesh(Test);

	return EXIT_SUCCESS;
}