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

// ow3view.h : interface of the COW3View class
//
#ifndef _OW3VIEW_H
#define _OW3VIEW_H

class OIFIILIB_API COW3View : public COWView
{
//protected: // create from serialization only
public: 
	COW3View();
	DECLARE_DYNCREATE(COW3View)

// Attributes
public:
	COW2Doc* GetDocument();

    CPalette    m_cPal;
    CPalette    *m_pOldPal;
	CRect		m_oldRect;
	double		m_fRadius;
	void CreateRGBPalette(HDC hDC);
    BOOL bSetupPixelFormat(HDC hDC);
    unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift);
    void DrawScene(void);

	BOOL DrawPixelset();
	BOOL DrawPointset(int idstatchannelDimX, int idstatchannelDimY);
	BOOL DrawTriangleset(int idstatchannelMappedStat, double fRangeForMappedStat);
	BOOL DrawVoronoiset();
	BOOL DrawSegmentset(int iNumberOfSegmentForFinalPartition);


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COW3View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COW3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL	m_play;

	/*
	GLfloat m_xTranslate;
	GLfloat m_yTranslate;
	GLfloat m_zTranslate;

	GLfloat m_xScale;
	GLfloat m_yScale;
	GLfloat m_zScale;

	GLfloat m_xRotate;
	GLfloat m_yRotate;
	GLfloat m_zRotate;
	*/

	int m_ix_init;
	int m_iy_init;
	int m_ix_translation;
	int m_iy_translation;

public:	


// Generated message map functions
protected:
	//{{AFX_MSG(COW3View)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ow3view.cpp
inline COW2Doc* COW3View::GetDocument()
   { return (COW2Doc*)m_pDocument; }
#endif

#endif //_OW3VIEW_H