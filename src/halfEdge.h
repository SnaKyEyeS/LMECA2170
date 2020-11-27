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
  float coordinates[2];
  HalfEdge *he;

  Vertex *nextList;
};

struct Face
{
  HalfEdge *he;
  float point[2];
};

struct PolygonMesh
{

  Vertex *firstVertex;
  Vertex *lastVertex;
  HalfEdge *firstHedge;
  HalfEdge *lastHedge;
  Face **faces;

  int nFaces;
};

PolygonMesh *InitEmptyPolygonMesh();
void initFaces(PolygonMesh *PM, coord *points, int n);
void addVertex(PolygonMesh *PM, Vertex *v);
void addHE(PolygonMesh *PM, HalfEdge *he);

HalfEdge *createHe();
Vertex *createVertex(float point[2]);

/* To refactor
bov_points_t *getVerticesBOVPolygonMesh(PolygonMesh *PM);
bov_points_t *getHalfEdgesBOVPolygonMesh(PolygonMesh *PM, float factor);
PolygonMesh *LoadPolygonMesh(float *Vertices, int nVertices, int *hEdges, int nEdges, int nFaces);
void FreePolygonMesh(PolygonMesh *PM);

// For debug
void printVertices(PolygonMesh *PM);
void printHEdges(PolygonMesh *PM);
void printFaces(PolygonMesh *PM);
void printPolygonMesh(PolygonMesh *PM);
*/
#endif