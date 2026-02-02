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
//#include "datamdi.h"
//#include "graphmdi.h"
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

IMPLEMENT_DYNCREATE(COWGraphDataMDIChildWnd, CMDIChildWnd)

COWGraphDataMDIChildWnd::COWGraphDataMDIChildWnd()
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

COWGraphDataMDIChildWnd::~COWGraphDataMDIChildWnd()
{   
}


BEGIN_MESSAGE_MAP(COWGraphDataMDIChildWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(COWGraphDataMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_MDIACTIVATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



int COWGraphDataMDIChildWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	 
	 
	return 0;
}



void COWGraphDataMDIChildWnd::SetFrameSize( CSize sizeGraphWindow )
{   
	// compute the frame size
	//ImageFrame::CalcFrameSize( &sizeGraphWindow );	
    
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



void COWGraphDataMDIChildWnd::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	return;
}
