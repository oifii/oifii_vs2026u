// OIII.h : main header file for the OIII application
//
#ifndef _OIII_H
#define _OIII_H

#ifndef __AFXWIN_H__
	//#error include 'stdafx.h' before including this file for PCH //spi 2014
#endif

#include "resource.h"       // main symbols

//2012august16, poirier, begin
#define OIII_USE_FREEIMAGE		1
#define OIII_USE_OWIFFACCUSOFT	0
//2012august16, poirier, end

const UINT  SB_PANE_ZERO = 0;
const UINT  SB_PANE_ONE  = 1;
const UINT  SB_PANE_TWO = 2;
const UINT  SB_PANE_THREE = 3;
const UINT  SB_PANE_FOUR = 4;


#define OWM_GOTODLG_UPDATEINFO				WM_USER+1
#define OWM_SETTINGSSHEET_UPDATEINFO		WM_USER+2




typedef struct OIFIILIB_API tagCONTEXTINFO //structure used when interpreting OWM_MOUSEMOVED_UPDATEPROFILE
{
	int idROI;
	int idVertex;
	int idSegment;
} CONTEXTINFO;
#ifndef COMPILING_OIII_CPP
	extern struct tagCONTEXTINFO globalstructCONTEXTINFO;
#endif

#define OWM_MOUSEMOVED_UPDATELOCALGRAPH		WM_USER+10

//2012august23, poirier, begin
#define OWM_PLAYING_EXECUTE					WM_USER+21
#define OWM_PLAYING_EXIT					WM_USER+22
#define OWM_SWAPPING_EXECUTE				WM_USER+23
#define OWM_SWAPPING_EXIT					WM_USER+24
#define OWM_SUMMING_EXECUTE					WM_USER+25
#define OWM_SUMMING_EXIT					WM_USER+26
//2012august23, poirier, end



/////////////////////////////////////////////////////////////////////////////
// COIIIApp:
// See OIII.cpp for the implementation of this class
//


class OIFIILIB_API COIIIApp : public CWinApp
{
public:
	COIIIApp();
	~COIIIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COIIIApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL
	
	int PeekKeyState(int nVirtKey);

	//temporary, used on new document creation
	BOOL m_DocCreate_TempInfo_framealwaysontop;
	double m_DocCreate_TempInfo_initzoomfactor;
	CRect m_DocCreate_TempInfo_worldrect;
	BOOL m_DocCreate_TempInfo_bRawData;
	CRect m_DocCreate_TempInfo_rectRawData;
	class COW2Doc* m_DocCreate_TempInfo_pDoc;
	class COWPixelset* m_DocCreate_TempInfo_pPixelset;

	COleTemplateServer m_server;
	CMultiDocTemplate* m_pOW2DocTemplate;
	CMultiDocTemplate* m_pOWGEDocTemplate;
	CMultiDocTemplate* m_pOWOLDocTemplate;
	CMultiDocTemplate* m_pOW2Doc_ZoomedViewTemplate;
	CMultiDocTemplate* m_pOW3DocTemplate;
	CMultiDocTemplate* m_pGraphViewTemplate;	// Graph View Template (graphic)
	CMultiDocTemplate* m_pGraphDataViewTemplate;// Graph Data View Template (non-editable tabbed text)
//2012august09, poirier, begin
	CMultiDocTemplate* m_pOW2AGDocTemplate;
//2012august09, poirier, end

		// Server object for document creation

	BOOL IsLogDocumentPresent();
	CMultiDocTemplate* m_pLogDocTemplate;
	class CLogDocument* GetLogDocument();

	class CGotoPartitionDlg* m_pGotoPartitionDlg;
	class CGotoPartitionDlg* GetGotoPartitionDlg();
	class CGotoPartitionDlg* m_pGotoClassificationDlg;
	class CGotoPartitionDlg* GetGotoClassificationDlg();

	class CSettingsSheet* m_pSettingsSheet;
	class CSettingsSheet* GetSettingsSheet();
	BOOL IsSettingsSheetVisible();
	BOOL IsSettingsSheetGraphUpdateRequired();

	class CExtractionSettings*		m_pExtractionSettings;
	class CFusionSettings*			m_pFusionSettings;
	class CClassificationSettings*	m_pClassificationSettings;
	class CProfileSettings*			m_pProfileSettings;
	class CHistoSettings*		m_pHistoSettings;
	class CScatterSettings*		m_pScatterSettings;
	class CViewDataSettings*		m_pViewDataSettings;
	class CProjectionSettings*		m_pProjectionSettings;
	class CTransformationSettings*	m_pTransformationSettings;
	class CTraceSettings*			m_pTraceSettings;
	class CBehaviorSettings*		m_pBehaviorSettings;


	//{{AFX_MSG(COIIIApp)
	afx_msg void OnAppAbout();
	afx_msg void OnSettingsClassification();
	afx_msg void OnSettingsExtraction();
	afx_msg void OnSettingsFusion();
	afx_msg void OnSettingsScatter();
	afx_msg void OnSettingsBehavior();
	afx_msg void OnSettingsHisto();
	afx_msg void OnSettingsProjection();
	afx_msg void OnSettingsTrace();
	afx_msg void OnSettingsTransformation();
	afx_msg void OnSettingsViewdata();
	afx_msg void OnSettingsProfile();
	afx_msg void OnViewGraphdataview();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

#endif// _OIII_H

/////////////////////////////////////////////////////////////////////////////
