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

#include "c_pointset.h"
#include "c_segmentset.h"

#include <gl\gl.h>
#include <gl\glu.h>
#include "oifii_propsettings.h"
#include "oifii_propsheet.h"
#include "oifii_proppage.h"

#include "OWpointset.h"
#include "OWedgeset.h"
#include "OWpixelset.h"

#include "OWdoc.h"
#include "OW2doc.h"
#include "OW2mdi.h"
#include "OWview.h"
#include "OW2view.h"
#include "OW3view.h"

#include "gl_owgraph.h"
#include "owgl_profilevw.h"
#include "owgl_profile.h"
#include "owgl_histovw.h"
#include "owgl_histo.h"
#include "owgl_scattervw.h"
#include "owgl_scatter.h"


#include "logdoc.h"

/*
#include "ow2mdi.h"
#include "owgraph.h"
#include "histovw.h"
#include "histo.h"
*/
/*
#include "ow2mdi.h"
#include "owgraph.h"
#include "scattervw.h"
#include "scatter.h"
*/

/*
#include <gl\gl.h>
#include <gl\glu.h>
#include "pointset.h"

#include "owdoc.h"
#include "ow2doc.h"
#include "owview.h"
#include "ow3view.h"
*/

/*
#include "ow2mdi.h"
#include "owgraph.h"
#include "profilevw.h"
#include "profile.h"

#include "fusion.h"
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




char* pszProfilePointCollectionNames[PROFILE_NUMBEROF_POINTCOLLECTIONNAME] = 
{
	"BR - X Center",
	"BR - Y Center",
	"BR - Perimeter",	
	"Edge - Along",	
	"Edge - Inner",	
	"Edge - Outter"	
};

char* pszHistoPointCollectionNames[HISTO_NUMBEROF_POINTCOLLECTIONNAME] = 
{
	"BR - Interior",
	"SBR - Interiors",
	"SBR - Inner Edge Halfs",
	"SBR - Outter Edge Halfs",
	"Edge - Along",	
	"Edge - Inner",	
	"Edge - Outter",
	"Edge - AIO",
	"Polygon - Interior"
};

char* pszScatterPointCollectionNames[SCATTER_NUMBEROF_POINTCOLLECTIONNAME] = 
{
	"BR - Interior",
	"SBR - Interiors",
	"SBR - Inner Edge Halfs",
	"SBR - Outter Edge Halfs",
	"Edge - Along",	
	"Edge - Inner",	
	"Edge - Outter",
	"Edge - AIO",
	"Polygon - Interior"
};



/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// owpropag.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "oifii_app.h"
//#include "owpropag.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

IMPLEMENT_DYNCREATE(COWPropertyPage, CPropertyPage)

COWPropertyPage::COWPropertyPage()
{
	m_pSettingsSheet = NULL;
}

COWPropertyPage::COWPropertyPage(int iIDD) : CPropertyPage(iIDD)
{
}

COWPropertyPage::~COWPropertyPage()
{
}

BOOL COWPropertyPage::UpdateSettingsFromDocToSheet()
{
	//this member function should never be called, since it is a virtual function,
	//the version in the derived class should be called instead
	ASSERT(FALSE);
	return FALSE;
}

BOOL COWPropertyPage::UpdateSettingsFromSheetToDoc()
{
	//this member function should never be called, since it is a virtual function,
	//the version in the derived class should be called instead
	ASSERT(FALSE);
	return FALSE;
}


BOOL COWPropertyPage::FillComboBoxWithPointCollectionNames(CComboBox* pComboBox)
{
	ASSERT(m_pSettingsSheet!=NULL);
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc==NULL) 
	{
		pComboBox->ResetContent();
		return FALSE;
	}

	pComboBox->ResetContent();
	for(int i=0; i<PROFILE_NUMBEROF_POINTCOLLECTIONNAME; i++) 
	{
		CString myString(pszProfilePointCollectionNames[i]);
		pComboBox->AddString(myString);
	}
	return TRUE;
}

BOOL COWPropertyPage::FillComboBoxWithImageChannelNames(CComboBox* pComboBox)
{
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc==NULL) 
	{
		return FALSE;
	}
	if(pDoc->m_pPixelset==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	int nchannel = pDoc->m_pPixelset->GetPixelsetNumberOfChannel();
	pComboBox->ResetContent();
	CString myString;
	for(int i=0; i<nchannel; i++) 
	{
		myString.Format(L"Channel %d", i);
		pComboBox->AddString(myString);
	}
	if(this->IsKindOf(RUNTIME_CLASS(CScatterPage)))
	{
		//do nothing
	}
	else
	{
		pComboBox->AddString(L"All channels");
	}
	return TRUE;
}

BOOL COWPropertyPage::FillComboBoxWithStatisticsChannelNames(CComboBox* pComboBox)
{
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc==NULL) 
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//use combo to select statistics
	/*
	CString myText;
	m_ComboStatic.GetWindowText(myText);
	if(myText!="Statistics channel")
	{
		m_ComboStatic.SetWindowText("Statistics channel");
	}
	*/
	pComboBox->ResetContent();
	for(int i=0; i<pDoc->GetNumberOfStatPerPoint(); i++) //POINTSET_DEFAULT_NUMBEROFSTAT + POINTSET_EXTRA_NUMBEROFSTAT
	{
		CString myString(pszStatChannelNames[i]);
		pComboBox->AddString(myString);
	}	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// behapage.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "oifii_app.h"
//#include "owpropag.h"

//#include "pointset.h"

//#include "behapage.h"

/*
#include "setsheet.h"
#include "behasett.h"

#include "owdoc.h"
#include "ow2doc.h"

#include "owview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

/////////////////////////////////////////////////////////////////////////////
// CBehaviorPage property page

IMPLEMENT_DYNCREATE(CBehaviorPage, COWPropertyPage)

CBehaviorPage::CBehaviorPage() : COWPropertyPage(CBehaviorPage::IDD)
{
	//{{AFX_DATA_INIT(CBehaviorPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBehaviorPage::~CBehaviorPage()
{
}

void CBehaviorPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBehaviorPage)
	DDX_Control(pDX, IDC_MBHIGHLIGHTSEGMENTCHECK, m_MBHighlightSegmentCheck);
	DDX_Control(pDX, IDC_MBHIGHLIGHTNEARESTNEIGHBORSCHECK, m_MBHighlightNearestNeighborsCheck);
	DDX_Control(pDX, IDC_MBHIGHLIGHTNEARESTPOINTCHECK, m_MBHighlightNearestPointCheck);
	DDX_Control(pDX, IDC_MBLOCALSEGMENTATIONCHECK, m_MBLocalSegmentationCheckbox);
	DDX_Control(pDX, IDC_MBLOCALEXTRACTIONCHECK, m_MBLocalExtractionCheckbox);
	DDX_Control(pDX, IDC_MBLOCALCLASSIFICATIONCHECK, m_MBLocalClassificationCheckbox);
	DDX_Control(pDX, IDC_IBSELFTRAININGCHECK, m_IBSelfTrainingCheckbox);
	DDX_Control(pDX, IDC_IBSELFEXTRACSEGMENTVALIDCHECK, m_IBSelfExtracSegValidCheckbox);
	DDX_Control(pDX, IDC_OKBUTTON, m_OKButton);
	DDX_Control(pDX, IDC_APPLYBUTTON, m_ApplyButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBehaviorPage, COWPropertyPage)
	//{{AFX_MSG_MAP(CBehaviorPage)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_APPLYBUTTON, OnApplybutton)
	ON_BN_CLICKED(IDC_CANCELBUTTON, OnCancelbutton)
	ON_BN_CLICKED(IDC_OKBUTTON, OnOkbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CBehaviorPage::UpdateSettingsFromDocToSheet()
{
	CString myText;

	//0) assumptions for now
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc!=NULL) ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));

	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView!=NULL) ASSERT(pView->IsKindOf(RUNTIME_CLASS(COWView)));
	
	//1) copy view parameters from attached view
	if(pDoc) 
	{
		m_pBehaviorSettings->CopyAllValuesFrom(pDoc->m_pBehaviorSettings);
	}
	else 
	{
		m_pBehaviorSettings->CopyAllValuesFrom(NULL); //Erase
	}

	//2) set CExtractionPage controls ...
	//pixelset parameters
	BOOL bState = FALSE;
	if(pDoc==NULL || pView==NULL)
	{
		bState = FALSE;
	}
	else
	{
		bState = TRUE;
	}

	//pDoc->m_pPointset!=NULL
	m_ApplyButton.EnableWindow(bState);
	m_OKButton.EnableWindow(bState);

	BOOL bPixelset = FALSE;
	BOOL bPointset = FALSE; 
	BOOL bSegmentset = FALSE;
	if(pDoc!=NULL)
	{
		if(pDoc->m_pPixelset!=NULL) bPixelset = TRUE;
		if(pDoc->m_pPointset!=NULL) bPointset = TRUE;
		if(pDoc->m_pSegmentset!=NULL) bSegmentset = TRUE;
	}
	//poirier august 1999, begin
	else
	{
		bPixelset = FALSE;
		bPointset = FALSE;
		bSegmentset = FALSE;
	}
	//poirier august 1999, end
	m_MBHighlightSegmentCheck.EnableWindow(bState && bSegmentset);
	m_MBHighlightNearestNeighborsCheck.EnableWindow(bState && bPointset);
	m_MBHighlightNearestPointCheck.EnableWindow(bState && bPointset);

	m_MBLocalSegmentationCheckbox.EnableWindow(FALSE);
	m_MBLocalExtractionCheckbox.EnableWindow(FALSE);
	m_MBLocalClassificationCheckbox.EnableWindow(FALSE);
	m_IBSelfTrainingCheckbox.EnableWindow(FALSE);
	m_IBSelfExtracSegValidCheckbox.EnableWindow(FALSE);

	m_MBHighlightSegmentCheck.SetCheck(m_pBehaviorSettings->bMBHighlightSegment);
	m_MBHighlightNearestNeighborsCheck.SetCheck(m_pBehaviorSettings->bMBHighlightNearestNeighbors);
	m_MBHighlightNearestPointCheck.SetCheck(m_pBehaviorSettings->bMBHighlightNearestPoint);

	m_MBLocalSegmentationCheckbox.SetCheck(m_pBehaviorSettings->bMBLocalSegmentation);
	m_MBLocalExtractionCheckbox.SetCheck(m_pBehaviorSettings->bMBLocalExtraction);
	m_MBLocalClassificationCheckbox.SetCheck(m_pBehaviorSettings->bMBLocalClassification);
	m_IBSelfTrainingCheckbox.SetCheck(m_pBehaviorSettings->bIBSelfTraining);
	m_IBSelfExtracSegValidCheckbox.SetCheck(m_pBehaviorSettings->bIBSelfExtractionSegmentationValidation);
	return TRUE;
}

BOOL CBehaviorPage::UpdateSettingsFromSheetToDoc()
{
	//0) assumptions for now
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc!=NULL) ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));

	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView!=NULL) ASSERT(pView->IsKindOf(RUNTIME_CLASS(COWView)));
	
	m_pBehaviorSettings->bMBHighlightSegment = m_MBHighlightSegmentCheck.GetCheck();
	m_pBehaviorSettings->bMBHighlightNearestNeighbors = m_MBHighlightNearestNeighborsCheck.GetCheck();
	m_pBehaviorSettings->bMBHighlightNearestPoint = m_MBHighlightNearestPointCheck.GetCheck();

	m_pBehaviorSettings->bMBLocalSegmentation = m_MBLocalSegmentationCheckbox.GetCheck();
	m_pBehaviorSettings->bMBLocalExtraction = m_MBLocalExtractionCheckbox.GetCheck();
	m_pBehaviorSettings->bMBLocalClassification = m_MBLocalClassificationCheckbox.GetCheck();
	m_pBehaviorSettings->bIBSelfTraining = m_IBSelfTrainingCheckbox.GetCheck();
	m_pBehaviorSettings->bIBSelfExtractionSegmentationValidation = m_IBSelfExtracSegValidCheckbox.GetCheck();

	//3) copy sheet parameters from sheet to attached doc
	if(pDoc) pDoc->m_pBehaviorSettings->CopyAllValuesFrom(m_pBehaviorSettings);
	return TRUE;
}

int CBehaviorPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COWPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CWnd* myParent = GetParent();
	if(myParent!=NULL)
	{
		ASSERT(myParent->IsKindOf(RUNTIME_CLASS(CSettingsSheet)));
		m_pSettingsSheet = (CSettingsSheet*)myParent;
		m_pBehaviorSettings = m_pSettingsSheet->m_pBehaviorSettings;
	}
	
	return 0;
}

BOOL CBehaviorPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COWPropertyPage::OnKillActive();
}

BOOL CBehaviorPage::OnSetActive() 
{
	UpdateSettingsFromDocToSheet();	
	return COWPropertyPage::OnSetActive();
}

void CBehaviorPage::OnApplybutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
}

void CBehaviorPage::OnCancelbutton() 
{
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}

void CBehaviorPage::OnOkbutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// claspage.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


/*
#include "stdafx.h"
#include "oifii_app.h"

#include "owpropag.h"
#include "claspage.h"

#include "setsheet.h"
#include "classett.h"

//#include "pointset.h"
#include "owdoc.h"
#include "ow2doc.h"

#include "owview.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/


IMPLEMENT_DYNCREATE(CClassificationPage, COWPropertyPage)

CClassificationPage::CClassificationPage() : COWPropertyPage(CClassificationPage::IDD)
{
	//{{AFX_DATA_INIT(CClassificationPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CClassificationPage::~CClassificationPage()
{
}

void CClassificationPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClassificationPage)
	DDX_Control(pDX, IDC_CTSUPERVISEDCHECK, m_CTSupervisedCheckbox);
	DDX_Control(pDX, IDC_CTNONSUPERVISEDCHECK, m_CTNonsupervisedCheckbox);
	DDX_Control(pDX, IDC_OKBUTTON, m_OKButton);
	DDX_Control(pDX, IDC_APPLYBUTTON, m_ApplyButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClassificationPage, COWPropertyPage)
	//{{AFX_MSG_MAP(CClassificationPage)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_OKBUTTON, OnOkbutton)
	ON_BN_CLICKED(IDC_CANCELBUTTON, OnCancelbutton)
	ON_BN_CLICKED(IDC_APPLYBUTTON, OnApplybutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassificationPage message handlers

BOOL CClassificationPage::UpdateSettingsFromDocToSheet()
{
	return TRUE;
}

BOOL CClassificationPage::UpdateSettingsFromSheetToDoc()
{
	return TRUE;
}

int CClassificationPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COWPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	CWnd* myParent = GetParent();
	if(myParent!=NULL)
	{
		ASSERT(myParent->IsKindOf(RUNTIME_CLASS(CSettingsSheet)));
		m_pSettingsSheet = (CSettingsSheet*)myParent;
		m_pClassificationSettings = m_pSettingsSheet->m_pClassificationSettings;
	}
	return 0;
}

BOOL CClassificationPage::OnSetActive() 
{
	UpdateSettingsFromDocToSheet();	
	return COWPropertyPage::OnSetActive();
}

BOOL CClassificationPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COWPropertyPage::OnKillActive();
}

void CClassificationPage::OnOkbutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}

void CClassificationPage::OnCancelbutton() 
{
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}

void CClassificationPage::OnApplybutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// extrpage.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/*
#include "stdafx.h"
#include "oifii_app.h"

#include "owpropag.h"
#include "extrpage.h"

#include "setsheet.h"
#include "extrsett.h"
#include "pointset.h"

#include "owdoc.h"
#include "ow2doc.h"

#include "owview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/


IMPLEMENT_DYNCREATE(CExtractionPage, COWPropertyPage)

CExtractionPage::CExtractionPage() : COWPropertyPage(CExtractionPage::IDD)
{
	//{{AFX_DATA_INIT(CExtractionPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSettingsSheet = NULL;
	m_pExtractionSettings = NULL;
}

CExtractionPage::~CExtractionPage()
{
}

void CExtractionPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtractionPage)
	DDX_Control(pDX, IDC_OKBUTTON, m_OKButton);
	DDX_Control(pDX, IDC_TRAINING, m_EZTrainingButton);
	DDX_Control(pDX, IDC_TCDDETECTCHECK, m_TCDDetectCheck);
	DDX_Control(pDX, IDC_TCCSPECTRALVARIABILITYCHECK, m_TCCSpectralVariabilityCheck);
	DDX_Control(pDX, IDC_TCCHEIGHTCHECK, m_TCCHeightCheck);
	DDX_Control(pDX, IDC_TCCDENSITYCHECK, m_TCCDensityCheck);
	DDX_Control(pDX, IDC_TCCAVERAGEINTENSITYCHECK, m_TCCAverageIntensityCheck);
	DDX_Control(pDX, IDC_TCCAREACHECK, m_TCCAreaCheck);
	DDX_Control(pDX, IDC_THRESHOLDEDIT, m_TCDThresholdEdit);
	DDX_Control(pDX, IDC_SCALEEDIT, m_TCDScaleEdit);
	DDX_Control(pDX, IDC_EXCLUDEROADCHECK, m_EZRoadCheck);
	DDX_Control(pDX, IDC_EXCLUDEMANMADECHECK, m_EZManMadeCheck);
	DDX_Control(pDX, IDC_EXCLUDEFORESTCHECK, m_EZForestationCheck);
	DDX_Control(pDX, IDC_EXCLUDEDEFORESTATIONCHECK, m_EZDeforestationCheck);
	DDX_Control(pDX, IDC_CANCELBUTTON, m_CancelButton);
	DDX_Control(pDX, IDC_APPLYBUTTON, m_ApplyButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtractionPage, COWPropertyPage)
	//{{AFX_MSG_MAP(CExtractionPage)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_APPLYBUTTON, OnApplybutton)
	ON_BN_CLICKED(IDC_CANCELBUTTON, OnCancelbutton)
	ON_BN_CLICKED(IDC_OKBUTTON, OnOkbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int CExtractionPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CWnd* myParent = GetParent();
	if(myParent!=NULL)
	{
		ASSERT(myParent->IsKindOf(RUNTIME_CLASS(CSettingsSheet)));
		m_pSettingsSheet = (CSettingsSheet*)myParent;
		m_pExtractionSettings = m_pSettingsSheet->m_pExtractionSettings;
	}
	return 0;
}

BOOL CExtractionPage::OnSetActive() 
{
	UpdateSettingsFromDocToSheet();	
	return CPropertyPage::OnSetActive();
}

BOOL CExtractionPage::OnKillActive() 
{
	return CPropertyPage::OnKillActive();
}

BOOL CExtractionPage::UpdateSettingsFromDocToSheet()
{
	CString myText;

	//0) assumptions for now
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc!=NULL) ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));

	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView!=NULL) ASSERT(pView->IsKindOf(RUNTIME_CLASS(COWView)));
	
	//1) copy view parameters from attached view
	if(pDoc) 
	{
		m_pExtractionSettings->CopyAllValuesFrom(pDoc->m_pExtractionSettings);
	}
	else 
	{
		m_pExtractionSettings->CopyAllValuesFrom(NULL); //Erase
	}

	//2) set CExtractionPage controls ...
	//pixelset parameters
	BOOL bState = FALSE;
	if(pDoc==NULL || pView==NULL || 
		!(pDoc->m_pPixelset!=NULL && pDoc->m_pPointset==NULL))
	{
		bState = FALSE;
	}
	else
	{
		bState = TRUE;
	}

	m_ApplyButton.EnableWindow(bState);
	m_OKButton.EnableWindow(bState);

	m_TCDDetectCheck.EnableWindow(bState);
	m_TCDThresholdEdit.EnableWindow(bState);
	m_TCDScaleEdit.EnableWindow(bState);

	m_EZTrainingButton.EnableWindow(m_pExtractionSettings->bAllowEZTraining);
	m_TCCSpectralVariabilityCheck.EnableWindow(FALSE);
	m_TCCHeightCheck.EnableWindow(FALSE);
	m_TCCDensityCheck.EnableWindow(bState);
	m_TCCAverageIntensityCheck.EnableWindow(bState);
	m_TCCAreaCheck.EnableWindow(bState);

	m_EZRoadCheck.EnableWindow(FALSE);
	m_EZManMadeCheck.EnableWindow(FALSE);
	m_EZForestationCheck.EnableWindow(FALSE);
	m_EZDeforestationCheck.EnableWindow(FALSE);

	m_TCDDetectCheck.SetCheck(m_pExtractionSettings->bTCDbyLocalMax);
	myText.Format(L"%d", m_pExtractionSettings->iTCDThreshold);
	m_TCDThresholdEdit.SetWindowText(myText);
	myText.Format(L"%d", m_pExtractionSettings->iTCDScale);
	m_TCDScaleEdit.SetWindowText(myText);

	m_TCCAreaCheck.SetCheck(m_pExtractionSettings->bTCCArea);
	m_TCCDensityCheck.SetCheck(m_pExtractionSettings->bTCCDensity);
	m_TCCHeightCheck.SetCheck(m_pExtractionSettings->bTCCHeight);
	m_TCCAverageIntensityCheck.SetCheck(m_pExtractionSettings->bTCCAverageIntensity);
	m_TCCSpectralVariabilityCheck.SetCheck(m_pExtractionSettings->bTCCSpectralVariability);

	m_EZRoadCheck.SetCheck(m_pExtractionSettings->bEZRoads);
	m_EZManMadeCheck.SetCheck(m_pExtractionSettings->bEZManMade);
	m_EZForestationCheck.SetCheck(m_pExtractionSettings->bEZForestation);
	m_EZDeforestationCheck.SetCheck(m_pExtractionSettings->bEZDeforestation);

	return TRUE;
}

BOOL CExtractionPage::UpdateSettingsFromSheetToDoc()
{
	//0) assumptions for now
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc!=NULL) ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));

	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView!=NULL) ASSERT(pView->IsKindOf(RUNTIME_CLASS(COWView)));
	

	WCHAR pszText[255];
	//float fFloat, fValidatedFloat;
	long iValue, iValidatedValue;

	m_pExtractionSettings->bTCDbyLocalMax = m_TCDDetectCheck.GetCheck();
	m_TCDThresholdEdit.GetWindowText(pszText, 50);
	iValue = (long) _wtoi(pszText);
	iValidatedValue = iValue; //no validation for now
	m_pExtractionSettings->iTCDThreshold = iValidatedValue;

	m_TCDScaleEdit.GetWindowText(pszText, 50);
	iValue = (long) _wtoi(pszText);
	iValidatedValue = iValue; //no validation for now
	m_pExtractionSettings->iTCDScale = iValidatedValue;

	m_pExtractionSettings->bTCCArea = m_TCCAreaCheck.GetCheck();
	m_pExtractionSettings->bTCCDensity = m_TCCDensityCheck.GetCheck();
	m_pExtractionSettings->bTCCHeight = m_TCCHeightCheck.GetCheck();
	m_pExtractionSettings->bTCCAverageIntensity = m_TCCAverageIntensityCheck.GetCheck();
	m_pExtractionSettings->bTCCSpectralVariability = m_TCCSpectralVariabilityCheck.GetCheck();

	m_pExtractionSettings->bEZRoads = m_EZRoadCheck.GetCheck();
	m_pExtractionSettings->bEZManMade = m_EZManMadeCheck.GetCheck();
	m_pExtractionSettings->bEZForestation = m_EZForestationCheck.GetCheck();
	m_pExtractionSettings->bEZDeforestation = m_EZDeforestationCheck.GetCheck();

	//3) copy sheet parameters from sheet to attached doc
	if(pDoc) pDoc->m_pExtractionSettings->CopyAllValuesFrom(m_pExtractionSettings);
	return TRUE;
}

void CExtractionPage::OnApplybutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
}

void CExtractionPage::OnCancelbutton() 
{
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}

void CExtractionPage::OnOkbutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// fuspage.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

// fuspage.cpp : implementation file
//

//#include "stdafx.h"
//#include "oifii_app.h"
//#include "owpropag.h"

/*
#include "fuspage.h"

#include "setsheet.h"
#include "fussett.h"
#include "pointset.h"

#include "owdoc.h"
#include "ow2doc.h"

#include "owview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/


IMPLEMENT_DYNCREATE(CFusionPage, COWPropertyPage)

CFusionPage::CFusionPage() : COWPropertyPage(CFusionPage::IDD)
{
	//{{AFX_DATA_INIT(CFusionPage)
	//}}AFX_DATA_INIT
}

CFusionPage::~CFusionPage()
{
}

void CFusionPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFusionPage)
	DDX_Control(pDX, IDC_STMIXEDHSOHRGCHECK, m_STMixedHSOHRGCheckbox);
	DDX_Control(pDX, IDC_STHSOCHECK, m_STHSOCheckbox);
	DDX_Control(pDX, IDC_STHRGCHECK, m_STHRGCheckbox);
	DDX_Control(pDX, IDC_STATISTICMINEDIT, m_StatMinEdit);
	DDX_Control(pDX, IDC_STATISTICMAXEDIT, m_StatMaxEdit);
	DDX_Control(pDX, IDC_PRECLASSIFICATIONCHANNELCOMBO, m_USCWPreClassificationChannelCombobox);
	DDX_Control(pDX, IDC_FUSIONCHANNELCOMBO, m_FusionChannelCombobox);
	DDX_Control(pDX, IDC_CSCWPRECLASSIFICATIONCHECK, m_USCWPreClassificationCheckbox);
	DDX_Control(pDX, IDC_CSCWEXCLUSIONTHRESHOLDEDIT, m_USCWExclusionThresholdEdit);
	DDX_Control(pDX, IDC_CSCWEXCLUSIONTHRESHOLDCHECK, m_USCWExclusionThresholdCheckbox);
	DDX_Control(pDX, IDC_OKBUTTON, m_OKButton);
	DDX_Control(pDX, IDC_CANCELBUTTON, m_CancelButton);
	DDX_Control(pDX, IDC_APPLYBUTTON, m_ApplyButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFusionPage, COWPropertyPage)
	//{{AFX_MSG_MAP(CFusionPage)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_APPLYBUTTON, OnApplybutton)
	ON_BN_CLICKED(IDC_CANCELBUTTON, OnCancelbutton)
	ON_BN_CLICKED(IDC_OKBUTTON, OnOkbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int CFusionPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CWnd* myParent = GetParent();
	if(myParent!=NULL)
	{
		ASSERT(myParent->IsKindOf(RUNTIME_CLASS(CSettingsSheet)));
		m_pSettingsSheet = (CSettingsSheet*)myParent;
		m_pFusionSettings = m_pSettingsSheet->m_pFusionSettings;
	}
	return 0;
}

BOOL CFusionPage::OnSetActive() 
{
	UpdateSettingsFromDocToSheet();	
	return CPropertyPage::OnSetActive();
}

BOOL CFusionPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}

void CFusionPage::OnApplybutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
}

void CFusionPage::OnCancelbutton() 
{
	m_pSettingsSheet->ShowWindow(SW_HIDE);	
}

void CFusionPage::OnOkbutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}


BOOL CFusionPage::UpdateSettingsFromDocToSheet()
{
	CString myText;

	//0) assumptions for now
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc!=NULL) ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));

	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView!=NULL) ASSERT(pView->IsKindOf(RUNTIME_CLASS(COWView)));
	
	//1) copy view parameters from attached view
	if(pDoc) 
	{
		m_pFusionSettings->CopyAllValuesFrom(pDoc->m_pFusionSettings);
	}
	else 
	{
		m_pFusionSettings->CopyAllValuesFrom(NULL); //Erase
	}

	//2) set CFusionPage controls ...
	//pixelset parameters
	BOOL bState = FALSE;
	if(pDoc==NULL || pView==NULL || //not a ow2doc
		pDoc->m_pPointset==NULL	|| //extraction not performed yet
		(pDoc->m_pPointset!=NULL && pDoc->m_pSegmentset!=NULL))	//segmentation already performed
	{
		bState = FALSE;
	}
	else
	{
		bState = TRUE;
	}

	m_STMixedHSOHRGCheckbox.EnableWindow(bState);
	m_STHSOCheckbox.EnableWindow(bState);
	m_STHRGCheckbox.EnableWindow(bState);
	m_StatMinEdit.EnableWindow(FALSE);
	m_StatMaxEdit.EnableWindow(FALSE);
	m_USCWPreClassificationChannelCombobox.EnableWindow(bState);
	m_FusionChannelCombobox.EnableWindow(bState);
	m_USCWPreClassificationCheckbox.EnableWindow(bState);
	m_USCWExclusionThresholdEdit.EnableWindow(bState);
	m_USCWExclusionThresholdCheckbox.EnableWindow(bState);
	m_OKButton.EnableWindow(bState);
	m_ApplyButton.EnableWindow(bState);


	m_STHSOCheckbox.SetCheck(m_pFusionSettings->bSegTypeHSO);
	m_STHRGCheckbox.SetCheck(m_pFusionSettings->bSegTypeHRG);
	m_STMixedHSOHRGCheckbox.SetCheck(m_pFusionSettings->bSegTypeMixedHSOHRG);

	m_FusionChannelCombobox.ResetContent();
	if(pDoc!=NULL && pDoc->m_pPointset!=NULL)
	{			   	 
		for(int i=0; i<pDoc->GetNumberOfStatPerPoint(); i++)  //POINTSET_DEFAULT_NUMBEROFSTAT + POINTSET_EXTRA_NUMBEROFSTAT
		{
			CString myString(pszStatChannelNames[i]);
			m_FusionChannelCombobox.AddString(myString);
		}
		CString myString(pszStatChannelNames[m_pFusionSettings->idstatchannelUsedAsSimilarityCriteria]);
		m_FusionChannelCombobox.SelectString(0, myString);
	}
	double dfMin, dfMax;
	if(pDoc!=NULL && pDoc->m_pPointset!=NULL && pDoc->GetMinMaxForStatChannel(m_pFusionSettings->idstatchannelUsedAsSimilarityCriteria, dfMin, dfMax))
	{
		myText.Format(L"%f", dfMin);	
		m_StatMinEdit.SetWindowText(myText);
		myText.Format(L"%f", dfMax);
		m_StatMaxEdit.SetWindowText(myText);
	}
	else
	{
		myText = "";
		m_StatMinEdit.SetWindowText(myText);
		m_StatMaxEdit.SetWindowText(myText);
	}
	m_USCWExclusionThresholdCheckbox.SetCheck(m_pFusionSettings->bUseExclusionThreshold);
	myText.Format(L"%d", m_pFusionSettings->fExclusionThreshold);
	m_USCWExclusionThresholdEdit.SetWindowText(myText);
	m_USCWExclusionThresholdEdit.EnableWindow(bState & m_pFusionSettings->bUseExclusionThreshold);

	m_USCWPreClassificationCheckbox.SetCheck(m_pFusionSettings->bUsePreClassificationInfo);
	m_USCWPreClassificationChannelCombobox.EnableWindow(bState & m_pFusionSettings->bUsePreClassificationInfo);
	m_USCWPreClassificationChannelCombobox.ResetContent();
	if(pDoc!=NULL && pDoc->m_pPointset!=NULL)
	{			   	 
		for(int i=0; i<pDoc->GetNumberOfStatPerPoint(); i++)  //POINTSET_DEFAULT_NUMBEROFSTAT + POINTSET_EXTRA_NUMBEROFSTAT
		{
			CString myString(pszStatChannelNames[i]);
			m_USCWPreClassificationChannelCombobox.AddString(myString);
		}
		CString myString(pszStatChannelNames[m_pFusionSettings->idstatchannelUsedAsPreClassificationInfo]);
		m_USCWPreClassificationChannelCombobox.SelectString(0, myString);
	}


	return TRUE;
}

BOOL CFusionPage::UpdateSettingsFromSheetToDoc()
{
	//0) assumptions for now
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc!=NULL) ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));

	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView!=NULL) ASSERT(pView->IsKindOf(RUNTIME_CLASS(COWView)));

	/*
	char pszText[255];
	float fFloat, fValidatedFloat;
	long iValue, iValidatedValue;
	*/

	//3) copy sheet parameters from sheet to attached doc
	if(pDoc) pDoc->m_pFusionSettings->CopyAllValuesFrom(m_pFusionSettings);
	return TRUE;
}




/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// tracepage.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/*
#include "stdafx.h"
#include "oifii_app.h"

#include "owpropag.h"
#include "tracpage.h"

#include "setsheet.h"
#include "tracsett.h"
#include "pointset.h"

#include "owdoc.h"
#include "ow2doc.h"

#include "owview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

/////////////////////////////////////////////////////////////////////////////
// CTracePage property page

IMPLEMENT_DYNCREATE(CTracePage, COWPropertyPage)

CTracePage::CTracePage() : COWPropertyPage(CTracePage::IDD)
{
	//{{AFX_DATA_INIT(CTracePage)
	m_TLAStat2Combo = _T("");
	//}}AFX_DATA_INIT
}

CTracePage::~CTracePage()
{
}

void CTracePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTracePage)
	DDX_Control(pDX, IDC_TLASTAT4COMBO, m_TLAStat4Combo);
	DDX_Control(pDX, IDC_TLASTAT3COMBO, m_TLAStat3Combo);
	DDX_Control(pDX, IDC_TLASTAT1COMBO, m_TLAStat1Combo);
	DDX_Control(pDX, IDC_TLASTAT4CHECK, m_TLAStat4Checkbox);
	DDX_Control(pDX, IDC_TLASTAT3CHECK, m_TLAStat3Checkbox);
	DDX_Control(pDX, IDC_TLASTAT2CHECK, m_TLAStat2Checkbox);
	DDX_Control(pDX, IDC_TLASTAT1CHECK, m_TLAStat1Checkbox);
	DDX_Control(pDX, IDC_VORONOISETSTATIC, m_TPVoronoisetCheckbox);
	DDX_Control(pDX, IDC_TRIANGLESETSTATIC, m_TPTrianglesetCheckbox);
	DDX_Control(pDX, IDC_SEGMENTSETSTATIC, m_TPSegmentsetCheckbox);
	DDX_Control(pDX, IDC_POINTSETSTATIC, m_TPPointsetCheckbox);
	DDX_Control(pDX, IDC_PIXELSETSTATIC, m_TPPixelsetCheckbox);
	DDX_Control(pDX, IDC_OKBUTTON, m_OKButton);
	DDX_Control(pDX, IDC_APPLYBUTTON, m_ApplyButton);
	DDX_CBString(pDX, IDC_TLASTAT2COMBO, m_TLAStat2Combo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTracePage, COWPropertyPage)
	//{{AFX_MSG_MAP(CTracePage)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_APPLYBUTTON, OnApplybutton)
	ON_BN_CLICKED(IDC_CANCELBUTTON, OnCancelbutton)
	ON_BN_CLICKED(IDC_OKBUTTON, OnOkbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CTracePage::UpdateSettingsFromDocToSheet()
{
	//0) assumptions for now
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc!=NULL) ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));

	//1) copy trace parameters from attached doc
	if(pDoc) 
	{
		m_pTraceSettings->CopyAllValuesFrom(pDoc->m_pTraceSettings);
	}
	else 
	{
		//2) copy trace parameters from app
		m_pTraceSettings->CopyAllValuesFrom(((COIIIApp*)AfxGetApp())->m_pTraceSettings); 
	}

	m_TPPixelsetCheckbox.SetCheck(m_pTraceSettings->bTPPixelset);
	m_TPPointsetCheckbox.SetCheck(m_pTraceSettings->bTPPointset);
	m_TPTrianglesetCheckbox.SetCheck(m_pTraceSettings->bTPTriangleset);
	m_TPVoronoisetCheckbox.SetCheck(m_pTraceSettings->bTPVoronoiset);
	m_TPSegmentsetCheckbox.SetCheck(m_pTraceSettings->bTPSegmentset);

	return TRUE;
}

BOOL CTracePage::UpdateSettingsFromSheetToDoc()
{
	//0) assumptions for now
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();

	m_pTraceSettings->bTPPixelset = m_TPPixelsetCheckbox.GetCheck();
	m_pTraceSettings->bTPPointset = m_TPPointsetCheckbox.GetCheck();
	m_pTraceSettings->bTPTriangleset = m_TPTrianglesetCheckbox.GetCheck();
	m_pTraceSettings->bTPVoronoiset = m_TPVoronoisetCheckbox.GetCheck();
	m_pTraceSettings->bTPSegmentset = m_TPSegmentsetCheckbox.GetCheck();

	//1) update doc
	if(pDoc && pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)))
	{
		pDoc->m_pTraceSettings->CopyAllValuesFrom(m_pTraceSettings);
	}
	//2) update app
	((COIIIApp*)AfxGetApp())->m_pTraceSettings->CopyAllValuesFrom(m_pTraceSettings);

	return TRUE;
}

int CTracePage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COWPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	CWnd* myParent = GetParent();
	if(myParent!=NULL)
	{
		ASSERT(myParent->IsKindOf(RUNTIME_CLASS(CSettingsSheet)));
		m_pSettingsSheet = (CSettingsSheet*)myParent;
		m_pTraceSettings = m_pSettingsSheet->m_pTraceSettings;
	}
	return 0;
}

BOOL CTracePage::OnSetActive() 
{
	UpdateSettingsFromDocToSheet();	
	return COWPropertyPage::OnSetActive();
}

BOOL CTracePage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COWPropertyPage::OnKillActive();
}

void CTracePage::OnApplybutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
}

void CTracePage::OnCancelbutton() 
{
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}

void CTracePage::OnOkbutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// tranpage.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/*
#include "stdafx.h"
#include "oifii_app.h"

#include "owpropag.h"
#include "tranpage.h"

#include "setsheet.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include "transett.h"
#include "pointset.h"

#include "owdoc.h"
#include "ow2doc.h"

#include "owview.h"

#include "ow3view.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

/////////////////////////////////////////////////////////////////////////////
// CTransformationPage property page

IMPLEMENT_DYNCREATE(CTransformationPage, COWPropertyPage)

CTransformationPage::CTransformationPage() : COWPropertyPage(CTransformationPage::IDD)
{
	//{{AFX_DATA_INIT(CTransformationPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CTransformationPage::~CTransformationPage()
{
}

void CTransformationPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransformationPage)
	DDX_Control(pDX, IDC_DEFAULTSCOMBO, m_DefaultsComboBox);
	DDX_Control(pDX, IDC_VTYEDIT, m_VTYEdit);
	DDX_Control(pDX, IDC_VTXEDIT, m_VTXEdit);
	DDX_Control(pDX, IDC_VTWIDTHEDIT, m_VTWidthEdit);
	DDX_Control(pDX, IDC_VTHEIGHTEDIT, m_VTHeightEdit);
	DDX_Control(pDX, IDC_MTUSEITCHECK, m_MTUseItCheckbox);
	DDX_Control(pDX, IDC_MTTRANSLATEZEDIT, m_MTTranslateZEdit);
	DDX_Control(pDX, IDC_MTTRANSLATEYEDIT, m_MTTranslateYEdit);
	DDX_Control(pDX, IDC_MTTRANSLATEXEDIT, m_MTTranslateXEdit);
	DDX_Control(pDX, IDC_MTSCALEZEDIT, m_MTScaleZEdit);
	DDX_Control(pDX, IDC_MTSCALEYEDIT, m_MTScaleYEdit);
	DDX_Control(pDX, IDC_MTSCALEXEDIT, m_MTScaleXEdit);
	DDX_Control(pDX, IDC_MTROTATEZEDIT, m_MTRotateZEdit);
	DDX_Control(pDX, IDC_MTROTATEYEDIT, m_MTRotateYEdit);
	DDX_Control(pDX, IDC_MTROTATEXEDIT, m_MTRotateXEdit);
	DDX_Control(pDX, IDC_MTRELATIVECHECK, m_MTRelativeCheckbox);
	DDX_Control(pDX, IDC_MTIGNORECHECK, m_MTIgnoreCheckbox);
	DDX_Control(pDX, IDC_MTABSOLUTECHECK, m_MTAbsoluteCheckbox);
	DDX_Control(pDX, IDC_CANCELBUTTON, m_CancelButton);
	DDX_Control(pDX, IDC_OKBUTTON, m_OKButton);
	DDX_Control(pDX, IDC_APPLYBUTTON, m_ApplyButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransformationPage, COWPropertyPage)
	//{{AFX_MSG_MAP(CTransformationPage)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_APPLYBUTTON, OnApplybutton)
	ON_BN_CLICKED(IDC_CANCELBUTTON, OnCancelbutton)
	ON_BN_CLICKED(IDC_OKBUTTON, OnOkbutton)
	ON_CBN_SELCHANGE(IDC_DEFAULTSCOMBO, OnSelchangeDefaultscombo)
	ON_EN_KILLFOCUS(IDC_MTROTATEXEDIT, OnKillfocusMtrotatexedit)
	ON_EN_SETFOCUS(IDC_MTROTATEXEDIT, OnSetfocusMtrotatexedit)
	ON_EN_SETFOCUS(IDC_MTROTATEYEDIT, OnSetfocusMtrotateyedit)
	ON_EN_SETFOCUS(IDC_MTROTATEZEDIT, OnSetfocusMtrotatezedit)
	ON_EN_SETFOCUS(IDC_MTSCALEXEDIT, OnSetfocusMtscalexedit)
	ON_EN_SETFOCUS(IDC_MTSCALEYEDIT, OnSetfocusMtscaleyedit)
	ON_EN_SETFOCUS(IDC_MTSCALEZEDIT, OnSetfocusMtscalezedit)
	ON_EN_SETFOCUS(IDC_MTTRANSLATEXEDIT, OnSetfocusMttranslatexedit)
	ON_EN_SETFOCUS(IDC_MTTRANSLATEYEDIT, OnSetfocusMttranslateyedit)
	ON_EN_SETFOCUS(IDC_MTTRANSLATEZEDIT, OnSetfocusMttranslatezedit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransformationPage message handlers

BOOL CTransformationPage::UpdateSettingsFromDocToSheet()
{
	CString myText;

	//0) assumptions for now
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc!=NULL) ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));

	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView!=NULL) ASSERT(pView->IsKindOf(RUNTIME_CLASS(COWView)));
	
	//1) copy view parameters from attached view
	if(pView) 
	{
		m_pTransformationSettings->CopyAllValuesFrom(pView->m_pTransformationSettings);
	}
	else if(pDoc)
	{
		m_pTransformationSettings->CopyAllValuesFrom(pDoc->m_pTransformationSettings);
	}
	else 
	{
		m_pTransformationSettings->CopyAllValuesFrom(NULL); //Erase
	}

	//2) set CProjectionPage controls ...
	BOOL bState = FALSE;
	BOOL bIs3DView = FALSE;
	if(pView) bIs3DView = pView->IsKindOf(RUNTIME_CLASS(COW3View));
	if(pDoc==NULL || pView==NULL || !bIs3DView)
	{
		bState = FALSE;
	}
	else
	{
		bState = TRUE;
	}

	//enable/disable controls
	m_ApplyButton.EnableWindow(bState);
	m_OKButton.EnableWindow(bState);

	m_VTYEdit.EnableWindow(FALSE);
	m_VTXEdit.EnableWindow(FALSE);
	m_VTWidthEdit.EnableWindow(FALSE);
	m_VTHeightEdit.EnableWindow(FALSE);
	m_MTTranslateZEdit.EnableWindow(bState);
	m_MTTranslateYEdit.EnableWindow(bState);
	m_MTTranslateXEdit.EnableWindow(bState);
	m_MTScaleZEdit.EnableWindow(bState);
	m_MTScaleYEdit.EnableWindow(bState);
	m_MTScaleXEdit.EnableWindow(bState);
	m_MTRotateZEdit.EnableWindow(bState);
	m_MTRotateYEdit.EnableWindow(bState);
	m_MTRotateXEdit.EnableWindow(bState);
	m_MTUseItCheckbox.EnableWindow(bState);
	m_MTIgnoreCheckbox.EnableWindow(bState);
	m_MTRelativeCheckbox.EnableWindow(bState);
	m_MTAbsoluteCheckbox.EnableWindow(bState);
	m_DefaultsComboBox.EnableWindow(bState);

	//set controls values
	myText.Format(L"%f", m_pTransformationSettings->fVTX);
	m_VTXEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pTransformationSettings->fVTY);
	m_VTYEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pTransformationSettings->fVTWidth);
	m_VTWidthEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pTransformationSettings->fVTHeight);
	m_VTHeightEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pTransformationSettings->fMTTranslateZ);
	m_MTTranslateZEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pTransformationSettings->fMTTranslateY);
	m_MTTranslateYEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pTransformationSettings->fMTTranslateX);
	m_MTTranslateXEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pTransformationSettings->fMTScaleZ);
	m_MTScaleZEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pTransformationSettings->fMTScaleY);
	m_MTScaleYEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pTransformationSettings->fMTScaleX);
	m_MTScaleXEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pTransformationSettings->fMTRotateZ);
	m_MTRotateZEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pTransformationSettings->fMTRotateY);
	m_MTRotateYEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pTransformationSettings->fMTRotateX);
	m_MTRotateXEdit.SetWindowText(myText);
	m_MTUseItCheckbox.SetCheck(m_pTransformationSettings->bMTUseIt);
	m_MTIgnoreCheckbox.SetCheck(m_pTransformationSettings->bMTIgnore);
	m_MTRelativeCheckbox.SetCheck(m_pTransformationSettings->bMTRelative);
	m_MTAbsoluteCheckbox.SetCheck(m_pTransformationSettings->bMTAbsolute);
	return TRUE;
}

BOOL CTransformationPage::UpdateSettingsFromSheetToDoc()
{
	WCHAR pszText[255];
	float fFloat, fValidatedFloat;

	//0) assumptions for now
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	ASSERT(pDoc!=NULL);
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));

	COWView* pView = m_pSettingsSheet->GetAttachedView();
	ASSERT(pView->IsKindOf(RUNTIME_CLASS(COWView)));

	m_VTXEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pTransformationSettings->fVTX = fValidatedFloat;
	m_VTYEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pTransformationSettings->fVTY = fValidatedFloat;
	m_VTWidthEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pTransformationSettings->fVTWidth = fValidatedFloat;
	m_VTHeightEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pTransformationSettings->fVTHeight = fValidatedFloat;

	m_MTTranslateZEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pTransformationSettings->fMTTranslateZ = fValidatedFloat;
	m_MTTranslateYEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pTransformationSettings->fMTTranslateY = fValidatedFloat;
	m_MTTranslateXEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pTransformationSettings->fMTTranslateX = fValidatedFloat;

	m_MTScaleZEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pTransformationSettings->fMTScaleZ = fValidatedFloat;
	m_MTScaleYEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pTransformationSettings->fMTScaleY = fValidatedFloat;
	m_MTScaleXEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pTransformationSettings->fMTScaleX = fValidatedFloat;

	m_MTRotateZEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pTransformationSettings->fMTRotateZ = fValidatedFloat;
	m_MTRotateYEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pTransformationSettings->fMTRotateY = fValidatedFloat;
	m_MTRotateXEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pTransformationSettings->fMTRotateX = fValidatedFloat;

	m_pTransformationSettings->bMTUseIt = m_MTUseItCheckbox.GetCheck();
	m_pTransformationSettings->bMTIgnore = m_MTIgnoreCheckbox.GetCheck();
	m_pTransformationSettings->bMTRelative = m_MTRelativeCheckbox.GetCheck();
	m_pTransformationSettings->bMTAbsolute = m_MTAbsoluteCheckbox.GetCheck();


	ASSERT(pView->IsKindOf(RUNTIME_CLASS(COW3View)));
	//3) copy view parameters from sheet to attached view
	pView->m_pTransformationSettings->CopyAllValuesFrom(m_pTransformationSettings);
	//if(pView) pView->PostMessage(OWM_VIEW_UPDATEINFO, (WPARAM)m_pSettingsSheet);

	//if(pDoc) pDoc->UpdateAllViews(NULL);
	Invalidate(FALSE);
	return TRUE;
}

int CTransformationPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COWPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	CWnd* myParent = GetParent();
	if(myParent!=NULL)
	{
		ASSERT(myParent->IsKindOf(RUNTIME_CLASS(CSettingsSheet)));
		m_pSettingsSheet = (CSettingsSheet*)myParent;
		m_pTransformationSettings = m_pSettingsSheet->m_pTransformationSettings;
	}
	return 0;
}

BOOL CTransformationPage::OnSetActive() 
{
	UpdateSettingsFromDocToSheet();	
	return COWPropertyPage::OnSetActive();
}

BOOL CTransformationPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COWPropertyPage::OnKillActive();
}

void CTransformationPage::OnApplybutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
}

void CTransformationPage::OnCancelbutton() 
{
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}

void CTransformationPage::OnOkbutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}

void CTransformationPage::OnSelchangeDefaultscombo() 
{
	int idDefault = m_DefaultsComboBox.GetCurSel();
	switch(idDefault)
	{
		case 0:	//VERTICAL
			m_pTransformationSettings->fMTTranslateZ = 0.0;
			m_pTransformationSettings->fMTTranslateY = 60.0;
			m_pTransformationSettings->fMTTranslateX = -100.0;
			m_pTransformationSettings->fMTScaleZ = 1.0;
			m_pTransformationSettings->fMTScaleY = 1.2;
			m_pTransformationSettings->fMTScaleX = 1.2;
			m_pTransformationSettings->fMTRotateZ = -50.0;
			m_pTransformationSettings->fMTRotateY = -10.0;
			m_pTransformationSettings->fMTRotateX = -60.0;
			break;
		case 1:	//HORIZONTAL
			m_pTransformationSettings->fMTTranslateZ = 0.0;
			m_pTransformationSettings->fMTTranslateY = 0.0;
			m_pTransformationSettings->fMTTranslateX = 0.0;
			m_pTransformationSettings->fMTScaleZ = 1.0;
			m_pTransformationSettings->fMTScaleY = 1.0;
			m_pTransformationSettings->fMTScaleX = 1.0;
			m_pTransformationSettings->fMTRotateZ = 0.0;
			m_pTransformationSettings->fMTRotateY = 40.0;
			m_pTransformationSettings->fMTRotateX = -30.0;
			break;
		case 2:	//ISOMETRIC
			m_pTransformationSettings->fMTTranslateZ = 0.0;
			m_pTransformationSettings->fMTTranslateY = 60.0;
			m_pTransformationSettings->fMTTranslateX = -100.0;
			m_pTransformationSettings->fMTScaleZ = 1.0;
			m_pTransformationSettings->fMTScaleY = 1.2;
			m_pTransformationSettings->fMTScaleX = 1.2;
			m_pTransformationSettings->fMTRotateZ = -15.0;
			m_pTransformationSettings->fMTRotateY = 0.0;
			m_pTransformationSettings->fMTRotateX = -15.0;
			break;
		case 3:	//CUSTOM
			//do not change anything
			break;

	}

	//if not CUSTOM, modify editboxes parameters
	if(idDefault!=3)
	{
		CString myText;

		myText.Format(L"%f", m_pTransformationSettings->fMTTranslateZ);
		m_MTTranslateZEdit.SetWindowText(myText);
		myText.Format(L"%f", m_pTransformationSettings->fMTTranslateY);
		m_MTTranslateYEdit.SetWindowText(myText);
		myText.Format(L"%f", m_pTransformationSettings->fMTTranslateX);
		m_MTTranslateXEdit.SetWindowText(myText);
		myText.Format(L"%f", m_pTransformationSettings->fMTScaleZ);
		m_MTScaleZEdit.SetWindowText(myText);
		myText.Format(L"%f", m_pTransformationSettings->fMTScaleY);
		m_MTScaleYEdit.SetWindowText(myText);
		myText.Format(L"%f", m_pTransformationSettings->fMTScaleX);
		m_MTScaleXEdit.SetWindowText(myText);
		myText.Format(L"%f", m_pTransformationSettings->fMTRotateZ);
		m_MTRotateZEdit.SetWindowText(myText);
		myText.Format(L"%f", m_pTransformationSettings->fMTRotateY);
		m_MTRotateYEdit.SetWindowText(myText);
		myText.Format(L"%f", m_pTransformationSettings->fMTRotateX);
		m_MTRotateXEdit.SetWindowText(myText);

		COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
		COWView* pView = m_pSettingsSheet->GetAttachedView();	
		if(pDoc!=NULL && pView!=NULL) 
		{
			UpdateSettingsFromSheetToDoc();
			pView->Invalidate(TRUE);
		}
	}
}

BOOL CTransformationPage::OnInitDialog() 
{
	COWPropertyPage::OnInitDialog();
	
	m_DefaultsComboBox.AddString(L"VERTICAL");
	m_DefaultsComboBox.AddString(L"HORIZONTAL");
	m_DefaultsComboBox.AddString(L"ISOMETRIC");
	m_DefaultsComboBox.AddString(L"CUSTOM");
	m_DefaultsComboBox.SelectString(0, L"VERTICAL");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTransformationPage::OnKillfocusMtrotatexedit() 
{
}

void CTransformationPage::SetDefaultComboboxToCUSTOM()
{
	int idDefault = m_DefaultsComboBox.GetCurSel();
	//if not CUSTOM
	if(idDefault!=3)
	{
		m_DefaultsComboBox.SelectString(0, L"CUSTOM");	
	}	
}

void CTransformationPage::OnSetfocusMtrotatexedit() 
{
	SetDefaultComboboxToCUSTOM();
}

void CTransformationPage::OnSetfocusMtrotateyedit() 
{
	SetDefaultComboboxToCUSTOM();
}

void CTransformationPage::OnSetfocusMtrotatezedit() 
{
	SetDefaultComboboxToCUSTOM();
}

void CTransformationPage::OnSetfocusMtscalexedit() 
{
	SetDefaultComboboxToCUSTOM();
}

void CTransformationPage::OnSetfocusMtscaleyedit() 
{
	SetDefaultComboboxToCUSTOM();
}

void CTransformationPage::OnSetfocusMtscalezedit() 
{
	SetDefaultComboboxToCUSTOM();
}

void CTransformationPage::OnSetfocusMttranslatexedit() 
{
	SetDefaultComboboxToCUSTOM();
}

void CTransformationPage::OnSetfocusMttranslateyedit() 
{
	SetDefaultComboboxToCUSTOM();
}

void CTransformationPage::OnSetfocusMttranslatezedit() 
{
	SetDefaultComboboxToCUSTOM();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// projpage.cpp : implementation file
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "oifii_app.h"
//#include "owpropag.h"

/*
#include "projpage.h"

#include "setsheet.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include "projsett.h"
#include "pointset.h"

#include "owdoc.h"
#include "ow2doc.h"

#include "owview.h"
#include "ow3view.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/


IMPLEMENT_DYNCREATE(CProjectionPage, COWPropertyPage)

CProjectionPage::CProjectionPage() : COWPropertyPage(CProjectionPage::IDD)
{
	//{{AFX_DATA_INIT(CProjectionPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CProjectionPage::~CProjectionPage()
{
}

void CProjectionPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectionPage)
	DDX_Control(pDX, IDC_VVSTOPEDIT, m_VVSTopEdit);
	DDX_Control(pDX, IDC_VVSRIGHTEDIT, m_VVSRightEdit);
	DDX_Control(pDX, IDC_VVSPERSPECTIVECHECK, m_VVSPerspectiveCheckbox);
	DDX_Control(pDX, IDC_VVSORTHOGRAPHICCHECK, m_VVSOrthoCheckbox);
	DDX_Control(pDX, IDC_VVSNEAREDIT, m_VVSNearEdit);
	DDX_Control(pDX, IDC_VVSLEFTEDIT, m_VVSLeftEdit);
	DDX_Control(pDX, IDC_VVSFAREDIT, m_VVSFarEdit);
	DDX_Control(pDX, IDC_VVSBOTTOMEDIT, m_VVSBottomEdit);
	DDX_Control(pDX, IDC_LAUPZEDIT, m_LAUpZEdit);
	DDX_Control(pDX, IDC_LAUPYEDIT, m_LAUpYEdit);
	DDX_Control(pDX, IDC_LAUPXEDIT, m_LAUpXEdit);
	DDX_Control(pDX, IDC_LAUSEITCHECK, m_LAUseItCheckbox);
	DDX_Control(pDX, IDC_LAIGNORECHECK, m_LAIgnoreCheckbox);
	DDX_Control(pDX, IDC_LAEYEZEDIT, m_LAEyeZEdit);
	DDX_Control(pDX, IDC_LAEYEYEDIT, m_LAEyeYEdit);
	DDX_Control(pDX, IDC_LAEYEXEDIT, m_LAEyeXEdit);
	DDX_Control(pDX, IDC_LACENTERZEDIT, m_LACenterZEdit);
	DDX_Control(pDX, IDC_LACENTERYEDIT, m_LACenterYEdit);
	DDX_Control(pDX, IDC_LACENTERXEDIT, m_LACenterXEdit);
	DDX_Control(pDX, IDC_OKBUTTON, m_OKButton);
	DDX_Control(pDX, IDC_APPLYBUTTON, m_ApplyButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProjectionPage, COWPropertyPage)
	//{{AFX_MSG_MAP(CProjectionPage)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_APPLYBUTTON, OnApplybutton)
	ON_BN_CLICKED(IDC_CANCELBUTTON, OnCancelbutton)
	ON_BN_CLICKED(IDC_OKBUTTON, OnOkbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int CProjectionPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COWPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	CWnd* myParent = GetParent();
	if(myParent!=NULL)
	{
		ASSERT(myParent->IsKindOf(RUNTIME_CLASS(CSettingsSheet)));
		m_pSettingsSheet = (CSettingsSheet*)myParent;
		m_pProjectionSettings = m_pSettingsSheet->m_pProjectionSettings;
	}
	return 0;
}

BOOL CProjectionPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COWPropertyPage::OnKillActive();
}

BOOL CProjectionPage::OnSetActive() 
{
	UpdateSettingsFromDocToSheet();	
	return COWPropertyPage::OnSetActive();
}

void CProjectionPage::OnApplybutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
}

void CProjectionPage::OnCancelbutton() 
{
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}

void CProjectionPage::OnOkbutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}

BOOL CProjectionPage::UpdateSettingsFromDocToSheet()
{
	CString myText;

	//0) assumptions for now
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc!=NULL) ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));

	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView!=NULL) ASSERT(pView->IsKindOf(RUNTIME_CLASS(COWView)));
	
	//1) copy view parameters from attached view
	if(pView) 
	{
		m_pProjectionSettings->CopyAllValuesFrom(pView->m_pProjectionSettings);
	}
	else if(pDoc)
	{
		m_pProjectionSettings->CopyAllValuesFrom(pDoc->m_pProjectionSettings);
	}
	else 
	{
		m_pProjectionSettings->CopyAllValuesFrom(NULL); //Erase
	}

	//2) set CProjectionPage controls ...
	BOOL bState = FALSE;
	BOOL bIs3DView = FALSE;
	if(pView) bIs3DView = pView->IsKindOf(RUNTIME_CLASS(COW3View));
	if(pDoc==NULL || pView==NULL || !bIs3DView)
	{
		bState = FALSE;
	}
	else
	{
		bState = TRUE;
	}

	//enable/disable controls
	m_ApplyButton.EnableWindow(bState);
	m_OKButton.EnableWindow(bState);
	m_VVSTopEdit.EnableWindow(bState);
	m_VVSRightEdit.EnableWindow(bState);
	m_VVSPerspectiveCheckbox.EnableWindow(bState);
	m_VVSOrthoCheckbox.EnableWindow(bState);
	m_VVSNearEdit.EnableWindow(bState);
	m_VVSLeftEdit.EnableWindow(bState);
	m_VVSFarEdit.EnableWindow(bState);
	m_VVSBottomEdit.EnableWindow(bState);
	m_LAUpZEdit.EnableWindow(bState);
	m_LAUpYEdit.EnableWindow(bState);
	m_LAUpXEdit.EnableWindow(bState);
	m_LAUseItCheckbox.EnableWindow(bState);
	m_LAIgnoreCheckbox.EnableWindow(bState);
	m_LAEyeZEdit.EnableWindow(bState);
	m_LAEyeYEdit.EnableWindow(bState);
	m_LAEyeXEdit.EnableWindow(bState);
	m_LACenterZEdit.EnableWindow(bState);
	m_LACenterYEdit.EnableWindow(bState);
	m_LACenterXEdit.EnableWindow(bState);

	//set controls values
	myText.Format(L"%f", m_pProjectionSettings->fVVSTop);
	m_VVSTopEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pProjectionSettings->fVVSBottom);
	m_VVSBottomEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pProjectionSettings->fVVSLeft);
	m_VVSLeftEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pProjectionSettings->fVVSRight);
	m_VVSRightEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pProjectionSettings->fVVSNear);
	m_VVSNearEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pProjectionSettings->fVVSFar);
	m_VVSFarEdit.SetWindowText(myText);

	m_VVSPerspectiveCheckbox.SetCheck(m_pProjectionSettings->bVVSPerspective);
	m_VVSOrthoCheckbox.SetCheck(m_pProjectionSettings->bVVSOrthogonal);

	myText.Format(L"%f", m_pProjectionSettings->fLAUpZ);
	m_LAUpZEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pProjectionSettings->fLAUpY);
	m_LAUpYEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pProjectionSettings->fLAUpX);
	m_LAUpXEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pProjectionSettings->fLAEyeZ);
	m_LAEyeZEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pProjectionSettings->fLAEyeY);
	m_LAEyeYEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pProjectionSettings->fLAEyeX);
	m_LAEyeXEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pProjectionSettings->fLACenterZ);
	m_LACenterZEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pProjectionSettings->fLACenterY);
	m_LACenterYEdit.SetWindowText(myText);
	myText.Format(L"%f", m_pProjectionSettings->fLACenterX);
	m_LACenterXEdit.SetWindowText(myText);

	m_LAUseItCheckbox.SetCheck(m_pProjectionSettings->bLAUseIt);
	m_LAIgnoreCheckbox.SetCheck(m_pProjectionSettings->bLAIgnore);

	return TRUE;
}

BOOL CProjectionPage::UpdateSettingsFromSheetToDoc()
{
	WCHAR pszText[255];
	float fFloat, fValidatedFloat;

	//0) assumptions for now
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	ASSERT(pDoc!=NULL);
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));

	COWView* pView = m_pSettingsSheet->GetAttachedView();
	ASSERT(pView->IsKindOf(RUNTIME_CLASS(COWView)));

	m_VVSTopEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pProjectionSettings->fVVSTop = fValidatedFloat;

	m_VVSBottomEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pProjectionSettings->fVVSBottom = fValidatedFloat;

	m_VVSLeftEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pProjectionSettings->fVVSLeft = fValidatedFloat;

	m_VVSRightEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pProjectionSettings->fVVSRight = fValidatedFloat;

	m_VVSNearEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pProjectionSettings->fVVSNear = fValidatedFloat;

	m_VVSFarEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pProjectionSettings->fVVSFar = fValidatedFloat;

	m_pProjectionSettings->bVVSPerspective = m_VVSPerspectiveCheckbox.GetCheck();
	m_pProjectionSettings->bVVSOrthogonal = m_VVSOrthoCheckbox.GetCheck();

	m_LAUpZEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pProjectionSettings->fLAUpZ = fValidatedFloat;

	m_LAUpYEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pProjectionSettings->fLAUpY = fValidatedFloat;

	m_LAUpXEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pProjectionSettings->fLAUpX = fValidatedFloat;

	m_LAEyeZEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pProjectionSettings->fLAEyeZ = fValidatedFloat;

	m_LAEyeYEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pProjectionSettings->fLAEyeY = fValidatedFloat;

	m_LAEyeXEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pProjectionSettings->fLAEyeX = fValidatedFloat;

	m_LACenterZEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pProjectionSettings->fLACenterZ = fValidatedFloat;

	m_LACenterYEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pProjectionSettings->fLACenterY = fValidatedFloat;

	m_LACenterXEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pProjectionSettings->fLACenterX = fValidatedFloat;

	m_pProjectionSettings->bLAUseIt = m_LAUseItCheckbox.GetCheck();
	m_pProjectionSettings->bLAIgnore = m_LAIgnoreCheckbox.GetCheck();

	ASSERT(pView->IsKindOf(RUNTIME_CLASS(COW3View)));
	//3) copy view parameters from sheet to attached view
	pView->m_pProjectionSettings->CopyAllValuesFrom(m_pProjectionSettings);
	//if(pView) pView->PostMessage(OWM_VIEW_UPDATEINFO, (WPARAM)m_pSettingsSheet);

	//if(pDoc) pDoc->UpdateAllViews(NULL);
	Invalidate(FALSE);
	return TRUE;
}








///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// viewpage.cpp : implementation file
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*
#include "stdafx.h"
#include "oifii_app.h"

#include "owpropag.h"
#include "viewpage.h"

#include "setsheet.h"
#include "viewsett.h"
#include "pointset.h"

#include "owdoc.h"
#include "ow2doc.h"

#include "owview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/


IMPLEMENT_DYNCREATE(CViewDataPage, COWPropertyPage)

CViewDataPage::CViewDataPage() : COWPropertyPage(CViewDataPage::IDD)
{
	//{{AFX_DATA_INIT(CViewDataPage)
	//}}AFX_DATA_INIT
	m_pSettingsSheet = NULL;
	m_pViewDataSettings = NULL;
}

CViewDataPage::~CViewDataPage()
{
}

void CViewDataPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewDataPage)
	DDX_Control(pDX, IDC_CLASSSETLABELS, m_ClasssetLabelsButton);
	DDX_Control(pDX, IDC_OKBUTTON, m_OKButton);
	DDX_Control(pDX, IDC_APPLYBUTTON, m_ApplyButton);
	DDX_Control(pDX, IDC_TRIANGLESETCENTERSSTATIC, m_TrianglesetCentersButton);
	DDX_Control(pDX, IDC_VORONOISETSTATIC, m_VoronoisetButton);
	DDX_Control(pDX, IDC_VORONOISETEDIT, m_VoronoisetEdit);
	DDX_Control(pDX, IDC_VIEWCHANNELEDIT, m_ViewChannelEdit);
	DDX_Control(pDX, IDC_VIEWCHANNELCOMBO, m_ViewChannelComboBox);
	DDX_Control(pDX, IDC_TRIANGLESETSTATIC, m_TrianglesetButton);
	DDX_Control(pDX, IDC_TRIANGLESETLABELSSTATIC, m_TrianglesetLabelsButton);
	DDX_Control(pDX, IDC_TRIANGLESETEDIT, m_TrianglesetEdit);
	DDX_Control(pDX, IDC_SEGMENTSETSTATIC, m_SegmentsetButton);
	DDX_Control(pDX, IDC_SEGMENTSETLABELSSTATIC, m_SegmentsetLabelsButton);
	DDX_Control(pDX, IDC_SEGMENTSETEDIT, m_SegmentsetEdit);
	DDX_Control(pDX, IDC_POINTSETLABELSSTATIC, m_PointsetLabelsButton);
	DDX_Control(pDX, IDC_POINTSETSTATIC, m_PointsetButton);
	DDX_Control(pDX, IDC_POINTSETEDIT, m_PointsetEdit);
	DDX_Control(pDX, IDC_PIXELSETEDIT, m_PixelsetEdit);
	DDX_Control(pDX, IDC_PIXELSETSTATIC, m_PixelsetButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewDataPage, COWPropertyPage)
	//{{AFX_MSG_MAP(CViewDataPage)
	ON_BN_CLICKED(IDC_CANCELBUTTON, OnCancelbutton)
	ON_BN_CLICKED(IDC_OKBUTTON, OnOkbutton)
	ON_BN_CLICKED(IDC_APPLYBUTTON, OnApplybutton)
	ON_EN_CHANGE(IDC_PIXELSETEDIT, OnChangePixelsetedit)
	ON_BN_CLICKED(IDC_PIXELSETSTATIC, OnPixelsetstatic)
	ON_EN_CHANGE(IDC_POINTSETEDIT, OnChangePointsetedit)
	ON_BN_CLICKED(IDC_POINTSETSTATIC, OnPointsetstatic)
	ON_EN_CHANGE(IDC_SEGMENTSETEDIT, OnChangeSegmentsetedit)
	ON_BN_CLICKED(IDC_SEGMENTSETSTATIC, OnSegmentsetstatic)
	ON_EN_CHANGE(IDC_TRIANGLESETEDIT, OnChangeTrianglesetedit)
	ON_BN_CLICKED(IDC_TRIANGLESETSTATIC, OnTrianglesetstatic)
	ON_CBN_SELCHANGE(IDC_VIEWCHANNELCOMBO, OnSelchangeViewchannelcombo)
	ON_EN_CHANGE(IDC_VIEWCHANNELEDIT, OnChangeViewchanneledit)
	ON_EN_CHANGE(IDC_VORONOISETEDIT, OnChangeVoronoisetedit)
	ON_BN_CLICKED(IDC_VORONOISETSTATIC, OnVoronoisetstatic)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_TRIANGLESETCENTERSSTATIC, OnTrianglesetcentersstatic)
	ON_BN_CLICKED(IDC_CLASSSETLABELS, OnClasssetlabels)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDataPage message handlers

void CViewDataPage::OnCancelbutton() 
{
	m_pSettingsSheet->ShowWindow(SW_HIDE);	
}

void CViewDataPage::OnOkbutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}

void CViewDataPage::OnApplybutton() 
{
	UpdateSettingsFromSheetToDoc();	
}

void CViewDataPage::OnChangePixelsetedit() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewDataPage::OnPixelsetstatic() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewDataPage::OnChangePointsetedit() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewDataPage::OnPointsetstatic() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewDataPage::OnChangeSegmentsetedit() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewDataPage::OnSegmentsetstatic() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewDataPage::OnChangeTrianglesetedit() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewDataPage::OnTrianglesetstatic() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewDataPage::OnSelchangeViewchannelcombo() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewDataPage::OnChangeViewchanneledit() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewDataPage::OnChangeVoronoisetedit() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewDataPage::OnVoronoisetstatic() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CViewDataPage::OnSetActive() 
{
    UpdateSettingsFromDocToSheet();
	return CPropertyPage::OnSetActive();
}

BOOL CViewDataPage::OnKillActive() 
{
	//UpdateViewDataSettingsFromSheetToView();	
	return CPropertyPage::OnKillActive();
}

//copy view settings from view to sheet
BOOL CViewDataPage::UpdateSettingsFromDocToSheet()
{
	CString myText;

	//0) assumptions for now
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc!=NULL) ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));

	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView!=NULL) ASSERT(pView->IsKindOf(RUNTIME_CLASS(COWView)));
	

	//1) copy view parameters from attached view
	if(pView) 
	{
		m_pViewDataSettings->CopyAllValuesFrom(pView->m_pViewDataSettings);
	}
	else 
	{
		m_pViewDataSettings->CopyAllValuesFrom(NULL); //Erase
	}

	//2) set CViewDataPage controls ...
	//pixelset parameters
	if(pDoc==NULL || pView==NULL)
	{
		m_ApplyButton.EnableWindow(FALSE);
		m_OKButton.EnableWindow(FALSE);

	}
	else
	{
		m_ApplyButton.EnableWindow(TRUE);
		m_OKButton.EnableWindow(TRUE);
	}
	if(pDoc!=NULL && pDoc->m_pPixelset!=NULL)
	{
		m_PixelsetButton.EnableWindow(TRUE);
		m_PixelsetEdit.EnableWindow(TRUE);
	}
	else 
	{
		m_PixelsetButton.EnableWindow(FALSE);
		m_PixelsetEdit.EnableWindow(FALSE);
	}
	m_PixelsetButton.SetCheck(m_pViewDataSettings->bDrawPixelset);
	myText.Format(L"%.2f", m_pViewDataSettings->fPixelset_zoffset);
	m_PixelsetEdit.SetWindowText(myText);

	//pointset parameters
	if(pDoc!=NULL && pDoc->m_pPointset!=NULL)
	{
		m_PointsetButton.EnableWindow(TRUE);
		m_PointsetEdit.EnableWindow(TRUE);
		m_PointsetLabelsButton.EnableWindow(TRUE);
	}
	else 
	{
		m_PointsetButton.EnableWindow(FALSE);
		m_PointsetEdit.EnableWindow(FALSE);
		m_PointsetLabelsButton.EnableWindow(FALSE);
	}
	m_PointsetButton.SetCheck(m_pViewDataSettings->bDrawPointset);
	myText.Format(L"%.2f", m_pViewDataSettings->fPointset_zoffset);
	m_PointsetEdit.SetWindowText(myText);
	m_PointsetLabelsButton.SetCheck(m_pViewDataSettings->bDrawPointsetLabels);

	//voronoiset parameter
	if(pDoc!=NULL && pDoc->m_pPointset!=NULL)
	{
		m_VoronoisetButton.EnableWindow(TRUE);
		m_VoronoisetEdit.EnableWindow(TRUE);
	}
	else 
	{
		m_VoronoisetButton.EnableWindow(FALSE);
		m_VoronoisetEdit.EnableWindow(FALSE);
	}
    m_VoronoisetButton.SetCheck(m_pViewDataSettings->bDrawVoronoiset);
	myText.Format(L"%.2f", m_pViewDataSettings->fVoronoiset_zoffset);
	m_VoronoisetEdit.SetWindowText(myText);

	//triangleset parameter
	if(pDoc!=NULL && pDoc->m_pPointset!=NULL)
	{
		m_TrianglesetButton.EnableWindow(TRUE);
		m_TrianglesetEdit.EnableWindow(TRUE);
		m_ViewChannelEdit.EnableWindow(TRUE);
		m_ViewChannelComboBox.EnableWindow(TRUE);
		m_TrianglesetLabelsButton.EnableWindow(TRUE);
		m_TrianglesetCentersButton.EnableWindow(TRUE);
	}
	else 
	{
		m_TrianglesetButton.EnableWindow(FALSE);
		m_TrianglesetEdit.EnableWindow(FALSE);
		m_ViewChannelEdit.EnableWindow(FALSE);
		m_ViewChannelComboBox.EnableWindow(FALSE);
		m_TrianglesetLabelsButton.EnableWindow(FALSE);
		m_TrianglesetCentersButton.EnableWindow(FALSE);
	}
	m_TrianglesetButton.SetCheck(m_pViewDataSettings->bDrawTriangleset);
	myText.Format(L"%.2f", m_pViewDataSettings->fTriangleset_zoffset);
	m_TrianglesetEdit.SetWindowText(myText);
	myText.Format(L"%.2f", m_pViewDataSettings->fTriangleset_zrange);
	m_ViewChannelEdit.SetWindowText(myText);
	m_ViewChannelComboBox.ResetContent();
	if(pDoc!=NULL && pDoc->m_pPointset!=NULL)
	{
		for(int i=0; i<pDoc->GetNumberOfStatPerPoint(); i++) //POINTSET_DEFAULT_NUMBEROFSTAT + POINTSET_EXTRA_NUMBEROFSTAT
		{
			CString myString(pszStatChannelNames[i]);
			m_ViewChannelComboBox.AddString(myString);
		}
		CString myString(pszStatChannelNames[m_pViewDataSettings->idstatchannelMappedToTriHeight]);
		m_ViewChannelComboBox.SelectString(0, myString);
	}
	m_TrianglesetLabelsButton.SetCheck(m_pViewDataSettings->bDrawTrianglesetLabels);
	m_TrianglesetCentersButton.SetCheck(m_pViewDataSettings->bDrawTriCenterLabels);

	//segmentset parameters
	if(pDoc!=NULL && pDoc->m_pPointset!=NULL
		&& m_pSettingsSheet->m_pOW2Doc->m_pSegmentset!=NULL)
	{
		m_SegmentsetButton.EnableWindow(TRUE);
		m_SegmentsetEdit.EnableWindow(TRUE);
		m_SegmentsetLabelsButton.EnableWindow(TRUE);
	}
	else 
	{
		m_SegmentsetButton.EnableWindow(FALSE);
		m_SegmentsetEdit.EnableWindow(FALSE);
		m_SegmentsetLabelsButton.EnableWindow(FALSE);
	}
	m_SegmentsetButton.SetCheck(m_pViewDataSettings->bDrawSegmentset);
	myText.Format(L"%.2f", m_pViewDataSettings->fSegmentset_zoffset);
	m_SegmentsetEdit.SetWindowText(myText);
	m_SegmentsetLabelsButton.SetCheck(m_pViewDataSettings->bDrawSegmentsetLabels);

	//classset parameters
	m_ClasssetLabelsButton.SetCheck(m_pViewDataSettings->bDrawClasssetLabels);


	return TRUE;
}

//copy view settings from sheet to view
BOOL CViewDataPage::UpdateSettingsFromSheetToDoc()
{
	WCHAR pszText[255];
	float fFloat, fValidatedFloat;

	//0) assumptions for now
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	ASSERT(pDoc!=NULL);
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));

	COWView* pView = m_pSettingsSheet->GetAttachedView();
	ASSERT(pView->IsKindOf(RUNTIME_CLASS(COWView)));

	//pixelset parameters
	m_pViewDataSettings->bDrawPixelset = m_PixelsetButton.GetCheck();
	m_PixelsetEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pViewDataSettings->fPixelset_zoffset = fValidatedFloat;

	//pointset parameters
	m_pViewDataSettings->bDrawPointset = m_PointsetButton.GetCheck();
	m_PointsetEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pViewDataSettings->fPointset_zoffset = fValidatedFloat;
	m_pViewDataSettings->bDrawPointsetLabels = m_PointsetLabelsButton.GetCheck();

	//voronoiset parameters
	m_pViewDataSettings->bDrawVoronoiset = m_VoronoisetButton.GetCheck();
	m_VoronoisetEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pViewDataSettings->fVoronoiset_zoffset = fValidatedFloat;

	//triangleset parameters
	m_pViewDataSettings->bDrawTriangleset = m_TrianglesetButton.GetCheck();
	m_TrianglesetEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pViewDataSettings->fTriangleset_zoffset = fValidatedFloat;
	m_ViewChannelEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pViewDataSettings->fTriangleset_zrange = fValidatedFloat;
	m_pViewDataSettings->idstatchannelMappedToTriHeight = m_ViewChannelComboBox.GetCurSel();
	ASSERT(m_pViewDataSettings->idstatchannelMappedToTriHeight !=CB_ERR);
	m_pViewDataSettings->bDrawTrianglesetLabels	= m_TrianglesetLabelsButton.GetCheck();
	m_pViewDataSettings->bDrawTriCenterLabels = m_TrianglesetCentersButton.GetCheck();

	//segmentset parameters
	m_pViewDataSettings->bDrawSegmentset = m_SegmentsetButton.GetCheck();
	m_SegmentsetEdit.GetWindowText(pszText, 50);
	fFloat = (float) _wtof(pszText);
	fValidatedFloat = fFloat; //no validation for now
	m_pViewDataSettings->fSegmentset_zoffset = fValidatedFloat;
	m_pViewDataSettings->bDrawSegmentsetLabels = m_SegmentsetLabelsButton.GetCheck();

	//classset parameters
	m_pViewDataSettings->bDrawClasssetLabels = m_ClasssetLabelsButton.GetCheck();

	//3) copy view parameters from sheet to attached view
	pView->m_pViewDataSettings->CopyAllValuesFrom(m_pViewDataSettings);
	//if(pView) pView->PostMessage(OWM_VIEW_UPDATEINFO, (WPARAM)m_pSettingsSheet);

	//if(pDoc) pDoc->UpdateAllViews(NULL);
	Invalidate(FALSE);
	return TRUE;
}

int CViewDataPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CWnd* myParent = GetParent();
	if(myParent!=NULL)
	{
		ASSERT(myParent->IsKindOf(RUNTIME_CLASS(CSettingsSheet)));
		m_pSettingsSheet = (CSettingsSheet*)myParent;
		m_pViewDataSettings = m_pSettingsSheet->m_pViewDataSettings;
	}
	return 0;
}

void CViewDataPage::OnTrianglesetcentersstatic() 
{
}

void CViewDataPage::OnClasssetlabels() 
{
	
}






///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// profilepage.cpp : implementation file
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//

/*
#include "stdafx.h"
#include "oifii_app.h"
#include "owpropag.h"
#include "profpage.h"

#include "setsheet.h"
#include "profsett.h"
#include "pointset.h"

#include "owdoc.h"
#include "ow2doc.h"

#include "owview.h"

#include "ow2mdi.h"
#include "owgraph.h"
#include "profilevw.h"
#include "profile.h"

#include "fusion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/


IMPLEMENT_DYNCREATE(CProfilePage, COWPropertyPage)

CProfilePage::CProfilePage() : COWPropertyPage(CProfilePage::IDD)
{
	m_pProfileGraph = NULL;
	m_idROI = -1;
	m_idVertexAtMouseLocation = -1;
	m_idSegmentAtMouseLocation = -1;
	bBusyUpdating = FALSE;

	//{{AFX_DATA_INIT(CProfilePage)
	//}}AFX_DATA_INIT
}

CProfilePage::~CProfilePage()
{
	if(m_pProfileGraph) delete(m_pProfileGraph);
}

void CProfilePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfilePage)
	DDX_Control(pDX, IDC_LOCALROICHECK, m_ROICheck);
	DDX_Control(pDX, IDC_LOCALPOINTCOLLECTIONCOMBO, m_PointCollectionCombo);
	DDX_Control(pDX, IDC_SCALE_AUTO, m_AutoScaleCheck);
	DDX_Control(pDX, IDC_SCALE_FIT, m_FitScaleCheck);
	DDX_Control(pDX, IDC_OKBUTTON, m_OKButton);
	DDX_Control(pDX, IDC_PROFILEFRAMESTATIC, m_ProfileFrameStatic);
	DDX_Control(pDX, IDC_NEIGHBORHOODEDIT, m_NeighborhoodEdit);
	DDX_Control(pDX, IDC_LOCALSTATCHANNELCOMBO, m_StatChannelComboBox);
	DDX_Control(pDX, IDC_LOCALNEIGHBORHOODCHECK, m_LocalNeighborhoodCheckBox);
	DDX_Control(pDX, IDC_IMAGEPIXELSCHECK, m_ImagePixelsCheckBox);
	DDX_Control(pDX, IDC_IMAGEFEATURESCHECK, m_ImageFeaturesCheckBox);
	DDX_Control(pDX, IDC_GLOBALCHECK, m_GlobalCheckBox);
	DDX_Control(pDX, IDC_GIVENSEGMENTEDIT, m_GivenSegmentEdit);
	DDX_Control(pDX, IDC_GIVENSEGMENTCHECK, m_GivenSegmentCheckBox);
	DDX_Control(pDX, IDC_COMBOSTATIC, m_ComboStatic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProfilePage, COWPropertyPage)
	//{{AFX_MSG_MAP(CProfilePage)
	ON_BN_CLICKED(IDC_CANCELBUTTON, OnCancelbutton)
	ON_BN_CLICKED(IDC_OKBUTTON, OnOkbutton)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_LOCALSTATCHANNELCOMBO, OnSelchangeProfilechannelcombo)
	ON_BN_CLICKED(IDC_GIVENSEGMENTCHECK, OnGivensegmentcheck)
	ON_BN_CLICKED(IDC_GLOBALCHECK, OnGlobalcheck)
	ON_BN_CLICKED(IDC_IMAGEFEATURESCHECK, OnImagefeaturescheck)
	ON_BN_CLICKED(IDC_IMAGEPIXELSCHECK, OnImagepixelscheck)
	ON_BN_CLICKED(IDC_LOCALNEIGHBORHOODCHECK, OnLocalneighborhoodcheck)
	ON_EN_SETFOCUS(IDC_NEIGHBORHOODEDIT, OnSetfocusNeighborhoodedit)
	ON_EN_CHANGE(IDC_NEIGHBORHOODEDIT, OnChangeNeighborhoodedit)
	ON_EN_CHANGE(IDC_GIVENSEGMENTEDIT, OnChangeGivensegmentedit)
	ON_EN_SETFOCUS(IDC_GIVENSEGMENTEDIT, OnSetfocusGivensegmentedit)
	ON_BN_CLICKED(IDC_SCALE_AUTO, OnScaleAuto)
	ON_BN_CLICKED(IDC_SCALE_FIT, OnScaleFit)
	ON_BN_CLICKED(IDC_LOCALROICHECK, OnLocalROIcheck)
	ON_CBN_SELCHANGE(IDC_LOCALPOINTCOLLECTIONCOMBO, OnSelchangeLocalpointcollectioncombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CProfilePage::FillComboBoxWithFusionInfoChannelNames()
{
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc==NULL) 
	{
		//ASSERT(FALSE);
		return FALSE;
	}

	//use combo to select fusion information (criteria, segment size, etc.)
	CString myText;
	m_ComboStatic.GetWindowText(myText);
	if(myText!="Fusion info channel")
	{
		m_ComboStatic.SetWindowText(L"Fusion info channel");
		m_StatChannelComboBox.ResetContent();
		for(int i=0; i<FUSION_NUMBEROFCHANNELNAMES; i++)
		{
			CString myString(pszFusionInfoChannelNames[i]);
			m_StatChannelComboBox.AddString(myString);
		}
		if(m_pProfileSettings->idchannelStat>=FUSION_NUMBEROFCHANNELNAMES)
		{
			m_pProfileSettings->idchannelStat = 0;
		}
		CString myString(pszFusionInfoChannelNames[m_pProfileSettings->idchannelStat]);
		m_StatChannelComboBox.SelectString(0, myString);
	}
	return TRUE;
}

BOOL CProfilePage::UpdateSettingsFromDocToSheet()
{
	CString myText;

	//0) assumptions for now
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc!=NULL) ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));

	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView!=NULL) ASSERT(pView->IsKindOf(RUNTIME_CLASS(COWView)));
	
	//1) copy view parameters from attached view
	if(pDoc)
	{
		m_pProfileSettings->CopyAllValuesFrom(pDoc->m_pProfileSettings);
	}
	else 
	{
		m_pProfileSettings->CopyAllValuesFrom(NULL); //Erase
	}

	//2) set CProfilePage controls ...
	BOOL bState = FALSE;
	if(pDoc==NULL || pView==NULL )
	{
		bState = FALSE;
	}
	else
	{
		bState = TRUE;
	}

	//enable/disable controls
	m_OKButton.EnableWindow(TRUE);

	BOOL bImagePixelsPresent = FALSE;
	BOOL bImageFeaturesPresent = FALSE;
	BOOL bSegmentPresent = FALSE;
	if(pDoc)
	{
		if(pDoc->m_pPixelset==NULL) bImagePixelsPresent = FALSE;
		  else bImagePixelsPresent = TRUE; //m_pProfileSettings->bImagePixels;
		if(pDoc->m_pPointset==NULL) bImageFeaturesPresent = FALSE;
		  else bImageFeaturesPresent = TRUE; //m_pProfileSettings->bImageFeaturesPresent;
		if(pDoc->m_pSegmentset==NULL) bSegmentPresent = FALSE;
		  else bSegmentPresent = TRUE;
	}

	m_FitScaleCheck.EnableWindow(bState);
	m_AutoScaleCheck.EnableWindow(bState);
	m_ImagePixelsCheckBox.EnableWindow(bState & bImagePixelsPresent);
	m_ImageFeaturesCheckBox.EnableWindow(bState & bImageFeaturesPresent);
	m_LocalNeighborhoodCheckBox.EnableWindow(bState & bImageFeaturesPresent);
	m_GlobalCheckBox.EnableWindow(bState);
	m_ROICheck.EnableWindow(bState);
	m_GivenSegmentCheckBox.EnableWindow(bState & bSegmentPresent);
	m_GivenSegmentEdit.EnableWindow(bState & bSegmentPresent);
	m_NeighborhoodEdit.EnableWindow(bState & bImageFeaturesPresent);
	m_StatChannelComboBox.EnableWindow(bState);
	m_PointCollectionCombo.EnableWindow(bState);

	//set controls values
	myText.Format(L"%d", m_pProfileSettings->iNeighborOrder);
	m_NeighborhoodEdit.SetWindowText(myText);
	myText.Format(L"%d", m_pProfileSettings->idSegment);
	m_GivenSegmentEdit.SetWindowText(myText);
	m_ImagePixelsCheckBox.SetCheck(m_pProfileSettings->bImagePixels);
	m_ImageFeaturesCheckBox.SetCheck(m_pProfileSettings->bImageFeatures);
	m_GlobalCheckBox.SetCheck(m_pProfileSettings->bGlobal);
	m_ROICheck.SetCheck(!m_pProfileSettings->bGlobal && !m_pProfileSettings->bLocal);
	m_LocalNeighborhoodCheckBox.SetCheck(m_pProfileSettings->bLocal);
	m_GivenSegmentCheckBox.SetCheck(m_pProfileSettings->bSegment);

	//fill point collection combo
	FillComboBoxWithPointCollectionNames(&m_PointCollectionCombo);			
	m_PointCollectionCombo.SetCurSel(0);

	//fill information combo
	if(m_pProfileSettings->bImagePixels)
	{
		//use combo to select image pixels statistics
		FillComboBoxWithImageChannelNames(&m_StatChannelComboBox);	
		m_StatChannelComboBox.SetCurSel(0);

	}
	else if(m_pProfileSettings->bImageFeatures) 
	{
		//use combo to select image features statistics
		FillComboBoxWithStatisticsChannelNames(&m_StatChannelComboBox);			
		m_StatChannelComboBox.SetCurSel(0); //SelectString(0, pszStatChannelNames[m_pProfileSettings->idchannelStat]);
	}
	else
	{
		ASSERT(FALSE);
	}

	m_AutoScaleCheck.SetCheck(m_pProfileSettings->bAutoScale);
	m_FitScaleCheck.SetCheck(!m_pProfileSettings->bAutoScale);
	return TRUE;
}

BOOL CProfilePage::UpdateSettingsFromSheetToDoc()
{
	int iInteger, iValidatedInteger;
	WCHAR pszText[255];

	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc!=NULL) 
	{
		if(!pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)))
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	m_NeighborhoodEdit.GetWindowText(pszText, 50);
	iInteger = _wtoi(pszText);
	iValidatedInteger = iInteger; //no validation for now
	m_pProfileSettings->iNeighborOrder = iValidatedInteger;

	m_GivenSegmentEdit.GetWindowText(pszText, 50);
	iInteger = _wtoi(pszText);
	iValidatedInteger = iInteger; //no validation for now
	m_pProfileSettings->idSegment	= iValidatedInteger;

	m_pProfileSettings->bImagePixels = m_ImagePixelsCheckBox.GetCheck();
	m_pProfileSettings->bImageFeatures = m_ImageFeaturesCheckBox.GetCheck();

	m_pProfileSettings->bGlobal = m_GlobalCheckBox.GetCheck();
	m_pProfileSettings->bLocal = m_LocalNeighborhoodCheckBox.GetCheck();
	m_pProfileSettings->bSegment = m_GivenSegmentCheckBox.GetCheck();

	m_pProfileSettings->idchannelStat = m_StatChannelComboBox.GetCurSel();

	//3) copy view parameters from sheet to attached view
	if(pDoc) pDoc->m_pProfileSettings->CopyAllValuesFrom(m_pProfileSettings);
	
	return TRUE;
}

//call to apply changes to document
void CProfilePage::Apply()
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
}

void CProfilePage::OnCancelbutton() 
{
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}

void CProfilePage::OnOkbutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}

BOOL CProfilePage::OnSetActive() 
{
	
	UpdateSettingsFromDocToSheet();	
	UpdateProfile();
	
	return COWPropertyPage::OnSetActive();
}

BOOL CProfilePage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}

BOOL CProfilePage::OnInitDialog() 
{
	COWPropertyPage::OnInitDialog();

	UpdateSettingsFromDocToSheet();	

	CRect myRect, myRefRect, myHistoRect;
	m_ProfileFrameStatic.GetWindowRect(&myRect);
	this->GetWindowRect(&myRefRect);
	myHistoRect.top = myRect.top - myRefRect.top;
	myHistoRect.left = myRect.left - myRefRect.left;
	myHistoRect.bottom = myHistoRect.top + myRect.Height();
	myHistoRect.right = myHistoRect.left + myRect.Width();
	
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	//if(pDoc==NULL || pDoc->m_pPointset==NULL) return TRUE;

	//initially, do not draw anything
	m_pProfileSettings->bImagePixels;
	int iPointType = PROFILE_IMAGEPIXELS;
	if(m_pProfileSettings->bImageFeatures)
	{
		iPointType = PROFILE_IMAGEFEATURES;
	}
	int iPointCollectionType = m_PointCollectionCombo.GetCurSel();
	int idROI = -1;
	int idVertex = -1;
	int iNumNeighbor = -1;
	int idSegment = -1;
	int iStatChannel = 0;
	m_pProfileGraph = new CProfileGraph(myHistoRect, 
										pDoc, 
										(CWnd*)this,
										iPointType,
										iPointCollectionType,
										idROI,
										idVertex,
										iNumNeighbor,
										idSegment, 
										iStatChannel); 
	ASSERT(m_pProfileGraph);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LONG CProfilePage::OnMouseMovedUpdateLocalGraph(struct tagCONTEXTINFO* pGlobalStructCONTEXTINFO)
{
	//poirier, sept 2001, begin
	if(pGlobalStructCONTEXTINFO==NULL)
	{
		m_idROI = 0;
		m_idVertexAtMouseLocation = 0;
		m_idSegmentAtMouseLocation = 0;
	}
	//poirier, sept 2001, end
	else
	{
		m_idROI = pGlobalStructCONTEXTINFO->idROI;
		m_idVertexAtMouseLocation = pGlobalStructCONTEXTINFO->idVertex;
		m_idSegmentAtMouseLocation = pGlobalStructCONTEXTINFO->idSegment;
	}
	if(!bBusyUpdating) UpdateProfile();
	return TRUE;
}

BOOL CProfilePage::UpdateProfile()
{
	ASSERT(m_pProfileGraph->IsKindOf(RUNTIME_CLASS(CProfileGraph)));

	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	//if(pDoc==NULL) return FALSE;

	int iPointType = 0;
	int iPointCollectionType = m_PointCollectionCombo.GetCurSel();
	int idROI = -1;
	int idVertex = -1;
	int iNumNeighbor = -1;
	int idSegment = -1;
	int iStatChannel = m_pProfileSettings->idchannelStat;

	if(pDoc!=NULL)
	{
		if(pDoc->m_pPixelset!=NULL && m_pProfileSettings->bImagePixels)
		{
			iPointType = PROFILE_IMAGEPIXELS;
			//iPointCollectionType = m_PointCollectionCombo.GetCurSel();

			if(m_pProfileSettings->bLocal && pDoc->m_pPointset!=NULL)
			{
				idROI = -1;
				idVertex = m_idVertexAtMouseLocation;
				idSegment = -1;
			}
			else if(m_pProfileSettings->bSegment && pDoc->m_pSegmentset!=NULL)
			{
				idROI = -1;
				idVertex = -1;
				idSegment = m_idSegmentAtMouseLocation;
			}
			else if(m_pProfileSettings->bGlobal)
			{
				idROI = -1;
				idVertex = -1;
				idSegment = -1;
			}
			else if(pDoc->m_pROIset)
			{
				//get selected roi
				idROI = pDoc->m_pROIset->GetSelectedROI();
				idVertex = -1;
				idSegment = -1;
			}
		}
		else if(pDoc->m_pPointset!=NULL && m_pProfileSettings->bImageFeatures)
		{
			iPointType = PROFILE_IMAGEFEATURES;
			//iPointCollectionType = PROFILE_BR_XCENTER;

			if(m_pProfileSettings->bLocal)
			{
				//will display circular neighborhood profile
				idROI = -1;
				idVertex = m_idVertexAtMouseLocation;
				iNumNeighbor = m_pProfileSettings->iNeighborOrder;
				idSegment = -1;
			}
			else if(m_pProfileSettings->bSegment && pDoc->m_pSegmentset!=NULL)
			{
				//will display segment edge strength profile
				idROI = -1;
				idVertex = -1;
				idSegment = m_idSegmentAtMouseLocation;
			}
			else if(m_pProfileSettings->bGlobal)  //m_pProfileSettings->bGlobal = TRUE for fusion info
			{
				idROI = -1;
				idVertex = -1;
				idSegment = -1;
			}
			else
			{
				//get selected roi
				idROI = pDoc->m_pROIset->GetSelectedROI();
				idVertex = -1;
				idSegment = -1;
			}
		}
	}
	
	bBusyUpdating = TRUE;
	m_pProfileGraph->Update(pDoc, 
							iPointType,
							iPointCollectionType,
							idROI,
							idVertex, 
							iNumNeighbor, 
							idSegment, 
							iStatChannel); 
	bBusyUpdating = FALSE;
	return TRUE;
}


void CProfilePage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CPropertyPage::OnPaint() for painting messages
}

int CProfilePage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COWPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;	
	CWnd* myParent = GetParent();
	if(myParent!=NULL)
	{
		ASSERT(myParent->IsKindOf(RUNTIME_CLASS(CSettingsSheet)));
		m_pSettingsSheet = (CSettingsSheet*)myParent;
		m_pProfileSettings = m_pSettingsSheet->m_pProfileSettings;
	}
	return 0;
}

void CProfilePage::OnSelchangeProfilechannelcombo() 
{
	m_pProfileSettings->idchannelStat = m_StatChannelComboBox.GetCurSel();
	Apply();
	UpdateProfile();
}

void CProfilePage::OnGivensegmentcheck() 
{
	m_pProfileSettings->bSegment = TRUE;
	m_pProfileSettings->bGlobal = FALSE;
	m_pProfileSettings->bLocal = FALSE;
	m_GlobalCheckBox.SetCheck(m_pProfileSettings->bGlobal);
	m_LocalNeighborhoodCheckBox.SetCheck(m_pProfileSettings->bLocal);
	//m_NeighborhoodEdit.SetWindowText("");
	m_GivenSegmentCheckBox.SetCheck(m_pProfileSettings->bSegment);
	CString myText;
	myText.Format(L"%d", m_pProfileSettings->idSegment);
	m_GivenSegmentEdit.SetWindowText(myText);

	FillComboBoxWithStatisticsChannelNames(&m_StatChannelComboBox);
	m_StatChannelComboBox.SetCurSel(0);
	Apply();
	UpdateProfile();
}

void CProfilePage::OnGlobalcheck() 
{
	//disable selection rect roi floating mode and mouse nearest neighbor mode
	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView->IsKindOf(RUNTIME_CLASS(COW2View))) 
	{
		if(((COW2View*)pView)->m_bMode_ROIFloating) ((COW2View*)pView)->DisableROIFloatingMode();
		((COW2View*)pView)->DisableMOUSENearestNeighborsMode();
	}
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc && pDoc->m_pPixelset) m_ImagePixelsCheckBox.EnableWindow(TRUE);
	m_PointCollectionCombo.EnableWindow(TRUE);
	m_PointCollectionCombo.SetCurSel(0);
	/*
	CString myString;
	m_PointCollectionCombo.GetLBText(0, myString);
	m_PointCollectionCombo.SetWindowText(myString);
	*/

	if(m_pProfileSettings->bSegment==FALSE)
	{
		m_pProfileSettings->bGlobal = TRUE;
		m_pProfileSettings->bSegment = FALSE;
		m_pProfileSettings->bLocal = FALSE;
	}
	else
	{
		m_pProfileSettings->bGlobal = FALSE;
		m_pProfileSettings->bSegment = FALSE;
		m_pProfileSettings->bLocal = FALSE;
	}
	//FillComboBoxWithFusionInfoChannelNames();
	m_GlobalCheckBox.SetCheck(m_pProfileSettings->bGlobal);
	m_ROICheck.SetCheck(!m_pProfileSettings->bGlobal && !m_pProfileSettings->bLocal);
	m_LocalNeighborhoodCheckBox.SetCheck(m_pProfileSettings->bLocal);
	//m_NeighborhoodEdit.SetWindowText("");
	m_GivenSegmentCheckBox.SetCheck(m_pProfileSettings->bSegment);
	//m_GivenSegmentEdit.SetWindowText("");
	Apply();
	UpdateProfile();
}

void CProfilePage::OnImagepixelscheck() 
{
	//allowed to change?
	BOOL bImagePixelsPresent = FALSE;
	BOOL bImageFeaturesPresent = FALSE;
	BOOL bSegmentPresent = FALSE;
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc)
	{
		if(pDoc->m_pPixelset==NULL) bImagePixelsPresent = FALSE;
		  else bImagePixelsPresent = TRUE; 
		if(pDoc->m_pPointset==NULL) bImageFeaturesPresent = FALSE;
		  else bImageFeaturesPresent = TRUE; 
		if(pDoc->m_pSegmentset==NULL) bSegmentPresent = FALSE;
		  else bSegmentPresent = TRUE;
	}

	if(m_pProfileSettings->bImagePixels==FALSE && bImagePixelsPresent)
	{
		//switch to image pixels
		m_pProfileSettings->bImagePixels = TRUE;
		m_pProfileSettings->bImageFeatures = FALSE;

		m_ImagePixelsCheckBox.SetCheck(m_pProfileSettings->bImagePixels);
		m_ImageFeaturesCheckBox.SetCheck(m_pProfileSettings->bImageFeatures);
		//use combo to select image pixel statistics
		FillComboBoxWithImageChannelNames(&m_StatChannelComboBox);			
		m_StatChannelComboBox.SetCurSel(0);
		Apply();
		UpdateProfile();

		/*
		BOOL bState = !(m_ImagePixelsCheckBox.GetCheck());
		m_GlobalCheckBox.EnableWindow(!bState);
		m_LocalNeighborhoodCheckBox.EnableWindow(bState);
		m_GivenSegmentCheckBox.EnableWindow(bState);
		m_GivenSegmentEdit.EnableWindow(bState);
		m_NeighborhoodEdit.EnableWindow(bState);
		m_StatChannelComboBox.EnableWindow(bState);
		*/
	
	}
	else if(m_pProfileSettings->bImagePixels==TRUE && bImageFeaturesPresent)
	{
		m_pProfileSettings->bImagePixels = FALSE;
		m_pProfileSettings->bImageFeatures = TRUE;

		m_ImagePixelsCheckBox.SetCheck(m_pProfileSettings->bImagePixels);
		m_ImageFeaturesCheckBox.SetCheck(m_pProfileSettings->bImageFeatures);
		//use combo to select image pixel statistics
		FillComboBoxWithImageChannelNames(&m_StatChannelComboBox);	
		m_StatChannelComboBox.SetCurSel(0);
		Apply();
		UpdateProfile();
	}
	else
	{
		//don't change
		m_ImagePixelsCheckBox.SetCheck(m_pProfileSettings->bImagePixels);
		m_ImageFeaturesCheckBox.SetCheck(m_pProfileSettings->bImageFeatures);
	}
}

void CProfilePage::OnImagefeaturescheck() 
{
	//allowed to change?
	BOOL bImagePixelsPresent = FALSE;
	BOOL bImageFeaturesPresent = FALSE;
	BOOL bSegmentPresent = FALSE;
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc)
	{
		if(pDoc->m_pPixelset==NULL) bImagePixelsPresent = FALSE;
		  else bImagePixelsPresent = TRUE; 
		if(pDoc->m_pPointset==NULL) bImageFeaturesPresent = FALSE;
		  else bImageFeaturesPresent = TRUE; 
		if(pDoc->m_pSegmentset==NULL) bSegmentPresent = FALSE;
		  else bSegmentPresent = TRUE;
	}

	if(m_pProfileSettings->bImageFeatures==FALSE && bImageFeaturesPresent)
	{
		//switch to image pixels
		m_pProfileSettings->bImagePixels = FALSE;
		m_pProfileSettings->bImageFeatures = TRUE;

		m_ImagePixelsCheckBox.SetCheck(m_pProfileSettings->bImagePixels);
		m_ImageFeaturesCheckBox.SetCheck(m_pProfileSettings->bImageFeatures);
		//use combo to select image features statistics
		FillComboBoxWithStatisticsChannelNames(&m_StatChannelComboBox);			
		m_StatChannelComboBox.SetCurSel(0);//SelectString(0, pszStatChannelNames[m_pProfileSettings->idchannelStat]);
		Apply();
		UpdateProfile();

		/*
		BOOL bState = !(m_ImagePixelsCheckBox.GetCheck());
		m_GlobalCheckBox.EnableWindow(!bState);
		m_LocalNeighborhoodCheckBox.EnableWindow(bState);
		m_GivenSegmentCheckBox.EnableWindow(bState);
		m_GivenSegmentEdit.EnableWindow(bState);
		m_NeighborhoodEdit.EnableWindow(bState);
		m_StatChannelComboBox.EnableWindow(bState);
		*/
	
	}	
	else if(m_pProfileSettings->bImageFeatures==TRUE && bImagePixelsPresent)
	{
		m_pProfileSettings->bImagePixels = TRUE;
		m_pProfileSettings->bImageFeatures = FALSE;

		m_ImagePixelsCheckBox.SetCheck(m_pProfileSettings->bImagePixels);
		m_ImageFeaturesCheckBox.SetCheck(m_pProfileSettings->bImageFeatures);
		//use combo to select image pixel statistics
		FillComboBoxWithImageChannelNames(&m_StatChannelComboBox);			
		m_StatChannelComboBox.SetCurSel(0);
		Apply();
		UpdateProfile();
	}
	else
	{
		//don't change
		m_ImagePixelsCheckBox.SetCheck(m_pProfileSettings->bImagePixels);
		m_ImageFeaturesCheckBox.SetCheck(m_pProfileSettings->bImageFeatures);
	}
}

void CProfilePage::OnLocalROIcheck() 
{
	//disable selection rect roi floating mode and mouse nearest neighbor mode
	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView && pView->IsKindOf(RUNTIME_CLASS(COW2View))) 
	{
		((COW2View*)pView)->DisableMOUSENearestNeighborsMode();
	}
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc && pDoc->m_pPixelset) m_ImagePixelsCheckBox.EnableWindow(TRUE);
	m_PointCollectionCombo.EnableWindow(TRUE);
	m_PointCollectionCombo.SetCurSel(0);

	if(m_pProfileSettings->bGlobal==FALSE && m_pProfileSettings->bLocal==FALSE)
	{
		m_pProfileSettings->bGlobal = TRUE;
		m_pProfileSettings->bLocal = FALSE;
		m_pProfileSettings->bSegment = FALSE;
	}
	else
	{
		m_pProfileSettings->bGlobal = FALSE;
		m_pProfileSettings->bLocal = FALSE;
		m_pProfileSettings->bSegment = FALSE;
	}
	m_GlobalCheckBox.SetCheck(m_pProfileSettings->bGlobal);
	m_ROICheck.SetCheck(!m_pProfileSettings->bGlobal && !m_pProfileSettings->bLocal);
	m_LocalNeighborhoodCheckBox.SetCheck(m_pProfileSettings->bLocal);
	//m_NeighborhoodEdit.SetWindowText("");
	m_GivenSegmentCheckBox.SetCheck(m_pProfileSettings->bSegment);
	//m_GivenSegmentEdit.SetWindowText("");

	Apply();
	UpdateProfile();
}

void CProfilePage::OnSelchangeLocalpointcollectioncombo() 
{
	Apply();
	UpdateProfile();
}

void CProfilePage::OnLocalneighborhoodcheck() 
{
	//enable mouse nearest neighbor mode
	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView->IsKindOf(RUNTIME_CLASS(COW2View))) 
	{
		if(((COW2View*)pView)->m_bMode_ROIFloating) ((COW2View*)pView)->DisableROIFloatingMode();
		((COW2View*)pView)->EnableMOUSENearestNeighborsMode();
	}
	m_ImagePixelsCheckBox.EnableWindow(FALSE);
	m_PointCollectionCombo.EnableWindow(FALSE);
	m_PointCollectionCombo.SetWindowText(L"Mouse Location");

	//switch to image features
	m_pProfileSettings->bImagePixels = FALSE;
	m_pProfileSettings->bImageFeatures = TRUE;
	m_ImagePixelsCheckBox.SetCheck(m_pProfileSettings->bImagePixels);
	m_ImageFeaturesCheckBox.SetCheck(m_pProfileSettings->bImageFeatures);
	//check local, uncheck others
	m_pProfileSettings->bGlobal = FALSE;
	m_pProfileSettings->bLocal = TRUE;
	m_pProfileSettings->bSegment = FALSE;
	m_GlobalCheckBox.SetCheck(m_pProfileSettings->bGlobal);
	m_LocalNeighborhoodCheckBox.SetCheck(m_pProfileSettings->bLocal);
	m_ROICheck.SetCheck(FALSE);
	m_GivenSegmentCheckBox.SetCheck(m_pProfileSettings->bSegment);
	//m_GivenSegmentEdit.SetWindowText("");

	CString myText;
	myText.Format(L"%d", m_pProfileSettings->iNeighborOrder);
	m_NeighborhoodEdit.SetWindowText(myText);

	FillComboBoxWithStatisticsChannelNames(&m_StatChannelComboBox);
	m_StatChannelComboBox.SetCurSel(0);
	Apply();
	UpdateProfile();
}

void CProfilePage::OnSetfocusNeighborhoodedit() 
{
	OnLocalneighborhoodcheck();	
}

void CProfilePage::OnChangeNeighborhoodedit() 
{
	/*
	//validate
	CString myText;
	m_NeighborhoodEdit.GetWindowText(myText);
	int ivalue = atoi(myText);
	if(ivalue<0) 
	{
		ivalue=0;
		myText.Format("%d", ivalue);
		m_NeighborhoodEdit.SetWindowText(myText);
	}
	else if(ivalue>5) 
	{
		ivalue=5;
		myText.Format("%d", ivalue);
		m_NeighborhoodEdit.SetWindowText(myText);
	}
	Apply();
	*/
}

void CProfilePage::OnChangeGivensegmentedit() 
{
	/*
	//validate
	CString myText;
	m_GivenSegmentEdit.GetWindowText(myText);
	int ivalue = atoi(myText);
	if(ivalue<0) 
	{
		ivalue=0;
		myText.Format("%d", ivalue);
		m_GivenSegmentEdit.SetWindowText(myText);
	}
	Apply();
	*/
}

void CProfilePage::OnSetfocusGivensegmentedit() 
{
	OnGivensegmentcheck();
}

void CProfilePage::OnScaleAuto() 
{
	if(m_pProfileSettings->bAutoScale==FALSE)
	{
		m_pProfileSettings->bAutoScale = TRUE;
	}
	else
	{
		m_pProfileSettings->bAutoScale = FALSE;
		ScaleFix();
	}

	m_AutoScaleCheck.SetCheck(m_pProfileSettings->bAutoScale);
	m_FitScaleCheck.SetCheck(!m_pProfileSettings->bAutoScale);
	Apply();
	UpdateProfile();
}

void CProfilePage::OnScaleFit() 
{
	if(m_pProfileSettings->bAutoScale==TRUE)
	{
		m_pProfileSettings->bAutoScale = FALSE;
		ScaleFix();
	}
	else
	{
		m_pProfileSettings->bAutoScale = TRUE;
	}

	m_AutoScaleCheck.SetCheck(m_pProfileSettings->bAutoScale);
	m_FitScaleCheck.SetCheck(!m_pProfileSettings->bAutoScale);
	Apply();
	UpdateProfile();
}	


void CProfilePage::ScaleFix()
{
	//fix profile graph min max y axis value with current ones
	POSITION pos = m_pProfileGraph->m_pProfileGraphDoc->GetFirstViewPosition();
	CView* pFirstView = m_pProfileGraph->m_pProfileGraphDoc->GetNextView(pos);
	if(pFirstView->IsKindOf(RUNTIME_CLASS(COWGraphView)))
	{
		double fMinY = ((COWGraphView*)pFirstView)->m_PROFILEdfMinValueY; 
		double fMaxY = ((COWGraphView*)pFirstView)->m_PROFILEdfMaxValueY;
		m_pProfileGraph->m_pProfileGraphDoc->FixPROFILEMinMaxXYValue(fMinY, fMaxY);
	}
	else
	{
		ASSERT(FALSE);
		m_pProfileSettings->bAutoScale = TRUE;
	}

}

BOOL CProfilePage::FillComboBoxWithPointCollectionNames(CComboBox* pComboBox)
{
	ASSERT(m_pSettingsSheet!=NULL);
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc==NULL) 
	{
		pComboBox->ResetContent();
		return FALSE;
	}

	pComboBox->ResetContent();
	for(int i=0; i<PROFILE_NUMBEROF_POINTCOLLECTIONNAME; i++) 
	{
		CString myString(pszProfilePointCollectionNames[i]);
		pComboBox->AddString(myString);
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// histopage.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/*
#include "stdafx.h"
#include "oifii_app.h"

#include "owpropag.h"
#include "histopage.h"

#include "setsheet.h"
#include "histosett.h"
#include "pointset.h"

#include "owdoc.h"
#include "ow2doc.h"

#include "owview.h"

#include "ow2mdi.h"
#include "owgraph.h"
#include "histovw.h"
#include "histo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/


IMPLEMENT_DYNCREATE(CHistoPage, COWPropertyPage)

CHistoPage::CHistoPage() : COWPropertyPage(CHistoPage::IDD)
{
	m_pHistogram = NULL;
	m_idROI = -1;
	m_idVertexAtMouseLocation = -1;
	m_idSegmentAtMouseLocation = -1;

	m_bBusyUpdating = FALSE;
	//{{AFX_DATA_INIT(CHistoPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CHistoPage::~CHistoPage()
{
	if(m_pHistogram) delete(m_pHistogram);
}

void CHistoPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistoPage)
	DDX_Control(pDX, IDC_LOCALROICHECK2, m_ROICheck);
	DDX_Control(pDX, IDC_LOCALPOINTCOLLECTIONCOMBO, m_PointCollectionCombo);
	DDX_Control(pDX, IDC_SCALE_AUTO, m_AutoScaleCheck);
	DDX_Control(pDX, IDC_SCALE_FIT, m_FitScaleCheck);
	DDX_Control(pDX, IDC_OKBUTTON, m_OKButton);
	DDX_Control(pDX, IDC_GIVENSEGMENTEDIT, m_GivenSegmentEdit);
	DDX_Control(pDX, IDC_STATHISTOCHECK, m_ImageFeaturesCheckBox);
	DDX_Control(pDX, IDC_IMAGEHISTOCHECK, m_ImagePixelsCheckBox);
	DDX_Control(pDX, IDC_LOCALNEIGHBORHOODCHECK, m_LocalNeighborhoodCheckBox);
	DDX_Control(pDX, IDC_GLOBALCHECK, m_GlobalCheckBox);
	DDX_Control(pDX, IDC_GIVENSEGMENTCHECK, m_GivenSegmentCheckBox);
	DDX_Control(pDX, IDC_HISTOFRAMESTATIC, m_HistoFrameStaticButton);
	DDX_Control(pDX, IDC_NEIGHBORHOODEDIT, m_NeighborhoodEdit);
	DDX_Control(pDX, IDC_LOCALSTATCHANNELCOMBO, m_StatChannelComboBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistoPage, COWPropertyPage)
	//{{AFX_MSG_MAP(CHistoPage)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_CANCELBUTTON, OnCancelbutton)
	ON_BN_CLICKED(IDC_OKBUTTON, OnOkbutton)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_LOCALSTATCHANNELCOMBO, OnSelchangeLocalstatchannelcombo)
	ON_BN_CLICKED(IDC_GIVENSEGMENTCHECK, OnGivensegmentcheck)
	ON_EN_SETFOCUS(IDC_GIVENSEGMENTEDIT, OnSetfocusGivensegmentedit)
	ON_EN_CHANGE(IDC_GIVENSEGMENTEDIT, OnChangeGivensegmentedit)
	ON_BN_CLICKED(IDC_GLOBALCHECK, OnGlobalcheck)
	ON_BN_CLICKED(IDC_LOCALNEIGHBORHOODCHECK, OnLocalneighborhoodcheck)
	ON_EN_CHANGE(IDC_NEIGHBORHOODEDIT, OnChangeNeighborhoodedit)
	ON_EN_SETFOCUS(IDC_NEIGHBORHOODEDIT, OnSetfocusNeighborhoodedit)
	ON_BN_CLICKED(IDC_IMAGEHISTOCHECK, OnImagepixelscheck)
	ON_BN_CLICKED(IDC_STATHISTOCHECK, OnImageFeaturescheck)
	ON_BN_CLICKED(IDC_SCALE_AUTO, OnScaleAuto)
	ON_BN_CLICKED(IDC_SCALE_FIT, OnScaleFit)
	ON_BN_CLICKED(IDC_LOCALROICHECK2, OnLocalROIcheck)
	ON_CBN_SELCHANGE(IDC_LOCALPOINTCOLLECTIONCOMBO, OnSelchangeLocalpointcollectioncombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CHistoPage::UpdateSettingsFromDocToSheet()
{
	CString myText;

	//0) assumptions for now
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc!=NULL) ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));
	//poirier august 1999
	//if(pDoc==NULL) return FALSE;

	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView!=NULL) ASSERT(pView->IsKindOf(RUNTIME_CLASS(COWView)));
	
	//1) copy view parameters from attached view
	if(pDoc)
	{
		m_pHistoSettings->CopyAllValuesFrom(pDoc->m_pHistoSettings);
	}
	else 
	{
		m_pHistoSettings->CopyAllValuesFrom(NULL); //Erase
	}

	//2) set CHistoPage controls ...
	BOOL bState = FALSE;
	if(pDoc==NULL || pView==NULL )
	{
		bState = FALSE;
	}
	else
	{
		bState = TRUE;
	}

	//enable/disable controls
	m_OKButton.EnableWindow(TRUE);


	BOOL bImagePixelsPresent = FALSE;
	BOOL bImageFeaturesPresent = FALSE;
	BOOL bSegmentPresent = FALSE;
	if(pDoc)
	{
		if(pDoc->m_pPixelset==NULL) bImagePixelsPresent = FALSE;
		  else bImagePixelsPresent = TRUE; //m_pProfileSettings->bImagePixels;
		if(pDoc->m_pPointset==NULL) bImageFeaturesPresent = FALSE;
		  else bImageFeaturesPresent = TRUE; //m_pProfileSettings->bImageFeaturesPresent;
		if(pDoc->m_pSegmentset==NULL) bSegmentPresent = FALSE;
		  else bSegmentPresent = TRUE;
	}
	m_FitScaleCheck.EnableWindow(bState);
	m_AutoScaleCheck.EnableWindow(bState);
	m_ImagePixelsCheckBox.EnableWindow(bState & bImagePixelsPresent);
	m_ImageFeaturesCheckBox.EnableWindow(bState & bImageFeaturesPresent);
	m_LocalNeighborhoodCheckBox.EnableWindow(bState & bImageFeaturesPresent);
	m_GlobalCheckBox.EnableWindow(bState);
	m_ROICheck.EnableWindow(bState);
	m_GivenSegmentCheckBox.EnableWindow(bState & bSegmentPresent);
	m_GivenSegmentEdit.EnableWindow(bState & bSegmentPresent);
	m_NeighborhoodEdit.EnableWindow(bState & bImageFeaturesPresent);
	m_StatChannelComboBox.EnableWindow(bState);
	m_PointCollectionCombo.EnableWindow(bState);

	//set controls values
	myText.Format(L"%d", m_pHistoSettings->iNeighborOrder);
	m_NeighborhoodEdit.SetWindowText(myText);
	myText.Format(L"%d", m_pHistoSettings->idSegment);
	m_GivenSegmentEdit.SetWindowText(myText);

	m_ImagePixelsCheckBox.SetCheck(m_pHistoSettings->bImagePixels);
	m_ImageFeaturesCheckBox.SetCheck(m_pHistoSettings->bImageFeatures);

	m_GlobalCheckBox.SetCheck(m_pHistoSettings->bGlobal);
	m_ROICheck.SetCheck(!m_pHistoSettings->bGlobal && !m_pHistoSettings->bLocal);
	m_LocalNeighborhoodCheckBox.SetCheck(m_pHistoSettings->bLocal);
	m_GivenSegmentCheckBox.SetCheck(m_pHistoSettings->bSegment);

	//fill point collection combo
	FillComboBoxWithPointCollectionNames(&m_PointCollectionCombo);			
	m_PointCollectionCombo.SetCurSel(0);

	//fill information combo
	if(m_pHistoSettings->bImagePixels)
	{
		//use combo to select image pixels statistics
		FillComboBoxWithImageChannelNames(&m_StatChannelComboBox);			
		m_StatChannelComboBox.SetCurSel(0);
	}
	else if(m_pHistoSettings->bImageFeatures) 
	{
		//use combo to select image features statistics
		FillComboBoxWithStatisticsChannelNames(&m_StatChannelComboBox);			
		m_StatChannelComboBox.SetCurSel(0); //SelectString(0, pszProfilePointCollectionNames[m_pHistoSettings->idchannelStat]);
	}
	else
	{
		ASSERT(FALSE);
	}

	m_AutoScaleCheck.SetCheck(m_pHistoSettings->bAutoScale);
	m_FitScaleCheck.SetCheck(!m_pHistoSettings->bAutoScale);
	return TRUE;
}


BOOL CHistoPage::UpdateSettingsFromSheetToDoc()
{
	int iInteger, iValidatedInteger;
	WCHAR pszText[255];

	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc!=NULL) 
	{
		if(!pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)))
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	m_NeighborhoodEdit.GetWindowText(pszText, 50);
	iInteger = _wtoi(pszText);
	iValidatedInteger = iInteger; //no validation for now
	m_pHistoSettings->iNeighborOrder = iValidatedInteger;

	m_GivenSegmentEdit.GetWindowText(pszText, 50);
	iInteger = _wtoi(pszText);
	iValidatedInteger = iInteger; //no validation for now
	m_pHistoSettings->idSegment	= iValidatedInteger;

	m_pHistoSettings->bImagePixels = m_ImagePixelsCheckBox.GetCheck();
	m_pHistoSettings->bImageFeatures = m_ImageFeaturesCheckBox.GetCheck();

	m_pHistoSettings->bGlobal = m_GlobalCheckBox.GetCheck();
	m_pHistoSettings->bLocal = m_LocalNeighborhoodCheckBox.GetCheck();
	m_pHistoSettings->bSegment = m_GivenSegmentCheckBox.GetCheck();

	m_pHistoSettings->idchannelStat = m_StatChannelComboBox.GetCurSel();

	//3) copy view parameters from sheet to attached view
	if(pDoc) pDoc->m_pHistoSettings->CopyAllValuesFrom(m_pHistoSettings);

	return TRUE;
}

int CHistoPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COWPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;	
	CWnd* myParent = GetParent();
	if(myParent!=NULL)
	{
		ASSERT(myParent->IsKindOf(RUNTIME_CLASS(CSettingsSheet)));
		m_pSettingsSheet = (CSettingsSheet*)myParent;
		m_pHistoSettings = m_pSettingsSheet->m_pHistoSettings;
	}
	return 0;
}

BOOL CHistoPage::OnSetActive() 
{
	
	UpdateSettingsFromDocToSheet();	
	UpdateHisto();
	
	return COWPropertyPage::OnSetActive();
}

BOOL CHistoPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COWPropertyPage::OnKillActive();
}

void CHistoPage::OnCancelbutton() 
{
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}

void CHistoPage::Apply()
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
}

void CHistoPage::OnOkbutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}

void CHistoPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// Do not call COWPropertyPage::OnPaint() for painting messages

	/*
	CRect myRect, myRefRect, myHistoRect;
	m_HistoFrameStaticButton.GetWindowRect(&myRect);
	this->GetWindowRect(&myRefRect);
	myHistoRect.top = myRect.top - myRefRect.top;
	myHistoRect.left = myRect.left - myRefRect.left;
	myHistoRect.bottom = myHistoRect.top + myRect.Height();
	myHistoRect.right = myHistoRect.left + myRect.Width();
	

	CBrush myBrush(RGB(255,0,0));
	CBrush* pOldBrush = dc.SelectObject(&myBrush);
	CPen myPen(PS_SOLID, 1, RGB(255,0,0));
	CPen* pOldPen = dc.SelectObject(&myPen);
	dc.Rectangle(&myHistoRect);
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
	*/
}

BOOL CHistoPage::OnInitDialog() 
{
	COWPropertyPage::OnInitDialog();

	UpdateSettingsFromDocToSheet();	

	CRect myRect, myRefRect, myHistoRect;
	m_HistoFrameStaticButton.GetWindowRect(&myRect);
	this->GetWindowRect(&myRefRect);
	myHistoRect.top = myRect.top - myRefRect.top;
	myHistoRect.left = myRect.left - myRefRect.left;
	myHistoRect.bottom = myHistoRect.top + myRect.Height();
	myHistoRect.right = myHistoRect.left + myRect.Width();
	
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	//if(pDoc==NULL || pDoc->m_pPointset==NULL) return TRUE;

	int iPointType = 0;
	int iPointCollectionType = 0;

	int idROI = -1;
	int idVertex = -1;
	int iNumNeighbor = -1;
	int idSegment = -1;

	//determine number of level
	int iNumberOfLevel;
	if(m_pHistoSettings->bGlobal) 
	{
		iNumberOfLevel = 256;
	}
	else if(m_pHistoSettings->bLocal) //local mouse location
	{
		iNumberOfLevel = 10;
	}
	else if(m_pHistoSettings->bSegment) //local segment
	{
		iNumberOfLevel = 10;
	}
	else
	{
		//local roi
		if(pDoc!=NULL && pDoc->m_pPixelset!=NULL)
		{
			if(pDoc->m_pROIset)
			{
				idROI = pDoc->m_pROIset->GetSelectedROI();
			}
		}
		iNumberOfLevel = 256;
	}

	m_pHistogram = new CHistogram(	myHistoRect, 
									pDoc, 
									(CWnd*)this,
									iPointType,
									iPointCollectionType,
									idROI,
									idVertex,
									iNumNeighbor,
									idSegment,
									m_pHistoSettings->idchannelStat,
									iNumberOfLevel); 
	ASSERT(m_pHistogram);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LONG CHistoPage::OnMouseMovedUpdateLocalGraph(struct tagCONTEXTINFO* pGlobalStructCONTEXTINFO)
{
	//poirier, sept 2001, begin
	if(pGlobalStructCONTEXTINFO==NULL)
	{
		m_idROI = 0;
		m_idVertexAtMouseLocation = 0;
		m_idSegmentAtMouseLocation = 0;
	}
	//poirier, sept 2001, end
	else
	{
		m_idROI = pGlobalStructCONTEXTINFO->idROI;
		m_idVertexAtMouseLocation = pGlobalStructCONTEXTINFO->idVertex;
		m_idSegmentAtMouseLocation = pGlobalStructCONTEXTINFO->idSegment;
	}
	if(!m_bBusyUpdating) UpdateHisto();
	return TRUE;
}


BOOL CHistoPage::UpdateHisto()
{
	ASSERT(m_pHistogram->IsKindOf(RUNTIME_CLASS(CHistogram)));

 	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	/*
	if(pDoc==NULL) 
	{
		#ifdef _DEBUG
		((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText("pDoc is NULL (in UpdateHisto())\r\n");
		#endif //_DEBUG
		return FALSE;
	}
	*/

	int iPointType = 0;
	int iPointCollectionType = iPointCollectionType = m_PointCollectionCombo.GetCurSel();
	int idROI = -1;
	int idVertex = -1;
	int iNumNeighbor = -1;
	int idSegment = -1;
	int idchannelStat = m_pHistoSettings->idchannelStat;
	int iNumberOfLevel = 256;

	if(pDoc!=NULL)
	{
		//determine number of level
		if(m_pHistoSettings->bGlobal) 
		{
			iNumberOfLevel = 256;
		}
		else if(m_pHistoSettings->bLocal) //local mouse location
		{
			iNumberOfLevel = 10;
		}
		else if(m_pHistoSettings->bSegment) //local segment
		{
			iNumberOfLevel = 10;
		}
		else
		{
			//local roi
			if(pDoc->m_pPixelset!=NULL)
			{
				if(pDoc->m_pROIset)
				{
					idROI = pDoc->m_pROIset->GetSelectedROI();
				}
			}
			iNumberOfLevel = 256;
		}

		if(pDoc->m_pPixelset!=NULL && m_pHistoSettings->bImagePixels)
		{
			iPointType = HISTO_IMAGEPIXELS;
			//iPointCollectionType = m_PointCollectionCombo.GetCurSel();

			if(m_pHistoSettings->bLocal && pDoc->m_pPointset!=NULL)
			{
				idROI = -1;
				idVertex = m_idVertexAtMouseLocation;
				idSegment = -1;
			}
			else if(m_pHistoSettings->bSegment && pDoc->m_pSegmentset!=NULL)
			{
				idROI = -1;
				idVertex = -1;
				idSegment = m_idSegmentAtMouseLocation;
			}
			else if(m_pHistoSettings->bGlobal)
			{
				idROI = -1;
				idVertex = -1;
				idSegment = -1;
			}
			else if(pDoc->m_pROIset)
			{
				//get selected roi
				idROI = pDoc->m_pROIset->GetSelectedROI();
				idVertex = -1;
				idSegment = -1;
			}
		}
		else if(pDoc->m_pPointset!=NULL && m_pHistoSettings->bImageFeatures)
		{
			iPointType = HISTO_IMAGEFEATURES;
			//iPointCollectionType = m_PointCollectionCombo.GetCurSel();

			if(m_pHistoSettings->bLocal)
			{
				//will display circular neighborhood profile
				idROI = -1;
				idVertex = m_idVertexAtMouseLocation;
				iNumNeighbor = m_pHistoSettings->iNeighborOrder;
				idSegment = -1;
			}
			else if(m_pHistoSettings->bSegment && pDoc->m_pSegmentset!=NULL)
			{
				//will display segment edge strength profile
				idROI = -1;
				idVertex = -1;
				idSegment = m_idSegmentAtMouseLocation;
			}
			else if(m_pHistoSettings->bGlobal)  //m_pProfileSettings->bGlobal = TRUE for fusion info
			{
				idROI = -1;
				idVertex = -1;
				idSegment = -1;
			}
			else
			{
				//get selected roi
				idROI = pDoc->m_pROIset->GetSelectedROI();
				idVertex = -1;
				idSegment = -1;
			}
		}

		/*
		if(pDoc->m_pPointset!=NULL)
		{
			if(m_pHistoSettings->bGlobal)  
			{
				//will display global histogram
			}
			else if(pDoc->m_pPointset!=NULL && m_pHistoSettings->bLocal)
			{
				//will display circular neighborhood histogram
				idVertex = m_idVertexAtMouseLocation;
				iNumNeighbor = m_pHistoSettings->iNeighborOrder;
			}
		}
		if(pDoc->m_pSegmentset!=NULL && m_pHistoSettings->bSegment)
		{
			//will display segment histogram
			idSegment = m_idSegmentAtMouseLocation;
		}
		*/
	}

	m_bBusyUpdating = TRUE;
	//m_pHistogram->Update(pDoc, idchannelStat, iNumberOfLevel); 
	m_pHistogram->Update(	pDoc,
							iPointType,
							iPointCollectionType,
							idROI,
							idVertex, 
							iNumNeighbor,
							idSegment, 
							idchannelStat, 
							iNumberOfLevel);
	m_bBusyUpdating = FALSE;
	return TRUE;
}

void CHistoPage::OnSelchangeLocalstatchannelcombo() 
{
	m_pHistoSettings->idchannelStat = m_StatChannelComboBox.GetCurSel();
	UpdateHisto();
	Apply();
}

void CHistoPage::OnGivensegmentcheck() 
{
	m_pHistoSettings->bSegment = TRUE;
	m_pHistoSettings->bGlobal = FALSE;
	m_pHistoSettings->bLocal = FALSE;
	m_GlobalCheckBox.SetCheck(m_pHistoSettings->bGlobal);
	m_LocalNeighborhoodCheckBox.SetCheck(m_pHistoSettings->bLocal);
	//m_NeighborhoodEdit.SetWindowText("");
	m_GivenSegmentCheckBox.SetCheck(m_pHistoSettings->bSegment);
	CString myText;
	myText.Format(L"%d", m_pHistoSettings->idSegment);
	m_GivenSegmentEdit.SetWindowText(myText);

	FillComboBoxWithStatisticsChannelNames(&m_StatChannelComboBox);			
	m_StatChannelComboBox.SetCurSel(0);//SelectString(0, pszStatChannelNames[m_pProfileSettings->idchannelStat]);
	Apply();
	UpdateHisto();
}

void CHistoPage::OnSetfocusGivensegmentedit() 
{
	OnGivensegmentcheck();
}

void CHistoPage::OnChangeGivensegmentedit() 
{
	// TODO: Add your control notification handler code here
	
}

void CHistoPage::OnGlobalcheck() 
{
	//disable selection rect roi floating mode and mouse nearest neighbor mode
	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView->IsKindOf(RUNTIME_CLASS(COW2View))) 
	{
		if(((COW2View*)pView)->m_bMode_ROIFloating) ((COW2View*)pView)->DisableROIFloatingMode();
		((COW2View*)pView)->DisableMOUSENearestNeighborsMode();
	}
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc && pDoc->m_pPixelset) m_ImagePixelsCheckBox.EnableWindow(TRUE);
	m_PointCollectionCombo.EnableWindow(TRUE);
	m_PointCollectionCombo.SetCurSel(0);

	if(m_pHistoSettings->bSegment==FALSE)
	{
		m_pHistoSettings->bGlobal = TRUE;
		m_pHistoSettings->bSegment = FALSE;
		m_pHistoSettings->bLocal = FALSE;
	}
	else
	{
		m_pHistoSettings->bGlobal = FALSE;
		m_pHistoSettings->bSegment = FALSE;
		m_pHistoSettings->bLocal = FALSE;
	}
	m_GlobalCheckBox.SetCheck(m_pHistoSettings->bGlobal);
	m_ROICheck.SetCheck(!m_pHistoSettings->bGlobal && !m_pHistoSettings->bLocal);
	m_LocalNeighborhoodCheckBox.SetCheck(m_pHistoSettings->bLocal);
	//m_NeighborhoodEdit.SetWindowText("");
	m_GivenSegmentCheckBox.SetCheck(m_pHistoSettings->bSegment);
	//m_GivenSegmentEdit.SetWindowText("");

	Apply();
	UpdateHisto();
}


void CHistoPage::OnLocalneighborhoodcheck() 
{
	//enable mouse nearest neighbor mode
	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView->IsKindOf(RUNTIME_CLASS(COW2View))) 
	{
		if(((COW2View*)pView)->m_bMode_ROIFloating) ((COW2View*)pView)->DisableROIFloatingMode();
		((COW2View*)pView)->EnableMOUSENearestNeighborsMode();
	}
	m_ImagePixelsCheckBox.EnableWindow(FALSE);
	m_PointCollectionCombo.EnableWindow(FALSE);
	m_PointCollectionCombo.SetWindowText(L"Mouse Location");

	//switch to image features
	m_pHistoSettings->bImagePixels = FALSE;
	m_pHistoSettings->bImageFeatures = TRUE;
	m_ImagePixelsCheckBox.SetCheck(m_pHistoSettings->bImagePixels);
	m_ImageFeaturesCheckBox.SetCheck(m_pHistoSettings->bImageFeatures);
	//check local, uncheck others
	m_pHistoSettings->bGlobal = FALSE;
	m_pHistoSettings->bLocal = TRUE;
	m_pHistoSettings->bSegment = FALSE;
	m_GlobalCheckBox.SetCheck(m_pHistoSettings->bGlobal);
	m_LocalNeighborhoodCheckBox.SetCheck(m_pHistoSettings->bLocal);
	m_ROICheck.SetCheck(FALSE);
	m_GivenSegmentCheckBox.SetCheck(m_pHistoSettings->bSegment);
	//m_GivenSegmentEdit.SetWindowText("");

	CString myText;
	myText.Format(L"%d", m_pHistoSettings->iNeighborOrder);
	m_NeighborhoodEdit.SetWindowText(myText);

	FillComboBoxWithStatisticsChannelNames(&m_StatChannelComboBox);			
	m_StatChannelComboBox.SetCurSel(0);//SelectString(0, pszStatChannelNames[m_pProfileSettings->idchannelStat]);
	Apply();
	UpdateHisto();
}

void CHistoPage::OnLocalROIcheck() 
{
	//disable selection rect roi floating mode and mouse nearest neighbor mode
	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView && pView->IsKindOf(RUNTIME_CLASS(COW2View))) 
	{
		((COW2View*)pView)->DisableMOUSENearestNeighborsMode();
	}
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc && pDoc->m_pPixelset) m_ImagePixelsCheckBox.EnableWindow(TRUE);
	m_PointCollectionCombo.EnableWindow(TRUE);
	m_PointCollectionCombo.SetCurSel(0);

	if(m_pHistoSettings->bGlobal==FALSE && m_pHistoSettings->bLocal==FALSE)
	{
		m_pHistoSettings->bGlobal = TRUE;
		m_pHistoSettings->bLocal = FALSE;
		m_pHistoSettings->bSegment = FALSE;
	}
	else
	{
		m_pHistoSettings->bGlobal = FALSE;
		m_pHistoSettings->bLocal = FALSE;
		m_pHistoSettings->bSegment = FALSE;
	}
	m_GlobalCheckBox.SetCheck(m_pHistoSettings->bGlobal);
	m_ROICheck.SetCheck(!m_pHistoSettings->bGlobal && !m_pHistoSettings->bLocal);
	m_LocalNeighborhoodCheckBox.SetCheck(m_pHistoSettings->bLocal);
	//m_NeighborhoodEdit.SetWindowText("");
	m_GivenSegmentCheckBox.SetCheck(m_pHistoSettings->bSegment);
	//m_GivenSegmentEdit.SetWindowText("");

	Apply();
	UpdateHisto();
}

void CHistoPage::OnSelchangeLocalpointcollectioncombo() 
{
	Apply();
	UpdateHisto();
}

void CHistoPage::OnChangeNeighborhoodedit() 
{
	// TODO: Add your control notification handler code here
	
}

void CHistoPage::OnSetfocusNeighborhoodedit() 
{
	OnLocalneighborhoodcheck();	
}

void CHistoPage::OnImagepixelscheck() 
{
	//allowed to change?
	BOOL bImagePixelsPresent = FALSE;
	BOOL bImageFeaturesPresent = FALSE;
	BOOL bSegmentPresent = FALSE;
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc)
	{
		if(pDoc->m_pPixelset==NULL) bImagePixelsPresent = FALSE;
		  else bImagePixelsPresent = TRUE; 
		if(pDoc->m_pPointset==NULL) bImageFeaturesPresent = FALSE;
		  else bImageFeaturesPresent = TRUE; 
		if(pDoc->m_pSegmentset==NULL) bSegmentPresent = FALSE;
		  else bSegmentPresent = TRUE;

		if(m_pHistoSettings->bImagePixels==FALSE && bImagePixelsPresent)
		{
			m_pHistoSettings->bImagePixels = TRUE;
			m_pHistoSettings->bImageFeatures = FALSE;

			m_ImagePixelsCheckBox.SetCheck(m_pHistoSettings->bImagePixels);
			m_ImageFeaturesCheckBox.SetCheck(m_pHistoSettings->bImageFeatures);
			FillComboBoxWithImageChannelNames(&m_StatChannelComboBox);			
			m_StatChannelComboBox.SetCurSel(0);//SelectString(0, pszStatChannelNames[m_pProfileSettings->idchannelStat]);
			Apply();
			UpdateHisto();
			/*
			BOOL bState = !(m_ImagePixelsCheckBox.GetCheck());
			m_GlobalCheckBox.EnableWindow(!bState);
			m_LocalNeighborhoodCheckBox.EnableWindow(bState);
			m_GivenSegmentCheckBox.EnableWindow(bState);
			m_GivenSegmentEdit.EnableWindow(bState);
			m_NeighborhoodEdit.EnableWindow(bState);
			m_StatChannelComboBox.EnableWindow(bState);
			*/
		}
		else if(m_pHistoSettings->bImagePixels==TRUE && bImageFeaturesPresent)
		{
			m_pHistoSettings->bImagePixels = FALSE;
			m_pHistoSettings->bImageFeatures = TRUE;

			m_ImagePixelsCheckBox.SetCheck(m_pHistoSettings->bImagePixels);
			m_ImageFeaturesCheckBox.SetCheck(m_pHistoSettings->bImageFeatures);
			FillComboBoxWithStatisticsChannelNames(&m_StatChannelComboBox);			
			m_StatChannelComboBox.SetCurSel(0);//SelectString(0, pszStatChannelNames[m_pProfileSettings->idchannelStat]);
			Apply();
			UpdateHisto();
		}
		else
		{
			//don't change
			m_ImagePixelsCheckBox.SetCheck(m_pHistoSettings->bImagePixels);
			m_ImageFeaturesCheckBox.SetCheck(m_pHistoSettings->bImageFeatures);
		}
	}
}

void CHistoPage::OnImageFeaturescheck() 
{
	//allowed to change?
	BOOL bImagePixelsPresent = FALSE;
	BOOL bImageFeaturesPresent = FALSE;
	BOOL bSegmentPresent = FALSE;
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc)
	{
		if(pDoc->m_pPixelset==NULL) bImagePixelsPresent = FALSE;
		  else bImagePixelsPresent = TRUE; 
		if(pDoc->m_pPointset==NULL) bImageFeaturesPresent = FALSE;
		  else bImageFeaturesPresent = TRUE; 
		if(pDoc->m_pSegmentset==NULL) bSegmentPresent = FALSE;
		  else bSegmentPresent = TRUE;

		if(m_pHistoSettings->bImageFeatures==FALSE && bImageFeaturesPresent)
		{
			m_pHistoSettings->bImagePixels = FALSE;
			m_pHistoSettings->bImageFeatures = TRUE;

			m_ImagePixelsCheckBox.SetCheck(m_pHistoSettings->bImagePixels);
			m_ImageFeaturesCheckBox.SetCheck(m_pHistoSettings->bImageFeatures);
			FillComboBoxWithStatisticsChannelNames(&m_StatChannelComboBox);			
			m_StatChannelComboBox.SetCurSel(0);//SelectString(0, pszStatChannelNames[m_pProfileSettings->idchannelStat]);
			Apply();
			UpdateHisto();
			/*
			BOOL bState = !(m_ImagePixelsCheckBox.GetCheck());
			m_GlobalCheckBox.EnableWindow(!bState);
			m_LocalNeighborhoodCheckBox.EnableWindow(bState);
			m_GivenSegmentCheckBox.EnableWindow(bState);
			m_GivenSegmentEdit.EnableWindow(bState);
			m_NeighborhoodEdit.EnableWindow(bState);
			m_StatChannelComboBox.EnableWindow(bState);
			*/
		}
		else if(m_pHistoSettings->bImageFeatures==TRUE && bImagePixelsPresent)
		{
			m_pHistoSettings->bImagePixels = TRUE;
			m_pHistoSettings->bImageFeatures = FALSE;

			m_ImagePixelsCheckBox.SetCheck(m_pHistoSettings->bImagePixels);
			m_ImageFeaturesCheckBox.SetCheck(m_pHistoSettings->bImageFeatures);
			FillComboBoxWithImageChannelNames(&m_StatChannelComboBox);			
			m_StatChannelComboBox.SetCurSel(0);//SelectString(0, pszStatChannelNames[m_pProfileSettings->idchannelStat]);
			Apply();
			UpdateHisto();
		}
		else
		{
			//don't change
			m_ImagePixelsCheckBox.SetCheck(m_pHistoSettings->bImagePixels);
			m_ImageFeaturesCheckBox.SetCheck(m_pHistoSettings->bImageFeatures);
		}
	}
}

void CHistoPage::OnScaleAuto() 
{
	if(m_pHistoSettings->bAutoScale==FALSE)
	{
		m_pHistoSettings->bAutoScale = TRUE;
	}
	else
	{
		m_pHistoSettings->bAutoScale = FALSE;
		ScaleFix();
	}

	m_AutoScaleCheck.SetCheck(m_pHistoSettings->bAutoScale);
	m_FitScaleCheck.SetCheck(!m_pHistoSettings->bAutoScale);
	Apply();
	UpdateHisto();
}

void CHistoPage::OnScaleFit() 
{
	if(m_pHistoSettings->bAutoScale==TRUE)
	{
		m_pHistoSettings->bAutoScale = FALSE;
		ScaleFix();
	}
	else
	{
		m_pHistoSettings->bAutoScale = TRUE;
	}

	m_AutoScaleCheck.SetCheck(m_pHistoSettings->bAutoScale);
	m_FitScaleCheck.SetCheck(!m_pHistoSettings->bAutoScale);
	Apply();
	UpdateHisto();
}

void CHistoPage::ScaleFix()
{
	//fix scatter graph min max axis value with current ones
	POSITION pos = m_pHistogram->m_pHistoDoc->GetFirstViewPosition();
	CView* pFirstView = m_pHistogram->m_pHistoDoc->GetNextView(pos);
	if(pFirstView->IsKindOf(RUNTIME_CLASS(COWGraphView)))
	{
		double fMinY = ((COWGraphView*)pFirstView)->m_HISTOdfMinValueY; 
		double fMaxY = ((COWGraphView*)pFirstView)->m_HISTOdfMaxValueY;
		m_pHistogram->m_pHistoDoc->FixHISTOMinMaxXYValue(fMinY, fMaxY);
	}
	else
	{
		ASSERT(FALSE);
		m_pHistoSettings->bAutoScale = TRUE;
	}
}


BOOL CHistoPage::FillComboBoxWithPointCollectionNames(CComboBox* pComboBox)
{
	ASSERT(m_pSettingsSheet!=NULL);
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc==NULL) 
	{
		pComboBox->ResetContent();
		return FALSE;
	}

	pComboBox->ResetContent();
	for(int i=0; i<HISTO_NUMBEROF_POINTCOLLECTIONNAME; i++) 
	{
		CString myString(pszHistoPointCollectionNames[i]);
		pComboBox->AddString(myString);
	}
	return TRUE;
}




/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// scatterpage.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/*
#include "stdafx.h"
#include "oifii_app.h"

#include "owpropag.h"
#include "scatterpage.h"

#include "setsheet.h"
#include "scattersett.h"
#include "pointset.h"

#include "owdoc.h"
#include "ow2doc.h"

#include "owview.h"

#include "ow2mdi.h"
#include "owgraph.h"
#include "scattervw.h"
#include "scatter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

IMPLEMENT_DYNCREATE(CScatterPage, COWPropertyPage)

CScatterPage::CScatterPage() : COWPropertyPage(CScatterPage::IDD)
{
	m_pScatterGraph = NULL;
	m_idROI = -1;
	m_idVertexAtMouseLocation = -1;
	m_idSegmentAtMouseLocation = -1;

	m_bBusyUpdating = FALSE;

	//{{AFX_DATA_INIT(CScatterPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CScatterPage::~CScatterPage()
{
	if(m_pScatterGraph) delete(m_pScatterGraph);
}

void CScatterPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScatterPage)
	DDX_Control(pDX, IDC_LOCALROICHECK2, m_ROICheck);
	DDX_Control(pDX, IDC_LOCALPOINTCOLLECTIONCOMBO, m_PointCollectionCombo);
	DDX_Control(pDX, IDC_STATHISTOCHECK, m_ImageFeaturesCheckBox);
	DDX_Control(pDX, IDC_IMAGEHISTOCHECK, m_ImagePixelsCheckBox);
	DDX_Control(pDX, IDC_SCALE_FIT, m_FitScaleCheck);
	DDX_Control(pDX, IDC_SCALE_AUTO, m_AutoScaleCheck);
	DDX_Control(pDX, IDC_SCATTERCHANNELCOMBO2, m_StatChannelCombo2);
	DDX_Control(pDX, IDC_SCATTERCHANNELCOMBO1, m_StatChannelCombo1);
	DDX_Control(pDX, IDC_NEIGHBORHOODEDIT, m_NeighborhoodEdit);
	DDX_Control(pDX, IDC_LOCALNEIGHBORHOODCHECK, m_LocalNeighborhoodCheckBox);
	DDX_Control(pDX, IDC_GLOBALCHECK, m_GlobalCheckBox);
	DDX_Control(pDX, IDC_GIVENSEGMENTEDIT, m_GivenSegmentEdit);
	DDX_Control(pDX, IDC_GIVENSEGMENTCHECK, m_GivenSegmentCheckBox);
	DDX_Control(pDX, IDC_HISTOFRAMESTATIC, m_HistoFrameStaticButton);
	DDX_Control(pDX, IDC_OKBUTTON, m_OKButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScatterPage, COWPropertyPage)
	//{{AFX_MSG_MAP(CScatterPage)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_CANCELBUTTON, OnCancelbutton)
	ON_BN_CLICKED(IDC_OKBUTTON, OnOkbutton)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_SCATTERCHANNELCOMBO1, OnSelchangeScatterchannelcombo1)
	ON_CBN_SELCHANGE(IDC_SCATTERCHANNELCOMBO2, OnSelchangeScatterchannelcombo2)
	ON_BN_CLICKED(IDC_GIVENSEGMENTCHECK, OnGivensegmentcheck)
	ON_EN_CHANGE(IDC_GIVENSEGMENTEDIT, OnChangeGivensegmentedit)
	ON_EN_SETFOCUS(IDC_GIVENSEGMENTEDIT, OnSetfocusGivensegmentedit)
	ON_BN_CLICKED(IDC_GLOBALCHECK, OnGlobalcheck)
	ON_BN_CLICKED(IDC_LOCALNEIGHBORHOODCHECK, OnLocalneighborhoodcheck)
	ON_EN_CHANGE(IDC_NEIGHBORHOODEDIT, OnChangeNeighborhoodedit)
	ON_EN_SETFOCUS(IDC_NEIGHBORHOODEDIT, OnSetfocusNeighborhoodedit)
	ON_BN_CLICKED(IDC_SCALE_AUTO, OnScaleAuto)
	ON_BN_CLICKED(IDC_SCALE_FIT, OnScaleFit)
	ON_BN_CLICKED(IDC_IMAGEHISTOCHECK, OnImagepixelscheck)
	ON_BN_CLICKED(IDC_STATHISTOCHECK, OnImageFeaturescheck)
	ON_BN_CLICKED(IDC_LOCALROICHECK2, OnLocalROIcheck)
	ON_CBN_SELCHANGE(IDC_LOCALPOINTCOLLECTIONCOMBO, OnSelchangeLocalpointcollectioncombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CScatterPage::UpdateSettingsFromDocToSheet()
{
	CString myText;

	//0) assumptions for now
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc!=NULL) ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));

	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView!=NULL) ASSERT(pView->IsKindOf(RUNTIME_CLASS(COWView)));
	
	//1) copy view parameters from attached view
	if(pDoc)
	{
		m_pScatterSettings->CopyAllValuesFrom(pDoc->m_pScatterSettings);
	}
	else 
	{
		m_pScatterSettings->CopyAllValuesFrom(NULL); //Erase
	}

	//2) set CScatterPage controls ...
	BOOL bState = FALSE;
	if(pDoc==NULL || pView==NULL)
	{
		bState = FALSE;
	}
	else
	{
		bState = TRUE;
	}

	//enable/disable controls
	m_OKButton.EnableWindow(TRUE);

	BOOL bImagePixelsPresent = FALSE;
	BOOL bImageFeaturesPresent = FALSE;
	BOOL bSegmentPresent = FALSE;
	if(pDoc)
	{
		if(pDoc->m_pPixelset==NULL) bImagePixelsPresent = FALSE;
		  else bImagePixelsPresent = TRUE; 
		if(pDoc->m_pPointset==NULL) bImageFeaturesPresent = FALSE;
		  else bImageFeaturesPresent = TRUE; 
		if(pDoc->m_pSegmentset==NULL) bSegmentPresent = FALSE;
		  else bSegmentPresent = TRUE;
	}

	m_FitScaleCheck.EnableWindow(bState);
	m_AutoScaleCheck.EnableWindow(bState);
	m_ImagePixelsCheckBox.EnableWindow(bState & bImagePixelsPresent);
	m_ImageFeaturesCheckBox.EnableWindow(bState & bImageFeaturesPresent);
	m_GlobalCheckBox.EnableWindow(bState);
	m_ROICheck.EnableWindow(bState);
	m_LocalNeighborhoodCheckBox.EnableWindow(bState & bImageFeaturesPresent);
	m_NeighborhoodEdit.EnableWindow(bState & bImageFeaturesPresent);
	m_GivenSegmentCheckBox.EnableWindow(bState & bSegmentPresent);
	m_GivenSegmentEdit.EnableWindow(bState & bSegmentPresent);
	m_StatChannelCombo1.EnableWindow(bState);
	m_StatChannelCombo2.EnableWindow(bState);
	m_PointCollectionCombo.EnableWindow(bState);

	//set controls values
	myText.Format(L"%d", m_pScatterSettings->iNeighborOrder);
	m_NeighborhoodEdit.SetWindowText(myText);
	myText.Format(L"%d", m_pScatterSettings->idSegment);
	m_GivenSegmentEdit.SetWindowText(myText);

	m_ImagePixelsCheckBox.SetCheck(m_pScatterSettings->bImagePixels);
	m_ImageFeaturesCheckBox.SetCheck(m_pScatterSettings->bImageFeatures);

	m_GlobalCheckBox.SetCheck(m_pScatterSettings->bGlobal);
	m_ROICheck.SetCheck(!m_pScatterSettings->bGlobal && !m_pScatterSettings->bLocal);
	m_LocalNeighborhoodCheckBox.SetCheck(m_pScatterSettings->bLocal);
	m_GivenSegmentCheckBox.SetCheck(m_pScatterSettings->bSegment);

	//fill point collection combo
	FillComboBoxWithPointCollectionNames(&m_PointCollectionCombo);	
	m_PointCollectionCombo.SetCurSel(0);

	//fill information combo
	if(m_pScatterSettings->bImagePixels)
	{
		//use combo to select image pixels statistics
		FillComboBoxWithImageChannelNames(&m_StatChannelCombo1);
		if(m_StatChannelCombo1.GetCount()==1) 
		{
			m_StatChannelCombo1.EnableWindow(FALSE);
		}
		m_pScatterSettings->idchannelStat1 = 0;
		m_StatChannelCombo1.SetCurSel(m_pScatterSettings->idchannelStat1);
		FillComboBoxWithImageChannelNames(&m_StatChannelCombo2);	
		if(m_StatChannelCombo2.GetCount()==1) 
		{
			m_StatChannelCombo2.EnableWindow(FALSE);
			m_pScatterSettings->idchannelStat2 = 0;
			m_StatChannelCombo2.SetCurSel(m_pScatterSettings->idchannelStat2);
		}
		else
		{
			m_pScatterSettings->idchannelStat2 = 1;
			m_StatChannelCombo2.SetCurSel(m_pScatterSettings->idchannelStat2);
		}
	}
	else if(m_pScatterSettings->bImageFeatures) 
	{
		//use combo to select image features statistics
		FillComboBoxWithStatisticsChannelNames(&m_StatChannelCombo1);			
		m_StatChannelCombo1.SetCurSel(0); //SelectString(0, pszProfilePointCollectionNames[m_pScatterSettings->idchannelStat1]);
		m_StatChannelCombo1.EnableWindow(TRUE);
		FillComboBoxWithStatisticsChannelNames(&m_StatChannelCombo2);			
		m_StatChannelCombo2.SetCurSel(1); //SelectString(0, pszProfilePointCollectionNames[m_pScatterSettings->idchannelStat2]);
		m_StatChannelCombo2.EnableWindow(TRUE);	
	}
	else
	{
		ASSERT(FALSE);
	}

	m_AutoScaleCheck.SetCheck(m_pScatterSettings->bAutoScale);
	m_FitScaleCheck.SetCheck(!m_pScatterSettings->bAutoScale);
	return TRUE;
}

BOOL CScatterPage::UpdateSettingsFromSheetToDoc()
{
	int iInteger, iValidatedInteger;
	WCHAR pszText[255];

	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc!=NULL) 
	{
		if(!pDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)))
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	m_NeighborhoodEdit.GetWindowText(pszText, 50);
	iInteger = _wtoi(pszText);
	iValidatedInteger = iInteger; //no validation for now
	m_pScatterSettings->iNeighborOrder = iValidatedInteger;

	m_GivenSegmentEdit.GetWindowText(pszText, 50);
	iInteger = _wtoi(pszText);
	iValidatedInteger = iInteger; //no validation for now
	m_pScatterSettings->idSegment	= iValidatedInteger;

	m_pScatterSettings->bGlobal = m_GlobalCheckBox.GetCheck();
	m_pScatterSettings->bLocal = m_LocalNeighborhoodCheckBox.GetCheck();
	m_pScatterSettings->bSegment = m_GivenSegmentCheckBox.GetCheck();

	m_pScatterSettings->idchannelStat1 = m_StatChannelCombo1.GetCurSel();
	m_pScatterSettings->idchannelStat2 = m_StatChannelCombo2.GetCurSel();

	m_pScatterSettings->bAutoScale = m_AutoScaleCheck.GetCheck();

	//3) copy view parameters from sheet to attached doc
	if(pDoc) pDoc->m_pScatterSettings->CopyAllValuesFrom(m_pScatterSettings);
	return TRUE;
}

int CScatterPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COWPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CWnd* myParent = GetParent();
	if(myParent!=NULL)
	{
		ASSERT(myParent->IsKindOf(RUNTIME_CLASS(CSettingsSheet)));
		m_pSettingsSheet = (CSettingsSheet*)myParent;
		m_pScatterSettings = m_pSettingsSheet->m_pScatterSettings;
	}
	
	return 0;
}

BOOL CScatterPage::OnSetActive() 
{
	
	UpdateSettingsFromDocToSheet();	
	UpdateScatter();
	
	return COWPropertyPage::OnSetActive();
}

BOOL CScatterPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COWPropertyPage::OnKillActive();
}


void CScatterPage::OnCancelbutton() 
{
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}

void CScatterPage::Apply() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
}

void CScatterPage::OnOkbutton() 
{
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	COWView* pView = m_pSettingsSheet->GetAttachedView();	
	if(pDoc!=NULL && pView!=NULL) 
	{
		UpdateSettingsFromSheetToDoc();	
	}
	m_pSettingsSheet->ShowWindow(SW_HIDE);
}

void CScatterPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// Do not call COWPropertyPage::OnPaint() for painting messages
	/*
	CRect myRect, myRefRect, myHistoRect;
	m_HistoFrameStaticButton.GetWindowRect(&myRect);
	this->GetWindowRect(&myRefRect);
	myHistoRect.top = myRect.top - myRefRect.top;
	myHistoRect.left = myRect.left - myRefRect.left;
	myHistoRect.bottom = myHistoRect.top + myRect.Height();
	myHistoRect.right = myHistoRect.left + myRect.Width();
	

	CBrush myBrush(RGB(255,0,0));
	CBrush* pOldBrush = dc.SelectObject(&myBrush);
	CPen myPen(PS_SOLID, 1, RGB(255,0,0));
	CPen* pOldPen = dc.SelectObject(&myPen);
	dc.Rectangle(&myHistoRect);
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
	*/
}

BOOL CScatterPage::OnInitDialog() 
{
	COWPropertyPage::OnInitDialog();

	UpdateSettingsFromDocToSheet();	
	
	CRect myRect, myRefRect, myHistoRect;
	m_HistoFrameStaticButton.GetWindowRect(&myRect);
	this->GetWindowRect(&myRefRect);
	myHistoRect.top = myRect.top - myRefRect.top;
	myHistoRect.left = myRect.left - myRefRect.left;
	myHistoRect.bottom = myHistoRect.top + myRect.Height();
	myHistoRect.right = myHistoRect.left + myRect.Width();
	
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();

	//int iNumberOfPoints = 0;
	//if(pDoc!=NULL && pDoc->m_pPointset!=NULL) iNumberOfPoints = pDoc->GetNumberOfPoint();;

	//initially, do not draw anything
	int iPointType = 0;
	int iPointCollectionType = 0;
	int idROI = -1;
	int idVertex = -1;
	int iNumNeighbor = -1;
	int idSegment = -1;
	m_pScatterGraph = new CScatterGraph(	myHistoRect, 
										pDoc, 
										(CWnd*)this,
										iPointType,
										iPointCollectionType,
										idROI,
										idVertex,
										iNumNeighbor,
										idSegment, 
										m_pScatterSettings->idchannelStat1, //x
										m_pScatterSettings->idchannelStat2); //y
	ASSERT(m_pScatterGraph);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


LONG CScatterPage::OnMouseMovedUpdateLocalGraph(struct tagCONTEXTINFO* pGlobalStructCONTEXTINFO)
{
	//poirier, sept 2001, begin
	if(pGlobalStructCONTEXTINFO==NULL)
	{
		m_idROI = 0;
		m_idVertexAtMouseLocation = 0;
		m_idSegmentAtMouseLocation = 0;
	}
	//poirier, sept 2001, end
	else
	{
		m_idROI = pGlobalStructCONTEXTINFO->idROI;
		m_idVertexAtMouseLocation = pGlobalStructCONTEXTINFO->idVertex;
		m_idSegmentAtMouseLocation = pGlobalStructCONTEXTINFO->idSegment;
	}
	if(!m_bBusyUpdating) UpdateScatter();
	return TRUE;
}

BOOL CScatterPage::UpdateScatter()
{
 	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	//if(pDoc==NULL) return FALSE;
	//if(pDoc->m_pPointset==NULL) return FALSE;

	int iPointType = 0;
	int iPointCollectionType = m_PointCollectionCombo.GetCurSel();
	int idROI = -1;
	int idVertex = -1;
	int iNumNeighbor = -1;
	int idSegment = -1;

	if(pDoc!=NULL)
	{
		if(pDoc->m_pPixelset!=NULL && m_pScatterSettings->bImagePixels)
		{
			iPointType = SCATTER_IMAGEPIXELS;
			//iPointCollectionType = m_PointCollectionCombo.GetCurSel();

			if(m_pScatterSettings->bLocal && pDoc->m_pPointset!=NULL)
			{
				idROI = -1;
				idVertex = m_idVertexAtMouseLocation;
				idSegment = -1;
			}
			else if(m_pScatterSettings->bSegment && pDoc->m_pSegmentset!=NULL)
			{
				idROI = -1;
				idVertex = -1;
				idSegment = m_idSegmentAtMouseLocation;
			}
			else if(m_pScatterSettings->bGlobal)
			{
				idROI = -1;
				idVertex = -1;
				idSegment = -1;
			}
			else if(pDoc->m_pROIset)
			{
				//get selected roi
				idROI = pDoc->m_pROIset->GetSelectedROI();
				idVertex = -1;
				idSegment = -1;
			}
		}
		else if(pDoc->m_pPointset!=NULL && m_pScatterSettings->bImageFeatures)
		{
			iPointType = SCATTER_IMAGEFEATURES;
			//iPointCollectionType = m_PointCollectionCombo.GetCurSel();


			if(m_pScatterSettings->bLocal)
			{
				//will display circular neighborhood profile
				idROI = -1;
				idVertex = m_idVertexAtMouseLocation;
				iNumNeighbor = m_pScatterSettings->iNeighborOrder;
				idSegment = -1;
			}
			else if(m_pScatterSettings->bSegment && pDoc->m_pSegmentset!=NULL)
			{
				//will display segment edge strength profile
				idROI = -1;
				idVertex = -1;
				idSegment = m_idSegmentAtMouseLocation;
			}
			else if(m_pScatterSettings->bGlobal)  //m_pProfileSettings->bGlobal = TRUE for fusion info
			{
				idROI = -1;
				idVertex = -1;
				idSegment = -1;
			}
			else
			{
				//get selected roi
				idROI = pDoc->m_pROIset->GetSelectedROI();
				idVertex = -1;
				idSegment = -1;
			}
		}
	}
	m_bBusyUpdating = TRUE;

	m_pScatterGraph->m_pScatterGraphDoc->SetAutoRescaling(m_pScatterSettings->bAutoScale);

	//int iNumberOfPoint = pDoc->GetNumberOfPoint();
	ASSERT(m_pScatterGraph->IsKindOf(RUNTIME_CLASS(CScatterGraph)));
	m_pScatterGraph->Update(pDoc, 
							iPointType,
							iPointCollectionType,
							idROI,
							idVertex, 
							iNumNeighbor, 
							idSegment, 
							m_pScatterSettings->idchannelStat1, 
							m_pScatterSettings->idchannelStat2); 
	m_bBusyUpdating = FALSE;
	return TRUE;
}

void CScatterPage::OnSelchangeScatterchannelcombo1() 
{
	m_pScatterSettings->idchannelStat1 = m_StatChannelCombo1.GetCurSel();
	UpdateScatter();
	Apply();
}

void CScatterPage::OnSelchangeScatterchannelcombo2() 
{
	m_pScatterSettings->idchannelStat2 = m_StatChannelCombo2.GetCurSel();
	UpdateScatter();
	Apply();
}

void CScatterPage::OnGivensegmentcheck() 
{
	m_pScatterSettings->bSegment = TRUE;
	m_pScatterSettings->bGlobal = FALSE;
	m_pScatterSettings->bLocal = FALSE;
	m_GlobalCheckBox.SetCheck(m_pScatterSettings->bGlobal);
	m_LocalNeighborhoodCheckBox.SetCheck(m_pScatterSettings->bLocal);
	//m_NeighborhoodEdit.SetWindowText("");
	m_GivenSegmentCheckBox.SetCheck(m_pScatterSettings->bSegment);
	CString myText;
	myText.Format(L"%d", m_pScatterSettings->idSegment);
	m_GivenSegmentEdit.SetWindowText(myText);

	//use combo to select image features statistics
	FillComboBoxWithStatisticsChannelNames(&m_StatChannelCombo1);			
	m_StatChannelCombo1.SetCurSel(0); //SelectString(0, pszProfilePointCollectionNames[m_pScatterSettings->idchannelStat1]);
	FillComboBoxWithStatisticsChannelNames(&m_StatChannelCombo2);			
	m_StatChannelCombo2.SetCurSel(1); //SelectString(0, pszProfilePointCollectionNames[m_pScatterSettings->idchannelStat2]);
	Apply();
	UpdateScatter();
}

void CScatterPage::OnChangeGivensegmentedit() 
{
}

void CScatterPage::OnSetfocusGivensegmentedit() 
{
	OnGivensegmentcheck();
}

void CScatterPage::OnGlobalcheck() 
{
	//disable selection rect roi floating mode and mouse nearest neighbor mode
	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView->IsKindOf(RUNTIME_CLASS(COW2View))) 
	{
		if(((COW2View*)pView)->m_bMode_ROIFloating) ((COW2View*)pView)->DisableROIFloatingMode();
		((COW2View*)pView)->DisableMOUSENearestNeighborsMode();
	}
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc && pDoc->m_pPixelset) m_ImagePixelsCheckBox.EnableWindow(TRUE);
	m_PointCollectionCombo.EnableWindow(TRUE);
	m_PointCollectionCombo.SetCurSel(0);

	if(m_pScatterSettings->bGlobal==FALSE)
	{
		m_pScatterSettings->bGlobal = TRUE;
		m_pScatterSettings->bLocal = FALSE;
		m_pScatterSettings->bSegment = FALSE;
	}
	else
	{
		m_pScatterSettings->bGlobal = FALSE;
		m_pScatterSettings->bLocal = FALSE;
		m_pScatterSettings->bSegment = FALSE;
	}
	m_GlobalCheckBox.SetCheck(m_pScatterSettings->bGlobal);
	m_ROICheck.SetCheck(!m_pScatterSettings->bGlobal && !m_pScatterSettings->bLocal);
	m_LocalNeighborhoodCheckBox.SetCheck(m_pScatterSettings->bLocal);
	//m_NeighborhoodEdit.SetWindowText("");
	m_GivenSegmentCheckBox.SetCheck(m_pScatterSettings->bSegment);
	//m_GivenSegmentEdit.SetWindowText("");

	Apply();
	UpdateScatter();
}

void CScatterPage::OnLocalneighborhoodcheck() 
{
	//enable mouse nearest neighbor mode
	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView->IsKindOf(RUNTIME_CLASS(COW2View))) 
	{
		if(((COW2View*)pView)->m_bMode_ROIFloating) ((COW2View*)pView)->DisableROIFloatingMode();
		((COW2View*)pView)->EnableMOUSENearestNeighborsMode();
	}
	m_ImagePixelsCheckBox.EnableWindow(FALSE);
	m_PointCollectionCombo.EnableWindow(FALSE);
	m_PointCollectionCombo.SetWindowText(L"Mouse Location");

	//switch to image features
	m_pScatterSettings->bImagePixels = FALSE;
	m_pScatterSettings->bImageFeatures = TRUE;
	m_ImagePixelsCheckBox.SetCheck(m_pScatterSettings->bImagePixels);
	m_ImageFeaturesCheckBox.SetCheck(m_pScatterSettings->bImageFeatures);
	//check local, uncheck others
	m_pScatterSettings->bGlobal = FALSE;
	m_pScatterSettings->bLocal = TRUE;
	m_pScatterSettings->bSegment = FALSE;
	m_GlobalCheckBox.SetCheck(m_pScatterSettings->bGlobal);
	m_LocalNeighborhoodCheckBox.SetCheck(m_pScatterSettings->bLocal);
	m_ROICheck.SetCheck(FALSE);
	m_GivenSegmentCheckBox.SetCheck(m_pScatterSettings->bSegment);
	m_GivenSegmentEdit.SetWindowText(L"");

	CString myText;
	myText.Format(L"%d", m_pScatterSettings->iNeighborOrder);
	m_NeighborhoodEdit.SetWindowText(myText);

	//use combo to select image features statistics
	FillComboBoxWithStatisticsChannelNames(&m_StatChannelCombo1);			
	m_StatChannelCombo1.SetCurSel(0); //SelectString(0, pszProfilePointCollectionNames[m_pScatterSettings->idchannelStat1]);
	FillComboBoxWithStatisticsChannelNames(&m_StatChannelCombo2);			
	m_StatChannelCombo2.SetCurSel(1); //SelectString(0, pszProfilePointCollectionNames[m_pScatterSettings->idchannelStat2]);
	Apply();
	UpdateScatter();
}

void CScatterPage::OnChangeNeighborhoodedit() 
{
	// TODO: Add your control notification handler code here
	
}

void CScatterPage::OnSetfocusNeighborhoodedit() 
{
	OnLocalneighborhoodcheck();	
}

void CScatterPage::OnScaleAuto() 
{
	if(m_pScatterSettings->bAutoScale==FALSE)
	{
		m_pScatterSettings->bAutoScale = TRUE;
	}
	else
	{
		m_pScatterSettings->bAutoScale = FALSE;
		ScaleFix();
	}

	m_AutoScaleCheck.SetCheck(m_pScatterSettings->bAutoScale);
	m_FitScaleCheck.SetCheck(!m_pScatterSettings->bAutoScale);
	Apply();
	UpdateScatter();
}

void CScatterPage::OnScaleFit() 
{
	if(m_pScatterSettings->bAutoScale==TRUE)
	{
		m_pScatterSettings->bAutoScale = FALSE;
		ScaleFix();
	}
	else
	{
		m_pScatterSettings->bAutoScale = TRUE;
	}
	m_AutoScaleCheck.SetCheck(m_pScatterSettings->bAutoScale);
	m_FitScaleCheck.SetCheck(!m_pScatterSettings->bAutoScale);
	Apply();
	UpdateScatter();
}

void CScatterPage::ScaleFix()
{
	//fix scatter graph min max axis value with current ones
	POSITION pos = m_pScatterGraph->m_pScatterGraphDoc->GetFirstViewPosition();
	CView* pFirstView = m_pScatterGraph->m_pScatterGraphDoc->GetNextView(pos);
	if(pFirstView->IsKindOf(RUNTIME_CLASS(COWGraphView)))
	{
		double fMinX = ((COWGraphView*)pFirstView)->m_SCATTERdfMinValueX; 
		double fMaxX = ((COWGraphView*)pFirstView)->m_SCATTERdfMaxValueX;
		double fMinY = ((COWGraphView*)pFirstView)->m_SCATTERdfMinValueY;
		double fMaxY = ((COWGraphView*)pFirstView)->m_SCATTERdfMaxValueY;
		m_pScatterGraph->m_pScatterGraphDoc->FixSCATTERMinMaxXYValue(fMinX, fMaxX, fMinY, fMaxY);
	}
	else
	{
		ASSERT(FALSE);
		m_pScatterSettings->bAutoScale = TRUE;
		m_AutoScaleCheck.SetCheck(m_pScatterSettings->bAutoScale);
		m_FitScaleCheck.SetCheck(!m_pScatterSettings->bAutoScale);
	}
}

void CScatterPage::OnImagepixelscheck() 
{
	//allowed to change?
	BOOL bImagePixelsPresent = FALSE;
	BOOL bImageFeaturesPresent = FALSE;
	BOOL bSegmentPresent = FALSE;
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc)
	{
		if(pDoc->m_pPixelset==NULL) bImagePixelsPresent = FALSE;
		  else bImagePixelsPresent = TRUE; 
		if(pDoc->m_pPointset==NULL) bImageFeaturesPresent = FALSE;
		  else bImageFeaturesPresent = TRUE; 
		if(pDoc->m_pSegmentset==NULL) bSegmentPresent = FALSE;
		  else bSegmentPresent = TRUE;

		if(m_pScatterSettings->bImagePixels==FALSE && bImagePixelsPresent)
		{
			m_pScatterSettings->bImagePixels = TRUE;
			m_pScatterSettings->bImageFeatures = FALSE;

			m_ImagePixelsCheckBox.SetCheck(m_pScatterSettings->bImagePixels);
			m_ImageFeaturesCheckBox.SetCheck(m_pScatterSettings->bImageFeatures);
			//use combo to select image features statistics
			FillComboBoxWithImageChannelNames(&m_StatChannelCombo1);			
			if(m_StatChannelCombo1.GetCount()==1) m_StatChannelCombo1.EnableWindow(FALSE);
			m_StatChannelCombo1.SetCurSel(0); //SelectString(0, pszProfilePointCollectionNames[m_pScatterSettings->idchannelStat1]);
			FillComboBoxWithImageChannelNames(&m_StatChannelCombo2);			
			if(m_StatChannelCombo2.GetCount()==1) 
			{
				m_StatChannelCombo2.SetCurSel(0); //SelectString(0, pszProfilePointCollectionNames[m_pScatterSettings->idchannelStat2]);
				m_StatChannelCombo2.EnableWindow(FALSE);
			}
			else
			{
				m_StatChannelCombo2.SetCurSel(1); //SelectString(0, pszProfilePointCollectionNames[m_pScatterSettings->idchannelStat2]);
			}
			Apply();
			UpdateScatter();
		}
		else if(m_pScatterSettings->bImagePixels==TRUE && bImageFeaturesPresent)
		{
			m_pScatterSettings->bImagePixels = FALSE;
			m_pScatterSettings->bImageFeatures = TRUE;

			m_ImagePixelsCheckBox.SetCheck(m_pScatterSettings->bImagePixels);
			m_ImageFeaturesCheckBox.SetCheck(m_pScatterSettings->bImageFeatures);
			//use combo to select image features statistics
			FillComboBoxWithStatisticsChannelNames(&m_StatChannelCombo1);			
			m_StatChannelCombo1.SetCurSel(0); //SelectString(0, pszProfilePointCollectionNames[m_pScatterSettings->idchannelStat1]);
			m_StatChannelCombo1.EnableWindow(TRUE);
			FillComboBoxWithStatisticsChannelNames(&m_StatChannelCombo2);			
			m_StatChannelCombo2.SetCurSel(1); //SelectString(0, pszProfilePointCollectionNames[m_pScatterSettings->idchannelStat2]);
			m_StatChannelCombo2.EnableWindow(TRUE);
			Apply();
			UpdateScatter();
		}
		else
		{
			//don't change
			m_ImagePixelsCheckBox.SetCheck(m_pScatterSettings->bImagePixels);
			m_ImageFeaturesCheckBox.SetCheck(m_pScatterSettings->bImageFeatures);
		}

	}
}

void CScatterPage::OnImageFeaturescheck() 
{
	//allowed to change?
	BOOL bImagePixelsPresent = FALSE;
	BOOL bImageFeaturesPresent = FALSE;
	BOOL bSegmentPresent = FALSE;
	COW2Doc * pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc)
	{
		if(pDoc->m_pPixelset==NULL) bImagePixelsPresent = FALSE;
		  else bImagePixelsPresent = TRUE; 
		if(pDoc->m_pPointset==NULL) bImageFeaturesPresent = FALSE;
		  else bImageFeaturesPresent = TRUE; 
		if(pDoc->m_pSegmentset==NULL) bSegmentPresent = FALSE;
		  else bSegmentPresent = TRUE;

		if(m_pScatterSettings->bImageFeatures==FALSE && bImageFeaturesPresent)
		{
			m_pScatterSettings->bImagePixels = FALSE;
			m_pScatterSettings->bImageFeatures = TRUE;

			m_ImagePixelsCheckBox.SetCheck(m_pScatterSettings->bImagePixels);
			m_ImageFeaturesCheckBox.SetCheck(m_pScatterSettings->bImageFeatures);
			//use combo to select image features statistics
			FillComboBoxWithStatisticsChannelNames(&m_StatChannelCombo1);			
			m_StatChannelCombo1.SetCurSel(0); //SelectString(0, pszProfilePointCollectionNames[m_pScatterSettings->idchannelStat1]);
			m_StatChannelCombo1.EnableWindow(TRUE);	
			FillComboBoxWithStatisticsChannelNames(&m_StatChannelCombo2);			
			m_StatChannelCombo2.SetCurSel(1); //SelectString(0, pszProfilePointCollectionNames[m_pScatterSettings->idchannelStat2]);
			m_StatChannelCombo2.EnableWindow(TRUE);	
			Apply();
			UpdateScatter();
		}
		else if(m_pScatterSettings->bImageFeatures==TRUE && bImagePixelsPresent)
		{
			m_pScatterSettings->bImagePixels = TRUE;
			m_pScatterSettings->bImageFeatures = FALSE;

			m_ImagePixelsCheckBox.SetCheck(m_pScatterSettings->bImagePixels);
			m_ImageFeaturesCheckBox.SetCheck(m_pScatterSettings->bImageFeatures);
			//use combo to select image features statistics
			FillComboBoxWithImageChannelNames(&m_StatChannelCombo1);			
			m_StatChannelCombo1.SetCurSel(0); //SelectString(0, pszProfilePointCollectionNames[m_pScatterSettings->idchannelStat1]);
			if(m_StatChannelCombo1.GetCount()==1) m_StatChannelCombo1.EnableWindow(FALSE);
			FillComboBoxWithImageChannelNames(&m_StatChannelCombo2);			
			if(m_StatChannelCombo2.GetCount()==1) 
			{
				m_StatChannelCombo2.SetCurSel(0); //SelectString(0, pszProfilePointCollectionNames[m_pScatterSettings->idchannelStat2]);
				m_StatChannelCombo2.EnableWindow(FALSE);
			}
			else
			{
				m_StatChannelCombo2.SetCurSel(1); //SelectString(0, pszProfilePointCollectionNames[m_pScatterSettings->idchannelStat2]);
			}
			Apply();
			UpdateScatter();
		}
		else
		{
			//don't change
			m_ImagePixelsCheckBox.SetCheck(m_pScatterSettings->bImagePixels);
			m_ImageFeaturesCheckBox.SetCheck(m_pScatterSettings->bImageFeatures);
		}

	}
}



void CScatterPage::OnLocalROIcheck() 
{
	//disable selection rect roi floating mode and mouse nearest neighbor mode
	COWView* pView = m_pSettingsSheet->GetAttachedView();
	if(pView && pView->IsKindOf(RUNTIME_CLASS(COW2View))) 
	{
		((COW2View*)pView)->DisableMOUSENearestNeighborsMode();
	}
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc && pDoc->m_pPixelset) m_ImagePixelsCheckBox.EnableWindow(TRUE);
	m_PointCollectionCombo.EnableWindow(TRUE);
	m_PointCollectionCombo.SetCurSel(0);

	if(m_pScatterSettings->bGlobal==FALSE && m_pScatterSettings->bLocal==FALSE)
	{
		m_pScatterSettings->bGlobal = TRUE;
		m_pScatterSettings->bLocal = FALSE;
		m_pScatterSettings->bSegment = FALSE;
	}
	else
	{
		m_pScatterSettings->bGlobal = FALSE;
		m_pScatterSettings->bLocal = FALSE;
		m_pScatterSettings->bSegment = FALSE;
	}
	m_GlobalCheckBox.SetCheck(m_pScatterSettings->bGlobal);
	m_ROICheck.SetCheck(!m_pScatterSettings->bGlobal && !m_pScatterSettings->bLocal);
	m_LocalNeighborhoodCheckBox.SetCheck(m_pScatterSettings->bLocal);
	//m_NeighborhoodEdit.SetWindowText("");
	m_GivenSegmentCheckBox.SetCheck(m_pScatterSettings->bSegment);
	//m_GivenSegmentEdit.SetWindowText("");

	Apply();
	UpdateScatter();
}

void CScatterPage::OnSelchangeLocalpointcollectioncombo() 
{
	Apply();
	UpdateScatter();
}



BOOL CScatterPage::FillComboBoxWithPointCollectionNames(CComboBox* pComboBox)
{
	ASSERT(m_pSettingsSheet!=NULL);
	COW2Doc* pDoc = m_pSettingsSheet->GetAttachedDoc();
	if(pDoc==NULL) 
	{
		pComboBox->ResetContent();
		return FALSE;
	}

	pComboBox->ResetContent();
	for(int i=0; i<SCATTER_NUMBEROF_POINTCOLLECTIONNAME; i++) 
	{
		CString myString(pszScatterPointCollectionNames[i]);
		pComboBox->AddString(myString);
	}
	return TRUE;
}
