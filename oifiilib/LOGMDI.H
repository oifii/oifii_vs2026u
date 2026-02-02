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

///////////////////////////////////////////////////////
// logmdi.h : definition for the class CLogMDIChildWnd
//
// Stephane Poirier, jan 1996
///////////////////////////////////////////////////////

#ifndef _LOGMDI_H
#define _LOGMDI_H


class OIFIILIB_API CLogMDIChildWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CLogMDIChildWnd)
protected:
	CLogMDIChildWnd();			// protected constructor used by dynamic creation

// Attributes
public:
protected:
// Operations
public:
	BOOL PreCreateWindow(CREATESTRUCT& cs);	

// Implementation
protected:
	virtual ~CLogMDIChildWnd();
	// Generated message map functions
	//{{AFX_MSG(CLogMDIChildWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct); 
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
    
    
    
#endif //_LOGMDI_H
