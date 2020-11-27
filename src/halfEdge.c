#include "halfEdge.h"

/*
 * Vertices -> contain position of each Vertex
 * hEdges -> Vertex Face Eprev Enext Eopp
 */
PolygonMesh *LoadPolygonMesh(float *Vertices, int nVertices, int *hEdges, int nEdges, int nFaces)
{
  PolygonMesh *PM = malloc(sizeof(PolygonMesh));

  PM->nFaces = nFaces;
  PM->faces = malloc(sizeof(Face *) * nFaces);

  for (int i = 0; i < nFaces; i++)
  {
    PM->faces[i] = malloc(sizeof(Face));
    PM->faces[i]->he = NULL;
  }

  PM->nVertices = nVertices;
  PM->vertices = malloc(sizeof(Vertex *) * nVertices);

  for (int i = 0; i < nVertices; i++)
  {
    PM->vertices[i] = malloc(sizeof(Vertex));
    PM->vertices[i]->coordinates[0] = Vertices[i * 2];
    PM->vertices[i]->coordinates[1] = Vertices[i * 2 + 1];
    PM->vertices[i]->he = NULL;
  }

  // No edge are boundary -> worst case
  // Will reshape after i guess
  PM->nHEdges = nEdges;
  PM->hedges = malloc(sizeof(HalfEdge *) * nEdges);
  for (int i = 0; i < nEdges; i++)
  {
    PM->hedges[i] = malloc(sizeof(HalfEdge));
  }
  for (int i = 0; i < nEdges; i++)
  {
    PM->hedges[i]->v = PM->vertices[hEdges[i * 5]];
    if (PM->vertices[hEdges[i * 5]]->he == NULL)
    {
      PM->vertices[hEdges[i * 5]]->he = PM->hedges[i];
    }
    PM->hedges[i]->f = PM->faces[hEdges[i * 5 + 1]];
    if (PM->faces[hEdges[i * 5 + 1]]->he == NULL)
    {
      PM->faces[hEdges[i * 5 + 1]]->he = PM->hedges[i];
    }
    PM->hedges[i]->prev = PM->hedges[hEdges[i * 5 + 2]];
    PM->hedges[i]->next = PM->hedges[hEdges[i * 5 + 3]];
    if (hEdges[i * 5 + 4] == -1)
    {
      PM->hedges[i]->Opposite = NULL;
    }
    else
    {
      PM->hedges[i]->Opposite = PM->hedges[hEdges[i * 5 + 4]];
    }
  }

  return PM;
}

bov_points_t *getVerticesBOVPolygonMesh(PolygonMesh *PM)
{
  GLfloat coord[PM->nVertices][2];
  for (int i = 0; i < PM->nVertices; i++)
  {
    coord[i][0] = PM->vertices[i]->coordinates[0];
    coord[i][1] = PM->vertices[i]->coordinates[1];
  }
  return bov_points_new(coord, PM->nVertices, GL_STATIC_DRAW);
}

bov_points_t *getHalfEdgesBOVPolygonMesh(PolygonMesh *PM, float fact)
{
  GLfloat coord[PM->nHEdges * 2][2];
  int nPts = 0;
  for (int i = 0; i < PM->nFaces; i++)
  {

    float center[2];
    center[0] = PM->faces[i]->he->v->coordinates[0];
    center[1] = PM->faces[i]->he->v->coordinates[1];
    int n = 1;

    HalfEdge *next = PM->faces[i]->he->next;
    while (next != PM->faces[i]->he)
    {

      center[0] += next->v->coordinates[0];
      center[1] += next->v->coordinates[1];
      n += 1;
      next = next->next;
    }

    center[0] /= n;
    center[1] /= n;

    next = PM->faces[i]->he->next;
    coord[nPts][0] = PM->faces[i]->he->v->coordinates[0] + (center[0] - PM->faces[i]->he->v->coordinates[0]) * fact;
    coord[nPts][1] = PM->faces[i]->he->v->coordinates[1] + (center[1] - PM->faces[i]->he->v->coordinates[1]) * fact;
    nPts += 1;
    while (next != PM->faces[i]->he)
    {

      coord[nPts][0] = next->v->coordinates[0] + (center[0] - next->v->coordinates[0]) * fact;
      coord[nPts][1] = next->v->coordinates[1] + (center[1] - next->v->coordinates[1]) * fact;
      coord[nPts + 1][0] = coord[nPts][0];
      coord[nPts + 1][1] = coord[nPts][1];

      nPts += 2;
      next = next->next;
    }

    coord[nPts][0] = PM->faces[i]->he->v->coordinates[0] + (center[0] - PM->faces[i]->he->v->coordinates[0]) * fact;
    coord[nPts][1] = PM->faces[i]->he->v->coordinates[1] + (center[1] - PM->faces[i]->he->v->coordinates[1]) * fact;
    nPts += 1;
  }

  return bov_points_new(coord, PM->nHEdges * 2, GL_STATIC_DRAW);
}

void printPolygonMesh(PolygonMesh *PM)
{
  printVertices(PM);
  printFaces(PM);
  printHEdges(PM);
}

void printVertices(PolygonMesh *PM)
{

  int val = 2;
  int tmp = PM->nVertices;
  while (tmp > 99)
  {
    val++;
    tmp /= 10;
  }
  printf("VERTICES\n%*s X      Y       &vert         &half-edge\n", val, "id");
  for (int i = 0; i < PM->nVertices; i++)
  {
    printf("%*d %+2.3f %+2.3f %p %p\n", val, i, PM->vertices[i]->coordinates[0], PM->vertices[i]->coordinates[1], PM->vertices[i], PM->vertices[i]->he);
  }
}

void printFaces(PolygonMesh *PM)
{
  int val = 2;
  int tmp = PM->nFaces;
  while (tmp > 99)
  {
    val++;
    tmp /= 10;
  }
  printf("FACES\n%*s &face          &half-edge\n", val, "id");
  for (int i = 0; i < PM->nFaces; i++)
  {
    printf("%*d %p %p\n", val, i, PM->faces[i], PM->faces[i]->he);
  }
}

void printHEdges(PolygonMesh *PM)
{
  int val = 2;
  int tmp = PM->nHEdges;
  while (tmp > 99)
  {
    val++;
    tmp /= 10;
  }

  printf("HALF-EDGES\n%*s &half-edge     &previous      &next          &opposite      &vertex        &face\n", val, "id");

  for (int i = 0; i < PM->nHEdges; i++)
  {
    printf("%*d %14p %14p %14p %14p %14p %14p\n", val, i, PM->hedges[i], PM->hedges[i]->prev, PM->hedges[i]->next, PM->hedges[i]->Opposite, PM->hedges[i]->v, PM->hedges[i]->f);
  }
}

void FreePolygonMesh(PolygonMesh *PM)
{
  for (int i = 0; i < PM->nFaces; i++)
  {
    free(PM->faces[i]);
  }
  free(PM->faces);

  for (int i = 0; i < PM->nVertices; i++)
  {
    free(PM->vertices[i]);
  }
  free(PM->vertices);

  for (int i = 0; i < PM->nHEdges; i++)
  {
    free(PM->hedges[i]);
  }
  free(PM->hedges);

  free(PM);
}

//TODO LOG/PRINT 4 debug