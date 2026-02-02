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

#include "stdafx.h"
#include "oifiilib.h" //spi 2014


#include "gl_owgraph.h" 
       
#include "limits.h" 
#include "afxext.h"

//#include "graphset.h"
//#include "graphmdi.h"
//#include "graphdoc.h"   
//#include "graphvw.h"
//#include "dataview.h"
//#include "datamdi.h"
//#include "private.h"  
   
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
    

///////////////////////////////
//macros private to this module
///////////////////////////////  

//#define MMHIMETRIC_TO_MMTEXT_SCALEFACTOR	35 
#define MMHIMETRIC_TO_MMTEXT_SCALEFACTOR	28 
                    

//a few graph drawing parameter
#define GRAPH_MINXDIVISIONSIZE_INCHARWIDTH	5
#define GRAPH_TOTALXMARGINSIZE_INCHARWIDTH	10
#define GRAPH_TOTALYMARGINSIZE_INCHARHEIGHT	3
#define GRAPH_NUMBEROFHORZDIV	4	
#define GRAPH_NUMBEROFVERTDIV	4

//proportion of one division size by a tick size
#define GRAPH_DIVISIONSIZE_BY_TICKSIZE_RATIO	30  

//number of pixel per barwidth required for a bar to have a line contour (using black pen)
#define GRAPH_BARCONTOUR_THRESHOLD_INPIXEL	5

#define GRAPH_BARPOS_INVISIBLE	-1
                                
                                
                                
                                
// Allowed Values for Axis Graduations, GraphSet Color and Name
static int nValidGraduations[] =
{
	1, 2, 4, 5, 8, 10, 16, 20, 25, 32, 40, 50, 60, 64, 75, 100, 128, 150, 200, 250, 
	256, 300, 400, 500, 512, 750, 1000, 1024, 1500, 2000, 2048, 2500, 3000, 4000, 5000, 
	7500, 10000, 15000, 20000, 25000, 30000, 40000, 0
};


//maximum number of graph set displayed at one time, it is in fact limited by the number of
//color present in our color table.  more graphsets can be stored in the graphdoc but only
//MAXCHANNELNUMBER will be displayed at one time.
#define MAXCHANNELNUMBER 20


//this color table is used by the view to give a default color to the graphset only when
//the graphset's own color index is set to 0.  If MAXCHANNELNUMBER is modified, the size of 
//this table should also be.
static COLORREF colorTable[] =
{
	RGB(128, 128, 128),     // Grey
	RGB(255,   0,   0),     // Red
	RGB(  0, 255,   0),     // Green
	RGB(  0,   0, 255),     // Blue
	RGB(255, 255,   0),     // Yellow
	RGB(255,   0, 255),     // Magenta
	RGB(  0, 255, 255),     // Cyan
	RGB(  0,   0,   0),     // Black (id = 7)
	RGB(128, 128, 128),     // Grey
	RGB(128, 128,   0),     // Brown
	RGB(128,   0, 128),      // Purple
	RGB(128, 128, 128),     // Grey
	RGB(255,   0,   0),     // Red
	RGB(  0, 255,   0),     // Green
	RGB(  0,   0, 255),     // Blue
	RGB(255, 255,   0),     // Yellow
	RGB(255,   0, 255),     // Magenta
	RGB(  0, 255, 255),     // Cyan
	RGB(  0,   0,   0),     // Black (id = 18)
	RGB(128, 128, 128),     // Grey
	RGB(128, 128,   0),     // Brown
	RGB(128,   0, 128)      // Purple
};

static COLORREF colorGrayTable[] =
{
	RGB(255,   0,   0),     // Red (first channel)
	RGB(128, 128, 128),     // Gray 50%
	RGB(210, 210, 210),     // Gray 80%
	RGB(196, 196, 196),     // Gray 75% 
	RGB(160, 160, 160),     // Gray 60%
	RGB(255, 255, 255),     // Gray 100% (white)
	RGB(64,   64,  64),     // Gray 25%
	RGB(128, 128, 128),     // Gray 50%
	RGB(196, 196, 196),     // Gray 75% 
	RGB(90,   90,  90),     // Gray 40%
	RGB(225, 225, 225),     // Gray 90%
	RGB(128, 128, 128),     // Gray
	RGB(64,   64,  64),     // Gray 25%
	RGB(128, 128, 128),     // Gray 50%
	RGB(196, 196, 196),     // Gray 75% 
	RGB(90,   90,  90),     // Gray 40%
	RGB(255, 255, 255),     // Gray 100% (white)
	RGB(128, 128, 128),     // Gray 50%
	RGB(  0,   0,   0),     // Gray 0% (black)
	RGB(128, 128, 128),     // Gray	50%
	RGB(30,  30,   30),     // Gray 10%
	RGB(225, 225, 225)      // Gray 90%
};

//this name table is used by the view to give a default color to the graphset only when
//the graphset's own name is set to "".  If MAXCHANNELNUMBER is modified, the size of 
//this table should also be.
static char* channelnumberTable[] =
{    
	"Ch0",  //Channel 0
	"Ch1", 	//Channel 1
	"Ch2", 	//Channel 2
	"Ch3", 	//Channel 3
	"Ch4",	//Channel 4
	"Ch5",	//Channel 5
	"Ch6",	//Channel 6
	"Ch7",	//Channel 7
	"Ch8",	//Channel 8
	"Ch9", 	//Channel 9
	"Ch10", //Channel 10
	"Ch11", //Channel 11
	"Ch12", //Channel 12
	"Ch13", //Channel 13
	"Ch14",	//Channel 14
	"Ch15",	//Channel 15
	"Ch16",	//Channel 16
	"Ch17",	//Channel 17
	"Ch18",	//Channel 18
	"Ch19", //Channel 19
	"Ch20", //Channel 20
	"Ch##"  //Channel ..
};


static char* channelnameTable[] =
{                      
	"Gray", //Channel 0
	"Red", 	//Channel 1
	"Green",//Channel 2
	"Blue", //Channel 3
	"Band4",//Channel 4
	"Band5",//Channel 5
	"Band6",//Channel 6
	"Band7",//Channel 7
	"Band8",//Channel 8
	"Band9",//Channel 9
	"Band10",//Channel 10
	"Band11",//Channel 11
	"Band12",//Channel 12
	"Band13",//Channel 13
	"Band14",//Channel 14
	"Band15",//Channel 15
	"Band16",//Channel 16
	"Band17",//Channel 17
	"Band18",//Channel 18
	"Band19",//Channel 19
	"Band20",//Channel 20
	"Band##" //Channel ..
};


            
            
IMPLEMENT_DYNCREATE(COWGraphView, CScrollView)
                   
BEGIN_MESSAGE_MAP(COWGraphView, CScrollView)
	//{{AFX_MSG_MAP(COWGraphView)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU0, OnSetVisibility)
	ON_COMMAND_EX(ID_VIEW_ZOOM_IN_11, OnViewZoomIn)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_WM_RBUTTONDOWN() 
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS() 
	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_VIEW_STATUS_BAR, OnViewStatusBar)  
	ON_COMMAND_EX(ID_VIEW_ZOOM_IN_21, OnViewZoomIn)
	ON_COMMAND_EX(ID_VIEW_ZOOM_IN_31, OnViewZoomIn)
	ON_COMMAND_EX(ID_VIEW_ZOOM_IN_41, OnViewZoomIn)
	ON_COMMAND_EX(ID_VIEW_ZOOM_IN_51, OnViewZoomIn)
	ON_COMMAND_EX(ID_VIEW_ZOOM_IN_61, OnViewZoomIn)
	ON_COMMAND_EX(ID_VIEW_ZOOM_IN_71, OnViewZoomIn)
	ON_COMMAND_EX(ID_VIEW_ZOOM_IN_81, OnViewZoomIn)
	ON_COMMAND(ID_VIEW_AS_LINEGRAPH, OnViewAsLineGraph)
	ON_COMMAND(ID_VIEW_AS_VBARGRAPH, OnViewAsVBarGraph)
	ON_COMMAND(ID_VIEW_AS_HBARGRAPH, OnViewAsHBarGraph)
	ON_COMMAND(ID_VIEW_FIT_ALL, OnViewFitAll)
	ON_COMMAND(ID_VIEW_FIT_ONEVALUEPERPIXEL, OnViewFitOneValuePerPixel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FIT_ONEVALUEPERPIXEL, OnUpdateViewFitOneValuePerPixel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateViewStatusBar)
	ON_COMMAND(ID_OPTIONS_AUTORESCALING, OnOptionAutoRescaling)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_AUTORESCALING, OnUpdateOptionAutoRescaling)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU1, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU2, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU3, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU4, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU5, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU6, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU7, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU8, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU9, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU10, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU11, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU12, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU13, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU14, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU15, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU16, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU17, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU18, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU19, OnSetVisibility)
	ON_COMMAND_EX(ID_BASE_POPUP_GRAPHMENU20, OnSetVisibility)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
#ifdef OWGRAPH_INTERNAL	//when OWGRAPH linked with the application
	#ifdef OIIIAPP //and when the app is OIII Application
	#include "resource.h"
	ON_COMMAND(ID_VIEW_GRAPHDATAVIEW, OnViewGraphData)
	#endif //OWGRAPH_INTERNAL && OIIIAPP with OIII Application
#endif
END_MESSAGE_MAP()


//////////////
// Constructor
//////////////     
COWGraphView::COWGraphView()
{   
	//user-selectable default values
	m_ldisplaymode = FITINWINDOW;
	//poirier, march 27 1997, moved in graphdoc and in OnInitialUpdate() here below 
	m_lgraphtype = 0; //this value should always be overriden by the graphdoc's m_lgraphtype value (GRAPH_VBAR, GRAPH_LINE, ...)
	m_ldisplayfit = DISPLAY_FIT_MAX_VALUE;
	m_ldisplayorder = DISPLAY_ORDER_NONE; 

	m_bFitDataIn = TRUE;  
	m_bStatusBarVisible = TRUE; 
	//m_lGrid = GRID_HORZ; //you can combine GRID_HORZ with GRID_VERT or 0 for none
	m_bInterpolationDesired = FALSE; 

	m_bVBarVisible = TRUE;
	m_nPrevVBarPos = GRAPH_BARPOS_INVISIBLE; 
	m_nPrevVBarOffset = GRAPH_BARPOS_INVISIBLE;
	m_nPrevVBarHeight = GRAPH_BARPOS_INVISIBLE;
	
	m_bUpdate = TRUE;  
	
	//initialize to zero a few variables (these values are usualy computed during
	//the paint process, by function GetGraphMetric() and PlotGraphAsLine() or Bar() ).
	//these values are member variables because they are shared between the paint and
	//the OnMouseMove() functions.
	m_ptGraphOrigin.x = 0;
	m_ptGraphOrigin.y = 0;
	m_sizeGraph.cx = 0;
	m_sizeGraph.cy = 0;  
	m_sizeText.cx = 0;
	m_sizeText.cy = 0;
	m_sizePixelsPerTick.cx = 0; 
	m_sizePixelsPerTick.cy = 0; 
	m_nXValuePerTick = 0L;
	m_nYValuePerTick = 0L;  
    m_nBarWidth = 0L;
	
	m_zoomfactor = 1L;
	 
	m_numberofvalueperpixel = 0;
	m_nremainder = 0; 
	
	/* SPoirier fix */
	m_HasFocus = TRUE; 

	//poirier, march 27 1997
	m_SCATTERdfMaxValueX = 0.0;
	m_SCATTERdfMinValueX = 0.0;
	m_SCATTERdfMaxValueY = 0.0;
	m_SCATTERdfMinValueY = 0.0;
	m_SCATTERdfRangeX = 0.0;
	m_SCATTERdfRangeY = 0.0;

	//default, but will be set by the corresponding graphdoc's data member 
	m_bUseDefaultMultiColorTable = TRUE;
	m_bUseDefaultGrayColorTable = FALSE;
}
  
              
/////////////
// Destructor
/////////////              
COWGraphView::~COWGraphView()
{
}



///////////////////
// message handlers
///////////////////
int COWGraphView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//SetScrollSize is called only when window is created, here, and everytime
	//the window is resized.
	SetScrollSizes();

	//poirier, march 27 1997
	COWGraphDoc* pDoc = (COWGraphDoc*)GetDocument();
	if(pDoc) 
	{
		m_lgraphtype = pDoc->m_lgraphtype; //GRAPH_LINE; //GRAPH_VBAR  //GRAPH_LINE
		m_lGrid = pDoc->m_lviewgrid; //you can combine GRID_HORZ with GRID_VERT or 0 for none
		m_bUseDefaultMultiColorTable = pDoc->m_bUseDefaultMultiColorTable;
		m_bUseDefaultGrayColorTable = pDoc->m_bUseDefaultGrayColorTable;

	}
	else
	{
		m_lgraphtype = GRAPH_LINE;
		m_lGrid = GRID_HORZ | GRID_VERT;
		m_bUseDefaultMultiColorTable = TRUE;
		m_bUseDefaultGrayColorTable = FALSE;
	}
	m_crBackgroundColor = pDoc->m_crDefaultBackgroundColor;
	m_crTextColor = pDoc->m_crDefaultTextColor;;
	colorTable[7] = m_crTextColor;
	colorTable[18] = m_crTextColor;
	return 0;
}
                            
void COWGraphView::OnInitialUpdate()
{    
	// get graph device size.
    CDC* pDC = GetDC();
    CSize sizeGraphWnd; 
	sizeGraphWnd.cx = (pDC->GetDeviceCaps(HORZRES))/2;
	sizeGraphWnd.cy = (pDC->GetDeviceCaps(VERTRES))/3;
    ReleaseDC(pDC);
         
	// adjust frame dimension to some fraction of graph device size.
	COWGraphMDIChildWnd* pMDIFrame = (COWGraphMDIChildWnd*)GetParentFrame();	
	ASSERT(pMDIFrame->IsKindOf(RUNTIME_CLASS(COWGraphMDIChildWnd)));
	pMDIFrame->SetFrameSize(sizeGraphWnd);
	
    return;
}


// DO NOT:
// Do not call this function during OnDraw() is processed, because CScrollView::SetScrollSizes()
// may generate a WM_PAINT message, therefore you could end up having multiple repaint. 
// 
// USE:
// SetScrollSize is called only when window is created, here, and everytime
// the window is resized.
//
void COWGraphView::SetScrollSizes()
{          
	//1) using GetGraphMetrics(), we compute new graphic size
	int nHorzDiv = GRAPH_NUMBEROFHORZDIV;
	int nVertDiv = GRAPH_NUMBEROFVERTDIV; 
	CSize sizeGraph;
	CSize sizePixelsPerTick;    
	CPoint ptOrigin;
		
	CDC* pDC = GetDC(); 
	int nCharHeight = GetGraphMetrics(pDC, nHorzDiv, nVertDiv,
								sizeGraph, sizePixelsPerTick, ptOrigin);  
	//2) then we can set the scroll sizes properly.
	CScrollView::SetScrollSizes(MM_TEXT, sizeGraph);
	ReleaseDC(pDC);
	
	return;
}


///////////////////////////////////
// OnView ... menu message handlers
///////////////////////////////////
void COWGraphView::OnViewAsLineGraph()
{          
	if(!(m_lgraphtype&GRAPH_LINE)) 
	{
		m_lgraphtype = m_lgraphtype|GRAPH_LINE;
		OnUpdateMenu();
		Invalidate();
	}
	return;
}

void COWGraphView::OnViewAsVBarGraph()
{          
	if(!(m_lgraphtype&GRAPH_VBAR)) 
	{
		m_lgraphtype = m_lgraphtype|GRAPH_VBAR;
		OnUpdateMenu();
		Invalidate();
	}
	return;
}

void COWGraphView::OnViewAsHBarGraph()
{          
	if(!(m_lgraphtype&GRAPH_HBAR)) 
	{
		m_lgraphtype = m_lgraphtype|GRAPH_HBAR;
		OnUpdateMenu();
		Invalidate();
	}
	return;
}

void COWGraphView::OnViewFitAll()
{
	if(!m_bFitDataIn) 
	{
		m_bFitDataIn = TRUE;
		OnUpdateMenu();
		Invalidate();
	}
	return;
}

void COWGraphView::OnOptionAutoRescaling()
{    
	COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument();
	
	if(pDoc->m_autorescaling) 
	{   
		//disable
		pDoc->m_autorescaling = FALSE;  
	}	
	else 
	{   
		//reset back default TRUE
		pDoc->m_autorescaling = TRUE;
		//disabling autorescaling causes m_maxnumberofpointsperset not to be maintained, therefore
		//we must set it back to its proper value.
		int iactualmax = pDoc->GetXMax();  
		if(pDoc->m_maxnumberofpointsperset != iactualmax)
		{   
			pDoc->m_maxnumberofpointsperset = iactualmax;
			pDoc->UpdateAllViews(NULL);
		}	
	}	
	OnUpdateMenu();
	return;
}

void COWGraphView::OnUpdateOptionAutoRescaling(CCmdUI* pCmdUI)
{   
	COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument();
	
	if(pDoc->m_autorescaling) pCmdUI->SetCheck(TRUE);  
	  else pCmdUI->SetCheck(FALSE);  
	return;
}


void COWGraphView::OnViewFitOneValuePerPixel()
{
	if(m_bFitDataIn) 
	{
		m_bFitDataIn = FALSE;  
		OnUpdateMenu();
		Invalidate();
	}
	return;
}

void COWGraphView::OnUpdateViewFitOneValuePerPixel(CCmdUI* pCmdUI)
{   
	if(m_lgraphtype&GRAPH_LINE) pCmdUI->Enable(FALSE); 
	  else pCmdUI->Enable(TRUE);
	return;
}


void COWGraphView::OnViewStatusBar()
{   
	//get this child frame window                                   
    COWGraphMDIChildWnd* pThisMDIChildWnd = (COWGraphMDIChildWnd*) this->GetParentFrame(); 
    ASSERT(pThisMDIChildWnd);
    
	//get this child frame window's status bar
	CStatusBar* pStatusBar = (CStatusBar*) AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_GRAPHVIEW_STATUSBAR);
	ASSERT(pStatusBar);

	if(m_bStatusBarVisible)
	{
		m_bStatusBarVisible = FALSE;
    	pStatusBar->ShowWindow(SW_HIDE); 
    	pThisMDIChildWnd->RecalcLayout();
		Invalidate();
	}                
	else
	{
		m_bStatusBarVisible = TRUE;
    	pStatusBar->ShowWindow(SW_SHOWNORMAL);
    	pThisMDIChildWnd->RecalcLayout();
		Invalidate();
	}
}
  
  
void COWGraphView::OnUpdateViewStatusBar(CCmdUI* pCmdUI)
{   
	if(m_bStatusBarVisible) pCmdUI->SetCheck(TRUE); 
	  else pCmdUI->SetCheck(FALSE);
	return;
}
  
  
#ifdef OWGRAPH_INTERNAL	//when OWGRAPH linked with the application
	#ifdef OIIIAPP //and when the app is OIII Application
	#include "oifii_app.h"
	#endif //OIIIAPP
#endif //OWGRAPH_INTERNAL
void COWGraphView::OnViewGraphData()
{   
	//////////////////////////////////////////////////////
	// create or activate the graph data childwindow frame  
	//////////////////////////////////////////////////////
	
	//1) get this view's document
	COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument(); 
	ASSERT(pDoc != NULL); 
	
	//2) get graphdata view and activate if existing
	CView* pView;
	POSITION pos = pDoc->GetFirstViewPosition();
	while(pos)
	{
		pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(COWGraphDataView))) 
		{   
			//graphdata view is already existing, activate it and return
			pView->GetParentFrame()->ActivateFrame();
			return;
		}
	}
	
	//3) since graphdata view does not exist, create and activate it
#ifdef OWGRAPH_INTERNAL	//when OWGRAPH linked with the application
	#ifdef OIIIAPP //and when the app is OIII Application
	CMDIChildWnd* pNewFrame = (CMDIChildWnd*) (((COIIIApp*)AfxGetApp())->m_pGraphDataViewTemplate->CreateNewFrame(pDoc, NULL));
	if(pNewFrame == NULL) return; //not created
	ASSERT(pNewFrame->IsKindOf(RUNTIME_CLASS(COWGraphDataMDIChildWnd)));
	((COIIIApp*)AfxGetApp())->m_pGraphDataViewTemplate->InitialUpdateFrame(pNewFrame, pDoc);
	#else
	ASSERT(FALSE); //development-time error, should not use this function is not in OIII app.
	#endif //OIIIAPP
#endif //OWGRAPH_INTERNAL

	return;
}

void COWGraphView::OnUpdateMenu()
{   
	CMenu* p_Menu = AfxGetApp()->m_pMainWnd->GetMenu(); 
	ASSERT(p_Menu);
	
	//uncheck all zoom item
	p_Menu->CheckMenuItem(ID_VIEW_ZOOM_IN_11, MF_BYCOMMAND | MF_UNCHECKED);
	p_Menu->CheckMenuItem(ID_VIEW_ZOOM_IN_21, MF_BYCOMMAND | MF_UNCHECKED);
	p_Menu->CheckMenuItem(ID_VIEW_ZOOM_IN_31, MF_BYCOMMAND | MF_UNCHECKED);
	p_Menu->CheckMenuItem(ID_VIEW_ZOOM_IN_41, MF_BYCOMMAND | MF_UNCHECKED);
	p_Menu->CheckMenuItem(ID_VIEW_ZOOM_IN_51, MF_BYCOMMAND | MF_UNCHECKED);
	p_Menu->CheckMenuItem(ID_VIEW_ZOOM_IN_61, MF_BYCOMMAND | MF_UNCHECKED);
	p_Menu->CheckMenuItem(ID_VIEW_ZOOM_IN_71, MF_BYCOMMAND | MF_UNCHECKED);
	p_Menu->CheckMenuItem(ID_VIEW_ZOOM_IN_81, MF_BYCOMMAND | MF_UNCHECKED);
	//check the proper zoom item
	UINT nID = ID_VIEW_ZOOM_IN_11 + (UINT)m_zoomfactor - 1;
	p_Menu->CheckMenuItem(nID, MF_BYCOMMAND | MF_CHECKED);

	if(m_lgraphtype&GRAPH_LINE) 
	{
		p_Menu->CheckMenuItem(ID_VIEW_AS_LINEGRAPH, MF_BYCOMMAND | MF_CHECKED);                         
		p_Menu->CheckMenuItem(ID_VIEW_AS_VBARGRAPH, MF_BYCOMMAND | MF_UNCHECKED);
		
		m_bFitDataIn = TRUE; 
		p_Menu->CheckMenuItem(ID_VIEW_FIT_ALL, MF_BYCOMMAND | MF_CHECKED);
		p_Menu->CheckMenuItem(ID_VIEW_FIT_ONEVALUEPERPIXEL, MF_BYCOMMAND | MF_UNCHECKED);
		p_Menu->EnableMenuItem(ID_VIEW_FIT_ONEVALUEPERPIXEL, MF_BYCOMMAND | MF_GRAYED);
	}	
	else 
	{
		p_Menu->CheckMenuItem(ID_VIEW_AS_VBARGRAPH, MF_BYCOMMAND | MF_CHECKED);                         
		p_Menu->CheckMenuItem(ID_VIEW_AS_LINEGRAPH, MF_BYCOMMAND | MF_UNCHECKED);   
		
		p_Menu->EnableMenuItem(ID_VIEW_FIT_ONEVALUEPERPIXEL, MF_BYCOMMAND | MF_ENABLED); 
		if(m_bFitDataIn) 
		{
			p_Menu->CheckMenuItem(ID_VIEW_FIT_ALL, MF_BYCOMMAND | MF_CHECKED);                         
			p_Menu->CheckMenuItem(ID_VIEW_FIT_ONEVALUEPERPIXEL, MF_BYCOMMAND | MF_UNCHECKED);
                         
		}	
		else 
		{
			p_Menu->CheckMenuItem(ID_VIEW_FIT_ONEVALUEPERPIXEL, MF_BYCOMMAND | MF_CHECKED); 
			p_Menu->CheckMenuItem(ID_VIEW_FIT_ALL, MF_BYCOMMAND | MF_UNCHECKED);                         
		}		
	}
	DrawMenuBar();	
	return;
}
     
          

///////////////////////////////////////////////////
// member functions to construct the graphic layout                           
///////////////////////////////////////////////////
void COWGraphView::OnPrepareDC(CDC* , CPrintInfo* )
{    
	return;
}  
         
void COWGraphView::OnUpdate(CView* , LPARAM , CObject* )
{	
	/*
	COWGraphDoc* pDoc = (COWGraphDoc*)GetDocument();
	if(pDoc->m_maxnumberofpointsperset==0)
	{
		//graph data has not been entered yet
		return;
	}

	if(m_lgraphtype&GRAPH_VBAR || m_lgraphtype&GRAPH_HBAR)
	{
		BOOL bVert = TRUE;
		if(m_lgraphtype&GRAPH_VBAR) bVert = TRUE;
		  else if(m_lgraphtype&GRAPH_HBAR) bVert = FALSE;
		struct tagGRAPHRENDERINGATTRIBUTES myRenderingAttrib;
		CDC* pDC = GetDC();
		pDC->AssertValid();
		if(pDC!=NULL)
		{
			myRenderingAttrib = ComputeBarGraphRenderingAtributes(pDC, bVert);
			if(myRenderingAttrib.bSkipDrawing == TRUE)
			{
				//do not erase previous graph rendering
				//Invalidate(FALSE);
			}
			else
			{
				Invalidate();
			}
			ReleaseDC(pDC);
		}
	}
	*/

	Invalidate(); //force full redrawing

	return;
}

CRect COWGraphView::GetDisplayRect(CDC* pDC)
{
	CRect rect;
      
    //when the DC is used for printing, this function should return a CRect according
    //with the printer resolution. 
     
	if (pDC->IsPrinting()) 
	{
		rect.left = rect.top = 0;
		rect.right  = pDC->GetDeviceCaps(HORZRES);
		rect.bottom = pDC->GetDeviceCaps(VERTRES);
		
		//usualy, getting HORZRES and VERTRES would be enough, of course draw would have
		//a different proportion than what is displayed on screen.
		//
		//if we wish to fit in the drawing with proportion defined by Client Window Rect,
		//we can had these few lines.
		CRect rect1;
		GetClientRect(&rect1);
		float fprop = (float)rect1.Width()/(float)rect1.Height();
		if(rect.right<rect.bottom)
		{
			//page is vertical
			rect.bottom = (int) (rect.right/fprop);
		}
		else
		{                     
			//page is horizontal
			rect.right = (int) (fprop * rect.bottom );
		}		
	}
	else
	{    
		//Most common case, when drawing is displayed to screen inside a mdi child window
		GetClientRect(&rect);
	} 
	                                                                     
	return rect;
}



void COWGraphView::OnDraw(CDC* pDC)
{   
	//get pointer to COWGraphDoc
	COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument();
	m_lgraphtype = pDoc->m_lgraphtype; //GRAPH_LINE; //GRAPH_VBAR  //GRAPH_LINE
	m_lGrid = pDoc->m_lviewgrid;

	//set background and text color
	//TRACE("view's bkcolor is 0x%x, view's txt color is 0x%x\n", m_crBackgroundColor, m_crTextColor );
	COLORREF crOldBackgroundColor = pDC->SetBkColor( m_crBackgroundColor );
	COLORREF crOldTextColor = pDC->SetTextColor( m_crTextColor );

	//create font with proper size
	CFont myfont; 
	CFont* p_oldfont = NULL; 
	
	CSize sizeText = pDC->GetTextExtent(L"0", 1);
	 
	//assuming MM_TEXT mapmode  
	if(myfont.CreateFont(sizeText.cy, 0, 0, 0, 0, 0, 0, 0, 0, OUT_TT_PRECIS, 
	   					 CLIP_DEFAULT_PRECIS, PROOF_QUALITY, TMPF_TRUETYPE, L"Arial"))
	{
		//if no error, select this font
		p_oldfont = pDC->SelectObject(&myfont);
	}

	
	if(pDoc->GetNumberOfVisibleSets() > 0)
	{   
		/*
		switch (m_lgraphtype)
		{		
			case GRAPH_SCATTER: 
				if (pDoc->m_maxnumberofpointsperset > 1)
					PlotScatterGraph(pDC);
				else 
					//not enough point in the graph, minimum 2 points required
					pDC->TextOut(10, 10, "Not Enough Points in the Graph, use View: As Bar");
				break;

			case GRAPH_LINE: 
				if (pDoc->m_maxnumberofpointsperset > 1)
					PlotLineGraph(pDC);
				else 
					//not enough point in the graph, minimum 2 points required
					pDC->TextOut(10, 10, "Not Enough Points in the Graph, use View: As Bar");
				break;
	
			case GRAPH_VBAR: 
			    if(pDoc->m_maxnumberofpointsperset > 1)
					PlotBarGraph(pDC, TRUE);
				break;

			case GRAPH_VBAR_PILED: 
			    if(pDoc->m_maxnumberofpointsperset > 1)
					PlotBarGraph(pDC, TRUE);
				break;

			case GRAPH_VBAR_PILED_AND_FIRSTCHANNEL_TOPVBARONLY: 
			    if(pDoc->m_maxnumberofpointsperset > 1)
					PlotBarGraph(pDC, TRUE);
				break;

			case GRAPH_HBAR: 
				//not fully supported yet, this case should be revised by a programmer
				PlotBarGraph(pDC, FALSE);
				break;				
		}
		*/
		if(m_lgraphtype&GRAPH_SCATTER)
		{	
			PlotScatterGraph(pDC);
			/*
			if (pDoc->m_maxnumberofpointsperset > 1)
				PlotScatterGraph(pDC);
			else 
				//not enough point in the graph, minimum 2 points required
				pDC->TextOut(10, 10, "Not Enough Points in the Graph, use View: As Bar");
			*/
		}
		else if(m_lgraphtype&GRAPH_LINE)
		{
			if (pDoc->m_maxnumberofpointsperset > 1)
				PlotLineGraph(pDC);
			else 
				//not enough point in the graph, minimum 2 points required
				pDC->TextOut(10, 10, "Not Enough Points in the Graph, use View: As Bar");
		}
		else if(m_lgraphtype&GRAPH_VBAR) 
		{
			if(pDoc->m_maxnumberofpointsperset > 1)
				PlotBarGraph(pDC, TRUE);
		}
		else if(m_lgraphtype&GRAPH_HBAR)
		{
			//not fully supported yet, this case should be revised by a programmer
			if(pDoc->m_maxnumberofpointsperset > 1)
				PlotBarGraph(pDC, FALSE);
		}
	    if( (pDC->IsPrinting()) || (pDC->IsKindOf(RUNTIME_CLASS(CMetaFileDC))) )
		{
			//force a repaint, when this paint will have been fully processed.  this has been added
			//because the mouse tracking was not working after a print or copy to clipboard.  
			//mouse tracking works with m_sizeGraph and this variable is MMHIMETRIC_TO_MMTEXT_SCALEFACTOR times too big after a
			//copy to clipboard (because it draws in MM_HIMETRIC units)
			Invalidate();  
		}
	
		
		
	}
	//reset drawing flag
	m_bUpdate = FALSE;
	//reset CDC drawing attributes
	if(p_oldfont) pDC->SelectObject(p_oldfont);
	pDC->SetBkColor( crOldBackgroundColor );
	pDC->SetTextColor( crOldTextColor );	
	return;
}


///////////////////////////////////////////////////////////////////////////////
// determine the graduation of the axis according to the maximum value
// to be displayed divided by the number of division (or ticks) requested.
// it will return the closest integer value (looking in the array
// of all possible graduation values nValidGraduations[], that can
// be specified at compile-time, in the current implementation).
///////////////////////////////////////////////////////////////////////////////
int COWGraphView::GetTickValue(int nMaxValue, int nNumDiv)
{
	// Determine values for vertical ticks
	int nValuePerTick = (nMaxValue + nNumDiv - 1) / nNumDiv;
	for (int i = 0; nValidGraduations[i] > 0; i++)
	{
		if (nValidGraduations[i] >= nValuePerTick)
		{   
			//return the valid graduation that is just bigger than nValuePerTick
			return nValidGraduations[i];
		}
	} 
	//if none of the valid graduation bigger than nValuePerTick, return nValuePerTick
	return nValuePerTick;
}
 
 
///////////////////////////////////////////////////////////////////////////////
// GetGraphMetrics determines several things:
// Returns Character Height
// &sizeGraph contains overall dimensions of the graph (may not be window size)
// &sizePixelsPerTick contains pixels per tick in both directions
// &ptOrigin contains the origin point
//
//  Parameters:
//  pDC   - DC for display device
//  nVDiv - Number of vertical Divisions
//  nHDiv - Number of horizontal Divisions
///////////////////////////////////////////////////////////////////////////////
int COWGraphView::GetGraphMetrics(CDC* pDC, 
								int nHDiv, 
								int nVDiv,
								CSize& sizeGraph, 
								CSize& sizePixelsPerTick, 
								CPoint& ptOrigin)
{        
	m_sizeText = pDC->GetTextExtent(L"0", 1);
	sizeGraph = m_sizeText;

                                     
    //                                 
	// 1) Determine minimum size for graph
	//

	// Minimum size of a Division == GRAPH_MINXDIVISIONSIZE_INCHARWIDTH chars times the number of division
	// Allow 5 chars for Vertical Captions and 2.5 chars on each side of Graph
	sizeGraph.cx *= (int)((GRAPH_MINXDIVISIONSIZE_INCHARWIDTH * nHDiv) + GRAPH_TOTALXMARGINSIZE_INCHARWIDTH);
	sizeGraph.cy *= (int)(nVDiv + GRAPH_TOTALYMARGINSIZE_INCHARHEIGHT);
     
    //       
	// 2) Grow size if permitted by display area
	//
	CRect rect = GetDisplayRect(pDC);
	if (rect.Width() > sizeGraph.cx)
		sizeGraph.cx = rect.Width();
	if (rect.Height() > sizeGraph.cy)
		sizeGraph.cy = rect.Height();


    //
    // 3) if BAR graph, sizeGraph is restricted to 
    //    some discrete multiple of numvalues*numvisiblegraphsets
    //
	COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument();
    if(	m_lgraphtype&GRAPH_VBAR || m_lgraphtype&GRAPH_HBAR)
    {   
    	//reset variable                                               
		m_numberofvalueperpixel = 0;
		
		
    	//do not include margins
    	sizeGraph.cx = sizeGraph.cx - GRAPH_TOTALXMARGINSIZE_INCHARWIDTH * m_sizeText.cx;
    	
	    //compute the minimum lenght in pixel of the bar graph
	    int mingraphsize = (pDoc->GetNumberOfVisibleSets() * pDoc->m_maxnumberofpointsperset);
		if(	m_lgraphtype&GRAPH_PILED)
		{
			mingraphsize = pDoc->m_maxnumberofpointsperset;
		}
	    int newgraphsize = mingraphsize;
	    if(sizeGraph.cx > mingraphsize ) 
	    {   
			ASSERT(mingraphsize!=0);
	    	//if bar graph must be strech, set the new size to a multiple
	    	while(sizeGraph.cx > newgraphsize) newgraphsize = newgraphsize+mingraphsize;	
	    	sizeGraph.cx = (int) (newgraphsize-mingraphsize);	    	
		    //include the margins back
		    sizeGraph.cx = sizeGraph.cx + GRAPH_TOTALXMARGINSIZE_INCHARWIDTH * m_sizeText.cx;
	    }
	    else if( sizeGraph.cx<=mingraphsize )
	    {    
	    	//depending on the display mode
			if( m_bFitDataIn ) //SPoirier, removed //&& !(pDC->IsPrinting()) 
			{                                                          
				//force graphic output to fit values in (average or skip values) 
				int i=1;         
				newgraphsize = mingraphsize; //at start mingraphsize is 1 pixel/bar
				while(sizeGraph.cx < newgraphsize)
				{
					i++;
					newgraphsize = mingraphsize/i;
				}                       
				float ratio =  (float)mingraphsize/(float)i;
				float remainder = ratio - (float)newgraphsize; 
				m_numberofvalueperpixel = i;
				if(remainder>0.0000001) 
				{
					sizeGraph.cx = (int)newgraphsize+1; //one extra pixel 
					m_nremainder = (int) ((float)i*remainder); 
				}	
				else 
				{
					sizeGraph.cx = (int)newgraphsize;
					m_nremainder = 0;
				}	
				 
			    //include the margins back
			    sizeGraph.cx = sizeGraph.cx + GRAPH_TOTALXMARGINSIZE_INCHARWIDTH * m_sizeText.cx;
			}
			else
			{     
				m_numberofvalueperpixel = 0;
		    	//set sizeGraph to minimum required size so that each bar has 1 pixel width
		    	sizeGraph.cx = (int)mingraphsize; 
			    //include the margins back
			    sizeGraph.cx = sizeGraph.cx + GRAPH_TOTALXMARGINSIZE_INCHARWIDTH * m_sizeText.cx;
		    }
	    }
	 	sizePixelsPerTick.cx = (int) (m_nXValuePerTick * (sizeGraph.cx - GRAPH_TOTALXMARGINSIZE_INCHARWIDTH * m_sizeText.cx) / pDoc->m_maxnumberofpointsperset);
		sizePixelsPerTick.cy = (int) ((sizeGraph.cy -  GRAPH_TOTALYMARGINSIZE_INCHARHEIGHT * m_sizeText.cy) / nVDiv);
    }//end of if(lgraphtype == GRAPH_BAR)
    else if(m_lgraphtype&GRAPH_LINE || m_lgraphtype&GRAPH_SCATTER)
    {
	 	sizePixelsPerTick.cx = (int) ((sizeGraph.cx - GRAPH_TOTALXMARGINSIZE_INCHARWIDTH * m_sizeText.cx) / nHDiv);
		sizePixelsPerTick.cy = (int) ((sizeGraph.cy -  GRAPH_TOTALYMARGINSIZE_INCHARHEIGHT * m_sizeText.cy) / nVDiv);
    }  
    
	ptOrigin.x = (int) (m_sizeText.cx * GRAPH_MINXDIVISIONSIZE_INCHARWIDTH * 3 /2 ); 
	ptOrigin.y = (int) (sizePixelsPerTick.cy * nVDiv + m_sizeText.cy * GRAPH_TOTALYMARGINSIZE_INCHARHEIGHT / 2);
		
	return (int)m_sizeText.cy;
}
 
         
         
//////////////////////////////////////////////////////////////////
// This function writes alpha or numeric label for each axis ticks
//////////////////////////////////////////////////////////////////
void COWGraphView::PlotCaptions(CDC* pDC, 
							  CPoint ptOrigin, 
							  CSize sizePixelsPerTick, 
							  int nValue, 
							  int nValuePerTick, 
							  int nNumTicks, 
							  int nCharHeight, 
							  BOOL bVert, 
							  BOOL bAlpha)
{
	char buf[80];

	int nTickSize = (bVert ? sizePixelsPerTick.cx : sizePixelsPerTick.cy) / GRAPH_DIVISIONSIZE_BY_TICKSIZE_RATIO;
	if (nTickSize < 2)
		nTickSize = 2;

	int nPos  = bVert ? ptOrigin.y : ptOrigin.x;
	int nStep = bVert ? -sizePixelsPerTick.cy : sizePixelsPerTick.cx;

	pDC->SetTextAlign(bVert ? TA_RIGHT : TA_CENTER);

	for (int i = 0; i < nNumTicks; i++)
	{
		if (bAlpha)
			wsprintfA(buf, "%c", nValue); //alpha values like A, B, C, ...
		else
		{
			COWGraphDoc* pDoc = (COWGraphDoc*)GetDocument();
			//for scatter graph
			if(m_lgraphtype&GRAPH_SCATTER)
			{
				//map integer values to float according to user specify dfMin and dfMax values
				if(bVert)
				{
					float fValue = (float) ((float)nValue + (float)m_SCATTERdfMinValueY);
					sprintf(buf, "%.2f", fValue); //float
				}
				else
				{
					float fValue = (float) ((float)nValue + (float)m_SCATTERdfMinValueX);
					sprintf(buf, "%.2f", fValue); //float
				}
			}
			//for histogram
			else if(!bVert && pDoc->m_bMapIndexToFloatDISPLAYFLOAT)
			{
				//along horizontal axis only
				//map integer values to float according to user specify dfMin and dfMax values
				float fValue = (float)((float)nValue) * 
					((float)pDoc->m_dfMapIndexToFloatMAXFLOAT 
					- (float)pDoc->m_dfMapIndexToFloatMINFLOAT)/(float)pDoc->m_iMapIndexToFloatNUMBEROFLEVEL
					+ (float)pDoc->m_dfMapIndexToFloatMINFLOAT;
				sprintf(buf, "%.0f", fValue+0.5); //float
			}
			else
			{
				//along vertical axis
				wsprintfA(buf, "%ld", nValue);//integers 
			}
		}


		if (bVert)
		{
			pDC->TextOut((int)(ptOrigin.x - 2 * nTickSize),
						 (int)(nPos - nCharHeight / 2), buf);
		}								
		else
		{
		    pDC->TextOut((int)nPos, (int)(ptOrigin.y + nCharHeight / 2), buf);
		}	

		nValue += nValuePerTick;
		nPos   += nStep;
	}
	return;
}

/////////////////////
// Plot Axes and Grid
/////////////////////
void COWGraphView::PlotAxes(CDC* pDC, 
						  CPoint ptOrigin, 
						  CSize sizePixelsPerTick,
						  int nHorzTicks, 
						  int nVertTicks, 
						  int nDrawTicks)
{
	int nHeight = sizePixelsPerTick.cy * nVertTicks;
	int nWidth  = sizePixelsPerTick.cx * nHorzTicks;
    
    //use the text pen color for grid. previous version://use a black dotted Pen for the grid 
    CPen* pOldPen = NULL;
    CPen* pGridPen = new(CPen);
    pGridPen->CreatePen(PS_DOT, 1, m_crTextColor); //was RGB(0,0,0)
                                         
    //use default black solid pen for the axis and ticks
    
                                         
    
	// Draw Axes
	pDC->MoveTo(ptOrigin);
	pDC->LineTo(ptOrigin.x, ptOrigin.y - (int)nHeight);
	pDC->MoveTo(ptOrigin);
	pDC->LineTo(ptOrigin.x + (int)nWidth, ptOrigin.y);

	if (nDrawTicks & TICKS_VERT)
	{
		// Draw Vertical Ticks
		int nTickSize = (sizePixelsPerTick.cx / GRAPH_DIVISIONSIZE_BY_TICKSIZE_RATIO);
		if (nTickSize < 2)
			nTickSize = 2;

		int nVPos = ptOrigin.y;
		int xLeft  = ptOrigin.x - nTickSize;
		int xRight = ptOrigin.x + nTickSize + 1;

		for (int i = 0; i <= nVertTicks; i++)
		{
			pDC->MoveTo((int)xLeft, (int)nVPos);
			pDC->LineTo((int)xRight, (int)nVPos); 
			// Draw Horizontal Grid
			if(m_lGrid & GRID_HORZ)
			{                  
				pOldPen = pDC->SelectObject(pGridPen); 
				ASSERT(pOldPen);
				pDC->MoveTo((int)xRight, (int)nVPos);
				pDC->LineTo(ptOrigin.x+(int)nWidth, (int)nVPos);
				pDC->SelectObject(pOldPen);
			}
			nVPos -= (int)sizePixelsPerTick.cy;
		}
	}

	if (nDrawTicks & TICKS_HORZ)
	{
		// Draw Horizontal Ticks
		int nTickSize = (sizePixelsPerTick.cy / GRAPH_DIVISIONSIZE_BY_TICKSIZE_RATIO);
		if (nTickSize < 2)
			nTickSize = 2;

		int nHPos = ptOrigin.x;
		int yTop    = ptOrigin.y - nTickSize;
		int yBottom = ptOrigin.y + nTickSize + 1;

		for (int i = 0; i <= nHorzTicks; i++)
		{
			pDC->MoveTo((int)nHPos, (int)yTop);
			pDC->LineTo((int)nHPos, (int)yBottom); 
			// Draw Vertical Grid
			if(m_lGrid & GRID_VERT)
			{                  
				pOldPen = pDC->SelectObject(pGridPen); 
				ASSERT(pOldPen);
				pDC->MoveTo((int)nHPos, ptOrigin.y - (int)nTickSize);
				pDC->LineTo((int)nHPos, ptOrigin.y - (int)nHeight);
				pDC->SelectObject(pOldPen);
			}
			nHPos += sizePixelsPerTick.cx;
		}
	}
	delete(pGridPen);
	return;
}


/////////////////////
// Plot Scatter Graph
/////////////////////	                         
void COWGraphView::PlotScatterGraph(CDC* pDC)
{    
    COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument();
	int iNumChannel = pDoc->m_GraphSetList.GetSize();  
	ASSERT(iNumChannel==2);

	/*
	int nMaxValue = (int) (pDoc->GetYMax() + 0.5f);
    if(pDoc->m_autorescaling) pDoc->m_oldymax = nMaxValue; 
      else nMaxValue = pDoc->m_oldymax;
	//the member variable m_zoomfactor
	
	if( !pDC->IsKindOf(RUNTIME_CLASS(CMetaFileDC)) )
	{    
		ASSERT(m_zoomfactor!=0L);
		nMaxValue = nMaxValue/m_zoomfactor;
	}	
	*/

	//for each COWGraphSet in the m_GraphSetList (2 when scatter)
	COWGraphSet* p_GraphSetX = (COWGraphSet*) pDoc->m_GraphSetList.GetAt(0);
	ASSERT(p_GraphSetX->IsVisible());
	if(pDoc->m_autorescaling)
	{
		m_SCATTERdfMinValueX = p_GraphSetX->GetYMin();
		m_SCATTERdfMaxValueX = p_GraphSetX->GetYMax();
	}
	else
	{
		m_SCATTERdfMinValueX = pDoc->m_PreFixed_SCATTERdfMinValueX;
		m_SCATTERdfMaxValueX = pDoc->m_PreFixed_SCATTERdfMaxValueX;
	}
	COWGraphSet* p_GraphSetY = (COWGraphSet*) pDoc->m_GraphSetList.GetAt(1);
	ASSERT(p_GraphSetY->IsVisible());
	if(pDoc->m_autorescaling)
	{
		m_SCATTERdfMinValueY = p_GraphSetY->GetYMin();
		m_SCATTERdfMaxValueY = p_GraphSetY->GetYMax();
	}
	else
	{
		m_SCATTERdfMinValueY = pDoc->m_PreFixed_SCATTERdfMinValueY;
		m_SCATTERdfMaxValueY = pDoc->m_PreFixed_SCATTERdfMaxValueY;
	}
	ASSERT(p_GraphSetX->GetDataSize() == p_GraphSetY->GetDataSize());
	ASSERT(p_GraphSetX->GetDataType() == p_GraphSetY->GetDataType());

	int nHorzDiv = GRAPH_NUMBEROFHORZDIV;
	int nVertDiv = GRAPH_NUMBEROFVERTDIV; 
	
	m_SCATTERdfRangeX = m_SCATTERdfMaxValueX - m_SCATTERdfMinValueX;
	m_SCATTERdfRangeY = m_SCATTERdfMaxValueY - m_SCATTERdfMinValueY;
	//m_nXValuePerTick = GetTickValue(((int)(dfMaxValueX+0.5)), nHorzDiv);
	//m_nYValuePerTick = GetTickValue(((int)(dfMaxValueY+0.5)), nVertDiv);
	m_nXValuePerTick = GetTickValue(((int)(m_SCATTERdfRangeX+0.5)), nHorzDiv);
	m_nYValuePerTick = GetTickValue(((int)(m_SCATTERdfRangeY+0.5)), nVertDiv);
	 
	CSize sizeGraph;
	CSize sizePixelsPerTick;    
	CPoint ptOrigin;
	
	int nCharHeight = GetGraphMetrics(pDC, nHorzDiv, nVertDiv,
								sizeGraph, sizePixelsPerTick, ptOrigin);
	CScrollView::OnPrepareDC(pDC, NULL); 

    int nHorzTicks = nHorzDiv;
    int nVertTicks = nVertDiv;
	PlotAxes(pDC, ptOrigin, sizePixelsPerTick, nHorzTicks, nVertTicks, TICKS_BOTH);

	PlotCaptions(pDC, ptOrigin, sizePixelsPerTick, 0, m_nYValuePerTick,
						nVertTicks+1, nCharHeight, TRUE, FALSE);

	PlotCaptions(pDC, ptOrigin, sizePixelsPerTick, 0, m_nXValuePerTick,
						nHorzTicks+1, nCharHeight, FALSE, FALSE);
	
                            
    ////////////////////////////////////////////
    //Plot data from the COWGraphDoc::m_GraphSetList
    ////////////////////////////////////////////                        
	COWGraphSet* p_currentGraphSet = NULL; 
	CPen* pOldPen = NULL;
	CPen* pPen = NULL;
	
	COLORREF crColor = GetGraphSetColor(p_GraphSetX, 1);
	pPen = (CPen*) new(CPen);		 
    if(pPen->CreatePen(PS_SOLID, 1, crColor)==0) 
	{
		//ERR_HANDLER_SHOWWARNING( IDS_ERR_CREATE_GDIOBJ );
		AfxMessageBox(L"Not enough GDI memory available, free other applications");
	}
	pOldPen = pDC->SelectObject(pPen);
	
	int iOldBkMode = pDC->SetBkMode(TRANSPARENT);

	/*
	CBrush myBrush;
	myBrush.CreateSolidBrush(RGB(255,0,0));
	CBrush* pOldBrush = NULL;
	pOldBrush = pDC->SelectObject(&myBrush);
	*/
	//no brush
    CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);

	int iPtSizeX = (int)((float)sizePixelsPerTick.cx/15.0f+0.5f);
	int iPtSizeY = (int)((float)sizePixelsPerTick.cy/15.0f+0.5f);
	for(int i=0; i<p_GraphSetX->GetDataSize(); i++)
	{					      
		int nVPos = (int)(((p_GraphSetY->GetValueAsFloat(i)-m_SCATTERdfMinValueY) * 
							(float)sizePixelsPerTick.cy)/(float)m_nYValuePerTick); 
		int nHPos = (int)(2.0 + ((p_GraphSetX->GetValueAsFloat(i)-m_SCATTERdfMinValueX) 
					* (float)sizePixelsPerTick.cx) / (float)m_nXValuePerTick);
		/*				   
		pDC->Ellipse(ptOrigin.x + nHPos - iPtSizeX, ptOrigin.y - nVPos + iPtSizeY, 
				ptOrigin.x + nHPos + iPtSizeX, ptOrigin.y - nVPos - iPtSizeY);
		*/
		/* //each point as a cross
		pDC->MoveTo(ptOrigin.x + nHPos - iPtSizeX, ptOrigin.y - nVPos);
		pDC->LineTo(ptOrigin.x + nHPos + iPtSizeX, ptOrigin.y - nVPos);
		pDC->MoveTo(ptOrigin.x + nHPos, ptOrigin.y - nVPos - iPtSizeY);
		pDC->LineTo(ptOrigin.x + nHPos, ptOrigin.y - nVPos + iPtSizeY);
		*/
		//each point as a rectangle (faster drawing time than cross)
		CRect myRect(ptOrigin.x + nHPos - iPtSizeX, ptOrigin.y - nVPos - iPtSizeY,
					ptOrigin.x + nHPos + iPtSizeX, ptOrigin.y - nVPos + iPtSizeY);
		pDC->Rectangle(&myRect);
		
		/*
		//each point as filled rectangle
		CRect myRect(ptOrigin.x + nHPos - iPtSizeX, ptOrigin.y - nVPos - iPtSizeY,
					ptOrigin.x + nHPos + iPtSizeX, ptOrigin.y - nVPos + iPtSizeY);
		pDC->FillRect(&myRect, &myBrush);
		*/

	}
	/*
	if(pOldBrush) pDC->SelectObject(pOldBrush);
	*/
	pDC->SelectObject(pOldBrush);

	pDC->SetBkMode(iOldBkMode);

	pDC->SelectObject(pOldPen);
	delete(pPen);

	//
	//before returning, update the graph scale-related member variables
	m_ptGraphOrigin = ptOrigin;
	/*
	m_sizeGraph.cx = (int)((pDoc->m_maxnumberofpointsperset) * 
					 ((float)sizePixelsPerTick.cx/(float)m_nXValuePerTick));
	m_sizeGraph.cy = (int)(nMaxValue * 
					 ((float)sizePixelsPerTick.cy/(float)m_nYValuePerTick));
	*/
	m_sizeGraph.cx = (int)((pDoc->m_maxnumberofpointsperset) * 
					 ((float)sizePixelsPerTick.cx/(float)m_nXValuePerTick));
	m_sizeGraph.cy = (int)(m_SCATTERdfRangeY * 
					 ((float)sizePixelsPerTick.cy/(float)m_nYValuePerTick));
	m_sizePixelsPerTick = sizePixelsPerTick;
	m_nPrevVBarPos = GRAPH_BARPOS_INVISIBLE;
	m_bUpdate = FALSE;
}

	                         
//////////////////
// Plot Line Graph
//////////////////
void COWGraphView::PlotLineGraph(CDC* pDC)
{    
    COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument();

	//y scale
	int nMaxValue = (int) (pDoc->GetYMax() + 0.5f);
    if(pDoc->m_autorescaling==TRUE) 
	{
		//auto
		pDoc->m_oldymax = nMaxValue; 
	}
    else 
	{
		//fix
		nMaxValue = pDoc->m_oldymax;
	}


	//the member variable m_zoomfactor
	if( !pDC->IsKindOf(RUNTIME_CLASS(CMetaFileDC)) )
	{    
		ASSERT(m_zoomfactor!=0L);
		nMaxValue = nMaxValue/m_zoomfactor;
	}	
	
	int nHorzDiv = GRAPH_NUMBEROFHORZDIV;
	int nVertDiv = GRAPH_NUMBEROFVERTDIV; 
	
	m_nXValuePerTick = GetTickValue(pDoc->m_maxnumberofpointsperset, nHorzDiv);
	m_nYValuePerTick = GetTickValue(nMaxValue, nVertDiv);
	 
	CSize sizeGraph;
	CSize sizePixelsPerTick;    
	CPoint ptOrigin;
	
	int nCharHeight = GetGraphMetrics(pDC, nHorzDiv, nVertDiv,
								sizeGraph, sizePixelsPerTick, ptOrigin);
	CScrollView::OnPrepareDC(pDC, NULL); 

    int nHorzTicks = nHorzDiv;
    int nVertTicks = nVertDiv;
	PlotAxes(pDC, ptOrigin, sizePixelsPerTick, nHorzTicks, nVertTicks, TICKS_BOTH);

	PlotCaptions(pDC, ptOrigin, sizePixelsPerTick, 0, m_nYValuePerTick,
						nVertTicks+1, nCharHeight, TRUE, FALSE);

	PlotCaptions(pDC, ptOrigin, sizePixelsPerTick, 0, m_nXValuePerTick,
						nHorzTicks+1, nCharHeight, FALSE, FALSE);
	
                            
    ////////////////////////////////////////////
    //Plot data from the COWGraphDoc::m_GraphSetList
    ////////////////////////////////////////////                        
	COWGraphSet* p_currentGraphSet = NULL; 
	CPen* pOldPen = NULL;
	CPen* pPen = NULL;
	
    int nSet = 0;
	int indexend = pDoc->m_GraphSetList.GetSize();  
	for(int index=0; index<indexend; index++)
	{                                                
		//for each COWGraphSet in the m_GraphSetList
		p_currentGraphSet = (COWGraphSet*) pDoc->m_GraphSetList.GetAt((int)index);
		//check if this set is visible
		if(p_currentGraphSet->IsVisible())
		{          
			//visible set counter
			nSet++; 

			COLORREF crColor = GetGraphSetColor(p_currentGraphSet, nSet-1);
			pPen = (CPen*) new(CPen);		 
			if(pPen->CreatePen(PS_SOLID, 1, crColor)==0) 
			{
				//ERR_HANDLER_SHOWWARNING( IDS_ERR_CREATE_GDIOBJ );
				AfxMessageBox(L"Not enough GDI memory available, free other applications");
			}
			pOldPen = pDC->SelectObject(pPen);

			//optimize depending if int or float array
			if(p_currentGraphSet->GetDataType() == LONGTYPE)
			{	
				for(int i=0; i<p_currentGraphSet->GetDataSize(); i++)
				{					      
					int nVPos = ((p_currentGraphSet->GetValueAsLong(i) * 
								  (int)sizePixelsPerTick.cy) / m_nYValuePerTick); 
					//MulDiv does the same as above, faster for 16bit integer
					//multiplacation and division
					int nHPos = 2 + (i * (int)sizePixelsPerTick.cx) / m_nXValuePerTick;
									   
					if (i == 0) pDC->MoveTo(ptOrigin.x + (int)nHPos, ptOrigin.y - (int)nVPos);
					  else pDC->LineTo(ptOrigin.x + (int)nHPos, ptOrigin.y - (int)nVPos);					  
				}
			}
			else if(p_currentGraphSet->GetDataType() == FLOATTYPE
				|| p_currentGraphSet->GetDataType() == DOUBLETYPE)
			{    
				for(int i=0; i<p_currentGraphSet->GetDataSize(); i++)
				{					      
					int nVPos = (int)((p_currentGraphSet->GetValueAsFloat(i) * 
								 		(float)sizePixelsPerTick.cy)/
								 		(float)m_nYValuePerTick); 
					int nHPos = 2 + (i * (int)sizePixelsPerTick.cx) / m_nXValuePerTick;
									   
					if (i == 0) pDC->MoveTo(ptOrigin.x + (int)nHPos, ptOrigin.y - (int)nVPos);
					  else pDC->LineTo(ptOrigin.x + (int)nHPos, ptOrigin.y - (int)nVPos);					  
				}
			}
			pDC->SelectObject(pOldPen);
			delete(pPen);
		}//end of if(visible set)				    
	}//end of for(i=0 to end of m_GraphSetList)	
	// 
	//
	//before returning, update the graph scale-related member variables
	m_ptGraphOrigin = ptOrigin;
	//m_sizeGraph.cx = sizePixelsPerTick.cx*nHorzTicks;
	//m_sizeGraph.cy = sizePixelsPerTick.cy*nVertTicks;
	m_sizeGraph.cx = (int)((pDoc->m_maxnumberofpointsperset) * 
					 ((float)sizePixelsPerTick.cx/(float)m_nXValuePerTick));
	m_sizeGraph.cy = (int)(nMaxValue * 
					 ((float)sizePixelsPerTick.cy/(float)m_nYValuePerTick));
	m_sizePixelsPerTick = sizePixelsPerTick;
	m_nPrevVBarPos = GRAPH_BARPOS_INVISIBLE;
	m_bUpdate = FALSE;
}
     
                                                                        
/////////////////////////////////////////////////////////////////
// compute bar graph rendering attributes, like bar width, etc...
/////////////////////////////////////////////////////////////////
struct tagGRAPHRENDERINGATTRIBUTES COWGraphView::ComputeBarGraphRenderingAtributes(CDC* pDC, BOOL bVert)
{
	struct tagGRAPHRENDERINGATTRIBUTES myRenderingAttrib;
    COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument();

	//y scale
	int nMaxValue = (int) (pDoc->GetYMax() + 0.5f);
    if(pDoc->m_autorescaling) 
	{
		//auto
		pDoc->m_oldymax = nMaxValue; 
	}
    else 
	{
		//fix
		nMaxValue = pDoc->m_oldymax; 
	}

      
 	if( !pDC->IsKindOf(RUNTIME_CLASS(CMetaFileDC)) )
	{    
		ASSERT(m_zoomfactor!=0L);
		nMaxValue = nMaxValue/m_zoomfactor;
	}	
	int nHorzDiv = GRAPH_NUMBEROFHORZDIV;
	int nVertDiv = GRAPH_NUMBEROFVERTDIV;
	
	//the final number of values per tick is obtained by multiplying nValuePerTick * nvisibleset    
	m_nXValuePerTick = GetTickValue(pDoc->m_maxnumberofpointsperset, nHorzDiv);
	m_nYValuePerTick = GetTickValue(nMaxValue, nVertDiv);

	//if bVert is TRUE, Ticks = Div, otherwise swap them 
	int nVertTicks = bVert ? nVertDiv : nHorzDiv;	
	int nHorzTicks = bVert ? nHorzDiv : nVertDiv;
	
	CSize sizeGraph;
	CSize sizePixelsPerTick;
	CPoint ptOrigin;
	
	int nCharHeight = GetGraphMetrics(pDC, nHorzTicks, nVertTicks,
								sizeGraph, sizePixelsPerTick, ptOrigin);

    //////////////////////////////////////////////////////////   
	// now compute BarWidth, space between each BarSets, etc..
	//////////////////////////////////////////////////////////
	int nDataSetWidth = 0; //number of pixels per sets of values
	int nBarWidth     = 0; //number of pixels per Bar
	int nSpaceWidth   = 0; //number of pixels between sets of Bars
	
	//if(bVert) nDataSetWidth = sizePixelsPerTick.cx/nXValuePerTick;
	//  else nDataSetWidth = sizePixelsPerTick.cy/nXValuePerTick;
	if(bVert) nDataSetWidth = (int) ((sizeGraph.cx - GRAPH_TOTALXMARGINSIZE_INCHARWIDTH*m_sizeText.cx)/pDoc->m_maxnumberofpointsperset);
	  else nDataSetWidth = (int)sizeGraph.cy/nMaxValue;
	
	if(	m_lgraphtype&GRAPH_PILED)
	{
		if(nDataSetWidth<1)	nDataSetWidth = 1; //minsize is 1
		nBarWidth = nDataSetWidth;
		nSpaceWidth = 0;
	}
	else if( nDataSetWidth < pDoc->GetNumberOfVisibleSets() )
	{    
	    if(m_bFitDataIn)
	    {
			if(m_numberofvalueperpixel>1)
			{
				//if fit in all values, we will use same parameters as if exact fit but we 
				//will skip m_numberofvalueperpixel-1 value
	        	nBarWidth = 1; 
	        	nSpaceWidth = 0;
	        	nDataSetWidth = pDoc->GetNumberOfVisibleSets();
			} 
		}                    
		else
		{        
			//nBarWidth will be one pixel and scroll bar will be provided.
			//SPoirier dec 8, just added
			nBarWidth = 1; 
			nSpaceWidth = 0;
		}
	}   
	//else graphic output will have 1 pixel per value
	else if(nDataSetWidth == pDoc->GetNumberOfVisibleSets())
	{                                                 
		nBarWidth = 1;
		nSpaceWidth = 0;
	}
	else
	{                       
		//more than one pixel per value
		nBarWidth = nDataSetWidth / (pDoc->GetNumberOfVisibleSets());
		nSpaceWidth = 0;
    }

	myRenderingAttrib.ptOrigin = ptOrigin; 
	myRenderingAttrib.nMaxValue = nMaxValue; 
	myRenderingAttrib.sizePixelsPerTick = sizePixelsPerTick; 
	myRenderingAttrib.pDoc = pDoc; 
	myRenderingAttrib.nBarWidth = nBarWidth; 
	myRenderingAttrib.nHorzTicks = nHorzTicks; 
	myRenderingAttrib.nVertTicks = nVertTicks;
	myRenderingAttrib.nCharHeight = nCharHeight;
	myRenderingAttrib.nDataSetWidth	= nDataSetWidth;
	myRenderingAttrib.nSpaceWidth = nSpaceWidth;


	//this is an optimization to prevent flickering when redrawing graph with exact same attributes .
	//
    //if the bar graph's rendering attributes did not change
	//and if the window size was reduced, in x only, skip redrawing 
	//(however, if size in y changes, we must always redraw all)
	//
	//we can use the data members because, at this stage, they reflect the previous 
	//rendering attributes
	if(m_ptGraphOrigin.x == ptOrigin.x &&
	   m_ptGraphOrigin.y == ptOrigin.y &&
	   m_sizeGraph.cy == ((int)(nMaxValue * ((float)sizePixelsPerTick.cy/
						  (float)m_nYValuePerTick))) &&
	   m_sizeGraph.cx == ((int)((pDoc->m_maxnumberofpointsperset) * 
						 ((float)sizePixelsPerTick.cx/(float)m_nXValuePerTick))) &&
	   m_sizePixelsPerTick.cx == sizePixelsPerTick.cx &&
	   m_sizePixelsPerTick.cy == sizePixelsPerTick.cy &&
	   m_nBarWidth == nBarWidth
	  )

	{
		//skip drawing
		myRenderingAttrib.bSkipDrawing = TRUE;
	}
	else
	{
		myRenderingAttrib.bSkipDrawing = FALSE;
	}

	return myRenderingAttrib;
}

/////////////////
// Plot Bar Graph
/////////////////                         
void COWGraphView::PlotBarGraph(CDC* pDC, BOOL bVert)
{
	struct tagGRAPHRENDERINGATTRIBUTES myRenderingAttrib;
	myRenderingAttrib = ComputeBarGraphRenderingAtributes(pDC, bVert);
	CPoint ptOrigin = myRenderingAttrib.ptOrigin; 
	int nMaxValue = myRenderingAttrib.nMaxValue; 
	CSize sizePixelsPerTick = myRenderingAttrib.sizePixelsPerTick; 
	COWGraphDoc* pDoc = myRenderingAttrib.pDoc; 
	int nBarWidth = myRenderingAttrib.nBarWidth; 
	int nHorzTicks = myRenderingAttrib.nHorzTicks; 
	int nVertTicks = myRenderingAttrib.nVertTicks;
	int nCharHeight = myRenderingAttrib.nCharHeight;
	int nDataSetWidth = myRenderingAttrib.nDataSetWidth;
	int nSpaceWidth = myRenderingAttrib.nSpaceWidth;
	//note, down here we do not care about skip bSkipDrawing flag, we should always
	//      redraw the graph because at this stage the background has been already erased


    //////////////////////////////////////////////////////////////////////////////////
	// start drawing graph
    //////////////////////////////////////////////////////////////////////////////////
	CScrollView::OnPrepareDC(pDC, NULL); 
    
    ///////////////////////////////////
    //draw the x and y axis, with ticks
    ///////////////////////////////////
	PlotAxes(pDC, ptOrigin, sizePixelsPerTick, nHorzTicks, nVertTicks, 
							bVert ? TICKS_VERT : TICKS_HORZ);
                                                          
    ////////////////////
    //plot y axis values
    ////////////////////                                                      
	PlotCaptions(pDC, ptOrigin, sizePixelsPerTick, 0, m_nYValuePerTick,
						nVertTicks+1, nCharHeight, bVert, FALSE);
                        
    ////////////////////                    
    //plot x axis values
    ////////////////////
	//offset Origin to plot column letters in middle of Division
	CPoint ptTemp = ptOrigin;
	if (!bVert) ptTemp.y -= (int)(nDataSetWidth / 2);
		else ptTemp.x += (int)(nDataSetWidth / 2);
	//PlotCaptions(pDC, ptTemp, sizePixelsPerTick, 'A' , nXValuePerTick,
	//					nHorzTicks, nCharHeight, !bVert, TRUE);
	PlotCaptions(pDC, ptTemp, sizePixelsPerTick, 0, m_nXValuePerTick,
						nHorzTicks+1, nCharHeight, !bVert, FALSE);
      

    

    ////////////////////////////////////////////
    //Plot data from the COWGraphDoc::m_GraphSetList
    ////////////////////////////////////////////                        
	COWGraphSet* p_currentGraphSet = NULL; 
	COWGraphSet* p_previousGraphSet = NULL; 
	CPen* pPen = NULL;
	CPen* pOldPen = NULL;
	CBrush* pBrush = NULL;
	CBrush* pOldBrush = NULL;
	COLORREF crColor;
	int nOldBkMode = -1;

    int nSet = 0;
	int indexstart = 0;
	int indexend = pDoc->m_GraphSetList.GetSize(); 
	int indexstep = 1;
	if(m_lgraphtype&GRAPH_REVERSE_CHANNEL_DISPLAY_ORDER)
	{
		indexstart = indexend-1;
		indexend = 0-1;
		indexstep = -1;
	}
	//for(int index=0; index<indexend; index++)
	for(int index=indexstart; index!=indexend; index+=indexstep)
	{   
		/*poirier oct 97,  work under progress ...
		int nPrevIndex = -1;
		*/
		//for each COWGraphSet in the m_GraphSetList
		p_currentGraphSet = (COWGraphSet*) pDoc->m_GraphSetList.GetAt((int)index);
		//check if this set is visible
		if(p_currentGraphSet->IsVisible())
		{   
			/*poirier oct 97, work under progress ...
			//store index
			nPrevIndex = index;
			*/
			//visible set counter
			nSet++; 

			int iDefaultColorIndex = -1;
			if(m_lgraphtype&GRAPH_REVERSE_CHANNEL_DISPLAY_ORDER)
			{
				iDefaultColorIndex = pDoc->GetNumberOfVisibleSets() - nSet;
			}
			else
			{
				iDefaultColorIndex = nSet-1;
			}
			crColor = GetGraphSetColor(p_currentGraphSet, iDefaultColorIndex);
			
			//if GRAPH_FORFIRSTCHANNEL_OUTLINE_BAR_TOP_ONLY flag present AND drawing the 
			//first channel graphset, the pen will be used to draw the bar top only.
			BOOL bCreatePenToDrawBarTopOnly = FALSE;
			if(m_lgraphtype&GRAPH_FORFIRSTCHANNEL_OUTLINE_BAR_TOP_ONLY)
			{
				if(m_lgraphtype&GRAPH_REVERSE_CHANNEL_DISPLAY_ORDER)
				{
					if(nSet==pDoc->GetNumberOfVisibleSets())
					{
						//this is the last graphset to be drawn (it is the first graphdoc's channel to display) 
						bCreatePenToDrawBarTopOnly = TRUE;
					}
				}
				else
				{
					if(nSet==1)
					{
						//this is the first graphset to be drawn (it is also the first graphdoc's channel to display) 
						bCreatePenToDrawBarTopOnly = TRUE;
					}
				}
			}
			if(bCreatePenToDrawBarTopOnly==TRUE)
			{
				//use pen to draw the bar top only  
	        	pPen = (CPen*) new(CPen);
				if(pPen->CreatePen(PS_SOLID, pDoc->m_iPenWidthForFirstChannelBarTop, crColor) == 0) 
				{
					//ERR_HANDLER_SHOWWARNING( IDS_ERR_CREATE_GDIOBJ );
					AfxMessageBox(L"Not enough GDI memory available, free other applications");
				}
				pOldPen = pDC->SelectObject(pPen);
				//set background mode to transparent
				nOldBkMode =  pDC->SetBkMode( TRANSPARENT );

			}
			else
			{
				//create pen with proper color, the pen is used to draw the bars contour 
				if( nBarWidth<GRAPH_BARCONTOUR_THRESHOLD_INPIXEL || ( pDC->IsKindOf(RUNTIME_CLASS(CMetaFileDC))
									 && nBarWidth<( GRAPH_BARCONTOUR_THRESHOLD_INPIXEL
													* MMHIMETRIC_TO_MMTEXT_SCALEFACTOR ) )
				  )
				{   
					//use a pen for the bar contour that has the same color as the filling 
	        		pPen = (CPen*) new(CPen);
					if(pPen->CreatePen(PS_SOLID, 1, crColor) == 0) 
					{
						//ERR_HANDLER_SHOWWARNING( IDS_ERR_CREATE_GDIOBJ );
						AfxMessageBox(L"Not enough GDI memory available, free other applications");
					}
					pOldPen = pDC->SelectObject(pPen);
				}
				else
				{   
					//since there is room, draw a black contour around each bars 
	        		pPen = (CPen*) new(CPen);
					if(pPen->CreatePen(PS_SOLID, 1, RGB(0,0,0)) == 0) 
					{
						//ERR_HANDLER_SHOWWARNING( IDS_ERR_CREATE_GDIOBJ );
						AfxMessageBox(L"Not enough GDI memory available, free other applications");
					}
					pOldPen = pDC->SelectObject(pPen);
				}
			}

			//create brush with proper color, used for the filling 
			pBrush = (CBrush*) new(CBrush);
			if(pBrush->CreateSolidBrush(crColor) == 0) 
			{
				//ERR_HANDLER_SHOWWARNING( IDS_ERR_CREATE_GDIOBJ );
				AfxMessageBox(L"Not enough GDI memory available, free other applications");
			}
			pOldBrush = pDC->SelectObject(pBrush);
			
			//the offset for the 1st visible set is 0, therefore (nSet-1)*nBarWidth                                          
			int nOffset = (nSet-1) * nBarWidth; 
			//if GRAPH_PILED flag present, all bars will be displayed on top of each other
			if(m_lgraphtype&GRAPH_PILED) nOffset = 0;
			//optimize depending if int or float array
			if(p_currentGraphSet->GetDataType() == LONGTYPE)
			{
				for(int i=0; i<p_currentGraphSet->GetDataSize(); i++)
				{	
				    BOOL bskip = FALSE;
				    if(m_numberofvalueperpixel>1)
				    {   
				    	//skip values that are not a multiple of m_numberofvalueperpixel 
				    	int iratio = ((i+1)/m_numberofvalueperpixel);
				    	float fratio = (float)(i+1)/(float)m_numberofvalueperpixel;
				    	float fdiff = fratio-(float)iratio;
				    	if(fdiff!=0.0f) bskip = TRUE;
				    }
				    
				    if(!bskip)
				    {
						//
						//compute xPos, xPos means distance from origin along graphset axis 
						//not necessary horizontal position.
						int xPos;
						if(m_numberofvalueperpixel>1)
						{ 
							xPos = nOffset + (nDataSetWidth * (((i+1)/m_numberofvalueperpixel)-1) ) + nSpaceWidth;
						}
						else	
						{
							xPos = nOffset + (nDataSetWidth * i) + nSpaceWidth; 
						}                               
						                               
						//if vertical 2D Bar Graph Display				      
						if (bVert) 
						{   

							//if m_numberofvalueperpixel is greater than 1, it means
							//we display one out of m_numberofvalueperpixel values.
							//therefore, skipping all values that are not even multiple
							//of m_numberofvalueperpixel would give something like: 
							// i=0, 2, 4, ... or  i=0, 3, 6, ... (displayed value index).
							//
							//since we now handle different display fit
							int ii = 0; 
							int jj = 0; 
							int ldisplayedvalue = p_currentGraphSet->GetValueAsLong(i); 
							int lnextvalue = 0L;
					    	if(m_numberofvalueperpixel>1)
					    	{
					    		switch(m_ldisplayfit)
					    		{
						    		case DISPLAY_FIT_MAX_VALUE:	
						    				//select the maximum value for display
						    		        ldisplayedvalue = p_currentGraphSet->GetValueAsLong(i);											
						    				for(ii=1; ii<m_numberofvalueperpixel; ii++)
						    				{   
						    					if( (i+ii)>=(p_currentGraphSet->GetDataSize()) ) break;
						    					lnextvalue = p_currentGraphSet->GetValueAsLong(i+ii);
						    					if( lnextvalue>ldisplayedvalue ) ldisplayedvalue = lnextvalue;	
						    				}
						    				break;
						    		case DISPLAY_FIT_MIN_VALUE: 
						    				//select the minimum value for display
						    		        ldisplayedvalue = p_currentGraphSet->GetValueAsLong(i);
						    				for(ii=1; ii<m_numberofvalueperpixel; ii++)
						    				{
												if( (i+ii)>=(p_currentGraphSet->GetDataSize()) ) break;
						    					lnextvalue = p_currentGraphSet->GetValueAsLong(i+ii);
						    					if( lnextvalue<ldisplayedvalue ) ldisplayedvalue = lnextvalue;	
						    				}
						    				break;	    
									case DISPLAY_FIT_AVERAGE_VALUE:
						    				//compute the average value for display
						    				jj=0;
						    				for(ii=0; ii<m_numberofvalueperpixel; ii++)
						    				{   
						    					if( (i+ii)>=(p_currentGraphSet->GetDataSize()) ) break;
						    					jj++;
						    		        	ldisplayedvalue+= p_currentGraphSet->GetValueAsLong(i+ii);
						    				}
						    				ldisplayedvalue = ldisplayedvalue / jj;
											break;
									case DISPLAY_FIT_FIRST_VALUE: 
											//doing nothing additional, we get the first value out of 
											//each m_numberofvalueperpixel. 
											ldisplayedvalue = p_currentGraphSet->GetValueAsLong(i);
											break;
						    	    default: 
						    	    	    m_ldisplayfit = DISPLAY_FIT_MAX_VALUE;
						    				//select the maximum value for display
						    		        ldisplayedvalue = p_currentGraphSet->GetValueAsLong(i);
						    				for(ii=1; ii<m_numberofvalueperpixel; ii++)
						    				{   
						    					if( (i+ii)>=(p_currentGraphSet->GetDataSize()) ) break;
						    					lnextvalue = p_currentGraphSet->GetValueAsLong(i+ii);
						    					if( lnextvalue>ldisplayedvalue ) ldisplayedvalue = lnextvalue;	
						    				}
						    	    		break;
					    	    }		
					    	}						  
						
						  	//compute y
							int yPos = (  (ldisplayedvalue * (int)sizePixelsPerTick.cy) 
											/ m_nYValuePerTick  );      
							/*
							poirier, oct 97, work under progress ....
							int nVBarBaseY;
							if(	GRAPH_VBAR_PILED ||
								GRAPH_VBAR_PILED_AND_FIRSTCHANNEL_TOPVBARONLY)
							{
								if(nPrevIndex!=-1)
								{
									//base value equals previous visible set top y value
									p_previousGraphSet = (COWGraphSet*) pDoc->m_GraphSetList.GetAt((int)index);
									nVBarBaseY = p_previousGraphSet->GetValueAsLong(i);
								}
								//top value equals base + yPos
								
							}
							*/
							//if GRAPH_FORFIRSTCHANNEL_OUTLINE_BAR_TOP_ONLY flag present AND drawing the 
							//first channel graphset, the pen will be used to draw the bar top only.
							if(bCreatePenToDrawBarTopOnly==TRUE)
							{
								//draw bar top only (thick line)
								if(i==0) 
								{
									//move to this bar top left corner 
									pDC->MoveTo(ptOrigin.x + (int)xPos, ptOrigin.y - (int)yPos);
									//draw a line to this bar top right corner
									pDC->LineTo(ptOrigin.x + (int)(xPos + (nBarWidth-1)), ptOrigin.y - (int)yPos);
								}
								else
								{
									//draw a line (from last bar top right corner) to this bar top left corner 
									pDC->LineTo(ptOrigin.x + (int)xPos, ptOrigin.y - (int)yPos);
									//draw a line from this bar top left corner to top right corner
									pDC->LineTo(ptOrigin.x + (int)(xPos + (nBarWidth-1)), ptOrigin.y - (int)yPos);
								}
							}
							else
							{
								//draw bar
								if(nBarWidth==1)
								{   
									pDC->MoveTo(ptOrigin.x + (int)xPos, ptOrigin.y - (int)yPos);
									pDC->LineTo(ptOrigin.x + (int)xPos, ptOrigin.y);
								}
								else if(nBarWidth==2)
								{
									pDC->MoveTo(ptOrigin.x + (int)xPos, ptOrigin.y - (int)yPos);
									pDC->LineTo(ptOrigin.x + (int)xPos, ptOrigin.y);
									pDC->MoveTo(ptOrigin.x + (int)xPos+1, ptOrigin.y - (int)yPos);
									pDC->LineTo(ptOrigin.x + (int)xPos+1, ptOrigin.y);
								}
								else
								{			 	   
									pDC->Rectangle(ptOrigin.x + (int)xPos, ptOrigin.y - (int)yPos,
												   ptOrigin.x + (int)(xPos + (nBarWidth-1)), ptOrigin.y);
								}
							}
						}	 
						//if horizontal 2D Bar Graph Display				      
						else 
						{
							int yPos = ((p_currentGraphSet->GetValueAsLong(i) *
										 (int)sizePixelsPerTick.cx) / m_nXValuePerTick ); 
							pDC->Rectangle(ptOrigin.x, ptOrigin.y - (int)xPos - ((int)nBarWidth-1),
										   ptOrigin.x + (int)yPos, ptOrigin.y - (int)xPos);
						}
						xPos += nBarWidth;	
					}//end of if(!bskip)
				}
			}
			else if(p_currentGraphSet->GetDataType() == FLOATTYPE
				|| p_currentGraphSet->GetDataType() == DOUBLETYPE)
			{    
				for(int i=0; i<p_currentGraphSet->GetDataSize(); i++)
				{	
				    BOOL bskip = FALSE;
				    if(m_numberofvalueperpixel>1)
				    {   
				    	//skip values that are not a multiple of m_numberofvalueperpixel 
				    	int iratio = ((i+1)/m_numberofvalueperpixel);
				    	float fratio = (float)(i+1)/(float)m_numberofvalueperpixel;
				    	float fdiff = fratio-(float)iratio;
				    	if(fdiff!=0.0f) bskip = TRUE;
				    }
				    
				    if(!bskip)
				    {   
						//
						//compute xPos, xPos means distance from origin along graphset axis 
						//not necessary horizontal position.
						int xPos;
						if(m_numberofvalueperpixel>1)
						{ 
							xPos = nOffset + (nDataSetWidth * (((i+1)/m_numberofvalueperpixel)-1) ) + nSpaceWidth;
						}
						else	
						{
							xPos = nOffset + (nDataSetWidth * i) + nSpaceWidth; 
						}                               
						                               
						//if vertical 2D Bar Graph Display				      
						if (bVert) 
						{   
							//if m_numberofvalueperpixel is greater than 1, it means
							//we display one out of m_numberofvalueperpixel values.
							//therefore, skipping all values that are not even multiple
							//of m_numberofvalueperpixel would give something like: 
							// i=0, 2, 4, ... or  i=0, 3, 6, ... (displayed value index).
							//
							//since we now handle different display fit
							int ii = 0;   
							int jj = 0;
							double fdisplayedvalue = p_currentGraphSet->GetValueAsFloat(i);
							double fnextvalue = 0.0f;
					    	if(m_numberofvalueperpixel>1)
					    	{
					    		switch(m_ldisplayfit)
					    		{
						    		case DISPLAY_FIT_MAX_VALUE:	
						    				//select the maximum value for display
						    		        fdisplayedvalue = p_currentGraphSet->GetValueAsFloat(i);
						    				for(ii=1; ii<m_numberofvalueperpixel; ii++)
						    				{
						    					if( (i+ii)>=(p_currentGraphSet->GetDataSize()) ) break;
						    					fnextvalue = p_currentGraphSet->GetValueAsFloat(i+ii);
						    					if( fnextvalue>fdisplayedvalue ) fdisplayedvalue = fnextvalue;	
						    				}
						    				break;
						    		case DISPLAY_FIT_MIN_VALUE: 
						    				//select the minimum value for display
						    		        fdisplayedvalue = p_currentGraphSet->GetValueAsFloat(i);
						    				for(ii=1; ii<m_numberofvalueperpixel; ii++)
						    				{   
						    					if( (i+ii)>=(p_currentGraphSet->GetDataSize()) ) break;
						    					fnextvalue = p_currentGraphSet->GetValueAsFloat(i+ii);
						    					if( fnextvalue<fdisplayedvalue ) fdisplayedvalue = fnextvalue;	
						    				}
						    				break;	    
									case DISPLAY_FIT_AVERAGE_VALUE:
						    				//compute the average value for display 
						    				jj=0;
						    				for(ii=0; ii<m_numberofvalueperpixel; ii++)
						    				{
						    					if( (i+ii)>=(p_currentGraphSet->GetDataSize()) ) break; 
						    					jj++;
						    		        	fdisplayedvalue+= p_currentGraphSet->GetValueAsFloat(i+ii);
						    				}
						    				fdisplayedvalue = fdisplayedvalue / (float)jj;
											break;
									case DISPLAY_FIT_FIRST_VALUE: 
											//doing nothing additional, we get the first value out of 
											//each m_numberofvalueperpixel. 
											fdisplayedvalue = p_currentGraphSet->GetValueAsFloat(i);
											break;
						    	    default: 
						    	    	    m_ldisplayfit = DISPLAY_FIT_MAX_VALUE;
						    				//select the maximum value for display
						    		        fdisplayedvalue = p_currentGraphSet->GetValueAsFloat(i);
						    				for(ii=1; ii<m_numberofvalueperpixel; ii++)
						    				{
						    					if( (i+ii)>=(p_currentGraphSet->GetDataSize()) ) break;
						    					fnextvalue = p_currentGraphSet->GetValueAsFloat(i+ii);
						    					if( fnextvalue>fdisplayedvalue ) fdisplayedvalue = fnextvalue;	
						    				}
						    	    		break;
					    	    }		
					    	}						  
							int yPos = (int) ( (fdisplayedvalue * (float)sizePixelsPerTick.cy) 
												  / (float)m_nYValuePerTick );      
							//if GRAPH_FORFIRSTCHANNEL_OUTLINE_BAR_TOP_ONLY flag present AND drawing the 
							//first channel graphset, the pen will be used to draw the bar top only.
							if(bCreatePenToDrawBarTopOnly==TRUE)
							{
								//draw bar top only (thick line)
								if(i==0) 
								{
									//move to this bar top left corner 
									pDC->MoveTo(ptOrigin.x + (int)xPos, ptOrigin.y - (int)yPos);
									//draw a line to this bar top right corner
									pDC->LineTo(ptOrigin.x + (int)(xPos + (nBarWidth-1)), ptOrigin.y - (int)yPos);
								}
								else
								{
									//draw a line (from last bar top right corner) to this bar top left corner 
									pDC->LineTo(ptOrigin.x + (int)xPos, ptOrigin.y - (int)yPos);
									//draw a line from this bar top left corner to top right corner
									pDC->LineTo(ptOrigin.x + (int)(xPos + (nBarWidth-1)), ptOrigin.y - (int)yPos);
								}
							}
							else
							{
								//draw bar
								if(nBarWidth==1)
								{   
									pDC->MoveTo(ptOrigin.x + (int)xPos, ptOrigin.y - (int)yPos);
									pDC->LineTo(ptOrigin.x + (int)xPos, ptOrigin.y);
								}
								else if(nBarWidth==2)
								{
									pDC->MoveTo(ptOrigin.x + (int)xPos, ptOrigin.y - (int)yPos);
									pDC->LineTo(ptOrigin.x + (int)xPos, ptOrigin.y);
									pDC->MoveTo(ptOrigin.x + (int)xPos+1, ptOrigin.y - (int)yPos);
									pDC->LineTo(ptOrigin.x + (int)xPos+1, ptOrigin.y);
								}
								else
								{			 	   
									pDC->Rectangle(ptOrigin.x + (int)xPos, ptOrigin.y - (int)yPos,
												   ptOrigin.x + (int)(xPos + (nBarWidth-1)), ptOrigin.y);
								}
							}
						}	 
						//if horizontal 2D Bar Graph Display				      
						else 
						{
							//not fully supported yet            
							
							//TODO add your code similar to the vertical case
						}
						xPos += nBarWidth;	
					}//end of if(!bskip)
				}
			}   
			//
			//depending pen and brush
			pDC->SelectObject((CPen*)pOldPen); 
			delete(pPen);
			pDC->SelectObject((CBrush*)pOldBrush); 
			delete(pBrush);	
			if(nOldBkMode!=-1) pDC->SetBkMode(nOldBkMode);
		}//end of if(visible set)				    
	}//end of for(i=0 to end of m_GraphSetList)	
	// 
	//
	//before returning, update the graph scale-related member variables
	m_ptGraphOrigin = ptOrigin;
	m_sizeGraph.cx = (int)((pDoc->m_maxnumberofpointsperset) * 
					 ((float)sizePixelsPerTick.cx/(float)m_nXValuePerTick));
	m_sizeGraph.cy = (int)(nMaxValue * 
					 ((float)sizePixelsPerTick.cy/(float)m_nYValuePerTick));
	m_sizePixelsPerTick = sizePixelsPerTick;
	m_nPrevVBarPos = GRAPH_BARPOS_INVISIBLE;   
	m_nBarWidth = nBarWidth;
	m_bUpdate = FALSE;
}     


////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to display a vertical bar at the mouse location     
// 
// It draws in NOT mode a vertical 1 pixel width line in the case of GRAPH_LINE
// or a rectangle nBarWith*numvisiblechannels pixel width in the case of GRAPH_VBAR
////////////////////////////////////////////////////////////////////////////////////////////////
void COWGraphView::DrawVBar(CDC* pDC, int n_xoffset, int nBarOffset, int nBarHeight)         
{            
	//this function assumes graph area related member variables have been refresh.
	if(m_lgraphtype&GRAPH_LINE)
	{
		int oldDrawMode = pDC->SetROP2(R2_NOT);
		pDC->MoveTo(m_ptGraphOrigin.x + (int)n_xoffset, m_ptGraphOrigin.y);	
		pDC->LineTo(m_ptGraphOrigin.x + (int)n_xoffset, m_ptGraphOrigin.y - m_sizeGraph.cy);
		pDC->SetROP2(oldDrawMode);
	}
	else if(m_lgraphtype&GRAPH_VBAR)
	{
		int oldDrawMode = pDC->SetROP2(R2_NOT);
		if(m_nBarWidth>0 && m_nBarWidth<4)
		{   
			//bar width is 1 pixel wide
		pDC->MoveTo(m_ptGraphOrigin.x + (int)n_xoffset, m_ptGraphOrigin.y);	
		pDC->LineTo(m_ptGraphOrigin.x + (int)n_xoffset, m_ptGraphOrigin.y - m_sizeGraph.cy);
		}
		else if(m_nBarWidth>0)
		{   
			//the bar width is large enough
			int nleftpos = m_ptGraphOrigin.x + n_xoffset + (nBarOffset*m_nBarWidth);
			pDC->Rectangle((int)nleftpos, 						//left
						   m_ptGraphOrigin.y,               	//top
						   (int)(nleftpos + m_nBarWidth), 		//right
						   m_ptGraphOrigin.y - (int)nBarHeight);//bottom
		}			   
		pDC->SetROP2(oldDrawMode);
	}
	return;
}           


////////////////////////////////////////////////////////////////////////////////////////////////
// This function returns information on the vertical bar that is drawn at the specified location.
////////////////////////////////////////////////////////////////////////////////////////////////
VBARINFO COWGraphView::GetNextVBarInfo(CPoint point)
{   

    VBARINFO NextVBarInfo;

    NextVBarInfo.nPos = 0;  	//from 0 to GetDataSize() (it's the index of the value in the data array)
    NextVBarInfo.nOffset = 0;   //from 0 to number of graphset (it's the index of the graphset)
    NextVBarInfo.nHeight = 0;   //it corresponds to the y value, from the data array, converted in pixels
         
	COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument();
	COWGraphSet* p_currentGraphSet = NULL; 

	// 1) find the index of the bar set at the mouse location
	int xindex = (((int)point.x - (int)m_ptGraphOrigin.x -1) * 
			(int)m_nXValuePerTick)/(int)m_sizePixelsPerTick.cx; 
	//in the case of a bar graph, we want the bar pos to be discrete.		
	NextVBarInfo.nPos = ((int)xindex * m_sizePixelsPerTick.cx / (int)m_nXValuePerTick);

    // 2) find the index of the graphset at the mouse location			   
	//m_nPrevVBarOffset could be 0,1,2,... representing the ith visible graphset.		
    NextVBarInfo.nOffset = ( point.x - m_ptGraphOrigin.x - NextVBarInfo.nPos) / m_nBarWidth; 

    // 3) now, we can get the yvalue of the graphset at the mouse location 
    //    and convert it in pixel
    int indexend = pDoc->m_GraphSetList.GetSize();					
    //
    if( (NextVBarInfo.nOffset < (pDoc->GetNumberOfVisibleSets())) && NextVBarInfo.nOffset > -1)
    {					
	    p_currentGraphSet = NULL;
	    int index = 0;
	    int nSet = 0;
		while(nSet <= NextVBarInfo.nOffset)
		{                                                
			//for each COWGraphSet in the m_GraphSetList
			p_currentGraphSet = (COWGraphSet*) pDoc->m_GraphSetList.GetAt((int)index);
			//check if this set is visible
			if(p_currentGraphSet->IsVisible())
			{          
				//visible set counter
				nSet++; 
	    	}
	    	index++;
	    }
	    ASSERT(p_currentGraphSet);
	    NextVBarInfo.nHeight = ((p_currentGraphSet->GetValueAsLong((int)xindex) * 
	    						(int)m_sizePixelsPerTick.cy) / m_nYValuePerTick );	
    }//end of if no error in the graphset index
    else
    {
		//if error was found trying to get a valid graphset index                      
		NextVBarInfo.nOffset = GRAPH_BARPOS_INVISIBLE;
		NextVBarInfo.nHeight = GRAPH_BARPOS_INVISIBLE;
    }//end of if error was found in the graphset index
    return NextVBarInfo;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// This function returns the graphset's name.
// 		-if name is null, it returns a default name based on either the lchannelid or 
//		 the suplied index.
////////////////////////////////////////////////////////////////////////////////////////////////
CString COWGraphView::GetGraphSetName(COWGraphSet* p_currentGraphSet, int index)
{                 
	CString string("");
	if(p_currentGraphSet!=NULL)
	{
		string = p_currentGraphSet->GetGraphSetName();
		//
		//if string is null, give a default name
		if(string.IsEmpty())
		{			
			//if graphset's member lchannelid is not null, use it for default naming 					
			if(p_currentGraphSet->GetChannelID()>0
			   && p_currentGraphSet->GetChannelID()<MAXCHANNELNUMBER
			   && index<MAXCHANNELNUMBER)
			{   
				//in this case, we use the channelid number (default 1,2,3 is Red,Green,Blue)
				string += channelnameTable[(p_currentGraphSet->GetChannelID())]; 
			} 
			//else use the index for the default naming
			else if( index>-1 && index<MAXCHANNELNUMBER )
			{   
				//in this case, we use the index number (default 0,0,0,0 is Ch1,Ch2,Ch3,Ch4)								
				string += channelnumberTable[index]; 
			}				
		}
	}
	return string;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// This function returns a string containing the y-values from all the visible graphsets
// in a given format, it includes also the channel names.  Ex:  "Red: 101  Green: 332  Blue: 12" 
////////////////////////////////////////////////////////////////////////////////////////////////
CString COWGraphView::GetVisibleGraphSetValues(CPoint point)
{   
	char text[255];         
	CString string(""); 
	COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument();
	COWGraphSet* p_currentGraphSet = NULL; 

	int xindex = (((int)point.x - (int)m_ptGraphOrigin.x -1) * (int)m_nXValuePerTick)/
				    (int)m_sizePixelsPerTick.cx; 
	
    int nSet = 0;			    
    p_currentGraphSet = NULL; 
	int indexend = pDoc->m_GraphSetList.GetSize();  
	for(int index=0; index<indexend; index++)
	{                                                
		//for each COWGraphSet in the m_GraphSetList
		p_currentGraphSet = (COWGraphSet*) pDoc->m_GraphSetList.GetAt((int)index);
		//check if this set is visible
		if(p_currentGraphSet->IsVisible())
		{          
			//visible set counter
			nSet++; 
			string += "  ";  
			string += GetGraphSetName(p_currentGraphSet, index); //we provide the index in case graphset has no name 
			string += ":  ";
			//
			//get value directly from data array 
			//or if (interpolation required and desired) interpolate between two values  
			//
			int lvalue = 0;
			double fvalue = 0.0f;							
			//check if linear interpolation desired and required
			if( (m_sizeGraph.cx > (3 * pDoc->m_maxnumberofpointsperset)) 
			     && ((xindex+1) < pDoc->m_maxnumberofpointsperset)
			     && (m_lgraphtype&GRAPH_LINE)
			     && m_bInterpolationDesired
			  )
			{															
				double fvalue1 = p_currentGraphSet->GetValueAsFloat((int)xindex);
				int xindex2 = xindex+1;
				double fvalue2 = p_currentGraphSet->GetValueAsFloat((int)xindex2);
				//xfloat is the "true" mouse position while
				//xindex is the data array's index corresponding to mouse position.      
				double xfloat = ( ((float)(point.x - m_ptGraphOrigin.x)) * 
								 ( ((float)m_nXValuePerTick)/
			    				   ((float)m_sizePixelsPerTick.cx) )
			    			   );
			    //the interpolated value is then: 			   								
				fvalue =  (fvalue1 + 
						  ((fvalue2-fvalue1)/((float)(xindex2-xindex)))
						   *(xfloat-((float)xindex)) 
						  );
		    	sprintf(text, "%.3f", fvalue);   
		    	string += text;
			} 
			//else if no interpolation to do, take data directly from array
			else
			{   
				WCHAR wctext[255];
		    	lstrcpy( wctext, p_currentGraphSet->GetValueAsString((int)xindex) );
		    	string += wctext;
		    }	
		}//end of if(visible set)				    
	}//end of for(i=0 to end of m_GraphSetList)  
    return string;
}



                            
////////////////////////////////////////////////////////////////////////////////////
// OnMouseMove() function
// 
// In this function, you will find repeated pieces of code.  this has been done 
// on purpose since we want to optimize for speed (you should know that this routine
// is called every single time the user moves the mouse cursor).
////////////////////////////////////////////////////////////////////////////////////                           
void COWGraphView::OnMouseMove(UINT nFlags, CPoint point)
{     
	//
    //first, checks if this child view window is selected   
    //
    COWGraphMDIChildWnd* pThisMDIChildWnd = (COWGraphMDIChildWnd*) this->GetParentFrame(); 
	if(pThisMDIChildWnd) 
	{
		if(!pThisMDIChildWnd->IsKindOf(RUNTIME_CLASS(COWGraphMDIChildWnd))) return;
	}
	CMDIFrameWnd* pMDIParentFrameWnd = (CMDIFrameWnd*) pThisMDIChildWnd->GetMDIFrame(); 
	COWGraphMDIChildWnd* pActiveMDIChildWnd = (COWGraphMDIChildWnd*) pMDIParentFrameWnd->MDIGetActive();

    
	if( m_HasFocus &&  pThisMDIChildWnd == pActiveMDIChildWnd)
	{
	    char text[255];
	
	    CStatusBar* pStatus = (CStatusBar*)
	        AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_GRAPHVIEW_STATUSBAR);
	    
	    //if the graphview's status bar is present                                          
	    if (pStatus) 
	    { 
			COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument();
			COWGraphSet* p_currentGraphSet = NULL; 
			
			//if the mouse curosr i\lies inside the graph area 
			//and if more than one visible graphset
	    	if( (point.x > m_ptGraphOrigin.x) && 
	    		(point.x <= (m_ptGraphOrigin.x + m_sizeGraph.cx)) &&
	    		(point.y <= m_ptGraphOrigin.y) && 
	    		(point.y >= (m_ptGraphOrigin.y - m_sizeGraph.cy))
	    	   && (pDoc->GetNumberOfVisibleSets()>0)
	    	  )
	    	{	     
		      	////////////////////////////////////////////////////////////
		      	//first, take care of the vertical bar (but only if visible)
		      	////////////////////////////////////////////////////////////
		      	if(m_bVBarVisible)
		      	{   
		      		CDC* pDC = GetDC();
		      		
		      		if(m_lgraphtype&GRAPH_LINE)
		      		{   
		      			//
		      			// 1) if a VBar is drawn on the graph, erase it.
		      			//
			      		if(m_nPrevVBarPos > 0)
			      		{   
			      			DrawVBar(pDC, m_nPrevVBarPos, m_nPrevVBarOffset, m_nPrevVBarHeight); 
			      		}  
			      		//
		      			// 2) find new VBar position
		      			//
		      			m_nPrevVBarPos = point.x - m_ptGraphOrigin.x;		      			                     
		      			m_nPrevVBarOffset = 0;  //not used for line graph
		      			m_nPrevVBarHeight = 0;  //not used for line graph 
		      			//
		      			// 3) draw new VBar
		      			//
		      			DrawVBar(pDC, m_nPrevVBarPos, m_nPrevVBarOffset, m_nPrevVBarHeight); 
		      		}		
		      		
		      		
		      		if(m_lgraphtype&GRAPH_VBAR)
		      		{   
		      		    
						//
						// 1) find which bar would be the next to highlight
						//		     
		      		    VBARINFO NextVBarInfo = GetNextVBarInfo(point);	
		      		    	      		    		      	
		      		    //
		      		    // 2) if not the same VBar as for previous mouse pos, 
		      		    //    erase old VBar and display new one, 
		      		    //    otherwise leave things unchanged. 
		      		    //	
		      		    if( NextVBarInfo.nOffset != m_nPrevVBarOffset ||
		      		        NextVBarInfo.nPos != m_nPrevVBarPos )
		      		    {   
		      		    	//erase old VBar
		      		    	if( m_nPrevVBarPos>0 || (m_nPrevVBarPos==0 && (point.x-m_ptGraphOrigin.x)>0) )
		      		    	{  
			      				DrawVBar(pDC, m_nPrevVBarPos, m_nPrevVBarOffset, m_nPrevVBarHeight); 
			      			}	
				      		//store new VBar info  
		      		    	m_nPrevVBarPos = NextVBarInfo.nPos;
		      		    	m_nPrevVBarOffset = NextVBarInfo.nOffset;
		      		    	m_nPrevVBarHeight = NextVBarInfo.nHeight;
		      		    	//display new VBar
				      		DrawVBar(pDC, m_nPrevVBarPos, m_nPrevVBarOffset, m_nPrevVBarHeight);  
		      		    }    
		      		    	      		    
		      		}//end of if graph VBar 
		      		ReleaseDC(pDC);
		      	} 
		      	
		      	
		      	////////////////////////////////////////////////////////////////
		      	//display, the mouse position-dependent	values in the status bar
		      	////////////////////////////////////////////////////////////////
		      	
		      	// 1) display x-index
			    CString string("");			    
	    		int xindex = (((int)point.x - (int)m_ptGraphOrigin.x -1) * (int)m_nXValuePerTick)/
	    					    (int)m_sizePixelsPerTick.cx;  
	    					    
	    		string = pDoc->m_XAxisLabelString;
                string += ": ";
	      		wsprintfA(text, "%ld", xindex);  
	      		string += text;
	      		pStatus->SetPaneText( 0, string); // first pane
	            
	             
	            // 2) display y-values (for all the visible graphsets)
	            string = pDoc->m_YAxisLabelString;
	            string += ":  "; 
			    string += GetVisibleGraphSetValues(point);				
				//all the y-values should have been writen to the string
	      		pStatus->SetPaneText( 1, string); // second pane					
	      	}//end of if(inside graph drawing area)	      	
	      	//
	      	//else if mouse cursor outside the graph area and if a VBar was drawn
	      	else if(m_nPrevVBarPos >= 0 && m_bVBarVisible )
	      	{                      
	      		//erase old VBar
		      	CDC* pDC = GetDC();		      	
		      	DrawVBar(pDC, m_nPrevVBarPos, m_nPrevVBarOffset, m_nPrevVBarHeight); 
	      		m_nPrevVBarPos = GRAPH_BARPOS_INVISIBLE;
		      	ReleaseDC(pDC);
	      	}//end of if(outside graph drawing area)
	    }
	}//end of if(this window is activated)
	    
	CScrollView::OnMouseMove(nFlags, point);
}

               
                 
                 
void COWGraphView::OnSize(UINT nType, int cx, int cy)
{
	if(m_nMapMode > 0)
	{   
		//1) reset the scroll position to 0, because our graphic drawing
		//   functions assume so.
		CPoint ptScroll = GetScrollPosition();
		if (ptScroll.x > 0 || ptScroll.y > 0)
		{
			ptScroll.x = 0;
			ptScroll.y = 0;
			ScrollToPosition(ptScroll);
		}
		SetScrollSizes();
	}
	CScrollView::OnSize(nType, cx, cy);

	return;
}
   
                 
                 
void COWGraphView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    //substract actual view offset to graph origin
	CPoint ptViewOffset;
	ptViewOffset = GetScrollPosition();
    m_ptGraphOrigin.x += ptViewOffset.x;
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
	//add new view offset to graph origin
	ptViewOffset = GetScrollPosition();
	m_ptGraphOrigin.x -= ptViewOffset.x;
}


void COWGraphView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CPoint ptViewOffset;
	ptViewOffset = GetScrollPosition();
    m_ptGraphOrigin.y += ptViewOffset.y;
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
	//add new view offset to graph origin
	ptViewOffset = GetScrollPosition();
	m_ptGraphOrigin.y -= ptViewOffset.y;
}


 
void COWGraphView::OnEditCopy()
{	
	//if we want to copy the graph as a metafile to the clipboard,
	//the technique is to redraw the graph in a MetafileDC and pass
	//the handle to metafile to the clipboard.
	//
	//technicaly, we replace the pCDC by a pCMetaFileDC that has its original m_hDC
	//but that has a redirected m_hAttribDC.  see Microsoft Visual C++1.0 Class Ref. 
	//p.556 for better understanding.
	
	                 
	//get and prepare a DC.                 
	CDC* pDC = GetDC(); 
	
	CMetaFileDC* pMetaFileDC = new CMetaFileDC; 
	
	//create a memory metafile
	VERIFY(pMetaFileDC->Create(NULL));         
	//now, we redirect the pMetaFileDC->m_hAttribDC from NULL to pDC->m_hAttribDC
	pMetaFileDC->SetAttribDC(pDC->m_hAttribDC);
	CRect rect = GetDisplayRect(pMetaFileDC);  
	
	//poirier  
	//
	//with what seems to be the new CMetaFileDC OLE standard, we must draw to
	//the metafile DC using MM_TEXT units.  When exporting (copy/paste) in
	//MM_HIMETRIC unit, we specify the size in MM_HIMETRIC unit and the drawing
	//will be automaticalt converted to these units.  The automatic conversion
	//was not perform in the past, therefore we were drawing the graph 35 times
	//bigger.
	//
	//convert to MM_HIMETRIC unit (1 unit is.01mm)	
	rect.right = rect.right * MMHIMETRIC_TO_MMTEXT_SCALEFACTOR ; //     
	rect.bottom = rect.bottom * MMHIMETRIC_TO_MMTEXT_SCALEFACTOR ; //    
	
	
	pMetaFileDC->SetWindowOrg(0,0);		
	pMetaFileDC->SetWindowExt(rect.right, rect.bottom); 	
	//draw to the MetaFile DC
	OnDraw(pMetaFileDC);
	
	//get a metafile handle and pass it to clipboard, do not delete 
	//this handle later, it will be owned by the clipboard
	HMETAFILE hMetaFile = pMetaFileDC->Close(); 
	VERIFY(hMetaFile);
	            
	//now that we have the hMetaFile, we store it and determine its size using
	//a METAFILEPICT structure.  And, of course, allocate this structure with
	//GlobalAlloc because the clipboard needs a global handle.            
	HGLOBAL hMetaFilePict = NULL;
	METAFILEPICT* hpv_MetaFilePict = NULL;
	VERIFY(hMetaFilePict = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, sizeof(METAFILEPICT))); 
	VERIFY(hpv_MetaFilePict = (METAFILEPICT*) GlobalLock(hMetaFilePict)); 
	                                     
	hpv_MetaFilePict->mm = MM_ISOTROPIC;
	hpv_MetaFilePict->xExt = rect.right;
	hpv_MetaFilePict->yExt = rect.bottom;
	hpv_MetaFilePict->hMF = hMetaFile;	
	VERIFY(!GlobalUnlock(hMetaFilePict));

	
	//Open, Erase, Write to and Close the Clipboard 
	HWND hMainWindow = NULL;
	VERIFY(hMainWindow = AfxGetApp()->m_pMainWnd->GetSafeHwnd());
	if( ::OpenClipboard(hMainWindow) )
	{
		VERIFY(EmptyClipboard());
		VERIFY(SetClipboardData(CF_METAFILEPICT, hMetaFilePict));
		VERIFY(CloseClipboard());
	}
	else
	{
		//Cannot open clipboard, in case where an other application would not have closed clipboard
		//ERR_HANDLER_SHOWWARNING(IDS_ERR_OPENING_CLIPBOARD); 
		AfxMessageBox(L"Error, Cannot Open Clipboard");
		GlobalFree(hMetaFilePict);
		
	}                
	//delete the DC pointers                
	delete(pMetaFileDC);
	ReleaseDC(pDC); 
	return;
}

void COWGraphView::OnEditCut()
{
	OnEditCopy();
}
 
 
 





  
//////////////////////////////////// 
// Printing and Previewing Graph
//
// these following member functions
// can be used to customize the 
// rendering of the printout, in 
// case the printout needs to differ
// from the screen rendering.
////////////////////////////////////
  BOOL COWGraphView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->SetMaxPage(1);       // one page printing/preview
	return DoPreparePrinting(pInfo);
}

void COWGraphView::Print()
{
	OnFilePrint();
}

void COWGraphView::Preview()
{
	OnFilePrintPreview();
}

void COWGraphView::OnPrint(CDC* pDC, CPrintInfo* )
{    
	OnDraw(pDC);
}
    
void COWGraphView::OnFilePrint()
{	  
	CScrollView::OnFilePrint();
}

void COWGraphView::OnFilePrintPreview()
{ 
	CScrollView::OnFilePrintPreview();
}
    
    
    


///////////////////////////////////// 
// Creating, Opening and Saving Graph
/////////////////////////////////////
void COWGraphView::OnFileNew()
{
}
    




    
/////////////////////////////////////
// processing of mouse messages
/////////////////////////////////////
                                               
                                               
///////////////////////////////////////////////
//Left Button, 1 - zoom in (until maximum zoom)
///////////////////////////////////////////////
void COWGraphView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// zoom in (y-only zoom)
	
	CScrollView::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////
//Right Button, 1 - zoom out (if zoom in was previously performed)
//              2 - popup floating menu to set channel visibility
//////////////////////////////////////////////////////////////////
void COWGraphView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect ((LPRECT)&rc); 
	
    COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument();
	int isize = pDoc->m_GraphSetList.GetSize();
	
	// check if click in the graph window AND more than one graph set
	if (rc.PtInRect (point) && isize>1)
	{   
		//popup the floating menu
		HandlePopupMenu (point);
	}	
	
	CScrollView::OnRButtonDown(nFlags, point);
}



//////////////////////////////////////
// COWGraphView floating popup menu
//
// the popup menu is ised to set
// the graphsets visibility attributes
//////////////////////////////////////
void COWGraphView::HandlePopupMenu(CPoint point)
{   
	CMenu popup;
	int nb;
	
    // Get the menu for the popup from the resource file. 
    //if (!pop.LoadMenu (IDR_IMG_POPUP))
 	//	return; 
 	
 	//1) create empty popupmenu 
 	popup.CreatePopupMenu();
 	//2) for each graphset present, add a menu item stating if visible or not
 	
 	popup.AppendMenu(MF_DISABLED, 0, L"Visibility");
 	popup.AppendMenu(MF_SEPARATOR);
                       
    CString string("");
    UINT nStateFlag = 0;                    
    COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument();
    int nSet = 0;			    
    COWGraphSet* p_currentGraphSet = NULL; 
	int indexend = pDoc->m_GraphSetList.GetSize();
	if(!indexend) 
	{   
		popup.DestroyMenu();
		return;
	}	
	for(int index=0; index<indexend; index++)
	{                                                
		//for each COWGraphSet in the m_GraphSetList
		p_currentGraphSet = (COWGraphSet*) pDoc->m_GraphSetList.GetAt((int)index);
		
		//if not default name, set the user specified name
		if(p_currentGraphSet->GetGraphSetName()[0] != 0) 
		{ 
			//set with user specified name
			string = p_currentGraphSet->GetGraphSetName();
		} 
		else
		{    
			//set with default channel name
							
			if(p_currentGraphSet->GetChannelID()>0
			   && p_currentGraphSet->GetChannelID()<MAXCHANNELNUMBER
			   && index<MAXCHANNELNUMBER)
			{   
				//in this case, we use the channelid number (default 1,2,3 is Red,Green,Blue)
				string = channelnameTable[(p_currentGraphSet->GetChannelID())]; 
			}
			else if(p_currentGraphSet->GetChannelID()==0
					&&index<MAXCHANNELNUMBER)
			{   
				//in this case, we use the index number (default 0,0,0,0 is Ch1,Ch2,Ch3,Ch4)								
				string = channelnumberTable[index]; 
			}
			else
			{
			}

        }
		if(p_currentGraphSet->IsVisible())
		{
			nStateFlag = MF_CHECKED;          
        }
        else
        {   
        	nStateFlag = MF_UNCHECKED;
        }
        //create a new menu item
 		popup.AppendMenu(nStateFlag, ((UINT)ID_BASE_POPUP_GRAPHMENU0 + (UINT)index), string); 
 		VERIFY(index<MAXCHANNELNUMBER);
    }

    //here's a little cross check         
	nb = popup.GetMenuItemCount();
	ASSERT( nb==(indexend+2) ); 
		
    // Convert the mouse point to screen coordinates since that is what
    // TrackPopup expects.
    ClientToScreen((LPPOINT)&point);
 	
    // Draw and track the "floating" popup 
    popup.TrackPopupMenu ((TPM_LEFTALIGN || TPM_LEFTBUTTON), point.x, point.y, this, NULL);
    
    // Destroy the menu since we are done with it. 
    popup.DestroyMenu();
}


/////////////////////////////////////////////////////////////////////
//this function will change the visibility of the specified graphset.
//if graphset was visible, it will disable it and vis-versa.
/////////////////////////////////////////////////////////////////////
BOOL COWGraphView::OnSetVisibility(UINT nID)
{	        
	nID = nID - (UINT)ID_BASE_POPUP_GRAPHMENU0;
	//nID now correspond to the graphset's index, range from 0 to ...
	
    COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument();
	pDoc->ModifyVisibilityOfGraphSet(nID);
    if(pDoc->GetNumberOfVisibleSets()==0)
    {   
    	//if no more graphset left to display, undo the present command (and warn the user?)
    	pDoc->ModifyVisibilityOfGraphSet(nID);
    }
    if(pDoc->GetNumberOfVisibleSets()>MAXCHANNELNUMBER)
    {
    	//if more visible set than the limit imposed by COWGraphView, undo the present command
    	//and warn the user.
    	pDoc->ModifyVisibilityOfGraphSet(nID); 
    	WCHAR text[255];                                                             
    	int imax = MAXCHANNELNUMBER;
    	wsprintf(text, L"You cannot have more than %d visible sets at one time", imax); 
    	MessageBox(L"OIII's Graph Module", text, MB_OK);
    }
	pDoc->UpdateAllViews(NULL);
	return TRUE;	
}


BOOL COWGraphView::OnViewZoomIn(UINT nID)
{   
	switch(nID)
	{
		case ID_VIEW_ZOOM_IN_11: 
			m_zoomfactor = 1L;
			break;            
			
		case ID_VIEW_ZOOM_IN_21:
			m_zoomfactor = 2L; 
			break; 
			
		case ID_VIEW_ZOOM_IN_31: 
			m_zoomfactor = 3L; 
			break;
			
		case ID_VIEW_ZOOM_IN_41: 
			m_zoomfactor = 4L; 
			break;
		case ID_VIEW_ZOOM_IN_51: 
			m_zoomfactor = 5L;
			break;            
			
		case ID_VIEW_ZOOM_IN_61:
			m_zoomfactor = 6L; 
			break; 
			
		case ID_VIEW_ZOOM_IN_71: 
			m_zoomfactor = 7L; 
			break;
			
		case ID_VIEW_ZOOM_IN_81: 
			m_zoomfactor = 8L; 
			break;
	}  
	OnUpdateMenu();
	Invalidate();							
	return TRUE;
}



void COWGraphView::OnSetFocus(CWnd* pWnd)
{          
	m_HasFocus = TRUE;
	Invalidate();							
	CScrollView::OnSetFocus(pWnd);
	return;
}
 

void COWGraphView::OnKillFocus(CWnd* pWnd)
{          
	m_HasFocus = FALSE;
	//erase the vertical bar on the graph view  
	/*
	CDC* pDC = GetDC();	
	if(m_nPrevVBarPos > 0)
	{   
		DrawVBar(pDC, m_nPrevVBarPos, m_nPrevVBarOffset, m_nPrevVBarHeight); 
	}  
	ReleaseDC(pDC);
	*/
	CScrollView::OnKillFocus(pWnd);
	return;
}


int COWGraphView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	//we intercept the onMouseActivate message (if no COWGraphMDIChildWnd present)
	COWGraphMDIChildWnd* pMDIFrame = (COWGraphMDIChildWnd*)GetParentFrame();	
	if(!pMDIFrame->IsKindOf(RUNTIME_CLASS(COWGraphMDIChildWnd))) return TRUE;

	return CScrollView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

BOOL COWGraphView::OnEraseBkgnd(CDC* pDC) 
{
	COWGraphDoc* pGraphDoc = (COWGraphDoc*)GetDocument();

	/* not fully functional yet
	//check if should skip erasing
	if(m_lgraphtype&GRAPH_VBAR || m_lgraphtype&GRAPH_HBAR)
	{
		BOOL bVert = TRUE;
		if(m_lgraphtype&GRAPH_VBAR) bVert = TRUE;
		  else if(m_lgraphtype&GRAPH_HBAR) bVert = FALSE;
		struct tagGRAPHRENDERINGATTRIBUTES myRenderingAttrib;
		myRenderingAttrib = ComputeBarGraphRenderingAtributes(pDC, bVert);
		if(myRenderingAttrib.bSkipDrawing == TRUE)
		{
			//do not erase previous graph rendering
			//Invalidate(FALSE);
			return TRUE;
		}
	}
	*/

	//erase background
	m_crBackgroundColor = pGraphDoc->m_crDefaultBackgroundColor;
	m_crTextColor = pGraphDoc->m_crDefaultTextColor;
	COLORREF crOldBackgroundColor = pDC->SetBkColor( m_crBackgroundColor );
	COLORREF crOldTextColor = pDC->SetTextColor( m_crTextColor );
	CRect myRect;
	GetClientRect(&myRect);
	pDC->FillSolidRect( &myRect, m_crBackgroundColor);
	BOOL bResult = TRUE; //erased successfull
	pDC->SetTextColor(crOldTextColor);
	pDC->SetBkColor(crOldBackgroundColor);

//	BOOL bResult = CScrollView::OnEraseBkgnd(pDC);
	return bResult;
}

COLORREF COWGraphView::GetGraphSetColor(class COWGraphSet* pGraphSet, int nDefaultIndex)
{
	COLORREF crColor;
	if(pGraphSet->GetColor()!=GRAPH_DEFAULT_COLOR) 
	{ 
		//set to user specified color
		if(pGraphSet->m_bUseColorTable)  
		{
			if(m_bUseDefaultGrayColorTable) crColor = colorGrayTable[pGraphSet->GetColor()];
			else crColor = colorTable[pGraphSet->GetColor()];
			ASSERT(m_bUseDefaultGrayColorTable!=m_bUseDefaultMultiColorTable);
		}
		else
		{
			crColor = pGraphSet->GetColor();
		}
	} 
	else
	{    
		//set with supplied default color index 
		if(m_bUseDefaultGrayColorTable) crColor = colorGrayTable[nDefaultIndex];
		else crColor = colorTable[nDefaultIndex];
	}
	return crColor;
}
