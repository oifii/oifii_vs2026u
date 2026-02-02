// OIII.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "oifiilib.h" //spi 2014
#include "resource.h"

//2012august23, poirier, begin
#include "portaudio.h"
#include "pa_asio.h"
#include <map>
#include <string>
using namespace std;
//2012august23, poirier, end
#include "oifii_mainfrm.h"

#include "OWpixelset.h"

#include "c_pointset.h" //offworld c pointset library
#include "c_segmentset.h" //offworld c segmentset library

#include "OWpointset.h" //offworld c++ pointset library (interface sitting on pointset.h)
#include "OWsegmentset.h" //offworld c++ segmentset library (interface sitting on segmentset.h)
#include "OWclassset.h" //offworld c++ classset library (interface sitting on segmentset.h)
#include "OWedgeset.h" //offworld edgeset library (for line, polyline, polygone  and shape)

#include "OWdoc.h"
#include "OW2doc.h"
#include "OW2mdi.h"
#include "OWview.h"
#include "OW2view.h"
#include "OWHtmlView.h"

#include <gl\gl.h>
#include <gl\glu.h>
#include "ow3view.h"

#include "logdoc.h"
#include "logview.h"
#include "logmdi.h"

#include "oifii_gotodlg.h"

#include "oifii_propsettings.h"
#include "oifii_proppage.h"
#include "oifii_propsheet.h"

#include "gl_owgraph.h"
#define COMPILING_OIII_CPP
#include "oifii_app.h"


#include "owmultidoc.h"

//2012august09, poirier, begin
#include "OW2AGdoc.h"
#include "OW2AGmdi.h"
#include "OW2AGview.h"
//2012august09, poirier, end


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
struct tagCONTEXTINFO
{
	int idROI;
	int idVertex;
	int idSegment;
} 
*/
struct tagCONTEXTINFO globalstructCONTEXTINFO;




BEGIN_MESSAGE_MAP(COIIIApp, CWinApp)
	//{{AFX_MSG_MAP(COIIIApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_SETTINGS_CLASSIFICATION, OnSettingsClassification)
	ON_COMMAND(ID_SETTINGS_EXTRACTION, OnSettingsExtraction)
	ON_COMMAND(ID_SETTINGS_FUSION, OnSettingsFusion)
	ON_COMMAND(ID_SETTINGS_SCATTER, OnSettingsScatter)
	ON_COMMAND(ID_SETTINGS_IDLEBEHAVIOR, OnSettingsBehavior)
	ON_COMMAND(ID_SETTINGS_HISTO, OnSettingsHisto)
	ON_COMMAND(ID_SETTINGS_PROJECTION, OnSettingsProjection)
	ON_COMMAND(ID_SETTINGS_TRACEBEHAVIOR, OnSettingsTrace)
	ON_COMMAND(ID_SETTINGS_TRANSFORMATION, OnSettingsTransformation)
	ON_COMMAND(ID_SETTINGS_VIEWDATA, OnSettingsViewdata)
	ON_COMMAND(ID_SETTINGS_PROFILE, OnSettingsProfile)
	ON_COMMAND(ID_VIEW_GRAPHDATAVIEW, OnViewGraphdataview)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COIIIApp construction

COIIIApp::COIIIApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_DocCreate_TempInfo_framealwaysontop = FALSE;
	m_DocCreate_TempInfo_initzoomfactor = 1.0; //100.0;
	m_DocCreate_TempInfo_worldrect.SetRectEmpty();
	m_DocCreate_TempInfo_bRawData = FALSE;
	m_DocCreate_TempInfo_rectRawData.SetRectEmpty();
	m_DocCreate_TempInfo_pDoc = NULL;
	m_DocCreate_TempInfo_pPixelset = NULL;

	m_pOW2DocTemplate = NULL;
	m_pOW3DocTemplate = NULL;
	m_pLogDocTemplate = NULL;
	m_pGraphViewTemplate = NULL;
	m_pGraphDataViewTemplate = NULL; 
	//2012august09, poirier, begin
	m_pOW2AGDocTemplate = NULL;
	//2012august09, poirier, end

	m_pGotoPartitionDlg = NULL;
	m_pGotoClassificationDlg = NULL;

	m_pSettingsSheet = NULL;

	m_pExtractionSettings = NULL;
	m_pFusionSettings = NULL;
	m_pClassificationSettings = NULL;
	m_pProfileSettings = NULL;
	m_pHistoSettings = NULL;
	m_pScatterSettings = NULL;
	m_pViewDataSettings = NULL;
	m_pProjectionSettings = NULL;
	m_pTransformationSettings = NULL;
	m_pTraceSettings = NULL;
	m_pBehaviorSettings = NULL;

}

COIIIApp::~COIIIApp()
{
	if(m_pOW2Doc_ZoomedViewTemplate) delete m_pOW2Doc_ZoomedViewTemplate;
	if(m_pOW3DocTemplate) delete m_pOW3DocTemplate;
	if(m_pLogDocTemplate) delete m_pLogDocTemplate;
	if(m_pGraphViewTemplate) delete m_pGraphViewTemplate;
	if(m_pGraphDataViewTemplate) delete m_pGraphDataViewTemplate; 
	//2012august09, poirier, begin
	//if(m_pOW2AGDocTemplate) delete m_pOW2AGDocTemplate;
	//2012august09, poirier, end

   	if(m_pGotoPartitionDlg) 
   	{
   		m_pGotoPartitionDlg->DestroyWindow();
		delete m_pGotoPartitionDlg;
		m_pGotoPartitionDlg = NULL;
	}
   	if(m_pGotoClassificationDlg) 
   	{
   		m_pGotoClassificationDlg->DestroyWindow();
		delete m_pGotoClassificationDlg;
		m_pGotoClassificationDlg = NULL;
	}

   	if(m_pSettingsSheet) 
   	{
   		m_pSettingsSheet->DestroyWindow();
		delete m_pSettingsSheet;
		m_pSettingsSheet = NULL;
	}

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
/////////////////////////////////////////////////////////////////////////////
// The one and only COIIIApp object

COIIIApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.
static const CLSID BASED_CODE clsid =
{ 0x8f6d7d0, 0x5b01, 0x11cf, { 0xb0, 0x42, 0x20, 0x4c, 0x4f, 0x4f, 0x50, 0x20 } };

/////////////////////////////////////////////////////////////////////////////
// COIIIApp initialization

BOOL COIIIApp::InitInstance()
{
	/*
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	*/

	//2012july24, poirier, begin
	//initialize random seed
	srand((unsigned int)time(NULL));
	SetRegistryKey(_T("Offworld"));
	//2012july24, poirier, end


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	/*
	//Enable3dControls();
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	*/

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	m_pOW2DocTemplate = new COWMultiDocTemplate( IDR_COMMON,
		RUNTIME_CLASS(COW2Doc),
		RUNTIME_CLASS(COW2MDI),          // standard MDI child frame
		RUNTIME_CLASS(COW2View));
	AddDocTemplate(m_pOW2DocTemplate);

	m_pOWGEDocTemplate = new COWMultiDocTemplate( IDR_COMMON_GE,
		RUNTIME_CLASS(COW2Doc),
		RUNTIME_CLASS(COW2MDI),          // standard MDI child frame
		RUNTIME_CLASS(COWGEView));
	AddDocTemplate(m_pOWGEDocTemplate);

	m_pOWOLDocTemplate = new COWMultiDocTemplate( IDR_COMMON_OL,
		RUNTIME_CLASS(COW2Doc),
		RUNTIME_CLASS(COW2MDI),          // standard MDI child frame
		RUNTIME_CLASS(COWOLView));
	AddDocTemplate(m_pOWOLDocTemplate);

	//2012august09, poirier, begin
	m_pOW2AGDocTemplate = new COWMultiDocTemplate( IDR_COMMON_AG,
		RUNTIME_CLASS(COW2AGDoc),
		RUNTIME_CLASS(COW2AGMDI),          // standard MDI child frame
		RUNTIME_CLASS(COW2AGView));
	AddDocTemplate(m_pOW2AGDocTemplate);
	//2012august09, poirier, end

	m_pOW2Doc_ZoomedViewTemplate = new COWMultiDocTemplate( IDR_COMMON,
		RUNTIME_CLASS(COW2Doc),
		RUNTIME_CLASS(COW2ZoomedMDI),          // standard MDI child frame
		RUNTIME_CLASS(COW2ZoomedView));
	//AddDocTemplate(m_pOW2Doc_ZoomedViewTemplate);

	m_pOW3DocTemplate = new COWMultiDocTemplate( IDR_COMMON,
		RUNTIME_CLASS(COW2Doc),
		RUNTIME_CLASS(COW2MDI),          // standard MDI child frame
		RUNTIME_CLASS(COW3View));
	//AddDocTemplate(m_pOW3DocTemplate);  //will have to delete m_pOW3DocTemplate
	
	m_pLogDocTemplate = new COWMultiDocTemplate( IDR_COMMON,  //IDR_LOG_TYPE for specific icon
			RUNTIME_CLASS(CLogDocument),      
			RUNTIME_CLASS(CLogMDIChildWnd),     
			RUNTIME_CLASS(CLogEditView));
    //AddDocTemplate(m_pLogDocTemplate); //will have to delete m_pLogDocTemplate

	m_pGraphViewTemplate = new COWMultiDocTemplate(IDR_COMMON, //IDR_STAT_TYPE for specific icon
			RUNTIME_CLASS(COWGraphDoc),
			RUNTIME_CLASS(COWGraphMDIChildWnd),
			RUNTIME_CLASS(COWGraphView));
	//AddDocTemplate(m_pGraphViewTemplate); //will have to delete m_pGraphViewTemplate

	m_pGraphDataViewTemplate = new COWMultiDocTemplate(IDR_COMMON, //IDR_STATDATA_TYPE for specific icon
			RUNTIME_CLASS(COWGraphDoc),
			RUNTIME_CLASS(COWGraphDataMDIChildWnd),
			RUNTIME_CLASS(COWGraphDataView));
	//AddDocTemplate(m_pGraphDataViewTemplate); //will have to delete m_pGraphDataViewTemplate


	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	m_server.ConnectTemplate(clsid, m_pOW2DocTemplate, FALSE);

	// Register all OLE server factories as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleTemplateServer::RegisterAll();
		// Note: MDI applications register all server objects without regard
		//  to the /Embedding or /Automation on the command line.

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes();

	// Parse the command line to see if launched as OLE server
	if (RunEmbedded() || RunAutomated())
	{
		// Application was run with /Embedding or /Automation.  Don't show the
		//  main window in this case.
		return TRUE;
	}

	// When a server application is launched stand-alone, it is a good idea
	//  to update the system registry in case it has been damaged.
	m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
	COleObjectFactory::UpdateRegistryAll();

	// simple command line parsing
	if (m_lpCmdLine[0] == '\0')
	{
		// create a new (empty) document
		//OnFileNew();
	}
	else
	{
		// open an existing document
		OpenDocumentFile(m_lpCmdLine);
	}

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();


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

	if(m_pExtractionSettings==NULL || m_pFusionSettings==NULL || m_pClassificationSettings==NULL
		|| m_pProfileSettings==NULL || m_pHistoSettings==NULL || m_pScatterSettings==NULL
		|| m_pViewDataSettings==NULL || m_pProjectionSettings==NULL || m_pTransformationSettings==NULL
		|| m_pTraceSettings==NULL || m_pBehaviorSettings==NULL)
	{
		return FALSE;
	}


	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	//2012august23, poirier, begin
	#ifdef OIII_AUDIO
		pMainFrame->InitializeAudio();
		pMainFrame->CreateAllAudioThreads();
	#endif //OIII_AUDIO
	//2012august23, poirier, end

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
//	ON_WM_CREATE()
END_MESSAGE_MAP()

// App command to run the dialog
void COIIIApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// COIIIApp commands

BOOL COIIIApp::IsLogDocumentPresent()
{
	CLogDocument* pLogDocument = NULL;
	POSITION pos = m_pLogDocTemplate->GetFirstDocPosition(); 
	if(pos!=NULL) 
	{
		pLogDocument = (CLogDocument*) m_pLogDocTemplate->GetNextDoc(pos); 
	}	

	if(pLogDocument==NULL) return FALSE;
	  else return TRUE;
}

CLogDocument* COIIIApp::GetLogDocument()
{
	CLogDocument* pLogDocument = NULL;
	
	//1) check if result log document already opened, if yes return its pointer, 
	//   otherwise open a new document and return its pointer. 
	POSITION pos = m_pLogDocTemplate->GetFirstDocPosition(); 
	if(pos != NULL) 
	{
		pLogDocument = (CLogDocument*) m_pLogDocTemplate->GetNextDoc(pos); 
	}	
	if(pLogDocument==NULL)
	{ 
		//create a new document
		pLogDocument = (CLogDocument*) m_pLogDocTemplate->OpenDocumentFile(NULL); 
		pLogDocument->SetTitle("Log Window");
	}
	return pLogDocument;	
}

CGotoPartitionDlg* COIIIApp::GetGotoPartitionDlg()
{
	CGotoPartitionDlg* pGotoPartitionDlg = NULL;

	//check if GotoPartitionDlg already opened
	if(m_pGotoPartitionDlg!=NULL) 
	{
		pGotoPartitionDlg = m_pGotoPartitionDlg;
	}
	else
	{
		//DoModal();

		//create modeless dlg
		pGotoPartitionDlg = (CGotoPartitionDlg*) new CGotoPartitionDlg(m_pMainWnd);
		if(pGotoPartitionDlg==NULL) 
		{
			AfxMessageBox("pGotoPartitionDlg is NULL, see OIII.cpp");
			return NULL;
		}
		if(pGotoPartitionDlg->Create(IDD_GOTOPARTITION, m_pMainWnd) == FALSE)
		{
			AfxMessageBox("Problem creating GotoPartitionDlg, see OIII.cpp");
			return NULL;
		}
		m_pGotoPartitionDlg = pGotoPartitionDlg;
	}
	pGotoPartitionDlg->PostMessage(OWM_GOTODLG_UPDATEINFO);
	pGotoPartitionDlg->ShowWindow(SW_SHOW);
	return pGotoPartitionDlg;
}

CGotoPartitionDlg* COIIIApp::GetGotoClassificationDlg()
{
	CGotoPartitionDlg* pGotoClassificationDlg = NULL;

	//check if GotoClassificationDlg already opened
	if(m_pGotoClassificationDlg!=NULL) 
	{
		pGotoClassificationDlg = m_pGotoClassificationDlg;
	}
	else
	{
		//create modeless dlg
		pGotoClassificationDlg = (CGotoPartitionDlg*) new CGotoPartitionDlg(m_pMainWnd);
		if(pGotoClassificationDlg==NULL) 
		{
			AfxMessageBox("pGotoClassificationDlg is NULL, see OIII.cpp");
			return NULL;
		}
		pGotoClassificationDlg->m_bIsForClassification = TRUE;

		if(pGotoClassificationDlg->Create(IDD_GOTOPARTITION, m_pMainWnd) == FALSE)
		{
			AfxMessageBox("Problem creating GotoClassificationDlg, see OIII.cpp");
			return NULL;
		}
		pGotoClassificationDlg->SetWindowText("Goto Classification");
		m_pGotoClassificationDlg = pGotoClassificationDlg;
	}
	pGotoClassificationDlg->PostMessage(OWM_GOTODLG_UPDATEINFO);
	pGotoClassificationDlg->ShowWindow(SW_SHOW);
	return pGotoClassificationDlg;
}

BOOL COIIIApp::IsSettingsSheetGraphUpdateRequired()
{
	BOOL bRequired = FALSE;
	if(IsSettingsSheetVisible())
	{
		if(m_pSettingsSheet!=NULL) 
		{
			//1) check if profile, histogram or scatter graph tabs selected
			CPropertyPage* pPropertyPage = m_pSettingsSheet->GetActivePage();
			ASSERT(pPropertyPage!=NULL);
			if(pPropertyPage->IsKindOf(RUNTIME_CLASS(CProfilePage)))
			{
				CProfilePage* pProfilePage = (CProfilePage*) pPropertyPage;
				//poirier, sept 2001, begin
				if(pProfilePage->m_pProfileSettings->bGlobal==FALSE) bRequired = TRUE;
				  else bRequired = FALSE;
				//poirier, sept 2001, end
			}
			else if(pPropertyPage->IsKindOf(RUNTIME_CLASS(CHistoPage))) //histogram
			{
				CHistoPage* pHistoPage = (CHistoPage*) pPropertyPage;
				//poirier, sept 2001, begin
				if(pHistoPage->m_pHistoSettings->bGlobal==FALSE) bRequired = TRUE;
				  else bRequired = FALSE;
				//poirier, sept 2001, end
			}
			else if(pPropertyPage->IsKindOf(RUNTIME_CLASS(CScatterPage))) //scatter graph
			{
				CScatterPage* pScatterPage = (CScatterPage*) pPropertyPage;
				//poirier, sept 2001, begin
				if(pScatterPage->m_pScatterSettings->bGlobal==FALSE) bRequired = TRUE;
				  else bRequired = FALSE;
				//poirier, sept 2001, end	
			}
		}
		else
		{
			//settings sheet visible but pointer NULL???
			ASSERT(FALSE);
			return FALSE;
		}
	}
	//settings sheet not visible
	return bRequired;
}

CSettingsSheet* COIIIApp::GetSettingsSheet()
{
	CSettingsSheet* pSettingsSheet = NULL;

	//check if CSettingsSheet already opened
	if(m_pSettingsSheet!=NULL) 
	{
		pSettingsSheet = m_pSettingsSheet;
	}
	else
	{
		//DoModal();

		//create modeless dlg
		pSettingsSheet = (CSettingsSheet*) new CSettingsSheet("Settings", m_pMainWnd);
		if(pSettingsSheet==NULL) 
		{
			AfxMessageBox("pSettingsSheet is NULL, see OIII.cpp");
			return NULL;
		}
		if(pSettingsSheet->Create(m_pMainWnd,  WS_SYSMENU 
											| WS_POPUP 
											| WS_CAPTION 
											| DS_MODALFRAME 
											//| WS_VISIBLE
											) == FALSE)
		{
			AfxMessageBox("Problem creating pSettingsSheet, see OIII.cpp");
			return NULL;
		}
		m_pSettingsSheet = pSettingsSheet;
	}


	pSettingsSheet->PostMessage(OWM_SETTINGSSHEET_UPDATEINFO); //we must add a pointer to the class containing all info
	//pSettingsSheet->ShowWindow(SW_SHOW); //not at this stage, we will pass the data first, then display
	return pSettingsSheet;
}

BOOL COIIIApp::IsSettingsSheetVisible()
{
	BOOL bState = FALSE;
	if(m_pSettingsSheet!=NULL) 
	{
		bState = m_pSettingsSheet->IsWindowVisible();
	}

	return bState;
}


BOOL COIIIApp::OnIdle(LONG lCount) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWinApp::OnIdle(lCount);
}

void COIIIApp::OnSettingsClassification() 
{
	CSettingsSheet* pSettingsSheet = GetSettingsSheet();
	pSettingsSheet->SetActivePage( (CPropertyPage*) pSettingsSheet->m_pClassificationPage );
	pSettingsSheet->ShowWindow(SW_SHOW);
}

void COIIIApp::OnSettingsExtraction() 
{
	CSettingsSheet* pSettingsSheet = GetSettingsSheet();
	pSettingsSheet->SetActivePage( (CPropertyPage*) pSettingsSheet->m_pExtractionPage );
	pSettingsSheet->ShowWindow(SW_SHOW);
}

void COIIIApp::OnSettingsFusion() 
{
	CSettingsSheet* pSettingsSheet = GetSettingsSheet();
	pSettingsSheet->SetActivePage( (CPropertyPage*) pSettingsSheet->m_pFusionPage );
	pSettingsSheet->ShowWindow(SW_SHOW);
}

void COIIIApp::OnSettingsScatter() 
{
	CSettingsSheet* pSettingsSheet = GetSettingsSheet();
	pSettingsSheet->SetActivePage( (CPropertyPage*) pSettingsSheet->m_pScatterPage );
	pSettingsSheet->ShowWindow(SW_SHOW);
}

void COIIIApp::OnSettingsBehavior() 
{
	CSettingsSheet* pSettingsSheet = GetSettingsSheet();
	pSettingsSheet->SetActivePage( (CPropertyPage*) pSettingsSheet->m_pBehaviorPage );
	pSettingsSheet->ShowWindow(SW_SHOW);
}

void COIIIApp::OnSettingsHisto() 
{
	CSettingsSheet* pSettingsSheet = GetSettingsSheet();
	//if not active
	if(pSettingsSheet->GetActivePage()!=(CPropertyPage*) pSettingsSheet->m_pHistoPage)
	{
		//activate
		pSettingsSheet->SetActivePage( (CPropertyPage*) pSettingsSheet->m_pHistoPage );
	}
	pSettingsSheet->ShowWindow(SW_SHOW);
}

void COIIIApp::OnSettingsProjection() 
{
	CSettingsSheet* pSettingsSheet = GetSettingsSheet();
	pSettingsSheet->SetActivePage( (CPropertyPage*) pSettingsSheet->m_pProjectionPage );
	pSettingsSheet->ShowWindow(SW_SHOW);
}

void COIIIApp::OnSettingsTrace() 
{
	CSettingsSheet* pSettingsSheet = GetSettingsSheet();
	pSettingsSheet->SetActivePage( (CPropertyPage*) pSettingsSheet->m_pTracePage );
	pSettingsSheet->ShowWindow(SW_SHOW);
}

void COIIIApp::OnSettingsTransformation() 
{
	CSettingsSheet* pSettingsSheet = GetSettingsSheet();
	pSettingsSheet->SetActivePage( (CPropertyPage*) pSettingsSheet->m_pTransformationPage );
	pSettingsSheet->ShowWindow(SW_SHOW);
}

void COIIIApp::OnSettingsViewdata() 
{
	CSettingsSheet* pSettingsSheet = GetSettingsSheet();
	pSettingsSheet->SetActivePage( (CPropertyPage*) pSettingsSheet->m_pViewDataPage );
	pSettingsSheet->ShowWindow(SW_SHOW);
}

void COIIIApp::OnSettingsProfile() 
{
	CSettingsSheet* pSettingsSheet = GetSettingsSheet();
	pSettingsSheet->SetActivePage( (CPropertyPage*) pSettingsSheet->m_pProfilePage );
	pSettingsSheet->ShowWindow(SW_SHOW);
}

void COIIIApp::OnViewGraphdataview() 
{
    // 1) create a new set doc-frame-view for the graphic
    COWGraphDoc* pGraphDoc = (COWGraphDoc*) ((COIIIApp*)AfxGetApp())->m_pGraphDataViewTemplate->OpenDocumentFile(NULL); 
    pGraphDoc->SetTitle("Test Graph Document");
    pGraphDoc->SetXAxisLabel("X");
    pGraphDoc->SetYAxisLabel("Stat");
    pGraphDoc->SetGraphType(GRAPH_VBAR
							|GRAPH_PILED
							|GRAPH_REVERSE_CHANNEL_DISPLAY_ORDER
							|GRAPH_FORFIRSTCHANNEL_OUTLINE_BAR_TOP_ONLY
							);
	pGraphDoc->SetDefaultGraphBackgroundAndTextColor(RGB(0,0,0), RGB(255,255,255));
	pGraphDoc->UseDefaultGraphGrayColorTable();
    
	return;
}

void COIIIApp::OnFileOpen() 
{
	((CMainFrame*)m_pMainWnd)->OnFileOpen();
}

//similar to GetKeyState() but relies on PeekMessage() to detect new active wnd keyboard message
int COIIIApp::PeekKeyState(int nVirtKey)
{
	CWnd* pWnd = AfxGetApp()->m_pMainWnd->GetActiveWindow();
	MSG myMSG;
	if(PeekMessage(	&myMSG,         // message information
					pWnd->GetSafeHwnd(),           // handle to window
					WM_KEYFIRST,  // first message WM_KEYFIRST 
					WM_KEYLAST,  // last message WM_KEYLAST
					PM_REMOVE))      // removal options
	{
		/* 
		typedef struct tagMSG {
		HWND   hwnd; 
		UINT   message; 
		WPARAM wParam; 
		LPARAM lParam; 
		DWORD  time; 
		POINT  pt; 
		} MSG, *PMSG; 
		*/
		if(myMSG.message==WM_KEYDOWN && myMSG.wParam==VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return FALSE;	
}




int COIIIApp::ExitInstance()
{
	//2012august23, poirier, begin
	#ifdef OIII_AUDIO
	((CMainFrame*)m_pMainWnd)->UninitializeAudio();
	((CMainFrame*)m_pMainWnd)->CloseAllAudioThreads();
	#endif //OIII_AUDIO
	//2012august23, poirier, end

	return CWinApp::ExitInstance();
}
