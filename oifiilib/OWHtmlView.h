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
#ifndef _OWHTMLVIEW_H
#define _OWHTMLVIEW_H

#pragma once

#ifdef _WIN32_WCE
#error "CHtmlView is not supported for Windows CE."
#endif 

//////////////////////////////////////////////////////////////////////////////////////
//class COWHtmlView
//
//view that embeds an internet explorer window enabling to open remote and local html pages 
//////////////////////////////////////////////////////////////////////////////////////
class OIFIILIB_API COWHtmlView : public CHtmlView
{
	DECLARE_DYNCREATE(COWHtmlView)

protected:
	COWHtmlView();           // protected constructor used by dynamic creation
	virtual ~COWHtmlView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
};

//////////////////////////////////////////////////////////////////////////////////////
//class COWGEView
//
//view derived from COWHtmlView to be used when opening html page embeding a google earth plugin 
//////////////////////////////////////////////////////////////////////////////////////
class OIFIILIB_API COWGEView  : public COWHtmlView
{
	DECLARE_DYNCREATE(COWGEView)

protected:

public:
	COWGEView();
	~COWGEView();
	void DefaultInitialization();
	void OnInitialUpdate(void);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};



//////////////////////////////////////////////////////////////////////////////////////
//class COWOLView
//
//view derived from COWHtmlView to be used when opening html page embeding an openlayer (pure javascript) library 
//////////////////////////////////////////////////////////////////////////////////////
class OIFIILIB_API COWOLView  : public COWHtmlView
{
	DECLARE_DYNCREATE(COWOLView)

protected:

public:
	COWOLView();
	~COWOLView();
	void DefaultInitialization();
	void OnInitialUpdate(void);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

#endif// _OWHTMLVIEW_H
