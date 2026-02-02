# oifiilib Core Libraries (POINTSET and SEGMENTSET)

## POINTSET Library Overview

The **POINTSET** C library underpins the representation and processing of 2D point clouds and their associated triangulations. It offers:

- Efficient storage for point and triangle‐center coordinates
- Topology and adjacency structures for triangle networks
- Routines to build and query a triangular irregular network (TIN)
- Geometric feature computations (Voronoi areas & densities)
- Line and intersection helper functions for primitive geometry tests

---

### Data Structures

POINTSET encapsulates all data for a TIN-based point set:

| Member | Type | Description |
| --- | --- | --- |
| `npts` | `int` | Number of points |
| `px`, `py` | `double*` | X/Y coordinates of each point |
| `vt[3]` | `int*` | Triangle‐vertex indices (`vt[i][itri]`) |
| `nt[3]` | `int*` | Triangle‐neighbor indices (`nt[i][itri]`) |
| `ctx`, `cty` | `double*` | Geometric centers of each triangle |
| `pfStatistics` | `double*` | Per‐vertex statistics array (e.g., Voronoi areas) |
| `nStatPerPoint` | `int` | Number of statistics stored per point |
| `xmin, xmax, ymin, ymax` | `double` | Bounding box of all points |


---

### Core Data Arrays

Emphasis on the most critical arrays:

| Array | Purpose |
| --- | --- |
| `px, py` | Store 2D point coordinates |
| `vt` | For each triangle, the three point‐indices (0–2) |
| `nt` | For each triangle, the three neighboring tri‐indices |
| `ctx, cty` | Centroids of triangles (for Voronoi computations) |


---

### Triangle Network Construction 🛠️

Builds a robust triangulation covering all points within the convex hull:

```c
void OIFIILIB_API BuildTriangleNetwork(POINTSET* pPointset);
```

- Implements a strip‐based incremental TIN algorithm
- Allocates and fills `vt` and `nt` arrays for topology
- Records optimization steps in `loptim` for debugging

#### Neighborhood Queries

Once the network exists, spatial adjacency can be queried:

- **Find all triangles around a vertex**

```c
  int OIFIILIB_API FindAllTriSurroundingVertex(
      POINTSET* pPointset, int ivertex,
      int* p_itriseed, int* p_numtrifound,
      int* p_arraytri, int* p_arrayneighbor
  );
```

- **Find all neighboring vertices**

```c
  int OIFIILIB_API FindAllNeighborSurroundingVertex(
      POINTSET* pPointset, int iVertex,
      int iOrder, int* p_iTriSeed,
      int* p_final_numfound, int* p_arrayneighbor
  );
```

- **Consistent neighbor filtering**

```c
  int OIFIILIB_API CheckNeighborConsistensyForAllVertex(POINTSET* pPointset);
```

- **Single nearest‐neighbor search**

```c
  int OIFIILIB_API FindNearestNeighbor(
      POINTSET* pPointset, double xa, double ya,
      int* p_itriseed, int adjacenttriflag
  );
```

- **Adjacency and utility**

```c
  int OIFIILIB_API GetAdjTri(
      POINTSET* pPointset, int v1, int v2, int itri
  );
  int OIFIILIB_API GeThirdVertex(
      POINTSET* pPointset, int v1, int v2, int itri
  );
```

---

### Geometric Computations 📐

#### Triangle Centers

Compute and store the centroids of each triangle:

```c
void OIFIILIB_API ComputeAllTriangleCenters(POINTSET* pPointset);
void OIFIILIB_API ComputeTriangleCenter(
    POINTSET* pPointset, int idtri
);
```

Each centroid is the mean of its three vertex coordinates .

#### Voronoi Area & Density

Calculates per‐vertex Voronoi statistics using adjacent triangle centers:

```c
int OIFIILIB_API ComputeVoronoiAreaForAllVertex(POINTSET* pPointset);
int OIFIILIB_API ComputeVoronoiAreaForVertex(
    POINTSET* pPointset, int idvertex
);
```

- **Workflow**
- Gather all valid surrounding triangles (`FindAllValidTriSurroundingVertex`)
- Close the polygon of triangle centers
- Sum triangle areas via the shoelace formula
- **Storage**
- Voronoi area → `pfStatistics[ivertex*nStatPerPoint + POINTSET_OFFSETSTAT_VOROAREA]`
- Voronoi density (1/area) → `… + POINTSET_OFFSETSTAT_VORODENSITY`

```card
{
    "title": "Tip",
    "content": "Voronoi density is amplified by 1000 when visualized to improve numerical stability."
}
```

---

### Helper Routines for Lines & Intersections

Available for primitive geometric tests, all exported via **OIFIILIB_API**:

| Function | Signature |
| --- | --- |
| **LineMB** | `int LineMB(double x1,double y1,double x2,double y2,double* m,double* b);` |
| **IsPointUpLineMB** | `int IsPointUpLineMB(double x,double y,double* m,double* b);` |
| **IsPointOnLineSegment** | `int IsPointOnLineSegment(double x1,double y1,double x2,double y2,double x,double y,double x_res=1.0,double y_res=1.0);` |
| **LineSegmentsIntersect** | `int LineSegmentsIntersect(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,double* pfX,double* pfY);` |


These enable:

- Slope–intercept line construction
- Point‐line side tests
- Segment intersection detection and intersection point retrieval

---

### Interaction with Other Components

- **SEGMENTSET** builds upon POINTSET to create segment trees and fusion lists
- **View modules** (e.g., `draw_voronoi_regions_from_memory`) rely on center and Voronoi data
- **C++ wrappers** (`OWpointset`, `COWSegmentset`) encapsulate the C-API for use in `Oiii.exe`

This core library thus provides a foundation for TIN construction, spatial queries, and geometric analysis essential to oifii’s scientific visualization toolkit.