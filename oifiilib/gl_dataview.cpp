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
//#include "dataview.h"
//#include "graphdoc.h"
//#include "graphvw.h"
//#include "private.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
                 
                 
// private macros
#define NUMBEROFCHARACTER_PERTABSTOPS 10


/////////////////////////////
// constructor and destructor
/////////////////////////////
COWGraphDataView::COWGraphDataView()
{   
	m_hTabbedTextBuffer = NULL;  //this buffer is allocated by OnInitialUpdate()
	m_nEachTabStop = NUMBEROFCHARACTER_PERTABSTOPS; //number of character for each tab stops
}

COWGraphDataView::~COWGraphDataView()
{
	if(m_hTabbedTextBuffer!=NULL) GlobalFree(m_hTabbedTextBuffer);	
}


//////////////
// message map
//////////////
IMPLEMENT_DYNCREATE(COWGraphDataView, CEditView)

BEGIN_MESSAGE_MAP(COWGraphDataView, CEditView)
	//{{AFX_MSG_MAP(COWGraphDataView)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
#ifdef OWGRAPH_INTERNAL //when OWGRAPH linked with the application
	#ifdef OIIIAPP //and if the app is OIII Application
	#include "resource.h"
	ON_COMMAND(ID_VIEW_NEWGRAPHVIEW, OnViewGraph)
	#endif //OWGRAPH_INTERNAL && OIIIAPP with OIII Application
#endif
END_MESSAGE_MAP()
     
       
       
///////////////////
// message handlers
///////////////////       
void COWGraphDataView::OnInitialUpdate()
{                                                      
	//call the bass-class OnInitialUpdate();
	CEditView::OnInitialUpdate(); 
	
	//set edit view to read-only
    VERIFY(	GetEditCtrl().SetReadOnly(TRUE));
    
	//set edit view's tabstops to ... 
	WORD DlgWidthUnits = LOWORD(::GetDialogBaseUnits()); 
	int cxEachStop = ( DlgWidthUnits * m_nEachTabStop * 2 ) / 4; //for details see MS Prog. Ref. p.361
    SetTabStops(cxEachStop); //for details see MFC Class Ref. p. 390


	// get graph device size.
    CDC* pDC = GetDC();
    CSize sizeGraphWnd; 
	sizeGraphWnd.cx = (pDC->GetDeviceCaps(HORZRES))/2;
	sizeGraphWnd.cy = (pDC->GetDeviceCaps(VERTRES))/2;
    ReleaseDC(pDC);
         
	// adjust frame dimension to some fraction of graphic device size.
	COWGraphDataMDIChildWnd* pMDIFrame = (COWGraphDataMDIChildWnd*)GetParentFrame();	
	ASSERT(pMDIFrame->IsKindOf(RUNTIME_CLASS(COWGraphDataMDIChildWnd)));
	pMDIFrame->SetFrameSize(sizeGraphWnd);
    
	return;
}     
     

void COWGraphDataView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{                                                                         
    ///////////////////////////////////////////////////
	//pass the tabbedtextbuffer to the CEditView object
	///////////////////////////////////////////////////
	
	//get graphdoc pointer
	COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument(); 
	
	//pass the address of member tabbed text buffer handle, generatetabbedtextbuffer() will
	//allocate the buffer, handle should be freed by the COWGraphDataView's destructor. 
	m_hTabbedTextBuffer = pDoc->GenerateTabbedTextBuffer();
	VERIFY( m_hTabbedTextBuffer!=NULL );
	  
	char* hpv_TabbedTextBuffer = (char*) GlobalLock(m_hTabbedTextBuffer);
	CString myString(hpv_TabbedTextBuffer);
	GetEditCtrl().SetWindowText(myString);
	GlobalUnlock(m_hTabbedTextBuffer);  
	
	CEditView::OnUpdate(pSender, lHint, pHint);
	return;
}

     
///////////////////////////
// COWGraphDataView drawing
///////////////////////////
void COWGraphDataView::OnDraw(CDC* pDC)
{       
	// there is no drawing to be performed, the CEdit text display is automaticaly 
	// handled by the CEdit control.
	CEditView::OnDraw(pDC);
}
  
void COWGraphDataView::OnEditCopy()
{
	//if we want to copy the data to clipboard
	COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument();
	if( !pDoc->CopyGraphSetsToClipboardVerticaly() ) ::MessageBeep(MB_ICONEXCLAMATION); 
	//if( !pDoc->CopyDataToClipboard() ) ::MessageBeep(-1); 
	return;
}

void COWGraphDataView::OnEditPaste()
{ 
	//Pasting data is a feature only supported for testing the graph library
	//ONLY AVAILABLE FOR DEBUG VERSION and with data copied from MS-Excel (each row taken as graphset) 
	//note: on import: each spreadsheet row becomes a graph channel
	#ifdef _DEBUG
	#endif // _DEBUG 	
	COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument();
	pDoc->PasteGraphSetsFromClipboard();  
	return;
}


#ifdef OWGRAPH_INTERNAL	//when OWGRAPH linked with the application
	#ifdef OIIIAPP //and when the app is OIII Application
		#include "oifii_app.h" 
	#endif //OIIIAPP
#endif //OWGRAPH_INTERNAL
//use this function when the data view is activated and you 
//want to activate graph view 
void COWGraphDataView::OnViewGraph()
{	
	/////////////////////////////////////////////////
	// create or activate the graph childwindow frame  
	/////////////////////////////////////////////////
	
	//1) get this view's document
	COWGraphDoc* pDoc = (COWGraphDoc*) GetDocument(); 
	ASSERT(pDoc != NULL); 
	
	//2) get graph view and activate if existing
	CView* pView;
	POSITION pos = pDoc->GetFirstViewPosition();
	while(pos)
	{
		pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(COWGraphView))) 
		{   
			//graph view is already existing, activate it and return
			pView->GetParentFrame()->ActivateFrame();
			return;
		}
	}
	
	//3) since graph view does not exist, create and activate it
#ifdef OWGRAPH_INTERNAL	//when OWGRAPH linked with the application
	#ifdef OIIIAPP //and when the app is OIII Application
	CMDIChildWnd* pNewFrame = (CMDIChildWnd*) (((COIIIApp*)AfxGetApp())->m_pGraphViewTemplate->CreateNewFrame(pDoc, NULL));
	if(pNewFrame == NULL) return; //not created
	ASSERT(pNewFrame->IsKindOf(RUNTIME_CLASS(COWGraphMDIChildWnd)));
	((COIIIApp*)AfxGetApp())->m_pGraphViewTemplate->InitialUpdateFrame(pNewFrame, pDoc);
	#else
	ASSERT(FALSE); //development-time error, should not use this function if not in OIII app.
	#endif //OIIIAPP
#endif //OWGRAPH_INTERNAL
	return;	
}
 
void COWGraphDataView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{   
	//by default, a CEditView initialy disable EDIT_COPY menuitem
	pCmdUI->Enable(TRUE);
}

void COWGraphDataView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{   
	//by default, a CEditView initialy disable EDIT_PASTE menuitem (because it is in the non-editable mode)
	pCmdUI->Enable(TRUE);
}

void COWGraphDataView::OnLButtonDown(UINT, CPoint )
{
	//we want to desactivate the mouse click
	//CEditView::OnLButtonDown(nFlags, point);
}

void COWGraphDataView::OnLButtonUp(UINT, CPoint )
{
	//we want to desactivate the mouse click
	//CEditView::OnLButtonUp(nFlags, point);   
}

BOOL COWGraphDataView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	return CEditView::OnSetCursor(pWnd, nHitTest, message);
}
