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

#include "stdafx.h"
#include "oifiilib.h" //spi 2014

//#include "ow2mdi.h"
#include "gl_owgraph.h"
#include "owgl_scattervw.h"
                   
                   
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CScatterGraphView, COWGraphView)
           
BEGIN_MESSAGE_MAP(CScatterGraphView, COWGraphView)
	//{{AFX_MSG_MAP(CScatterGraphView)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//constructor           
CScatterGraphView::CScatterGraphView()
{
	return;
}

//destructor
CScatterGraphView::~CScatterGraphView()
{
	return;
}
        
void CScatterGraphView::OnInitialUpdate()
{    
	//do not call COWGraphView::OnInitialUpdate, otherwise graph size would be set according to frame size        
	return;    
}

void CScatterGraphView::OnMouseMove(UINT /*nFlags*/, CPoint /*point*/)
{
	return;     
} 