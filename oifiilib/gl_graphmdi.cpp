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
//#include "graphdoc.h"
//#include "graphmdi.h"
//#include "graphvw.h"
//#include "private.h"     
     
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
       
       
static UINT BASED_CODE myIndicators[] =
{
	ID_SEPARATOR,			// status line indicator
	ID_SEPARATOR,
	ID_SEPARATOR
};
       

// static data used to store wndframe origin position offset, see constructor and
// COWGraphMDIChildWnd::SetFrameSize(). 
static int nframeoffset=-1;
       
/////////////////////////////////////////////////////////////////////////////
// COWGraphMDIChildWnd

IMPLEMENT_DYNCREATE(COWGraphMDIChildWnd, CMDIChildWnd)

COWGraphMDIChildWnd::COWGraphMDIChildWnd()
{   
	// checking wether to reset or increment the static data member m_nframeposition.
	// this data member is used to store the offset of the initial frame position.
	if(nframeoffset<0 || nframeoffset>3)
	{
		nframeoffset = 0;	
	}
	else
	{   
		nframeoffset++;
	}
}

COWGraphMDIChildWnd::~COWGraphMDIChildWnd()
{   
}


BEGIN_MESSAGE_MAP(COWGraphMDIChildWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(COWGraphMDIChildWnd)
	ON_COMMAND(ID_VIEW_STATUS_BAR, OnViewStatusBar)
	ON_WM_CREATE()
	ON_WM_MDIACTIVATE()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COWGraphMDIChildWnd message handlers


void COWGraphMDIChildWnd::OnViewStatusBar()
{
	// TODO: Add your command handler code here
	
}

int COWGraphMDIChildWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//does not work
	//lpCreateStruct->hbrBackground = GetStockObject(BLACK_BRUSH);

	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	 
	//create and show the StatusBar                                    
	//(the StatusBar must have the MDIFrameWnd for parent)                   
	if (!m_stBarImgFrm.Create( this, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM, 
						   ID_GRAPHVIEW_STATUSBAR )
		|| !m_stBarImgFrm.SetIndicators( myIndicators,sizeof(myIndicators)/sizeof(UINT) ))
	{
		TRACE("Failed to create status bar\n");
		return -1; //fail to create
	}
	if( m_stBarImgFrm.GetStyle() & WS_VISIBLE )
	{
		m_stBarImgFrm.ShowWindow(SW_SHOWNORMAL);
		this->RecalcLayout();
	}
	// set info for 2nd and third pane	
	m_stBarImgFrm.SetPaneInfo( 0, 0, SBPS_NORMAL, 100 );	
	m_stBarImgFrm.SetPaneInfo( 1, 0, SBPS_NORMAL, 400 );	


	// set the font of the status bar to something thiner
	LOGFONT logfont;
	memset(&logfont, 0, sizeof(logfont));
	logfont.lfHeight = 8;
	logfont.lfWeight = FW_THIN;
	wcscpy(logfont.lfFaceName, L"MS Sans Serif");        // best look 
	VERIFY(m_fontStatusBar.CreateFontIndirect(&logfont));
	m_stBarImgFrm.SetFont(&m_fontStatusBar);
	
	return 0;
}



void COWGraphMDIChildWnd::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	if(bActivate)
	{   
		
		//Only if activating the window, get the view and refresh display
	    COWGraphView* pGraphView = (COWGraphView*) GetActiveView(); 
	    pGraphView->Invalidate(); 
	    
	    //refresh menu state
	    pGraphView->OnUpdateMenu();
	}
	
}

///////////////////////////////////////////////////////////////////////////////
// Because SetWindowPos takes NET window sizes, therefore we have to take under 
//  consideration the sizes of the title bar, window frames, etc.  
//  We also have to take care for the size of the frame having in mind
//  its position in the AppMainWnd, the presence of StatusBar etc. 
///////////////////////////////////////////////////////////////////////////////
void COWGraphMDIChildWnd::CalcFrameSize( CSize* sizeDoc )
{
	// sizeDoc is the actual Image size, therefore recommended frame size.
	//  We shell see whether this size is acceptable or it needs corrections.
	ASSERT( sizeDoc->cx >= 0 ||  sizeDoc->cy >= 0 );
    
    // get the height of the title bar and add it to the sizeDoc.cy
	int inflateY = GetSystemMetrics( SM_CYCAPTION ) -
					GetSystemMetrics( SM_CYBORDER );
	// add the height of the window frame
	inflateY += 2*GetSystemMetrics( SM_CYFRAME );
	// get the width of the window frame
	int inflateX = 2*GetSystemMetrics( SM_CXFRAME );

	sizeDoc->cx += inflateX;        
	sizeDoc->cy += inflateY;
	// take the height of the image frame status bar

	CRect rectImgFrmSBar(0, 0, 0 ,0 );
	if(m_stBarImgFrm.GetSafeHwnd()!= NULL)
	{
		if(m_stBarImgFrm.GetStyle() & WS_VISIBLE)
			m_stBarImgFrm.GetWindowRect( &rectImgFrmSBar );
    }
	sizeDoc->cy += rectImgFrmSBar.Height();

	// now take the height of the mainframe status bar
	CRect rectMainFrmSBar(0, 0, 0 ,0 );
#ifdef IDR_MAINFRAME_STATUS_BAR
	CStatusBar* pStatusBar = (CStatusBar*)
	//((COIIIApp*)AfxGetApp())->m_pMainWnd->GetDescendantWindow(IDR_MAINFRAME_STATUS_BAR);
	(AfxGetApp())->m_pMainWnd->GetDescendantWindow(IDR_MAINFRAME_STATUS_BAR);

	if( pStatusBar != NULL && (pStatusBar->GetStyle() & WS_VISIBLE))
		pStatusBar->GetWindowRect( &rectMainFrmSBar );
#else
	//TODO: estimate statusbar size or skip
#endif //IDR_MAINFRAME_STATUS_BAR

	// we need the coord. of the upper left corner of the frame window
	//   and we need them in App. client area coordinates
	CRect rectFrame;
	GetWindowRect(&rectFrame);
	//((COIIIApp*)AfxGetApp())->m_pMainWnd->ScreenToClient(&rectFrame);
	(AfxGetApp())->m_pMainWnd->ScreenToClient(&rectFrame);
	
	// take the coordinates of the application client area
	CRect rectAppClient;
	//((COIIIApp*)AfxGetApp())->m_pMainWnd->GetClientRect( &rectAppClient );
	(AfxGetApp())->m_pMainWnd->GetClientRect( &rectAppClient );

	int nWidth = 15 * rectAppClient.Width() / 16;
	int nHeight = rectAppClient.Height() - rectMainFrmSBar.Height() - rectFrame.top;// - rectToolBar.Height();	

	// check if the image sizes are too big
	sizeDoc->cx = ( sizeDoc->cx > nWidth ) ? 
										nWidth : sizeDoc->cx;
    sizeDoc->cy = ( sizeDoc->cy > nHeight) ? 
    									nHeight : sizeDoc->cy;

}

void COWGraphMDIChildWnd::SetFrameSize( CSize sizeGraphWindow )
{   
	// compute the frame size
	CalcFrameSize( &sizeGraphWindow );	
    
    // get the height of the title bar
	int ntitlebarheight = GetSystemMetrics( SM_CYCAPTION ) - GetSystemMetrics( SM_CYBORDER ) + 3;
     
     
    // compute initial frame position 
    CDC* pDC = GetDC(); 
	int xpos = (pDC->GetDeviceCaps(HORZRES)) - sizeGraphWindow.cx - 4*ntitlebarheight
				+ (nframeoffset * ntitlebarheight);
    if(xpos < 0) xpos = 0;			
	int ypos = 0 + (nframeoffset * ntitlebarheight);
    ReleaseDC(pDC); 
    
    
	SetWindowPos(&wndTop,
    			 xpos,
    			 ypos,
    			 sizeGraphWindow.cx,
    			 sizeGraphWindow.cy,
    			 0);
    return;
}


void COWGraphMDIChildWnd::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	return;
}


BOOL COWGraphMDIChildWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;

	COWGraphView* pGraphView = (COWGraphView*) GetActiveView(); 
	COWGraphDoc* pDoc = (COWGraphDoc*)pGraphView->GetDocument();

	/*
	//check if should skip erasing
	if(pGraphView->lgraphtype&GRAPH_VBAR || pGraphView->lgraphtype&GRAPH_HBAR)
	{
		BOOL bVert = TRUE;
		if(pGraphView->lgraphtype&GRAPH_VBAR) bVert = TRUE;
		  else if(pGraphView->lgraphtype&GRAPH_HBAR) bVert = FALSE;
		struct tagGRAPHRENDERINGATTRIBUTES myRenderingAttrib;
		myRenderingAttrib = pGraphView->ComputeBarGraphRenderingAtributes(pDC, bVert);
		if(myRenderingAttrib.bSkipDrawing == TRUE)
		{
			//do not erase previous graph rendering
			//Invalidate(FALSE);
			return TRUE;
		}
		//return TRUE;
	}
	*/

	//erase background
	COLORREF crOldTextColor = pDC->SetTextColor(pDoc->m_crDefaultTextColor);
	COLORREF crOldBkColor = pDC->SetBkColor(pDoc->m_crDefaultBackgroundColor);
	CRect myRect;
	GetClientRect(&myRect);
	pDC->FillSolidRect( &myRect, pDoc->m_crDefaultBackgroundColor);
	BOOL bResult = TRUE; //erased successfull
	pDC->SetBkColor(crOldBkColor);
	pDC->SetTextColor(crOldTextColor);
	//BOOL bResult = CMDIChildWnd::OnEraseBkgnd(pDC);
	return bResult;
}

HBRUSH COWGraphMDIChildWnd::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	COWGraphView* pGraphView = (COWGraphView*) GetActiveView(); 
	COWGraphDoc* pDoc = (COWGraphDoc*)pGraphView->GetDocument();
	pDC->SetTextColor(pDoc->m_crDefaultTextColor);
	pDC->SetBkColor(pDoc->m_crDefaultBackgroundColor);
	if(pDoc->m_crDefaultBackgroundColor==RGB(0,0,0))
	  return (HBRUSH) GetStockObject(BLACK_BRUSH);
	else if(pDoc->m_crDefaultBackgroundColor==RGB(255,255,255))
	  return (HBRUSH) GetStockObject(WHITE_BRUSH);
	else
	  return (HBRUSH) GetStockObject(WHITE_BRUSH);

}

