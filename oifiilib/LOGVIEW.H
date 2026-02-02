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

///////////////////////////////////////////////////////////////
// logview.h : definition file for the class CLogEditView
//
// SPoirier August 23rd, 1994.
///////////////////////////////////////////////////////////////


#ifndef _LOGVIEW_H
#define _LOGVIEW_H


class OIFIILIB_API CLogEditView : public CEditView
{
	DECLARE_DYNCREATE(CLogEditView)
protected:
	CLogEditView();			// protected constructor used by dynamic creation

// Attributes
private: 
	CFont m_Font;
// Operations
public:
// Implementation
protected:
	virtual ~CLogEditView();
	virtual	void OnDraw(CDC* pDC);		// overridden to draw this view   
    virtual void OnInitialUpdate(void); 
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);   
  
protected:
	//{{AFX_MSG(CLogEditView)
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //_LOGVIEW_H
