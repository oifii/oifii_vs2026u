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

//////////////////////////////////////////////////////
// logmdi.cpp : behavior for the class CLogMDIChildWnd
//
// Stephane Poirier, jan 1996
//////////////////////////////////////////////////////

#include "stdafx.h" 
#include "oifiilib.h" //spi 2014
#include "logmdi.h"       
  
       
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
       
       
IMPLEMENT_DYNCREATE(CLogMDIChildWnd, CMDIChildWnd)

CLogMDIChildWnd::CLogMDIChildWnd()
{
}

CLogMDIChildWnd::~CLogMDIChildWnd()
{   
}

  
BEGIN_MESSAGE_MAP(CLogMDIChildWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CLogMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CLogMDIChildWnd::PreCreateWindow(CREATESTRUCT& cs)
{  
	//1) set frame dimension to some fraction of graphic device size.
    CDC* pDC = AfxGetApp()->m_pMainWnd->GetDC();
	cs.cx = (pDC->GetDeviceCaps(HORZRES))/3;
	cs.cy = (pDC->GetDeviceCaps(VERTRES))/2;
    AfxGetApp()->m_pMainWnd->ReleaseDC(pDC);
         
    //2) compute initial frame position
    CMDIFrameWnd* pMDIParentFrameWnd = (CMDIFrameWnd*) AfxGetApp()->m_pMainWnd;
    CRect myRect;
    pMDIParentFrameWnd->GetClientRect(&myRect);
    cs.x = myRect.right - cs.cx - 5;
    if( cs.x < 0 ) cs.x = 0;
    // get the height of the title bar
	int ntitlebarheight = GetSystemMetrics( SM_CYCAPTION ) - GetSystemMetrics( SM_CYBORDER ) + 5;
    cs.y = myRect.bottom - cs.cy - 2*ntitlebarheight;
    if( cs.y < 0 ) cs.y = 0;
	
	//poirier august 1999, begin
	//cs.hwndParent = NULL;
	//poirier august 1999, end
	return CWnd::PreCreateWindow(cs);
}

int CLogMDIChildWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1) return -1;
	return 0;
}
  
  
void CLogMDIChildWnd::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	return;
}

BOOL CLogMDIChildWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//pDC->SetBkColor(RGB(0,0,0));
	return CMDIChildWnd::OnEraseBkgnd(pDC);
}

HBRUSH CLogMDIChildWnd::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	//ASSERT(nCtlColor==CTLCOLOR_EDIT);
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetBkColor(RGB(0, 0, 0));
	return (HBRUSH) GetStockObject(BLACK_BRUSH);
}
