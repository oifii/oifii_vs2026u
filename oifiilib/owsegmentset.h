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

//////////////////////////////////////////////////////////////////////////////////////
//owsegmentset.h
//
//defines the class COWSegmentset which provides the c++ interface to the encapsulated
//and previously c segment set library (old interface was through the c structure SEGSET
//defined in the segset.h
//
//the COWSegmentset class embeds the c structure SEGSET data member and segset.h 
//functionality. this new c++ interface also embeds other COWEdgeset derived classes.  
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _OWSEGMENTSET_H
#define _OWSEGMENTSET_H


//////////////////////////////////////////////////////////////////////////////////////
//class COWSegmentset
//
//////////////////////////////////////////////////////////////////////////////////////
class OIFIILIB_API COWSegmentset : public COWSet
{
protected:
	POINTSET* m_pPointset;
	SEGMENTSET* m_pSegmentset; 

public:
	COWSegmentset();
	~COWSegmentset();
	void DefaultInitialization();

	POINTSET* GetPointsetPointer();
	SEGMENTSET* GetSegmentsetPointer();
};


//////////////////////////////////
//class CSegmentset
//////////////////////////////////

class OIFIILIB_API CSegmentset : public CObject
{
protected:

public:
	CSegmentset();
	~CSegmentset();
	void DefaultInitialization();
};



#endif //_OWSEGMENTSET_H