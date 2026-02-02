/*
 * Copyright (c) 1994-2016 Stephane Poirier
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
#include "oifiilib.h"

#include "resource.h"

#include "oiii2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(COIFIIApp, COIIIApp)
	//{{AFX_MSG_MAP(COIIIApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


COIFIIApp::COIFIIApp()
{
}

COIFIIApp::~COIFIIApp()
{
}

COIFIIApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.
/* //spi 2014
static const CLSID BASED_CODE clsid =
{ 0x8f6d7d0, 0x5b01, 0x11cf, { 0xb0, 0x42, 0x20, 0x4c, 0x4f, 0x4f, 0x50, 0x20 } };
*/
static const CLSID BASED_CODE clsid =
{ 0x8f6d7d0, 0x5b01, 0x11cf, { 0xb0, 0x42, 0x20, 0x4c, 0x4f, 0x4f, 0x50, 0x21 } };


BOOL COIFIIApp::InitInstance()
{
	//spi 2014, begin
	//note: should I be using FreeLibrary() in ExitInstance()?
	HINSTANCE hRes = NULL;
	hRes = LoadLibrary(_T("oifiilib.dll"));
	if(hRes)
	   AfxSetResourceHandle(hRes);

	LoadIcon(IDI_ICON1); //does not work
	//spi 2014, end

	/*
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	return COIIIApp::InitInstance();
	*/
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

// App command to run the dialog
void COIFIIApp::OnAppAbout()
{
	/*
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
	*/
	//MessageBox(NULL, "Developed by Stephane Poirier", "OIFII", MB_OK);
	COIIIApp::OnAppAbout();
}

BOOL COIFIIApp::OnIdle(LONG lCount) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COIIIApp::OnIdle(lCount);
}

int COIFIIApp::ExitInstance()
{
	return COIIIApp::ExitInstance();
}