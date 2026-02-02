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
#ifndef _OIFII_PROPSHEET_H_
#define _OIFII_PROPSHEET_H_




/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// setsheet.h : header file
//
// CSettingsSheet
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//#define OWM_SETTINGSSHEET_UPDATEINFO		WM_USER+2


class OIFIILIB_API CSettingsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSettingsSheet)

// Construction
public:
	CSettingsSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSettingsSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	class CExtractionPage*		m_pExtractionPage;
	class CFusionPage*			m_pFusionPage;
	class CClassificationPage*	m_pClassificationPage;
	class CProfilePage*			m_pProfilePage;
	class CHistoPage*		m_pHistoPage;
	class CScatterPage*		m_pScatterPage;
	class CViewDataPage*		m_pViewDataPage;
	class CProjectionPage*		m_pProjectionPage;
	class CTransformationPage*	m_pTransformationPage;
	class CTracePage*			m_pTracePage;
	class CBehaviorPage*		m_pBehaviorPage;

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

	BOOL m_bIsOW2View;
	BOOL m_bIsOW3View;
	BOOL m_bIsAppDefault;

	class COWView* GetAttachedView();
	class COW2Doc* GetAttachedDoc();
	BOOL SetAttachedView(class COWView* pOWView);
	BOOL SetAttachedDoc(class COW2Doc* pOW2Doc);

	class COW2View* m_pOW2View;
	class COW3View* m_pOW3View;
	class COW2Doc* m_pOW2Doc;

protected:
	void AddControlPages(void);
	HICON m_hIcon;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingsSheet)
	public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSettingsSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSettingsSheet)
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
 	afx_msg LRESULT OnUpdateSettingsSheetInfo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnChildActivate();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
 	afx_msg LRESULT OnMouseMovedUpdateLocalGraph(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif _OIFII_PROPSHEET_H_

