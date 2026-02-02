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

/////////////////////////////////////////////////////////////////////////////
// mainfrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
#ifndef _OIFII_MAINFRM_H
#define _OIFII_MAINFRM_H


//2012august23, poirier, begin
//Select sample format. 
#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 1
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE  paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE  (128)
#define PRINTF_S_FORMAT "%d"
#endif
//2012august23, poirier, end


class OIFIILIB_API CZoomBar : public CToolBar
{
public:
    CComboBox   m_comboBox;
    CFont       m_font;
};


class OIFIILIB_API CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
    CComboBox   m_comboBox;
    CFont       m_font;

	//for registry access
	BOOL m_bInitCoorValid;
	int m_ix;
	int m_iy;
	int m_icx;
	int m_icy;

	//2012august23, poirier, begin
	//2012august23, poirier, end

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void MyUpdatePane();
    void MyChangePane(int,LPCTSTR);

	//2012august23, poirier, begin
	bool InitializeAudio();
	bool UninitializeAudio();
	bool SelectAudioDevice();
	void DisplayAudioError();
	BOOL CreateAllAudioThreads();
	BOOL CloseAllAudioThreads();
	//2012august23, poirier, end

public:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	//CToolBar    m_wndToolBar;
	CZoomBar   m_wndToolBar;
	int m_nUpdates;

// Generated message map functions
public:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTestPointset();
	afx_msg void OnTestSegmentset();
	afx_msg void OnTestTreefinder();
	afx_msg void OnFileOpen();
	afx_msg void OnFileAcquire();
	afx_msg void OnUpdateFileAcquire(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnViewLogwindow();
	afx_msg void OnUpdateViewLogwindow(CCmdUI* pCmdUI);
	//}}AFX_MSG
    afx_msg void OnUpdatePercent(CCmdUI *);
    afx_msg void OnUpdateLevel(CCmdUI *);
    afx_msg void OnUpdateLocation(CCmdUI *);
    afx_msg void OnUpdatePoint(CCmdUI *);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
protected:
	void RestoreFromINI();
	void StoreToINI(int x, int y, int cx, int cy);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnTransportPlay();
	afx_msg void OnUpdateTransportPlay(CCmdUI *pCmdUI);
	afx_msg void OnTransportPause();
	afx_msg void OnUpdateTransportPause(CCmdUI *pCmdUI);
	afx_msg void OnTransportStop();
	afx_msg void OnUpdateTransportStop(CCmdUI *pCmdUI);
};

#endif // _OIFII_MAINFRM_H
