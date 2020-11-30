#include "BOV.h"
#include <math.h>
#include "utils.h"
#ifndef HALFEDGE
#define HALFEDGE

typedef struct HalfEdge HalfEdge;
typedef struct Vertex Vertex;
typedef struct Face Face;
typedef struct PolygonMesh PolygonMesh;

struct HalfEdge
{
  Vertex *v;
  Face *f;
  HalfEdge *prev;
  HalfEdge *next;
  HalfEdge *Opposite;

  HalfEdge *nextList;
};

struct Vertex
{
  double coordinates[2];
  HalfEdge *he;

  Vertex *nextList;
};

struct Face
{
  HalfEdge *he;
  double point[2];
};

struct PolygonMesh
{

  Vertex *firstVertex;
  Vertex *lastVertex;
  HalfEdge *firstHedge;
  HalfEdge *lastHedge;
  Face **faces;

  int nFaces;
  int nHE;
  int nVertices;
};

PolygonMesh *InitEmptyPolygonMesh();
void initFaces(PolygonMesh *PM, coord *points, int n);
void addVertex(PolygonMesh *PM, Vertex *v);
void addHE(PolygonMesh *PM, HalfEdge *he);

Face *getFace(PolygonMesh *PM, double point[2]);
HalfEdge *createHe();
Vertex *createVertex(double point[2]);

// Half-edge operation
void linkHe(HalfEdge *a, HalfEdge *b);
void oppositeHe(HalfEdge *a, HalfEdge *b);

/* To refactor
bov_points_t *getVerticesBOVPolygonMesh(PolygonMesh *PM);
bov_points_t *getHalfEdgesBOVPolygonMesh(PolygonMesh *PM, double factor);
PolygonMesh *LoadPolygonMesh(double *Vertices, int nVertices, int *hEdges, int nEdges, int nFaces);
*/

// For draw
int getHePoints(PolygonMesh *PM, coord *points);

// For print in terminal
void printVertices(PolygonMesh *PM);
void printHEdges(PolygonMesh *PM);
void printHEdge(HalfEdge *he);
void printFaces(PolygonMesh *PM);
void printPolygonMesh(PolygonMesh *PM);

void freePolygonMesh(PolygonMesh *PM);
void freeVertices(Vertex *v);
void freeFace(Face *f);
void freeHe(HalfEdge *he);

#endif