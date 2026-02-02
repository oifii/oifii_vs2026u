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

//////////////////////////////////////////////////////////
// logview.cpp : behavior for the class CLogEditView
//
// Stephane Poirier, jan 1996
//////////////////////////////////////////////////////////

#include "stdafx.h"
#include "oifiilib.h" //spi 2014
#include "logview.h"
#include "logdoc.h"
#include "logmdi.h"      
      
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
 
                 
/////////////////                 
// private macros 
/////////////////



CLogEditView::CLogEditView()
{ 
}

CLogEditView::~CLogEditView()
{
}

IMPLEMENT_DYNCREATE(CLogEditView, CEditView)

BEGIN_MESSAGE_MAP(CLogEditView, CEditView)
	//{{AFX_MSG_MAP(CLogEditView)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear) 
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
     
       
       
void CLogEditView::OnUpdateEditClear(CCmdUI* pCmdUI)
{
	//ID_EDIT_CLEAR is defined as ID_EDIT_DELETE which is always disabled when no selection.
	//since we want to override this default behavior in order to always have EDIT_CLEAR available.  
	pCmdUI->Enable(TRUE);	                 
}     
void CLogEditView::OnEditClear()
{   
	//1) get document pointer
	CLogDocument* pResultLogDoc = (CLogDocument*) GetDocument(); 
	pResultLogDoc->ClearAllText();
	return;
}
       
void CLogEditView::OnInitialUpdate()
{                                                      
	//1) call the bass-class OnInitialUpdate();
	CView::OnInitialUpdate(); 
	
	//2) set edit view to read-only
    VERIFY(	GetEditCtrl().SetReadOnly(TRUE));
    //GetEditCtrl().SetParent(this);
    //GetEditCtrl().SubclassWindow(this->GetSafeHwnd());  

     
	//3) set the font
	LOGFONT logfont;
	memset(&logfont, 0, sizeof(logfont));
	logfont.lfHeight = 16;
	logfont.lfWeight = FW_NORMAL;  //FW_THIN
	wcscpy(logfont.lfFaceName, L"Helvetica");  //MS Sans Serif    
	VERIFY(m_Font.CreateFontIndirect(&logfont));
	GetEditCtrl().SetFont(&m_Font);
	
	return;
}     
     

void CLogEditView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{                                                                         
	//1) get document pointer
	CLogDocument* pResultLogDoc = (CLogDocument*) GetDocument(); 
	
	//2) set the editview's text buffer according to what's in the document
	GetEditCtrl().SetWindowText( (pResultLogDoc->m_BufferString) );  
	
	//3) set scroll position to display end of text buffer
	int nTotalLine = GetEditCtrl().GetLineCount();
	CRect myRect;
	GetClientRect((LPRECT)&myRect);
	CDC* pDC = GetDC();
	CSize mySize = pDC->GetTextExtent(L"A", 1); 
	int nVisibleLine = (int)(myRect.Height()/mySize.cy); 
	ReleaseDC(pDC);
	GetEditCtrl().LineScroll(nTotalLine-nVisibleLine, 0);  
	return;
}

     
/////////////////////////////
// CLogEditView drawing
/////////////////////////////
void CLogEditView::OnDraw(CDC* /*pDC*/)
{       
	// there is no drawing to be performed, the CEdit text display is automaticaly 
	// handled by the CEdit control.
}
