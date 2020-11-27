#include "BOV.h"
#include <math.h>

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
};

struct Vertex
{
  float coordinates[2];
  HalfEdge *he;
};

struct Face
{
  HalfEdge *he;
};

struct PolygonMesh
{
  int nVertices, nHEdges, nFaces;
  Vertex **vertices;
  HalfEdge **hedges;
  Face **faces;
};

bov_points_t *getVerticesBOVPolygonMesh(PolygonMesh *PM);
bov_points_t *getHalfEdgesBOVPolygonMesh(PolygonMesh *PM, float factor);
PolygonMesh *LoadPolygonMesh(float *Vertices, int nVertices, int *hEdges, int nEdges, int nFaces);
void FreePolygonMesh(PolygonMesh *PM);

// For debug
void printVertices(PolygonMesh *PM);
void printHEdges(PolygonMesh *PM);
void printFaces(PolygonMesh *PM);
void printPolygonMesh(PolygonMesh *PM);

#endif