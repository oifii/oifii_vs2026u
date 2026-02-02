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

////////////////////////////////////////////////////////
// owdoc.h : definition file for the class COWDocument
//
// SPoirier August 23rd, 1994.
////////////////////////////////////////////////////////


#ifndef _OWDOC_H
#define _OWDOC_H

   
   
//////////////////////
// Our Document Macros
//////////////////////





      
class OIFIILIB_API COWDocument : public CDocument
{
	DECLARE_DYNCREATE(COWDocument)
	// Attributes
private: 
	// Operations
	// Implementation
public:
	COWDocument();  
	COWDocument(HWND hwnd); //spi 2014, new constructor
	~COWDocument(); 
protected:
	CString GetPathNameOnly();
	CString GetFileNameOnly();
	CString GetCurrentDirectory();
    HWND m_parentHWND;  //spi 2014                                                             
protected:
	// Generated message map functions
	//{{AFX_MSG(COWDocument)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //_OWDOC_H
