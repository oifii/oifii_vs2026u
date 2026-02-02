/*
 * Copyright (c) 1994-2014 Stephane Poirier
 *
 * stephane.poirier@oifii.org
 *
 * Stephane Poirier
 * 3532 rue Ste-Famille, #3
 * Montreal, QC, H2X 2L1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// OWview.cpp : implementation of the COWView class
//

#include "stdafx.h"
#include "oifiilib.h" //spi 2014
#include "oifii_app.h"

#include "OWpixelset.h" //offworld c++ image library

#include "c_pointset.h" //offworld c pointset library
#include "c_segmentset.h" //offworld c segmentset library

#include "OWpointset.h" //offworld c++ pointset library (interface sitting on pointset.h)
#include "OWsegmentset.h" //offworld c++ segmentset library (interface sitting on segmentset.h)
#include "OWclassset.h" //offworld c++ classset library (interface sitting on segmentset.h)
#include "OWedgeset.h" //offworld edgeset library (for line, polyline, polygone  and shape)

#include "OWdoc.h"
#include "logdoc.h"
#include "OW2doc.h"
#include "OW2mdi.h"
#include "OWview.h"

#include "oifii_gotodlg.h"

#include "ow_iff.h"
/*
#include "imglib.h"
#include "img_err.h"
*/

#include <gl\gl.h>
#include <gl\glu.h>
#include "oifii_propsettings.h"
#include "oifii_propsheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COWView

IMPLEMENT_DYNCREATE(COWView, CScrollView) //CView) //SECPanView) //SECPanView

BEGIN_MESSAGE_MAP(COWView, CScrollView) //CView)//SECPanView) //SECPanView
	//{{AFX_MSG_MAP(COWView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_PIXELSET, OnViewPixelset)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PIXELSET, OnUpdateViewPixelset)
	ON_COMMAND(ID_VIEW_POINTSET, OnViewPointset)
	ON_UPDATE_COMMAND_UI(ID_VIEW_POINTSET, OnUpdateViewPointset)
	ON_COMMAND(ID_VIEW_POINTSET_LABELS, OnViewPointsetLabels)
	ON_UPDATE_COMMAND_UI(ID_VIEW_POINTSET_LABELS, OnUpdateViewPointsetLabels)
	ON_COMMAND(ID_VIEW_TRIANGLESET, OnViewTriangleset)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TRIANGLESET, OnUpdateViewTriangleset)
	ON_COMMAND(ID_VIEW_VORONOISET, OnViewVoronoiset)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VORONOISET, OnUpdateViewVoronoiset)
	ON_COMMAND(ID_VIEW_SEGMENTSET, OnViewSegmentset)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SEGMENTSET, OnUpdateViewSegmentset)
	ON_COMMAND(ID_VIEW_GOTOPARTITION, OnViewGotopartition)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GOTOPARTITION, OnUpdateViewGotopartition)
	ON_COMMAND(ID_VIEW_GOTOCLASSIFICATION, OnViewGotoclassification)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GOTOCLASSIFICATION, OnUpdateViewGotoclassification)
	ON_COMMAND(ID_VIEW_PIXELSET_BLUE, OnViewPixelsetBlue)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PIXELSET_BLUE, OnUpdateViewPixelsetBlue)
	ON_COMMAND(ID_VIEW_PIXELSET_GREEN, OnViewPixelsetGreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PIXELSET_GREEN, OnUpdateViewPixelsetGreen)
	ON_COMMAND(ID_VIEW_PIXELSET_HUE, OnViewPixelsetHue)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PIXELSET_HUE, OnUpdateViewPixelsetHue)
	ON_COMMAND(ID_VIEW_PIXELSET_INTENSITY, OnViewPixelsetIntensity)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PIXELSET_INTENSITY, OnUpdateViewPixelsetIntensity)
	ON_COMMAND(ID_VIEW_PIXELSET_RED, OnViewPixelsetRed)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PIXELSET_RED, OnUpdateViewPixelsetRed)
	ON_COMMAND(ID_VIEW_PIXELSET_SATURATION, OnViewPixelsetSaturation)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PIXELSET_SATURATION, OnUpdateViewPixelsetSaturation)
	ON_COMMAND(ID_VIEW_POINTSET_CROSS, OnViewPointsetCross)
	ON_COMMAND(ID_VIEW_POINTSET_ELLIPSE, OnViewPointsetEllipse)
	ON_COMMAND(ID_VIEW_POINTSET_MARK, OnViewPointsetMark)
	ON_COMMAND(ID_VIEW_TRIANGLESET_LABELS, OnViewTrianglesetLabels)
	ON_COMMAND(ID_VIEW_TRIANGLESET_3VERTICES, OnViewTriangleset3vertices)
	ON_COMMAND(ID_VIEW_TRIANGLESET_CENTERPOINTS, OnViewTrianglesetCenterpoints)
	ON_COMMAND(ID_VIEW_TRIANGLESET_POLYGON, OnViewTrianglesetPolygon)
	ON_COMMAND(ID_VIEW_VORONOISET_LABELS, OnViewVoronoisetLabels)
	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(ID_VIEW_POINTSET_CROSS, OnUpdateViewPointsetCross)
	ON_UPDATE_COMMAND_UI(ID_VIEW_POINTSET_ELLIPSE, OnUpdateViewPointsetEllipse)
	ON_UPDATE_COMMAND_UI(ID_VIEW_POINTSET_MARK, OnUpdateViewPointsetMark)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TRIANGLESET_3VERTICES, OnUpdateViewTriangleset3vertices)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TRIANGLESET_CENTERPOINTS, OnUpdateViewTrianglesetCenterpoints)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TRIANGLESET_LABELS, OnUpdateViewTrianglesetLabels)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TRIANGLESET_POLYGON, OnUpdateViewTrianglesetPolygon)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VORONOISET_LABELS, OnUpdateViewVoronoisetLabels)
	//}}AFX_MSG_MAP
	ON_MESSAGE(OWM_DOC_UPDATEINFO, OnUpdateDocInfo) //offworld defined message
	ON_MESSAGE(OWM_VIEW_UPDATEINFO, OnUpdateViewInfo) //offworld defined message
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)	//CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview) //CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COWView construction/destruction

COWView::COWView(HWND hwnd, COWDocument* pCOWDoc) //spi 2014
{
	m_parentHWND = hwnd;
	m_pCOWDoc = pCOWDoc;
	Init();
}

COWView::COWView()
{
	m_parentHWND = NULL;//spi 2014
	m_pCOWDoc = NULL;
	Init();
}

void COWView::Init()
{
	m_bViewPixelset_DisplayRed = FALSE;
	m_bViewPixelset_DisplayGreen = FALSE;
	m_bViewPixelset_DisplayBlue = FALSE;
	m_bViewPixelset_DisplayIntensity = FALSE;
	m_bViewPixelset_DisplayHue = FALSE;
	m_bViewPixelset_DisplaySaturation = FALSE;

	m_dfZoomFactor = 1.0;
	m_dfZoomFactorMin = -1.0;
	m_dfZoomFactorMax = -1.0;

    ZoomFactorChanged();
    m_bCenter = TRUE;   

 	SetScrollSizes(MM_TEXT, CSize(0, 0));

	if(m_parentHWND==NULL) //spi 2014
	{
		m_hZoomInPointCursor  = AfxGetApp()->LoadCursor(IDC_ZOOMIN);
		m_hZoomOutPointCursor = AfxGetApp()->LoadCursor(IDC_ZOOMOUT);
		m_hZoomRectCursor     = AfxGetApp()->LoadCursor(IDC_ZOOMRECT);    
		m_hZoomRectDragCursor = AfxGetApp()->LoadCursor(IDC_ZOOMRECTDRAG); 

		m_hDrawEllipseCursor     = AfxGetApp()->LoadCursor(IDC_DRAWELLIPSE);    
		m_hDrawEllipseDragCursor = AfxGetApp()->LoadCursor(IDC_DRAWELLIPSEDRAG); 

		m_hROIRectCursor     = AfxGetApp()->LoadCursor(IDC_ROIRECT);    
		m_hROIRectDragCursor = AfxGetApp()->LoadCursor(IDC_ROIRECTDRAG); 
		m_hROIPolylineCursor     = AfxGetApp()->LoadCursor(IDC_ROIPOLYLINE);    
		m_hROIPolylineDragCursor = AfxGetApp()->LoadCursor(IDC_ROIPOLYLINEDRAG); 
		m_hROIFreehandCursor     = AfxGetApp()->LoadCursor(IDC_ROIFREEHAND);    
		m_hROIFreehandDragCursor = AfxGetApp()->LoadCursor(IDC_ROIFREEHANDDRAG); 
	}
	else
	{
		m_hZoomInPointCursor  = NULL;
		m_hZoomOutPointCursor = NULL;
		m_hZoomRectCursor     = NULL;    
		m_hZoomRectDragCursor = NULL; 

		m_hDrawEllipseCursor     = NULL;    
		m_hDrawEllipseDragCursor = NULL; 

		m_hROIRectCursor     = NULL;    
		m_hROIRectDragCursor = NULL; 
		m_hROIPolylineCursor     = NULL;    
		m_hROIPolylineDragCursor = NULL; 
		m_hROIFreehandCursor     = NULL;    
		m_hROIFreehandDragCursor = NULL; 
	}
	m_iNumberOfSegmentForFinalPartition = -1;
	m_iNumberOfSegmentForFinalClassification = -1;

	m_pViewDataSettings = NULL;
	m_pProjectionSettings = NULL;
	m_pTransformationSettings = NULL;

    //Turn on zooming, default is zooming off.
    SetZoomMode(OW_ZOOMNORMAL); //SECZoomView
    m_owviewMode = NORMAL;
    m_bCaptured = FALSE;

	m_pTempViewDataSettings = NULL;
}

COWView::~COWView()
{
    //Free cursors
    if (m_hZoomInPointCursor)  DestroyCursor(m_hZoomInPointCursor);
    if (m_hZoomOutPointCursor) DestroyCursor(m_hZoomOutPointCursor);
    if (m_hZoomRectCursor)     DestroyCursor(m_hZoomRectCursor);
    if (m_hZoomRectDragCursor) DestroyCursor(m_hZoomRectDragCursor);
    if (m_hDrawEllipseCursor)     DestroyCursor(m_hDrawEllipseCursor);
    if (m_hDrawEllipseDragCursor) DestroyCursor(m_hDrawEllipseDragCursor);

    if(m_hROIRectCursor) DestroyCursor(m_hROIRectCursor);    
    if(m_hROIRectDragCursor) DestroyCursor(m_hROIRectDragCursor);   
    if(m_hROIPolylineCursor) DestroyCursor(m_hROIPolylineCursor);      
    if(m_hROIPolylineDragCursor) DestroyCursor(m_hROIPolylineDragCursor);   
    if(m_hROIFreehandCursor) DestroyCursor(m_hROIFreehandCursor);   
    if(m_hROIFreehandDragCursor) DestroyCursor(m_hROIFreehandDragCursor);

	if(m_pViewDataSettings) delete m_pViewDataSettings;
	if(m_pProjectionSettings) delete m_pProjectionSettings;
	if(m_pTransformationSettings) delete m_pTransformationSettings;
}

void COWView::OnInitialUpdate() 
{
	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
	
	//initialize view settings from document
	if(m_pTempViewDataSettings)
	{
		m_pViewDataSettings->CopyAllValuesFrom(m_pTempViewDataSettings);
		m_pTempViewDataSettings = NULL;
	}
	else
	{
		m_pViewDataSettings->CopyAllValuesFrom(pDoc->m_pViewDataSettings);
	}
	m_pProjectionSettings->CopyAllValuesFrom(pDoc->m_pProjectionSettings);
	m_pTransformationSettings->CopyAllValuesFrom(pDoc->m_pTransformationSettings);

	UpdateViewPixelsetDisplayFlags();

	SetZoomFactorMinMax(0.02f,44.0f);
	SetScrollInfo();

	//CView::OnInitialUpdate();
	CScrollView::OnInitialUpdate();
}

void COWView::SetZoomMode(UINT zoomMode)
{
    ASSERT_VALID(this);
    ASSERT(m_nMapMode == MM_ANISOTROPIC);
    if (zoomMode != m_zoomMode)
	m_zoomMode = zoomMode;
    return;
} 

double COWView::GetZoomFactor()
{
	return m_dfZoomFactor;
}

void COWView::SetZoomFactor(double dfNewZoomFactor)
{
    //Developer has not changed min/max, no need to check.
    if (m_dfZoomFactorMax == -1 && m_dfZoomFactorMin == -1)
	{
		m_dfZoomFactor = dfNewZoomFactor;
		//return;
    }
    else if (dfNewZoomFactor <=m_dfZoomFactorMax && dfNewZoomFactor >=m_dfZoomFactorMin)
	{
		m_dfZoomFactor = dfNewZoomFactor;
		//return;
    }
    else if (dfNewZoomFactor < m_dfZoomFactorMin)
	{
	    //Problems here.
		ZoomOutOfRange(TRUE,dfNewZoomFactor); //Underflow
	}
    else
	{
		//Problems here.
		ZoomOutOfRange(FALSE,dfNewZoomFactor); //Overflow
	}

	ZoomFactorChanged(); //to notify the app, for proper update of toolbar's combox
	return;
}

void COWView::ZoomOutOfRange(BOOL bUnderFlow,double dfAttempt)
{
    //dfAttempt; //UNUSED
    if (bUnderFlow)
	TRACE2("COWView detected zoom underflow level: %f attempt: %f\n",
	       m_dfZoomFactor,dfAttempt);
    else
	TRACE2("COWView detected zoom overflow: level: %f attempt: %f\n",
	       m_dfZoomFactor,dfAttempt);
    
}

void COWView::GetZoomFactorMinMax(double* pfZoomFactorMin, double* pfZoomFactorMax)
{
	ASSERT(pfZoomFactorMin!=NULL);
	ASSERT(pfZoomFactorMax!=NULL);
	pfZoomFactorMin[0] = m_dfZoomFactorMin;
	pfZoomFactorMax[0] = m_dfZoomFactorMax;

	return;
}

void COWView::SetZoomFactorMinMax(double dfZoomFactorMin, double dfZoomFactorMax)
{
	m_dfZoomFactorMin = dfZoomFactorMin;
	m_dfZoomFactorMax = dfZoomFactorMax;
	return;
}

BOOL COWView::IsActiveView()
{
	if(m_parentHWND==NULL) //spi 2014
	{
		//1) capture active mdi child view and doc
		CMDIFrameWnd* pMDIParentFrameWnd = (CMDIFrameWnd*) AfxGetApp()->m_pMainWnd;
		CFrameWnd* pFrameWnd = pMDIParentFrameWnd->GetActiveFrame();
		//1.1) is the active child of type COW2MDI ?
		BOOL bIsOW2MDI = pFrameWnd->IsKindOf(RUNTIME_CLASS(COW2MDI));
		if(!bIsOW2MDI)
		{
			return FALSE; //this view is not the active view
		}
		else
		{
			CView* pView = pFrameWnd->GetActiveView();
			COWView* pOWView = (COWView*) pView;
			if(this == pOWView) return TRUE; //this is the active view
			  else return FALSE; //this view is not the active view
		}
	}
	else
	{
		if(m_parentHWND==::GetActiveWindow())
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

int COWView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(m_parentHWND==NULL) //spi 2014
	{
		//if (CView::OnCreate(lpCreateStruct) == -1)
		//	return -1;
		if (CScrollView::OnCreate(lpCreateStruct) == -1)
			return -1;
	}
	m_pViewDataSettings = (CViewDataSettings*) new CViewDataSettings;
	ASSERT(m_pViewDataSettings!=NULL);
	m_pProjectionSettings = (CProjectionSettings*) new CProjectionSettings;
	ASSERT(m_pProjectionSettings!=NULL);
	m_pTransformationSettings = (CTransformationSettings*) new CTransformationSettings;
	ASSERT(m_pTransformationSettings!=NULL);
	
	return 0;
}

//
// SECZoomView overridden CScrollView members
//

//	
//	SetScrollSizes
//	
//	Store the original values in member data for later, so
//	we always know what our original setup was.
//
//      Most code borrowed from CScrollView, we're just in ANISOTROPIC mode.
//

void COWView::SetScrollSizes (	int nMapMode,
								SIZE sizeTotal,
								const SIZE& sizePage /* = sizeDefault */, // in logical units
								const SIZE& sizeLine /* = sizeDefault */) // in logical units
{
    ASSERT_VALID(this);
    
    nMapMode; //UNUSED
    // Set up the defaults
    if (sizeTotal.cx == 0 && sizeTotal.cy == 0)
	{
		sizeTotal.cx = 1;
		sizeTotal.cy = 1;
    }
    //SetWindowExt doesn't like 0,0 will bomb out.
    ASSERT(sizeTotal.cx > 0 && sizeTotal.cy > 0);
    
    m_nMapMode    = MM_ANISOTROPIC; // Need for arbitrary scaling
    m_totalLog    = sizeTotal;
    
    // Setup default Viewport extent to be conversion of Window extent
    // into device units.
    
    //BLOCK for DC
    {
		CWindowDC dc(NULL);
		dc.SetMapMode(m_nMapMode);
	
		// total size
		m_totalDev = m_totalLog;
		dc.LPtoDP((LPPOINT)&m_totalDev);
    } // Release DC here
    
    m_szOrigTotalDev = m_totalDev;
    m_szOrigPageDev  = sizePage;
    m_szOrigLineDev  = sizeLine;
    ReCalcBars();
    
    ZoomFactorChanged(); //Notify app that there's a new zoom level 1.0f.   
}

//	
//	OnPrepareDC
//	
//	Does all the work for SECZoomView.
//	
//	I copied lots of CScrollView code to here, because I can't call
//	CScrollView::OnPrepareDC, it will hose my dc that I've setup.
//	
void COWView::OnPrepareDC (	CDC* pDC, CPrintInfo* pInfo)
{
#ifdef _DEBUG
    if (m_nMapMode != MM_ANISOTROPIC) {
	TRACE0("Error: must call SetScrollSizes() before painting view\n");
	ASSERT(FALSE);
	return;
    }
#endif //_DEBUG
    
    ASSERT_VALID(pDC);
    ASSERT(m_totalLog.cx >= 0 && m_totalLog.cy >= 0);
    ASSERT(m_totalDev.cx >= 0 && m_totalDev.cx >= 0);
    
    // Set the Mapping mode, and the window and viewport extents
    // Note: This will always be ANISOTROPIC for SECZoomView
    pDC->SetMapMode(m_nMapMode);
    pDC->SetWindowExt(m_totalLog);  //Set up the logical window

    //Now figure out the origin for the viewport, depends on
    //This code is from CSCrollView
    CPoint ptVpOrg;
    
    if (!pDC->IsPrinting()) 
	{
		pDC->SetViewportExt(m_totalDev); // in device coordinates
	
		// by default shift viewport origin in negative direction of scroll
		ASSERT(pDC->GetWindowOrg() == CPoint(0,0));
		
		/*
		if (m_zoomMode == COWView::OW_ZOOMPANOVER)
		{ //Special mode for pan overviewing
			ptVpOrg.x = 0; //m_totalDev.cx /2;
			ptVpOrg.y = 0; //m_totalDev.cy /2;
		} 
		else
	
		{
		*/

		ptVpOrg = -GetDeviceScrollPosition();
		/*} */

		// Center full fit
		if (m_bCenter) 
		{
			CRect rect;
			GetClientRect(&rect);
	  
			// if client area is larger than total device size,
			// override scroll positions to place origin such that
			// output is centered in the window
			if (m_totalDev.cx < rect.Width())
			  ptVpOrg.x = (rect.Width() - m_totalDev.cx) / 2;
			if (m_totalDev.cy < rect.Height())
			  ptVpOrg.y = (rect.Height() - m_totalDev.cy) / 2;
		}
	} 
	else 
	{
		// Special case for printing
		CSize  printSize;
		printSize.cx = pDC->GetDeviceCaps(HORZRES);
		printSize.cy = pDC->GetDeviceCaps(VERTRES);
		// Maintain the origional ratio, setup origin shift
		PersistRatio(m_totalLog, printSize, ptVpOrg);
		// Zoom completely out
		pDC->SetViewportExt(printSize);
    }
    
    // Set the new viewport origin, call CView for printing behavior
    pDC->SetViewportOrg(ptVpOrg);
    CView::OnPrepareDC(pDC, pInfo);
} 


//	COWView::ReCalcBars
//	
//	
//	Since we're changing the viewport around, we'll need to
//	modify the scrollbars where CScrollView just sets them
//	up at start of day and scrolls.
//	
//      This code originally lived in CScrollView::SetScrollSizes, but
//      I had to move it here to facilitate updating the scroll bars.
//
void COWView::ReCalcBars (void)
{
    ASSERT_VALID(this);
    {  // BLOCK for DC
      CWindowDC dc(NULL);
      dc.SetMapMode(m_nMapMode);

      // Calculate new device units for scrollbar
      // Start with origional logical units from SetScrollPos
      m_pageDev = m_szOrigPageDev;
      dc.LPtoDP((LPPOINT)&m_pageDev);
      m_lineDev = m_szOrigLineDev;
      dc.LPtoDP((LPPOINT)&m_lineDev);
   } // Free DC

   // Make sure of the range
   if (m_pageDev.cy < 0)  m_pageDev.cy = -m_pageDev.cy;
   if (m_lineDev.cy < 0)  m_lineDev.cy = -m_lineDev.cy;

   // If none specified - use one tenth, Just like CScrollView
   
   //ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);
   if (m_totalDev.cx < 0 || m_totalDev.cy < 0){
       TRACE0("SECZoomView: Zoom level too low, divide by zero possible\n");
       ASSERT(FALSE);
   }
   
   if (m_pageDev.cx == 0) m_pageDev.cx = m_totalDev.cx / 10;
   if (m_pageDev.cy == 0) m_pageDev.cy = m_totalDev.cy / 10;
   if (m_lineDev.cx == 0) m_lineDev.cx = m_pageDev.cx  / 10;
   if (m_lineDev.cy == 0) m_lineDev.cy = m_pageDev.cy  / 10;

   // Now update the scrollbars
   if (m_hWnd != NULL) 
   {
      UpdateBars();
      Invalidate(TRUE); // Zoom scale changed, redraw all
   }
}



/////////////////////////////////////////////////////////////////////////////
// COWView printing

BOOL COWView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COWView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COWView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


/////////////////////////////////////////////////////////////////////////////
// COWView message handlers

void COWView::OnDraw(CDC* pDC)
{
}
 
BOOL COWView::OnEraseBkgnd(CDC* pDC) 
{
	//return CWnd::OnEraseBkgnd(pDC);

	
	// Set brush to desired background color
	CBrush backBrush(RGB(0, 0, 0));
	// Save old brush
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);     // Erase the area needed
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;
}



void  COWView::ZoomIn (CRect &rect)   // rect in logical coordinates
{
    ASSERT_VALID(this);
    ASSERT(m_nMapMode == MM_ANISOTROPIC);
    if (m_zoomMode == COWView::OW_ZOOMNORMAL)
	{	//Only if we're in normal zoom mode.
		// Make sure that the rect is normalized
		CRect zoomRect = rect;
		zoomRect.NormalizeRect();

		// Get the center of rect
		CPoint ptCenter;
		ptCenter.x = ((zoomRect.left + zoomRect.right)  / 2);
		ptCenter.y = ((zoomRect.top  + zoomRect.bottom) / 2);

		CRect clientRect;
		GetClientRect(&clientRect);

		// Calculate the new zoom scale based on input rect
		// and the current client rect.

		float fHorizScale =
		(float) (clientRect.right  + 1) / (float) zoomRect.Width();
		float fVertScale  =
		(float) (clientRect.bottom + 1) / (float) zoomRect.Height();

		// Keep the scale Isotropic
		SetZoomFactor(min(fHorizScale, fVertScale));

		UpdateViewport(&ptCenter);

		//poirier, sept 2001, begin
		//ZoomFactorChanged();
		//poirier, sept 2001, end
    }
	return;
} 

//	
//	ZoomIn
//	
//	
void COWView::ZoomIn(	CPoint *point,   // point in logical coordinates
						double  delta)    // scale factor
{
    ASSERT_VALID(this);
    ASSERT(m_nMapMode == MM_ANISOTROPIC);
    if (m_zoomMode == COWView::OW_ZOOMNORMAL)
	{
		// Increase the zoom scale.
		SetZoomFactor(m_dfZoomFactor * delta);
		
		UpdateViewport(point);

		//poirier, sept 2001, begin
		ZoomFactorChanged();
		//poirier, sept 2001, end

    } //Not in zoom mode, simply ignore it.

    return;
} 

//	
//	ZoomOut
//	
void COWView::ZoomOut(	CPoint *point,   // point in logical coordinates
						double  delta)    // scale factor
{
    ASSERT_VALID(this);
    ASSERT(m_nMapMode == MM_ANISOTROPIC);
    if (m_zoomMode == COWView::OW_ZOOMNORMAL)
	{	
		// Decrease the zoom scale.
		SetZoomFactor(m_dfZoomFactor / delta);
		
		UpdateViewport(point);

		//poirier, sept 2001, begin
		ZoomFactorChanged();
		//poirier, sept 2001, end

    }// Not in zoom mode, ignore.
    
    return;
} 

//	ZoomFit
//	
//	Zoom based on client's size.
//	Make sure client is always filled by using zoom level
//
void  COWView::ZoomFit(void)
{
    ASSERT_VALID(this);
    ASSERT(m_nMapMode == MM_ANISOTROPIC);
    
    CRect  rc;
    CPoint pt;
    CSize  sizeSb;
    
    if (m_zoomMode == COWView::OW_ZOOMFIT)
	{
		GetTrueClientSize(m_totalDev, sizeSb);    
		PersistRatio(m_totalLog, m_totalDev, pt);
		
		//poirier, sept 2001, begin
		/* 
		SetZoomFactor((double) m_totalDev.cx / m_szOrigTotalDev.cx);
		*/
		double fx = (double)m_totalDev.cx / m_szOrigTotalDev.cx;
		double fy = (double)m_totalDev.cy / m_szOrigTotalDev.cy;
		double f=fx;
		if(fy>fx) 
		{
			f = fy;
		}
		SetZoomFactor(f);
		//poirier, sept 2001, end
		UpdateBars(); //Scrollbars should go away, view fills client
		Invalidate(TRUE);
		
		ZoomFactorChanged();
    } //Not in zoom fit mode, ignore.
    return;
} 

//	
//	ZoomOriginal
//	Convenience function for zooming to original proportions.
//	
void  COWView::ZoomOriginal (CPoint *point)   // point in logical coordinates
{
    //Ok for point to be null.
    ASSERT_VALID(this);
    ASSERT(m_nMapMode == MM_ANISOTROPIC);
    // Reset the zoom scale to 1.0
    if (m_zoomMode == COWView::OW_ZOOMNORMAL)
	{
		m_dfZoomFactor = 1.0;
	
		UpdateViewport(point);

		//poirier, sept 2001, begin
		ZoomFactorChanged();
		//poirier, sept 2001, end
    
	} // We're not in zoom mode, ignore.
    return;
} 

//	ZoomPercent
//	
//	Zoom based on an integer percent..
//	
//	
void  COWView::ZoomPercent (int nPercent /* = 25% */,
							CPoint * point    /* = NULL */)
{
    if(m_parentHWND==NULL) ASSERT_VALID(this);
    ASSERT(m_nMapMode == MM_ANISOTROPIC);
    ASSERT(nPercent != 0);
    
    if (GetZoomMode() == COWView::OW_ZOOMNORMAL)
	{
		//Calculate new zoom scale based on %
		m_dfZoomFactor = nPercent/100.0;
		
		UpdateViewport(point);

		//poirier, sept 2001, begin
		ZoomFactorChanged();
		//poirier, sept 2001, end
    
	}
    return;
} 


//	
//	CenterOnLogicalPoint
//	
//	Same as CScrollView::CenterOnPoint, but for log point.
//	
//	
void COWView::CenterOnLogicalPoint(CPoint pt)
{
    // Convert the point to device coordinates
    ViewLPtoDP(&pt);
    // Account for scroll bar position
    ClientToDevice(pt);
    // Use CScrollView's function for device coordinates
    CScrollView::CenterOnPoint(pt);
} 

//	
//	GetLogicalCenterPoint
//	
CPoint COWView::GetLogicalCenterPoint (void)  
{
    CPoint pt;
    CRect rect;
    // Get the center of screen
    GetClientRect(&rect);
    pt.x = (rect.Width()  / 2);
    pt.y = (rect.Height() / 2);
    
    // Convert the point to logical coordinates
    ViewDPtoLP(&pt);
    return pt;
} 

//	
//	ViewDPtoLP
//	
//	Same as DPtoLP, but uses Client DC.
void COWView::ViewDPtoLP (
    LPPOINT lpPoints,
    int     nCount)
{
    // Convert to logical units
    // Called from View when no DC is available
    ASSERT(m_nMapMode > 0); // must be set
    CWindowDC dc(this);
    OnPrepareDC(&dc);
    dc.DPtoLP(lpPoints, nCount);
} 

void COWView::ViewDPtoLP(RECT* pRECT)
{
    // Convert to logical units
    // Called from View when no DC is available
    ASSERT(m_nMapMode > 0); // must be set
    CWindowDC dc(this);
    OnPrepareDC(&dc);
    dc.DPtoLP(pRECT);
} 

//	
//	Same as LPtoDP, but uses Client DC
//	
void COWView::ViewLPtoDP (	LPPOINT lpPoints,
							int nCount)
{
    // Convert to logical units
    // Called from View when no DC is available
    ASSERT(m_nMapMode > 0); // must be set
    CWindowDC dc(this);
    OnPrepareDC(&dc);
    dc.LPtoDP(lpPoints, nCount);
} 

void COWView::ClientToDevice (CPoint &point)
{
    // Need to account for scrollbar position
    CPoint scrollPt = GetDeviceScrollPosition();
    point.x += scrollPt.x;
    point.y += scrollPt.y;
} 

//	
//	PersistRatio
//	
//	Since we're not ISOTROPIC, make sure we maintain
//	the same x/y ratios.
//	
void COWView::PersistRatio (const CSize &orig,
							CSize &dest,
							CPoint &remainder)
{
    float ratio1 = (float) orig.cx / orig.cy;
    float ratio2 = (float) dest.cx / dest.cy;
    int   newSize;
    
    // Do nothing if they are the same
    if (ratio1 > ratio2) 
	{
		// Shrink hieght
		newSize = (int)(dest.cx / ratio1);
		remainder.x = 0;
		remainder.y = dest.cy - newSize;
		dest.cy = newSize;
    } 
	else if (ratio2 > ratio1) 
	{
		// Shrink width
		newSize = (int)(dest.cy * ratio1);
		remainder.x = dest.cx - newSize;
		remainder.y = 0;
		dest.cx = newSize;
    }
} 


void COWView::OnSize(UINT nType, int cx, int cy) 
{
    ASSERT_VALID(this);
    CScrollView::OnSize(nType, cx, cy);
    
	//2021fev08, spi, begin
	//leaving original code back unchanged
	/**/
	//2021fev08, spi, end
	if (GetZoomMode() == COWView::OW_ZOOMFIT)
	{
		ZoomFit();
	}
	
} 

//	UpdateViewport
//	
//      Called after the scale factor has
//	changed, calculates center if needed,
//	then updates the viewport, updates the
//	scroll bars and finally notifies our decendents
//      that the zoom factor has changed via ZoomLevelChanged.
//	
void COWView::UpdateViewport(CPoint * ptNewCenter)
{
    //Ok for ptNewCenter = NULL
    if(m_parentHWND==NULL) ASSERT_VALID(this);
    CPoint ptCenter;
    
	if(m_parentHWND==NULL) 
	{
		if (!ptNewCenter)
		ptCenter = GetLogicalCenterPoint();
		else 
		ptCenter = *ptNewCenter;
	}
    // Modify the Viewport extent
    m_totalDev.cx = (int) (m_szOrigTotalDev.cx * m_dfZoomFactor);
    m_totalDev.cy = (int) (m_szOrigTotalDev.cy * m_dfZoomFactor);
    if(m_parentHWND==NULL) ReCalcBars();
    
    // Set the current center point.
    if(m_parentHWND==NULL) CenterOnLogicalPoint(ptCenter);
    
    // Notify the class that a new zoom scale was done
    ZoomFactorChanged();
    return;
}


void COWView::OnViewPointset() 
{
	if(m_pViewDataSettings->bDrawPointset) 
	{
		m_pViewDataSettings->bDrawPointset = FALSE;
		Invalidate(TRUE); //erases background
	}
	else 
	{
		m_pViewDataSettings->bDrawPointset = TRUE;
		Invalidate(FALSE); //does not erase background
	}
	return;
}

void COWView::OnUpdateViewPointset(CCmdUI* pCmdUI) 
{
	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
	if(pDoc==NULL) return;
	if(pDoc->m_pPointset!=NULL)
	{
		pCmdUI->Enable(TRUE);
	}
	else 
	{
		pCmdUI->Enable(FALSE);
	}

	if(m_pViewDataSettings->bDrawPointset)	 pCmdUI->SetCheck(TRUE);
	  else pCmdUI->SetCheck(FALSE);
}


void COWView::OnViewPointsetLabels() 
{
	if(m_pViewDataSettings->bDrawPointsetLabels)
	{
		m_pViewDataSettings->bDrawPointsetLabels = FALSE;
		Invalidate(TRUE); //erases background
	}
	else 
	{
		m_pViewDataSettings->bDrawPointsetLabels = TRUE;
		Invalidate(FALSE); //does not erase background
	}
	return;
}

void COWView::OnUpdateViewPointsetLabels(CCmdUI* pCmdUI) 
{
	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
	if(pDoc==NULL) return;
	if(pDoc->m_pPointset!=NULL)
	{
		pCmdUI->Enable(TRUE);
	}
	else 
	{
		pCmdUI->Enable(FALSE);
	}
	
	if(m_pViewDataSettings->bDrawPointsetLabels) pCmdUI->SetCheck(TRUE);
	  else pCmdUI->SetCheck(FALSE);
}

void COWView::OnViewTriangleset() 
{
	if(m_pViewDataSettings->bDrawTriangleset) 
	{
		m_pViewDataSettings->bDrawTriangleset = FALSE;
		Invalidate(TRUE); //erases background
	}
	else 
	{
		m_pViewDataSettings->bDrawTriangleset = TRUE;
		Invalidate(FALSE); //does not erase background
	}
	return;
}

void COWView::OnUpdateViewTriangleset(CCmdUI* pCmdUI) 
{
	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
	if(pDoc==NULL) return;
	if(pDoc->m_pPointset!=NULL)
	{
		pCmdUI->Enable(TRUE);
	}
	else 
	{
		pCmdUI->Enable(FALSE);
	}

	if(m_pViewDataSettings->bDrawTriangleset)	 pCmdUI->SetCheck(TRUE);
	  else pCmdUI->SetCheck(FALSE);
}

void COWView::OnViewVoronoiset() 
{
	if(m_pViewDataSettings->bDrawVoronoiset) 
	{
		m_pViewDataSettings->bDrawVoronoiset = FALSE;
		Invalidate(TRUE); //erases background
	}
	else 
	{
		m_pViewDataSettings->bDrawVoronoiset = TRUE;
		Invalidate(FALSE); //does not erase background
	}
	return;
}

void COWView::OnUpdateViewVoronoiset(CCmdUI* pCmdUI) 
{
	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
	if(pDoc==NULL) return;
	if(pDoc->m_pPointset!=NULL)
	{
		pCmdUI->Enable(TRUE);
	}
	else 
	{
		pCmdUI->Enable(FALSE);
	}

	if(m_pViewDataSettings->bDrawVoronoiset) pCmdUI->SetCheck(TRUE);
	  else pCmdUI->SetCheck(FALSE);
}

void COWView::OnViewSegmentset() 
{
	if(m_pViewDataSettings->bDrawSegmentset) 
	{
		m_pViewDataSettings->bDrawSegmentset = FALSE;
		Invalidate(TRUE); //erases background
	}
	else 
	{
		m_pViewDataSettings->bDrawSegmentset = TRUE;
		Invalidate(FALSE); //does not erase background
	}
	return;
}

void COWView::OnUpdateViewSegmentset(CCmdUI* pCmdUI) 
{
	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
	if(pDoc==NULL) return;
	if(pDoc->m_pPointset==NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if(pDoc->m_pSegmentset!=NULL)
	{
		pCmdUI->Enable(TRUE);
	}
	else 
	{
		pCmdUI->Enable(FALSE);
	}


	if(m_pViewDataSettings->bDrawSegmentset) pCmdUI->SetCheck(TRUE);
	  else pCmdUI->SetCheck(FALSE);
}

void COWView::OnViewGotopartition() 
{
	CGotoPartitionDlg* pGotoPartitionDlg = ((COIIIApp*)AfxGetApp())->GetGotoPartitionDlg();
	if(pGotoPartitionDlg==NULL) return;
	return;
}

void COWView::OnUpdateViewGotopartition(CCmdUI* pCmdUI) 
{
	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
	if(pDoc==NULL) return;
	if(pDoc->m_pPointset==NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if(pDoc->m_pSegmentset!=NULL)
	{
		pCmdUI->Enable(TRUE);
	}
	else 
	{
		pCmdUI->Enable(FALSE);
	}
}

//spi, nov 2018, begin
//LONG COWView::OnUpdateViewInfo(WPARAM wParam, LPARAM lParam)
LRESULT COWView::OnUpdateViewInfo(WPARAM wParam, LPARAM lParam)
//spi, nov 2018, end
{
	CSettingsSheet* pSettingsSheet = (CSettingsSheet*) wParam;
	ASSERT(pSettingsSheet->IsKindOf(RUNTIME_CLASS(CSettingsSheet)));

	m_pViewDataSettings->CopyAllValuesFrom(pSettingsSheet->m_pViewDataSettings);
	m_pProjectionSettings->CopyAllValuesFrom(pSettingsSheet->m_pProjectionSettings);
	m_pTransformationSettings->CopyAllValuesFrom(pSettingsSheet->m_pTransformationSettings);
	Invalidate();
	return TRUE;
}

//spi, nov 2018, begin
//LONG COWView::OnUpdateDocInfo(WPARAM wParam, LPARAM lParam)
LRESULT COWView::OnUpdateDocInfo(WPARAM wParam, LPARAM lParam)
//spi, nov 2018, end
{
	CSettingsSheet* pSettingsSheet = (CSettingsSheet*) wParam;
	ASSERT(pSettingsSheet->IsKindOf(RUNTIME_CLASS(CSettingsSheet)));

	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
	if(pDoc==NULL) return FALSE;

	//keep the default document's view settings
	//m_pViewDataSettings->CopyAllValuesFrom(pSettingsSheet->m_pViewDataSettings);
	//m_pProjectionSettings->CopyAllValuesFrom(pSettingsSheet->m_pProjectionSettings);
	//m_pTransformationSettings->CopyAllValuesFrom(pSettingsSheet->m_pTransformationSettings);

	pDoc->m_pExtractionSettings->CopyAllValuesFrom(pSettingsSheet->m_pExtractionSettings);
	pDoc->m_pFusionSettings->CopyAllValuesFrom(pSettingsSheet->m_pFusionSettings);
	pDoc->m_pClassificationSettings->CopyAllValuesFrom(pSettingsSheet->m_pClassificationSettings);
	pDoc->m_pHistoSettings->CopyAllValuesFrom(pSettingsSheet->m_pHistoSettings);
	pDoc->m_pScatterSettings->CopyAllValuesFrom(pSettingsSheet->m_pScatterSettings);

	pDoc->m_pTraceSettings->CopyAllValuesFrom(pSettingsSheet->m_pTraceSettings);
	pDoc->m_pBehaviorSettings->CopyAllValuesFrom(pSettingsSheet->m_pBehaviorSettings);

	return TRUE;
}



void COWView::SetScrollInfo()
{
	CRect myRect = GetDocument()->GetWorldRect();
    CSize sizeDoc;
	sizeDoc.cx = myRect.Width();
	sizeDoc.cy = myRect.Height();
    SetScrollSizes(MM_TEXT, sizeDoc);    
}


/////////////////////////////////////////////////////////////////////////////
// COWView diagnostics

#ifdef _DEBUG
void COWView::AssertValid() const
{
	//CView::AssertValid();
	//CScrollView::AssertValid();
    if (m_dfZoomFactorMin != -1 && m_dfZoomFactorMax !=-1)
	{
		ASSERT(m_dfZoomFactorMin <= m_dfZoomFactor);
		ASSERT(m_dfZoomFactor <= m_dfZoomFactorMax);
    }
    CView::AssertValid();

}

void COWView::Dump(CDumpContext& dc) const
{
	//CView::Dump(dc);
	CScrollView::Dump(dc);
}

COW2Doc* COWView::GetDocument() // non-debug version is inline
{
	if(m_parentHWND==NULL) //spi 2014
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COW2Doc)));
		return (COW2Doc*)m_pDocument;
	}
	else
	{
		//note: for the time being, only COW2Doc* can be used
		ASSERT(m_pCOWDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));
		return (COW2Doc*)m_pCOWDoc;
	}
}
#endif //_DEBUG

void COWView::OnViewGotoclassification() 
{
	CGotoPartitionDlg* pGotoPartitionDlg = ((COIIIApp*)AfxGetApp())->GetGotoClassificationDlg();
	if(pGotoPartitionDlg==NULL) return;
	return;
}

void COWView::OnUpdateViewGotoclassification(CCmdUI* pCmdUI) 
{
	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
	if(pDoc==NULL) return;
	if(pDoc->m_pPointset==NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if(pDoc->m_pClassset!=NULL)
	{
		pCmdUI->Enable(TRUE);
	}
	else 
	{
		pCmdUI->Enable(FALSE);
	}
}

void COWView::UpdateViewPixelsetDisplayFlags()
{
	//should be called right after setting m_pViewDataSettings->bDrawPixelset,
	//from OnInitialUpdate(), OnViewPixelset() and etc.
	if(m_pViewDataSettings->bDrawPixelset==FALSE)
	{
		m_bViewPixelset_DisplayRed = FALSE;
		m_bViewPixelset_DisplayGreen = FALSE;
		m_bViewPixelset_DisplayBlue = FALSE;
		m_bViewPixelset_DisplayHue = FALSE;
		m_bViewPixelset_DisplaySaturation = FALSE;
		m_bViewPixelset_DisplayIntensity = FALSE;
	}
	else
	{
		//if color image
		COW2Doc* pDoc = (COW2Doc*)GetDocument();	
		if(pDoc!=NULL)
		{
			//if rgb
			if(pDoc->m_pPixelset!=NULL && pDoc->m_pPixelset->IsRedPresent())
			{
				m_bViewPixelset_DisplayRed = TRUE;
			}
			if(pDoc->m_pPixelset!=NULL && pDoc->m_pPixelset->IsGreenPresent())
			{
				m_bViewPixelset_DisplayGreen = TRUE;
			}
			if(pDoc->m_pPixelset!=NULL && pDoc->m_pPixelset->IsBluePresent())
			{
				m_bViewPixelset_DisplayBlue = TRUE;
			}

			if(m_bViewPixelset_DisplayRed==FALSE
				&& m_bViewPixelset_DisplayGreen==FALSE
				&& m_bViewPixelset_DisplayBlue==FALSE)
			{
				//if hsi
				if(pDoc->m_pPixelset!=NULL && pDoc->m_pPixelset->IsIntensityPresent())
				{
					m_bViewPixelset_DisplayIntensity = TRUE;
				}
				if(pDoc->m_pPixelset!=NULL && pDoc->m_pPixelset->IsHuePresent())
				{
					m_bViewPixelset_DisplayHue = TRUE;
				}
				if(pDoc->m_pPixelset!=NULL && pDoc->m_pPixelset->IsSaturationPresent())
				{
					m_bViewPixelset_DisplaySaturation = TRUE;
				}
			}
			else
			{
				m_bViewPixelset_DisplayIntensity = FALSE;
				m_bViewPixelset_DisplayHue = FALSE;
				m_bViewPixelset_DisplaySaturation = FALSE;
			}
		}
	}
	return;
}

void COWView::OnViewPixelset() 
{
	if(m_pViewDataSettings->bDrawPixelset) 
	{
		m_pViewDataSettings->bDrawPixelset = FALSE;
		UpdateViewPixelsetDisplayFlags();
		Invalidate(TRUE); //erase background
	}
	else 
	{
		m_pViewDataSettings->bDrawPixelset = TRUE;
		UpdateViewPixelsetDisplayFlags();
		Invalidate(FALSE); //does not erase background
	}
	//UpdateWindow();
	GetDocument()->UpdateAllViews(NULL);
	return;
}

void COWView::OnUpdateViewPixelset(CCmdUI* pCmdUI) 
{
	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
	if(pDoc==NULL) return;
	if(pDoc->m_pPixelset!=NULL)
	{
		pCmdUI->Enable(TRUE);
	}
	else 
	{
		pCmdUI->Enable(FALSE);
	}
	BOOL bCheckit = FALSE;
	//poirier, sept 2001, begin
	/*
	if(m_bViewPixelset_DisplayRed==FALSE &&
		m_bViewPixelset_DisplayGreen==FALSE &&
		m_bViewPixelset_DisplayBlue==FALSE &&
		m_bViewPixelset_DisplayHue==FALSE &&
		m_bViewPixelset_DisplaySaturation==FALSE &&
		m_bViewPixelset_DisplayIntensity==FALSE)
	*/
	if(m_bViewPixelset_DisplayRed==TRUE ||
		m_bViewPixelset_DisplayGreen==TRUE ||
		m_bViewPixelset_DisplayBlue==TRUE ||
		m_bViewPixelset_DisplayHue==TRUE ||
		m_bViewPixelset_DisplaySaturation==TRUE ||
		m_bViewPixelset_DisplayIntensity==TRUE)
	//poirier, sept 2001, end
	{
		bCheckit = TRUE;
	}
	if(m_pViewDataSettings->bDrawPixelset) 
	{
		if(bCheckit) pCmdUI->SetCheck(TRUE);
		  else pCmdUI->SetCheck(FALSE);
	}
	else 
	{
		pCmdUI->SetCheck(FALSE);
	}
	return;
}


void COWView::GetPixelsetChannelVisibility(int* pINT, int nINT)
{
	COW2Doc* pDoc = GetDocument();

	if(pINT==NULL)
	{
		ASSERT(FALSE);
		return;
	}
	if(m_pViewDataSettings->bDrawPixelset==TRUE && pDoc->m_pPixelset)
	{
		int nchannel = pDoc->m_pPixelset->GetPixelsetNumberOfChannel();
		if(nchannel==3)
		{
			if(m_bViewPixelset_DisplayRed==TRUE ||
				m_bViewPixelset_DisplayGreen==TRUE ||
				m_bViewPixelset_DisplayBlue==TRUE)
			{
				pINT[0] = m_bViewPixelset_DisplayRed;
				pINT[1] = m_bViewPixelset_DisplayGreen;
				pINT[2] = m_bViewPixelset_DisplayBlue;
			}
			else if(m_bViewPixelset_DisplayHue==TRUE ||
				m_bViewPixelset_DisplaySaturation==TRUE ||
				m_bViewPixelset_DisplayIntensity==TRUE)
			{
				pINT[0] = m_bViewPixelset_DisplayHue;
				pINT[1] = m_bViewPixelset_DisplaySaturation;
				pINT[2] = m_bViewPixelset_DisplayIntensity;
			}
		}
		else if(nchannel==1)
		{
			pINT[0] = m_bViewPixelset_DisplayIntensity;
		}
		else
		{
			for(int i=0; i<nchannel; i++) pINT[i]=1;
		}

	}
	else
	{
		int i=0; while(i<nINT) pINT[i] = 0;
	}
	return;
}

void COWView::Update_ViewDataSettings_DrawPixelsetFlag()
{
	if(m_bViewPixelset_DisplayRed==FALSE &&
		m_bViewPixelset_DisplayGreen==FALSE &&
		m_bViewPixelset_DisplayBlue==FALSE &&
		m_bViewPixelset_DisplayHue==FALSE &&
		m_bViewPixelset_DisplaySaturation==FALSE &&
		m_bViewPixelset_DisplayIntensity==FALSE)
	{
		m_pViewDataSettings->bDrawPixelset = FALSE;
	}
	else
	{
		m_pViewDataSettings->bDrawPixelset = TRUE;
	}
}

void COWView::OnViewPixelsetBlue() 
{
	if(m_bViewPixelset_DisplayBlue) 
	{
		m_bViewPixelset_DisplayBlue = FALSE;
		Update_ViewDataSettings_DrawPixelsetFlag();
		Invalidate(TRUE); 
	}
	else 
	{
		m_bViewPixelset_DisplayBlue = TRUE;
		m_pViewDataSettings->bDrawPixelset = TRUE;
		Invalidate(FALSE); //does not erase background
	}
	//UpdateWindow();
	GetDocument()->UpdateAllViews(NULL);
	return;
}

void COWView::OnUpdateViewPixelsetBlue(CCmdUI* pCmdUI) 
{
	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
	if(pDoc==NULL) return;
	
	if(pDoc->m_pPixelset!=NULL && pDoc->m_pPixelset->IsBluePresent())
	{
		pCmdUI->Enable(TRUE);
	}
	else 
	{
		pCmdUI->Enable(FALSE);
	}

	if(m_pViewDataSettings->bDrawPixelset && m_bViewPixelset_DisplayBlue)	 
	{
		pCmdUI->SetCheck(TRUE);
	}
	else 
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void COWView::OnViewPixelsetGreen() 
{
	if(m_bViewPixelset_DisplayGreen) 
	{
		m_bViewPixelset_DisplayGreen = FALSE;
		Update_ViewDataSettings_DrawPixelsetFlag();
		Invalidate(TRUE); 
	}
	else 
	{
		m_bViewPixelset_DisplayGreen = TRUE;
		m_pViewDataSettings->bDrawPixelset = TRUE;
		Invalidate(FALSE); //does not erase background
	}
	//UpdateWindow();
	GetDocument()->UpdateAllViews(NULL);
	return;
}

void COWView::OnUpdateViewPixelsetGreen(CCmdUI* pCmdUI) 
{
	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
	if(pDoc==NULL) return;
	
	if(pDoc->m_pPixelset!=NULL && pDoc->m_pPixelset->IsGreenPresent())
	{
		pCmdUI->Enable(TRUE);
	}
	else 
	{
		pCmdUI->Enable(FALSE);
	}

	if(m_pViewDataSettings->bDrawPixelset && m_bViewPixelset_DisplayGreen)	 
	{
		pCmdUI->SetCheck(TRUE);
	}
	else 
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void COWView::OnViewPixelsetHue() 
{
	if(m_bViewPixelset_DisplayHue) 
	{
		m_bViewPixelset_DisplayHue = FALSE;
		Update_ViewDataSettings_DrawPixelsetFlag();
		Invalidate(TRUE); //erases background
	}
	else 
	{
		m_bViewPixelset_DisplayHue = TRUE;
		m_pViewDataSettings->bDrawPixelset = TRUE;
		Invalidate(FALSE); //does not erase background
	}
	return;
}

void COWView::OnUpdateViewPixelsetHue(CCmdUI* pCmdUI) 
{
	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
	if(pDoc==NULL) return;
	
	if(pDoc->m_pPixelset!=NULL 
		&& !pDoc->m_pPixelset->IsRGB() 
		&& pDoc->m_pPixelset->IsHuePresent())
	{
		pCmdUI->Enable(TRUE);
	}
	else 
	{
		pCmdUI->Enable(FALSE);
	}

	if(m_pViewDataSettings->bDrawPixelset && m_bViewPixelset_DisplayHue)	 
	{
		pCmdUI->SetCheck(TRUE);
	}
	else 
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void COWView::OnViewPixelsetIntensity() 
{
	if(m_bViewPixelset_DisplayIntensity) 
	{
		m_bViewPixelset_DisplayIntensity = FALSE;
		Update_ViewDataSettings_DrawPixelsetFlag();
		Invalidate(TRUE); //erases background
	}
	else 
	{
		m_bViewPixelset_DisplayIntensity = TRUE;
		m_pViewDataSettings->bDrawPixelset = TRUE;
		Invalidate(FALSE); //does not erase background
	}
	return;
}

void COWView::OnUpdateViewPixelsetIntensity(CCmdUI* pCmdUI) 
{
	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
	if(pDoc==NULL) return;
	
	if(pDoc->m_pPixelset!=NULL 
		&& !pDoc->m_pPixelset->IsRGB() 
		&& pDoc->m_pPixelset->IsIntensityPresent())
	{
		pCmdUI->Enable(TRUE);
	}
	else 
	{
		pCmdUI->Enable(FALSE);
	}

	if(m_pViewDataSettings->bDrawPixelset && m_bViewPixelset_DisplayIntensity)	 
	{
		pCmdUI->SetCheck(TRUE);
	}
	else 
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void COWView::OnViewPixelsetRed() 
{
	if(m_bViewPixelset_DisplayRed) 
	{
		m_bViewPixelset_DisplayRed = FALSE;
		Update_ViewDataSettings_DrawPixelsetFlag();
		Invalidate(TRUE); 
	}
	else 
	{
		m_bViewPixelset_DisplayRed = TRUE;
		m_pViewDataSettings->bDrawPixelset = TRUE;
		Invalidate(FALSE); //does not erase background
	}
	//UpdateWindow();
	GetDocument()->UpdateAllViews(NULL);
	return;
}

void COWView::OnUpdateViewPixelsetRed(CCmdUI* pCmdUI) 
{
	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
	if(pDoc==NULL) return;
	
	if(pDoc->m_pPixelset!=NULL && pDoc->m_pPixelset->IsRedPresent())
	{
		pCmdUI->Enable(TRUE);
	}
	else 
	{
		pCmdUI->Enable(FALSE);
	}

	if(m_pViewDataSettings->bDrawPixelset && m_bViewPixelset_DisplayRed)	 
	{
		pCmdUI->SetCheck(TRUE);
	}
	else 
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void COWView::OnViewPixelsetSaturation() 
{
	if(m_bViewPixelset_DisplaySaturation) 
	{
		m_bViewPixelset_DisplaySaturation = FALSE;
		Update_ViewDataSettings_DrawPixelsetFlag();
		Invalidate(TRUE); //erases background
	}
	else 
	{
		m_bViewPixelset_DisplaySaturation = TRUE;
		m_pViewDataSettings->bDrawPixelset = TRUE;
		Invalidate(FALSE); //does not erase background
	}
}

void COWView::OnUpdateViewPixelsetSaturation(CCmdUI* pCmdUI) 
{
	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
	if(pDoc==NULL) return;
	
	if(pDoc->m_pPixelset!=NULL 
		&& !pDoc->m_pPixelset->IsRGB() 
		&& pDoc->m_pPixelset->IsSaturationPresent())
	{
		pCmdUI->Enable(TRUE);
	}
	else 
	{
		pCmdUI->Enable(FALSE);
	}

	if(m_pViewDataSettings->bDrawPixelset && m_bViewPixelset_DisplaySaturation)	 
	{
		pCmdUI->SetCheck(TRUE);
	}
	else 
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void COWView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
}

void COWView::OnViewPointsetCross() 
{
	// TODO: Add your command handler code here
	
}

void COWView::OnViewPointsetEllipse() 
{
	// TODO: Add your command handler code here
	
}

void COWView::OnViewPointsetMark() 
{
	// TODO: Add your command handler code here
	
}

void COWView::OnViewTrianglesetLabels() 
{
	if(m_pViewDataSettings->bDrawTrianglesetLabels==TRUE) 
	{
		m_pViewDataSettings->bDrawTrianglesetLabels = FALSE;
		Invalidate(TRUE); 

	}
	else
	{
		m_pViewDataSettings->bDrawTrianglesetLabels = TRUE;
		Invalidate(FALSE); 
	}
	return;
}

void COWView::OnViewTriangleset3vertices() 
{
	// TODO: Add your command handler code here
	
}

void COWView::OnViewTrianglesetCenterpoints() 
{
	// TODO: Add your command handler code here
	
}

void COWView::OnViewTrianglesetPolygon() 
{
	// TODO: Add your command handler code here
	
}

void COWView::OnViewVoronoisetLabels() 
{
	// TODO: Add your command handler code here
	
}

void COWView::OnUpdateViewPointsetCross(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void COWView::OnUpdateViewPointsetEllipse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void COWView::OnUpdateViewPointsetMark(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void COWView::OnUpdateViewTriangleset3vertices(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void COWView::OnUpdateViewTrianglesetCenterpoints(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void COWView::OnUpdateViewTrianglesetLabels(CCmdUI* pCmdUI) 
{
	if(m_pViewDataSettings->bDrawTrianglesetLabels) pCmdUI->SetCheck(TRUE);
	  else pCmdUI->SetCheck(FALSE);
	
}

void COWView::OnUpdateViewTrianglesetPolygon(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void COWView::OnUpdateViewVoronoisetLabels(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

BOOL COWView::HasToDrawPixel()
{
	if(m_pViewDataSettings->bDrawPixelset==TRUE)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL COWView::HasToDrawOverlay()
{
	if(m_pViewDataSettings->bDrawPointset==TRUE
		|| m_pViewDataSettings->bDrawTriangleset==TRUE
		|| m_pViewDataSettings->bDrawVoronoiset==TRUE
		|| m_pViewDataSettings->bDrawSegmentset==TRUE
		|| m_pViewDataSettings->bDrawPointsetLabels==TRUE
		|| m_pViewDataSettings->bDrawTrianglesetLabels==TRUE
		|| m_pViewDataSettings->bDrawTriCenterLabels==TRUE
		|| m_pViewDataSettings->bDrawSegmentsetLabels==TRUE
		|| m_pViewDataSettings->bDrawClasssetLabels==TRUE)
	{
		return TRUE;
	}
	return FALSE;
}