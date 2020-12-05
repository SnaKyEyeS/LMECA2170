#include "halfEdge.h"

/*
 * Create an empty Polygon mesh
 */
PolygonMesh *InitEmptyPolygonMesh()
{
  PolygonMesh *pm = malloc(sizeof(PolygonMesh));

  pm->nFaces = 0;
  pm->faces = NULL;

  pm->firstVertex = NULL;
  pm->lastVertex = NULL;

  pm->firstHedge = NULL;
  pm->lastHedge = NULL;

  return pm;
}

/*
 * Add Vertex to the PM
 */
void addVertex(PolygonMesh *PM, Vertex *v)
{
  //TODO convert linked list to array bounded by max value
  PM->nVertices++;
  if (PM->firstVertex == NULL)
  {
    PM->firstVertex = v;
    PM->lastVertex = v;
    v->nextList = NULL;
    return;
  }
  PM->lastVertex->nextList = v;
  PM->lastVertex = v;
  v->nextList = NULL;
}

/*
 * Add Half-Edge to PM
 */
void addHE(PolygonMesh *PM, HalfEdge *he)
{
  //TODO convert linked list to array bounded by max value
  PM->nHE++;
  if (PM->firstHedge == NULL)
  {
    PM->firstHedge = he;
    PM->lastHedge = he;
    he->nextList = NULL;
    return;
  }
  PM->lastHedge->nextList = he;
  PM->lastHedge = he;
  he->nextList = NULL;
}

/*
 * Faces are defined by points. Will ad in pm
 */
void initFaces(PolygonMesh *PM, coord *points, int n)
{
  PM->nFaces = n;
  PM->faces = malloc(sizeof(Face *) * n);
  for (int i = 0; i < n; i++)
  {
    PM->faces[i] = malloc(sizeof(Face));
    PM->faces[i]->point[0] = points[i][0];
    PM->faces[i]->point[1] = points[i][1];
    PM->faces[i]->he = NULL;
  }
}

/*
 * Create a vertex from a point
 */
Vertex *createVertex(float point[2])
{
  Vertex *v = malloc(sizeof(Vertex));
  v->coordinates[0] = point[0];
  v->coordinates[1] = point[1];
  return v;
}

/*
 * Create an empty Half-edge
 */
HalfEdge *createHe()
{
  HalfEdge *he = malloc(sizeof(HalfEdge));
  he->f = NULL;
  he->v = NULL;
  he->next = NULL;
  he->prev = NULL;
  he->Opposite = NULL;
  he->nextList = NULL;
  return he;
}

/*
 * return the face corresponding to a point
 */
Face *getFace(PolygonMesh *PM, float point[2])
{
  int a = 0;
  int b = PM->nFaces - 1;
  if (b < 0)
  {
    return NULL;
  }

  while (a <= b)
  {
    int n = floor((a + b) / 2);
    int comp = comparefloats(PM->faces[n]->point, point);
    if (comp == 0)
    {
      return PM->faces[n];
    }
    else if (comp > 0)
    {
      b = n - 1;
    }
    else
    {
      a = n + 1;
    }
  }
  return NULL;
}

/*
 * Will create the connection from HE a to HE b
 * a: a HE
 * b: a HE
 */
void linkHe(HalfEdge *a, HalfEdge *b)
{
  if (a == NULL)
  {
    printf("Error first HE is NULL\n");
    return;
  }

  if (b == NULL)
  {
    printf("Error second is NULL \n");
    return;
  }
  a->next = b;
  b->prev = a;
}

/*
 * Will create the opposite link between a and b
 * a: HE
 * b: HE
 */
void oppositeHe(HalfEdge *a, HalfEdge *b)
{
  // TODO remove to go faster ?
  if (a == NULL)
  {
    printf("Error a is NULL\n");
    return;
  }

  if (b == NULL)
  {
    printf("Error b is NULL \n");
    return;
  }
  a->Opposite = b;
  b->Opposite = a;
}

/*
 * Return the number of point to draw
 * points should be 2*Pm->nHe
 */
int getHePoints(PolygonMesh *PM, coord *points)
{
  if (PM == NULL)
  {
    return 0;
  }

  int n = 0;
  HalfEdge *var = PM->firstHedge;
  while (var != NULL)
  {
    if (var->v != NULL && var->next != NULL && var->next->v != NULL)
    {
      points[n][0] = var->v->coordinates[0];
      points[n][1] = var->v->coordinates[1];
      points[n + 1][0] = var->next->v->coordinates[0];
      points[n + 1][1] = var->next->v->coordinates[1];
      n += 2;
    }
    var = var->nextList;
  }
  return n;
}

/*
 * Print the polygon mesh
 */
void printPolygonMesh(PolygonMesh *PM)
{
  printVertices(PM);
  printFaces(PM);
  printHEdges(PM);
}

/*
 *  Print all the vertices
 */
void printVertices(PolygonMesh *PM)
{

  int val = 4;
  int i = 0;
  if (PM == NULL)
  {
    printf("Polygonmesh is NULL \n");
    return;
  }
  Vertex *var = PM->firstVertex;

  printf("VERTICES\n%*s X      Y       &vert         &half-edge\n", val, "id");
  while (var != NULL)
  {
    printf("%*d %+2.3f %+2.3f %p %p\n", val, i, var->coordinates[0], var->coordinates[1], var, var->he);

    var = var->nextList;
    i++;
  }
}

/*
 * Print all the faces
 */
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
    printf("%*d (%f,%f) %p %p\n", val, i, PM->faces[i]->point[0], PM->faces[i]->point[1], PM->faces[i], PM->faces[i]->he);
  }
}

/*
 * Print all the Half edges
 */
void printHEdges(PolygonMesh *PM)
{
  int val = 4;
  int i = 0;
  if (PM != NULL)
  {
    HalfEdge *var = PM->firstHedge;
    printf("HALF-EDGES\n%*s &half-edge     &previous      &next          &opposite      &vertex        &face\n", val, "id");

    while (var != NULL)
    {
      printf("%*d %14p %14p %14p %14p %14p %14p\n", val, i, var, var->prev, var->next, var->Opposite, var->v, var->f);
      var = var->nextList;
      i++;
    }
  }
  else
  {
    printf("PolygonMesh is NULL \n");
  }
}

/*
 * Print One HE
 */
void printHEdge(HalfEdge *he)
{
  if (he == NULL)
  {
    printf("HalfEdge is NULL \n");
  }
  HalfEdge *var = he;
  printf("HalfEdge %14p %14p %14p %14p %14p %14p %14p\n", var, var->prev, var->next, var->Opposite, var->v, var->f, var->nextList);
}

/*
 * Free a polygon mesh
 */
void freePolygonMesh(PolygonMesh *PM)
{
  if (PM == NULL)
    return;
  freeVertices(PM->firstVertex);

  for (int i = 0; i < PM->nFaces; i++)
  {
    freeFace(PM->faces[i]);
  }

  free(PM->faces);
  freeHe(PM->firstHedge);
  free(PM);
}

/*
 * Free all vertices 
 */
void freeVertices(Vertex *v)
{
  if (v == NULL)
  {
    return;
  }

  Vertex *tmp = v->nextList;
  free(v);
  while (tmp != NULL)
  {
    Vertex *tmp2 = tmp->nextList;
    free(tmp);
    tmp = tmp2;
  }
}

/*
 * Free a face
 */
void freeFace(Face *f)
{
  if (f != NULL)
    free(f);
}

/*
 * Free all the HE
 */
void freeHe(HalfEdge *he)
{
  if (he == NULL)
  {
    return;
  }
  HalfEdge *tmp = he->nextList;
  free(he);
  while (tmp != NULL)
  {
    HalfEdge *tmp2 = tmp->nextList;
    free(tmp);
    tmp = tmp2;
  }
}