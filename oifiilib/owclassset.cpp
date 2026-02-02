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

//2012july23, poirier, begin
/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "stdafx.h"
*/
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//2012july23, poirier, end
#include "oifiilib.h" //spi 2014
#include "OWpixelset.h" //offworld c++ pixelset library

#include "c_pointset.h" //offworld c pointset library
#include "c_segmentset.h" //offworld c segmentset library

#include "OWpointset.h" //offworld c++ pointset library (interface sitting on pointset.h)
#include "OWsegmentset.h" //offworld c++ segmentset library (interface sitting on segmentset.h)
#include "OWclassset.h" //offworld c++ classset library (interface sitting on segmentset.h)
#include "OWedgeset.h" //offworld edgeset library (for line, polyline, polygone  and shape)







//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//class COWClassset
//
//computer defined with oiii segmentation functionality 
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
COWClassset::COWClassset()
{
	DefaultInitialization();
}

COWClassset::COWClassset(POINTSET* pPointset, int initialnumberofsegment, int finalnumberofsegment)
{
	m_pPointset = pPointset;
	ASSERT(m_pPointset);
	m_pSegmentset = NewClassSet(initialnumberofsegment, finalnumberofsegment);
	ASSERT(m_pSegmentset);
	InitializeClassSet(m_pSegmentset, m_pPointset);
}

COWClassset::~COWClassset()
{
}

void COWClassset::DefaultInitialization()
{
	ASSERT(FALSE);
	return;
}

void COWClassset::BuildClassFusionTree()
{
	ASSERT(m_pSegmentset);
	::BuildClassFusionTree(m_pSegmentset);
}



//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//class CClassset
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

//TODO - major oiii architecture change, backup the project in separate directory.
//TODO - remove #include "classset.h" from the code and try to move all calls/reference
//       to classset.h data structure and classset.c code (in the cpp here).
//     - use the new CClassset class to provide c++ interface to classset.h
//	   - when compile and run stable, move out the c++ layer (CClassset and COWClassset)
//       to their own source file owclassset.h/.cpp
CClassset::CClassset()
{
}

CClassset::~CClassset()
{
}

void CClassset::DefaultInitialization()
{
}
