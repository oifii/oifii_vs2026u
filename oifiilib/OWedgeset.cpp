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

#include "ow_iff.h"


#include "c_pointset.h" //offworld c pointset library
#include "c_segmentset.h" //offworld c segmentset library

#include "OWpointset.h" //offworld c++ pointset library
#include "OWsegmentset.h" //offworld c++ segmentset library
#include "OWclassset.h" //offworld c++ classset library
#include "OWedgeset.h" //offworld edgeset library (for line, polyline, polygone  and shape)

//#include "oifii_app.h"
//#include "logdoc.h"

//poirier, sept 2001, for drawing, begin
#include "owview.h"
#include "ow2view.h"
//poirier, sept 2001, for drawing, end

//poirier, sept 2001, for debugging, begin
#include "oifii_app.h"
#include "logdoc.h"
//poirier, sept 2001, for debugging, end

//poirier, oct 2001, for flood fill algorithms, begin
#include "math.h"
//poirier, oct 2001, for flood fill algorithms, end

//spi, nov 2018, begin
#include "freeimageplus.h"
#include "ow2doc.h"
#include "utility.h"
//spi, nov 2018, end


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//sequence (items physical array ids should match with the #define EDGESET_SHAPETYPEs
const char* pSHAPEtypelist[] = 
{
	"Rectangular",
	"Square",
	"Linear",		//1 dimension
	"Elliptical",
	"Circular",
	"Ponctual",		//0 dimension
	"Trapezoidale", 
	"Triangular",
	"Polygonal",
	"Parallebipeidale", //3 dimensions
	"Cubic",			//3 dimensions
	"Ellipsoidale",		//3 dimensions
	"Spherical",		//3 dimensions
	"Conical",			//3 dimensions
	"Pyramidale",		//3 dimensions
	"Unknown Volume"	//3 dimensions
};



////////////////////////////////
//class COWGeoPrimitive
////////////////////////////////
COWGeoPrimitive::COWGeoPrimitive()
{
	DefaultInitialization();
}

COWGeoPrimitive::~COWGeoPrimitive()
{
}

void COWGeoPrimitive::DefaultInitialization()
{
	return;
}


////////////////////////////////
//class COWEdgeset
////////////////////////////////
COWEdgeset::COWEdgeset()
{
	DefaultInitialization();
}

COWEdgeset::COWEdgeset(BOOL bIntegerMode)
{
	DefaultInitialization();

	m_bINTEGERPOINT = bIntegerMode;
	m_bFLOATPOINT = !bIntegerMode;
	m_pPointset = NULL;
	m_idset_pointset = -1;
}

COWEdgeset::COWEdgeset(COWPointset* pPointset, BOOL bDual)
{
	DefaultInitialization();
	ASSERT(pPointset!=NULL);
	if(pPointset) 
	{
		m_pPointset = pPointset->m_pPOINTSET;
		m_idset_pointset = pPointset->m_idset;
		m_bDual = FALSE;
	}
	m_bINTEGERPOINT = FALSE;
	m_bFLOATPOINT = FALSE;

	//ASSERT that TIN has been created 
	/* should have been performed within the pointset
	BuildTriangleNetwork(POINTSET* pPointset);
	*/
	if(bDual==TRUE)
	{
		//ASSERT that TIN triangle centers have been computed
		/* should have been performed within the pointset
		ComputeAllTriangleCenters(POINTSET* pPointset);
		*/
		//will refer to this pointset's dual representation set of points (voronoi edges)
		//POINTSET's ctx[0...n], and cty[0...n] instead of px[0...n] and py[0...n]
		m_bDual = TRUE;
	}


}

void COWEdgeset::DefaultInitialization()
{
	//default mode is internal list of floating point 
	m_bINTEGERPOINT = FALSE;
	m_bFLOATPOINT = TRUE;
	m_pPointset = NULL;
	m_idset_pointset = -1;
	m_bDual = FALSE;
	return;
}


COWEdgeset::~COWEdgeset()
{
	DeleteAndRemoveAll();
}

void COWEdgeset::ClearAll()
{
	DeleteAndRemoveAll();
}

void COWEdgeset::DeleteAndRemoveAll()
{
	//////////////////
	//INTEGER POINTS//
	//////////////////
	//delete all elements in m_ListINTEGERPOINT
	POSITION pos = m_ListINTEGERPOINT.GetHeadPosition();
	while(pos)
	{
		delete m_ListINTEGERPOINT.GetNext(pos);
	}
	//empty m_ListINTEGERPOINT
	m_ListINTEGERPOINT.RemoveAll();

	////////////////
	//FLOAT POINTS//
	////////////////
	//delete all elements in m_ListFLOATPOINT
	pos = m_ListFLOATPOINT.GetHeadPosition();
	while(pos)
	{
		delete m_ListFLOATPOINT.GetNext(pos);
	}
	//empty m_ListINTEGERPOINT
	m_ListFLOATPOINT.RemoveAll();

	/////////
	//LINES//
	/////////
	//delete all elements in m_ListLINE
	pos = m_ListLINE.GetHeadPosition();
	while(pos)
	{
		delete m_ListLINE.GetNext(pos);
	}
	//empty m_ListLINE
	m_ListLINE.RemoveAll();

	/////////////
	//POLYLINES//
	/////////////
	//delete all elements in m_ListPOLYLINE
	pos = m_ListPOLYLINE.GetHeadPosition();
	while(pos)
	{
		delete m_ListPOLYLINE.GetNext(pos);
	}
	//empty m_ListLINE
	m_ListPOLYLINE.RemoveAll();
}

BOOL COWEdgeset::ReadAllValuesFrom(FILE* pFILE)
{
	COWSet::ReadAllValuesFrom(pFILE);
	
	//empty default edgeset
	DeleteAndRemoveAll();

	fread(&m_bINTEGERPOINT, sizeof(BOOL), 1, pFILE);
	fread(&m_bFLOATPOINT, sizeof(BOOL), 1, pFILE);
	if(m_bINTEGERPOINT)
	{
		//read internal integer pointset
		int npoints;
		if(fread(&npoints, sizeof(int), 1, pFILE)==1 && npoints>0)
		{
			for(int i=0; i<npoints; i++)
			{
				INTEGERPOINT myINTEGERPOINT;
				fread(&myINTEGERPOINT, sizeof(INTEGERPOINT), 1, pFILE);
				AddPoint(&myINTEGERPOINT);
			}
		}
	}
	else if(m_bFLOATPOINT)
	{
		//read internal floating pointset
		int npoints;
		if(fread(&npoints, sizeof(int), 1, pFILE)==1 && npoints>0)
		{
			for(int i=0; i<npoints; i++)
			{
				FLOATPOINT myFLOATPOINT;
				fread(&myFLOATPOINT, sizeof(FLOATPOINT), 1, pFILE);
				AddPoint(&myFLOATPOINT);
			}
		}
	}
	else 
	{
		//external pointset
		int npoints;
		if(fread(&npoints, sizeof(int), 1, pFILE)==1 && npoints>0)
		{
			fread(&m_idset_pointset, sizeof(int), 1, pFILE);
			fread(&m_bDual, sizeof(BOOL), 1, pFILE);
		}
		m_pPointset = NULL;
		//(after reading all recorded sets) attach pointset
		//or better, on first attempt to access m_pPointset
		//when m_pPointset==NULL, attach pointer then, use m_idset_pointset
		//to retreive from pDoc COWSet list
		ASSERT(FALSE);
	}

	//read lines
	int nlines;
	if(fread(&nlines, sizeof(int), 1, pFILE)==1 && nlines>0)
	{
		for(int i=0; i<nlines; i++)
		{
			LINE myLINE;
			fread(&myLINE, sizeof(LINE), 1, pFILE);
			AddLine(&myLINE);
		}
	}

	//read polylines
	int npolylines;
	if(fread(&npolylines, sizeof(int), 1, pFILE)==1 && npolylines>0)
	{
		for(int i=0; i<npolylines; i++)
		{
			POLYLINE myPOLYLINE;
			fread(&myPOLYLINE, sizeof(POLYLINE), 1, pFILE);
			AddPolyline(&myPOLYLINE);
		}
	}
	return TRUE;
}

BOOL COWEdgeset::WriteAllValuesTo(FILE* pFILE)
{
	COWSet::WriteAllValuesTo(pFILE);

	fwrite(&m_bINTEGERPOINT, sizeof(BOOL), 1, pFILE);
	fwrite(&m_bFLOATPOINT, sizeof(BOOL), 1, pFILE);
	if(m_bINTEGERPOINT)
	{
		//write internal integer pointset
		int npoints = m_ListINTEGERPOINT.GetCount();
		if(fwrite(&npoints, sizeof(int), 1, pFILE)==1 && npoints>0)
		{
			POSITION pos = m_ListINTEGERPOINT.GetHeadPosition();
			while(pos)
			{
				INTEGERPOINT* pINTEGERPOINT = (INTEGERPOINT*)m_ListINTEGERPOINT.GetNext(pos);
				fwrite(pINTEGERPOINT, sizeof(INTEGERPOINT), 1, pFILE);
			}
		}
	}
	else if(m_bFLOATPOINT)
	{
		//read internal floating pointset
		int npoints = m_ListFLOATPOINT.GetCount();
		if(fwrite(&npoints, sizeof(int), 1, pFILE)==1 && npoints>0)
		{
			POSITION pos = m_ListFLOATPOINT.GetHeadPosition();
			while(pos)
			{
				FLOATPOINT* pFLOATPOINT = (FLOATPOINT*)m_ListFLOATPOINT.GetNext(pos);
				fwrite(pFLOATPOINT, sizeof(FLOATPOINT), 1, pFILE);
			}
		}
	}
	else 
	{
		//external pointset
		int npoints = m_pPointset->npts;
		if(fwrite(&npoints, sizeof(int), 1, pFILE)==1 && npoints>0)
		{
			fwrite(&m_idset_pointset, sizeof(int), 1, pFILE);
			fwrite(&m_bDual, sizeof(BOOL), 1, pFILE);
		}
		m_pPointset = NULL;
		//(after reading all recorded sets) attach pointset
		//or better, on first attempt to access m_pPointset
		//when m_pPointset==NULL, attach pointer then, use m_idset_pointset
		//to retreive from pDoc COWSet list
		ASSERT(FALSE);
	}

	//write lines
	int nlines = m_ListLINE.GetCount();
	if(fwrite(&nlines, sizeof(int), 1, pFILE)==1 && nlines>0)
	{
		POSITION pos = m_ListLINE.GetHeadPosition();
		while(pos)
		{
			LINE* pLINE = (LINE*)m_ListLINE.GetNext(pos);
			fwrite(pLINE, sizeof(LINE), 1, pFILE);
		}
	}

	//write polylines
	int npolylines = m_ListPOLYLINE.GetCount();
	if(fwrite(&npolylines, sizeof(int), 1, pFILE)==1 && npolylines>0)
	{
		POSITION pos = m_ListPOLYLINE.GetHeadPosition();
		while(pos)
		{
			POLYLINE* pPOLYLINE = (POLYLINE*)m_ListPOLYLINE.GetNext(pos);
			fwrite(pPOLYLINE, sizeof(POLYLINE), 1, pFILE);
		}
	}
	return TRUE;
}

//to add 1 integer point, returns point id
int COWEdgeset::AddPoint(	int iX,	//integer X
							int iY,	//integer Y
							int iDX, //integer Delta X 
							int iDY,//integer Delta Y 
							int id_info) //default = -1 for not used
{
	INTEGERPOINT* pINTEGERPOINT = new INTEGERPOINT;
	if(pINTEGERPOINT==NULL)
	{
		ASSERT(FALSE); //out of memory?
		return -1;
	}
	int idpoint = m_ListINTEGERPOINT.GetCount();

	pINTEGERPOINT->iX = iX;	
	pINTEGERPOINT->iY = iY;	
	pINTEGERPOINT->iDX = iDX;
	pINTEGERPOINT->iDY = iDY;
	pINTEGERPOINT->id_info = id_info;

	m_ListINTEGERPOINT.AddTail(pINTEGERPOINT);

	#ifdef _DEBUG
		//CLogDocument* pLog = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	if(AfxGetApp()!=NULL)
	{
		CLogDocument* pLog = ((COIIIApp*)AfxGetApp())->GetLogDocument();
		if(pLog!=NULL)
		{
			CString myString;
			myString.Format(L"idnew_point: %d (%d, %d)\r\n", idpoint, pINTEGERPOINT->iX, pINTEGERPOINT->iY);
			pLog->AddTextW(myString);
		}
	}
	#endif //_DEBUG

	return idpoint;
}

int COWEdgeset::AddPoint(INTEGERPOINT* pINTEGERPOINT) 
{
	return AddPoint(pINTEGERPOINT->iX,	//integer X
					pINTEGERPOINT->iY,	//integer Y
					pINTEGERPOINT->iDX, //integer Delta X 
					pINTEGERPOINT->iDY,//integer Delta Y 
					pINTEGERPOINT->id_info);
}

//to add 1 floating point, returns point id
int COWEdgeset::AddPoint(	double fX,	//integer X
							double fY,	//integer Y
							double fDX, //integer Delta X 
							double fDY,//integer Delta Y 
							int id_info) //default = -1 for not used	
{

	FLOATPOINT* pFLOATPOINT = new FLOATPOINT;
	if(pFLOATPOINT==NULL)
	{
		ASSERT(FALSE); //out of memory?
		return -1;
	}
	int idpoint = m_ListFLOATPOINT.GetCount();;

	pFLOATPOINT->fX = fX;	
	pFLOATPOINT->fY = fY;	
	pFLOATPOINT->fDX = fDX;
	pFLOATPOINT->fDY = fDY;
	pFLOATPOINT->id_info = id_info;

	#ifdef _DEBUG
	if(AfxGetApp()!=NULL)
	{
		CLogDocument* pLog = ((COIIIApp*)AfxGetApp())->GetLogDocument();
		if(pLog!=NULL)
		{
			CString myString;
			myString.Format(L"idnew_point: %d (%f, %f)\r\n", idpoint, pFLOATPOINT->fX, pFLOATPOINT->fY);
			pLog->AddTextW(myString);
		}
	}
	#endif //_DEBUG

	m_ListINTEGERPOINT.AddTail(pFLOATPOINT);
	return idpoint;
}

int COWEdgeset::AddPoint(FLOATPOINT* pFLOATPOINT) 
{
	return AddPoint(pFLOATPOINT->fX,	
					pFLOATPOINT->fY,
					pFLOATPOINT->fDX, 
					pFLOATPOINT->fDY,
					pFLOATPOINT->id_info);
}

//to add 1 line segment, returns line segment id 
int COWEdgeset::AddLine(int idpointP1, 
						int idpointP2, 
						int idlineNL, //default -1
						double fLineWidth, //default = -1.0,
						double fLineWeight, //default = 1.0,	
						int id_info) //default = -1 for not used
{
	LINE* pLINE = new LINE;
	if(pLINE==NULL)
	{
		ASSERT(FALSE); //out of memory?
		return -1;
	}
	int idline = m_ListLINE.GetCount();

	pLINE->idpointP1 = idpointP1;
	pLINE->idpointP2 = idpointP2;
	pLINE->idlineNL = idlineNL;
	pLINE->fLineWidth = fLineWidth;	
	pLINE->fLineWeight = fLineWeight;	
	pLINE->id_info = id_info;	

	m_ListLINE.AddTail(pLINE);
	return idline;
}

int COWEdgeset::AddLine(LINE* pLINE) 
{
	return AddLine(	pLINE->idpointP1, 
					pLINE->idpointP2, 
					pLINE->idlineNL, 
					pLINE->fLineWidth, 
					pLINE->fLineWeight, 	
					pLINE->id_info);
}

//to add 1 chain of line segments, specifying a list of points
//returns polyline id
int COWEdgeset::AddPolyline(int* idpointP, 
							int nP, 
							int idpointPN, //default = -1 for last point not connected
							RECT* pBoundingRect, //default = NULL,
							int idshape, //default = EDGESET_SHAPETYPE_LINE,
							int id_info) //default = -1 for not used
{
	//add line segments
	if(nP<3)
	{
		if(nP<2)
		{
			//single point
			ASSERT(FALSE);
			return -1; 
		}
		//single line segment
		/* //let it go through
		ASSERT(FALSE);
		return -1;
		*/
	}
	int numberoflinesegment = nP-1; 
	if(idpointPN==idpointP[0])
	{
		//closed polyline
		numberoflinesegment++;
	}
	int* pidline = new int[numberoflinesegment]; //number of idline
	//add lines
	for(int i=0; i<numberoflinesegment; i++)
	{
		if(i==numberoflinesegment-1 && idpointPN==idpointP[0])
		{
			//for last line segment in the case of a closed polyline
			pidline[i] = AddLine(idpointP[i], idpointP[0]);

		}
		else
		{
			pidline[i] = AddLine(idpointP[i], idpointP[i+1]);		
		}
	}
	for(int i=0; i<(numberoflinesegment-1); i++)
	{
		//attach lines
		POSITION pos = m_ListLINE.FindIndex(pidline[i]);
		((LINE*)m_ListLINE.GetAt(pos))->idlineNL = pidline[i+1];	
	}
	POSITION pos = m_ListLINE.FindIndex(pidline[numberoflinesegment-1]);
	if(((LINE*)m_ListLINE.GetAt(pos))->idlineNL != -1)
	{
		//end of polyline, the last line must not point to a next one
		ASSERT(FALSE);
		//POSITION pos = m_ListLINE.FindIndex(pidline[i]);
		POSITION pos = m_ListLINE.FindIndex(pidline[numberoflinesegment-1]);
		((LINE*)m_ListLINE.GetAt(pos))->idlineNL = -1;
	}
	//last point connected to first point or last point (polygone or polyline)
	if(idpointP[0]==idpointP[nP-1]) 
	{
		if(idpointPN==-1)
		{
			idpointPN = idpointP[0];
		}
		ASSERT(idpointPN==idpointP[0] || idpointPN==idpointP[nP-1]); 
	}
	int idlineL0 = pidline[0];
	int	idlineLN = pidline[numberoflinesegment-1];

	int idpolyline = AddPolyline(idlineL0, 
								idlineLN, 
								idpointPN, 
								pBoundingRect, 
								idshape, 
								id_info); 
	delete[] pidline;
	return idpolyline;
}

//to add one polyline  (when point and line segments already exist), return polyline id
int COWEdgeset::AddPolyline(int idlineL0, 
							int idlineLN, 
							int idpointPN, //default = -1 for last point not connected
							RECT* pBoundingRect, //default = NULL
							int idshape, //default EDGESET_SHAPETYPE_LINE
							int id_info) //default -1 for not used
{
	//add polyline
	POLYLINE* pPOLYLINE = new POLYLINE;
	if(pPOLYLINE==NULL)
	{
		ASSERT(FALSE); //out of memory?
		return -1;
	}
	int idpolyline = m_ListPOLYLINE.GetCount();

	pPOLYLINE->idlineL0 = idlineL0;	
	pPOLYLINE->idlineLN = idlineLN;	
	pPOLYLINE->idpointPN = idpointPN;  
	if(pBoundingRect)
	{
		CRect myRect(pBoundingRect);
		myRect.NormalizeRect();
		pPOLYLINE->BoundingRect = myRect;
		
		//for unique segment polyline, inflate bounding rect 
		if(pPOLYLINE->idlineL0==pPOLYLINE->idlineLN)
		{
			if(myRect.IsRectEmpty() && myRect.Width()>0)
			{
				//inflate height, so this one segment polyline has a bounding rect
				myRect.InflateRect(0,1);
				pPOLYLINE->BoundingRect = myRect;
			}
			else if(myRect.IsRectEmpty() && myRect.Height()>0)
			{
				//inflate width
				myRect.InflateRect(1,0);
				pPOLYLINE->BoundingRect = myRect;
			}
		}
	}
	else
	{
		ASSERT(FALSE);
		pPOLYLINE->BoundingRect.left = 0;
		pPOLYLINE->BoundingRect.top = 0;
		pPOLYLINE->BoundingRect.right = 0;
		pPOLYLINE->BoundingRect.bottom = 0;
	}
	pPOLYLINE->idshape = idshape;    
	pPOLYLINE->id_info = id_info;	

	m_ListPOLYLINE.AddTail(pPOLYLINE);
	
	return idpolyline;
}

int COWEdgeset::AddPolyline(POLYLINE* pPOLYLINE) 
{
	return AddPolyline(	pPOLYLINE->idlineL0, 
						pPOLYLINE->idlineLN, 
						pPOLYLINE->idpointPN, 
						&(pPOLYLINE->BoundingRect), 
						pPOLYLINE->idshape, 	
						pPOLYLINE->id_info);
}

BOOL COWEdgeset::DeletePolyline(int idpolyline)
{
	//find polyline
	POLYLINE* pPOLYLINE = NULL;
	int idpolyline_found = -1;
	POSITION pos = m_ListPOLYLINE.GetHeadPosition();
	POSITION pos_prev = pos;
	while(pos!=NULL)
	{
		idpolyline_found++;
		pos_prev = pos;
		pPOLYLINE = (POLYLINE*)m_ListPOLYLINE.GetNext(pos);
		if(idpolyline_found==idpolyline)
		{
			break;
		}
	}
	if(idpolyline_found==idpolyline)
	{
		//delete POLYLINE structure from memory
		if(pPOLYLINE) delete pPOLYLINE;

		//TODO (later)
		//delete other related element from memory and from their respective list
		//TODO (later)

		//delete element from m_ListPOLYLINE
		m_ListPOLYLINE.RemoveAt(pos_prev);
	}
	return TRUE;
}

int COWEdgeset::GetNumberOfPolyline()
{
	return m_ListPOLYLINE.GetCount();
}

INTEGERPOINT* COWEdgeset::FindIntegerPoint(int idpoint)
{
	INTEGERPOINT* pINTEGERPOINT = NULL;
	int idpoint_found = -1;
	POSITION pos = m_ListINTEGERPOINT.GetHeadPosition();
	while(pos!=NULL && idpoint_found!=idpoint)
	{
		idpoint_found++;
		pINTEGERPOINT = (INTEGERPOINT*)m_ListINTEGERPOINT.GetNext(pos);
		if(idpoint_found==idpoint) break;
	}
	return pINTEGERPOINT;
}

FLOATPOINT* COWEdgeset::FindFloatPoint(int idpoint)
{
	FLOATPOINT* pFLOATPOINT = NULL;
	int idpoint_found = -1;
	POSITION pos = m_ListFLOATPOINT.GetHeadPosition();
	while(pos!=NULL && idpoint_found!=idpoint)
	{
		idpoint_found++;
		pFLOATPOINT = (FLOATPOINT*)m_ListFLOATPOINT.GetNext(pos);
		if(idpoint_found==idpoint) break;
	}
	return pFLOATPOINT;
}

LINE* COWEdgeset::FindLine(int idline)
{
	LINE* pLINE = NULL;
	int idline_found = -1;
	POSITION pos = m_ListLINE.GetHeadPosition();
	while(pos!=NULL && idline_found!=idline)
	{
		idline_found++;
		pLINE = (LINE*)m_ListLINE.GetNext(pos);
		if(idline_found==idline) break;
	}
	return pLINE;
}

POLYLINE* COWEdgeset::FindPolyline(int idpolyline)
{
	POLYLINE* pPOLYLINE = NULL;
	//spi, nov 2018, begin
	/*
	int idpolyline_found = -1;
	POSITION pos = m_ListPOLYLINE.GetHeadPosition();
	while(pos!=NULL && idpolyline_found!=idpolyline)
	{
		idpolyline_found++;
		pPOLYLINE = (POLYLINE*)m_ListPOLYLINE.GetNext(pos);
		if(idpolyline_found==idpolyline) break;
	}
	*/
	POSITION pos = m_ListPOLYLINE.FindIndex(idpolyline);
	pPOLYLINE = (POLYLINE*)m_ListPOLYLINE.GetAt(pos);
	//spi, nov 2018, end
	return pPOLYLINE;
}

int COWEdgeset::TranslatePolyline(int idpolyline, int iX_offset, int iY_offset)
{
	POLYLINE* pPOLYLINE = FindPolyline(idpolyline);

	//first line segment
	int idline = pPOLYLINE->idlineL0;
	LINE* pLINE = FindLine(idline);
	//first point of first polyline line segment
	INTEGERPOINT* pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP1);
	pINTEGERPOINT->iX = pINTEGERPOINT->iX + iX_offset;
	pINTEGERPOINT->iY = pINTEGERPOINT->iY + iY_offset;
	//second point of first polyline line segment
	pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP2);
	pINTEGERPOINT->iX = pINTEGERPOINT->iX + iX_offset;
	pINTEGERPOINT->iY = pINTEGERPOINT->iY + iY_offset;
	while(pLINE->idlineNL != -1) // && pLINE-pPOLYLINE->idlineLN)
	{
		pLINE = FindLine(pLINE->idlineNL);
		/* //no need to redo this one repetitively!!! 
		//first point of next polyline line segment
		pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP1);
		pINTEGERPOINT->iX = pINTEGERPOINT->iX + iX_offset;
		pINTEGERPOINT->iY = pINTEGERPOINT->iY + iY_offset;
		*/
		//second point of next polyline line segment
		pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP2);
		pINTEGERPOINT->iX = pINTEGERPOINT->iX + iX_offset;
		pINTEGERPOINT->iY = pINTEGERPOINT->iY + iY_offset;
	}

	return idpolyline; //success
}


int COWEdgeset::TranslatePolyline(int idpolyline, double fX_offset, double fY_offset)
{

	return idpolyline; //success
}

int COWEdgeset::GetPolylineNumberOfPoints(POLYLINE* pPOLYLINE)
{
	//first line segment
	int idline = pPOLYLINE->idlineL0;
	LINE* pLINE = FindLine(idline);
	//first point of first polyline line segment
	int index = 0;
	//second point of first polyline line segment
	index++;
	while(pLINE->idlineNL!=-1) 
	{
		pLINE = FindLine(pLINE->idlineNL);
		//second point of next polyline line segment
		index++;
	}
	int npoint_found = index+1;
	return npoint_found;
}

int COWEdgeset::GetPolylinePoints(	POLYLINE* pPOLYLINE, 
									POINT2DD* pPOINT2DD, //allocated by caller
									int npoint_max, //maximum number of POINT structure allocated by caller
									int npoint_offset, //=0
									POINT2DD* pDeltaPOINT2DD) //=NULL)
{
	int npoint_found = 0;
	ASSERT(pPOINT2DD!=NULL);

	//POLYLINE* pPOLYLINE = FindPolyline(idpolyline);
	if(pPOLYLINE==NULL) return -1;

	ASSERT(npoint_offset==0); //no offset supported for now

	//first line segment
	int idline = pPOLYLINE->idlineL0;
	LINE* pLINE = FindLine(idline);
	//first point of first polyline line segment
	int index = 0;
	INTEGERPOINT* pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP1);
	pPOINT2DD[index].x = pINTEGERPOINT->iX;
	pPOINT2DD[index].y = pINTEGERPOINT->iY;
	if(pDeltaPOINT2DD)
	{
		pDeltaPOINT2DD[index].x = pINTEGERPOINT->iDX;
		pDeltaPOINT2DD[index].y = pINTEGERPOINT->iDY;
	}
	//second point of first polyline line segment
	index++;
	pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP2);
	pPOINT2DD[index].x = pINTEGERPOINT->iX;
	pPOINT2DD[index].y = pINTEGERPOINT->iY;
	if(pDeltaPOINT2DD)
	{
		pDeltaPOINT2DD[index].x = pINTEGERPOINT->iDX;
		pDeltaPOINT2DD[index].y = pINTEGERPOINT->iDY;
	}
	while(pLINE->idlineNL!=-1 && index<npoint_max) // && pLINE-pPOLYLINE->idlineLN)
	{
		pLINE = FindLine(pLINE->idlineNL);
		/* //no need to redo this one repetitively!!! 
		//first point of next polyline line segment
		pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP1);
		pINTEGERPOINT->iX = pINTEGERPOINT->iX + iX_offset;
		pINTEGERPOINT->iY = pINTEGERPOINT->iY + iY_offset;
		*/
		//second point of next polyline line segment
		index++;
		pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP2);
		pPOINT2DD[index].x = pINTEGERPOINT->iX;
		pPOINT2DD[index].y = pINTEGERPOINT->iY;
		if(pDeltaPOINT2DD)
		{
			pDeltaPOINT2DD[index].x = pINTEGERPOINT->iDX;
			pDeltaPOINT2DD[index].y = pINTEGERPOINT->iDY;
		}
	}

	npoint_found = index+1;
	return npoint_found;
}

int COWEdgeset::GetPolylinePoints(	POLYLINE* pPOLYLINE, 
									POINT* pPOINT, //allocated by caller
									int npoint_max, //maximum number of POINT structure allocated by caller
									int npoint_offset, //=0
									POINT* pDeltaPOINT) //=NULL)
{
	int npoint_found = 0;
	ASSERT(pPOINT!=NULL);

	//POLYLINE* pPOLYLINE = FindPolyline(idpolyline);
	if(pPOLYLINE==NULL) return -1;

	ASSERT(npoint_offset==0); //no offset supported for now

	//first line segment
	int idline = pPOLYLINE->idlineL0;
	LINE* pLINE = FindLine(idline);
	//first point of first polyline line segment
	int index = 0;
	INTEGERPOINT* pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP1);
	pPOINT[index].x = pINTEGERPOINT->iX;
	pPOINT[index].y = pINTEGERPOINT->iY;
	if(pDeltaPOINT)
	{
		pDeltaPOINT[index].x = pINTEGERPOINT->iDX;
		pDeltaPOINT[index].y = pINTEGERPOINT->iDY;
	}
	//second point of first polyline line segment
	index++;
	pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP2);
	pPOINT[index].x = pINTEGERPOINT->iX;
	pPOINT[index].y = pINTEGERPOINT->iY;
	if(pDeltaPOINT)
	{
		pDeltaPOINT[index].x = pINTEGERPOINT->iDX;
		pDeltaPOINT[index].y = pINTEGERPOINT->iDY;
	}
	while(pLINE->idlineNL!=-1 && index<npoint_max) // && pLINE-pPOLYLINE->idlineLN)
	{
		pLINE = FindLine(pLINE->idlineNL);
		/* //no need to redo this one repetitively!!! 
		//first point of next polyline line segment
		pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP1);
		pINTEGERPOINT->iX = pINTEGERPOINT->iX + iX_offset;
		pINTEGERPOINT->iY = pINTEGERPOINT->iY + iY_offset;
		*/
		//second point of next polyline line segment
		index++;
		pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP2);
		pPOINT[index].x = pINTEGERPOINT->iX;
		pPOINT[index].y = pINTEGERPOINT->iY;
		if(pDeltaPOINT)
		{
			pDeltaPOINT[index].x = pINTEGERPOINT->iDX;
			pDeltaPOINT[index].y = pINTEGERPOINT->iDY;
		}
	}

	npoint_found = index+1;
	return npoint_found;
}

int COWEdgeset::AddLineToPolyline(int idpolyline, RECT* pRect, int iDX, int iDY)
{
	int idpolyline_found = -1;

	POLYLINE* pPOLYLINE = FindPolyline(idpolyline);
	if(pPOLYLINE)
	{
		//0) polyline found OK
		idpolyline_found = idpolyline;

		if(pRect!=NULL)
		{
			CRect myRect(pRect);
			if(myRect.Width()!=0 || myRect.Height()!=0)
			{
				//1) find polyline's last line segment
				int idlineLN = pPOLYLINE->idlineLN;
				LINE* pLINE = FindLine(idlineLN);

				//2) get last point of polyline's last line segment
				int idpointPN = pPOLYLINE->idpointPN;
				ASSERT(idpointPN==pLINE->idpointP2);
				
				//2.1) assert supplied point (pRect->left, pRect->top) == last point of polyline's last line segment
				//given the 2 points, the first one should already existing (at the end of the idpolyline)
				INTEGERPOINT* pINTEGERPOINT = FindIntegerPoint(idpointPN);
				ASSERT(pRect->left==pINTEGERPOINT->iX && pRect->top==pINTEGERPOINT->iY);

				//3.1) convert (right, bottom) point coordinate
				CRect myRect(pRect);
				int iX = pRect->left + myRect.Width();
				int iY = pRect->top + myRect.Height();
				//3.2) add (right, bottom) point
				int idpoint_new = AddPoint(	iX, iY, iDX, iDY); 

				//4) add new line
				int idlineNL = -1; //this will be the polyline's new last line segment
				int idline_new = AddLine(idpointPN, idpoint_new, -1);

				//5) edit polyline's previous last line segment
				pLINE->idlineNL = idline_new;

				//6) edit polyline
				pPOLYLINE->idlineLN = idline_new;	
				pPOLYLINE->idpointPN = idpoint_new; 
				myRect = *pRect;
				myRect.NormalizeRect();
				pPOLYLINE->BoundingRect = myRect; //last line segment's bounding rect
			}
		}
		else
		{
			//when pRect==NULL simply update the bounding box

			//1) collect all points
			const int npoint_max = 1000;
			POINT pPOINT[npoint_max];
			int npoints = GetPolylinePoints(pPOLYLINE, pPOINT, npoint_max);
			if(npoints<2) 
			{
				ASSERT(FALSE);
				return -1;
			}
			//2) find min max
			CRect myRect(pPOINT[0].x, pPOINT[0].y, pPOINT[0].x, pPOINT[0].y);
			for(int i=1; i<npoints; i++)
			{
				if(pPOINT[i].x < myRect.left) myRect.left = pPOINT[i].x;
				if(pPOINT[i].y < myRect.top) myRect.top = pPOINT[i].y;
				if(pPOINT[i].x > myRect.right) myRect.right = pPOINT[i].x;
				if(pPOINT[i].y > myRect.bottom) myRect.bottom = pPOINT[i].y;
			}
			//3) update POLYLINE
			pPOLYLINE->BoundingRect = myRect;

			//4) for unique segment polyline, inflate bounding rect 
			if(myRect.IsRectEmpty() && myRect.Width()!=0)
			{
				//inflate height, so this one segment polyline has a bounding rect
				myRect.InflateRect(0,1);
				pPOLYLINE->BoundingRect = myRect;
			}
			else if(myRect.IsRectEmpty() && myRect.Height()!=0)
			{
				//inflate width
				myRect.InflateRect(1,0);
				pPOLYLINE->BoundingRect = myRect;
			}

		}
	}

	return idpolyline_found;
}

int COWEdgeset::IsTouchingPolylineEdge(CPoint pt)
{
	int idpolyline = -1;
	ASSERT(FALSE); //to implement further

	//1) find if inside bounding rect first
	idpolyline = IsTouchingPolylineBoundingRect(pt);

	if(idpolyline!=-1)
	{
		//2) find if inside polyline ...
	}
	return idpolyline;
}

int COWEdgeset::IsTouchingPolylineBoundingRect(CPoint pt)
{
	int idpolyline = -1;
		
	POLYLINE* pPOLYLINE = NULL;
	int idpolyline_found = -1;
	POSITION pos = m_ListPOLYLINE.GetHeadPosition();
	while(pos!=NULL)
	{
		idpolyline_found++;
		pPOLYLINE = (POLYLINE*)m_ListPOLYLINE.GetNext(pos);
		//skip first 4 default roi
		if(idpolyline_found>3)
		{
			//find first from set of user roi 
			CRect myBoundingRect(pPOLYLINE->BoundingRect);
			if(myBoundingRect.IsRectEmpty())
			{
				//no bounding rect specified for this polyline
				ASSERT(FALSE); 
			}
			if(myBoundingRect.PtInRect(pt))
			{
				idpolyline = idpolyline_found;
				break;
			}
		}
	}
	return idpolyline;
}

//collect all grid points along an edge (a line segment) defined by between P1 and P2 
//grid step default to 1 (i.e. 1x1 pixel units)
//
//grid - 4 neighbor implementation
int COWEdgeset::EdgeFill(	POINT P1, 
							POINT P2, 
							POINT* pPOINT, //allocated by caller to collect all intermediate points between P1 and P2 (given the grid step)
							int npoint_max, //maximum number of POINT structure allocated by caller
							int iGridStep,
							POINT* pInnerEdgePOINT, //=NULL,
							POINT* pOutterEdgePOINT, //=NULL,
							int npoint_ioe_max, //=0,
							int* p_npoint_ioe_collected, //=0,
							POINT* pInnerEBRPOINT, //=NULL,
							int npoint_iebr_max, //=0)
							int* p_npoint_iebr_collected, //=0,
							POINT* pOutterEBRPOINT, //=NULL,
							int npoint_oebr_max, //=0)
							int* p_npoint_oebr_collected) //=0,
{
	ASSERT(iGridStep==1); //otherwise, would require to revise implementation
	int npoints = EdgeFill(P1, P2, pPOINT, npoint_max);

	//use scanline algo to collect up to npoint_ioe_max into pInnerEdgePOINT and/or pOutterEdgePOINT
	if( (pInnerEdgePOINT!=NULL || pOutterEdgePOINT!=NULL)
		&& p_npoint_ioe_collected!=NULL )
	{
		POINT P1_ie_start;
		POINT P2_ie_end;
		POINT P1_oe_start;
		POINT P2_oe_end;
		if(P1.x<P2.x && P1.y<=P2.y)
		{
			//ligne horizontale ou deuxieme quadran
			if((P2.y-P1.y)<=(P2.x-P1.x)) 
			{
				//slope P1 P2 smaller than 45 degree
				P1_oe_start.x = P1.x; 
				P1_oe_start.y = P1.y - iGridStep;
				P2_oe_end.x = P2.x;
				P2_oe_end.y = P2.y - iGridStep;

				P1_ie_start.x = P1.x; 
				P1_ie_start.y = P1.y + iGridStep;
				P2_ie_end.x = P2.x;
				P2_ie_end.y = P2.y + iGridStep;
			}
			else
			{
				//slope P1 P2 greater than 45 degree
				P1_oe_start.x = P1.x + iGridStep;
				P1_oe_start.y = P1.y;
				P2_oe_end.x = P2.x + iGridStep;
				P2_oe_end.y = P2.y;

				P1_ie_start.x = P1.x - iGridStep;
				P1_ie_start.y = P1.y;
				P2_ie_end.x = P2.x - iGridStep;
				P2_ie_end.y = P2.y;
			}
		}
		else if(P1.x>=P2.x && P1.y<P2.y)
		{
			//ligne verticale ou troisieme quadran
			if((P2.y-P1.y)>=abs(P2.x-P1.x))
			{
				//slope P1 P2 greater than 45 degree
				P1_oe_start.x = P1.x + iGridStep;
				P1_oe_start.y = P1.y;
				P2_oe_end.x = P2.x + iGridStep;
				P2_oe_end.y = P2.y;

				P1_ie_start.x = P1.x - iGridStep;
				P1_ie_start.y = P1.y;
				P2_ie_end.x = P2.x - iGridStep;
				P2_ie_end.y = P2.y;
			}
			else
			{
				//slope P1 P2 smaller than 45 degree
				P1_oe_start.x = P1.x ;
				P1_oe_start.y = P1.y + iGridStep;
				P2_oe_end.x = P2.x;
				P2_oe_end.y = P2.y  + iGridStep;

				P1_ie_start.x = P1.x ;
				P1_ie_start.y = P1.y - iGridStep;
				P2_ie_end.x = P2.x;
				P2_ie_end.y = P2.y  - iGridStep;
			}
		}
		else if(P1.x>P2.x && P1.y>=P2.y)
		{
			//ligne horizontale ou quatrieme quadran
			//similaire au cas du deuxieme quadran mais avec P1 et P2 inverse
			if(abs(P2.y-P1.y)<=abs(P2.x-P1.x)) 
			{
				//slope P1 P2 smaller than 45 degree
				P1_oe_start.x = P1.x; 
				P1_oe_start.y = P1.y + iGridStep;
				P2_oe_end.x = P2.x;
				P2_oe_end.y = P2.y + iGridStep;

				P1_ie_start.x = P1.x; 
				P1_ie_start.y = P1.y - iGridStep;
				P2_ie_end.x = P2.x;
				P2_ie_end.y = P2.y - iGridStep;
			}
			else
			{
				//slope P1 P2 greater than 45 degree
				P1_oe_start.x = P1.x - iGridStep;
				P1_oe_start.y = P1.y;
				P2_oe_end.x = P2.x - iGridStep;
				P2_oe_end.y = P2.y;

				P1_ie_start.x = P1.x + iGridStep;
				P1_ie_start.y = P1.y;
				P2_ie_end.x = P2.x + iGridStep;
				P2_ie_end.y = P2.y;
			}
		}
		else if(P1.x<=P2.x && P1.y>P2.y)
		{
			//ligne verticale ou premier quadran
			//similaire au cas du troisieme quadran mais avec P1 et P2 inverse
			if(abs(P2.y-P1.y)>=(P2.x-P1.x)) 
			{
				//slope P1 P2 greater than 45 degree
				P1_oe_start.x = P1.x - iGridStep; 
				P1_oe_start.y = P1.y;
				P2_oe_end.x = P2.x - iGridStep;
				P2_oe_end.y = P2.y;

				P1_ie_start.x = P1.x + iGridStep; 
				P1_ie_start.y = P1.y;
				P2_ie_end.x = P2.x + iGridStep;
				P2_ie_end.y = P2.y;
			}
			else
			{
				//slope P1 P2 smaller than 45 degree
				P1_oe_start.x = P1.x;
				P1_oe_start.y = P1.y + iGridStep;
				P2_oe_end.x = P2.x;
				P2_oe_end.y = P2.y + iGridStep;

				P1_ie_start.x = P1.x;
				P1_ie_start.y = P1.y - iGridStep;
				P2_ie_end.x = P2.x;
				P2_ie_end.y = P2.y - iGridStep;
			}
		}
		else
		{
			ASSERT(FALSE);
		}

		int npoints_oe = 0;
		int npoints_ie = 0;
		if(pOutterEdgePOINT)
		{
			npoints_oe = EdgeFill(P1_oe_start, P2_oe_end, pOutterEdgePOINT, npoint_ioe_max);
		}
		if(pInnerEdgePOINT)
		{
			npoints_ie = EdgeFill(P1_ie_start, P2_ie_end, pInnerEdgePOINT, npoint_ioe_max);
		}
		if(npoints_oe!=0 && npoints_ie!=0)
		{
			ASSERT(npoints_ie==npoints_oe);
		}
		if(npoints_ie) p_npoint_ioe_collected[0] = npoints_ie;
		  else p_npoint_ioe_collected[0] = npoints_oe;



		int i=0;
		/*
		while(i<npoints && i<npoint_ioe_max) 
		{
			//for each edge point
			//i++; have i ok here 

			//count consecutive points
			int j=0; 
			while(pPOINT[i+j].y==pPOINT[i].y)
			{
				j++; //for each sets of edge point (set defined as consecutive edge point that are on the same grid line) 
			}
			int npoints_consecutive = j;

			//add inner and/or outter edge points (may exceed bounding rect)
			int i_edgestart = i;
			int i_edgeend = i + npoints_consecutive;
			for(j=i_edgestart; j<i_edgeend; j++)
			{
				if(pInnerEdgePOINT)
				{
					pInnerEdgePOINT[j].x = pPOINT[j].x - npoints_consecutive;
					pInnerEdgePOINT[j].y = pPOINT[j].y;
				}
				if(pOutterEdgePOINT)
				{
					pOutterEdgePOINT[j].x = pPOINT[j].x + npoints_consecutive;
					pOutterEdgePOINT[j].y = pPOINT[j].y;
				}
			}
			i = i_edgeend; //i is now set to the next index 
		}
		p_npoint_ioe_collected[0] = i;
		*/

		if(i==npoint_ioe_max)
		{
			//if loop was stoped because of npoint_ioe_max
			ASSERT(FALSE); //caller should allocate more
		}
	}


	//use scanline algo to collect up to npoint_ioebr_max into pInnerEBRPOINT and/or pOutterEBRPOINT
	if( pInnerEBRPOINT!=NULL  || pOutterEBRPOINT!=NULL )
	{
		if(pInnerEBRPOINT!=NULL)
		{
			ASSERT(p_npoint_iebr_collected!=NULL);
		}
		if(pOutterEBRPOINT!=NULL)
		{
			ASSERT(p_npoint_oebr_collected!=NULL);
		}

		int i=0;
		int k_inner = 0;
		int k_outter = 0;
		CRect myBoundingRect;
		if(P1.x<=P2.x)
		{
			myBoundingRect.left = P1.x;
			myBoundingRect.right = P2.x;
		}
		else
		{
			myBoundingRect.left = P2.x;
			myBoundingRect.right = P1.x;
		}
		if(P1.y<=P2.y)
		{
			myBoundingRect.top = P1.y;
			myBoundingRect.bottom = P2.y;
		}
		else
		{
			myBoundingRect.top = P2.y;
			myBoundingRect.bottom = P1.y;
		}
		if(myBoundingRect.Width()==0 || myBoundingRect.Height()==0)
		{
			//straight line segment, vertical or horizontal
			//no ebr point to collect
		}
		else
		{
			int jj=0; 
			while(i<npoints) 
			{
				//for each edge point
				//i++; have i ok here 

				//count consecutive points
				jj=0; 
				while(pPOINT[i+jj].y==pPOINT[i].y && (i+jj)<npoints)
				{
					jj++; //for each sets of edge point (set defined as consecutive edge point that are on the same grid line) 
				}
				int npoints_consecutive = jj;

				//add inner and/or outter edge points (may exceed bounding rect)
				int i_edgestart = i; //edge
				int i_edgeend = i + npoints_consecutive -1; //edge

				//no swapping required when normal case, that is P2.x > P1.x AND P2.y > P1.y
				BOOL bSwapInnerOutter = FALSE;
				int x_ebrstart = P1.x; //ebr, edge bounding rect
				int x_ebrend = P2.x; //ebr, edge bounding rect

				int x_outter_ebrstart;
				int x_outter_ebrend;
				int x_inner_ebrstart;
				int x_inner_ebrend;

				if((P2.x-P1.x)>0 && (P2.y-P1.y)>0)
				{
					//going from P1 towards P2, outter edge is defined to be above
					//line segment (P1,P2)
					x_outter_ebrstart = pPOINT[i_edgeend].x + 1; //+ 1; replaced by j_inc//next one after last edge point
					x_outter_ebrend = x_ebrend; //P2.x, right most
					x_inner_ebrstart = x_ebrstart; //P1.x, left most
					x_inner_ebrend = pPOINT[i_edgestart].x - 1; 

					for(int j=x_inner_ebrstart; j<=x_inner_ebrend; j++)
					{
						if(pInnerEBRPOINT)
						{
							pInnerEBRPOINT[k_inner].x = j;
							pInnerEBRPOINT[k_inner].y = pPOINT[i].y;
							k_inner++;
						}
					}
					for(int j=x_outter_ebrstart; j<=x_outter_ebrend; j++)
					{
						if(pOutterEBRPOINT)
						{
							pOutterEBRPOINT[k_outter].x = j;
							pOutterEBRPOINT[k_outter].y = pPOINT[i].y;
							k_outter++;
						}
					}
				}
				else if((P2.x-P1.x)>0 && (P2.y-P1.y)<0)
				{
					//going from P1 towards P2, outter edge is defined to be below
					//line segment (P1,P2)
					x_inner_ebrstart = pPOINT[i_edgeend].x + 1; //+ 1; replaced by j_inc//next one after last edge point
					x_inner_ebrend = x_ebrend; //P2.x, right most
					x_outter_ebrstart = x_ebrstart; //P1.x, left most
					x_outter_ebrend = pPOINT[i_edgestart].x - 1; 

					for(int j=x_outter_ebrstart; j<=x_outter_ebrend; j++)
					{
						if(pOutterEBRPOINT)
						{
							pOutterEBRPOINT[k_outter].x = j;
							pOutterEBRPOINT[k_outter].y = pPOINT[i].y;
							k_outter++;
						}
					}
					for(int j=x_inner_ebrstart; j<=x_inner_ebrend; j++)
					{
						if(pInnerEBRPOINT)
						{
							pInnerEBRPOINT[k_inner].x = j;
							pInnerEBRPOINT[k_inner].y = pPOINT[i].y;
							k_inner++;
						}
					}
				}
				else if((P2.x-P1.x)<0 && (P2.y-P1.y)>0)
				{
					//going from P1 towards P2, outter is defined to be below
					//line segment (P1,P2)
					x_inner_ebrstart = pPOINT[i_edgeend].x - 1; 
					x_inner_ebrend = x_ebrend; //P2.x, left most
					x_outter_ebrstart = x_ebrstart; //P1.x, right most
					x_outter_ebrend = pPOINT[i_edgestart].x + 1; 

					for(int j=x_inner_ebrstart; j>=x_inner_ebrend; j--)
					{
						if(pInnerEBRPOINT)
						{
							pInnerEBRPOINT[k_inner].x = j;
							pInnerEBRPOINT[k_inner].y = pPOINT[i].y;
							k_inner++;
						}
					}
					for(int j=x_outter_ebrstart; j>=x_outter_ebrend; j--)
					{
						if(pOutterEBRPOINT)
						{
							pOutterEBRPOINT[k_outter].x = j;
							pOutterEBRPOINT[k_outter].y = pPOINT[i].y;
							k_outter++;
						}
					}
				}
				else if((P2.x-P1.x)<0 && (P2.y-P1.y)<0)
				{
					//going from P1 towards P2, outter is defined to be below
					//line segment (P1,P2)
					x_outter_ebrstart = pPOINT[i_edgeend].x - 1; 
					x_outter_ebrend = x_ebrend; //P2.x, left most
					x_inner_ebrstart = x_ebrstart; //P1.x, right most
					x_inner_ebrend = pPOINT[i_edgestart].x + 1; 

					for(int j=x_outter_ebrstart; j>=x_outter_ebrend; j--)
					{
						if(pOutterEBRPOINT)
						{
							pOutterEBRPOINT[k_outter].x = j;
							pOutterEBRPOINT[k_outter].y = pPOINT[i].y;
							k_outter++;
						}
					}
					for(int j=x_inner_ebrstart; j>=x_inner_ebrend; j--)
					{
						if(pInnerEBRPOINT)
						{
							pInnerEBRPOINT[k_inner].x = j;
							pInnerEBRPOINT[k_inner].y = pPOINT[i].y;
							k_inner++;
						}
					}
				}
				else
				{
					//P2.x == P1.x ?
					ASSERT(FALSE);
				}

				i = i_edgeend + 1; //i is now set to the next index 
			}//end of while()
		}
		//ASSERT(k_inner==k_outter); //should it always be so symmetric?
		p_npoint_iebr_collected[0] = k_inner+1; //k_inner being an index
		p_npoint_oebr_collected[0] = k_outter+1; //k_outter being an index

	}
	return npoints;
}

//collect all grid points along an edge (a line segment) defined by between P1 and P2 
//grid step default to 1 (i.e. 1x1 pixel units)
//
//grid - 4 neighbor implementation
int COWEdgeset::EdgeFill(	POINT P1, 
							POINT P2, 
							POINT* pPOINT, //allocated by caller to collect all intermediate points between P1 and P2 (given the grid step)
							int npoint_max) //maximum number of POINT structure allocated by caller
{
	int npoints = 0;
	if(pPOINT==NULL)
	{
		//can't store any output, caller should allocate pPOINT
		ASSERT(FALSE);
		return -1;
	}
	//implementation not revised for various grid steps
	int iGridStep=1;

	int i_deltaX = P2.x-P1.x;
	int i_deltaY = P2.y-P1.y;
	
	//if( (i_deltaX>1 || i_deltaX<-1) || (i_deltaY>1 || i_deltaY<-1) )
	if( abs(i_deltaX)<2*iGridStep && abs(i_deltaY)<2*iGridStep )
	{
		//P1 and P2 are adjacent, collect no point, return safely the 2 original points
		//ASSERT(FALSE);
		npoints = 2;
		pPOINT[0] = P1;
		pPOINT[1] = P2;
		return npoints;
	}
	//approx estimated number of points to end up in final vector 
	//int n = sqrt(i_deltaX*i_deltaX + i_deltaY*i_deltaY);
	double abs_deltaX = fabs((double)i_deltaX);
	double abs_deltaY = fabs((double)i_deltaY);
	int n = (int)(abs_deltaX + abs_deltaY);
	if(npoint_max<n)
	{
		//caller should alloca more POINT structure, increase pPOINT vector size
		ASSERT(FALSE);
		return -1;
	}

	npoints = 0;
	pPOINT[npoints] = P1;
	int ix = 0;
	int iy = 0;
	int ix_increment = 0;
	int iy_increment = 0;
	double f_slope_optimal;
	if(abs_deltaX>abs_deltaY) f_slope_optimal = i_deltaY*1.0/i_deltaX;
	  else f_slope_optimal = i_deltaX*1.0/i_deltaY;

	if(i_deltaX<0)
	{
		//going towards P2 decreasing ix
		//ix--;
		ix_increment = -1*iGridStep;
	}
	else
	{
		//going towards P2 increasing ix
		//ix++;
		ix_increment = +1*iGridStep;
	}

	if(i_deltaY<0)
	{
		//going towards P2 decreasing iy
		//iy--;
		iy_increment = -1*iGridStep;
	}
	else
	{
		//going towards P2 increasing iy
		//iy++;
		iy_increment = +1*iGridStep;
	}
	
	//very first next point
	POINT myPOINT;
	if(abs_deltaX>abs_deltaY) ix = ix + ix_increment;
	  else iy = iy + iy_increment;
	myPOINT.x = P1.x + ix;
	myPOINT.y = P1.y + iy;
	npoints++;
	pPOINT[npoints] = myPOINT;
	//while(ix!=i_deltaX && iy!=i_deltaY)
	while( !(ix==i_deltaX && iy==i_deltaY) )
	{
		//next point should either be the adjacent one incrementing y (ix,iy++) 
		//or the adjacent one incrementing x (ix++,iy). 
		//determine which one should be picked based upon smallest resultant delta y/ delta x
		double f_slope_adjy;
		if(abs_deltaX>abs_deltaY) f_slope_adjy = (iy+iy_increment+0.0)/ix;
		  else f_slope_adjy = ix*1.0/(iy+iy_increment);

		double f_slope_adjx;
		if(abs_deltaX>abs_deltaY) f_slope_adjx = iy/(ix+ix_increment+0.0);
		  else f_slope_adjx = (ix+ix_increment)/(iy*1.0);

		if( fabs(f_slope_adjy-f_slope_optimal) < fabs(f_slope_adjx-f_slope_optimal) )
		{
			iy = iy + iy_increment;
		}
		else
		{
			ix = ix + ix_increment;
		}
		myPOINT.x = P1.x + ix;
		myPOINT.y = P1.y + iy;
		npoints++;
		pPOINT[npoints] = myPOINT;
		  
		if(npoints>n)
		{
			ASSERT(FALSE);
			break;
		}
	}
	/* P2 should already be included
	npoints++;
	pPOINT[npoints] = P2;
	*/
	npoints++;
	return npoints; //total number of point returned in pPOINT vector, including P1 and P2
}

int COWEdgeset::EstimateRectFillMemoryRequirement(	RECT* pRECT,
													int* p_npoint_e_max,
													int* p_npoint_ioe_max,
													int* p_npoint_br_max)
{
	//get polyline points
	const int npoints = 5;
	POINT pPOINT[npoints];
	pPOINT[0].x = pRECT->left;
	pPOINT[0].y = pRECT->top;
	pPOINT[1].x = pRECT->right;
	pPOINT[1].y = pRECT->top;
	pPOINT[2].x = pRECT->right;
	pPOINT[2].y = pRECT->bottom;
	pPOINT[3].x = pRECT->left;
	pPOINT[3].y = pRECT->bottom;
	pPOINT[4].x = pRECT->left;
	pPOINT[4].y = pRECT->top;
	
	for(int i=0; i<(npoints-1); i++)
	{
		//for each polyline's line segment (pair of points)
		p_npoint_e_max[0] = p_npoint_e_max[0] + 2 + (abs(pPOINT[i+1].x - pPOINT[i].x) + abs(pPOINT[i+1].y - pPOINT[i].y)); //estimate of max possible number 
	}
	p_npoint_ioe_max[0] = p_npoint_e_max[0];

	p_npoint_br_max[0] = abs( (pRECT->right - pRECT->left) * (pRECT->right - pRECT->left) );
	return TRUE;
}

int COWEdgeset::RectEdgeFill(	RECT pRECT, 
								POINT* pEdgePOINT, //allocated by caller to collect all intermediate points between P1 and P2 (given the grid step)
								int npoint_max) //maximum number of POINT structure allocated by caller
{
	POINT P1[4];
	POINT P2[4];
	//upper left and upper right corners
	P1[0].x = pRECT.left;
	P1[0].y = pRECT.top;
	P2[0].x = pRECT.right;
	P2[0].y = pRECT.top;
	//upper left and upper right corners
	P1[1].x = pRECT.right;
	P1[1].y = pRECT.top;
	P2[1].x = pRECT.right;
	P2[1].y = pRECT.bottom;
	//upper left and upper right corners
	P1[2].x = pRECT.right;
	P1[2].y = pRECT.bottom;
	P2[2].x = pRECT.left;
	P2[2].y = pRECT.bottom;
	//upper left and upper right corners
	P1[3].x = pRECT.left;
	P1[3].y = pRECT.bottom;
	P2[3].x = pRECT.left;
	P2[3].y = pRECT.top;

	int ipoint_max[4];
	int npoints_edge[4];
	POINT* pPOINT[4];
	//for each line segment, collect vector of POINTs
	for(int i=0; i<4; i++)
	{
		ipoint_max[i] = 2 + (abs(P2[i].x - P1[i].x) + abs(P2[i].y - P1[i].y)); //estimate of max possible number 
		pPOINT[i] = (POINT*) new POINT[ipoint_max[i]];
		npoints_edge[i] = EdgeFill(P1[i], P2[i], pPOINT[i], ipoint_max[i]);

	}
	//merge vector of POINTs collected
	int npoints = 0;
	int i=0;
	POINT* pEDGEPOINT = pEdgePOINT;
	while(i<4 && (npoints+npoints_edge[i])<npoint_max)
	{
		memcpy(pEDGEPOINT, &(pPOINT[i][0]), npoints_edge[i]*sizeof(POINT));
		//for next loop, have pEDGEPOINT point to the last element (it will overwrite, so
		//there will not be duplicate POINT, to handle cases, like here, when consecutive
		//line segments are connected)
		npoints = npoints + npoints_edge[i] -1; 
		pEDGEPOINT = &(pEdgePOINT[npoints]);
		i++;
	}

	//delete internal vector of POINTs
	for(i=0; i<4; i++) delete[] pPOINT[i];

	return npoints;
}

int COWEdgeset::EstimatePolylineFillMemoryRequirement(	POLYLINE* pPOLYLINE,
														int* p_npoint_e_max,
														int* p_npoint_ioe_max,
														int* p_npoint_iebr_max,
														int* p_npoint_oebr_max)
{
	ASSERT(p_npoint_e_max!=NULL);
	ASSERT(p_npoint_ioe_max!=NULL);
	ASSERT(p_npoint_iebr_max!=NULL);
	ASSERT(p_npoint_oebr_max!=NULL);

	p_npoint_e_max[0] = 0;
	p_npoint_ioe_max[0] = 0;
	p_npoint_iebr_max[0] = 0;
	p_npoint_oebr_max[0] = 0;

	//get polyline points
	int npoint_max = GetPolylineNumberOfPoints(pPOLYLINE);
	POINT* pPOINT = (POINT*) new POINT[npoint_max];
	int npoints = GetPolylinePoints(pPOLYLINE, pPOINT, npoint_max);
	for(int i=0; i<(npoints-1); i++)
	{
		//for each polyline's line segment (pair of points)
		int npoint_e_max = + 2 * (abs(pPOINT[i+1].x - pPOINT[i].x) + abs(pPOINT[i+1].y - pPOINT[i].y)); //estimate of max possible number 
		p_npoint_e_max[0] = p_npoint_e_max[0] + npoint_e_max;

		int npoint_iebr_max = (int) (abs((pPOINT[i+1].x - pPOINT[i].x) * (pPOINT[i+1].y - pPOINT[i].y))/2.0) + npoint_e_max;
		p_npoint_iebr_max[0] = p_npoint_iebr_max[0] + npoint_iebr_max;
	}
	p_npoint_ioe_max[0] = p_npoint_e_max[0];

	/*
	p_npoint_iebr_max[0] = (int)(( abs(pPOLYLINE->BoundingRect.right - pPOLYLINE->BoundingRect.left)
								* abs(pPOLYLINE->BoundingRect.bottom - pPOLYLINE->BoundingRect.top))/2.0
								+ p_npoint_e_max[0];
	*/
	p_npoint_oebr_max[0] = p_npoint_iebr_max[0];
	if(pPOINT) delete[] pPOINT;
	return TRUE;
}

int COWEdgeset::PolylineFill(	POLYLINE* pPOLYLINE, 							
								POINT* pEdgePOINT, //allocated by caller to collect all intermediate points between Pi and Pi+1 segments (given the grid step)
								int npoint_max, //maximum number of POINT structure allocated by caller
								int iGridStep, //=1,
								POINT* pInnerEdgePOINT, //=NULL,
								POINT* pOutterEdgePOINT, //=NULL,
								int npoint_ioe_max, //=0,
								int* p_npoint_ioe_collected, //=NULL,
								POINT* pInnerEBRPOINT, //=NULL,
								int npoint_iebr_max, //=0,
								int* p_npoint_iebr_collected, //=NULL)
								POINT* pOutterEBRPOINT, //=NULL,
								int npoint_oebr_max, //=0,
								int* p_npoint_oebr_collected) //=NULL)
{
	ASSERT(iGridStep==1);
	if(pPOLYLINE==NULL || pEdgePOINT==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//get polyline points
	int npoints_polyline_max = GetPolylineNumberOfPoints(pPOLYLINE);
	POINT* pPolylinePOINT = (POINT*) new POINT[npoints_polyline_max];
	int npoints_polyline = GetPolylinePoints(pPOLYLINE, pPolylinePOINT, npoints_polyline_max);

	POINT* pPOINT = NULL; 
	POINT* pPOINT_Inner = NULL;
	POINT* pPOINT_Outter = NULL;
	POINT* pPOINT_InnerEBR = NULL;
	POINT* pPOINT_OutterEBR = NULL;
	if(pEdgePOINT) pPOINT = &(pEdgePOINT[0]);
	if(pInnerEdgePOINT) pPOINT_Inner = &(pInnerEdgePOINT[0]);
	if(pOutterEdgePOINT) pPOINT_Outter = &(pOutterEdgePOINT[0]);
	if(pInnerEBRPOINT) pPOINT_InnerEBR = &(pInnerEBRPOINT[0]);
	if(pOutterEBRPOINT) pPOINT_OutterEBR = &(pOutterEBRPOINT[0]);
	
	
	int npoints = 0; 
	int npoints_prev = 0;
	int npoints_max_remaining = npoint_max;

	int npoints_ioe = 0; 
	int npoints_ioe_prev = 0;
	int npoints_ioe_max_remaining = npoint_ioe_max;

	int npoints_iebr = 0;
	int npoints_oebr = 0;
	int npoints_iebr_prev = 0;
	int npoints_oebr_prev = 0;
	int npoints_iebr_max_remaining = npoint_iebr_max;
	int npoints_oebr_max_remaining = npoint_oebr_max;

	//for each polyline's line segment, call edgefill
	int nlines_polyline = npoints_polyline -1;
	for(int i=0; i<nlines_polyline; i++)
	{
		npoints_max_remaining = npoints_max_remaining - npoints_prev - 1;
		npoints_ioe_max_remaining = npoints_ioe_max_remaining - npoints_ioe_prev - 1;
		npoints_iebr_max_remaining = npoints_iebr_max_remaining - npoints_iebr_prev - 1;
		npoints_oebr_max_remaining = npoints_oebr_max_remaining - npoints_oebr_prev - 1;
		
		/* this function was tested and is working fine
		npoints_prev = EdgeFill(pPolylinePOINT[i], 
								pPolylinePOINT[i+1], 
								pPOINT, 
								npoints_max_remaining);
		*/
		npoints_prev = EdgeFill(pPolylinePOINT[i], 
								pPolylinePOINT[i+1],
								pPOINT, 
								npoints_max_remaining, 
								iGridStep,
								pPOINT_Inner, 
								pPOINT_Outter, 
								npoints_ioe_max_remaining, 
								&npoints_ioe_prev, 
								pPOINT_InnerEBR, 
								npoints_iebr_max_remaining, 
								&npoints_iebr_prev,
								pPOINT_OutterEBR, 
								npoints_oebr_max_remaining, 
								&npoints_oebr_prev); 

		if(npoints_prev>0) npoints = npoints + npoints_prev - 1; 
		if(npoints_ioe_prev>0) npoints_ioe = npoints_ioe + npoints_ioe_prev - 1; 
		if(npoints_iebr_prev>0) npoints_iebr = npoints_iebr + npoints_iebr_prev - 1; 
		if(npoints_oebr_prev>0) npoints_oebr = npoints_oebr + npoints_oebr_prev - 1; 

		if(pEdgePOINT) pPOINT = &(pEdgePOINT[npoints]);
		if(pInnerEdgePOINT) pPOINT_Inner = &(pInnerEdgePOINT[npoints_ioe]);
		if(pOutterEdgePOINT) pPOINT_Outter = &(pOutterEdgePOINT[npoints_ioe]);
		if(pInnerEBRPOINT) pPOINT_InnerEBR = &(pInnerEBRPOINT[npoints_iebr]);
		if(pOutterEBRPOINT) pPOINT_OutterEBR = &(pOutterEBRPOINT[npoints_oebr]);
	}
	//spi, march 2010, replacing i with nlines_polyline, begin
	/*
	if(pPolylinePOINT[i].x==pPolylinePOINT[0].x
		&& pPolylinePOINT[i].y==pPolylinePOINT[0].y)	
	*/
	if(pPolylinePOINT[nlines_polyline].x==pPolylinePOINT[0].x
		&& pPolylinePOINT[nlines_polyline].y==pPolylinePOINT[0].y)	
	//spi, march 2010, replacing i with nlines_polyline, begin
	{
		//last point is excluded
		//npoints = npoints;
	}
	else
	{
		//re-include the last point
		npoints++;
	}
	p_npoint_ioe_collected[0] = npoints_ioe;
	p_npoint_iebr_collected[0] = npoints_iebr;
	p_npoint_oebr_collected[0] = npoints_oebr;


	delete[] pPolylinePOINT;
	//delete[] p_npoints_edge;
	return npoints;
}

//collect all grid points along a polygonal edge (a closed polyline segment) defined by pPOLYLINE 
//grid step default to 1 (i.e. 1x1 pixel units)
int COWEdgeset::PolygonFill(POLYLINE* pPOLYLINE, 							
							POINT* pPerimeterPOINT, //allocated by caller to collect all intermediate points between Pi and Pi+1 segments (given the grid step)
							int npoint_per_max, //maximum number of POINT structure allocated by caller
							POINT* pInteriorPOINT,
							int npoint_int_max,
							int iGridStep)
{
	int npoints = 0;
	return npoints;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//class COWRoiset
//
//region of interest vectorial overlay, user defined with mouse tools, mouse moves and clicks 
/////////////////////////////////////////////////////////////////////////////////////////////
COWRoiset::COWRoiset()
{
	DefaultInitialization();

	/////////////////////
	/////////////////////
	//edgeset sample code
	/////////////////////
	/////////////////////

	//////////////////////////////////
	//create 4 initial rectangular ROI
	//////////////////////////////////
	//i.e. smallest doc rect 
	//     largest doc rect
	//	   user view rect
	//     selection rect floating in the user viewing window

	//set edgeset to use internal integer point list
	m_bINTEGERPOINT = TRUE;
	m_bFLOATPOINT = FALSE;
	m_pPointset = NULL;
	m_bDual = FALSE;
	

	//add smallest COW2Doc rect (smallest world rect unit)
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = 1;
	rect.bottom = 1; //1 pixel in device coordinates
	int idpolyline = AddRectangularROI(&rect);
	ASSERT(idpolyline==EDGESET_SMALLEST_WORLDRECT_ROI_ID);

	//add smallest COW2Doc rect (largest world rect unit)
	rect.left = 0;
	rect.top = 0;
	rect.right = 0;
	rect.bottom = 0; //initial world rect is 0,0,0,0
	idpolyline = AddRectangularROI(&rect);
	ASSERT(idpolyline==EDGESET_LARGEST_WORLDRECT_ROI_ID);

	//add smallest COW2View rect (user view rect unit)
	rect.left = 0;
	rect.top = 0;
	rect.right = 0;
	rect.bottom = 0; //initially user view rect is 0,0,0,0
	idpolyline = AddRectangularROI(&rect);
	ASSERT(idpolyline==EDGESET_VIEWING_RECT_ROI_ID);

	//add smallest COW2View selection rect (selection rect unit)
	rect.left = 0;
	rect.top = 0;
	rect.right = 0;
	rect.bottom = 0; //initially world rect is 0,0,0,0
	idpolyline = AddRectangularROI(&rect);
	ASSERT(idpolyline==EDGESET_SELECTION_RECT_ROI_ID);
}

COWRoiset::~COWRoiset()
{
}

void COWRoiset::DefaultInitialization()
{
	//no allocation, the polyline list is already in the polyline base class
	m_pListROI = &m_ListPOLYLINE;

	//single roi selection support for now
	m_idroi_selected = EDGESET_VIEWING_RECT_ROI_ID;
	return;
}

BOOL COWRoiset::ReadAllValuesFrom(FILE* pFILE)
{
	COWEdgeset::ReadAllValuesFrom(pFILE);
	fread(&m_idroi_selected, sizeof(int), 1, pFILE);
	return TRUE;
}

BOOL COWRoiset::WriteAllValuesTo(FILE* pFILE)
{
	COWEdgeset::WriteAllValuesTo(pFILE);
	fwrite(&m_idroi_selected, sizeof(int), 1, pFILE);
	return TRUE;
}

int COWRoiset::AddRectangularROI(RECT* pRect)
{
	if(pRect==NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	int pidpoint[4];
	//add the 4 points
	pidpoint[0] = AddPoint(	pRect->left,
							pRect->top); 
	pidpoint[1] = AddPoint(	pRect->right,
							pRect->top); 
	pidpoint[2] = AddPoint(	pRect->right,
							pRect->bottom); 
	pidpoint[3] = AddPoint(	pRect->left,
							pRect->bottom); 

	//add polyline
	int idpolyline = AddPolyline(pidpoint, 4, pidpoint[0], pRect, EDGESET_SHAPETYPE_RECTANGLE);	
	return idpolyline;

}

int COWRoiset::AddRectangularROI(double fX_left, 
								 double fY_top, 
								 double fX_right, 
								 double fY_bottom)
{
	int pidpoint[4];
	//add the 4 points
	pidpoint[0] = AddPoint(	fX_left,
							fY_top); 
	pidpoint[1] = AddPoint(	fX_right,
							fY_top); 
	pidpoint[2] = AddPoint(	fX_right,
							fY_bottom); 
	pidpoint[3] = AddPoint(	fX_left,
							fY_bottom); 

	//add polyline
	RECT rect;
	rect.left = (int)(fX_left-0.5);
	rect.top = (int)(fY_top-0.5);
	rect.right = (int)(fX_right+0.5);
	rect.bottom = (int)(fY_bottom+0.5);

	int idpolyline = AddPolyline(pidpoint, 4, pidpoint[0], &rect, EDGESET_SHAPETYPE_RECTANGLE);	
	return idpolyline;
}

//adds a one line segment to non-existing polylinear roi
//use AddLineToPolylinearROI() to add subsequent line segments to existing polylinear roi
int COWRoiset::AddPolylinearROI(RECT* pRect, int iDX, int iDY)
{
	if(pRect->left==pRect->right && pRect->top==pRect->bottom)
	{
		//ASSERT(FALSE);
		return -1;
	}

	int pidpoint[2];
	//add the 2 points
	pidpoint[0] = AddPoint(	pRect->left, pRect->top, iDX, iDY); 
	pidpoint[1] = AddPoint(	pRect->right, pRect->bottom, iDX, iDY); 

	//add polyline
	int idpolyline = AddPolyline(pidpoint, 2, pidpoint[1], pRect, EDGESET_SHAPETYPE_LINE);	
	return idpolyline;

}

int COWRoiset::AddLineToPolylinearROI(int idroi, RECT* pRect, int iDX, int iDY)
{
	if(pRect!=NULL && (pRect->left==pRect->right && pRect->top==pRect->bottom) )
	{
		//ASSERT(FALSE);
		return -1;
	}

	int idpolyline = idroi;

	//given the 2 points (left, top) and (right, bottom), 
	//the first one (left, top) should already exist (point terminating idpolyline)
	return AddLineToPolyline(idpolyline, pRect, iDX, iDY);
}

BOOL COWRoiset::DeleteROI(int idroi)
{
	int idpolyline = idroi;
	ASSERT(idroi>3); //only allow to delete user roi, first 4 being reference by their index
	return DeletePolyline(idpolyline);
}

int COWRoiset::GetNumberOfUserROI()
{
	int nDefaultROI = 4;
	int nUserROI = 0;
	int nTotalROI = GetNumberOfROI();
	nUserROI = nTotalROI - nDefaultROI;

	return nUserROI;
}

int COWRoiset::GetNumberOfROI()
{
	return GetNumberOfPolyline();
}

int COWRoiset::ModifyRectangularROI(int idroi, RECT* pRect)
{
	int idpolyline = idroi;
	POLYLINE* pPOLYLINE = FindPolyline(idpolyline);
	if(pPOLYLINE==NULL)
	{	
		ASSERT(FALSE);
		return -1;
	}
	if(pPOLYLINE->idshape!=EDGESET_SHAPETYPE_RECTANGLE)
	{
		ASSERT(FALSE);
		return -1;
	}
	//update bounding rect
	pPOLYLINE->BoundingRect = *pRect;

	//first line segment
	int idline = pPOLYLINE->idlineL0;
	LINE* pLINE = FindLine(idline);
		//first point of first polyline line segment
		INTEGERPOINT* pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP1);
		pINTEGERPOINT->iX = pRect->left;
		pINTEGERPOINT->iY = pRect->top;
		//second point of first polyline line segment
		pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP2);
		pINTEGERPOINT->iX = pRect->right;
		pINTEGERPOINT->iY = pRect->top;
	//second line segment
	pLINE = FindLine(pLINE->idlineNL);
		/* //no need to redo this one repetitively!!! 
		//first point of second polyline line segment
		pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP1);
		pINTEGERPOINT->iX = pRect->right;
		pINTEGERPOINT->iY = ppRect->top;
		*/
		//second point of second polyline line segment
		pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP2);
		pINTEGERPOINT->iX = pRect->right;
		pINTEGERPOINT->iY = pRect->bottom;
	//third line segment
	pLINE = FindLine(pLINE->idlineNL);
		/* //no need to redo this one repetitively!!! 
		//first point of third polyline line segment
		pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP1);
		pINTEGERPOINT->iX = pRect->right;
		pINTEGERPOINT->iY = pRect->bottom;
		*/
		//second point of third polyline line segment
		pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP2);
		pINTEGERPOINT->iX = pRect->left;
		pINTEGERPOINT->iY = pRect->bottom;
	//fourth line segment
	pLINE = FindLine(pLINE->idlineNL);
		/* //no need to redo this one repetitively!!! 
		//first point of fourht polyline line segment
		pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP1);
		pINTEGERPOINT->iX = pRect->left;
		pINTEGERPOINT->iY = pRect->bottom;
		*/
		//second point of fourth polyline line segment
//debugging temp code begin
		ASSERT(pLINE->idlineNL<16);
		ASSERT(pLINE->idpointP2<16);
//debugging temp code end
		pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP2);
		pINTEGERPOINT->iX = pRect->left;
		pINTEGERPOINT->iY = pRect->top;
	return idroi; //success
}

int COWRoiset::ModifyRectangularROI(int idroi,
									double fX_left, 
									double fY_top, 
									double fX_right, 
									double fY_bottom)
{
	int idpolyline = idroi;
	POLYLINE* pPOLYLINE = FindPolyline(idpolyline);

	//update bounding rect
	CRect myRect((int)(fX_left-0.5), (int)(fY_top-0.5), (int)(fX_right+0.5), (int)(fY_bottom+0.5));
	pPOLYLINE->BoundingRect = myRect;

	//first line segment
	int idline = pPOLYLINE->idlineL0;
	LINE* pLINE = FindLine(idline);
		//first point of first polyline line segment
		FLOATPOINT* pFLOATPOINT = FindFloatPoint(pLINE->idpointP1);
		pFLOATPOINT->fX = fX_left;
		pFLOATPOINT->fY = fY_top;
		//second point of first polyline line segment
		pFLOATPOINT = FindFloatPoint(pLINE->idpointP2);
		pFLOATPOINT->fX = fX_right;
		pFLOATPOINT->fY = fY_top;
	//second line segment
	pLINE = FindLine(pLINE->idlineNL);
		/* //no need to redo this one repetitively!!! 
		//first point of second polyline line segment
		pFLOATPOINT = FindFloatPoint(pLINE->idpointP1);
		pFLOATPOINT->fX = pRect->right;
		pFLOATPOINT->fY = ppRect->top;
		*/
		//second point of second polyline line segment
		pFLOATPOINT = FindFloatPoint(pLINE->idpointP2);
		pFLOATPOINT->fX = fX_right;
		pFLOATPOINT->fY = fY_bottom;
	//third line segment
	pLINE = FindLine(pLINE->idlineNL);
		/* //no need to redo this one repetitively!!! 
		//first point of third polyline line segment
		pFLOATPOINT = FindFloatPoint(pLINE->idpointP1);
		pFLOATPOINT->fX = pRect->right;
		pFLOATPOINT->fY = pRect->bottom;
		*/
		//second point of third polyline line segment
		pFLOATPOINT = FindFloatPoint(pLINE->idpointP2);
		pFLOATPOINT->fX = fX_left;
		pFLOATPOINT->fY = fY_bottom;
	//fourth line segment
	pLINE = FindLine(pLINE->idlineNL);
		/* //no need to redo this one repetitively!!! 
		//first point of fourht polyline line segment
		pFLOATPOINT = FindFloatPoint(pLINE->idpointP1);
		pFLOATPOINT->fX = pRect->left;
		pFLOATPOINT->fY = pRect->bottom;
		*/
		//second point of fourth polyline line segment
		pFLOATPOINT = FindFloatPoint(pLINE->idpointP2);
		pFLOATPOINT->fX = fX_left;
		pFLOATPOINT->fY = fY_top;

	return idroi; //success
}

int COWRoiset::TranslateRectangularROI(int idpolyline, int iX_offset, int iY_offset)
{
	return TranslatePolyline(idpolyline, iX_offset, iY_offset);
}

int COWRoiset::TranslateRectangularROI(int idpolyline, double fX_offset, double fY_offset)
{
	return TranslatePolyline(idpolyline, fX_offset, fY_offset);
}

int COWRoiset::SelectROI(int idroi)
{
	if(idroi<0 || idroi>m_pListROI->GetCount()) return -1;
	m_idroi_selected = idroi;
	return m_idroi_selected;
}

RECT COWRoiset::GetRectROI(int idroi)
{
	RECT rect = {0,0,0,0};
	int idpolyline = idroi;
	POLYLINE* pPOLYLINE = FindPolyline(idpolyline);

	//for a general roi, non-null bounding rect implies bounding rect valid
	CRect myRect(pPOLYLINE->BoundingRect);
	if(!myRect.IsRectEmpty() )
	{
		return pPOLYLINE->BoundingRect;
	}
	else if(myRect.IsRectEmpty() && (myRect.Width()!=0 || myRect.Height()!=0) )
	{
		return pPOLYLINE->BoundingRect;
	}
	else
	{
		myRect.IsRectEmpty() ;
		ASSERT(FALSE);
	}
	//for rectangular roi
	if(pPOLYLINE->idshape==EDGESET_SHAPETYPE_RECTANGLE)
	{
		INTEGERPOINT* pINTEGERPOINT = NULL;
		FLOATPOINT* pFLOATPOINT = NULL;

		//first line segment
		int idline = pPOLYLINE->idlineL0;
		LINE* pLINE = FindLine(idline);
		if(m_bINTEGERPOINT) 
		{
			//first point of first polyline line segment
			pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP1);
			rect.left = pINTEGERPOINT->iX;
			rect.top = pINTEGERPOINT->iY;
			//second point of first polyline line segment
			pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP2);
			rect.right = pINTEGERPOINT->iX;
			ASSERT(rect.top == pINTEGERPOINT->iY);

			//second line segment
			pLINE = FindLine(pLINE->idlineNL);
			//second point of second polyline line segment
			pINTEGERPOINT = FindIntegerPoint(pLINE->idpointP2);
			ASSERT(rect.right == pINTEGERPOINT->iX);
			rect.bottom = pINTEGERPOINT->iY;

		}
		else if(m_bFLOATPOINT) 
		{
			//first point of first polyline line segment
			pFLOATPOINT = FindFloatPoint(pLINE->idpointP1);
			rect.left = (int)pFLOATPOINT->fX;
			rect.top = (int)pFLOATPOINT->fY;
			//second point of first polyline line segment
			pFLOATPOINT = FindFloatPoint(pLINE->idpointP2);
			rect.right = (int)pFLOATPOINT->fX;
			ASSERT(rect.top == (int)pFLOATPOINT->fY);

			//second line segment
			pLINE = FindLine(pLINE->idlineNL);
			//second point of second polyline line segment
			pFLOATPOINT = FindFloatPoint(pLINE->idpointP2);
			ASSERT(rect.right == (int)pFLOATPOINT->fX);
			rect.bottom = (int)pFLOATPOINT->fY;
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	return rect;
}

//capture roi
int COWRoiset::IsTouchingROI(CPoint pt)
{
	return COWEdgeset::IsTouchingPolylineBoundingRect(pt);
}

//draw roi
void COWRoiset::DrawUserROI(COW2View* pOW2View, CDC* pDC, int idroi, int iselected) //idroi=ROISET_ALL)
{
	if(pOW2View==NULL || pDC==NULL) return;

	POLYLINE* pPOLYLINE = NULL;
	int idpolyline_found = -1;
	POSITION pos = m_ListPOLYLINE.GetHeadPosition();
	while(pos!=NULL)
	{
		idpolyline_found++;
		pPOLYLINE = (POLYLINE*)m_ListPOLYLINE.GetNext(pos);
		//skip the first 4 default roi (largest and smallest world rect, view rect and moving selection rect)
		if(idpolyline_found>3)
		{
			if(idroi==ROISET_ALL || idroi==idpolyline_found)
			{
				if(pPOLYLINE->idshape==EDGESET_SHAPETYPE_RECTANGLE)
				{
					DrawRectangularROI(pOW2View, pDC, pPOLYLINE, iselected); //idroi=ROISET_ALL)
				}
				else if(pPOLYLINE->idshape==EDGESET_SHAPETYPE_LINE)
				{
					DrawPolylinearROI(pOW2View, pDC, pPOLYLINE, iselected); //idroi=ROISET_ALL)
				}
				else
				{
					//implement further
					ASSERT(FALSE);
				}
			}
		}
	}
	return;
}

void COWRoiset::DrawRectangularROI(COW2View* pOW2View, 
								   CDC* pDC, 
								   POLYLINE* pPOLYLINE, 
								   int iselected) //idroi=ROISET_ALL)
{
	int iborders = TRUE;
	DrawBoundingRectROI(pOW2View, pDC, pPOLYLINE, iselected, iborders);
	return;
}

void COWRoiset::DrawPolylinearROI(COW2View* pOW2View, 
								  CDC* pDC, 
								  POLYLINE* pPOLYLINE, 
								  int iselected) //idroi=ROISET_ALL)
{
	const int npoint_max = 1000;
	POINT pPOINT[npoint_max];
	POINT pDeltaPOINT[npoint_max];
	//collect all polyline's points
	int npoints = GetPolylinePoints(pPOLYLINE, pPOINT, npoint_max, 0, pDeltaPOINT);

	/*
	#ifdef _DEBUG
		CLogDocument* pLog = ((COIIIApp*)AfxGetApp())->GetLogDocument();
		if(pLog!=NULL)
		{
			pLog->AddText("GetPolylinePoints(), begin\r\n");
			for(int i=0; i<npoints; i++)
			{
				CString myString;
				myString.Format("point: (%d, %d)\r\n", pPOINT[i].x, pPOINT[i].y);
				pLog->AddText(myString);
			}
			pLog->AddText("GetPolylinePoints(), end\r\n\r\n");
		}
	#endif //_DEBUG
	*/

	//check if non-zero delta
	BOOL bSomeNonZero = FALSE;
	BOOL bAllIdentical = TRUE;
	int iDX = pDeltaPOINT[0].x;
	int iDY = pDeltaPOINT[0].y;
	for(int i=0; i<npoints; i++)
	{
		if(pDeltaPOINT[i].x!=0 || pDeltaPOINT[i].y!=0)
		{
			bSomeNonZero = TRUE;
		}
		if(pDeltaPOINT[i].x!=iDX || pDeltaPOINT[i].y!=iDY)
		{
			bAllIdentical = FALSE;
			if(pDeltaPOINT[i].x>iDX) iDX = pDeltaPOINT[i].x;
			if(pDeltaPOINT[i].y>iDY) iDY = pDeltaPOINT[i].y;
		}
	}

	//draw polyline's line segments, no width
	pDC->Polyline(pPOINT, npoints);
	if(bSomeNonZero==TRUE)
	{
		//draw end points of each polyline's line segments, as a rect
		if(bAllIdentical==TRUE)
		{
			for(int i=0; i<npoints; i++)
			{
				//draw end points
				pDC->Rectangle(pPOINT[i].x, pPOINT[i].y, pPOINT[i].x + iDX, pPOINT[i].y + iDY);
			
				//if selected, draw all intermediate grid points
				if(iselected!=ROISET_DRAWAS_NOTSELECTED)
				{
					if(i<npoints-1)
					{
						const int npoint_max = 1000;
						POINT pCollectedPOINT[npoint_max];
						int npoints_collected = EdgeFill(pPOINT[i], pPOINT[i+1], pCollectedPOINT, npoint_max, 1);
						for(int j=1; j<npoints_collected-1; j++) //boundaries to exclude P1 and P2, already drawn in the for(i=0 i++ loop
						{
							pDC->Rectangle(pCollectedPOINT[j].x, pCollectedPOINT[j].y, pCollectedPOINT[j].x + iDX, pCollectedPOINT[j].y + iDY);
						}
					}
				}

			}
		}
		else
		{
			for(int i=0; i<npoints; i++)
			{
				//draw end points
				pDC->Rectangle(pPOINT[i].x, pPOINT[i].y, pPOINT[i].x + pDeltaPOINT[i].x, pPOINT[i].y + pDeltaPOINT[i].y);

				//if selected, draw all intermediate grid points
				if(iselected!=ROISET_DRAWAS_NOTSELECTED)
				{
					if(i<npoints-1)
					{
						const int npoint_max = 1000;
						POINT pCollectedPOINT[npoint_max];
						int npoints_collected = EdgeFill(pPOINT[i], pPOINT[i+1], pCollectedPOINT, npoint_max, 1);
						for(int j=1; j<npoints_collected-1; j++) //boundaries to exclude P1 and P2, already drawn in the for(i=0 i++ loop
						{
							pDC->Rectangle(pCollectedPOINT[j].x, pCollectedPOINT[j].y, pCollectedPOINT[j].x + iDX, pCollectedPOINT[j].y + iDY);
						}
					}
				}
			}
		}
	}

	//draw polyline's bounding box
	int iborders = FALSE;
	DrawBoundingRectROI(pOW2View, pDC, pPOLYLINE, iselected, iborders);		
	return;
}

void COWRoiset::DrawBoundingRectROI(COW2View* pOW2View, 
									CDC* pDC, 
									POLYLINE* pPOLYLINE, 
									int iselected,
									int iborders)
									
{
	if(pOW2View==NULL || pDC==NULL || pPOLYLINE==NULL)
	{
		ASSERT(FALSE);
		return;
	}

	CPoint pPoint[5];

	CRect myRect(pPOLYLINE->BoundingRect);
	//ASSERT(!myRect.IsRectEmpty()); //no bounding rect specified for this polyline
	ASSERT(myRect.Width()>0 || myRect.Height()>0);
	POINT2DD myPoint2DD[4]; //rectangle
	myPoint2DD[0].x = myRect.left;
	myPoint2DD[0].y = myRect.top;
	myPoint2DD[1].x = myRect.right;
	myPoint2DD[1].y = myRect.top;
	myPoint2DD[2].x = myRect.right;
	myPoint2DD[2].y = myRect.bottom;
	myPoint2DD[3].x = myRect.left;
	myPoint2DD[3].y = myRect.bottom;
	int npoints = 4; //rectangular, 4 corners
	for(int i=0; i<npoints; i++)
	{
		//pPoint[i] = pOW2View->MapPointFromWorldToView( myPoint2DD[i]);
		pPoint[i].x = (int)myPoint2DD[i].x;
		pPoint[i].y = (int)myPoint2DD[i].y;
	}
	// To close the polygon, the last point is equal to the first point 
	pPoint[npoints].x = pPoint[0].x;
	pPoint[npoints].y = pPoint[0].y;

	//draw border (bounding rect)
	if(iborders)
	{
		//draw the polygon
		pDC->Polygon(pPoint, npoints+1); //+1 for the last point 
		//draw the polyline
		//pDC->Polyline(pPoint, npoints+1); //+1 for the last point 
	}

	//for the selected roi
	if(iselected==ROISET_DRAWAS_SELECTED)
	{
		//poirier, oct 2001, begin
		/*
		int iwidth_point = 2; //2 when zoom 100%
		int iwidth_point = 1; //1 when zoom 400% and up
		*/
		
		CPoint pt0(0,0);
		CPoint pt1(1,1);
		pOW2View->OnPrepareDC(pDC);
		pDC->LPtoDP(&pt0);
		pDC->LPtoDP(&pt1);
		CSize devsizeOneLogicalUnit(pt1.x-pt0.x, pt1.y-pt0.y);
		//int iwidth_point = 4 - (int)(sqrt(devsizeOneLogicalUnit.cx)+1.0);
		//if(iwidth_point<0) iwidth_point = 0;
		int iwidth_point = 1;
		if(devsizeOneLogicalUnit.cx<=2)
		{
			iwidth_point = 2;
		}
		else if(devsizeOneLogicalUnit.cx<4)
		{
			iwidth_point = 1;
		}
		else 
		{
			iwidth_point = 0;
		}
		//poirier, oct 2001, end

		//white pen
	    CPen myPen;
		myPen.CreatePen(PS_SOLID, 0, RGB(255,255,255)); 
		CPen* pOldPen = pDC->SelectObject(&myPen);

		//black brush
		CBrush myBrush;
		CBrush* pOldBrush = NULL;
		int nOldROP2Mode = 0;
		if(devsizeOneLogicalUnit.cx<30) 
		{
			myBrush.CreateStockObject(BLACK_BRUSH);
			pOldBrush = pDC->SelectObject(&myBrush);
		}
		else
		{
			//myBrush.CreateStockObject(NULL_BRUSH);
			nOldROP2Mode = pDC->SetROP2(R2_MASKPEN);
			myBrush.CreateHatchBrush(HS_DIAGCROSS, RGB(0,0,0)); //HS_DIAGCROSS, HS_CROSS, HS_VERTICAL   
			pOldBrush = pDC->SelectObject(&myBrush);
		}

		//highlight roi corners
		RECT pRECT[2];
		for(int j=0; j<npoints; j++)
		{
			//corner
			pRECT[0].left = pPoint[j].x - iwidth_point;
			pRECT[0].top = pPoint[j].y - iwidth_point;
			pRECT[0].right = pPoint[j].x + 1 + iwidth_point;
			pRECT[0].bottom = pPoint[j].y + 1 + iwidth_point;
			//pDC->FillRect(&pRECT[0], &myBrush);
			if(devsizeOneLogicalUnit.cx<30) pDC->FillRect(&pRECT[0], &myBrush);
			  else pDC->Rectangle(&pRECT[0]);

			if( abs(pPoint[j+1].x - pPoint[j].x)>5 || abs(pPoint[j+1].y - pPoint[j].y)>5)
			{
				//mid-segment, only if enough space
				pRECT[1].left = (pPoint[j].x + pPoint[j+1].x)/2 - iwidth_point;
				pRECT[1].top = (pPoint[j].y + pPoint[j+1].y)/2 - iwidth_point;
				pRECT[1].right = (pPoint[j].x + pPoint[j+1].x)/2 + 1 + iwidth_point;
				pRECT[1].bottom = (pPoint[j].y + pPoint[j+1].y)/2 + 1 + iwidth_point;
				//pDC->FillRect(&pRECT[1], &myBrush);
				if(devsizeOneLogicalUnit.cx<30) pDC->FillRect(&pRECT[1], &myBrush);
				  else pDC->Rectangle(&pRECT[1]);
			}
		}
		if(nOldROP2Mode) pDC->SetROP2(nOldROP2Mode);
		if(pOldBrush) pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		myBrush.DeleteObject();
		myPen.DeleteObject();
	}
	return;
}



//////////////////////////////////////////////////////////////////////////////////////
//class COWMapobjectset
//
//map objects vectorial overlay, user defined with mouse tools, mouse moves and clicks 
//////////////////////////////////////////////////////////////////////////////////////

COWMapobjectset::COWMapobjectset()
{
	DefaultInitialization();
}

COWMapobjectset::~COWMapobjectset()
{
}

void COWMapobjectset::DefaultInitialization()
{
	return;
}


//////////////////////////////////////////////////////////////////////////////////////
//class COWImagefeatureset
//
//image features vectorial overlay, computer defined with oiii analysis functionality 
//////////////////////////////////////////////////////////////////////////////////////
COWImagefeatureset::COWImagefeatureset()
{
	DefaultInitialization();
}

COWImagefeatureset::~COWImagefeatureset()
{
}

void COWImagefeatureset::DefaultInitialization()
{
	return;
}


//////////////////////////////////////////////////////////////////////////////////////
//class COWFusionset
//
//computer defined with oiii segmentation functionality 
//////////////////////////////////////////////////////////////////////////////////////
COWFusionset::COWFusionset()
{
	DefaultInitialization();
}

COWFusionset::~COWFusionset()
{
}

void COWFusionset::DefaultInitialization()
{
	return;
}

//////////////////////////////////////////////////////////////////////////////////////
//class COWPartitionset
//
//computer defined with oiii segmentation functionality 
//////////////////////////////////////////////////////////////////////////////////////
COWPartitionset::COWPartitionset()
{
	DefaultInitialization();
}

COWPartitionset::~COWPartitionset()
{
}

void COWPartitionset::DefaultInitialization()
{
	return;
}


/////////////////////////////////////////////////////////////////////////////////////////////
//class COWPolygonset
//
/////////////////////////////////////////////////////////////////////////////////////////////
COWPolygonset::COWPolygonset()
{
	DefaultInitialization();

	//set edgeset to use internal integer point list
	m_bINTEGERPOINT = TRUE;
	m_bFLOATPOINT = FALSE;
	m_pPointset = NULL;
	m_bDual = FALSE;
}

COWPolygonset::~COWPolygonset()
{
}

void COWPolygonset::DefaultInitialization()
{
	//no allocation, the polyline list is already in the polyline base class
	m_pListPolygon = &m_ListPOLYLINE;
	return;
}

int COWPolygonset::AddPolygon(POINT* pPOINT, int nCount)
{
	int idpolygon = -1;
	int p_idpoint[2048]; //max count
	if (nCount > 2048) return idpolygon;

	//-1) ignore last point if last point equals first point
	if ( (pPOINT[nCount-1].x == pPOINT[0].x) && (pPOINT[nCount-1].y == pPOINT[0].y))
	{
		nCount--;
	}
	if (nCount < 3) return idpolygon;

	//0) compute bounding rect
	int xmin = INT_MAX; //min x
	int ymin = INT_MAX; //min y
	int xmax = INT_MIN; //max x
	int ymax = INT_MIN; //max y
	for (int i = 0; i < nCount; i++)
	{
		if (pPOINT[i].x < xmin) xmin = pPOINT[i].x;
		if (pPOINT[i].y < ymin) ymin = pPOINT[i].y;
		if (pPOINT[i].x > xmax) xmax = pPOINT[i].x;
		if (pPOINT[i].y > ymax) ymax = pPOINT[i].y;
		//1) add each integer point and store each returned point id
		p_idpoint[i] = AddPoint(pPOINT[i].x, pPOINT[i].y);

	}
	RECT boundingRECT;
	boundingRECT.left = xmin; //min x
	boundingRECT.top = ymin; //min y
	boundingRECT.right = xmax; //max x
	boundingRECT.bottom = ymax; //max y

	//2) to add 1 chain of line segments, returns polyline id
	idpolygon = AddPolyline(p_idpoint, nCount, p_idpoint[0], &boundingRECT, EDGESET_SHAPETYPE_POLYGON, -1);

	return idpolygon;
}

int COWPolygonset::GetNumberOfPolygon()
{
	return GetNumberOfPolyline();
}

void COWPolygonset::DoScanlineFillForPolygon(int idpolygon, class COW2View* pOW2View, class fipWinImage* pfipWinImage, int inverseprob_onpixelfill, //)
//2020july04, spi, begin
int voronoisetedgesfilterflag, int voronoisetpointsfilterflag, class COW2Doc* pAlternateColorsOW2Doc, int voronoisetedgesmapperflag, int voronoisetpointsmapperflag)
//2020july04, spi, end
{
	POLYLINE* pPOLYLINE = FindPolyline(idpolygon);
	if (pPOLYLINE)
	{
		xmin = pPOLYLINE->BoundingRect.left;
		ymin = pPOLYLINE->BoundingRect.top;
		xmax = pPOLYLINE->BoundingRect.right;
		ymax = pPOLYLINE->BoundingRect.bottom;
		v = GetPolylinePoints(pPOLYLINE, p, maxpointsperpolygon);
		if (v > 2 && v < maxpointsperpolygon)
		{
			p[v].x = p[0].x;
			p[v].y = p[0].y;

			float s = ymin + 0.01;
			while (s<=ymax)
			{
				ints(s);
				sort(s, pOW2View, pfipWinImage, inverseprob_onpixelfill, //);
				//2020july04, spi, begin
				voronoisetedgesfilterflag, voronoisetpointsfilterflag, pAlternateColorsOW2Doc, voronoisetedgesmapperflag, voronoisetpointsmapperflag);
				//2020july04, spi, end
				s++;
			}
		}
	}
	return;
}

void COWPolygonset::DoScanlineFillForPolygon(POINT* pPOINT, int nCount, class COW2View* pOW2View, class fipWinImage* pfipWinImage, int inverseprob_onpixelfill, //)
//2020july04, spi, begin
int voronoisetedgesfilterflag, int voronoisetpointsfilterflag, class COW2Doc* pAlternateColorsOW2Doc, int voronoisetedgesmapperflag, int voronoisetpointsmapperflag)
//2020july04, spi, end
{
	if (nCount > maxpointsperpolygon - 1) return;

	//0) ignore last point if last point equals first point
	if ((pPOINT[nCount - 1].x == pPOINT[0].x) && (pPOINT[nCount - 1].y == pPOINT[0].y))
	{
		nCount--;
	}
	if (nCount < 3) return;

	//1) compute bounding rect
	xmin = INT_MAX; //min x
	ymin = INT_MAX; //min y
	xmax = INT_MIN; //max x
	ymax = INT_MIN; //max y
	for (int i = 0; i < nCount; i++)
	{
		if (pPOINT[i].x < xmin) xmin = pPOINT[i].x;
		if (pPOINT[i].y < ymin) ymin = pPOINT[i].y;
		if (pPOINT[i].x > xmax) xmax = pPOINT[i].x;
		if (pPOINT[i].y > ymax) ymax = pPOINT[i].y;
		//2) copy points
		p[i].x = pPOINT[i].x;
		p[i].y = pPOINT[i].y;

	}
	v = nCount;
	//3) ensure polygon is closed
	p[v].x = p[0].x;
	p[v].y = p[0].y;

	//4) do the scan line fill
	float s = ymin + 0.01;
	while (s <= ymax)
	{
		ints(s);
		sort(s, pOW2View, pfipWinImage, inverseprob_onpixelfill, //);
		//2020july04, spi, begin
		voronoisetedgesfilterflag, voronoisetpointsfilterflag, pAlternateColorsOW2Doc, voronoisetedgesmapperflag, voronoisetpointsmapperflag);
		//2020july04, spi, end
		s++;
	}

	return;
}

void COWPolygonset::ints(float z) //DEFINE FUNCTION INTS
{
	int x1, x2, y1, y2, temp;
	c = 0;
	for (int i = 0; i < v; i++)
	{
		x1 = p[i].x;
		y1 = p[i].y;
		x2 = p[i + 1].x;
		y2 = p[i + 1].y;
		if (y2 < y1)
		{
			temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		if (z <= y2 && z >= y1)
		{
			if ((y1 - y2) == 0)
				x = x1;
			else // used to make changes in x. so that we can fill our polygon after cerain distance
			{
				x = ((x2 - x1)*(z - y1)) / (y2 - y1);
				x = x + x1;
			}
			if (x <= xmax && x >= xmin)
				inter[c++] = x;
		}
	}
}

struct DrawLineInfipWinImage_userdata {
	fipWinImage* pfipWinImage;
	COW2Doc* pOW2Doc;
	int inverseprob_onpixelfill;
	//2020july04, spi, begin
	int voronoisetedgesfilterflag;
	int voronoisetpointsfilterflag;
	COW2Doc* pAlternateColorsOW2Doc;
	int voronoisetedgesmapperflag;
	int voronoisetpointsmapperflag;
	//2020july04, spi, end
};


VOID CALLBACK DrawLineInfipWinImage2(int X, int Y, LPARAM lpData)
{
	RGBQUAD myRGBQUAD;
	myRGBQUAD.rgbBlue = 255;
	myRGBQUAD.rgbGreen = 255;
	myRGBQUAD.rgbRed = 255;
	DrawLineInfipWinImage_userdata* puserdata;
	puserdata = (DrawLineInfipWinImage_userdata*)lpData;
	if (X > -1 && X<puserdata->pfipWinImage->getWidth() && Y>-1 && Y < puserdata->pfipWinImage->getHeight())
	{
		myRGBQUAD.rgbBlue = puserdata->pOW2Doc->m_pPixelset->GetPixelValue(X, Y, 2);
		myRGBQUAD.rgbGreen = puserdata->pOW2Doc->m_pPixelset->GetPixelValue(X, Y, 1);
		myRGBQUAD.rgbRed = puserdata->pOW2Doc->m_pPixelset->GetPixelValue(X, Y, 0);

		if (puserdata->inverseprob_onpixelfill < 2)
		{
			//always set pixel
			//2020july04, spi, begin
			//puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
			if (puserdata->voronoisetedgesfilterflag == VORONOISET_COLOR_ALL)
			{
				puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
			}
			else
			{
				bool enablecolormapper = false;
				RGBQUAD myaltRGBQUAD;
				myaltRGBQUAD.rgbBlue = 255;
				myaltRGBQUAD.rgbGreen = 255;
				myaltRGBQUAD.rgbRed = 255;
				if (puserdata->pAlternateColorsOW2Doc != NULL && puserdata->pAlternateColorsOW2Doc->m_pPixelset != NULL
					&& puserdata->voronoisetedgesmapperflag != VORONOISET_COLOR_MAP_NONE)
				{
					enablecolormapper = true;
					myaltRGBQUAD.rgbBlue = puserdata->pAlternateColorsOW2Doc->m_pPixelset->GetPixelValue(X, Y, 2);
					myaltRGBQUAD.rgbGreen = puserdata->pAlternateColorsOW2Doc->m_pPixelset->GetPixelValue(X, Y, 1);
					myaltRGBQUAD.rgbRed = puserdata->pAlternateColorsOW2Doc->m_pPixelset->GetPixelValue(X, Y, 0);
				}
				RgbColor myRgbColor;
				myRgbColor.b = myRGBQUAD.rgbBlue;
				myRgbColor.g = myRGBQUAD.rgbGreen;
				myRgbColor.r = myRGBQUAD.rgbRed;
				//2020july16, spi, begin
				//HsvColor myHsvColor = RgbToHsv(myRgbColor);
				HsvColor myHsvColor;
				if (puserdata->pOW2Doc->m_pPixelset->IsHSV())
				{
					myHsvColor.h = puserdata->pOW2Doc->m_pPixelset->GetPixelValue(X, Y, 3);
					myHsvColor.s = puserdata->pOW2Doc->m_pPixelset->GetPixelValue(X, Y, 4);
					myHsvColor.v = puserdata->pOW2Doc->m_pPixelset->GetPixelValue(X, Y, 5);
				}
				else
				{
					myHsvColor = RgbToHsv(myRgbColor);
				}
				//2020july16, spi, end

				if (puserdata->voronoisetpointsfilterflag & VORONOISET_COLOR_HUMANSKIN)
				{
					if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_HUMANSKIN))
					{
						if (enablecolormapper == false)
						{
							puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
						}
						else if (puserdata->voronoisetpointsmapperflag & VORONOISET_COLOR_MAP_HUMANSKIN)
						{
							puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
						}
					}
				}
				if (puserdata->voronoisetpointsfilterflag & VORONOISET_COLOR_RED)
				{
					if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_RED))
					{
						if (enablecolormapper == false)
						{
							puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
						}
						else if (puserdata->voronoisetpointsmapperflag & VORONOISET_COLOR_MAP_RED)
						{
							puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
						}
					}
				}
				if (puserdata->voronoisetpointsfilterflag & VORONOISET_COLOR_GREEN)
				{
					if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_GREEN))
					{
						if (enablecolormapper == false)
						{
							puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
						}
						else if (puserdata->voronoisetpointsmapperflag & VORONOISET_COLOR_MAP_GREEN)
						{
							puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
						}
					}
				}
				if (puserdata->voronoisetpointsfilterflag & VORONOISET_COLOR_BLUE)
				{
					if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_BLUE))
					{
						if (enablecolormapper == false)
						{
							puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
						}
						else if (puserdata->voronoisetpointsmapperflag & VORONOISET_COLOR_MAP_BLUE)
						{
							puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
						}
					}
				}
				if (puserdata->voronoisetpointsfilterflag & VORONOISET_COLOR_YELLOW)
				{
					if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_YELLOW))
					{
						if (enablecolormapper == false)
						{
							puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
						}
						else if (puserdata->voronoisetpointsmapperflag & VORONOISET_COLOR_MAP_YELLOW)
						{
							puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
						}
					}
				}
				if (puserdata->voronoisetpointsfilterflag & VORONOISET_COLOR_ORANGE)
				{
					if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_ORANGE))
					{
						if (enablecolormapper == false)
						{
							puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
						}
						else if (puserdata->voronoisetpointsmapperflag & VORONOISET_COLOR_MAP_ORANGE)
						{
							puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
						}
					}
				}
			}
			//2020july04, spi, end
		}
		else
		{
			//set pixel a fraction of the time (according to inverse prob)
			int dice = random_int(1, puserdata->inverseprob_onpixelfill);
			if (dice == 1)
			{
				//2020july04, spi, begin
				//puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
				if (puserdata->voronoisetedgesfilterflag == VORONOISET_COLOR_ALL)
				{
					puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
				}
				else
				{
					bool enablecolormapper = false;
					RGBQUAD myaltRGBQUAD;
					myaltRGBQUAD.rgbBlue = 255;
					myaltRGBQUAD.rgbGreen = 255;
					myaltRGBQUAD.rgbRed = 255;
					if (puserdata->pAlternateColorsOW2Doc != NULL && puserdata->pAlternateColorsOW2Doc->m_pPixelset != NULL
						&& puserdata->voronoisetedgesmapperflag != VORONOISET_COLOR_MAP_NONE)
					{
						enablecolormapper = true;
						myaltRGBQUAD.rgbBlue = puserdata->pAlternateColorsOW2Doc->m_pPixelset->GetPixelValue(X, Y, 2);
						myaltRGBQUAD.rgbGreen = puserdata->pAlternateColorsOW2Doc->m_pPixelset->GetPixelValue(X, Y, 1);
						myaltRGBQUAD.rgbRed = puserdata->pAlternateColorsOW2Doc->m_pPixelset->GetPixelValue(X, Y, 0);
					}
					RgbColor myRgbColor;
					myRgbColor.b = myRGBQUAD.rgbBlue;
					myRgbColor.g = myRGBQUAD.rgbGreen;
					myRgbColor.r = myRGBQUAD.rgbRed;
					//2020july16, spi, begin
					//HsvColor myHsvColor = RgbToHsv(myRgbColor);
					HsvColor myHsvColor;
					if (puserdata->pOW2Doc->m_pPixelset->IsHSV())
					{
						myHsvColor.h = puserdata->pOW2Doc->m_pPixelset->GetPixelValue(X, Y, 3);
						myHsvColor.s = puserdata->pOW2Doc->m_pPixelset->GetPixelValue(X, Y, 4);
						myHsvColor.v = puserdata->pOW2Doc->m_pPixelset->GetPixelValue(X, Y, 5);
					}
					else
					{
						myHsvColor = RgbToHsv(myRgbColor);
					}
					//2020july16, spi, end

					if (puserdata->voronoisetpointsfilterflag & VORONOISET_COLOR_HUMANSKIN)
					{
						if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_HUMANSKIN))
						{
							if (enablecolormapper == false)
							{
								puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
							}
							else if (puserdata->voronoisetpointsmapperflag & VORONOISET_COLOR_MAP_HUMANSKIN)
							{
								puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
							}
						}
					}
					if (puserdata->voronoisetpointsfilterflag & VORONOISET_COLOR_RED)
					{
						if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_RED))
						{
							if (enablecolormapper == false)
							{
								puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
							}
							else if (puserdata->voronoisetpointsmapperflag & VORONOISET_COLOR_MAP_RED)
							{
								puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
							}
						}
					}
					if (puserdata->voronoisetpointsfilterflag & VORONOISET_COLOR_GREEN)
					{
						if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_GREEN))
						{
							if (enablecolormapper == false)
							{
								puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
							}
							else if (puserdata->voronoisetpointsmapperflag & VORONOISET_COLOR_MAP_GREEN)
							{
								puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
							}
						}
					}
					if (puserdata->voronoisetpointsfilterflag & VORONOISET_COLOR_BLUE)
					{
						if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_BLUE))
						{
							if (enablecolormapper == false)
							{
								puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
							}
							else if (puserdata->voronoisetpointsmapperflag & VORONOISET_COLOR_MAP_BLUE)
							{
								puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
							}
						}
					}
					if (puserdata->voronoisetpointsfilterflag & VORONOISET_COLOR_YELLOW)
					{
						if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_YELLOW))
						{
							if (enablecolormapper == false)
							{
								puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
							}
							else if (puserdata->voronoisetpointsmapperflag & VORONOISET_COLOR_MAP_YELLOW)
							{
								puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
							}
						}
					}
					if (puserdata->voronoisetpointsfilterflag & VORONOISET_COLOR_ORANGE)
					{
						if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_ORANGE))
						{
							if (enablecolormapper == false)
							{
								puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
							}
							else if (puserdata->voronoisetpointsmapperflag & VORONOISET_COLOR_MAP_ORANGE)
							{
								puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
							}
						}
					}
				}
				//2020july04, spi, end
			}
		}
	}
}

void COWPolygonset::sort(int z, class COW2View* pOW2View, class fipWinImage* pfipWinImage, int inverseprob_onpixelfill, //) //SORT FUNCTION
//2020july04, spi, begin
	int voronoisetedgesfilterflag, int voronoisetpointsfilterflag, class COW2Doc* pAlternateColorsOW2Doc, int voronoisetedgesmapperflag, int voronoisetpointsmapperflag)
//2020july04, spi, end
{
	int temp, j, i;
	/*
	for (i = 0; i < v; i++)
	{
		line(p[i].x, p[i].y, p[i + 1].x, p[i + 1].y); // used to make hollow outlines of a polygon
	}
	*/
	//delay(100);
	for (i = 0; i < c; i += 2)
	{
		//delay(100);
		//line(inter[i], z, inter[i + 1], z);  // Used to fill the polygon ....
		DrawLineInfipWinImage_userdata userdata;
		userdata.pfipWinImage = pfipWinImage;
		userdata.pOW2Doc = (COW2Doc*)pOW2View->GetDocument();
		userdata.inverseprob_onpixelfill = inverseprob_onpixelfill;
		//2020july04, spi, begin
		userdata.voronoisetedgesfilterflag = voronoisetedgesfilterflag;
		userdata.voronoisetpointsfilterflag = voronoisetpointsfilterflag;
		userdata.pAlternateColorsOW2Doc = pAlternateColorsOW2Doc;
		userdata.voronoisetedgesmapperflag = voronoisetedgesmapperflag;
		userdata.voronoisetpointsmapperflag = voronoisetpointsmapperflag;
		//2020july04, spi, end
		//2020june20, spi, begin
		//::LineDDA(inter[i], z, inter[i + 1], z, (LINEDDAPROC)DrawLineInfipWinImage2, (long)&userdata);
		::LineDDA(inter[i], z, inter[i+1], z, (LINEDDAPROC)DrawLineInfipWinImage2, (LPARAM)&userdata);
		//2020june20, spi, end

	}
}
