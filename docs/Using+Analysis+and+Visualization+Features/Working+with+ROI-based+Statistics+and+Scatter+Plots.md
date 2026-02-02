# Using Analysis and Visualization Features – Working with ROI-based Statistics and Scatter Plots

Oiii’s desktop application supports ROI-constrained analysis workflows that let you define spatial regions of interest (ROIs), compute point-based statistics inside those ROIs, and visualize results as histograms or scatter plots. This section explains how to:

- Define and manage rectangular ROIs
- Map coordinates between view and world spaces
- Collect vertices inside an ROI
- Generate and render scatter plots constrained by ROIs

---

## Defining and Managing Rectangular ROIs

Use the `COW2Doc`’s `m_pROIset` to add, select, and modify ROIs:

| Method | Purpose |
| --- | --- |
| `GetRectROI(int idROI)` | Retrieve bounding rectangle of ROI `idROI` |
| `GetInfoROI(int idROI)` | Retrieve full polyline defining ROI shape |
| `ModifyRectangularROI(id, &rect)` | Adjust an existing ROI’s dimensions |
| `SelectROI(int idROI)` | Mark ROI `idROI` as active for subsequent analysis |


Users typically draw or drag ROIs in a view, and Oiii posts an update message (`OWM_MOUSEMOVED_UPDATELOCALGRAPH`) to trigger graph updates  .

---

## Mapping ROI Coordinates Between View and World

ROIs are defined in **view** (pixel) coordinates but analysis routines operate in **world** (model) coordinates. Convert between these using:

```cpp
POINT viewPt = { rect.left, rect.top };
POINT2DD worldPt = pOW2View->MapPointFromViewToWorld(viewPt);
```

This ensures that spatial selection (e.g. edge-fill) aligns with triangulated point data  .

---

## Collecting Points Within an ROI Using EDGEFILLPARAM

To gather vertices inside a rectangular ROI for scatter-plot statistics:

1. **Retrieve ROI bounds**

```cpp
   CRect myRect = pOW2Doc->m_pROIset->GetRectROI(idROI);
```

1. **Convert corners to world coords**

```cpp
   POINT p1 = { myRect.left,  myRect.top };
   POINT p2 = { myRect.right, myRect.bottom };
   POINT2DD P1 = pOW2View->MapPointFromViewToWorld(p1);
   POINT2DD P2 = pOW2View->MapPointFromViewToWorld(p2);
   RECT2DD worldRect = { P1.x, P1.y, P2.x, P2.y };
```

1. **Prepare **`**EDGEFILLPARAM**`

```cpp
   int maxVert = (int)(fabs(P2.x - P1.x) + fabs(P2.y - P1.y));
   EDGEFILLPARAM params;
   params.pIDVERTEX = new int[maxVert];
   params.nvertex_max = maxVert;
   params.p_nvertexfound = &nFound;
   // Optionally initialize pIDTRI, seeds, etc.
```

1. **Invoke fill routine**

```cpp
   int tris = pOW2Doc->m_pPointset->RectFill(&worldRect, &params);
```

1. **Extract and filter statistics**

```cpp
   for(int i=0, j=0; i<*params.p_nvertexfound; ++i) {
       double v1 = baseStats[params.pIDVERTEX[i]*stride + ch1];
       double v2 = baseStats[params.pIDVERTEX[i]*stride + ch2];
       if(v1 < 0 || v2 < 0) {
           ++nReject;  // e.g. invalid Voronoi
       } else {
           arr1[j] = v1;
           arr2[j] = v2;
           ++j;
       }
   }
```

This pattern guarantees only valid points inside the ROI contribute to the scatter plot  .

---

## Generating ROI-Constrained Scatter Plots

The `CScatterGraph` class orchestrates data collection and visualization:

```cpp
// 1. Create in parent window
CScatterGraph* pGraph = new CScatterGraph(
    rectScatterView,
    pOW2Doc,
    pParentWnd,
    SCATTER_IMAGEPOINTS,
    SCATTER_BR_INTERIOR,
    idROI,
    -1,    // idVertex
    -1,    // iNumNeighbor
    -1,    // idSegment
    statChannel1,
    statChannel2
);

// 2. Update on ROI change
pGraph->Update(
    pOW2Doc,
    SCATTER_IMAGEPOINTS,
    SCATTER_BR_INTERIOR,
    idROI,
    -1,
    -1,
    -1,
    statChannel1,
    statChannel2
);
```

Inside `Update()`, it:

- Removes previous graph sets
- Calls `MapStatChannelIntoScatterGraph` (i.e. the EDGEFILLPARAM workflow above)
- Sets graph type to `GRAPH_SCATTER`
- Adds two data series (`arr1`, `arr2`) to the `COWGraphDoc`
- Cleans up temporary buffers

---

## Rendering the Scatter Plot

`CScatterGraphView` (a subclass of `COWGraphView`) handles drawing:

- **Axes & grid**: horizontal/vertical grids via `PlotAxes` and `PlotGrid`
- **Data points**: plotted in `PlotScatterGraph`, mapping data ranges to view coords
- **Captions**: float-formatted ticks for scatter via `MapIndexToFloatValuesOnDisplay`

---

## Workflow Overview

```mermaid
flowchart LR
  User[User Draws/Adjusts ROI 📐]
  User -->|Shift+Click| Msg[PostMessage OWM_MOUSEMOVED_UPDATELOCALGRAPH]
  Msg -->|Invoke Update| Scatter[CScatterGraph]
  Scatter --> Fill[EDGEFILLPARAM Fill Data]
  Fill --> Filter[Filter Invalid Points]
  Filter --> GraphDoc[COWGraphDoc Populate 📈]
  GraphDoc --> View[CScatterGraphView Render]
```

---

## Key Constants and Types

| Symbol | Meaning |
| --- | --- |
| `SCATTER_IMAGEPIXELS` | Use pixel-set for scatter points |
| `SCATTER_BR_INTERIOR` | Bounding-rect interior fill mode |
| `GRAPH_SCATTER` | Scatter plot graph type |
| `EDGEFILLPARAM` | Parameters for edge-fill routines |


---

**Emojis**

- 📐 ROI definition
- 📊 Statistics collection
- 📈 Scatter plot visualization

---

By following these patterns, you can integrate spatially constrained statistics and scatter-plot visualization into Oiii, ensuring analyses respect user-defined regions and display results in context of the original data.