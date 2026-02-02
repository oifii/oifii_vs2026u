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

// OWHtmlView.cpp : implementation file
//

#include "stdafx.h"
#include "oifiilib.h" //spi 2014
#include "oifii_app.h"
#include "OWHtmlView.h"


// COWHtmlView

IMPLEMENT_DYNCREATE(COWHtmlView, CHtmlView)

COWHtmlView::COWHtmlView()
{

}

COWHtmlView::~COWHtmlView()
{
}

// COWHtmlView message handlers

void COWHtmlView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COWHtmlView, CHtmlView)
END_MESSAGE_MAP()


// COWHtmlView diagnostics

#ifdef _DEBUG
void COWHtmlView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void COWHtmlView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG






//////////////////////////////////////////////////////////////////////////////////////
//class COWGEView
//
//view derived from COWHtmlView to be used when opening html page embeding a google earth plugin 
//////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(COWGEView, COWHtmlView)

COWGEView::COWGEView()
{
	DefaultInitialization();
}

COWGEView::~COWGEView()
{
}

void COWGEView::DefaultInitialization()
{
	return;
}

void COWGEView::DoDataExchange(CDataExchange* pDX)
{
	COWHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COWGEView, COWHtmlView)
END_MESSAGE_MAP()


// COWGEView diagnostics

#ifdef _DEBUG
void COWGEView::AssertValid() const
{
	COWHtmlView::AssertValid();
}

void COWGEView::Dump(CDumpContext& dc) const
{
	COWHtmlView::Dump(dc);
}
#endif //_DEBUG


void COWGEView::OnInitialUpdate(void)
{
	//LoadFromResource(_T(IDR_HTML1));
    LoadFromResource(_T("owge_index.html"));    //that's all
}





//////////////////////////////////////////////////////////////////////////////////////
//class COWOLView
//
//view derived from COWHtmlView to be used when opening html page embeding an openlayer (pure javascript) library 
//////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(COWOLView, COWHtmlView)

COWOLView::COWOLView()
{
	DefaultInitialization();
}

COWOLView::~COWOLView()
{
}

void COWOLView::DefaultInitialization()
{
	return;
}

void COWOLView::DoDataExchange(CDataExchange* pDX)
{
	COWHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COWOLView, COWHtmlView)
END_MESSAGE_MAP()


// COWOLView diagnostics

#ifdef _DEBUG
void COWOLView::AssertValid() const
{
	COWHtmlView::AssertValid();
}

void COWOLView::Dump(CDumpContext& dc) const
{
	COWHtmlView::Dump(dc);
}
#endif //_DEBUG


void COWOLView::OnInitialUpdate(void)
{
    //LoadFromResource(_T("owol_index.html"));    //that's all
	Navigate(L"http://www.oifii.org/imt/dai/src/2009nov23/index.html");
}
