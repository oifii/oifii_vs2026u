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

// ow2mdi.cpp : implementation file
//

#include "stdafx.h"
#include "oifiilib.h" //spi 2014
#include "oifii_app.h"

#include "resource.h"
#include "oifii_gotodlg.h"
#include "ow2mdi.h"

#include "oifii_propsheet.h"

//2012august23, poirier, begin
#include "portaudio.h"
#include "pa_asio.h"
#include <map>
#include <string>
using namespace std;
//2012august23, poirier, end
#include "oifii_mainfrm.h"

#include "c_pointset.h"
#include "owpointset.h"
#include "OWdoc.h"
#include "OW2doc.h"
#include "OWview.h"

#include "OWpixelset.h"
#include "OW2view.h"

#include "OWedgeset.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static UINT BASED_CODE indicatorsImgFrm2P[] =
{
	ID_OW2MDIFRAME_INDCR_ZERO,
	ID_OW2MDIFRAME_INDCR_ONE,                
};
static UINT BASED_CODE indicatorsImgFrm3P[] =
{
	ID_OW2MDIFRAME_INDCR_ZERO,
	ID_OW2MDIFRAME_INDCR_ONE,                
    ID_OW2MDIFRAME_INDCR_TWO,
};


/////////////////////////////////////////////////////////////////////////////
// COW2MDI

IMPLEMENT_DYNCREATE(COW2MDI, CMDIChildWnd)

COW2MDI::COW2MDI()
{
	m_nPanes = SB_PANE_TWO;
	m_itriseed = 0;
}

COW2MDI::~COW2MDI()
{
}


BEGIN_MESSAGE_MAP(COW2MDI, CMDIChildWnd)
	//{{AFX_MSG_MAP(COW2MDI)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_MDIACTIVATE()
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_OW2MDIFRAME_INDCR_ZERO, OnUpdateKeyIndicator)
	ON_UPDATE_COMMAND_UI(ID_OW2MDIFRAME_INDCR_ONE, OnUpdateKeyIndicator)
	ON_UPDATE_COMMAND_UI(ID_OW2MDIFRAME_INDCR_TWO, OnUpdateKeyIndicator)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COW2MDI message handlers

BOOL COW2MDI::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	// call the base class
	return CMDIChildWnd::PreCreateWindow(cs);

	/*
	//i.e.
	cs.style &= ~WS_CHILD;
	//cs.style &= WS_POPUPWINDOW;
	cs.style &= WS_OVERLAPPEDWINDOW;
	return CFrameWnd::PreCreateWindow(cs);
	*/
}

void COW2MDI::RecalcLayout(BOOL bNotify) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CMDIChildWnd::RecalcLayout(bNotify);
}

void COW2MDI::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CMDIChildWnd::OnGetMinMaxInfo(lpMMI);
}

void COW2MDI::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	ResizeStatusBar();
	
	//poirier, sept 2001, begin
	int iShiftKey = GetKeyState(VK_SHIFT);
	if(iShiftKey<0) //shift key is down when higher order bit is set to 1, that is when iShiftKey < 0
	{
		CView* pActiveView = GetActiveView();
		if (pActiveView!=NULL && pActiveView->IsKindOf(RUNTIME_CLASS(COW2View)))
		{
			COW2View* pView = (COW2View*)pActiveView;
			if (pView->GetZoomMode()==COWView::OW_ZOOMNORMAL) 
			{
				CRect rect(pView->GetDocument()->GetWorldRect()); //whole image
				//double f = pView->GetZoomFactor();
				//int nPercent = (int)(100*f);

				if(rect.Width()!=0 && rect.Height()!=0)
				{
					double x = (int)(cx/(rect.Width()));
					double y = (int)(cy/(rect.Height()));
					if(x>=1.0 && y>=1.0)
					{
						int nPercent = (int) (100 * x);
						if(y<x)
						{
							//take smallest so it does not overzoom
							nPercent = (int) (100 * y);
						}
						pView->ZoomPercent(nPercent);
					}
				}
			}
		}
		//poirier, sept 2001, end
	}
}

void COW2MDI::ResizeStatusBar()
{
	//since OnSize seems to be called during the creation process
	if(m_stBarImgFrm.GetSafeHwnd()==NULL) return;

	CRect rectSBClient;
	m_stBarImgFrm.GetClientRect(&rectSBClient);

	UINT nID, nStyle;
	int cxWidth;
	/*
	int nDivider = (m_nPanes == SB_PANE_TWO) ? 2 : 3;
	int nWidthPane0 = rectSBClient.Width() / nDivider;
	int nWidthPane1 = rectSBClient.Width() / nDivider;
	*/
	int nDivider, nWidthPane0, nWidthPane1, nWidthPane2;
	if(m_nPanes == SB_PANE_TWO)
	{
		nDivider = 5; //20% for the first pane	
		nWidthPane0 = rectSBClient.Width() / nDivider;
		nWidthPane1 = rectSBClient.Width() - nWidthPane0;
	}
	else if (m_nPanes == SB_PANE_THREE)
	{
		nDivider = 5; //20% for the first pane	
		nWidthPane0 = rectSBClient.Width() / nDivider;
		int n1and2 = rectSBClient.Width() - nWidthPane0;
		nWidthPane1 = n1and2 / 2;
		nWidthPane2 = n1and2 / 2;
		m_stBarImgFrm.GetPaneInfo( SB_PANE_TWO, nID, nStyle, cxWidth);
		m_stBarImgFrm.SetPaneInfo( SB_PANE_TWO, nID, nStyle, nWidthPane2);

    }
	m_stBarImgFrm.GetPaneInfo( SB_PANE_ZERO, nID, nStyle, cxWidth);
	m_stBarImgFrm.SetPaneInfo( SB_PANE_ZERO, nID, nStyle, nWidthPane0);

	m_stBarImgFrm.GetPaneInfo( SB_PANE_ONE, nID, nStyle, cxWidth);
	m_stBarImgFrm.SetPaneInfo( SB_PANE_ONE, nID, nStyle, nWidthPane1);

	RecalcLayout();	
}

////////////////////////////////////////////////////////////////////////////////////
// Because SetWindowPos takes NET window sizes, therefore we have to take under 
//  consideration the sizes of the title bar, window frames, etc.  
//  We also have to take care for the size of the frame having in mind
//  its position in the AppMainWnd, the presence of StatusBar etc. 
void COW2MDI::CalcFrameSize( CSize* sizeDoc )
{
	// sizeDoc is the actual Image size, therefore recommended frame size.
	//  We shell see whether this size is acceptable or it needs corrections.
	ASSERT( sizeDoc->cx >= 0 ||  sizeDoc->cy >= 0 );
    
    // get the height of the title bar and add it to the sizeDoc.cy
	int inflateY = GetSystemMetrics( SM_CYCAPTION ) -
					GetSystemMetrics( SM_CYBORDER );
	// add the height of the window frame
	inflateY += 2*GetSystemMetrics( SM_CYFRAME ) * 2; //*2 added
	// get the width of the window frame
	int inflateX = 2*GetSystemMetrics( SM_CXFRAME ) *2; //*2 added

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
	CStatusBar* pStatusBar = (CStatusBar*)
		((COIIIApp*)AfxGetApp())->m_pMainWnd->GetDescendantWindow(IDR_MAINFRAME_STATUS_BAR);//ID_INSPTR_MAINFRM_STATUS_BAR);

	CRect rectMainFrmSBar(0, 0, 0 ,0 );
	if( pStatusBar != NULL && (pStatusBar->GetStyle() & WS_VISIBLE))
		pStatusBar->GetWindowRect( &rectMainFrmSBar );
	// take the height of the tool bar - don't need it anymore
//	CToolBar* pToolBar = (CToolBar*)
//		theApp.m_pMainWnd->GetDescendantWindow(AFX_IDW_TOOLBAR);
//	CRect rectToolBar;
//	if( pToolBar != NULL && (pToolBar->GetStyle() & WS_VISIBLE));
//		pToolBar->GetWindowRect( &rectToolBar );

	// we need the coord. of the upper left corner of the frame window
	//   and we need them in App. client area coordinates
	CRect rectFrame;
	GetWindowRect(&rectFrame);
	((COIIIApp*)AfxGetApp())->m_pMainWnd->ScreenToClient(&rectFrame);
	
	// take the coordinates of the application client area
	CRect rectAppClient;
	((COIIIApp*)AfxGetApp())->m_pMainWnd->GetClientRect( &rectAppClient );

	int nWidth = 15 * rectAppClient.Width() / 16;
	int nHeight = rectAppClient.Height() - rectMainFrmSBar.Height() - rectFrame.top;// - rectToolBar.Height();	

	// check if the image sizes are too big
	sizeDoc->cx = ( sizeDoc->cx > nWidth ) ? 
										nWidth : sizeDoc->cx;
    sizeDoc->cy = ( sizeDoc->cy > nHeight) ? 
    									nHeight : sizeDoc->cy;

}


void COW2MDI::SetFrameSizeAccordToDocumentSize(CSize mySize)
{
	CalcFrameSize( &mySize );	

	SetWindowPos(&wndTop,
    			 CW_USEDEFAULT,
    			 CW_USEDEFAULT,
    			 mySize.cx,
    			 mySize.cy,
    			 SWP_NOZORDER | SWP_NOMOVE);
	
	return;
}


void COW2MDI::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	 
	if( bActivate==TRUE || (pActivateWnd!=NULL && !pActivateWnd->IsKindOf(RUNTIME_CLASS(COW2MDI))) )
	{	
		COIIIApp* myApp = (COIIIApp*)AfxGetApp();
		//if goto partition dlg exist, update dlg info
		if(myApp->m_pGotoPartitionDlg!=NULL)
		{	
			myApp->m_pGotoPartitionDlg->PostMessage(OWM_GOTODLG_UPDATEINFO);	
		}
		/* oct 13 2001
		if(myApp->m_pSettingsSheet!=NULL)
		*/
		if(myApp->m_pSettingsSheet!=NULL && myApp->IsSettingsSheetVisible()==TRUE
			&& bActivate==TRUE)
		{
			myApp->m_pSettingsSheet->PostMessage(OWM_SETTINGSSHEET_UPDATEINFO);
			
			COW2Doc* pDoc = (COW2Doc*)GetActiveDocument();
			if(pDoc)
			{
				globalstructCONTEXTINFO.idROI = pDoc->m_pROIset->GetSelectedROI();
				globalstructCONTEXTINFO.idVertex = -1;
				globalstructCONTEXTINFO.idSegment = -1;
				//2020july03, spi, begin
				//myApp->m_pSettingsSheet->PostMessage(OWM_MOUSEMOVED_UPDATELOCALGRAPH, (DWORD)&globalstructCONTEXTINFO);
				myApp->m_pSettingsSheet->PostMessage(OWM_MOUSEMOVED_UPDATELOCALGRAPH, (LONG_PTR)&globalstructCONTEXTINFO);
				//2020july03, spi, end
			}
			
		}
	}
	return;
}

int COW2MDI::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_framealwaysontop==TRUE)
	{
		 lpCreateStruct->dwExStyle |= WS_EX_TOPMOST;
	}

	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_stBarImgFrm.Create(this, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,
											 IDR_OW2MDIFRAME_STATUS_BAR) ||
		!m_stBarImgFrm.SetIndicators(indicatorsImgFrm2P,
		  sizeof(indicatorsImgFrm2P)/sizeof(UINT)))
	{
		AfxMessageBox(L"Failed to create status bar\n");
		return -1;		// fail to create
	}
	// set the font of the status bar to something thiner
	LOGFONT logfont;
	memset(&logfont, 0, sizeof(logfont));
	logfont.lfHeight = 8;
	logfont.lfWeight = FW_THIN;
	wcscpy_s(logfont.lfFaceName, 32,L"MS Sans Serif");      
	VERIFY(m_fontStatusBar.CreateFontIndirect(&logfont));
	m_stBarImgFrm.SetFont(&m_fontStatusBar);

	// to give a beveled border of the first status bar pame
//	m_stBarImgFrm.SetPaneInfo( SB_PANE_ZERO, 0, SBPS_STRETCH | SBPS_NORMAL, 0 );	
	
	if( m_stBarImgFrm.GetStyle() & WS_VISIBLE )
	{
		m_stBarImgFrm.ShowWindow(SW_SHOWNORMAL);
		RecalcLayout();
	}
	
	return 0;
}

void COW2MDI::OnUpdateKeyIndicator(CCmdUI* pCmdUI)
{

//	char szBuffer[SZ_BUF_LENGTH];
	CPoint ptCurPos;
    
    //CPointD floated_position;	
	/*
	double dfX, dfY;			
    double  ZoomFactor;                     					//glachape  for calibration
    */

	// get the pane index (number)
	UINT nIndex = m_stBarImgFrm.CommandToIndex(pCmdUI->m_nID);

	CView* pActiveView = GetActiveView();
	BOOL bEnable = FALSE;
	if (pActiveView != NULL)
		if (pActiveView->IsKindOf(RUNTIME_CLASS(COWView)))
			bEnable = TRUE;

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd(); 

	if (bEnable == FALSE || pMainFrame->GetActiveFrame() != this)
	{
		pCmdUI->Enable(FALSE);
		return;	
	}

	// get and convert cursor position to client co-ordinates
	GetCursorPos(&ptCurPos);
	pActiveView->ScreenToClient(&ptCurPos);
	
	COW2Doc* pDoc = (COW2Doc*)pActiveView->GetDocument();
	CSize sizeDoc;
	sizeDoc.cx = pDoc->GetSizeX();
	sizeDoc.cy = pDoc->GetSizeY();
	// make sure the cursor is over the client area
	CRect rectClient;
	pActiveView->GetClientRect(&rectClient); 
	CPoint ptScrolledAt(((CScrollView*)pActiveView)->GetDeviceScrollPosition());
	
	// correct the rectClient in case the image has been scrolled
	rectClient.OffsetRect(ptScrolledAt);
	// correct the current cursor position as well
	CPoint ptTmp(ptCurPos);
	ptTmp.Offset(ptScrolledAt);		
	if (!rectClient.PtInRect(ptTmp)) 
	{
		pCmdUI->Enable(FALSE);
		return;	
	}		
	// we need the logical coordinates for all OUR calculations
	//  and now is the right time to convert
	CClientDC dc(pActiveView);
	pActiveView->OnPrepareDC(&dc);
	dc.DPtoLP(&ptCurPos);
                        
	// and of course inside the image   					//to be changed for check of floated_position (more precise)
	if (ptCurPos.x < 0 || ptCurPos.y < 0 ||
		ptCurPos.x >= sizeDoc.cx || ptCurPos.y >= sizeDoc.cy )   
	{
		pCmdUI->Enable(FALSE);
		return;	
	}     

	switch (pCmdUI->m_nID)
	{
		case ID_OW2MDIFRAME_INDCR_ZERO:
		{
			// show coordinates		
			/*
			ZoomFactor= ((COWView*)pActiveView)->GetZoomFactor();                    
			*/
			//floated_position.x= ( (double)ptTmp.x) / ZoomFactor;
			//floated_position.y= ( (double)ptTmp.y) / ZoomFactor; 
			/*
			dfX = ( (double)ptTmp.x) / ZoomFactor;
			dfY = ( (double)ptTmp.y) / ZoomFactor; 
			*/
			//TRACE("floated_position: %f %f\n",floated_position.x, floated_position.y);

			//CString str(pDoc->calibration->WorldPosition(floated_position));
			CString myString;
			myString.Format(L"x=%d, y=%d", ptCurPos.x, ptCurPos.y);
			//show pixel intensity value
			if(pDoc->m_pPixelset!=NULL && pDoc->m_pPixelset->IsBIIPresent())
			{
				CString myTempString;
				int iNumChannel = pDoc->GetPixelsetNumberOfChannel();
				for(int i=0; i<iNumChannel; i++)
				{
					int iIntensity = pDoc->GetPixelsetIntensity(i, ptCurPos.x, ptCurPos.y);
					myTempString.Format(L", z%d=%d", i, iIntensity);
					myString += myTempString;
				}
			}
			m_stBarImgFrm.SetPaneText( nIndex, myString); 
			break;
		}
		case ID_OW2MDIFRAME_INDCR_ONE:
		{
			if(pDoc->m_pPointset!=NULL)
			{
				CString myString = "";
				POINT2DD myPoint2DD = ((COW2View*)pActiveView)->MapPointFromViewToWorld(ptCurPos);
				int idObject = pDoc->GetNearestPointsetObject(myPoint2DD.x, myPoint2DD.y, &m_itriseed); 
				if(idObject<0) 
				{
					myString = "";
				}
				else 
				{
					//display all stats in pane
					/*
					myString = pDoc->GetPointsetObjectStatAsString(idObject, -1);
					m_stBarImgFrm.SetPaneText( nIndex, myString); 
					*/
					//2022jan03, spi, begin
					if (1) //for debugging
					{
						CString myTempString;
						myTempString.Format(L"%d", idObject);
						myString = "id=" + myTempString + ", ";
					}
					//myString = pDoc->GetPointsetObjectStatAsString(idObject, POINTSET_OFFSETSTAT_VOROAREA);
					//display a number of stats in pane
					myString += pDoc->GetPointsetObjectStatAsString(idObject, POINTSET_OFFSETSTAT_VOROAREA);
					//2022jan03, spi, end
					myString += ", ";
					myString += pDoc->GetPointsetObjectStatAsString(idObject, POINTSET_EXTRA_NUMBEROFSTAT + POINTSET_XYZI_INTENSITY);
					myString += ", ";
					myString += pDoc->GetPointsetObjectStatAsString(idObject, POINTSET_EXTRA_NUMBEROFSTAT + POINTSET_XYZI_TREEAREA);

					m_stBarImgFrm.SetPaneText( nIndex, myString); 

				}
			}
			// show the color values		
			/* sp
			((ImageView*)pActiveView)->ShowColorValues( &m_stBarImgFrm, SB_PANE_ONE, ptCurPos);
			*/
			break;
		}     
		case ID_OW2MDIFRAME_INDCR_TWO:
		{

			/* sp
			// if we draw, let the drawing tools to display status bar info		
			if (((ImageView*)pActiveView)->GetDrawingMode() == TRUE)
				break;
			else
			{
				// otherwise let the graphic object to display it's own info
				// get the list
				CGraphObj* pGraphObj;
		
				// check out whether there is a selected figure
				for (POSITION pos = pDoc->GetFirstGraphObjPos(); pos != NULL; )
				{
					pGraphObj = pDoc->GetNextGraphObj(pos);

					if (pGraphObj->IsSelected() == TRUE)
					{				
						pGraphObj->ShowGraphObjInfo(&m_stBarImgFrm, SB_PANE_TWO, ptCurPos);
						break;	
					}
				}
				break;
			}
			*/
		}
		default:
			TRACE1("Warning: OnUpdateKeyIndicator - unknown indicator 0x%04X\n",
				pCmdUI->m_nID);
			pCmdUI->ContinueRouting();

		return; // not for us
	}

//	pCmdUI->Enable(::GetKeyState(nVK) & 1);
	pCmdUI->Enable(TRUE);
		// enable static text based on toggled key state
	ASSERT(pCmdUI->m_bEnableChanged);
}


void COW2MDI::ChangeSBPanes(int nPanes)
{
	if (m_nPanes == nPanes)
		return;
	
	switch (nPanes)
	{
	default:
		ASSERT(FALSE);
	case SB_PANE_TWO:
		m_stBarImgFrm.SetIndicators(indicatorsImgFrm2P,
			sizeof(indicatorsImgFrm2P)/sizeof(UINT));
		m_nPanes = SB_PANE_TWO;
		break;
	case SB_PANE_THREE:
		m_stBarImgFrm.SetIndicators(indicatorsImgFrm3P,
			sizeof(indicatorsImgFrm3P)/sizeof(UINT));
		m_nPanes = SB_PANE_THREE;
		break;
	}
	ResizeStatusBar();
	
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//  class COW2ZoomedMDI
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(COW2ZoomedMDI, COW2MDI)

BEGIN_MESSAGE_MAP(COW2ZoomedMDI, COW2MDI)
	//{{AFX_MSG_MAP(COW2ZoomedMDI)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

COW2ZoomedMDI::COW2ZoomedMDI()
{
}
COW2ZoomedMDI::~COW2ZoomedMDI()
{
}

BOOL COW2ZoomedMDI::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	//cs.dwExStyle = cs.dwExStyle | WS_EX_TOPMOST;
	//cs.dwExStyle = cs.dwExStyle | WS_DLGFRAME;
	if(cs.style && WS_VISIBLE)
	{
		//cs.style = (cs.style)&(!WS_VISIBLE);   
	}

	//poirier, aout 2002, begin
	
	return COW2MDI::PreCreateWindow(cs);
	
	/*
	//i.e.
	cs.style &= ~WS_CHILD;
	//cs.style &= WS_POPUPWINDOW;
	cs.style &= WS_OVERLAPPEDWINDOW;
	return CFrameWnd::PreCreateWindow(cs);
	*/
	//poirier, aout 2002, end
}

int COW2ZoomedMDI::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COW2MDI::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//poirier, aout 2002, begin
	/*
	SetParent(NULL);
	*/
	//poirier, aout 2002, end

	return 0;
}

void COW2ZoomedMDI::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	COW2MDI::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

void COW2ZoomedMDI::OnSize(UINT nType, int cx, int cy) 
{
	/*
	if(nType==SIZE_MAXIMIZED)
	{
		return;
	}
	*/

	COW2MDI::OnSize(nType, cx, cy);
	return;
}
