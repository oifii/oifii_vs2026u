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
#ifndef _OIII2_H
#define _OIII2_H

#ifndef __AFXWIN_H__
	//#error include 'stdafx.h' before including this file for PCH //spi 2014
#endif

//2020june20, spi, begin
//#define OIII_USE_OPENIMAGEIO	1
#define OIII_USE_OPENIMAGEIO	0
#define OIII_USE_OPENCV			0
//#define OIII_USE_FREEIMAGE	1
#define OIII_USE_FREEIMAGE		1
#define OIII_USE_OWIFFACCUSOFT	0
//2020june20, spi, end

class COIFIIApp : public COIIIApp
{
public:
	COIFIIApp();
	~COIFIIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COIFIIApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL
	

	//{{AFX_MSG(COIFIIApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};


#endif //_OIII2_H