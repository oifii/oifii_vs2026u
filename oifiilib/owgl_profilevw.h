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

#ifndef _PROFILEVW_H
#define _PROFILEVW_H

     
      
class OIFIILIB_API CProfileGraphView : public COWGraphView
{  
	DECLARE_DYNCREATE(CProfileGraphView)
// Attributes
private:

// Implementation
public:
   	CProfileGraphView();
	~CProfileGraphView(); 
protected:                                                                   
	virtual void OnInitialUpdate();
	 
protected:	
	//{{AFX_MSG(CProfileGraphView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif // _PROFILEVW_H
