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

// OW2AGmdi.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COW2AGMDI frame
#ifndef _OW2AGMDI_H
#define _OW2AGMDI_H


//2012august23, poirier, begin
//2012august23, poirier, end



class OIFIILIB_API COW2AGMDI : public COW2MDI
{
	DECLARE_DYNCREATE(COW2AGMDI)
protected:
	COW2AGMDI();           // protected constructor used by dynamic creation

// Attributes
public:
	CStatusBar  m_wndStatusBar;
	//2012august23, poirier, begin
	//2012august23, poirier, end
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COW2AGMDI)
	public:
	//}}AFX_VIRTUAL

// Implementation
void MyUpdatePane();
void MyChangePane(int,LPCTSTR);

protected:
	virtual ~COW2AGMDI();

	// Generated message map functions
	//{{AFX_MSG(COW2AGMDI)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};
#endif //_OW2AGMDI_H
