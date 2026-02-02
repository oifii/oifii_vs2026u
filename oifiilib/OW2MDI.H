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

// ow2mdi.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COW2MDI frame
#ifndef _OW2MDI_H
#define _OW2MDI_H


class OIFIILIB_API COW2MDI : public CMDIChildWnd
{
	DECLARE_DYNCREATE(COW2MDI)
protected:
	COW2MDI();           // protected constructor used by dynamic creation

// Attributes
public:
	CStatusBar	m_stBarImgFrm;
	CFont		m_fontStatusBar;
	int 		m_nPanes;

	int		m_itriseed;

// Operations
public:
	void CalcFrameSize( CSize* sizeDoc );
	void SetFrameSizeAccordToDocumentSize(CSize mySize);
	void ResizeStatusBar();
	void ChangeSBPanes(int nPanes);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COW2MDI)
	public:
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~COW2MDI();

	// Generated message map functions
	//{{AFX_MSG(COW2MDI)
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnUpdateKeyIndicator(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//  class COW2ZoomedView
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

class OIFIILIB_API COW2ZoomedMDI : public COW2MDI
{
public:
	DECLARE_DYNCREATE(COW2ZoomedMDI)

protected: 

	//{{AFX_VIRTUAL(COW2ZoomedMDI)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:
	COW2ZoomedMDI();
	~COW2ZoomedMDI();

protected:
	//{{AFX_MSG(COW2ZoomedMDI)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //_OW2MDI_H
