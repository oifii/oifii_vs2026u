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
#include "oifii_app.h"

// setsheet.cpp : implementation file
//

//#include "stdafx.h"
//#include "oifii_app.h"


#include <gl\gl.h>
#include <gl\glu.h>

#include "oifii_propsettings.h"
#include "oifii_proppage.h"
#include "oifii_propsheet.h"

/*
#include "owpropag.h"
#include "extrpage.h"
#include "fuspage.h"
#include "claspage.h"
#include "profpage.h"
#include "histopage.h"
#include "scatterpage.h"
#include "viewpage.h"
#include "projpage.h"
#include "tranpage.h"
#include "tracpage.h"
#include "behapage.h"
*/

/*
#include "extrsett.h"
#include "fussett.h"
#include "classett.h"
#include "profsett.h"
#include "histosett.h"
#include "scattersett.h"
#include "viewsett.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include "projsett.h"
#include "transett.h"
#include "tracsett.h"
#include "behasett.h"
*/

#include "c_pointset.h"
#include "owpointset.h"

#include "owdoc.h"
#include "ow2doc.h"
#include "ow2mdi.h"
#include "owview.h"
#include "ow2view.h"

#include <gl\gl.h>
#include <gl\glu.h>
#include "ow3view.h"

//#include "setsheet.h"

/*
*/

//#include "owpropag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsSheet

IMPLEMENT_DYNAMIC(CSettingsSheet, CPropertySheet)

CSettingsSheet::CSettingsSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddControlPages();
}

CSettingsSheet::CSettingsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddControlPages();
}

CSettingsSheet::~CSettingsSheet()
{
	if(m_pExtractionPage) delete m_pExtractionPage;
	if(m_pFusionPage) delete m_pFusionPage;
	if(m_pClassificationPage) delete m_pClassificationPage;
	if(m_pProfilePage) delete m_pProfilePage;
	if(m_pHistoPage) delete m_pHistoPage;
	if(m_pScatterPage) delete m_pScatterPage;
	if(m_pViewDataPage) delete m_pViewDataPage;
	if(m_pProjectionPage) delete m_pProjectionPage;
	if(m_pTransformationPage) delete m_pTransformationPage;
	if(m_pTracePage) delete m_pTracePage;
	if(m_pBehaviorPage) delete m_pBehaviorPage;

	if(m_pExtractionSettings) delete m_pExtractionSettings;
	if(m_pFusionSettings) delete m_pFusionSettings;
	if(m_pClassificationSettings) delete m_pClassificationSettings;
	if(m_pProfileSettings) delete m_pProfileSettings;
	if(m_pHistoSettings) delete m_pHistoSettings;
	if(m_pScatterSettings) delete m_pScatterSettings;
	if(m_pViewDataSettings) delete m_pViewDataSettings;
	if(m_pProjectionSettings) delete m_pProjectionSettings;
	if(m_pTransformationSettings) delete m_pTransformationSettings;
	if(m_pTraceSettings) delete m_pTraceSettings;
	if(m_pBehaviorSettings) delete m_pBehaviorSettings;

}

void CSettingsSheet::AddControlPages()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;

	m_pExtractionSettings = (CExtractionSettings*) new CExtractionSettings;
	ASSERT(m_pExtractionSettings!=NULL);
	m_pFusionSettings = (CFusionSettings*) new CFusionSettings;
	ASSERT(m_pFusionSettings!=NULL);
	m_pClassificationSettings = (CClassificationSettings*) new CClassificationSettings;
	ASSERT(m_pClassificationSettings!=NULL);
	m_pProfileSettings = (CProfileSettings*) new CProfileSettings;
	ASSERT(m_pProfileSettings!=NULL);
	m_pHistoSettings = (CHistoSettings*) new CHistoSettings;
	ASSERT(m_pHistoSettings!=NULL);
	m_pScatterSettings = (CScatterSettings*) new CScatterSettings;
	ASSERT(m_pScatterSettings!=NULL);
	m_pViewDataSettings = (CViewDataSettings*) new CViewDataSettings;
	ASSERT(m_pViewDataSettings!=NULL);
	m_pProjectionSettings = (CProjectionSettings*) new CProjectionSettings;
	ASSERT(m_pProjectionSettings!=NULL);
	m_pTransformationSettings = (CTransformationSettings*) new CTransformationSettings;
	ASSERT(m_pTransformationSettings!=NULL);
	m_pTraceSettings = (CTraceSettings*) new CTraceSettings;
	ASSERT(m_pTraceSettings!=NULL);
	m_pBehaviorSettings = (CBehaviorSettings*) new CBehaviorSettings;
	ASSERT(m_pBehaviorSettings!=NULL);

	m_pExtractionPage = (CExtractionPage*) new CExtractionPage();//"Extraction", 0);
	ASSERT(m_pExtractionPage!=NULL);
	m_pFusionPage = (CFusionPage*) new CFusionPage();//"Fusion", 1);
	ASSERT(m_pFusionPage!=NULL);
	m_pClassificationPage = (CClassificationPage*) new CClassificationPage();//"Classification", 2);;
	ASSERT(m_pClassificationPage!=NULL);
	m_pProfilePage = (CProfilePage*) new CProfilePage();//"Local Statisticss", 3);
	ASSERT(m_pProfilePage!=NULL);
	m_pHistoPage = (CHistoPage*) new CHistoPage();//"Local Statisticss", 3);
	ASSERT(m_pHistoPage!=NULL);
	m_pScatterPage = (CScatterPage*) new CScatterPage();//"Global Statistics", 4);
	ASSERT(m_pScatterPage!=NULL);
	m_pViewDataPage = (CViewDataPage*) new CViewDataPage();//"Data Visualization", 5);
	ASSERT(m_pViewDataPage!=NULL);
	m_pProjectionPage = (CProjectionPage*) new CProjectionPage();//"Projection", 6);
	ASSERT(m_pProjectionPage!=NULL);
	m_pTransformationPage = (CTransformationPage*) new CTransformationPage();//"Transformation", 7);
	ASSERT(m_pTransformationPage!=NULL);
	m_pTracePage = (CTracePage*) new CTracePage();//"Trace", 8);
	ASSERT(m_pTracePage!=NULL);
	m_pBehaviorPage = (CBehaviorPage*) new CBehaviorPage();//"Behavior", 9);
	ASSERT(m_pBehaviorPage!=NULL);

	if(m_pExtractionPage) AddPage(m_pExtractionPage);
	if(m_pFusionPage) AddPage(m_pFusionPage);
	if(m_pClassificationPage) AddPage(m_pClassificationPage);
	if(m_pProfilePage) AddPage(m_pProfilePage);
	if(m_pHistoPage) AddPage(m_pHistoPage);
    if(m_pScatterPage) AddPage(m_pScatterPage);
    if(m_pViewDataPage) AddPage(m_pViewDataPage);
    if(m_pProjectionPage) AddPage(m_pProjectionPage);
	if(m_pTransformationPage) AddPage(m_pTransformationPage);
	if(m_pTracePage) AddPage(m_pTracePage);
	if(m_pBehaviorPage) AddPage(m_pBehaviorPage);

}

BEGIN_MESSAGE_MAP(CSettingsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CSettingsSheet)
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_MESSAGE(OWM_SETTINGSSHEET_UPDATEINFO, OnUpdateSettingsSheetInfo) //offworld defined message
	ON_WM_ACTIVATE()
	ON_WM_CHILDACTIVATE()
	ON_WM_MOUSEACTIVATE()
	ON_MESSAGE(OWM_MOUSEMOVED_UPDATELOCALGRAPH, OnMouseMovedUpdateLocalGraph) //offworld defined message
	ON_WM_NCMOUSEMOVE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsSheet message handlers

BOOL CSettingsSheet::OnInitDialog() 
{
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	return CPropertySheet::OnInitDialog();	//base class
}

HCURSOR CSettingsSheet::OnQueryDragIcon() 
{
	return (HCURSOR) m_hIcon;
	//return CPropertySheet::OnQueryDragIcon();	 //default
}

BOOL CSettingsSheet::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertySheet::DestroyWindow();
}

void CSettingsSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::PostNcDestroy();
}

void CSettingsSheet::OnClose() 
{
	
	ShowWindow(SW_HIDE);	
	//CPropertySheet::OnClose();
}

COWView* CSettingsSheet::GetAttachedView()
{
	COWView* pView = NULL;
	if(m_pOW2View!=NULL) pView = (COWView*) m_pOW2View;
	if(m_pOW3View!=NULL) pView = (COWView*) m_pOW3View;
	if(pView!=NULL) ASSERT(pView->IsKindOf(RUNTIME_CLASS(COWView)));
	return pView;
}

BOOL CSettingsSheet::SetAttachedView(COWView* pOWView)
{
	if(pOWView!=NULL && pOWView->IsKindOf(RUNTIME_CLASS(COW2View)))
	{
		m_pOW2View = (COW2View*)pOWView;
		m_pOW3View = NULL;
	}
	else if(pOWView!=NULL && pOWView->IsKindOf(RUNTIME_CLASS(COW3View)))
	{
		m_pOW3View = (COW3View*)pOWView;
		m_pOW2View = NULL;
	}
	else
	{
		m_pOW3View = NULL;
		m_pOW2View = NULL;
		return FALSE;
	}
	return TRUE;
}

COW2Doc* CSettingsSheet::GetAttachedDoc()
{
	if(m_pOW2Doc) ASSERT(m_pOW2Doc->IsKindOf(RUNTIME_CLASS(COW2Doc)));
	return m_pOW2Doc; 
}

BOOL CSettingsSheet::SetAttachedDoc(COW2Doc* pOW2Doc)
{
	if(pOW2Doc!=NULL && pOW2Doc->IsKindOf(RUNTIME_CLASS(COW2Doc)))
	{
		m_pOW2Doc = pOW2Doc;
		CString myString = "Settings for ";
		myString += m_pOW2Doc->GetTitle();
		SetWindowText(myString);
	}
	else
	{
		m_pOW2Doc = NULL;
		SetWindowText(L"No Document Selected");
		//erase graph
		COWPropertyPage* pPage = (COWPropertyPage*) GetActivePage();
		if(pPage) pPage->UpdateGraph();

	}
	return TRUE;
}



//spi, nov 2018, begin
//LONG CSettingsSheet::OnUpdateSettingsSheetInfo(WPARAM wParam, LPARAM lParam)
LRESULT CSettingsSheet::OnUpdateSettingsSheetInfo(WPARAM wParam, LPARAM lParam)
//spi, nov 2018, end
{
	//1) capture active mdi child view and doc
	CMDIFrameWnd* pMDIParentFrameWnd = (CMDIFrameWnd*) AfxGetApp()->m_pMainWnd;
	CFrameWnd* pFrameWnd = pMDIParentFrameWnd->GetActiveFrame();
	//1.1) is the active child of type COW2MDI ?
	BOOL bIsOW2MDI = pFrameWnd->IsKindOf(RUNTIME_CLASS(COW2MDI));

	COIIIApp* pOIIIApp = (COIIIApp*)AfxGetApp();

	if(bIsOW2MDI==TRUE)
	{
		CView* pView = pFrameWnd->GetActiveView();
		COWView* pOWView = (COWView*)pView;
		ASSERT(pView!=NULL);
		//1.2) is the active view of type COW2View, COW3View, or ...?
		m_bIsOW2View = pView->IsKindOf(RUNTIME_CLASS(COW2View));
		m_bIsOW3View = pView->IsKindOf(RUNTIME_CLASS(COW3View));
		if(!m_bIsOW2View && !m_bIsOW3View) m_bIsAppDefault = TRUE; 
		if(m_bIsOW2View || m_bIsOW3View)
		{
			SetAttachedView(pOWView);
			SetAttachedDoc((COW2Doc*) pOWView->GetDocument());
			ASSERT(m_pOW2Doc!=NULL);
			//copy content of all settings instances
			//... from document		
			m_pExtractionSettings->CopyAllValuesFrom(m_pOW2Doc->m_pExtractionSettings);
			m_pFusionSettings->CopyAllValuesFrom(m_pOW2Doc->m_pFusionSettings);
			m_pClassificationSettings->CopyAllValuesFrom(m_pOW2Doc->m_pClassificationSettings);
			m_pProfileSettings->CopyAllValuesFrom(m_pOW2Doc->m_pProfileSettings);
			m_pHistoSettings->CopyAllValuesFrom(m_pOW2Doc->m_pHistoSettings);
			m_pScatterSettings->CopyAllValuesFrom(m_pOW2Doc->m_pScatterSettings);
			m_pTraceSettings->CopyAllValuesFrom(m_pOW2Doc->m_pTraceSettings);
			m_pBehaviorSettings->CopyAllValuesFrom(m_pOW2Doc->m_pBehaviorSettings);
			//...from view
			m_pViewDataSettings->CopyAllValuesFrom(pOWView->m_pViewDataSettings);
			m_pProjectionSettings->CopyAllValuesFrom(pOWView->m_pProjectionSettings);
			m_pTransformationSettings->CopyAllValuesFrom(pOWView->m_pTransformationSettings);
			
		}
		else if(m_bIsAppDefault)
		{
			//an other type of MDI
			SetAttachedView(NULL);
			SetAttachedDoc(NULL);

			//copy content of all settings instances
			//... from application defaults
			m_pExtractionSettings->CopyAllValuesFrom(pOIIIApp->m_pExtractionSettings);
			m_pFusionSettings->CopyAllValuesFrom(pOIIIApp->m_pFusionSettings);
			m_pClassificationSettings->CopyAllValuesFrom(pOIIIApp->m_pClassificationSettings);
			m_pProfileSettings->CopyAllValuesFrom(pOIIIApp->m_pProfileSettings);
			m_pHistoSettings->CopyAllValuesFrom(pOIIIApp->m_pHistoSettings);
			m_pScatterSettings->CopyAllValuesFrom(pOIIIApp->m_pScatterSettings);
			m_pTraceSettings->CopyAllValuesFrom(pOIIIApp->m_pTraceSettings);
			m_pBehaviorSettings->CopyAllValuesFrom(pOIIIApp->m_pBehaviorSettings);
			//...from application defaults
			m_pViewDataSettings->CopyAllValuesFrom(pOIIIApp->m_pViewDataSettings);
			m_pProjectionSettings->CopyAllValuesFrom(pOIIIApp->m_pProjectionSettings);
			m_pTransformationSettings->CopyAllValuesFrom(pOIIIApp->m_pTransformationSettings);
		}
		else
		{
			ASSERT(FALSE); //development-time error
		}
	}
	else
	{
		//mainframe only
		m_bIsAppDefault = TRUE;

		SetAttachedView(NULL);
		SetAttachedDoc(NULL);

		//copy content of all settings instances
		//... from application defaults
		m_pExtractionSettings->CopyAllValuesFrom(pOIIIApp->m_pExtractionSettings);
		m_pFusionSettings->CopyAllValuesFrom(pOIIIApp->m_pFusionSettings);
		m_pClassificationSettings->CopyAllValuesFrom(pOIIIApp->m_pClassificationSettings);
		m_pProfileSettings->CopyAllValuesFrom(pOIIIApp->m_pProfileSettings);
		m_pHistoSettings->CopyAllValuesFrom(pOIIIApp->m_pHistoSettings);
		m_pScatterSettings->CopyAllValuesFrom(pOIIIApp->m_pScatterSettings);
		m_pTraceSettings->CopyAllValuesFrom(pOIIIApp->m_pTraceSettings);
		m_pBehaviorSettings->CopyAllValuesFrom(pOIIIApp->m_pBehaviorSettings);
		//...from application defaults
		m_pViewDataSettings->CopyAllValuesFrom(pOIIIApp->m_pViewDataSettings);
		m_pProjectionSettings->CopyAllValuesFrom(pOIIIApp->m_pProjectionSettings);
		m_pTransformationSettings->CopyAllValuesFrom(pOIIIApp->m_pTransformationSettings);
	}

	/*
	//poirier, sept 2001, begin
	if(pOIIIApp->IsSettingsSheetGraphUpdateRequired())
	{
		((COWPropertyPage*)GetActivePage())->UpdateGraph();

		//BOOL CHistoPage::OnSetActive() 
		//{
			//UpdateSettingsFromDocToSheet();	
			//UpdateHisto();
			//return COWPropertyPage::OnSetActive();
		//}

	}
	//poirier, sept 2001, end
	*/

	return TRUE;
}

int CSettingsSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_bIsOW2View = FALSE;
	m_bIsOW3View = FALSE;
	m_bIsAppDefault = FALSE;
	return 0;
}

void CSettingsSheet::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CPropertySheet::OnActivate(nState, pWndOther, bMinimized);

	if(nState>0)
	{
		//on sheet activation
		CPropertyPage* pPropertyPage = GetActivePage();
		if(pPropertyPage==NULL) return;
		ASSERT(pPropertyPage->IsKindOf(RUNTIME_CLASS(COWPropertyPage)));
		((COWPropertyPage*)pPropertyPage)->UpdateSettingsFromDocToSheet();
		//PostMessage(OWM_SETTINGSSHEET_UPDATEINFO);
	}
	else if(nState==0)
	{
		//PostMessage(OWM_SETTINGSSHEET_UPDATEINFO);

		/* very last commented out, oct 13 2001 
		//on sheet desactivation		
		SetAttachedDoc(NULL);
		SetAttachedView(NULL);
		PostMessage(OWM_SETTINGSSHEET_UPDATEINFO);
		*/
		/* //some time ago
		CPropertyPage* pPropertyPage = GetActivePage();
		if(pPropertyPage==NULL) return;
		if(pPropertyPage->IsKindOf(RUNTIME_CLASS(CViewDataPage)))
		{
			((CViewDataPage*)pPropertyPage)->UpdateViewDataSettingsFromViewToSheet();
		}
		*/
	}
	/* on desactivation, do nothing
	else if(nState==0 && pWndOther->IsKindOf(RUNTIME_CLASS(COWView)))
	{
		
		//on sheet desactivation
		CPropertyPage* pPropertyPage = GetActivePage();
		if(pPropertyPage==NULL) return;
		if(pPropertyPage->IsKindOf(RUNTIME_CLASS(CViewDataPage)))
		{
			((CViewDataPage*)pPropertyPage)->UpdateViewDataSettingsFromSheetToView();
		}
	}
	*/

	return;
}

void CSettingsSheet::OnChildActivate() 
{
	CPropertySheet::OnChildActivate();
	
	// TODO: Add your message handler code here
	
}

int CSettingsSheet::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CPropertySheet::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


//spi, nov 2018, begin
//LONG CSettingsSheet::OnMouseMovedUpdateLocalGraph(WPARAM wParam, LPARAM lParam)
LRESULT CSettingsSheet::OnMouseMovedUpdateLocalGraph(WPARAM wParam, LPARAM lParam)
//spi, nov 2018, end
{
	//if(((COIIIApp*)AfxGetApp())->IsSettingsSheetVisible())
	//{
		struct tagCONTEXTINFO* pGlobalStructCONTEXTINFO = (struct tagCONTEXTINFO*) wParam;
		CPropertyPage* pPropertyPage = GetActivePage();
		if(pPropertyPage->IsKindOf(RUNTIME_CLASS(CProfilePage))) 
			m_pProfilePage->OnMouseMovedUpdateLocalGraph(pGlobalStructCONTEXTINFO);
		else if(pPropertyPage->IsKindOf(RUNTIME_CLASS(CHistoPage)))
			m_pHistoPage->OnMouseMovedUpdateLocalGraph(pGlobalStructCONTEXTINFO);
		else if(pPropertyPage->IsKindOf(RUNTIME_CLASS(CScatterPage)))
			m_pScatterPage->OnMouseMovedUpdateLocalGraph(pGlobalStructCONTEXTINFO);
		else
		{
			//ASSERT(FALSE);
			return FALSE;
		}
		return TRUE;
	//}
	//else
	//{
	//	ASSERT(FALSE);
	//	return FALSE;
	//}
}

//poirier, sept 2001, begin
void CSettingsSheet::OnNcMouseMove(UINT nHitTest, CPoint point) 
{	
	//to prevent selection rectangle to be left dirty on the ow2view
	//history: selection rect was left dirty whenever user would grab/move 
	//settings window dlg (histo ...)

	//send erase selection rectangle command to ow2view
	CView* pView = GetAttachedView();
	COW2View* pOW2View = NULL;
	if(pView)
	{
		if(pView->IsKindOf(RUNTIME_CLASS(COW2View))) pOW2View = (COW2View*)pView;
		if(pOW2View!=NULL) pOW2View->ROIFloatingMode_EraseSelectionRect();
	}
	CPropertySheet::OnNcMouseMove(nHitTest, point);
}
//poirier, sept 2001, end

void CSettingsSheet::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertySheet::OnShowWindow(bShow, nStatus);
	
	if(bShow)
	{
		if(((COIIIApp*)AfxGetApp())->IsSettingsSheetGraphUpdateRequired())
		{
			((COWPropertyPage*)GetActivePage())->UpdateSettingsFromDocToSheet();;
			((COWPropertyPage*)GetActivePage())->UpdateGraph();

			//BOOL CHistoPage::OnSetActive() 
			//{
				//UpdateSettingsFromDocToSheet();	
				//UpdateHisto();
				//return COWPropertyPage::OnSetActive();
			//}

		}
	}
	
}
