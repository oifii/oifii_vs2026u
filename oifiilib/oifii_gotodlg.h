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
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// gotodlg.h : header file
//
//
// CGotoPartitionDlg dialog
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifndef _OIFII_GOTODLG_H
#define _OIFII_GOTODLG_H


//#define OWM_GOTODLG_UPDATEINFO		WM_USER+1

class OIFIILIB_API CGotoPartitionDlg : public CDialog
{
// Construction
public:
	CGotoPartitionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGotoPartitionDlg)
	enum { IDD = IDD_GOTOPARTITION };
	CButton	m_StopButton;
	CButton	m_PlayButton;
	CButton	m_GotoButton;
	CButton	m_GotoNextButton;
	CEdit	m_ToStartEdit;
	CEdit	m_ToEndEdit;
	CEdit	m_ToDisplayEdit;
	CString	m_FinalNumSegment;
	CString	m_InitialNumSegment;
	CString	m_DesiredNumSegment;
	CString	m_EndingNumSegment;
	CString	m_StartingNumSegment;
	//}}AFX_DATA
public:
	int m_iMinNumSeg;
	int m_iMaxNumSeg;
	class COW2View* m_pOW2View;
	class COW2Doc* m_pOW2Doc;

	BOOL m_bIsForClassification;

public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGotoPartitionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGotoPartitionDlg)
	afx_msg void OnKillfocusTodisplayEdit();
	afx_msg void OnClose();
	afx_msg void OnGoto();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg void OnGotoNext();
 	afx_msg LRESULT OnUpdateDlgInfo(WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



#endif //_OIFII_GOTODLG_H
