# Troubleshooting and Logging – Common Geometry and Segmentation Issues

This section describes typical failures in triangulation, Voronoi‐based statistics, and fusion‐tree construction. It explains how the toolkit logs these errors and offers guidance for diagnosing and resolving them.

## Invalid Triangles During Triangulation 🛑

During mesh construction, some triangles may become **invalid**—for example, their circumcenters lie outside the convex hull. The function `FindAllTriSurroundingVertex` detects these cases, logs an error, and returns early.

- **Symptoms**
- Error messages in the debug log.
- Incomplete adjacency lists.
- Downstream functions abort or skip processing.

- **Key Function**

```cpp
  int FindAllTriSurroundingVertex(
      POINTSET* pPointset,
      int ivertex,
      int* p_itriseed,
      int* p_numtrifound,
      int* p_arraytri,
      int* p_arrayneighbor
  );
```

On failure, it logs one of several messages to `pPointset->pFILE_debug` and returns `FALSE` or a negative code .

- **Common Log Messages**

| Condition | Log Message | Return Value |
| --- | --- | --- |
| Supplied pointer is null | “error in FindAllTriSurroundingVertex(), at least one of the supplied pointers is null” | –99 |
| No initial valid triangle found (CCW + CW search) | “error in FindAllTriSurroundingVertex(), cannot find one valid tri for vertex (ccw + cw search)” | –99 |
| Encountered an invalid triangle in CCW walk (center outside hull) | “error in FindAllTriSurroundingVertex(),  In searching for next counter-clockwise adjacent triangle we get an invalid triangle (index=-1 or tri center outside convexe envelope bounding rect)” | `FALSE` |
| Inconsistent vertex pair | “error in FindAllTriSurroundingVertex(), inconsistant vertices pair (%d,%d) for triangle %d” | `FALSE` |


- **Debugging Tips**
- Verify your input point set has no duplicates or colinear points on the convex boundary.
- Increase logging verbosity by ensuring `pPointset->pFILE_debug` is open.
- Inspect the logged triangle indices and vertex pairs to pinpoint problematic regions.

## Voronoi Area Computation Failures 📐

Voronoi statistics rely on a closed polygon around each vertex. Boundary or degenerate configurations yield invalid areas which are **flagged** and **excluded** from visualization and metrics.

- **Invalid‐Area Tag**

By convention, an uncomputable area is set to:

```cpp
  const double POINTSET_TAGFORINVALIDVOROAREA = -9999.99;
```

When `ComputeVoronoiAreaForVertex` cannot form a closed polygon, it stores this value .

- **Density Handling**

```cpp
  if (fVoronoiArea < 0)
      fVoronoiDensity = POINTSET_TAGFORINVALIDVOROAREA;
  else
      fVoronoiDensity = 1.0 / fVoronoiArea;
```

Invalid areas yield the same dummy density, ensuring they are filtered out of plots .

- **Neighbor Averaging Exclusion**

When computing neighboring Voronoi density, any neighbor with `voronoiArea < 0.0` is skipped:

```cpp
  if (fVoronoiArea < 0.0) {
      numvalidneighbor--;
  } else {
      fNeighboringVoronoiDensity += fVoronoiDensity;
  }
```

This prevents contamination of averages by invalid entries .

- **Log Entries**

The routines bracket their execution with debug messages:

```cpp
  fprintf(pPointset->pFILE_debug, "calling ComputeVoronoiAreaForAllVertex() start . \r\n");
  … 
  fprintf(pPointset->pFILE_debug, "calling ComputeVoronoiAreaForAllVertex() done. \r\n");
```

## Aborting Long‐Running Fusion‐Tree Construction ⏹️

Building the segmentation fusion tree can be time‐consuming on large data. The code includes an (currently commented out) hook to let the user cancel via the **Esc** key.

```cpp
/* //spi 2014
BOOL bEsc = ((COIIIApp*)AfxGetApp())->PeekKeyState(VK_ESCAPE);
if (bEsc) {
    CString msg;
    msg.Format(
      "\r\nBuildFusionTree() aborted by user after merging %d segments\r\n",
      idsegNewParent - iStart
    );
    ((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText(msg);
    return;
}
*/
```

This snippet checks `VK_ESCAPE`, logs partial progress, and returns safely .

### Enabling Abort Detection

1. Uncomment the `PeekKeyState(VK_ESCAPE)` block in `BuildFusionTree`.
2. Rebuild **oifiilib** or **owsegmentset**.
3. Press **Escape** during segmentation to trigger an early exit.

## General Troubleshooting Steps 🛠️

When analysis **hangs** or results look **unexpected**, follow these steps:

1. Inspect the **Log View**
2. Look for keywords like `error in FindAllTriSurroundingVertex` or `invalid voronoi area`.
3. Confirm that log file handles (`pFILE_debug` or `CLogDocument`) are active.

1. Verify **ROI Settings**
2. Ensure your region of interest is valid (convex, non‐empty).
3. Adjust or reset ROI to exclude boundary artifacts.

1. Simplify Your Data
2. Reduce the number of points.
3. Adjust segmentation parameters (e.g., initial segment count).
4. Re-run with a coarse mesh to isolate failure cases.

```card
{
    "title": "Invalid Voronoi Filter",
    "content": "Any Voronoi area < 0.0 is set to \u20139999.99 and excluded automatically."
}
```

> **Tip:** Use the **card** below for quick reference on filtering invalid Voronoi entries.

---

By following these guidelines and examining the logged messages, you can quickly identify and correct common issues in geometry processing and segmentation within the Oifii toolkit.