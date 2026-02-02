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

// OW2AGmdi.cpp : implementation file
//

#include "stdafx.h"
#include "oifiilib.h" //spi 2014

//2012august23, poirier, begin
#include "portaudio.h"
//2012august23, poirier, end

#include "OW2mdi.h"
#include "OW2AGmdi.h"
#include "afxpriv.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COW2MDI

/*
static UINT BASED_CODE indicators[] =
{
    ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};
*/

IMPLEMENT_DYNCREATE(COW2AGMDI, COW2MDI)

COW2AGMDI::COW2AGMDI()
{
}

COW2AGMDI::~COW2AGMDI()
{
}


BEGIN_MESSAGE_MAP(COW2AGMDI, COW2MDI)
	//{{AFX_MSG_MAP(COW2AGMDI)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COW2AGMDI message handlers
void COW2AGMDI::MyUpdatePane()
{
	/*
    m_wndStatusBar.SendMessage(WM_IDLEUPDATECMDUI); // Include afxpriv.h 
    m_wndStatusBar.UpdateWindow();
	*/
	m_stBarImgFrm.SendMessage(WM_IDLEUPDATECMDUI); // Include afxpriv.h 
    m_stBarImgFrm.UpdateWindow();
}                                 

void COW2AGMDI::MyChangePane(int nPane, LPCTSTR text )
{ 
	/*
    m_wndStatusBar.SetPaneText(nPane,text,TRUE) ;
    MyUpdatePane();
	*/
    m_stBarImgFrm.SetPaneText(nPane,text,TRUE) ;
    MyUpdatePane();
}


int COW2AGMDI::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COW2MDI::OnCreate(lpCreateStruct) == -1)
		return -1;
	/*
	if (!m_wndStatusBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,
												IDR_OW2MDIFRAME_STATUS_BAR ) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	*/
	return 0;
}


void COW2AGMDI::OnDestroy()
{
	COW2MDI::OnDestroy();

	// TODO: Add your message handler code here
}
