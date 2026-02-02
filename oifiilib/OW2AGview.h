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

// OW2AGview.h : interface of the COW2AGView class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _OW2AGVIEW_H
#define _OW2AGVIEW_H

class OIFIILIB_API COW2AGView : public COW2View
{
protected: // create from serialization only
	COW2AGView();
	DECLARE_DYNCREATE(COW2AGView)

// Attributes
public:
protected:
    COleDropTarget	m_droptarget;
private:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COW2AGView)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COW2AGView();
protected:
	BOOL ReadHdropData(COleDataObject* pDataObject);
	int GetVoronoiPolygon(int ivertex, POINT* pPointOutputPolygon, int* p_numpointoutputpolygon);
	void GetPolygonMinMax(POINT* pPointInputPolygon, int numpointinputpolygon, long* p_xmin, long* p_xmax, long* p_ymin, long* p_ymax, int* p_idpointymin);
	void SlicePolygon(POINT* pPointInputPolygon, int numpointinputpolygon, double dfValue, POINT* pPointOutputPolygon, int* p_numpointoutputpolygon);
	void MyChangeStatusBar(int nPane,LPCTSTR text);
	void UpdateStatusBarInfo(int idpoint, double dfRatio, int iflagUpdateThirdPane=TRUE);
	// Generated message map functions
protected:
	//{{AFX_MSG(COW2AGView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDraw(CDC* /*pDC*/);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	afx_msg void OnEditClear();
};


#endif //_OW2AGVIEW_H