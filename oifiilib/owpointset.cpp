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

#include "c_xyzi.h"
#include "logdoc.h"
#include "oifii_app.h"
#include "ow2doc.h"
#include "owview.h"
#include "ow2view.h"
#include "math.h"
#include "stdio.h"

#include <gl\gl.h>
#include <gl\glu.h>
#include "oifii_propsettings.h"

#include <errno.h>
#include <limits>

#include "FreeImagePlus.h"

//spi, nov 2018, begin
#include <vector>
#include "utility.h"
//spi, nov 2018, end


//2022jan04, spi, begin
POINT MapPointFromWorldToView(POINT2DD pt2DD)
{
	POINT pt;
	pt.x = pt2DD.x;
	pt.y = pt2DD.y;
	return pt;
}
POINT2DD MapPointFromViewToWorld(POINT pt)
{
	POINT2DD pt2DD;
	pt2DD.x = pt.x;
	pt2DD.y = pt.y;
	return pt2DD;
}
//2022jan04, spi, end


static int idset_next = 0;

EDGEFILLPARAM::EDGEFILLPARAM()
{
	//default -1 are mandatory, must be set by user before using the EdgeFill(), 
	//RectEdgeFill(), PolylineFill() or PolygonFill() COWPoinset member functions.
	pIDTRI = (IDTRI*)-1;			//allocated by caller to collect all intermediate points between P1 and P2 
	ntri_max = -1;				//maximum number of IDTRI allocated by caller
	p_ntrifound = (int*)-1;	
	p_itriseed = (int*)-1;		
	pIDVERTEX = (IDVERTEX*)-1;		//allocated by caller to collect all intermediate points between P1 and P2 
	nvertex_max = -1;			//maximum number of IDVERTEX allocated by caller
	p_nvertexfound = (int*)-1;
	
	pIDVERTEX2 = NULL;		//allocated by caller to collect all intermediate points between P1 and P2
	nvertex_max2 = 0;		//maximum number of IDVERTEX allocated by caller
	p_nvertexfound2 = NULL;

	pIDTRI_ie = NULL;		//idtri inner edge
	ntri_ie_max = 0;			//idtri inner edge
	p_ntrifound_ie=NULL;	//idtri inner edge
	pIDVERTEX_ie=NULL;//idvertex inner edge
	nvertex_ie_max = 0;		//idvertex inner edge
	p_nvertexfound_ie = NULL;//idvertex inner edge
	pIDTRI_oe = NULL;		//idtri outter edge
	ntri_oe_max = 0;			//idtri outter edge
	p_ntrifound_oe = NULL;	//idtri outter edge
	pIDVERTEX_oe = NULL;//idvertex outter edge
	nvertex_oe_max = 0;		//idvertex outter edge
	p_nvertexfound_oe = NULL;//idvertex outter edge
	pIDTRI_iebr = NULL;		//idtri inner edge to bounding rect limits
	ntri_iebr_max = 0;			//idtri inner edge to bounding rect limits
	p_ntrifound_iebr = NULL;		//idtri inner edge to bounding rect limits
	pIDVERTEX_iebr = NULL;	//idvertex inner edge to bounding rect limits
	nvertex_iebr_max = 0;			//idvertex inner edge to bounding rect limits
	p_nvertexfound_iebr = NULL;	//idvertex inner edge to bounding rect limits
	pIDTRI_oebr = NULL;		//idtri outter edge to bounding rect limits
	ntri_oebr_max = 0;			//idtri outter edge to bounding rect limits
	p_ntrifound_oebr = NULL;		//idtri outter edge to bounding rect limits
	pIDVERTEX_oebr=NULL,	//idvertex outter edge to bounding rect limits
	nvertex_oebr_max = 0;			//idvertex outter edge to bounding rect limits
	p_nvertexfound_oebr = NULL;	//idvertex outter edge to bounding rect limits
}



///////////////////////////////////////////////////////////////////////////
//class COWSet
//
//this is the 'would be' based class of pixelset, pointset, segmentset and 
//edgeset classes.
///////////////////////////////////////////////////////////////////////////

COWSet::COWSet()
{
	DefaultInitialization();
}

COWSet::~COWSet()
{
}

void COWSet::DefaultInitialization()
{
	//id of this set
	m_idset = idset_next;
	idset_next++; //to be ready for the next COWSet derived class to instanciate

	//type of 2D world set	 
	m_idOW2type = OW2TYPE_OWSET;
	//euclidian bounding cube
	m_euclidianCUBE2DD.left = 0.0;
	m_euclidianCUBE2DD.top = 0.0;
	m_euclidianCUBE2DD.right = 0.0;
	m_euclidianCUBE2DD.bottom = 0.0;
	m_euclidianCUBE2DD.neardepth = 0.0;
	m_euclidianCUBE2DD.fardepth = 0.0;
	//latlong center coordinate (bounding rect center)
	m_latlong_centerPOINT2DD.x = 0.0;
	m_latlong_centerPOINT2DD.y = 0.0;
	//latlong projection
	m_latlong_projCUBE2DD.left = 0.0;
	m_latlong_projCUBE2DD.top = 0.0;
	m_latlong_projCUBE2DD.right = 0.0;
	m_latlong_projCUBE2DD.bottom = 0.0;
	m_latlong_projCUBE2DD.neardepth = 0.0;
	m_latlong_projCUBE2DD.fardepth = 0.0;
	//sky model id and double triplet for sun angle: azimuth, zenith, time
	m_idskymodel = 0;
	m_skymodelCUBE2DD.left = 0.0;
	m_skymodelCUBE2DD.top = 0.0;
	m_skymodelCUBE2DD.right = 0.0;
	m_skymodelCUBE2DD.bottom = 0.0;
	m_skymodelCUBE2DD.neardepth = 0.0;
	m_skymodelCUBE2DD.fardepth = 0.0;
	//ground dem model id
	m_idgrounddemmodel = 0;
	m_grounddemmodelCUBE2DD.left = 0.0;
	m_grounddemmodelCUBE2DD.top = 0.0;
	m_grounddemmodelCUBE2DD.right = 0.0;
	m_grounddemmodelCUBE2DD.bottom = 0.0;
	m_grounddemmodelCUBE2DD.neardepth = 0.0;
	m_grounddemmodelCUBE2DD.fardepth = 0.0;
	//cover dem model id
	m_idcoverdemmodel = 0;
	m_coverdemmodelCUBE2DD.left = 0.0;
	m_coverdemmodelCUBE2DD.top = 0.0;
	m_coverdemmodelCUBE2DD.right = 0.0;
	m_coverdemmodelCUBE2DD.bottom = 0.0;
	m_coverdemmodelCUBE2DD.neardepth = 0.0;
	m_coverdemmodelCUBE2DD.fardepth = 0.0;
	return;
}


BOOL COWSet::ReadAllValuesFrom(FILE* pFILE)
{
	//2020july03, spi, begin
	//int inumread = -1;
	size_t inumread = 0;
	//2020july03, spi, end

	//id of this set
	inumread = fread(&m_idset, sizeof(int), 1, pFILE);
	if(inumread!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//type of 2D world set	 
	inumread = fread(&m_idOW2type, sizeof(int), 1, pFILE);
	if(inumread!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//euclidian bounding cube
	inumread = fread(&m_euclidianCUBE2DD, sizeof(CUBE2DD), 1, pFILE);
	if(inumread!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//latlong center coordinate (bounding rect center)
	inumread = fread(&m_latlong_centerPOINT2DD, sizeof(POINT2DD), 1, pFILE);
	if(inumread!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//latlong projection
	inumread = fread(&m_latlong_projCUBE2DD, sizeof(CUBE2DD), 1, pFILE);
	if(inumread!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//sky model id and double triplet for sun angle: azimuth, zenith, time
	inumread = fread(&m_idskymodel, sizeof(int), 1, pFILE);
	if(inumread!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	inumread = fread(&m_skymodelCUBE2DD, sizeof(CUBE2DD), 1, pFILE);
	if(inumread!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//ground dem model id
	inumread = fread(&m_idgrounddemmodel, sizeof(int), 1, pFILE);
	if(inumread!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	fread(&m_grounddemmodelCUBE2DD, sizeof(CUBE2DD), 1, pFILE); 	//double triplet not used
	//cover dem model id
	inumread = fread(&m_idcoverdemmodel, sizeof(int), 1, pFILE);
	if(inumread!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	inumread = fread(&m_coverdemmodelCUBE2DD, sizeof(CUBE2DD), 1, pFILE); 	//double triplet not used
	if(inumread!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}

BOOL COWSet::WriteAllValuesTo(FILE* pFILE)
{
	//2020july03, spi, begin
	//int inumwritten = -1;
	size_t inumwritten = 0;
	//2020july03, spi, end
	//id of this set
	inumwritten = fwrite(&m_idset, sizeof(int), 1, pFILE);
	if(inumwritten!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//type of 2D world set	 
	inumwritten = fwrite(&m_idOW2type, sizeof(int), 1, pFILE);
	if(inumwritten!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//euclidian bounding cube
	inumwritten = fwrite(&m_euclidianCUBE2DD, sizeof(CUBE2DD), 1, pFILE);
	if(inumwritten!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//latlong center coordinate (bounding rect center)
	inumwritten = fwrite(&m_latlong_centerPOINT2DD, sizeof(POINT2DD), 1, pFILE);
	if(inumwritten!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//latlong projection
	inumwritten = fwrite(&m_latlong_projCUBE2DD, sizeof(CUBE2DD), 1, pFILE);
	if(inumwritten!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//sky model id and double triplet for sun angle: azimuth, zenith, time
	inumwritten = fwrite(&m_idskymodel, sizeof(int), 1, pFILE);
	if(inumwritten!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	inumwritten = fwrite(&m_skymodelCUBE2DD, sizeof(CUBE2DD), 1, pFILE);
	if(inumwritten!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//ground dem model id
	inumwritten = fwrite(&m_idgrounddemmodel, sizeof(int), 1, pFILE);
	if(inumwritten!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	inumwritten = fwrite(&m_grounddemmodelCUBE2DD, sizeof(CUBE2DD), 1, pFILE); 	//double triplet not used
	if(inumwritten!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//cover dem model id
	inumwritten = fwrite(&m_idcoverdemmodel, sizeof(int), 1, pFILE);
	if(inumwritten!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	inumwritten = fwrite(&m_coverdemmodelCUBE2DD, sizeof(CUBE2DD), 1, pFILE); 	//double triplet not used
	if(inumwritten!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////
//class COWPointset
//////////////////////////////////

COWPointset::COWPointset()
{
	DefaultInitialization();
}

//COWPointset::COWPointset(int maxnumberofelements) : CPointset(maxnumberofelements)
COWPointset::COWPointset(int maxnumberofelements, int numberofstatsperelement) : CPointset(maxnumberofelements, numberofstatsperelement)
{
}

COWPointset::COWPointset(POINTSET* pPOINTSET) : CPointset(pPOINTSET)
{
}

COWPointset::COWPointset(COWPointset* pPointset)
{
	m_pPOINTSET = ::NewPointsetCopy(pPointset->m_pPOINTSET);
}

class COWPointset* COWPointset::NewPointsetCopy() 
{ 
	return (COWPointset*) new COWPointset(::NewPointsetCopy(m_pPOINTSET)); 
}

COWPointset::~COWPointset()
{
}

void COWPointset::DefaultInitialization()
{
	//2012august08, poirier, begin
	ASSERT(m_pPOINTSET);
	m_pPOINTSET->xmin = FLT_MAX;
	m_pPOINTSET->xmax = FLT_MIN;
	m_pPOINTSET->ymin = FLT_MAX;
	m_pPOINTSET->ymax = FLT_MIN;
	//2012august08, poirier, end
	return;
}

BOOL COWPointset::ReadAllValuesFrom(FILE* pFILE)
{
	return CPointset::ReadAllValuesFrom(pFILE);
}

BOOL COWPointset::WriteAllValuesTo(FILE* pFILE)
{
	return CPointset::WriteAllValuesTo(pFILE);
}


//////////////////////////////////
//class CPointset
//////////////////////////////////

//TODO - major oiii architecture change, backup the project in separate directory.
//TODO - remove #include "pointset.h" from the code and try to move all calls/reference
//       to pointset.h data structure and pointset.c code (in the cpp here).
//     - use the new CPointset class to provide c++ interface to pointset.h
//	   - when compile and run stable, move out the c++ layer (CPointset and COWPointset)
//       to their own source file owpointset.h/.cpp
CPointset::CPointset()
{
	DefaultInitialization();
}

//CPointset::CPointset(int maxnumberofelements)
CPointset::CPointset(int maxnumberofelements, int numberofstatsperelement)
{
	DefaultInitialization();
	m_maxnumberofelements = maxnumberofelements;
	m_pPOINTSET = NewPointset(maxnumberofelements);
	NewPointsetStatistics(maxnumberofelements, numberofstatsperelement);
	//2012august17, poirier, begin
	m_pPOINTSET->xmin = 0;
	m_pPOINTSET->xmax = 0;
	m_pPOINTSET->ymin = 0;
	m_pPOINTSET->ymax = 0;
	//2012august17, poirier, end
}


CPointset::~CPointset()
{
	if(m_pPOINTSET)
	{
		DeletePointset(m_pPOINTSET);
		m_pPOINTSET = NULL;
	}
}

void CPointset::DefaultInitialization()
{
	m_maxnumberofelements = 20; 
	m_pPOINTSET = NULL;

	m_fDX = 1.0; //use to store uncertainty on px[] points
	m_fDY = 1.0; //use to store uncertainty on py[] points

}

void CPointset::MinimalAlloc(int additionalnumberofelements/*=20*/)
{
	//check if realloc needed
	if(m_pPOINTSET->npts == m_maxnumberofelements)
	{
		int new_maxnumberofelements = m_maxnumberofelements + additionalnumberofelements; // 20;
		int iresult = ReallocPointset(new_maxnumberofelements);
		ReallocPointsetStatistics(new_maxnumberofelements);
	}
}

int CPointset::AddPointFrame(int nX, int nY, double dfXmin, double dfYmin, double dfXmax, double dfYmax)
{
	if(nX<2 || nY<2)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//line along (dfXmin,dfYmin) to (dfXmax, dfYmin)
	double dfXstep=(dfXmax-dfXmin)/(nX-1);
	for(int i=0; i<nX; i++)
	{
		double dfX=dfXmin+i*dfXstep;
		double dfY=dfYmin;
		AddPoint(dfX, dfY, POINTSET_TIN_NOUPDATE, NULL);
	}
	//line along (dfXmax, dfYmin) to (dfXmax,dfYmax)
	double dfYstep=(dfYmax-dfYmin)/(nY-1);
	//for(int i=0; i<nY; i++)
	for(int i=1; i<nY; i++) //skip (dfXmax, dfYmin), because already entered
	{
		double dfX=dfXmax;
		double dfY=dfYmin+i*dfYstep;
		AddPoint(dfX, dfY, POINTSET_TIN_NOUPDATE, NULL);
	}
	//line along (dfXmax,dfYmax) to (dfXmin, dfYmax)
	dfXstep=(dfXmax-dfXmin)/(nX-1);
	//for(int i=0; i<nY; i++)
	//2022may14, spi, begin
	//for (int i = 1; i < nY; i++) //skip (dfXmax,dfYmax), because already entered
	for(int i=1; i<nX; i++) //skip (dfXmax,dfYmax), because already entered
	//2022may14, spi, end
	{
		double dfX=dfXmax-i*dfXstep;
		double dfY=dfYmax;
		AddPoint(dfX, dfY, POINTSET_TIN_NOUPDATE, NULL);
	}
	//line along (dfXmin, dfYmax) to(dfXmin,dfYmin)
	dfYstep=(dfYmax-dfYmin)/(nY-1);
	//for(int i=0; i<nY; i++)
	for(int i=1; i<(nY-1); i++) //skip (dfXmin, dfYmax), because already entered AND skip (dfXmin,dfYmin), because already entered
	{
		double dfX=dfXmin;
		double dfY=dfYmax-i*dfYstep;
		AddPoint(dfX, dfY, POINTSET_TIN_NOUPDATE, NULL);
	}
	return TRUE;
}

int CPointset::AddPointSerie(int n, double dfX1, double dfY1, double dfX2, double dfY2, int iXMARGINflag)
{
	if(n<2)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	int nn=0;
	if(iXMARGINflag==POINTSET_ADDPOINT_NOXMARGIN) nn=0;
	  else if(iXMARGINflag==POINTSET_ADDPOINT_XMARGIN) nn=1;
	//line along (dfX1,dfY1) to (dfX2, dfY2)
	double dfXstep=(dfX2-dfX1)/(n-1+nn);
	double dfYstep=(dfY2-dfY1)/(n-1);
	double dfXoffset=0;
	if(iXMARGINflag==POINTSET_ADDPOINT_NOXMARGIN) dfXoffset=0;
	  else if(iXMARGINflag==POINTSET_ADDPOINT_XMARGIN) dfXoffset=dfXstep/2.0;
	for(int i=0; i<n; i++)
	{
		double dfX=dfXoffset+dfX1+i*dfXstep;
		double dfY=dfY1+i*dfYstep;
		AddPoint(dfX, dfY, POINTSET_TIN_NOUPDATE, NULL);
	}
	return TRUE;
}

int CPointset::AddPoint(double fX, double fY, int iTINupdateflag, int* p_itriseed, double fIntensity, double fTreeDimX, double fTreeDimY, double fTreeArea)
{
	if(iTINupdateflag==POINTSET_TIN_UPDATE && p_itriseed==NULL)
	{
		ASSERT(FALSE);
		return -1;
	}
	if(iTINupdateflag==POINTSET_TIN_UPDATE)
	{
		////////////////////////////////////////////////////
		//verify if attempt to add point onto existing point
		////////////////////////////////////////////////////
		int idtri = FindTriContainingPoint(m_pPOINTSET, fX, fY, p_itriseed);
		if(idtri==-1) 
		{
			//point (fX, fY) is outside convex hull
			return -1;
		}
		else if(idtri==-2)
		{
			//point (fX, fY) also correspond to a vertex of the triangle found,
			//can't add two points on top of each other, return error.
			return -1;
		}
	}
	else
	{
		#ifdef _DEBUG
			BOOL bResult=IsSiteAvailable(fX, fY);
			if(bResult==FALSE)
			{
				ASSERT(FALSE);
				return -1;
			}
		#endif //_DEBUG
	}

	//////////////////
	//add point
	//////////////////
	MinimalAlloc();
	
	//new point index
	int idpoint = m_pPOINTSET->npts;
	//update new number of points
	m_pPOINTSET->npts++;
	//add vertex
	m_pPOINTSET->px[idpoint] = fX;     		
	m_pPOINTSET->py[idpoint] = fY;     		
	//update world rect
	if(fX<m_pPOINTSET->xmin) m_pPOINTSET->xmin = fX;
	if(fX>m_pPOINTSET->xmax) m_pPOINTSET->xmax = fX;
	if(fY<m_pPOINTSET->ymin) m_pPOINTSET->ymin = fY;
	if(fY>m_pPOINTSET->ymax) m_pPOINTSET->ymax = fY;
	//set stats
	if(fIntensity<0.0) fIntensity=100.0;
	if(fTreeDimX<0.0) fTreeDimX=4.0;
	if(fTreeDimY<0.0) fTreeDimY=4.0;
	if(fTreeArea<0.0) fTreeArea=16.0;
	SetStatisticsValue(idpoint, POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_INTENSITY, fIntensity);
	SetStatisticsValue(idpoint, POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMX, fTreeDimX);
	SetStatisticsValue(idpoint, POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMY, fTreeDimY);
	SetStatisticsValue(idpoint, POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEAREA, fTreeArea);

	if(iTINupdateflag==POINTSET_TIN_NOUPDATE)
	{
		return idpoint;
	}
	else if(iTINupdateflag==POINTSET_TIN_UPDATE)
	{
		/*
		////////////////////
		//update TIN locally
		////////////////////
		//1) find existing tri containing point
		int idtri_existing = FindTriContainingPoint(m_pPOINTSET, fX, fY, p_itriseed);
		if(idtri_existing<0)
		{
			//triangle not found, TIN should always be previously built for now
			ASSERT(FALSE);
			return -1;
		}
		int V1=m_pPOINTSET->vt[0][idtri_existing];
		int V2=m_pPOINTSET->vt[1][idtri_existing];
		int V3=m_pPOINTSET->vt[2][idtri_existing];
		int T1=GetAdjTri(m_pPOINTSET,V3,V1,idtri_existing);
		int T2=GetAdjTri(m_pPOINTSET,V1,V2,idtri_existing);
		int T3=GetAdjTri(m_pPOINTSET,V2,V3,idtri_existing);
		//2) replace existing triangle by 3 new triangles (all inside the existing tri containing point)
		int T11 = idtri_existing;
		int T22 = m_pPOINTSET->ntri;
		int T33 = m_pPOINTSET->ntri+1;
		//update new number of triangles
		m_pPOINTSET->ntri=m_pPOINTSET->ntri+2; //1 tri replaced, 2 tri added
		//add new triangle vertices
		m_pPOINTSET->vt[0][T11] = V3;    		
		m_pPOINTSET->vt[1][T11] = V1;        		
		m_pPOINTSET->vt[2][T11] = idpoint;    		
		//add neighboring triangle
		m_pPOINTSET->nt[0][T11] = T1;
		m_pPOINTSET->nt[1][T11] = T22;
		m_pPOINTSET->nt[2][T11] = T33;
		//add new triangle vertices
		m_pPOINTSET->vt[0][T22] = V1;    		
		m_pPOINTSET->vt[1][T22] = V2;        		
		m_pPOINTSET->vt[2][T22] = idpoint;    		
		//add neighboring triangle
		m_pPOINTSET->nt[0][T22] = T2;
		m_pPOINTSET->nt[1][T22] = T33;
		m_pPOINTSET->nt[2][T22] = T11;
		//add new triangle vertices
		m_pPOINTSET->vt[0][T33] = V2;    		
		m_pPOINTSET->vt[1][T33] = V3;        		
		m_pPOINTSET->vt[2][T33] = idpoint;    		
		//add neighboring triangle
		m_pPOINTSET->nt[0][T33] = T3;
		m_pPOINTSET->nt[1][T33] = T11;
		m_pPOINTSET->nt[2][T33] = T22;
		//3) remove links to old triangle for T1, T2 and T3
		int oldT = idtri_existing;
		for(int k=0; k<3; k++)
		{
			if(m_pPOINTSET->nt[k][T1]==oldT) 
			{
				m_pPOINTSET->nt[k][T1]=T11;
			}
		}
		for(int k=0; k<3; k++)
		{
			if(m_pPOINTSET->nt[k][T2]==oldT) 
			{
				m_pPOINTSET->nt[k][T2]=T22;
			}
		}
		for(int k=0; k<3; k++)
		{
			if(m_pPOINTSET->nt[k][T3]==oldT)
			{
				m_pPOINTSET->nt[k][T3]=T33;
			}
		}
		////////////////////////////////
		//update TIN-dependent variables
		////////////////////////////////
		ComputeTriangleCenter(m_pPOINTSET, T11);
		ComputeTriangleCenter(m_pPOINTSET, T22);
		ComputeTriangleCenter(m_pPOINTSET, T33);
		//ComputeVoronoiAreaForVertex(m_pPOINTSET, idpoint);
		return idpoint;
		*/
		/////////////////////
		//update TIN globally
		/////////////////////
		BOOL bResult = BuildTINAndComputeStatistics();
		ASSERT(bResult==TRUE);
		return idpoint;
	}
	else
	{
		ASSERT(FALSE);
		return -1;
	}
}

int CPointset::MovePoint(int idpoint, double fX, double fY)
{
	if(idpoint<0 || idpoint>(m_pPOINTSET->npts-1))
	{
		ASSERT(FALSE);
		return -1;
	}
	if(IsSiteAvailable(fX, fY)==FALSE)
	{
		ASSERT(FALSE);
		return -1;
	}
	//update point coordinates
	m_pPOINTSET->px[idpoint]=fX;
	m_pPOINTSET->py[idpoint]=fY;
	//update world rect
	if(fX<m_pPOINTSET->xmin) m_pPOINTSET->xmin = fX;
	if(fX>m_pPOINTSET->xmax) m_pPOINTSET->xmax = fX;
	if(fY<m_pPOINTSET->ymin) m_pPOINTSET->ymin = fY;
	if(fY>m_pPOINTSET->ymax) m_pPOINTSET->ymax = fY;
	return idpoint;
}

BOOL CPointset::IsSiteAvailable(double fX, double fY)
{
	for(int i=0; i<m_pPOINTSET->npts; i++)
	{
		if( (abs(fX-m_pPOINTSET->px[i])<=1.0) && (abs(fY-m_pPOINTSET->py[i])<=1.0) )
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CPointset::OpenFileAsXYI(const char* lpszPathName)
{
	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();

	// open xyzi file and read xyzi header 
	char pszCharBuf[256];
	wsprintfA(pszCharBuf, "     opening %s as .xyi file-format\r\n",lpszPathName);
	pLogDocument->AddText(pszCharBuf);
	if ((xyzi_open_and_readheader(lpszPathName)) == FALSE)
	{
		wsprintfA(pszCharBuf, "     trimake: ne peut ouvrir le fichier d'entree '%s'\r\n",lpszPathName);
		pLogDocument->AddText(pszCharBuf);
		return FALSE; //error, return safely
	}

	// validate data file
	if ( xyzi_header.nombre_objet < 3)
	{
	  pLogDocument->AddText("     Fatal error, less than 3 points in file\r\n");
	  return FALSE; //error, return safely
	}
	if ( xyzi_header.nombre_objet > GetMaxNumberOfElements())
	{
		pLogDocument->AddText("     Must reallocate data ... ");
		if(ReallocPointset(xyzi_header.nombre_objet)==TRUE)
		{
			if(ReallocPointsetStatistics(xyzi_header.nombre_objet)==TRUE)
			{
				pLogDocument->AddText("     reallocation OK\r\n");
			}
			//keep going
		}
		else
		{
			pLogDocument->AddText("     ERROR in reallocation\r\n");
			return FALSE;
		}	 
	}
	if ( xyzi_header.nombre_coor < 2)
	{
	  pLogDocument->AddText("     Fatal error, minimum of 2 coordinates required to create spatialy referenced data structure\r\n");
	  return FALSE; //error, return safely
	}

	//2012july24, poirier, begin
	/*
	if(NewPointsetStatistics(xyzi_header.nombre_objet, xyzi_header.nombre_stat)==FALSE)
	{
		pLogDocument->AddText("     Fatal error, cannot allocate pfStatistics\r\n");
		return FALSE;
	}
	*/
	ASSERT(xyzi_header.nombre_stat==POINTSET_DEFAULT_NUMBEROFSTAT);
	FILE* pFILE = NULL;
	if(0)
	{
		//pFILE = fopen("test.xyz", "w");
		fopen_s(&pFILE,"test.xyz", "w");
	}
	//2012july24, poirier, end

	/*
	// optional, allocate pPointset->pfStatistics 
	pPointset->nStatPerPoint = xyzi_header.nombre_stat + POINTSET_EXTRA_NUMBEROFSTAT; // POINTSET_EXTRA_NUMBEROFSTAT, +1 for voroarea 
	pPointset->nSizeStatInByte = sizeof(double)	*(xyzi_header.nombre_objet) *(pPointset->nStatPerPoint); // +1 for voroarea     

	// *2 for list of merge segments				    				
	pPointset->pfStatistics = (double*) malloc( (pPointset->nSizeStatInByte) *2 ); // sizeof(float) replaced by *2 
	if( pPointset->pfStatistics == NULL )
	{
		pLogDocument->AddText("     Fatal error, cannot allocate pfStatistics\r\n");
		return FALSE; //error, return safely
	}
	*/

	for(int i=0; i<xyzi_header.nombre_objet; i++)
	{
		// read xyzi line 
		if( xyzi_readline() == FALSE)
		{
			// error 
			pLogDocument->AddText("     Error reading line from xyzi file\r\n");
			return FALSE; //error, return safely
		}
		m_pPOINTSET->px[m_pPOINTSET->npts] = xyzi_line.coor[0];
		m_pPOINTSET->py[m_pPOINTSET->npts] = xyzi_line.coor[1];
		if(pFILE)
		{
			float z=0;
			fprintf(pFILE, "%f\t%f\t%f\n", xyzi_line.coor[0], xyzi_line.coor[1], z);
		}
		// set voronoi area to 0.0    
		m_pPOINTSET->pfStatistics[i*(m_pPOINTSET->nStatPerPoint)] = 0.0;
		
		for(int ii=0; ii<xyzi_header.nombre_stat; ii++)
		{   
			//transfering all statistics from input file 
			m_pPOINTSET->pfStatistics[i*(m_pPOINTSET->nStatPerPoint)+ii+POINTSET_EXTRA_NUMBEROFSTAT] = xyzi_line.stat[ii]; // +1 on the left side, because first stat is voroarea 
		}
		// find min/max x-y ranges 
		m_pPOINTSET->xmin=(m_pPOINTSET->xmin < m_pPOINTSET->px[m_pPOINTSET->npts] ) ? m_pPOINTSET->xmin : m_pPOINTSET->px[m_pPOINTSET->npts];
		m_pPOINTSET->ymin=(m_pPOINTSET->ymin < m_pPOINTSET->py[m_pPOINTSET->npts] ) ? m_pPOINTSET->ymin : m_pPOINTSET->py[m_pPOINTSET->npts];
		m_pPOINTSET->xmax=(m_pPOINTSET->xmax > m_pPOINTSET->px[m_pPOINTSET->npts] ) ? m_pPOINTSET->xmax : m_pPOINTSET->px[m_pPOINTSET->npts];
		m_pPOINTSET->ymax=(m_pPOINTSET->ymax > m_pPOINTSET->py[m_pPOINTSET->npts] ) ? m_pPOINTSET->ymax : m_pPOINTSET->py[m_pPOINTSET->npts];
		m_pPOINTSET->npts++;
	}

	if(pFILE)
	{
		fclose(pFILE);
	}

	//close xyzi file
	xyzi_close();
	return TRUE;
}

BOOL CPointset::SaveFileAsXYI(const char* lpszPathName)
{
	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();

	// open xyzi file and read xyzi header 
	char pszCharBuf[256];
	wsprintfA(pszCharBuf, "     saving %s as .xyi file-format\r\n",lpszPathName);
	pLogDocument->AddText(pszCharBuf);
	xyzi_header.nombre_objet = m_pPOINTSET->npts;
	ASSERT(m_pPOINTSET->npts>2);
	xyzi_header.nombre_id = 1;
	xyzi_header.nombre_coor = 2;
	xyzi_header.nombre_stat = GetNumberOfStatPerPoint()-POINTSET_EXTRA_NUMBEROFSTAT;
	ASSERT(xyzi_header.nombre_stat==POINTSET_DEFAULT_NUMBEROFSTAT); //for version 1.0
	xyzi_header.liste_voisin_flag = 0;
	xyzi_header.liste_centre_flag = 0;
	if ((xyzi_open_and_writeheader(lpszPathName)) == FALSE)
	{
		wsprintfA(pszCharBuf, "     oiii.exe can't create xyzi file '%s'\r\n",lpszPathName);
		pLogDocument->AddText(pszCharBuf);
		return FALSE; //error, return safely
	}

	for(int i=0; i<xyzi_header.nombre_objet; i++)
	{
		// write xyzi line 
		xyzi_line.id[0] = i;
		xyzi_line.coor[0] = m_pPOINTSET->px[i];
		xyzi_line.coor[1] = m_pPOINTSET->py[i];
		for(int ii=0; ii<xyzi_header.nombre_stat; ii++)
		{   
			//transfering all statistics to output file 
			xyzi_line.stat[ii] = m_pPOINTSET->pfStatistics[i*(m_pPOINTSET->nStatPerPoint)+ii+POINTSET_EXTRA_NUMBEROFSTAT]; // +1 on the left side, because first stat is voroarea 
		}
		if( xyzi_writeline() == FALSE)
		{
			// error 
			pLogDocument->AddText("     Error writing line to xyzi file\r\n");
			return FALSE; //error, return safely
		}
		
	}

	//close xyzi file
	xyzi_close();
	return TRUE;
}


BOOL CPointset::OpenFileAsXYZ(const char* lpszPathName)
{
	FILE* pFile = NULL;
	double fDummy;

	if(lpszPathName=="")
	{
		ASSERT(FALSE);
		return FALSE;
	}
	ASSERT(m_pPOINTSET);
	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();


	// it is a .xyz file format
	char pszCharBuf[256];
	wsprintfA(pszCharBuf, "     opening %s as .xyz file-format\r\n",lpszPathName);
	pLogDocument->AddText(pszCharBuf);
	if( (pFile = fopen(lpszPathName, "r")) == NULL )
	{
		wsprintfA(pszCharBuf, "     Error, cannot open %s as .xyz file (do you have proper file-format?)\r\n",lpszPathName);
		pLogDocument->AddText(pszCharBuf);
		return FALSE; //error, return safely
	}
	int i;
	float df1, df2, df3;
	//while( (i=fscanf(pFile, "%f%f%f", &(m_pPOINTSET->px[m_pPOINTSET->npts]), &(m_pPOINTSET->py[m_pPOINTSET->npts]), &fDummy)) != EOF)
	while( (i=fscanf(pFile, "%f %f %f", &df1, &df2, &df3)) != EOF)
	{
		m_pPOINTSET->px[m_pPOINTSET->npts] = df1;
		m_pPOINTSET->py[m_pPOINTSET->npts] = df2;
		m_pPOINTSET->xmin=(m_pPOINTSET->xmin < m_pPOINTSET->px[m_pPOINTSET->npts] ) ? m_pPOINTSET->xmin : m_pPOINTSET->px[m_pPOINTSET->npts];
		m_pPOINTSET->ymin=(m_pPOINTSET->ymin < m_pPOINTSET->py[m_pPOINTSET->npts] ) ? m_pPOINTSET->ymin : m_pPOINTSET->py[m_pPOINTSET->npts];
		m_pPOINTSET->xmax=(m_pPOINTSET->xmax > m_pPOINTSET->px[m_pPOINTSET->npts] ) ? m_pPOINTSET->xmax : m_pPOINTSET->px[m_pPOINTSET->npts];
		m_pPOINTSET->ymax=(m_pPOINTSET->ymax > m_pPOINTSET->py[m_pPOINTSET->npts] ) ? m_pPOINTSET->ymax : m_pPOINTSET->py[m_pPOINTSET->npts];
		//2012july24, poirier, begin
		//set all stats to zero
		for(int ii=0; ii<m_pPOINTSET->nStatPerPoint; ii++)
		{   
			m_pPOINTSET->pfStatistics[i*(m_pPOINTSET->nStatPerPoint)+ii] = 0.0; 
		}
		//set some stat to non-zero
		m_pPOINTSET->pfStatistics[i*(m_pPOINTSET->nStatPerPoint)+POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_INTENSITY] = 100.0 + (rand()%10 + 1);
		double fDimX = 1.0 + (rand()%4 + 1);
		double fDimY = 1.0 + (rand()%4 + 1);
		m_pPOINTSET->pfStatistics[i*(m_pPOINTSET->nStatPerPoint)+POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMX] = fDimX; 
		m_pPOINTSET->pfStatistics[i*(m_pPOINTSET->nStatPerPoint)+POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMY] = fDimY; 
		m_pPOINTSET->pfStatistics[i*(m_pPOINTSET->nStatPerPoint)+POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEAREA] = fDimX*fDimY; 
		//2012july24, poirier, end
		m_pPOINTSET->npts++;
		if(m_pPOINTSET->npts == m_pPOINTSET->maxnumberofelements)
		{
			pLogDocument->AddText("     Error, too many points in file\r\n");
			return FALSE; //error, return safely
		}
	}
	if(m_pPOINTSET->npts<3)
	{
		pLogDocument->AddText("     Fatal error, less than 3 points in file\r\n");
		return FALSE; //error, return safely
	}
	return TRUE;
}


BOOL CPointset::BuildTINAndComputeStatistics(int iNORMALIZEflag) 
{
	ASSERT(m_pPOINTSET!=NULL);

	if(m_pPOINTSET->npts<3) return FALSE;

	/* //spi 2014
 	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	*/
	CLogDocument* pLogDocument = NULL;
	if(AfxGetApp()!=NULL) pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	CTime InitialTime = CTime::GetCurrentTime();

    //Construction du reseau de triangles en memoire
    if(pLogDocument) pLogDocument->AddText("     building triangle network ... \r\n");
    BuildTriangleNetwork(m_pPOINTSET);
	if (pLogDocument) pLogDocument->AddText("     triangle network built.\r\n");

	//2021fev08, spi, begin
	if (pLogDocument)
	{
		CTime FinalTINTime = CTime::GetCurrentTime();
		CTimeSpan TINBuildTime = FinalTINTime - InitialTime;
		char pszCharBuf[1024];
		wsprintfA(pszCharBuf, "TIN was built in %d min and %d sec.\r\n", TINBuildTime.GetMinutes(), TINBuildTime.GetSeconds());
		pLogDocument->AddText(pszCharBuf);
		int tri_count = m_pPOINTSET->ntri;
		int ccw_count = 0;
		int cw_count = 0;
		for (int idtri=0; idtri<tri_count; idtri++)
		{
			if (IsTriangleStoredCounterClockwise(m_pPOINTSET, idtri) == true)
			{
				ccw_count++;
			}
			else
			{
				cw_count++;
			}
		}
		wsprintfA(pszCharBuf, "%d out of %d TIN triangles have vertices stored counter-clockwise.\r\n", ccw_count, tri_count);
		pLogDocument->AddText(pszCharBuf);
		wsprintfA(pszCharBuf, "%d out of %d TIN triangles have vertices stored clockwise.\r\n", cw_count, tri_count);
		pLogDocument->AddText(pszCharBuf);
		CTime FinalTINCheckTime = CTime::GetCurrentTime();
		CTimeSpan TINCheckTime = FinalTINCheckTime - FinalTINTime;
		wsprintfA(pszCharBuf, "TIN was validated in %d min and %d sec.\r\n", TINCheckTime.GetMinutes(), TINCheckTime.GetSeconds());
		pLogDocument->AddText(pszCharBuf);
	}
	//2021fev08, spi, end

    //Calcul des centres qui sont les sommets des polygones voronoi
    if(pLogDocument) pLogDocument->AddText("     computing triangle centers ... ");
    ComputeAllTriangleCenters(m_pPOINTSET);
    if(pLogDocument) pLogDocument->AddText("done.\r\n");
#ifdef _DEBUG
	if(0)
	{
		save_triangle_to_file(m_pPOINTSET);
	}
#endif //_DEBUG
    
    //Calcul des statistiques qui sont les sommets des triangles
    if(pLogDocument) pLogDocument->AddText("     computing voronoi area and density ... ");
    ComputeVoronoiAreaForAllVertex(m_pPOINTSET);
    if(pLogDocument) pLogDocument->AddText("done.\r\n");

	if(iNORMALIZEflag==POINTSET_TIN_NORMALIZE)
	{
		//begin of poirier, march 27 1997
		if(pLogDocument) pLogDocument->AddText("     normalizing voronoi area ... ");
		NormalizeAllVertex(	m_pPOINTSET, 
							POINTSET_OFFSETSTAT_VOROAREA,
							POINTSET_OFFSETSTAT_VOROAREA, 
							0.0,
							100.0);
		if(pLogDocument) pLogDocument->AddText("done.\r\n");
		//2022may16, spi, begin
		if (GetNumberOfStatPerPoint() > POINTSET_OFFSETSTAT_INTENSITY)
		{
			if (pLogDocument) pLogDocument->AddText("     normalizing tree intensity ... ");
			NormalizeAllVertex(m_pPOINTSET,
				POINTSET_OFFSETSTAT_INTENSITY,
				POINTSET_OFFSETSTAT_INTENSITY,
				0.0,
				100.0);
			if (pLogDocument) pLogDocument->AddText("done.\r\n");
		}
		//2022may16, spi, end
		/*
		pLogDocument->AddText("done.\r\n");
		pLogDocument->AddText("     normalizing tree dimension x ... ");
		NormalizeAllVertex(	m_pPOINTSET, 
							POINTSET_OFFSETSTAT_INTENSITY + POINTSET_XYZI_TREEDIMX,
							POINTSET_OFFSETSTAT_INTENSITY + POINTSET_XYZI_TREEDIMX, 
							0.0,
							100.0);    
		pLogDocument->AddText("done.\r\n");
		pLogDocument->AddText("     normalizing tree dimension y ... ");
		NormalizeAllVertex(	m_pPOINTSET, 
							POINTSET_OFFSETSTAT_INTENSITY + POINTSET_XYZI_TREEDIMY,
							POINTSET_OFFSETSTAT_INTENSITY + POINTSET_XYZI_TREEDIMY, 
							0.0,
							100.0);    
		pLogDocument->AddText("done.\r\n");
		*/
		//2022may16, spi, begin
		if (GetNumberOfStatPerPoint() > POINTSET_OFFSETSTAT_INTENSITY + POINTSET_XYZI_TREEAREA)
		{
			if (pLogDocument) pLogDocument->AddText("     normalizing tree area ... ");
			NormalizeAllVertex(m_pPOINTSET,
				POINTSET_OFFSETSTAT_INTENSITY + POINTSET_XYZI_TREEAREA,
				POINTSET_OFFSETSTAT_INTENSITY + POINTSET_XYZI_TREEAREA,
				0.0,
				100.0);
			if (pLogDocument) pLogDocument->AddText("done.\r\n");
		}
		//end of poirier, march 27 1997
		if (GetNumberOfStatPerPoint() > POINTSET_OFFSETSTAT_NEIGHAVERAGEVOROAREA)
		{
			if (pLogDocument) pLogDocument->AddText("     computing local voronoi area average ... ");
			/* commented out, replaced by ApplyAverageFilterToAllVertex()
			ComputeNeighboringVoronoiDensityForAllVertex(m_pPOINTSET);
			*/
			/* commented out, replaced by ComputeLocalAverage()
			ApplyAverageFilterToAllVertex(	m_pPOINTSET,
											//POINTSET_OFFSETSTAT_VORODENSITY,
											POINTSET_OFFSETSTAT_VOROAREA,
											POINTSET_OFFSETSTAT_NEIGHAVERAGEVOROAREA,
											1);
			*/
			ComputeLocalAverage(m_pPOINTSET,
				POINTSET_OFFSETSTAT_VOROAREA,
				POINTSET_OFFSETSTAT_NEIGHAVERAGEVOROAREA,
				1);
			if (pLogDocument) pLogDocument->AddText("done.\r\n");
		}
		if (GetNumberOfStatPerPoint() > POINTSET_OFFSETSTAT_INTENSITY)
		{
			if (pLogDocument) pLogDocument->AddText("     computing local tree intensity average ... ");
			/*
			ApplyAverageFilterToAllVertex(	m_pPOINTSET,
											POINTSET_OFFSETSTAT_INTENSITY,
											POINTSET_OFFSETSTAT_NEIGHAVERAGEINTENSITY,
											1);
			*/
			ComputeLocalAverage(m_pPOINTSET,
				POINTSET_OFFSETSTAT_INTENSITY,
				POINTSET_OFFSETSTAT_NEIGHAVERAGEINTENSITY,
				1);
			if (pLogDocument) pLogDocument->AddText("done.\r\n");
		}
		if (GetNumberOfStatPerPoint() > POINTSET_OFFSETSTAT_INTENSITY + POINTSET_XYZI_TREEAREA)
		{
			if (pLogDocument) pLogDocument->AddText("     computing local tree area average ... ");
			/* commented out, replaced by ComputeLocalAverage()
			ApplyAverageFilterToAllVertex(	m_pPOINTSET,
											POINTSET_EXTRA_NUMBEROFSTAT + POINTSET_XYZI_TREEAREA,
											POINTSET_OFFSETSTAT_NEIGHAVERAGETREEAREA,
											1);
			*/
			ComputeLocalAverage(m_pPOINTSET,
				POINTSET_OFFSETSTAT_INTENSITY + POINTSET_XYZI_TREEAREA,
				POINTSET_OFFSETSTAT_NEIGHAVERAGETREEAREA,
				1);
			if (pLogDocument) pLogDocument->AddText("done.\r\n");
		}
		//2022may16, spi, end
		/* poirier, march 27 1997
		pLogDocument->AddText("     normalizing local voronoi area average ... ");
		NormalizeAllVertex(	m_pPOINTSET, 
							POINTSET_OFFSETSTAT_NEIGHAVERAGEVOROAREA,
							POINTSET_OFFSETSTAT_NEIGHAVERAGEVOROAREA, 
							0.0,
							100.0);    
		pLogDocument->AddText("done.\r\n");
		*/
	
		/* poirier, march 27 1997
		pLogDocument->AddText("     normalizing local tree intensity average ... ");
		NormalizeAllVertex(	m_pPOINTSET, 
							POINTSET_OFFSETSTAT_NEIGHAVERAGEINTENSITY,
							POINTSET_OFFSETSTAT_NEIGHAVERAGEINTENSITY, 
							0.0,
							100.0);    
		pLogDocument->AddText("done.\r\n");
		*/
	
		/* poirier, march 27 1997
		pLogDocument->AddText("     normalizing local tree area average ... ");
		NormalizeAllVertex(	m_pPOINTSET, 
							POINTSET_OFFSETSTAT_NEIGHAVERAGETREEAREA,
							POINTSET_OFFSETSTAT_NEIGHAVERAGETREEAREA, 
							0.0,
							100.0);    
		pLogDocument->AddText("done.\r\n");
		*/
		//2022may16, spi, begin
		if (GetNumberOfStatPerPoint() > POINTSET_OFFSETSTAT_NEIGHVARIANCEVOROAREA)
		{
			if (pLogDocument) pLogDocument->AddText("     computing local voronoi area variance ... ");
			ComputeLocalVariance(m_pPOINTSET,
				POINTSET_OFFSETSTAT_VOROAREA,
				POINTSET_OFFSETSTAT_NEIGHAVERAGEVOROAREA,
				POINTSET_OFFSETSTAT_NEIGHVARIANCEVOROAREA,
				1);
			if (pLogDocument) pLogDocument->AddText("done.\r\n");
		}
		if (GetNumberOfStatPerPoint() > POINTSET_OFFSETSTAT_INTENSITY)
		{
			if (pLogDocument) pLogDocument->AddText("     computing local tree intensity variance ... ");
			ComputeLocalVariance(m_pPOINTSET,
				POINTSET_OFFSETSTAT_INTENSITY,
				POINTSET_OFFSETSTAT_NEIGHAVERAGEINTENSITY,
				POINTSET_OFFSETSTAT_NEIGHVARIANCEINTENSITY,
				1);
			if (pLogDocument) pLogDocument->AddText("done.\r\n");
		}
		if (GetNumberOfStatPerPoint() > POINTSET_OFFSETSTAT_INTENSITY + POINTSET_XYZI_TREEAREA)
		{
			if (pLogDocument) pLogDocument->AddText("     computing local tree area variance ... ");
			ComputeLocalVariance(m_pPOINTSET,
				POINTSET_OFFSETSTAT_INTENSITY + POINTSET_XYZI_TREEAREA,
				POINTSET_OFFSETSTAT_NEIGHAVERAGETREEAREA,
				POINTSET_OFFSETSTAT_NEIGHVARIANCETREEAREA,
				1);
			if (pLogDocument) pLogDocument->AddText("done.\r\n");
		}
		if (GetNumberOfStatPerPoint() > POINTSET_OFFSETSTAT_CLASSTREEINTENSITY)
		{
			if (pLogDocument) pLogDocument->AddText("     initializing class tree intensity to zero ... ");
			InitializeClassToZero(m_pPOINTSET, POINTSET_OFFSETSTAT_CLASSTREEINTENSITY);
			if (pLogDocument) pLogDocument->AddText("done.\r\n");
		}
		if(pLogDocument) pLogDocument->AddText("     computing global statistics ... ");
		for(int istatoffset=0; istatoffset<GetNumberOfStatPerPoint(); istatoffset++)
		{
			UpdateGlobalStatisticsInfo(m_pPOINTSET, istatoffset);
		}
		if(pLogDocument) pLogDocument->AddText("done.\r\n");
		//2022may16, spi, end
		//poirier, march 27 1997
		/*
		pLogDocument->AddText("     testing neighbor consistency ... ");
		CheckNeighborConsistensyForAllVertex(m_pPOINTSET);
		pLogDocument->AddText("done.\r\n");
		*/
	}

	CTime FinalTime = CTime::GetCurrentTime();
	CTimeSpan DiffTime = FinalTime - InitialTime;

	if(pLogDocument) 
	{
		char pszCharBuf[1024];
		wsprintfA(pszCharBuf, "TIN built and stat computed successfully in %d min and %d sec.\r\n", DiffTime.GetMinutes(), DiffTime.GetSeconds());
		pLogDocument->AddText(pszCharBuf);
	}
	return TRUE;
}

BOOL CPointset::DrawPointset(COW2View* pOW2View, 
							 CDC* pDC, 
							 CRect* pViewRect, 
							 int idrawingflag) //POINTSET_DRAW_POINT_AS_ELLIPSE
{
	int idpoint = POINTSET_ALL;
	return DrawPointset(pOW2View, pDC, idpoint, idrawingflag); //POINTSET_DRAW_POINT_AS_ELLIPSE
}

/*
BOOL CPointset::DrawPointset(COW2View* pOW2View, 
							 CDC* pDC, 
							 int idpoint, 
							 int idrawingflag) //POINTSET_DRAW_POINT_AS_ELLIPSE
{
	if(pOW2View==NULL || pDC==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	COW2Doc* pDoc = pOW2View->GetDocument();

	///////////////////////////////////
	// This implementation will draw an
	// ellipse around each input data
	// point.
	//
	// In our specific application, the
	// point is actualy a tree position
	// and the ellipse x and y axis are
	// given by the tree spatial x-y
	// dimension found by the TreeFinder
	// Application.
	///////////////////////////////////

	int      	i, iStatBaseOffset, nStatPerPoint; 
	double 		dfTreePosX, dfTreePosY, dfTreeDimX, dfTreeDimY;
	double*		pfStatistics;
	pfStatistics = pDoc->GetPointerToStatistics();
	nStatPerPoint = pDoc->GetNumberOfStatPerPoint(); 


	//poirier dec 98, added for classification, begin
	int* pidsegList = NULL;
	//if(pOW2View->m_pViewDataSettings->bDrawClassLabels)
	if(idrawingflag & POINTSET_DRAW_CLASSSETLABEL)
	{
		ASSERT(pOW2View->m_iNumberOfSegmentForFinalClassification!=-1);
		//1) collect the id of the segment present in the final partition  
		pidsegList = new int[pOW2View->m_iNumberOfSegmentForFinalClassification];
		if(pidsegList==NULL)
		{
    		((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText("problem allocating pidsegList in ow2view.cpp\r\n");
			ASSERT(FALSE);
    		return FALSE;
		} 
		if(!GetRemainingSegmentListForFinalPartition(pDoc->m_pClassset->GetSegmentsetPointer(), pidsegList, pOW2View->m_iNumberOfSegmentForFinalClassification))
		{
			ASSERT(FALSE);
			return FALSE; //error, abort attempt to draw
		}
	}
	//poirier dec 98, added for classification, end

	for (i=0; i<GetNumberOfPoints(); i++)
	{   
	    iStatBaseOffset = i*nStatPerPoint;
		dfTreePosX = m_pPOINTSET->px[i];
		dfTreePosY = m_pPOINTSET->py[i];

		//if(pOW2View->m_pViewDataSettings->bDrawPointset)
		if(idrawingflag & POINTSET_DRAW_POINTSET)
		{
			dfTreeDimX = pfStatistics[iStatBaseOffset+POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMX];
			dfTreeDimY = pfStatistics[iStatBaseOffset+POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMY]; 
			double dfHalfX = dfTreeDimX/2.0;
			double dfHalfY = dfTreeDimY/2.0;
			RECT myRect;
			//top left
			POINT2DD myPoint2DD;
			POINT myPoint;
			myPoint2DD.x = dfTreePosX-dfHalfX;
			myPoint2DD.y = dfTreePosY-dfHalfY;
			myPoint = pOW2View->MapPointFromWorldToView( myPoint2DD);
			myRect.left = myPoint.x;
			myRect.top = myPoint.y;
			//bottom right
			myPoint2DD.x = dfTreePosX+dfHalfX;
			myPoint2DD.y = dfTreePosY+dfHalfY;
			myPoint = pOW2View->MapPointFromWorldToView( myPoint2DD);
			myRect.right = myPoint.x;
			myRect.bottom = myPoint.y;
			//draw an ellipse for each point (tree crown)
			if( idrawingflag & POINTSET_DRAW_POINTSET_AS_ELLIPSE )
			{
				pDC->Ellipse(&myRect);
			}
			else if( idrawingflag & POINTSET_DRAW_POINTSET_AS_CROSS )
			{
				//tree center position
				myPoint2DD.x = dfTreePosX;
				myPoint2DD.y = dfTreePosY;
				myPoint = pOW2View->MapPointFromWorldToView( myPoint2DD);
				
				pDC->MoveTo(myRect.left, myPoint.y);
				pDC->LineTo(myRect.right, myPoint.y); 
				pDC->MoveTo(myPoint.x, myRect.top);
				pDC->LineTo(myPoint.x, myRect.bottom); 
			}

		}

		//if(pOW2View->m_pViewDataSettings->bDrawPointLabels
		//	|| pOW2View->m_pViewDataSettings->bDrawClassLabels)
		if( (idrawingflag & POINTSET_DRAW_POINTSETLABEL) || (idrawingflag & POINTSET_DRAW_CLASSSETLABEL))
		{
			WCHAR text[50];
			POINT2DD myPoint2DD;
			POINT myPoint;
			myPoint2DD.x = dfTreePosX;
			myPoint2DD.y = dfTreePosY;
			myPoint = pOW2View->MapPointFromWorldToView( myPoint2DD);
			if( (idrawingflag & POINTSET_DRAW_POINTSETLABEL) && 
				!(idrawingflag & POINTSET_DRAW_CLASSSETLABEL))
			{
				//dessiner le numero du point
				swprintf_s(text,50, L"%d", i);
				int ncount = wcslen(text);
				pDC->TextOut( myPoint.x, myPoint.y, text, ncount);
			}
			else if(idrawingflag & POINTSET_DRAW_CLASSSETLABEL)
			{
				int idclass = GetOwnerParentClassThreshold(pDoc->m_pClassset->GetSegmentsetPointer(), i, pOW2View->m_iNumberOfSegmentForFinalClassification);
				int idclassdisplayed = pOW2View->LookUpKeyInIntList(pidsegList, pOW2View->m_iNumberOfSegmentForFinalClassification, idclass);
				ASSERT(idclassdisplayed!=-1);
				if(idrawingflag & POINTSET_DRAW_POINTSETLABEL) 
				{
					//dessiner le numero et la classe du point
					swprintf_s(text,50, L"%d[%d]", i, idclassdisplayed);
					int ncount = wcslen(text);
					pDC->TextOut( myPoint.x, myPoint.y, text, ncount);
				}
				else if(idrawingflag & POINTSET_DRAW_CLASSSETLABEL)
				{
					//dessiner la classe du point
					swprintf_s(text,50, L"[%d]", idclassdisplayed);
					int ncount = wcslen(text);
					pDC->TextOut( myPoint.x, myPoint.y, text, ncount);
				}
			}
		}
	}
	if(pidsegList) delete[] pidsegList;
	//pDC->SelectObject(pOldBrush); 
	//pDC->SelectObject(pOldPen);
	//if(pPen) delete pPen;
	//pDC->SetTextColor(crOldTextColor);
	//pDC->SetBkMode(nOldBackgroundMode);		 
	return TRUE;
}
*/

BOOL CPointset::DrawPointset(COW2View* pOW2View,
								CDC* pDC,
								int idpoint,
								int idrawingflag, //POINTSET_DRAW_POINT_AS_ELLIPSE
								HDC hdc/*=NULL*/) //when used by audiospi and videospi apps hdc should be non-null but pOW2View and pDC should be null
{
	/*
	if (pOW2View == NULL || pDC == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	*/
	COW2Doc* pDoc = NULL;
	if(pOW2View) pOW2View->GetDocument();

	///////////////////////////////////
	// This implementation will draw an
	// ellipse around each input data
	// point.
	//
	// In our specific application, the
	// point is actualy a tree position
	// and the ellipse x and y axis are
	// given by the tree spatial x-y
	// dimension found by the TreeFinder
	// Application.
	///////////////////////////////////

	int i, iStatBaseOffset, nStatPerPoint;
	double dfTreePosX, dfTreePosY;
	double dfTreeDimX = 10.0;
	double dfTreeDimY = 10.0;
	double* pfStatistics = NULL;
	if(pDoc) pfStatistics = pDoc->GetPointerToStatistics();
	nStatPerPoint = 0;
	if(pDoc) pDoc->GetNumberOfStatPerPoint();


	//poirier dec 98, added for classification, begin
	int* pidsegList = NULL;
	//if(pOW2View->m_pViewDataSettings->bDrawClassLabels)
	if (idrawingflag & POINTSET_DRAW_CLASSSETLABEL)
	{
		if (pOW2View)
		{
			ASSERT(pOW2View->m_iNumberOfSegmentForFinalClassification != -1);
			//1) collect the id of the segment present in the final partition  
			pidsegList = new int[pOW2View->m_iNumberOfSegmentForFinalClassification];
			if (pidsegList == NULL)
			{
				((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText("problem allocating pidsegList in ow2view.cpp\r\n");
				ASSERT(FALSE);
				return FALSE;
			}
			if (!GetRemainingSegmentListForFinalPartition(pDoc->m_pClassset->GetSegmentsetPointer(), pidsegList, pOW2View->m_iNumberOfSegmentForFinalClassification))
			{
				ASSERT(FALSE);
				return FALSE; //error, abort attempt to draw
			}
		}
	}
	//poirier dec 98, added for classification, end

	for (i = 0; i < GetNumberOfPoints(); i++)
	{
		if (idpoint!=POINTSET_ALL && i!=idpoint)
		{
			//when single idpoint is specified, skip all but this idpoint
			continue;
		}
		iStatBaseOffset = i * nStatPerPoint;
		dfTreePosX = m_pPOINTSET->px[i];
		dfTreePosY = m_pPOINTSET->py[i];

		//if(pOW2View->m_pViewDataSettings->bDrawPointset)
		if (idrawingflag & POINTSET_DRAW_POINTSET)
		{
			if (pfStatistics)
			{
				dfTreeDimX = pfStatistics[iStatBaseOffset + POINTSET_EXTRA_NUMBEROFSTAT + POINTSET_XYZI_TREEDIMX];
				dfTreeDimY = pfStatistics[iStatBaseOffset + POINTSET_EXTRA_NUMBEROFSTAT + POINTSET_XYZI_TREEDIMY];
			}
			double dfHalfX = dfTreeDimX / 2.0;
			double dfHalfY = dfTreeDimY / 2.0;
			RECT myRect;
			//top left
			POINT2DD myPoint2DD;
			POINT myPoint;
			myPoint2DD.x = dfTreePosX - dfHalfX;
			myPoint2DD.y = dfTreePosY - dfHalfY;
			if (pOW2View)
			{
				myPoint = myPoint = pOW2View->MapPointFromWorldToView(myPoint2DD);
			}
			else if(hdc)
			{
				myPoint = MapPointFromWorldToView(myPoint2DD);
			}
			myRect.left = myPoint.x;
			myRect.top = myPoint.y;
			//bottom right
			myPoint2DD.x = dfTreePosX + dfHalfX;
			myPoint2DD.y = dfTreePosY + dfHalfY;
			if (pOW2View)
			{
				myPoint = pOW2View->MapPointFromWorldToView(myPoint2DD);
			}
			else if(hdc)
			{
				myPoint = MapPointFromWorldToView(myPoint2DD);
			}
			myRect.right = myPoint.x;
			myRect.bottom = myPoint.y;
			//draw an ellipse for each point (tree crown)
			if (idrawingflag & POINTSET_DRAW_POINTSET_AS_ELLIPSE)
			{
				if (pDC)
				{
					pDC->Ellipse(&myRect);
				}
				else if(hdc)
				{
					Ellipse(hdc, myRect.left, myRect.top, myRect.right, myRect.bottom);
				}
			}
			else if (idrawingflag & POINTSET_DRAW_POINTSET_AS_CROSS)
			{
				//tree center position
				myPoint2DD.x = dfTreePosX;
				myPoint2DD.y = dfTreePosY;
				if (pOW2View)
				{
					myPoint = pOW2View->MapPointFromWorldToView(myPoint2DD);
				}
				else if(hdc)
				{
					myPoint = MapPointFromWorldToView(myPoint2DD);
				}
				if (pDC)
				{
					pDC->MoveTo(myRect.left, myPoint.y);
					pDC->LineTo(myRect.right, myPoint.y);
					pDC->MoveTo(myPoint.x, myRect.top);
					pDC->LineTo(myPoint.x, myRect.bottom);
				}
				else if (hdc)
				{
					bool bresult;
					bresult = MoveToEx(hdc, myRect.left, myPoint.y, NULL);
					bresult = LineTo(hdc, myRect.right, myPoint.y);
					bresult = MoveToEx(hdc, myPoint.x, myRect.top, NULL);
					bresult = LineTo(hdc, myPoint.x, myRect.bottom);
				}
			}

		}

		//if(pOW2View->m_pViewDataSettings->bDrawPointLabels
		//	|| pOW2View->m_pViewDataSettings->bDrawClassLabels)
		if ((idrawingflag & POINTSET_DRAW_POINTSETLABEL) || (idrawingflag & POINTSET_DRAW_CLASSSETLABEL))
		{
			WCHAR text[50];
			POINT2DD myPoint2DD;
			POINT myPoint;
			myPoint2DD.x = dfTreePosX;
			myPoint2DD.y = dfTreePosY;
			if (pOW2View)
			{
				myPoint = pOW2View->MapPointFromWorldToView(myPoint2DD);
			}
			else if (hdc)
			{
				myPoint = MapPointFromWorldToView(myPoint2DD); 
			}
			if ((idrawingflag & POINTSET_DRAW_POINTSETLABEL) &&
				!(idrawingflag & POINTSET_DRAW_CLASSSETLABEL))
			{
				//dessiner le numero du point
				swprintf_s(text, 50, L"%d", i);
				int ncount = wcslen(text);
				if (pDC)
				{
					pDC->TextOut(myPoint.x, myPoint.y, text, ncount);
				}
				else if (hdc)
				{
					TextOut(hdc, myPoint.x, myPoint.y, text, ncount);
				}
			}
			else if (idrawingflag & POINTSET_DRAW_CLASSSETLABEL)
			{
				if (pOW2View)
				{
					int idclass = GetOwnerParentClassThreshold(pDoc->m_pClassset->GetSegmentsetPointer(), i, pOW2View->m_iNumberOfSegmentForFinalClassification);
					int idclassdisplayed = pOW2View->LookUpKeyInIntList(pidsegList, pOW2View->m_iNumberOfSegmentForFinalClassification, idclass);
					ASSERT(idclassdisplayed != -1);
					if (idrawingflag & POINTSET_DRAW_POINTSETLABEL)
					{
						//dessiner le numero et la classe du point
						swprintf_s(text, 50, L"%d[%d]", i, idclassdisplayed);
						int ncount = wcslen(text);
						pDC->TextOut(myPoint.x, myPoint.y, text, ncount);
					}
					else if (idrawingflag & POINTSET_DRAW_CLASSSETLABEL)
					{
						//dessiner la classe du point
						swprintf_s(text, 50, L"[%d]", idclassdisplayed);
						int ncount = wcslen(text);
						pDC->TextOut(myPoint.x, myPoint.y, text, ncount);
					}
				}
			}
		}
	}
	if (pidsegList) delete[] pidsegList;
	//pDC->SelectObject(pOldBrush); 
	//pDC->SelectObject(pOldPen);
	//if(pPen) delete pPen;
	//pDC->SetTextColor(crOldTextColor);
	//pDC->SetBkMode(nOldBackgroundMode);		 
	return TRUE;
}

/*
void CPointset::DrawPointsetObject(COW2View* pOW2View, CDC * pDC, int idpointsetobject)
{
    CPen pen;
	CPen* pOldPen;
	int nOldDrawingMode;
    // Save the device context for modes
    pDC->SaveDC();
    nOldDrawingMode = pDC->SetROP2(R2_NOTXORPEN); //XOR mode, always have to erase what's drawn.
    
    pen.CreatePen(PS_SOLID,  0, RGB(0, 0, 0)); // 0 width = 1 device unit
    pOldPen = pDC->SelectObject(&pen);

	COW2Doc* pDoc = pOW2View->GetDocument();
	ASSERT(pDoc!=NULL && m_pPOINTSET!=NULL && idpointsetobject>-1 
			&& idpointsetobject<GetNumberOfPoints());

	double* pfStatistics = pDoc->GetPointerToStatistics();
	int nStatPerPoint = pDoc->GetNumberOfStatPerPoint(); 
	int iStatBaseOffset = idpointsetobject*nStatPerPoint;
	double dfTreePosX = m_pPOINTSET->px[idpointsetobject];
	double dfTreePosY = m_pPOINTSET->py[idpointsetobject];
	double dfTreeDimX = pfStatistics[iStatBaseOffset+POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMX];
	double dfTreeDimY = pfStatistics[iStatBaseOffset+POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMY]; 
 	double dfHalfX = dfTreeDimX/2.0;
	double dfHalfY = dfTreeDimY/2.0;
	RECT myRect;
	//top left
	POINT2DD myPoint2DD;
	POINT myPoint;
	myPoint2DD.x = dfTreePosX-dfHalfX;
	myPoint2DD.y = dfTreePosY-dfHalfY;
	myPoint = pOW2View->MapPointFromWorldToView( myPoint2DD);
	myRect.left = myPoint.x;
	myRect.top = myPoint.y;
	//bottom right
	myPoint2DD.x = dfTreePosX+dfHalfX;
	myPoint2DD.y = dfTreePosY+dfHalfY;
	myPoint = pOW2View->MapPointFromWorldToView( myPoint2DD);
	myRect.right = myPoint.x;
	myRect.bottom = myPoint.y;
	//draw an ellipse for each point (tree crown)
	pDC->Ellipse(&myRect);
    

    pDC->SelectObject(pOldPen);
    pDC->SetROP2(nOldDrawingMode); 
    pDC->RestoreDC(-1);
    return;
}
*/

void CPointset::DrawPointsetObject(COW2View* pOW2View, 
									CDC * pDC, 
									int idpointsetobject) //, 
									//HDC hdc/*=NULL*/) //when used by audiospi and videospi apps hdc should be non-null but pOW2View and pDC should be null)
{
	CPen pen;
	CPen* pOldPen;
	int nOldDrawingMode;
	// Save the device context for modes
	pDC->SaveDC();
	nOldDrawingMode = pDC->SetROP2(R2_NOTXORPEN); //XOR mode, always have to erase what's drawn.

	pen.CreatePen(PS_SOLID, 0, RGB(0, 0, 0)); // 0 width = 1 device unit
	pOldPen = pDC->SelectObject(&pen);

	COW2Doc* pDoc = pOW2View->GetDocument();
	ASSERT(pDoc != NULL && m_pPOINTSET != NULL && idpointsetobject > -1
		&& idpointsetobject < GetNumberOfPoints());

	double* pfStatistics = pDoc->GetPointerToStatistics();
	int nStatPerPoint = pDoc->GetNumberOfStatPerPoint();
	int iStatBaseOffset = idpointsetobject * nStatPerPoint;
	double dfTreePosX = m_pPOINTSET->px[idpointsetobject];
	double dfTreePosY = m_pPOINTSET->py[idpointsetobject];
	double dfTreeDimX = pfStatistics[iStatBaseOffset + POINTSET_EXTRA_NUMBEROFSTAT + POINTSET_XYZI_TREEDIMX];
	double dfTreeDimY = pfStatistics[iStatBaseOffset + POINTSET_EXTRA_NUMBEROFSTAT + POINTSET_XYZI_TREEDIMY];
	double dfHalfX = dfTreeDimX / 2.0;
	double dfHalfY = dfTreeDimY / 2.0;
	RECT myRect;
	//top left
	POINT2DD myPoint2DD;
	POINT myPoint;
	myPoint2DD.x = dfTreePosX - dfHalfX;
	myPoint2DD.y = dfTreePosY - dfHalfY;
	myPoint = pOW2View->MapPointFromWorldToView(myPoint2DD);
	myRect.left = myPoint.x;
	myRect.top = myPoint.y;
	//bottom right
	myPoint2DD.x = dfTreePosX + dfHalfX;
	myPoint2DD.y = dfTreePosY + dfHalfY;
	myPoint = pOW2View->MapPointFromWorldToView(myPoint2DD);
	myRect.right = myPoint.x;
	myRect.bottom = myPoint.y;
	//draw an ellipse for each point (tree crown)
	pDC->Ellipse(&myRect);


	pDC->SelectObject(pOldPen);
	pDC->SetROP2(nOldDrawingMode);
	pDC->RestoreDC(-1);
	return;
}

/*
BOOL CPointset::DrawTriangleset(COW2View* pOW2View,
	CDC* pDC,
	RECT* pViewRECT,
	int idrawingflag)
{
	COW2Doc* pDoc = pOW2View->GetDocument();
	if (pViewRECT == NULL)
	{
		ASSERT(FALSE);

		if (pDoc->m_pROIset)
		{
			int idroi = pDoc->m_pROIset->GetSelectedROI();
			*pViewRECT = pDoc->m_pROIset->GetRectROI(idroi);
		}
		else
		{
			*pViewRECT = pDoc->GetWorldRect();
		}

	}

	//draw a sublist of triangle from pointset

	POINT myP1;
	POINT myP2;
	myP1.x = pViewRECT->left;
	myP1.y = pViewRECT->top;
	myP2.x = pViewRECT->right;
	myP2.y = pViewRECT->bottom;

	//convert to OW2Doc coordinate system
	POINT2DD P1 = pOW2View->MapPointFromViewToWorld(myP1);
	POINT2DD P2 = pOW2View->MapPointFromViewToWorld(myP2);

	RECT2DD myRECT2DD;
	myRECT2DD.left = P1.x;
	myRECT2DD.top = P1.y;
	myRECT2DD.right = P2.x;
	myRECT2DD.bottom = P2.y;

	//edge
	int nvertex_max = (int)(fabs(P2.x - P1.x) + fabs(P2.y - P1.y));
	int nvertex_max2 = 0;
	int ntri_max = nvertex_max; //2*nvertex_max;
	int ntrifound = 0;
	int nvertexfound = 0;
	int nvertexfound2 = 0;
	EDGEFILLPARAM myEDGEFILLPARAM;
	myEDGEFILLPARAM.pIDTRI = (int*) new int[ntri_max];
	myEDGEFILLPARAM.ntri_max = ntri_max;
	myEDGEFILLPARAM.p_ntrifound = &ntrifound;
	myEDGEFILLPARAM.p_itriseed = &(pOW2View->m_itriseed);

	myEDGEFILLPARAM.pIDVERTEX = (int*) new int[nvertex_max];
	myEDGEFILLPARAM.nvertex_max = nvertex_max;
	myEDGEFILLPARAM.p_nvertexfound = &nvertexfound;

	//inner edge bounding rect
	int ntri_iebr_max = ntri_max * ntri_max / 2;
	int nvertex_iebr_max = nvertex_max * nvertex_max / 2;
	int ntrifound_iebr = 0;
	int nvertexfound_iebr = 0;
	myEDGEFILLPARAM.pIDTRI_iebr = (int*) new int[ntri_iebr_max];
	myEDGEFILLPARAM.ntri_iebr_max = ntri_iebr_max;
	myEDGEFILLPARAM.p_ntrifound_iebr = &ntrifound_iebr;
	myEDGEFILLPARAM.pIDVERTEX_iebr = (int*) new int[nvertex_iebr_max];
	myEDGEFILLPARAM.nvertex_iebr_max = nvertex_iebr_max;
	myEDGEFILLPARAM.p_nvertexfound_iebr = &nvertexfound_iebr;

	//outter edge bounding rect
	int ntri_oebr_max = ntri_max * ntri_max / 2;
	int nvertex_oebr_max = nvertex_max * nvertex_max / 2;
	int ntrifound_oebr = 0;
	int nvertexfound_oebr = 0;
	myEDGEFILLPARAM.pIDTRI_oebr = (int*) new int[ntri_oebr_max];
	myEDGEFILLPARAM.ntri_oebr_max = ntri_oebr_max;
	myEDGEFILLPARAM.p_ntrifound_oebr = &ntrifound_oebr;
	myEDGEFILLPARAM.pIDVERTEX_oebr = (int*) new int[nvertex_oebr_max];
	myEDGEFILLPARAM.nvertex_oebr_max = nvertex_oebr_max;
	myEDGEFILLPARAM.p_nvertexfound_oebr = &nvertexfound_oebr;

	int ntri_interior = pDoc->m_pPointset->RectFill(&myRECT2DD, &myEDGEFILLPARAM);

	//these 3 sets represent the rect interior 
	int nIDTRI[3];
	nIDTRI[0] = myEDGEFILLPARAM.p_ntrifound[0]; //edge
	nIDTRI[1] = myEDGEFILLPARAM.p_ntrifound_iebr[0]; //iebr
	nIDTRI[2] = myEDGEFILLPARAM.p_ntrifound_oebr[0]; //iebr
	int ntri = nIDTRI[0] + nIDTRI[1] + nIDTRI[2];
	IDTRI* pIDTRI[3];
	pIDTRI[0] = myEDGEFILLPARAM.pIDTRI;
	pIDTRI[1] = myEDGEFILLPARAM.pIDTRI_iebr;
	pIDTRI[2] = myEDGEFILLPARAM.pIDTRI_oebr;
	ASSERT(ntri_interior == ntri);
	if (ntri_interior > 0)
	{
		//edgefill() success
		for (int i = 0; i < 3; i++)
		{
			DrawTriangleset(pOW2View, pDC, pIDTRI[i], nIDTRI[i], idrawingflag);
		}
	}
	else
	{
		//edgefill() error
		CString myString;
		myString.Format(L"warning, no triangle collected in DrawTriangleset() for RECT (%d,%d) (%d,%d)\r\n", pViewRECT->left, pViewRECT->top, pViewRECT->right, pViewRECT->bottom);
		((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
	}
	if (myEDGEFILLPARAM.pIDTRI) delete[] myEDGEFILLPARAM.pIDTRI;
	if (myEDGEFILLPARAM.pIDVERTEX) delete[] myEDGEFILLPARAM.pIDVERTEX;
	if (myEDGEFILLPARAM.pIDTRI_iebr) delete[] myEDGEFILLPARAM.pIDTRI_iebr;
	if (myEDGEFILLPARAM.pIDVERTEX_iebr) delete[] myEDGEFILLPARAM.pIDVERTEX_iebr;
	if (myEDGEFILLPARAM.pIDTRI_oebr) delete[] myEDGEFILLPARAM.pIDTRI_oebr;
	if (myEDGEFILLPARAM.pIDVERTEX_oebr) delete[] myEDGEFILLPARAM.pIDVERTEX_oebr;
	return TRUE;
}
*/


BOOL CPointset::DrawTriangleset(COW2View* pOW2View,
								CDC* pDC,
								RECT* pViewRECT,
								int idrawingflag,
								HDC hdc /*=NULL*/, //when used by audiospi and videospi apps hdc should be non-null but pOW2View and pDC should be null)
								int* p_itriseed/*=NULL*/)
{
	COW2Doc* pDoc = NULL;
	if(pOW2View) pDoc = pOW2View->GetDocument();
	if(pViewRECT==NULL)
	{
		ASSERT(FALSE);
		if (pDoc)
		{
			if (pDoc->m_pROIset)
			{
				int idroi = pDoc->m_pROIset->GetSelectedROI();
				*pViewRECT = pDoc->m_pROIset->GetRectROI(idroi);
			}
			else
			{
				*pViewRECT = pDoc->GetWorldRect();
			}
		}		
	}
	int itriseed = 0;
	if (pOW2View == NULL && p_itriseed == NULL)
	{
		p_itriseed = &itriseed;
	}
	//draw a sublist of triangle from pointset

	/*
	//collect all triangles from viewrect edges
	POINT P1[4];
	POINT P2[4];
	P1[0].x = pViewRect->left;
	P1[0].y = pViewRect->top;
	P2[0].x = pViewRect->right;
	P2[0].y = pViewRect->top;

	P1[1].x = pViewRect->right;
	P1[1].y = pViewRect->top;
	P2[1].x = pViewRect->right;
	P2[1].y = pViewRect->bottom;

	P1[2].x = pViewRect->right;
	P1[2].y = pViewRect->bottom;
	P2[2].x = pViewRect->left;
	P2[2].y = pViewRect->bottom;

	P1[3].x = pViewRect->left;
	P1[3].y = pViewRect->bottom;
	P2[3].x = pViewRect->left;
	P2[3].y = pViewRect->top;

	for(int i=0; i<4; i++) //4 rect edges
	{	
		POINT2DD myP1 = pOW2View->MapPointFromViewToWorld(P1[i]);
		POINT2DD myP2 = pOW2View->MapPointFromViewToWorld(P2[i]);

		const int nvertex_max = 1000; //grid one pixel unit
		const int ntri_max = nvertex_max*2; //estimate of maximum number of triangle to be possibly found
		IDTRI pIDTRI[ntri_max];
		int ntrifound = 0;
		IDVERTEX pIDVERTEX[nvertex_max];
		int nvertexfound = 0;

		const int nvertex_max2 = nvertex_max;
		IDVERTEX pIDVERTEX2[nvertex_max2];
		int nvertexfound2 = 0;
		
		int ntri = EdgeFill(	myP1, 
								myP2, 
								pIDTRI, //allocated by caller to collect all intermediate points between P1 and P2 
								ntri_max, //maximum number of IDTRI allocated by caller
								&ntrifound,
								&(pOW2View->m_itriseed),
								pIDVERTEX, //allocated by caller to collect all intermediate points between P1 and P2 
								nvertex_max, //maximum number of IDVERTEX allocated by caller
								&nvertexfound,
								pIDVERTEX2,
								nvertex_max2,
								&nvertexfound2);
		if(ntri>0)
		{
			//edgefill() success
			DrawTriangleset(pOW2View, pDC, pIDTRI, ntrifound, idrawingflag);
		}
		else
		{
			//edgefill() error
		}
	}
	*/

	POINT myP1;
	POINT myP2;
	myP1.x = pViewRECT->left;
	myP1.y = pViewRECT->top;
	myP2.x = pViewRECT->right;
	myP2.y = pViewRECT->bottom;

	//convert to OW2Doc coordinate system
	POINT2DD P1;
	POINT2DD P2;
	if (pOW2View)
	{
		P1 = pOW2View->MapPointFromViewToWorld(myP1);
		P2 = pOW2View->MapPointFromViewToWorld(myP2);
	}
	else if (hdc)
	{
		P1 = MapPointFromViewToWorld(myP1);
		P2 = MapPointFromViewToWorld(myP2);
	}

	RECT2DD myRECT2DD;
	myRECT2DD.left = P1.x;
	myRECT2DD.top = P1.y;
	myRECT2DD.right = P2.x;
	myRECT2DD.bottom = P2.y;

	//edge
	int nvertex_max = (int) (fabs(P2.x-P1.x) + fabs(P2.y-P1.y));
	int nvertex_max2 = 0;
	int ntri_max = nvertex_max; //2*nvertex_max;
	int ntrifound = 0;
	int nvertexfound = 0;
	int nvertexfound2 = 0;
	EDGEFILLPARAM myEDGEFILLPARAM;
	myEDGEFILLPARAM.pIDTRI = (int*) new int[ntri_max];		
	myEDGEFILLPARAM.ntri_max = ntri_max;		
	myEDGEFILLPARAM.p_ntrifound = &ntrifound;
	if (pOW2View)
	{
		myEDGEFILLPARAM.p_itriseed = &(pOW2View->m_itriseed);
	}
	else if (hdc)
	{
		myEDGEFILLPARAM.p_itriseed = p_itriseed;
	}
	myEDGEFILLPARAM.pIDVERTEX = (int*) new int[nvertex_max];		
	myEDGEFILLPARAM.nvertex_max = nvertex_max;	
	myEDGEFILLPARAM.p_nvertexfound = &nvertexfound;

	//inner edge bounding rect
	int ntri_iebr_max = ntri_max * ntri_max/2;
	int nvertex_iebr_max = nvertex_max * nvertex_max/2;
	int ntrifound_iebr = 0;
	int nvertexfound_iebr = 0;
	myEDGEFILLPARAM.pIDTRI_iebr = (int*) new int[ntri_iebr_max];	
	myEDGEFILLPARAM.ntri_iebr_max = ntri_iebr_max;	
	myEDGEFILLPARAM.p_ntrifound_iebr = &ntrifound_iebr;
	myEDGEFILLPARAM.pIDVERTEX_iebr = (int*) new int[nvertex_iebr_max];
	myEDGEFILLPARAM.nvertex_iebr_max = nvertex_iebr_max;	
	myEDGEFILLPARAM.p_nvertexfound_iebr = &nvertexfound_iebr;
	
	//outter edge bounding rect
	int ntri_oebr_max = ntri_max * ntri_max/2;
	int nvertex_oebr_max = nvertex_max * nvertex_max/2;
	int ntrifound_oebr = 0;
	int nvertexfound_oebr = 0;
	myEDGEFILLPARAM.pIDTRI_oebr = (int*) new int[ntri_oebr_max];	
	myEDGEFILLPARAM.ntri_oebr_max = ntri_oebr_max;	
	myEDGEFILLPARAM.p_ntrifound_oebr = &ntrifound_oebr;
	myEDGEFILLPARAM.pIDVERTEX_oebr = (int*) new int[nvertex_oebr_max];
	myEDGEFILLPARAM.nvertex_oebr_max = nvertex_oebr_max;	
	myEDGEFILLPARAM.p_nvertexfound_oebr = &nvertexfound_oebr;

	int ntri_interior = pDoc->m_pPointset->RectFill(&myRECT2DD, &myEDGEFILLPARAM);

	//these 3 sets represent the rect interior 
	int nIDTRI[3];
	nIDTRI[0] = myEDGEFILLPARAM.p_ntrifound[0]; //edge
	nIDTRI[1] = myEDGEFILLPARAM.p_ntrifound_iebr[0]; //iebr
	nIDTRI[2] = myEDGEFILLPARAM.p_ntrifound_oebr[0]; //iebr
	int ntri = nIDTRI[0] + nIDTRI[1] + nIDTRI[2];
	IDTRI* pIDTRI[3];
	pIDTRI[0] = myEDGEFILLPARAM.pIDTRI;
	pIDTRI[1] = myEDGEFILLPARAM.pIDTRI_iebr;
	pIDTRI[2] = myEDGEFILLPARAM.pIDTRI_oebr;
	ASSERT(ntri_interior==ntri);
	if(ntri_interior>0)
	{
		//edgefill() success
		for(int i=0; i<3; i++)
		{
			DrawTriangleset(pOW2View, pDC, pIDTRI[i], nIDTRI[i], idrawingflag, hdc);
		}
	}
	else
	{
		//edgefill() error
		CString myString;
		myString.Format(L"warning, no triangle collected in DrawTriangleset() for RECT (%d,%d) (%d,%d)\r\n", pViewRECT->left, pViewRECT->top, pViewRECT->right, pViewRECT->bottom);
		((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
	}
	if(myEDGEFILLPARAM.pIDTRI) delete[] myEDGEFILLPARAM.pIDTRI;
	if(myEDGEFILLPARAM.pIDVERTEX) delete[] myEDGEFILLPARAM.pIDVERTEX;
	if(myEDGEFILLPARAM.pIDTRI_iebr) delete[] myEDGEFILLPARAM.pIDTRI_iebr;
	if(myEDGEFILLPARAM.pIDVERTEX_iebr) delete[] myEDGEFILLPARAM.pIDVERTEX_iebr;
	if(myEDGEFILLPARAM.pIDTRI_oebr) delete[] myEDGEFILLPARAM.pIDTRI_oebr;
	if(myEDGEFILLPARAM.pIDVERTEX_oebr) delete[] myEDGEFILLPARAM.pIDVERTEX_oebr;
	return TRUE;
}

/*
//draw an IDTRI list of triangle
BOOL CPointset::DrawTriangleset(class COW2View* pOW2View, 
								CDC* pDC,  
								IDTRI* pIDTRI,
								int ntrifound,
								int idrawingflag)// = POINTSET_DRAW_TRIANGLE_AS_3VERTEX)
{
	for(int j=0; j<ntrifound; j++)
	{
		int idtri = pIDTRI[j];
		DrawTriangleset(pOW2View, pDC, idtri, idrawingflag);
	}
	return TRUE;
}
*/

//draw an IDTRI list of triangle
BOOL CPointset::DrawTriangleset(class COW2View* pOW2View,
								class CDC* pDC,
								IDTRI* pIDTRI,
								int ntrifound,
								int idrawingflag/*= POINTSET_DRAW_TRIANGLE_AS_3VERTEX*/,
								HDC hdc/*=NULL*/) //when used by audiospi and videospi apps hdc should be non-null but pOW2View and pDC should be null)
{
	for (int j = 0; j < ntrifound; j++)
	{
		int idtri = pIDTRI[j];
		DrawTriangleset(pOW2View, pDC, idtri, idrawingflag, hdc);
	}
	return TRUE;
}

/*
//draw triangle, one triangle if specifying idtri (other than -1)
//idtri -1 (POINTSET_ALL) to draw all triangles
BOOL CPointset::DrawTriangleset(COW2View* pOW2View, 
								CDC* pDC, 
								int idtri, 
								int idrawingflag)
{
	if(pOW2View==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	int i,k, ivertex;
	POINT2DD myPoint2DD[3];
	POINT myPoint[3];
	double* pfStatistics;
	int nStatPerPoint;

	pfStatistics = GetPointerToStatistics();
	nStatPerPoint = GetNumberOfStatPerPoint(); 
    
	//draw all triangles from pointset
	int istart = 0;
	int iend = GetNumberOfTriangles();
	if(idtri!=POINTSET_ALL) 
	{
		//draw one triangle, idtri
		istart = idtri;
		iend = idtri;
	}
	for (i=istart; i<=iend; i++)
	{
		//
		//Pour chacun des triangles valides
		//
		if(!InvalidTri(i))
		{
			//obtenir les coordonnees du triangle
			for(k=0; k<3; k++)
			{
				ivertex = m_pPOINTSET->vt[k][i];
				myPoint2DD[k].x = m_pPOINTSET->px[ivertex];
				myPoint2DD[k].y = m_pPOINTSET->py[ivertex];
				myPoint[k] = pOW2View->MapPointFromWorldToView( myPoint2DD[k]);
			}
			if(idrawingflag & POINTSET_DRAW_TRIANGLESET)
			{
				//dessiner le triangle
				pDC->MoveTo(myPoint[0].x, myPoint[0].y);
				pDC->LineTo(myPoint[1].x, myPoint[1].y);
				pDC->LineTo(myPoint[2].x, myPoint[2].y);
				pDC->LineTo(myPoint[0].x, myPoint[0].y);
			}
			if(idrawingflag & POINTSET_DRAW_TRIANGLESET_LABEL)
			{
				//dessiner le numero du triangle
				WCHAR text[50];
				//myPoint2DD[0].x = m_pPOINTSET->ctx[i];
				//myPoint2DD[0].y = m_pPOINTSET->cty[i];
				//geometric center
				myPoint2DD[0].x = (m_pPOINTSET->px[m_pPOINTSET->vt[0][i]] + m_pPOINTSET->px[m_pPOINTSET->vt[1][i]] + m_pPOINTSET->px[m_pPOINTSET->vt[2][i]])/3.0;
				myPoint2DD[0].y = (m_pPOINTSET->py[m_pPOINTSET->vt[0][i]] + m_pPOINTSET->py[m_pPOINTSET->vt[1][i]] + m_pPOINTSET->py[m_pPOINTSET->vt[2][i]])/3.0;

				myPoint[0] = pOW2View->MapPointFromWorldToView( myPoint2DD[0]);
				swprintf_s(text,50, L"%d", i);
				int ncount = wcslen(text);
				CSize sizeText = pDC->GetTextExtent(text);
				int ix_offset = sizeText.cx/2;
				int iy_offset = sizeText.cy/2;
				pDC->TextOut( myPoint[0].x - ix_offset, myPoint[0].y - iy_offset, text, ncount);
			}
			if(idrawingflag & POINTSET_DRAW_TRIANGLESET_CENTER_LABEL)
			{
				//dessiner une croix au centre du triangle
				myPoint2DD[0].x = m_pPOINTSET->ctx[i];
				myPoint2DD[0].y = m_pPOINTSET->cty[i];
				myPoint[0] = pOW2View->MapPointFromWorldToView( myPoint2DD[0]);
				pDC->MoveTo(myPoint[0].x-1, myPoint[0].y);
				pDC->LineTo(myPoint[1].x+1, myPoint[1].y);
				pDC->LineTo(myPoint[2].x, myPoint[2].y-1);
				pDC->LineTo(myPoint[0].x, myPoint[0].y+1);
			}
		}
	}
	return TRUE;
}
*/

//draw triangle, one triangle if specifying idtri (other than -1)
//idtri -1 (POINTSET_ALL) to draw all triangles
BOOL CPointset::DrawTriangleset(COW2View* pOW2View,
								CDC* pDC,
								int idtri/*=POINTSET_ALL*/,
								int idrawingflag/*=POINTSET_DRAW_TRIANGLESET_AS_3VERTEX*/,
								HDC hdc/*=NULL*/) //when used by audiospi and videospi apps hdc should be non-null but pOW2View and pDC should be null)
{
	/*
	if (pOW2View == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	*/
	int i, k, ivertex;
	POINT2DD myPoint2DD[3];
	POINT myPoint[3];
	double* pfStatistics=NULL;
	int nStatPerPoint;
	pfStatistics = GetPointerToStatistics();
	nStatPerPoint = GetNumberOfStatPerPoint();

	//draw all triangles (or a single triangle) from pointset
	int istart = 0;
	int iend = GetNumberOfTriangles();
	if (idtri != POINTSET_ALL)
	{
		//draw one triangle, idtri
		istart = idtri;
		iend = idtri;
	}
	for (i = istart; i <= iend; i++)
	{
		//
		//Pour chacun des triangles valides
		//For each valid triangle
		//
		if (!InvalidTri(i))
		{
			//obtenir les coordonnees du triangle
			//get triangle coordinates
			for (k = 0; k < 3; k++)
			{
				ivertex = m_pPOINTSET->vt[k][i];
				myPoint2DD[k].x = m_pPOINTSET->px[ivertex];
				myPoint2DD[k].y = m_pPOINTSET->py[ivertex];
				if (pOW2View)
				{
					myPoint[k] = pOW2View->MapPointFromWorldToView(myPoint2DD[k]);
				}
				else if (hdc)
				{
					myPoint[k] = MapPointFromWorldToView(myPoint2DD[k]);
				}
			}
			if (idrawingflag & POINTSET_DRAW_TRIANGLESET)
			{
				//dessiner le triangle
				//draw triangle
				if (pDC)
				{
					pDC->MoveTo(myPoint[0].x, myPoint[0].y);
					pDC->LineTo(myPoint[1].x, myPoint[1].y);
					pDC->LineTo(myPoint[2].x, myPoint[2].y);
					pDC->LineTo(myPoint[0].x, myPoint[0].y);
				}
				else if (hdc)
				{
					bool bresult;
					bresult = MoveToEx(hdc, myPoint[0].x, myPoint[0].y, NULL);
					bresult = LineTo(hdc, myPoint[1].x, myPoint[1].y);
					bresult = LineTo(hdc, myPoint[2].x, myPoint[2].y);
					bresult = LineTo(hdc, myPoint[0].x, myPoint[0].y);
				}
			}
			if (idrawingflag & POINTSET_DRAW_TRIANGLESET_LABEL)
			{
				//dessiner le numero du triangle
				WCHAR text[50];
				//myPoint2DD[0].x = m_pPOINTSET->ctx[i];
				//myPoint2DD[0].y = m_pPOINTSET->cty[i];
				//geometric center
				myPoint2DD[0].x = (m_pPOINTSET->px[m_pPOINTSET->vt[0][i]] + m_pPOINTSET->px[m_pPOINTSET->vt[1][i]] + m_pPOINTSET->px[m_pPOINTSET->vt[2][i]]) / 3.0;
				myPoint2DD[0].y = (m_pPOINTSET->py[m_pPOINTSET->vt[0][i]] + m_pPOINTSET->py[m_pPOINTSET->vt[1][i]] + m_pPOINTSET->py[m_pPOINTSET->vt[2][i]]) / 3.0;
				if (pOW2View)
				{
					myPoint[0] = pOW2View->MapPointFromWorldToView(myPoint2DD[0]);
				}
				else if (hdc)
				{
					myPoint[0] = MapPointFromWorldToView(myPoint2DD[0]);
				}
				swprintf_s(text, 50, L"%d", i);
				int ncount = wcslen(text);
				CSize sizeText;
				if (pDC)
				{
					sizeText = pDC->GetTextExtent(text);
				}
				else if (hdc)
				{
					//dwExtent = GetTextExtent(hDC, lpString, nCount);
					bool bresult = GetTextExtentPoint32(hdc, text, ncount, &sizeText);
				}
				int ix_offset = sizeText.cx / 2;
				int iy_offset = sizeText.cy / 2;
				if (pDC)
				{
					pDC->TextOut(myPoint[0].x - ix_offset, myPoint[0].y - iy_offset, text, ncount);
				}
				else if (hdc)
				{
					TextOut(hdc, myPoint[0].x - ix_offset, myPoint[0].y - iy_offset, text, ncount);
				}
			}
			if (idrawingflag & POINTSET_DRAW_TRIANGLESET_CENTER_LABEL)
			{
				//dessiner une croix au centre du triangle
				myPoint2DD[0].x = m_pPOINTSET->ctx[i];
				myPoint2DD[0].y = m_pPOINTSET->cty[i];
				if (pOW2View)
				{
					myPoint[0] = pOW2View->MapPointFromWorldToView(myPoint2DD[0]);
				}
				else if (hdc)
				{
					myPoint[0] = MapPointFromWorldToView(myPoint2DD[0]);
				}
				if (pDC)
				{
					pDC->MoveTo(myPoint[0].x - 1, myPoint[0].y);
					pDC->LineTo(myPoint[1].x + 1, myPoint[1].y);
					pDC->LineTo(myPoint[2].x, myPoint[2].y - 1);
					pDC->LineTo(myPoint[0].x, myPoint[0].y + 1);
				}
				else if (hdc)
				{
					bool bresult;
					bresult = MoveToEx(hdc, myPoint[0].x - 1, myPoint[0].y, NULL);
					bresult = LineTo(hdc, myPoint[1].x + 1, myPoint[1].y);
					bresult = LineTo(hdc, myPoint[2].x, myPoint[2].y - 1);
					bresult = LineTo(hdc, myPoint[0].x, myPoint[0].y + 1);
				}
			}
		}
	}
	return TRUE;
}


struct DrawLineInfipWinImage_userdata {
	fipWinImage* pfipWinImage;
	COW2Doc* pOW2Doc;
	//2020july04, spi, begin
	int voronoisetedgesfilterflag;
	int voronoisetpointsfilterflag;
	COW2Doc* pAlternateColorsOW2Doc;
	int voronoisetedgesmapperflag;
	int voronoisetpointsmapperflag;
	//2020july04, spi, end
};



VOID CALLBACK DrawLineInfipWinImage(int X,int Y,LPARAM lpData)
{
	RGBQUAD myRGBQUAD;
	myRGBQUAD.rgbBlue = 255;
	myRGBQUAD.rgbGreen = 255;
	myRGBQUAD.rgbRed = 255;
	DrawLineInfipWinImage_userdata* puserdata;
    puserdata = (DrawLineInfipWinImage_userdata*)lpData;
	if(X>-1 && X<puserdata->pfipWinImage->getWidth() && Y>-1 && Y<puserdata->pfipWinImage->getHeight())
	{
		myRGBQUAD.rgbBlue = puserdata->pOW2Doc->m_pPixelset->GetPixelValue(X, Y, 2);
		myRGBQUAD.rgbGreen = puserdata->pOW2Doc->m_pPixelset->GetPixelValue(X, Y, 1);
		myRGBQUAD.rgbRed = puserdata->pOW2Doc->m_pPixelset->GetPixelValue(X, Y, 0);
		puserdata->pOW2Doc->fTempAverageLineColor_B += myRGBQUAD.rgbBlue/255.0;
		puserdata->pOW2Doc->fTempAverageLineColor_G += myRGBQUAD.rgbGreen / 255.0;
		puserdata->pOW2Doc->fTempAverageLineColor_R += myRGBQUAD.rgbRed / 255.0;
		puserdata->pOW2Doc->iTempAverageLineCount++;
		//2020july04, spi, begin
		//puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
		if (puserdata->voronoisetedgesfilterflag==VORONOISET_COLOR_ALL)
		{
			if (!(puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD)))
			{
				return;
			}
		}
		else
		{
			bool enablecolormapper = false;
			RGBQUAD myaltRGBQUAD;
			myaltRGBQUAD.rgbBlue = 255;
			myaltRGBQUAD.rgbGreen = 255;
			myaltRGBQUAD.rgbRed = 255;
			if (puserdata->pAlternateColorsOW2Doc!=NULL && puserdata->pAlternateColorsOW2Doc->m_pPixelset!=NULL
				&& puserdata->voronoisetedgesmapperflag!=VORONOISET_COLOR_MAP_NONE)
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

			if (puserdata->voronoisetedgesfilterflag & VORONOISET_COLOR_HUMANSKIN)
			{
				if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_HUMANSKIN))
				{
					if (enablecolormapper == false)
					{
						puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
					}
					else if (puserdata->voronoisetedgesmapperflag & VORONOISET_COLOR_MAP_HUMANSKIN)
					{
						puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
					}
				}
			}
			if (puserdata->voronoisetedgesfilterflag & VORONOISET_COLOR_RED)
			{
				if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_RED))
				{
					if (enablecolormapper == false)
					{
						puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
					}
					else if (puserdata->voronoisetedgesmapperflag & VORONOISET_COLOR_MAP_RED)
					{
						puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
					}
				}
			}
			if (puserdata->voronoisetedgesfilterflag & VORONOISET_COLOR_GREEN)
			{
				if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_GREEN))
				{
					if (enablecolormapper == false)
					{
						puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
					}
					else if (puserdata->voronoisetedgesmapperflag & VORONOISET_COLOR_MAP_GREEN)
					{
						puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
					}
				}
			}
			if (puserdata->voronoisetedgesfilterflag & VORONOISET_COLOR_BLUE)
			{
				if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_BLUE))
				{
					if (enablecolormapper == false)
					{
						puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
					}
					else if (puserdata->voronoisetedgesmapperflag & VORONOISET_COLOR_MAP_BLUE)
					{
						puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
					}
				}
			}
			if (puserdata->voronoisetedgesfilterflag & VORONOISET_COLOR_YELLOW)
			{
				if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_YELLOW))
				{
					if (enablecolormapper == false)
					{
						puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
					}
					else if (puserdata->voronoisetedgesmapperflag & VORONOISET_COLOR_MAP_YELLOW)
					{
						puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
					}
				}
			}
			if (puserdata->voronoisetedgesfilterflag & VORONOISET_COLOR_ORANGE)
			{
				if (IsInHSVColorRange(myHsvColor, UTILITY_HSVCOLORRANGE_TYPE_ORANGE))
				{
					if (enablecolormapper == false)
					{
						puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myRGBQUAD);
					}
					else if (puserdata->voronoisetedgesmapperflag & VORONOISET_COLOR_MAP_ORANGE)
					{
						puserdata->pfipWinImage->setPixelColor(X, puserdata->pfipWinImage->getHeight() - Y, &myaltRGBQUAD);
					}
				}
			}

		}
		//2020july04, spi, end
	}
}
//(LINEDDAPROC)DrawLineInfipWinImage,(long)pfipWinImage


BOOL CPointset::DrawVoronoiset(class COW2View* pOW2View, 
							   CDC* pDC, 
							   int idpoint, //=POINTSET_ALL, 
							   CRect* pViewRect, //=NULL);
							   int idrawingflag,
							   class fipWinImage* pfipWinImage,
							   float portionofvoronoipolygonstofill, // defaults to 0.0f, //0.0 for none, 1.0 for all, 0.5 for 50%, etc.
							   int inverseprob_onpixelfill, //) //defaults to 1  //1 for all, 2 for 50%, 3 for 33.33%, 4 for 25%, 5 for 20%, etc.
								//2020july04, spi, begin
								int voronoisetedgesfilterflag, int voronoisetpointsfilterflag,
								COW2Doc* pAlternateColorsOW2Doc, int voronoisetedgesmapperflag, int voronoisetpointsmapperflag)
								//2020july04, spi, end
{
	//spi, nov 2018, begin
	COWPolygonset myOWPolygonset;
	std::vector<int> myvector;
	if (pfipWinImage && portionofvoronoipolygonstofill > 0.0f)
	{
		if (portionofvoronoipolygonstofill > 1.0) portionofvoronoipolygonstofill = 1.0;

	}
	//spi, nov 2018, end

		//for geometric debugging
	/*
	char pszMessage[512];
	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	pLogDocument->AddText("Creating xpoint.dat file in default directory ... ");
	FILE* pFile;
	if ( ( pFile = fopen ( "xpoint.dat", "w" ) ) == NULL )
	{
		wsprintf(pszMessage, "Error Creating %s", "xpoint.dat");
		pLogDocument->AddText("Error\r\n");
		return;
	}
 	pLogDocument->AddText("OK\r\n");
	*/

	int ivertex,j;
	int numtrifound, numneighborfound, itriseed;
	int p_arraytri[200];
	int p_arrayneighbor[200];
	POINT pPoint[100];

	// the triangle seed must be an existing triangle index 
	itriseed = 0;
	for (ivertex=0; ivertex<GetNumberOfPoints(); ivertex++)
	{
		this->m_pPOINTSET->color_r[ivertex] = 0.0;
		this->m_pPOINTSET->color_g[ivertex] = 0.0;
		this->m_pPOINTSET->color_b[ivertex] = 0.0;
		this->m_pPOINTSET->color_a[ivertex] = 0.0;
		//2012august13, poirier, begin
		//skip drawing when previously "computed/or tagged" voronoi area is negative
		double dfValue = GetStatisticsValue(ivertex, POINTSET_OFFSETSTAT_VOROAREA);
		if(dfValue<0) 
		{
			continue;
		}
		//2012august13, poirier, end
		//
		//Pour chacun des points qui n'ont aucun triangle adjacents
		//touchant directement la frontiere convexe ...
		//
		if( FindAllValidTriSurroundingVertex( 	m_pPOINTSET,
							       				ivertex,
							       				&itriseed,
							       				&numtrifound,
							       				p_arraytri,
							       				&numneighborfound,
							       				p_arrayneighbor) == TRUE)
		{
			//if all surrounding triangles are valid,		 
			// build voronoi polygon using each adjtri's center 
			ASSERT(numtrifound<200); //development-time error, if allowed, define p_arraytri[n] with n>200
			for(j=0; j<numtrifound; j++)
			{
				POINT2DD myPoint2DD;
				myPoint2DD.x = m_pPOINTSET->ctx[p_arraytri[j]];
				myPoint2DD.y = m_pPOINTSET->cty[p_arraytri[j]];
				pPoint[j] = pOW2View->MapPointFromWorldToView( myPoint2DD);
			}
			// To close the polygon, the last point is equal to the first point 
			pPoint[numtrifound].x = pPoint[0].x;
			pPoint[numtrifound].y = pPoint[0].y;

			//draw the voronoi polygon
			if(pDC) pDC->Polygon(pPoint, numtrifound+1); //+1 for the last point
			if(pfipWinImage)
			{
				pOW2View->GetDocument()->fTempAveragePolygonColor_R = 0.0;
				pOW2View->GetDocument()->fTempAveragePolygonColor_G = 0.0;
				pOW2View->GetDocument()->fTempAveragePolygonColor_B = 0.0;
				pOW2View->GetDocument()->iTempAveragePolygonCount = 0;
				for(int iii=0; iii<numtrifound; iii++)
				{
					//::LineDDA(pPoint[iii].x,pPoint[iii].y,pPoint[iii+1].x,pPoint[iii+1].y,(LINEDDAPROC)DrawLineInfipWinImage,(long)pfipWinImage);
					DrawLineInfipWinImage_userdata userdata;
					userdata.pfipWinImage = pfipWinImage;
					userdata.pOW2Doc = (COW2Doc*) pOW2View->GetDocument();
					userdata.pOW2Doc->fTempAverageLineColor_R = 0.0;
					userdata.pOW2Doc->fTempAverageLineColor_G = 0.0;
					userdata.pOW2Doc->fTempAverageLineColor_B = 0.0;
					userdata.pOW2Doc->iTempAverageLineCount = 0;
					//2020july04, spi, begin
					userdata.voronoisetedgesfilterflag = voronoisetedgesfilterflag;
					userdata.voronoisetpointsfilterflag = voronoisetpointsfilterflag;
					userdata.pAlternateColorsOW2Doc = pAlternateColorsOW2Doc;
					userdata.voronoisetedgesmapperflag = voronoisetedgesmapperflag;
					userdata.voronoisetpointsmapperflag = voronoisetpointsmapperflag;
					//2020july04, spi, end

					//2020june20, spi, begin
					//::LineDDA(pPoint[iii].x, pPoint[iii].y, pPoint[iii + 1].x, pPoint[iii + 1].y, (LINEDDAPROC)DrawLineInfipWinImage, (long)&userdata);
					::LineDDA(pPoint[iii].x, pPoint[iii].y, pPoint[iii + 1].x, pPoint[iii + 1].y, (LINEDDAPROC)DrawLineInfipWinImage, (LPARAM)&userdata);
					//2020june20, spi, end
					if (userdata.pOW2Doc->iTempAverageLineCount == 0)
					{
					}
					else
					{
						userdata.pOW2Doc->fTempAverageLineColor_R = userdata.pOW2Doc->fTempAverageLineColor_R / userdata.pOW2Doc->iTempAverageLineCount;
						userdata.pOW2Doc->fTempAverageLineColor_G = userdata.pOW2Doc->fTempAverageLineColor_G / userdata.pOW2Doc->iTempAverageLineCount;
						userdata.pOW2Doc->fTempAverageLineColor_B = userdata.pOW2Doc->fTempAverageLineColor_B / userdata.pOW2Doc->iTempAverageLineCount;
					}
					pOW2View->GetDocument()->fTempAveragePolygonColor_R += userdata.pOW2Doc->fTempAverageLineColor_R;
					pOW2View->GetDocument()->fTempAveragePolygonColor_G += userdata.pOW2Doc->fTempAverageLineColor_G;
					pOW2View->GetDocument()->fTempAveragePolygonColor_B += userdata.pOW2Doc->fTempAverageLineColor_B;
					pOW2View->GetDocument()->iTempAveragePolygonCount++;
				}
				pOW2View->GetDocument()->fTempAveragePolygonColor_R = pOW2View->GetDocument()->fTempAveragePolygonColor_R / pOW2View->GetDocument()->iTempAveragePolygonCount;
				pOW2View->GetDocument()->fTempAveragePolygonColor_G = pOW2View->GetDocument()->fTempAveragePolygonColor_G / pOW2View->GetDocument()->iTempAveragePolygonCount;
				pOW2View->GetDocument()->fTempAveragePolygonColor_B = pOW2View->GetDocument()->fTempAveragePolygonColor_B / pOW2View->GetDocument()->iTempAveragePolygonCount;
				this->m_pPOINTSET->color_r[ivertex] = pOW2View->GetDocument()->fTempAveragePolygonColor_R;
				this->m_pPOINTSET->color_g[ivertex] = pOW2View->GetDocument()->fTempAveragePolygonColor_G;
				this->m_pPOINTSET->color_b[ivertex] = pOW2View->GetDocument()->fTempAveragePolygonColor_B;
				this->m_pPOINTSET->color_a[ivertex] = 0.0;
			}
			//spi, nov 2018, begin
			if (pfipWinImage && portionofvoronoipolygonstofill>0.0f)
			{
				if (portionofvoronoipolygonstofill==1.0f)
				{
					//fill now
					myOWPolygonset.DoScanlineFillForPolygon(pPoint, numtrifound, pOW2View, pfipWinImage, inverseprob_onpixelfill, //);
					//2020july04, spi, begin
					voronoisetedgesfilterflag, voronoisetpointsfilterflag,
					pAlternateColorsOW2Doc, voronoisetedgesmapperflag, voronoisetpointsmapperflag);
					//2020july04, spi, end
				}
				else
				{
					//add polygon
					int idpolygon = myOWPolygonset.AddPolygon(pPoint, numtrifound);
					if (idpolygon != -1) myvector.push_back(idpolygon);
				}
			}
			//spi, nov 2018, end
		}
		else
		{
			/*
			fprintf(pFile, "vertex=%d, numneighbor=%d,  ",ivertex,numneighborfound);
			for(j=0; j<numneighborfound; j++)
			{
				fprintf(pFile, " %d", p_arrayneighbor[j]);
			}
			fprintf(pFile, "\n");
			*/
		}
	}
	//spi, nov 2018, begin
	if (pfipWinImage && portionofvoronoipolygonstofill > 0.0f && portionofvoronoipolygonstofill<1.0f)
	{
		int numberofpolygons = myOWPolygonset.GetNumberOfPolygon();
		int npolytofill = (int)(portionofvoronoipolygonstofill* numberofpolygons); //2020july03, spi
		for (int i = 0; i < npolytofill; i++)
		{
			int randomindex = random_int(0, myvector.size() - 1);
			int idpolygon = myvector[randomindex];
			myvector.erase(myvector.begin()+randomindex);
			myOWPolygonset.DoScanlineFillForPolygon(idpolygon, pOW2View, pfipWinImage, inverseprob_onpixelfill, //);
			//2020july04, spi, begin
			voronoisetedgesfilterflag, voronoisetpointsfilterflag,
			pAlternateColorsOW2Doc, voronoisetedgesmapperflag, voronoisetpointsmapperflag);
			//2020july04, spi, end
			if (myvector.size() < 2) break;
		}
	}
	//spi, nov 2018, end
	return TRUE;
}


//
//the basic version, providing mandatory parameters only,
//collects all triangles along an edge (a line segment) defined by between P1 and P2 
//
//usage:	EdgeFill(	POINT2DD P1, 
//						POINT2DD P2, 
//						IDTRI* pIDTRI, //allocated by caller to collect all intermediate points between P1 and P2 
//						int ntri_max, //maximum number of IDTRI allocated by caller
//						int* p_ntrifound,
//						int* p_itriseed,
//						IDVERTEX* pIDVERTEX, //allocated by caller to collect all intermediate points between P1 and P2 
//						int nvertex_max, //maximum number of IDVERTEX allocated by caller
//						int* p_nvertexfound,
//						IDVERTEX* pIDVERTEX2, //allocated by caller to collect all intermediate points between P1 and P2 
//						int nvertex_max2, //maximum number of IDVERTEX allocated by caller
//						int* p_nvertexfound2);
int CPointset::EdgeFill(	POINT2DD P1, 
							POINT2DD P2, 
							IDTRI* pIDTRI, //allocated by caller to collect all intermediate points between P1 and P2 
							int ntri_max, //maximum number of IDTRI allocated by caller
							int* p_ntrifound,
							int* p_itriseed,
							IDVERTEX* pIDVERTEX, //allocated by caller to collect all intermediate points between P1 and P2 
							int nvertex_max, //maximum number of IDVERTEX allocated by caller
							int* p_nvertexfound,
							IDVERTEX* pIDVERTEX2, //allocated by caller to collect all intermediate points between P1 and P2 
							int nvertex_max2, //maximum number of IDVERTEX allocated by caller
							int* p_nvertexfound2)
{
	EDGEFILLPARAM myEDGEFILLPARAM;
	//fill up mandatory parameters
	myEDGEFILLPARAM.pIDTRI = pIDTRI;
	myEDGEFILLPARAM.ntri_max = ntri_max;
	myEDGEFILLPARAM.p_ntrifound = p_ntrifound;
	myEDGEFILLPARAM.p_itriseed = p_itriseed;
	myEDGEFILLPARAM.pIDVERTEX = pIDVERTEX;
	myEDGEFILLPARAM.nvertex_max = nvertex_max;
	myEDGEFILLPARAM.p_nvertexfound = p_nvertexfound;
	myEDGEFILLPARAM.pIDVERTEX2 = pIDVERTEX2;
	myEDGEFILLPARAM.nvertex_max2 = nvertex_max2;
	myEDGEFILLPARAM.p_nvertexfound2 = p_nvertexfound2;
	//the other optional parameter are all left set to null (default)

	return EdgeFill(P1, P2, &myEDGEFILLPARAM);	//idvertex outter edge to bounding rect limits
}


//
//the basic version, providing mandatory parameters only,
//collects all triangles along an edge (a line segment) defined by between P1 and P2 
//
//usage:	EdgeFill(	POINT2DD P1, 
//						POINT2DD P2, 
//						IDTRI* pIDTRI, //allocated by caller to collect all intermediate points between P1 and P2 
//						int ntri_max, //maximum number of IDTRI allocated by caller
//						int* p_ntrifound,
//						int* p_itriseed,
//						IDVERTEX* pIDVERTEX, //allocated by caller to collect all intermediate points between P1 and P2 
//						int nvertex_max, //maximum number of IDVERTEX allocated by caller
//						int* p_nvertexfound,
//						IDVERTEX* pIDVERTEX2, //allocated by caller to collect all intermediate points between P1 and P2 
//						int nvertex_max2, //maximum number of IDVERTEX allocated by caller
//						int* p_nvertexfound2);
//
//in order to provide oiii with automated user input interpretation, 
//interpretation of an edge (line segment or boundary), 
//as defined by the user over the a scene (image or vectorial world), 
//requires analysis of points collected in that edge neighborhood
//(i.e., not only along the edge but also on each sides).
//
//that is why the inner and outter edge triangle and vertex (points) may be interesting 
//to collect. also, the inner edge bounding rect and outter edge bounding rect can be 
//collected (iebr and oebr are triangular surfaces bounded by either innner or outter edge 
//and going up towards edge bounding rect limits). 
//
//optionally, the following parameter can be used (in groups of 3, idtri inner edge, idvertex
///inner edge, idtri outter edge, idvertex outter edge, etc.)
//
//		IDTRI* pIDTRI_ie=NULL,		//idtri inner edge
//		int ntri_ie_max=0,			//idtri inner edge
//		int* p_ntrifound_ie=NULL,	//idtri inner edge
//		IDVERTEX* pIDVERTEX_ie=NULL,//idvertex inner edge
//		int nvertex_ie_max=0,		//idvertex inner edge
//		int* p_nvertexfound_ie=NULL,//idvertex inner edge
//		IDTRI* pIDTRI_oe=NULL,		//idtri outter edge
//		int ntri_oe_max=0,			//idtri outter edge
//		int* p_ntrifound_oe=NULL,	//idtri outter edge
//		IDVERTEX* pIDVERTEX_oe=NULL,//idvertex outter edge
//		int nvertex_oe_max=0,		//idvertex outter edge
//		int* p_nvertexfound_oe=NULL,//idvertex outter edge
//		IDTRI* pIDTRI_iebr=NULL,		//idtri inner edge to bounding rect limits
//		int ntri_iebr_max=0,			//idtri inner edge to bounding rect limits
//		int* p_ntrifound_iebr=NULL,		//idtri inner edge to bounding rect limits
//		IDVERTEX* pIDVERTEX_iebr=NULL,	//idvertex inner edge to bounding rect limits
//		int nvertex_iebr_max=0,			//idvertex inner edge to bounding rect limits
//		int* p_nvertexfound_iebr=NULL,	//idvertex inner edge to bounding rect limits
//		IDTRI* pIDTRI_oebr=NULL,		//idtri outter edge to bounding rect limits
//		int ntri_oebr_max=0,			//idtri outter edge to bounding rect limits
//		int* p_ntrifound_oebr=NULL,		//idtri outter edge to bounding rect limits
//		IDVERTEX* pIDVERTEX_oebr=NULL,	//idvertex outter edge to bounding rect limits
//		int nvertex_oebr_max=0,			//idvertex outter edge to bounding rect limits
//		int* p_nvertexfound_oebr=NULL)	//idvertex outter edge to bounding rect limits
int CPointset::EdgeFill(POINT2DD P1, 
						POINT2DD P2, 
						EDGEFILLPARAM* pEDGEFILLPARAM)	//idvertex outter edge to bounding rect limits
{
	pEDGEFILLPARAM->p_nvertexfound[0] = 0;
	pEDGEFILLPARAM->p_ntrifound[0] = 0;

	///////////////////////////
	//validate entry parameters
	///////////////////////////

	if(pEDGEFILLPARAM->pIDTRI==NULL || pEDGEFILLPARAM->pIDVERTEX==NULL)
	{
		//can't store any output, caller should allocate pIDTRI
		CLogDocument* pLogDocument = NULL;
		if(AfxGetApp()!=NULL) 
		{
			pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
			CString myString;
			myString.Format(L"Error calling EdgeFill(), either pIDTRI or pIDVERTEX is NULL\r\n");
			pLogDocument->AddTextW(myString);
		}
		ASSERT(FALSE);
		return -1;
	}
	if(pEDGEFILLPARAM->p_itriseed==NULL)
	{
		//warning, not optimal, but itriseed will be 0 
		CLogDocument* pLogDocument = NULL;
		if(AfxGetApp()!=NULL) 
		{
			pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
			CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
			CString myString;
			myString.Format(L"Error calling EdgeFill(), either p_itri_seed is NULL\r\n");
			pLogDocument->AddTextW(myString);
		}
		ASSERT(FALSE);
		return -1;
	}
	int nvertex_upperlimit = (int)(fabs(P2.x-P1.x)+fabs(P2.y-P1.y)); //estimate, worst case with grid one pixel unit and ...
	if(pEDGEFILLPARAM->nvertex_max < nvertex_upperlimit)
	{
		//may have to allocate more IDVERTEX
		ASSERT(FALSE);
	}
	int ntri_upperlimit = nvertex_upperlimit; //2* nvertex_upperlimit;
	if(pEDGEFILLPARAM->ntri_max < ntri_upperlimit)
	{
		//may have to allocate more IDTRI
		ASSERT(FALSE);
	}


	//////////////////////////////////////
	//collect all edge triangle and vertex
	//////////////////////////////////////

	int idtri_T1 = -1; //embedding P1
	int idtri_T2 = -1; //embedding P2

	//1) find triangle that contains P1
	double xa = P1.x;
	double ya = P1.y;
	int itrianglefound = FindTriContainingPoint(m_pPOINTSET, xa, ya, pEDGEFILLPARAM->p_itriseed);
	if(itrianglefound==-1)
	{
		//P1 is outside convex hull
		
		//try finding the nearest triangle
		itrianglefound = pEDGEFILLPARAM->p_itriseed[0];
		idtri_T1 = itrianglefound;
	}
    if(itrianglefound>-1 //triangle found successfully (idtri returned and also stored in p_itriseed[0])
		|| itrianglefound==-2) //P1 also correspond to a vertex of the triangle found (idtri stored in p_itriseed[0])
    {
		//1.1) triangle found successfully		
		if(itrianglefound>-1)idtri_T1 = itrianglefound; //embedding P1
			else idtri_T1 = pEDGEFILLPARAM->p_itriseed[0];

		xa = P2.x;
		ya = P2.y;
		if(pEDGEFILLPARAM->pIDVERTEX2==NULL 
			|| pEDGEFILLPARAM->p_nvertexfound2==NULL 
			|| pEDGEFILLPARAM->nvertex_max2==0)
		{
			//collect all vertex (no sort according to edge side)
			itrianglefound = FindTriContainingPoint_CATAV(	m_pPOINTSET,
															xa,
															ya,
															pEDGEFILLPARAM->p_itriseed,
															pEDGEFILLPARAM->p_ntrifound,
															pEDGEFILLPARAM->pIDTRI,
															pEDGEFILLPARAM->p_nvertexfound,
															pEDGEFILLPARAM->pIDVERTEX);
		}
		else
		{
			//collect all vertex (sort between 2 sets, according to edge side)
			itrianglefound = FindTriContainingPoint_CATAV(	m_pPOINTSET,
															xa,
															ya,
															pEDGEFILLPARAM->p_itriseed,
															pEDGEFILLPARAM->p_ntrifound,
															pEDGEFILLPARAM->pIDTRI,
															pEDGEFILLPARAM->p_nvertexfound,
															pEDGEFILLPARAM->pIDVERTEX,
															pEDGEFILLPARAM->p_nvertexfound2,
															pEDGEFILLPARAM->pIDVERTEX2);
		}
		if(itrianglefound>=0 //success
			|| itrianglefound==-2) //success, P2 also corresponds to a vertex (triangle vertex)
		{
			//success
			if(itrianglefound>-1)idtri_T2 = itrianglefound; //embedding P1
				else idtri_T2 = pEDGEFILLPARAM->p_itriseed[0];
		}
		else if(itrianglefound==-1)
		{
			//P2 is outside the convex hull

			//try finding the nearest triangle
			idtri_T2 = pEDGEFILLPARAM->p_itriseed[0];
			//return -1;
		}

	}
    else if(itrianglefound==-1)
    {
		//1.3) P1 is outside the convex hull
		return -1;
    }
	
	#ifdef _DEBUG
		RenderToLog_Edge_TAV(P1, P2, pEDGEFILLPARAM);
	#endif //_DEBUG

	//now see if inner and/or outter edge triangle collection is required

	BOOL bCollectInnerEdgeIDTRI = FALSE;
	if(pEDGEFILLPARAM->pIDTRI_ie!=NULL
		&& pEDGEFILLPARAM->ntri_ie_max!=0
		&& pEDGEFILLPARAM->p_ntrifound_ie!=NULL)
	{
		bCollectInnerEdgeIDTRI = TRUE;
	}

	BOOL bCollectInnerEdgeIDVERTEX = FALSE;
	if(pEDGEFILLPARAM->pIDVERTEX_ie!=NULL
		&& pEDGEFILLPARAM->nvertex_ie_max!=0
		&& pEDGEFILLPARAM->p_nvertexfound_ie!=NULL)
	{
		bCollectInnerEdgeIDVERTEX = TRUE;
	}

	BOOL bCollectOutterEdgeIDTRI = FALSE;
	if(pEDGEFILLPARAM->pIDTRI_oe!=NULL
		&& pEDGEFILLPARAM->ntri_oe_max!=0
		&& pEDGEFILLPARAM->p_ntrifound_oe!=NULL)
	{
		bCollectOutterEdgeIDTRI = TRUE;
	}

	BOOL bCollectOutterEdgeIDVERTEX = FALSE;
	if(pEDGEFILLPARAM->pIDVERTEX_oe!=NULL
		&& pEDGEFILLPARAM->nvertex_oe_max!=0
		&& pEDGEFILLPARAM->p_nvertexfound_oe!=NULL)
	{
		bCollectOutterEdgeIDVERTEX = TRUE;
	}


	//now see if inner and/or outter edge to bounding rect triangle collection is required

	BOOL bCollectIEBR_IDTRI = FALSE;
	if(pEDGEFILLPARAM->pIDTRI_iebr!=NULL
		&& pEDGEFILLPARAM->ntri_iebr_max!=0
		&& pEDGEFILLPARAM->p_ntrifound_iebr!=NULL)
	{
		bCollectIEBR_IDTRI = TRUE;
	}

	BOOL bCollectIEBR_IDVERTEX = FALSE;
	if(pEDGEFILLPARAM->pIDVERTEX_iebr!=NULL
		&& pEDGEFILLPARAM->nvertex_iebr_max!=0
		&& pEDGEFILLPARAM->p_nvertexfound_iebr!=NULL)
	{
		bCollectIEBR_IDVERTEX = TRUE;
	}

	BOOL bCollectOEBR_IDTRI = FALSE;
	if(pEDGEFILLPARAM->pIDTRI_oebr!=NULL
		&& pEDGEFILLPARAM->ntri_oebr_max!=0
		&& pEDGEFILLPARAM->p_ntrifound_oebr!=NULL)
	{
		bCollectOEBR_IDTRI = TRUE;
	}

	BOOL bCollectOEBR_IDVERTEX = FALSE;
	if(pEDGEFILLPARAM->pIDVERTEX_oebr!=NULL
		&& pEDGEFILLPARAM->nvertex_oebr_max!=0
		&& pEDGEFILLPARAM->p_nvertexfound_oebr!=NULL)
	{
		bCollectOEBR_IDVERTEX = TRUE;
	}


	////////////////////////////////////////////
	//collect all inner edge triangle and vertex
	////////////////////////////////////////////

	if(bCollectInnerEdgeIDTRI==TRUE
		|| bCollectInnerEdgeIDVERTEX==TRUE)
	//	|| bCollectIEBR_IDTRI==TRUE
	//	|| bCollectIEBR_IDVERTEX==TRUE)
	{
		int ntri_ie = InnerEdgeFill(pEDGEFILLPARAM->pIDTRI, 
									pEDGEFILLPARAM->p_ntrifound[0],  
									pEDGEFILLPARAM->pIDTRI_ie, 
									pEDGEFILLPARAM->ntri_ie_max, 
									pEDGEFILLPARAM->p_ntrifound_ie, 
									pEDGEFILLPARAM->pIDVERTEX_ie,
									pEDGEFILLPARAM->nvertex_ie_max,
									pEDGEFILLPARAM->p_nvertexfound_ie);

		#ifdef _DEBUG
			RenderToLog_InnerEdge_TAV(P1, P2, pEDGEFILLPARAM);
		#endif //_DEBUG
	}





	/////////////////////////////////////////////
	//collect all outter edge triangle and vertex
	/////////////////////////////////////////////
	if(bCollectOutterEdgeIDTRI==TRUE
		|| bCollectOutterEdgeIDVERTEX==TRUE)
	//	|| bCollectOEBR_IDTRI==TRUE
	//	|| bCollectOEBR_IDVERTEX==TRUE)
	{
		int ntri_oe = OutterEdgeFill(	pEDGEFILLPARAM->pIDTRI, //edge triangle, starting at T1
										pEDGEFILLPARAM->p_ntrifound[0], //number of edge triangle, indirectly specifying T2 
										pEDGEFILLPARAM->pIDTRI_oe, 
										pEDGEFILLPARAM->ntri_oe_max, 
										pEDGEFILLPARAM->p_ntrifound_oe, 
										pEDGEFILLPARAM->pIDVERTEX_oe,
										pEDGEFILLPARAM->nvertex_oe_max,
										pEDGEFILLPARAM->p_nvertexfound_oe);
		#ifdef _DEBUG
			RenderToLog_OutterEdge_TAV(P1, P2, pEDGEFILLPARAM);
		#endif //_DEBUG
	}




	/////////////////////////////////////////////////////////////
	//collect all inner edge to bounding rect triangle and vertex
	/////////////////////////////////////////////////////////////

	if(bCollectIEBR_IDTRI==TRUE
		|| bCollectIEBR_IDVERTEX==TRUE)
	{
		RECT2DD myRECT2DD;
		myRECT2DD.left = P1.x;
		myRECT2DD.top = P1.y;
		myRECT2DD.right = P2.x;
		myRECT2DD.bottom = P2.y;
		int ntri_iebr = IEBRFill(	pEDGEFILLPARAM->pIDTRI, //edge idtri
									pEDGEFILLPARAM->p_ntrifound[0], //number of edge idtri
									&myRECT2DD,
									pEDGEFILLPARAM->pIDTRI_iebr, 
									pEDGEFILLPARAM->ntri_iebr_max, 
									pEDGEFILLPARAM->p_ntrifound_iebr, 
									pEDGEFILLPARAM->pIDVERTEX_iebr,
									pEDGEFILLPARAM->nvertex_iebr_max,
									pEDGEFILLPARAM->p_nvertexfound_iebr);

		#ifdef _DEBUG
			RenderToLog_IEBR_TAV(P1, P2, pEDGEFILLPARAM);
		#endif //_DEBUG
	}




	//////////////////////////////////////////////////////////////
	//collect all outter edge to bounding rect triangle and vertex
	//////////////////////////////////////////////////////////////

	if(bCollectOEBR_IDTRI==TRUE
		|| bCollectOEBR_IDVERTEX==TRUE)
	{
		RECT2DD myRECT2DD;
		myRECT2DD.left = P1.x;
		myRECT2DD.top = P1.y;
		myRECT2DD.right = P2.x;
		myRECT2DD.bottom = P2.y;
		int ntri_oebr = OEBRFill(	pEDGEFILLPARAM->pIDTRI, //edge idtri
									pEDGEFILLPARAM->p_ntrifound[0], //number of edge idtri
									&myRECT2DD,
									pEDGEFILLPARAM->pIDTRI_oebr, 
									pEDGEFILLPARAM->ntri_oebr_max, 
									pEDGEFILLPARAM->p_ntrifound_oebr, 
									pEDGEFILLPARAM->pIDVERTEX_oebr,
									pEDGEFILLPARAM->nvertex_oebr_max,
									pEDGEFILLPARAM->p_nvertexfound_oebr);
		#ifdef _DEBUG
			RenderToLog_OEBR_TAV(P1, P2, pEDGEFILLPARAM);
		#endif //_DEBUG
	}

	return pEDGEFILLPARAM->p_ntrifound[0];
}


//		IDTRI* pIDTRI_ie,		//idtri inner edge
//		int ntri_ie_max,			//idtri inner edge
//		int* p_ntrifound_ie,	//idtri inner edge
//		IDVERTEX* pIDVERTEX_ie=NULL,//idvertex inner edge
//		int nvertex_ie_max=0,		//idvertex inner edge
//		int* p_nvertexfound_ie=NULL,//idvertex inner edge
int CPointset::InnerEdgeFill(	IDTRI* pIDTRI_e, 
								int nIDTRI_e, 
								IDTRI* pIDTRI_ie, 
								int ntri_ie_max,
								int* p_ntrifound_ie,
								IDVERTEX* pIDVERTEX_ie,
								int nvertex_ie_max,
								int* p_nvertexfound_ie,
								RECT2DD* pRECT2DD) //!=NULL, with edge bounding rect rectrictions)	
{
	int index_T1 = 0;
	int index_T2 = nIDTRI_e-1; 
	int index_inc = 1;
	ASSERT(pIDTRI_ie!=NULL); //if NULL, must alloc and free within this function!!

	p_ntrifound_ie[0] = 0; 

	BOOL bCollectInnerEdgeIDVERTEX = FALSE;
	if(pIDVERTEX_ie!=NULL && p_nvertexfound_ie!=NULL) 
	{
		bCollectInnerEdgeIDVERTEX = TRUE;
		p_nvertexfound_ie[0] = 0;
	}

	int prev_idtri_e = pIDTRI_e[index_T1]; 
	int prev_idtri_e_next = pIDTRI_e[index_T1+index_inc]; 

	int prev_idvertex_i; //edge, inner side
	int prev_idvertex_o; //edge, outter side 
	int prev_idtri_i;	//edge, inner side
	int prev_idtri_o;	//edge, outter side
	int prev_idvertex_e_next = GetInnerAndOutterEdge_TAV(	prev_idtri_e, prev_idtri_e_next, 
															&prev_idvertex_i, &prev_idvertex_o,
															&prev_idtri_i, &prev_idtri_o);
	int k_inner_tri = 0;
	int k_inner_vertex = 0;

	if(prev_idtri_i!=-1 && prev_idtri_o!=-1 && prev_idvertex_e_next>=0)
	{
		//initialize collection of inner edge triangle
		int prev_idtri_seed = prev_idtri_i; //first idtri_i
		if(pRECT2DD==NULL || (pRECT2DD!=NULL && TriInRect(prev_idtri_i, pRECT2DD)))
		{
			//add first inner tri
			if(k_inner_tri<ntri_ie_max) 
			{
				pIDTRI_ie[k_inner_tri] = prev_idtri_i; 
				k_inner_tri++;
				p_ntrifound_ie[0] = k_inner_tri; //k_inner pointing to the next index equals the number found
			}
			//add first inner vertex
			if(bCollectInnerEdgeIDVERTEX && k_inner_vertex<nvertex_ie_max) 
			{
				int idvertex_e0 = GetThirdVertex(prev_idtri_e, prev_idvertex_i, prev_idvertex_o);
				int idvertex_i0 = GetThirdVertex(prev_idtri_i, prev_idvertex_i, idvertex_e0);
				pIDVERTEX_ie[k_inner_vertex] = idvertex_i0;
				k_inner_vertex++;
				p_nvertexfound_ie[0] = k_inner_vertex; //k_inner pointing to the next index equals the number found
			}
		}
		//follow the edge, going from idtri_T1 to idtri_T2
		//for(int i=(index_T1+index_inc); i<(index_T2+index_inc); i++) 
		for(int i=(index_T1+index_inc); i<(index_T2); i++) 
		{
			
			int idtri_e = pIDTRI_e[i]; 
			int idtri_e_next; 
			if(i<index_T2)
			{
				idtri_e_next = pIDTRI_e[i+index_inc]; //was [i+1]
			}
			else
			{
				int idvertex_i_last = GetThirdVertex(idtri_e, prev_idvertex_i, prev_idvertex_o);
				int idtri_i_last = GetAdjTri(m_pPOINTSET, prev_idvertex_i, idvertex_i_last, idtri_e);
				idtri_e_next = GetThirdAdjTri(idtri_e, prev_idtri_e, idtri_i_last); //for the last step, use the last outter edge idtri
			}

			int idvertex_i; //edge, inner side
			int idvertex_o; //edge, outter side 
			int idtri_i;	//edge, inner side
			int idtri_o;	//edge, outter side
			int idvertex_e_next = GetInnerAndOutterEdge_TAV(idtri_e, idtri_e_next, 
															&idvertex_i, &idvertex_o,
															&idtri_i, &idtri_o,
															&prev_idvertex_i, &prev_idvertex_o);

			if(idvertex_e_next<0) 
			{
				break;
			}
			if(idtri_o<0) 
			{
				break;
			}
			if(idtri_i<0) 
			{
				break;
			}
			//for each pair of adjacent edge triangle, 
			//collect counter-clockwise triangle, from idtri_T1_inner until hitting  edge triangle
			
			if(idvertex_i!=prev_idvertex_i)
			{
				//find all tri surrounding inner side vertex
				int idtri_seed = prev_idtri_seed;
				int numtrifound = 0;
				IDTRI pIDTRI[100];
				IDVERTEX pIDVERTEX[100];
				int iresult = FindAllTriSurroundingVertex(	m_pPOINTSET,
															prev_idvertex_i,
															&idtri_seed,
															&numtrifound,
															pIDTRI,
															pIDVERTEX);
				if(iresult<=0)
				{
					//ASSERT(FALSE);
					#ifdef _DEBUG
					((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText("FindAllTriSurroundingVertex() returned error in InnerEdgeFill(), tri collection aborted\r\n");
					#endif //_DEBUG
					//abort, return safely
					p_ntrifound_ie[0] = k_inner_tri; //+1;	//idtri inner edge
					p_nvertexfound_ie[0] = k_inner_vertex; //+1;	//idvertex inner edge
					if(k_inner_tri!=0 && k_inner_vertex!=0)
					{
						ASSERT(k_inner_tri==k_inner_vertex);
					}

					return p_ntrifound_ie[0];

				}
				//add tri that are not edge triangle and that have not already been added to inner edge  collection
				//these should be triangle ids from prev_idtri_i to idtri_i (in here, excluding prev_idtri_i and including idtri_i)
				//int j=0;
				int j=1; //skip the first, its the same idtri as the idtri_seed 
				while(j<numtrifound && pIDTRI[j]!=idtri_e) //so we include idtri_i
				{
					if(pRECT2DD==NULL || (pRECT2DD!=NULL && TriInRect(pIDTRI[j], pRECT2DD)))
					{
						//add inner tri (if ok to add, that is not priorly added)
						if(k_inner_tri<ntri_ie_max) 
						{
							//before adding, make sure new idtri is not in the previous 10 added
							BOOL bOK2Add = TRUE;
							for(int k=k_inner_tri-1; k>(k_inner_tri-15) && k>0; k--)
							//for(int k=k_inner_tri-1; k>0; k--)
							{
								if(pIDTRI_ie[k]==pIDTRI[j]) 
								{
									bOK2Add = FALSE;
									break;
								}
							}
							if(bOK2Add)
							{
								//add inner tri
								pIDTRI_ie[k_inner_tri] = pIDTRI[j]; 
								k_inner_tri++;
								p_ntrifound_ie[0] = k_inner_tri; //k_inner pointing to the next index equals the number found

								//add inner vertex
								if(bCollectInnerEdgeIDVERTEX && k_inner_vertex<nvertex_ie_max) 
								{
									//add vertex that are not edge vertex and that have not been added to inner edge collection
									pIDVERTEX_ie[k_inner_vertex] = pIDVERTEX[j]; 
									k_inner_vertex++;
									p_nvertexfound_ie[0] = k_inner_vertex; //k_inner pointing to the next index equals the number found
								}

							}
						}
					}
					prev_idtri_seed = pIDTRI[j]; //on exit of this while loop, prev_idtri_seed will be pointing to the last inner edge triangle
					j++;
				}
				if(prev_idtri_seed!=idtri_i)
				{
					//ASSERT(FALSE);
					#ifdef _DEBUG
						CString myString;
						myString.Format(L"warning, prev_idtri_seed!=idtri_i (%d!=%d) in InnerEdgeFill()\r\n", prev_idtri_seed, idtri_i);
						((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
					#endif //_DEBUG
				}
				prev_idtri_seed = idtri_i;
			}
			prev_idtri_e = idtri_e;
			prev_idtri_e_next = idtri_e_next;
			//prev_idtri_seed = idtri_seed; //moved in the if() up here, to be updated only when
			prev_idvertex_i = idvertex_i;
			prev_idvertex_o = idvertex_o;
			prev_idtri_i = idtri_i;	
			prev_idtri_o = idtri_o;	
			prev_idvertex_e_next = idvertex_e_next; 
		}
	}
	else
	{
		//if(prev_idtri_i!=-1 && prev_idtri_o!=-1 && prev_idvertex_e_next>=0)
		//ASSERT(FALSE);
		#ifdef _DEBUG
			((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText("GetInnerAndOutterEdge_TAV() returned error in InnerEdgeFill(), tri collection aborted\r\n");
		#endif //_DEBUG
	}

	p_ntrifound_ie[0] = k_inner_tri; //+1;	//idtri inner edge
	p_nvertexfound_ie[0] = k_inner_vertex; //+1;	//idvertex inner edge
	if(k_inner_tri!=0 && k_inner_vertex!=0)
	{
		ASSERT(k_inner_tri==k_inner_vertex);
	}

	if(pRECT2DD)
	{
		for(int i=0; i<k_inner_tri; i++)
		{
			if(!TriInRect(pIDTRI_ie[i], pRECT2DD))
			{
				p_ntrifound_ie[0] = 0;
				p_nvertexfound_ie[0] = 0;
			}
		}
	}
	return p_ntrifound_ie[0];
}


//		IDTRI* pIDTRI_oe=NULL,		//idtri outter edge
//		int ntri_oe_max=0,			//idtri outter edge
//		int* p_ntrifound_oe=NULL,	//idtri outter edge
//		IDVERTEX* pIDVERTEX_oe=NULL,//idvertex outter edge
//		int nvertex_oe_max=0,		//idvertex outter edge
//		int* p_nvertexfound_oe=NULL,//idvertex outter edge
int CPointset::OutterEdgeFill(	IDTRI* pIDTRI_e, 
								int nIDTRI_e, 
								IDTRI* pIDTRI_oe, 
								int ntri_oe_max,
								int* p_ntrifound_oe,
								IDVERTEX* pIDVERTEX_oe,
								int nvertex_oe_max,
								int* p_nvertexfound_oe,
								RECT2DD* pRECT2DD) //!=NULL, with edge bounding rect rectrictions
{
	//same as above for inner tri and vertex collection except
	//switch these variables (obviously) and ...
	//		IDTRI* pIDTRI_oe=NULL,		//idtri outter edge
	//		int ntri_oe_max=0,			//idtri outter edge
	//		int* p_ntrifound_oe=NULL,	//idtri outter edge
	//		IDVERTEX* pIDVERTEX_oe=NULL,//idvertex outter edge
	//		int nvertex_oe_max=0,		//idvertex outter edge
	//		int* p_nvertexfound_oe=NULL,//idvertex outter edge

	//same as above for inner tri and vertex collection except
	//switch index_T1 and index_T2 so we can use same collection
	//algorithm going counter-clockwise from T2 to T1 in this
	//case collecting outter edge tri and vertex backward. at
	//the end, reverse collection back to proper (as if would
	//have been collected from T1 to T2.
	int index_T1 = 0;
	int index_T2 = nIDTRI_e-1; 
	int index_inc = 1;
	int prev_index_T1 = 0;
	int prev_index_T2 = nIDTRI_e-1;
	int prev_index_inc = 1;
	index_T1 = prev_index_T2; 
	index_T2 = prev_index_T1; 
	index_inc = -1;
	ASSERT(pIDTRI_oe!=NULL); //if NULL, must alloc and free within this function!!

	p_ntrifound_oe[0] = 0; 

	BOOL bCollectOutterEdgeIDVERTEX = FALSE;
	if(pIDVERTEX_oe!=NULL && p_nvertexfound_oe!=NULL) 
	{
		bCollectOutterEdgeIDVERTEX = TRUE;
		p_nvertexfound_oe[0] = 0;
	}


	int prev_idtri_e = pIDTRI_e[index_T1]; 
	int prev_idtri_e_next = pIDTRI_e[index_T1+index_inc]; 

	int prev_idvertex_i; //edge, inner side
	int prev_idvertex_o; //edge, outter side 
	int prev_idtri_i;	//edge, inner side
	int prev_idtri_o;	//edge, outter side
	//switch between _i and _o because we are collecting tri and vertex in reverse order
	/*
	int prev_idvertex_e_next = GetInnerAndOutterEdge_TAV(	prev_idtri_e, prev_idtri_e_next, 
															&prev_idvertex_i, &prev_idvertex_o,
															&prev_idtri_i, &prev_idtri_o);
	*/
	int prev_idvertex_e_next = GetInnerAndOutterEdge_TAV(	prev_idtri_e, prev_idtri_e_next, 
															&prev_idvertex_o, &prev_idvertex_i,
															&prev_idtri_o, &prev_idtri_i);
	int k_outter_tri = 0;
	int k_outter_vertex = 0;

	if(prev_idtri_o!=-1 && prev_idtri_i!=-1 && prev_idvertex_e_next>=0)
	{
		//initialize collection of inner edge triangle
		int prev_idtri_seed = prev_idtri_o; //first idtri_o
		if(pRECT2DD==NULL || (pRECT2DD!=NULL && TriInRect(prev_idtri_o, pRECT2DD)))
		{
			//add first outter tri
			if(k_outter_tri<ntri_oe_max) 
			{
				pIDTRI_oe[k_outter_tri] = prev_idtri_o; 
				k_outter_tri++;
				p_ntrifound_oe[0] = k_outter_tri; //k_outter_tri pointing to the next index equals the number found
			}
			//add first outter vertex
			if(bCollectOutterEdgeIDVERTEX && k_outter_vertex<nvertex_oe_max) 
			{
				int idvertex_e0 = GetThirdVertex(prev_idtri_e, prev_idvertex_o, prev_idvertex_i);
				int idvertex_o0 = GetThirdVertex(prev_idtri_o, prev_idvertex_o, idvertex_e0);
				pIDVERTEX_oe[k_outter_vertex] = idvertex_o0; 
				k_outter_vertex++;
				p_nvertexfound_oe[0] = k_outter_vertex; //k_inner pointing to the next index equals the number found
			}
		}
		//follow the edge, going from idtri_T1 to idtri_T2
		//for(int i=(index_T1+index_inc); i>(index_T2+index_inc); i--) 
		for(int i=(index_T1+index_inc); i>(index_T2); i--) 
		{
			
			int idtri_e = pIDTRI_e[i]; 
			int idtri_e_next; 
			if(i>index_T2)
			{
				idtri_e_next = pIDTRI_e[i+index_inc]; //was [i+1]
			}
			else
			{
				int idvertex_o_last = GetThirdVertex(idtri_e, prev_idvertex_i, prev_idvertex_o);
				int idtri_o_last = GetAdjTri(m_pPOINTSET, prev_idvertex_o, idvertex_o_last, idtri_e);
				idtri_e_next = GetThirdAdjTri(idtri_e, prev_idtri_e, idtri_o_last); //for the last step, use the last outter edge idtri
			}

			int idvertex_i; //edge, inner side
			int idvertex_o; //edge, outter side 
			int idtri_i;	//edge, inner side
			int idtri_o;	//edge, outter side
			//switch between _i and _o because we are collecting tri and vertex in reverse order
			/*
			int idvertex_e_next = GetInnerAndOutterEdge_TAV(idtri_e, idtri_e_next, 
															&idvertex_i, &idvertex_o,
															&idtri_i, &idtri_o);
			*/
			int idvertex_e_next = GetInnerAndOutterEdge_TAV(idtri_e, idtri_e_next, 
															&idvertex_o, &idvertex_i,
															&idtri_o, &idtri_i,
															&prev_idvertex_o, &prev_idvertex_i);
			if(idvertex_e_next<0) 
			{
				break;
			}
			if(idtri_o<0) 
			{
				break;
			}
			if(idtri_i<0) 
			{
				break;
			}
			//for each pair of adjacent edge triangle, 
			//collect counter-clockwise triangle, from idtri_T1_inner until hitting  edge triangle
			
			if(idvertex_o!=prev_idvertex_o)
			{
				//find all tri surrounding inner side vertex
				int idtri_seed = prev_idtri_seed;
				int numtrifound = 0;
				IDTRI pIDTRI[100];
				IDVERTEX pIDVERTEX[100];
				int iresult = FindAllTriSurroundingVertex(	m_pPOINTSET,
															prev_idvertex_o,
															&idtri_seed,
															&numtrifound,
															pIDTRI,
															pIDVERTEX);
				if(iresult<=0)
				{
					//ASSERT(FALSE);
					#ifdef _DEBUG
					((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText("FindAllTriSurroundingVertex() returned error in OutterEdgeFill(), tri collection aborted\r\n");
					#endif //_DEBUG
					//abort, return safely
					p_ntrifound_oe[0] = k_outter_tri; 
					p_nvertexfound_oe[0] = k_outter_vertex; 
					if(k_outter_tri!=0 && k_outter_vertex!=0)
					{
						ASSERT(k_outter_tri==k_outter_vertex);
					}

					return p_ntrifound_oe[0];
				}
				//add tri that are not edge triangle and that have not already been added to inner edge  collection
				//these should be triangle ids from prev_idtri_i to idtri_i (in here, excluding prev_idtri_i and including idtri_i)
				//int j=0;
				int j=1; //skip the first, its the same idtri as the idtri_seed 
				while(j<numtrifound && pIDTRI[j]!=idtri_e) //so we include idtri_i
				{
					if(pRECT2DD==NULL || (pRECT2DD!=NULL && TriInRect(pIDTRI[j], pRECT2DD)))
					{
						//add outter tri (if ok to add, that is not priorly added)
						if(k_outter_tri<ntri_oe_max) //pEDGEFILLPARAM->ntri_oe_max)
						{
							//before adding, make sure new idtri is not in the previous 10 added
							BOOL bOK2Add = TRUE;
							for(int k=k_outter_tri-1; k>(k_outter_tri-15) && k>0; k--)
							//for(int k=k_outter_tri-1; k>0; k--)
							{
								if(pIDTRI_oe[k]==pIDTRI[j]) 
								{
									bOK2Add = FALSE;
									break;
								}
							}
							if(bOK2Add)
							{
								//add outter tri
								pIDTRI_oe[k_outter_tri] = pIDTRI[j]; 
								k_outter_tri++;
								p_ntrifound_oe[0] = k_outter_tri; //k_inner pointing to the next index equals the number found

								//add outter vertex
								if(bCollectOutterEdgeIDVERTEX && k_outter_vertex<nvertex_oe_max) 
								{
									//add vertex that are not edge vertex and that have not been added to inner edge collection
									pIDVERTEX_oe[k_outter_vertex] = pIDVERTEX[j]; 
									k_outter_vertex++;
									p_nvertexfound_oe[0] = k_outter_vertex; //k_inner pointing to the next index equals the number found
								}

							}
						}
					}
					prev_idtri_seed = pIDTRI[j]; //on exit of this while loop, prev_idtri_seed will be pointing to the last inner edge triangle
					j++;
				}
				if(prev_idtri_seed!=idtri_o)
				{
					//ASSERT(FALSE);
					#ifdef _DEBUG
					CString myString;
					myString.Format(L"warning, prev_idtri_seed!=idtri_o (%d!=%d) in OutterEdgeFill()\r\n", prev_idtri_seed, idtri_o);
					((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
					#endif //_DEBUG
				}
				prev_idtri_seed = idtri_o;
			}
			prev_idtri_e = idtri_e;
			prev_idtri_e_next = idtri_e_next;
			//prev_idtri_seed = idtri_seed; //moved in the if() up here, to be updated only when
			prev_idvertex_i = idvertex_i;
			prev_idvertex_o = idvertex_o;
			prev_idtri_i = idtri_i;	
			prev_idtri_o = idtri_o;	
			prev_idvertex_e_next = idvertex_e_next; 
		}
	}
	else
	{
		//if(prev_idtri_o!=-1 && prev_idtri_i!=-1 && prev_idvertex_e_next>=0)
		//ASSERT(FALSE);
		#ifdef _DEBUG
			((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText("GetInnerAndOutterEdge_TAV() returned error in OutterEdgeFill(), tri collection aborted\r\n");
		#endif //_DEBUG
	}
	p_ntrifound_oe[0] = k_outter_tri; //+1;	//idtri inner edge
	p_nvertexfound_oe[0] = k_outter_vertex; //+1;	//idvertex inner edge
	if(k_outter_tri!=0 && k_outter_vertex!=0)
	{
		ASSERT(k_outter_tri==k_outter_vertex);
	}

	/* //not required anymore, since this function is local
	//switch T1 and T2 back to normal.
	idtri_T1 = prev_idtri_T1;
	idtri_T2 = prev_idtri_T2;
	*/

	//reverse collection back to proper (as if would have been collected from T1 to T2.
	int kmax = p_ntrifound_oe[0]/2; //pEDGEFILLPARAM->p_ntrifound_oe[0]/2;
	for(int k=0; k<kmax; k++)
	{
		int prev_IDTRI1 = pIDTRI_oe[k]; 
		int prev_IDTRI2 = pIDTRI_oe[p_ntrifound_oe[0]-1-k]; 
		pIDTRI_oe[k] = prev_IDTRI2; 
		pIDTRI_oe[p_ntrifound_oe[0]-1-k] = prev_IDTRI1; 
	}
	if(bCollectOutterEdgeIDVERTEX)
	{
		kmax = p_nvertexfound_oe[0]/2; 
		for(int k=0; k<kmax; k++)
		{
			int prev_IDVERTEX1 = pIDVERTEX_oe[k]; 
			int prev_IDVERTEX2 = pIDVERTEX_oe[p_nvertexfound_oe[0]-1-k]; 
			pIDVERTEX_oe[k] = prev_IDVERTEX2; 
			pIDVERTEX_oe[p_nvertexfound_oe[0]-1-k] = prev_IDVERTEX1; 
		}
	}

	if(pRECT2DD)
	{
		for(int i=0; i<k_outter_tri; i++)
		{
			if(!TriInRect(pIDTRI_oe[i], pRECT2DD))
			{
				p_ntrifound_oe[0] = 0;
				p_nvertexfound_oe[0] = 0;
			}
		}
	}
	return p_nvertexfound_oe[0];
}


//		IDTRI* pIDTRI_iebr=NULL,		//idtri inner edge to bounding rect limits
//		int ntri_iebr_max=0,			//idtri inner edge to bounding rect limits
//		int* p_ntrifound_iebr=NULL,		//idtri inner edge to bounding rect limits
//		IDVERTEX* pIDVERTEX_iebr=NULL,	//idvertex inner edge to bounding rect limits
//		int nvertex_iebr_max=0,			//idvertex inner edge to bounding rect limits
//		int* p_nvertexfound_iebr=NULL,	//idvertex inner edge to bounding rect limits
int CPointset::IEBRFill(IDTRI* pIDTRI_e, 
						int nIDTRI_e,
						RECT2DD* pRECT2DD,
						IDTRI* pIDTRI_iebr, 
						int ntri_iebr_max,
						int* p_ntrifound_iebr,
						IDVERTEX* pIDVERTEX_iebr,
						int nvertex_iebr_max,
						int* p_nvertexfound_iebr)	
{
	
	m_pFloodFill_IDTRIList.RemoveAll();
	m_pFloodFill_nIDTRIList.RemoveAll();
	m_pDropped_IDTRIList.RemoveAll();
	

	p_ntrifound_iebr[0] = 0;

	IDTRI* prev_prev_pIDTRI_e = NULL;
	int prev_prev_nIDTRI_e = 0;

	int ntri_ie = -1;
	IDTRI* prev_pIDTRI_e = pIDTRI_e;
	int prev_nIDTRI_e = nIDTRI_e;
	IDTRI* prev_pIDTRI_ie = &(pIDTRI_iebr[0]);
	int prev_ntri_ie_max = ntri_iebr_max;
	int prev_ntrifound_ie = 0;
	IDVERTEX* prev_pIDVERTEX_ie = &(pIDVERTEX_iebr[0]);
	int prev_nvertex_ie_max = nvertex_iebr_max;
	int prev_nvertexfound_ie = 0;
	while( (ntri_ie==-1 && prev_nIDTRI_e>1) || (ntri_ie>1 && prev_nIDTRI_e>1) ) 	//while(ntri_ie!=0)
	{
		ntri_ie = InnerEdgeFill(prev_pIDTRI_e, 
								prev_nIDTRI_e, 
								prev_pIDTRI_ie, 
								prev_ntri_ie_max,
								&prev_ntrifound_ie,
								prev_pIDVERTEX_ie,
								prev_nvertex_ie_max,
								&prev_nvertexfound_ie,
								pRECT2DD);
		BOOL bEsc = ((COIIIApp*)AfxGetApp())->PeekKeyState(VK_ESCAPE);
		if(bEsc) 
		{
			ntri_ie = 0;
			prev_ntrifound_ie = 0;
			p_ntrifound_iebr[0] = p_ntrifound_iebr[0] + prev_ntrifound_ie; 
			p_nvertexfound_iebr[0] = p_nvertexfound_iebr[0] + prev_nvertexfound_ie; 
			CString myString;
			myString.Format(L"\r\nIEBREdgeFill() aborted by user after collecting %d triangles\r\n", p_ntrifound_iebr[0]);
			((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
			return p_ntrifound_iebr[0];
		}
		//make sure none of the newly collected prev_pIDTRI_ie triangles are not part of 2nd previous collected triangle
		/*
		int istart_dup = -1;
		int nstart_dup = 0;
		int iend_dup = -1;
		int nend_dup = 0;
		*/
		
		if(prev_prev_pIDTRI_e!=NULL)
		{
			for(int i=0; i<prev_ntrifound_ie; i++)
			{
				for(int j=(prev_prev_nIDTRI_e-1); j>=0; j--)
				{
					if(prev_pIDTRI_ie[i]==prev_prev_pIDTRI_e[j] && i!=0)
					{
						
						//stop collection, do not include any of these newly found triangles
						//#ifdef _DEBUG
						//	CString myString;
						//	myString.Format("stop collection in IEBREdgeFill(), some of the newly collected tri were already in previously collected tri set, see idtri %d\r\n", prev_prev_pIDTRI_e[j]);
						//	((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText(myString);
						//	((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText("dumping conflicting set, begin\r\n");
						//	RenderToLog_T(prev_pIDTRI_ie, prev_ntrifound_ie);
						//	((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText("dumping conflicting set, end\r\n");
						//#endif //_DEBUG
						//ntri_ie = 0;
						//return p_ntrifound_iebr[0]; //sum of collected idtri
						
						//try to skip bad idtri chain
						if(i>2)
						{
							//find offset
							int idtri3 = GetThirdAdjTri(prev_pIDTRI_ie[i-1], prev_pIDTRI_ie[i], prev_pIDTRI_ie[i-2]);
							int k=0; while(prev_pIDTRI_ie[i+k]!=idtri3 && (i+k)<prev_ntrifound_ie) k++;
							if((i+k)==prev_ntrifound_ie && i>3)
							{
								idtri3 = GetThirdAdjTri(prev_pIDTRI_ie[i-2], prev_pIDTRI_ie[i-1], prev_pIDTRI_ie[i-3]);
								k=0; while(prev_pIDTRI_ie[i+k]!=idtri3 && (i+k)<prev_ntrifound_ie) k++;
							}
							if((i+k)==prev_ntrifound_ie && i>4)
							{
								idtri3 = GetThirdAdjTri(prev_pIDTRI_ie[i-3], prev_pIDTRI_ie[i-2], prev_pIDTRI_ie[i-4]);
								k=0; while(prev_pIDTRI_ie[i+k]!=idtri3 && (i+k)<prev_ntrifound_ie) k++;
							}
							if((i+k)==prev_ntrifound_ie && i<(prev_ntrifound_ie-1))
							{
								idtri3 = GetThirdAdjTri(prev_pIDTRI_ie[i+1], prev_pIDTRI_ie[i], prev_pIDTRI_ie[i-1]);
								k=0; while(prev_pIDTRI_ie[i+k]!=idtri3 && (i+k)<prev_ntrifound_ie) k++;
							}
							if((i+k)==prev_ntrifound_ie && (prev_ntrifound_ie-i)<8)
							{
								ntri_ie = i;
								prev_ntrifound_ie = i;
								break;
							}
							if((i+k)<prev_ntrifound_ie)
							{
								ASSERT(prev_pIDTRI_ie[i+k]==idtri3);
								//fix collection
								for(int ii=i; ii<(prev_ntrifound_ie-k); ii++)
								{
									prev_pIDTRI_ie[ii] = prev_pIDTRI_ie[ii+k];
									if(prev_pIDVERTEX_ie) prev_pIDVERTEX_ie[ii] = prev_pIDVERTEX_ie[ii+k];
								}
								ntri_ie = prev_ntrifound_ie-k;
								prev_ntrifound_ie = ntri_ie;
								break;
								/*
								p_ntrifound_iebr[0] = p_ntrifound_iebr[0] + prev_ntrifound_ie; 
								p_nvertexfound_iebr[0] = p_nvertexfound_iebr[0] + prev_nvertexfound_ie; 
								return p_ntrifound_iebr[0];
								*/
							}

						}
						else
						{
							int k=i+1;
							//fix collection
							for(int ii=i; ii<(prev_ntrifound_ie-k); ii++)
							{
								prev_pIDTRI_ie[ii] = prev_pIDTRI_ie[ii+k];
								if(prev_pIDVERTEX_ie) prev_pIDVERTEX_ie[ii] = prev_pIDVERTEX_ie[ii+k];
							}
							ntri_ie = prev_ntrifound_ie-k;
							prev_ntrifound_ie = ntri_ie;
							break;
							/*
							p_ntrifound_iebr[0] = p_ntrifound_iebr[0] + prev_ntrifound_ie; 
							p_nvertexfound_iebr[0] = p_nvertexfound_iebr[0] + prev_nvertexfound_ie; 
							return p_ntrifound_iebr[0];
							*/
						}
						//drop part of the collection, do not include any of the duplicate triangles
						if(1)
						{
							#ifdef _DEBUG
								RenderToLog_T(&(prev_pIDTRI_ie[0]), i, "idtri (kept): ", "\r\n");
								//for(int k=0; k<i; k++) m_pFloodFill_IDTRIList.AddTail((void*)prev_pIDTRI_ie[k]);
								//if(k>0) m_pFloodFill_nIDTRIList.AddTail((void*)k);
								RenderToLog_T(&(prev_pIDTRI_ie[i]), (prev_ntrifound_ie-i), "idtri (dropped): ", "\r\n");
								//for(k=i; k<(prev_ntrifound_ie-i); k++) m_pDropped_IDTRIList.AddTail((void*)prev_pIDTRI_ie[k]);
							#endif //_DEBUG
							ntri_ie = i;
							prev_ntrifound_ie = i;
							if(prev_ntrifound_ie>10) break;
							p_ntrifound_iebr[0] = p_ntrifound_iebr[0] + prev_ntrifound_ie; 
							p_nvertexfound_iebr[0] = p_nvertexfound_iebr[0] + prev_nvertexfound_ie; 
							return p_ntrifound_iebr[0];
						}
						

						//if(istart_dup==-1) 
						//{
						//	istart_dup = i;
						//	nstart_dup = 1;
						//}
						//else if(istart_dup==(i-nstart_dup))
						//{
						//	nstart_dup++;
						//}
						//else if(iend_dup==-1)
						//{
						//	iend_dup = i;
						//	nend_dup = 1;
						//	break;
						//}
						

					}
				}
			}
		}
		
		/*
		if(istart_dup!=-1 && iend_dup!=-1)
		{
			//copy all idtri between istart_dup and iend_dup
			int nk = prev_ntrifound_ie;
			if(iend_dup!=-1) nk = iend_dup - (istart_dup+nstart_dup);
			for(int k=0; k<nk; k++)
			{
				prev_pIDTRI_ie[k] = prev_pIDTRI_ie[istart_dup+nstart_dup+k];
			}
			ntri_ie = nk;
			prev_ntrifound_ie = nk;
		}
		else if(istart_dup!=-1 && iend_dup==-1 && istart_dup==0)
		{
			//copy all idtri from istart_dup (skipping the nstart_dup duplicate) to the end 
			int nk = prev_ntrifound_ie - nstart_dup;
			if(iend_dup!=-1) nk = iend_dup - istart_dup;
			for(int k=0; k<nk; k++)
			{
				prev_pIDTRI_ie[k] = prev_pIDTRI_ie[nstart_dup+k];
			}
			ntri_ie = nk;
			prev_ntrifound_ie = nk;
		}
		else if(istart_dup!=-1)
		{
			ntri_ie = istart_dup;
			prev_ntrifound_ie = istart_dup;
		}
		*/
			
		//keep going
		if(ntri_ie!=0)
		{
			#ifdef _DEBUG
				RenderToLog_T(prev_pIDTRI_ie, prev_ntrifound_ie);
			#endif //_DEBUG

			prev_prev_pIDTRI_e = prev_pIDTRI_e;
			prev_prev_nIDTRI_e = prev_nIDTRI_e;
			prev_pIDTRI_e = prev_pIDTRI_ie;
			prev_nIDTRI_e = prev_ntrifound_ie;
			prev_ntri_ie_max = prev_ntri_ie_max - prev_ntrifound_ie;
			p_ntrifound_iebr[0] = p_ntrifound_iebr[0] + prev_ntrifound_ie; 
			prev_pIDTRI_ie = &(pIDTRI_iebr[p_ntrifound_iebr[0]]);
			p_nvertexfound_iebr[0] = p_nvertexfound_iebr[0] + prev_nvertexfound_ie; 
			prev_pIDVERTEX_ie = &(pIDVERTEX_iebr[p_nvertexfound_iebr[0]]);
			if(prev_ntri_ie_max<=0)
			{
				//ASSERT(FALSE);
				ASSERT(prev_ntrifound_ie==ntri_ie);
				CString myString;
				myString.Format(L"WARNING (serious), collected as many triangle as allocated space, total of %d triangle\r\n", ntri_iebr_max);
				((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
				return p_ntrifound_iebr[0];
			}
		}
		
	}

	return p_ntrifound_iebr[0]; //sum of collected idtri
}


int CPointset::OEBRFill(IDTRI* pIDTRI_e, 
						int nIDTRI_e,
						RECT2DD* pRECT2DD,
						IDTRI* pIDTRI_oebr, 
						int ntri_oebr_max,
						int* p_ntrifound_oebr,
						IDVERTEX* pIDVERTEX_oebr,
						int nvertex_oebr_max,
						int* p_nvertexfound_oebr)	
{
	p_ntrifound_oebr[0] = 0;

	IDTRI* prev_prev_pIDTRI_e = NULL;
	int prev_prev_nIDTRI_e = 0;

	int ntri_oe = -1;
	IDTRI* prev_pIDTRI_e = pIDTRI_e;
	int prev_nIDTRI_e = nIDTRI_e;
	IDTRI* prev_pIDTRI_oe = &(pIDTRI_oebr[0]);
	int prev_ntri_oe_max = ntri_oebr_max;
	int prev_ntrifound_oe = 0;
	IDVERTEX* prev_pIDVERTEX_oe = &(pIDVERTEX_oebr[0]);
	int prev_nvertex_oe_max = nvertex_oebr_max;
	int prev_nvertexfound_oe = 0;
	while( (ntri_oe==-1 && prev_nIDTRI_e>1) || (ntri_oe>1 && prev_nIDTRI_e>1) ) 	//while(ntri_oe!=0)
	{
		ntri_oe = OutterEdgeFill(	prev_pIDTRI_e, 
									prev_nIDTRI_e, 
									prev_pIDTRI_oe, 
									prev_ntri_oe_max,
									&prev_ntrifound_oe,
									prev_pIDVERTEX_oe,
									prev_nvertex_oe_max,
									&prev_nvertexfound_oe,
									pRECT2DD);

		BOOL bEsc = ((COIIIApp*)AfxGetApp())->PeekKeyState(VK_ESCAPE);
		if(bEsc) //shift key is down when higher order bit is set to 1, that is when iShiftKey < 0
		{
			ntri_oe = 0;
			prev_ntrifound_oe = 0;
			p_ntrifound_oebr[0] = p_ntrifound_oebr[0] + prev_ntrifound_oe; 
			p_nvertexfound_oebr[0] = p_nvertexfound_oebr[0] + prev_nvertexfound_oe; 
			CString myString;
			myString.Format(L"\r\nOEBREdgeFill() aborted by user after collecting %d triangles\r\n", p_ntrifound_oebr[0]);
			((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
			return p_ntrifound_oebr[0];
		}
		
		//make sure none of the newly collected prev_pIDTRI_ie triangles are not part of 2nd previous collected triangle
		/*
		int istart_dup = -1;
		int nstart_dup = 0;
		int iend_dup = -1;
		int nend_dup = 0;
		*/
		
		if(prev_prev_pIDTRI_e!=NULL)
		{
			for(int i=0; i<prev_ntrifound_oe; i++)
			{
				for(int j=(prev_prev_nIDTRI_e-1); j>=0; j--)
				{
					if(prev_pIDTRI_oe[i]==prev_prev_pIDTRI_e[j] && i!=0)
					{
						
						//stop collection, do not include any of these newly found triangles
						//#ifdef _DEBUG
						//	CString myString;
						//	myString.Format("stop collection in IEBREdgeFill(), some of the newly collected tri were already in previously collected tri set, see idtri %d\r\n", prev_prev_pIDTRI_e[j]);
						//	((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText(myString);
						//	((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText("dumping conflicting set, begin\r\n");
						//	RenderToLog_T(prev_pIDTRI_oe, prev_ntrifound_oe);
						//	((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText("dumping conflicting set, end\r\n");
						//#endif //_DEBUG
						//ntri_oe = 0;
						//return p_ntrifound_oebr[0]; //sum of collected idtri
						
						//try to skip bad idtri chain
						if(i>2)
						{
							//find offset
							int idtri3 = GetThirdAdjTri(prev_pIDTRI_oe[i-1], prev_pIDTRI_oe[i], prev_pIDTRI_oe[i-2]);
							int k=0; while(prev_pIDTRI_oe[i+k]!=idtri3 && (i+k)<prev_ntrifound_oe) k++;
							if((i+k)==prev_ntrifound_oe && i>3)
							{
								idtri3 = GetThirdAdjTri(prev_pIDTRI_oe[i-2], prev_pIDTRI_oe[i-1], prev_pIDTRI_oe[i-3]);
								k=0; while(prev_pIDTRI_oe[i+k]!=idtri3 && (i+k)<prev_ntrifound_oe) k++;
							}
							if((i+k)==prev_ntrifound_oe && i>4)
							{
								idtri3 = GetThirdAdjTri(prev_pIDTRI_oe[i-3], prev_pIDTRI_oe[i-2], prev_pIDTRI_oe[i-4]);
								k=0; while(prev_pIDTRI_oe[i+k]!=idtri3 && (i+k)<prev_ntrifound_oe) k++;
							}
							if((i+k)==prev_ntrifound_oe && i<(prev_ntrifound_oe-1))
							{
								idtri3 = GetThirdAdjTri(prev_pIDTRI_oe[i+1], prev_pIDTRI_oe[i], prev_pIDTRI_oe[i-1]);
								k=0; while(prev_pIDTRI_oe[i+k]!=idtri3 && (i+k)<prev_ntrifound_oe) k++;
							}
							if((i+k)==prev_ntrifound_oe && (prev_ntrifound_oe-i)<8)
							{
								ntri_oe = i;
								prev_ntrifound_oe = i;
								break;
							}
							if((i+k)<prev_ntrifound_oe)
							{
								ASSERT(prev_pIDTRI_oe[i+k]==idtri3);
								//fix collection
								for(int ii=i; ii<(prev_ntrifound_oe-k); ii++)
								{
									prev_pIDTRI_oe[ii] = prev_pIDTRI_oe[ii+k];
									if(prev_pIDVERTEX_oe) prev_pIDVERTEX_oe[ii] = prev_pIDVERTEX_oe[ii+k];
								}
								ntri_oe = prev_ntrifound_oe-k;
								prev_ntrifound_oe = ntri_oe;
								break;
								/*
								p_ntrifound_oebr[0] = p_ntrifound_oebr[0] + prev_ntrifound_oe; 
								p_nvertexfound_oebr[0] = p_nvertexfound_oebr[0] + prev_nvertexfound_oe; 
								return p_ntrifound_oebr[0];
								*/
							}

						}
						else
						{
							int k=i+1;
							//fix collection
							for(int ii=i; ii<(prev_ntrifound_oe-k); ii++)
							{
								prev_pIDTRI_oe[ii] = prev_pIDTRI_oe[ii+k];
								if(prev_pIDVERTEX_oe) prev_pIDVERTEX_oe[ii] = prev_pIDVERTEX_oe[ii+k];
							}
							ntri_oe = prev_ntrifound_oe-k;
							prev_ntrifound_oe = ntri_oe;
							break;
							/*
							p_ntrifound_oebr[0] = p_ntrifound_oebr[0] + prev_ntrifound_oe; 
							p_nvertexfound_oebr[0] = p_nvertexfound_oebr[0] + prev_nvertexfound_oe; 
							return p_ntrifound_oebr[0];
							*/
						}
						
						//drop part of the collection, do not include any of the duplicate triangles
						if(1)
						{
							#ifdef _DEBUG
								RenderToLog_T(&(prev_pIDTRI_oe[0]), i, "idtri (kept): ", "\r\n");
								RenderToLog_T(&(prev_pIDTRI_oe[i]), (prev_ntrifound_oe-i), "idtri (dropped): ", "\r\n");
							#endif //_DEBUG
							ntri_oe = i;
							prev_ntrifound_oe = i;
							if(prev_ntrifound_oe>10) break;
							p_ntrifound_oebr[0] = p_ntrifound_oebr[0] + prev_ntrifound_oe; 
							p_nvertexfound_oebr[0] = p_nvertexfound_oebr[0] + prev_nvertexfound_oe; 
							return p_ntrifound_oebr[0];

						}

						
						//if(istart_dup==-1) 
						//{
						//	istart_dup = i;
						//	nstart_dup = 1;
						//}
						//else if(istart_dup==(i-nstart_dup))
						//{
						//	nstart_dup++;
						//}
						//else if(iend_dup==-1)
						//{
						//	iend_dup = i;
						//	nend_dup = 1;
						//	break;
						//}
						
					}
				}
			}
		}
		
		/*
		if(istart_dup!=-1 && iend_dup!=-1)
		{
			//copy all idtri between istart_dup and iend_dup
			int nk = prev_ntrifound_oe;
			if(iend_dup!=-1) nk = iend_dup - (istart_dup+nstart_dup);
			for(int k=0; k<nk; k++)
			{
				prev_pIDTRI_oe[k] = prev_pIDTRI_oe[istart_dup+nstart_dup+k];
			}
			ntri_oe = nk;
			prev_ntrifound_oe = nk;
		}
		else if(istart_dup!=-1 && iend_dup==-1 && istart_dup==0)
		{
			//copy all idtri from istart_dup (skipping the nstart_dup duplicate) to the end 
			int nk = prev_ntrifound_oe - nstart_dup;
			if(iend_dup!=-1) nk = iend_dup - istart_dup;
			for(int k=0; k<nk; k++)
			{
				prev_pIDTRI_oe[k] = prev_pIDTRI_oe[nstart_dup+k];
			}
			ntri_oe = nk;
			prev_ntrifound_oe = nk;
		}
		else if(istart_dup!=-1)
		{
			ntri_oe = istart_dup;
			prev_ntrifound_oe = istart_dup;
		}
		*/

		//keep going
		if(ntri_oe!=0)
		{
			#ifdef _DEBUG
				RenderToLog_T(prev_pIDTRI_oe, prev_ntrifound_oe);
			#endif //_DEBUG

			prev_prev_pIDTRI_e = prev_pIDTRI_e;
			prev_prev_nIDTRI_e = prev_nIDTRI_e;
			prev_pIDTRI_e = prev_pIDTRI_oe;
			prev_nIDTRI_e = prev_ntrifound_oe;
			prev_ntri_oe_max = prev_ntri_oe_max - prev_ntrifound_oe;
			p_ntrifound_oebr[0] = p_ntrifound_oebr[0] + prev_ntrifound_oe; 
			prev_pIDTRI_oe = &(pIDTRI_oebr[p_ntrifound_oebr[0]]);
			p_nvertexfound_oebr[0] = p_nvertexfound_oebr[0] + prev_nvertexfound_oe; 
			prev_pIDVERTEX_oe = &(pIDVERTEX_oebr[p_nvertexfound_oebr[0]]);
			if(prev_ntri_oe_max<=0)
			{
				//ASSERT(FALSE);
				ASSERT(prev_ntri_oe_max==ntri_oe);
				CString myString;
				myString.Format(L"WARNING (serious), collected as many triangle as allocated space, total of %d triangle\r\n", ntri_oebr_max);
				((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
				return p_ntrifound_oebr[0];
			}
		}
		
	}
	return p_ntrifound_oebr[0]; //sum of collected idtri
}

//to collect triangle and vertex along the 4 edges of the specified rectangle
//see member function CPointset::RectFill() to collect triangle and vertex
//inside rectangle
int CPointset::RectEdgeFill(RECT2DD* pRECT2DD, 
							EDGEFILLPARAM* pEDGEFILLPARAM)
{
	if(pRECT2DD==NULL) 
	{
		ASSERT(FALSE);
		return -1;
	}

	const int npoints_polyline = 5;
	POINT2DD pPolylinePOINT2DD[npoints_polyline];
	pPolylinePOINT2DD[0].x = pRECT2DD->left;
	pPolylinePOINT2DD[0].y = pRECT2DD->top;
	pPolylinePOINT2DD[1].x = pRECT2DD->right;
	pPolylinePOINT2DD[1].y = pRECT2DD->top;
	pPolylinePOINT2DD[2].x = pRECT2DD->right;
	pPolylinePOINT2DD[2].y = pRECT2DD->bottom;
	pPolylinePOINT2DD[3].x = pRECT2DD->left;
	pPolylinePOINT2DD[3].y = pRECT2DD->bottom;
	pPolylinePOINT2DD[4].x = pPolylinePOINT2DD[0].x;
	pPolylinePOINT2DD[4].y = pPolylinePOINT2DD[0].y;

	int ntri_rect_perimeter = PolylineFill(	pPolylinePOINT2DD, 
											npoints_polyline,
											pEDGEFILLPARAM);

	return ntri_rect_perimeter;	
}

//		IDTRI* pIDTRI, //allocated by caller to collect all intermediate points between P1 and P2 
//		int ntri_max, //maximum number of IDTRI allocated by caller
//		int* p_ntrifound,
//		int* p_itriseed,
//		IDVERTEX* pIDVERTEX, //allocated by caller to collect all intermediate points between P1 and P2 
//		int nvertex_max, //maximum number of IDVERTEX allocated by caller
//		int* p_nvertexfound,
//		IDVERTEX* pIDVERTEX2, //allocated by caller to collect all intermediate points between P1 and P2 
//		int nvertex_max2, //maximum number of IDVERTEX allocated by caller
//		int* p_nvertexfound2,
//
//		IDTRI* pIDTRI_ie=NULL,		//idtri inner edge
//		int ntri_ie_max=0,			//idtri inner edge
//		int* p_ntrifound_ie=NULL,	//idtri inner edge
//		IDVERTEX* pIDVERTEX_ie=NULL,//idvertex inner edge
//		int nvertex_ie_max=0,		//idvertex inner edge
//		int* p_nvertexfound_ie=NULL,//idvertex inner edge
//		IDTRI* pIDTRI_oe=NULL,		//idtri outter edge
//		int ntri_oe_max=0,			//idtri outter edge
//		int* p_ntrifound_oe=NULL,	//idtri outter edge
//		IDVERTEX* pIDVERTEX_oe=NULL,//idvertex outter edge
//		int nvertex_oe_max=0,		//idvertex outter edge
//		int* p_nvertexfound_oe=NULL,//idvertex outter edge
//		IDTRI* pIDTRI_iebr=NULL,		//idtri inner edge to bounding rect limits
//		int ntri_iebr_max=0,			//idtri inner edge to bounding rect limits
//		int* p_ntrifound_iebr=NULL,		//idtri inner edge to bounding rect limits
//		IDVERTEX* pIDVERTEX_iebr=NULL,	//idvertex inner edge to bounding rect limits
//		int nvertex_iebr_max=0,			//idvertex inner edge to bounding rect limits
//		int* p_nvertexfound_iebr=NULL,	//idvertex inner edge to bounding rect limits
//		IDTRI* pIDTRI_oebr=NULL,		//idtri outter edge to bounding rect limits
//		int ntri_oebr_max=0,			//idtri outter edge to bounding rect limits
//		int* p_ntrifound_oebr=NULL,		//idtri outter edge to bounding rect limits
//		IDVERTEX* pIDVERTEX_oebr=NULL,	//idvertex outter edge to bounding rect limits
//		int nvertex_oebr_max=0,			//idvertex outter edge to bounding rect limits
//		int* p_nvertexfound_oebr=NULL)	//idvertex outter edge to bounding rect limits
int CPointset::PolylineFill(COWEdgeset* pEdgeset,
							POLYLINE* pPOLYLINE, 
							EDGEFILLPARAM* pEDGEFILLPARAM,
							COW2View* pOW2View) //=NULL
{
	if(pEdgeset==NULL || pPOLYLINE==NULL || pEDGEFILLPARAM==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//get polyline points
	int npoints_polyline_max = pEdgeset->GetPolylineNumberOfPoints(pPOLYLINE);
	POINT2DD* pPolylinePOINT2DD = (POINT2DD*) new POINT2DD[npoints_polyline_max];
	int npoints_polyline;

	if(pOW2View==NULL)
	{
		//no coordinate conversion
		npoints_polyline = pEdgeset->GetPolylinePoints(	pPOLYLINE, 
														pPolylinePOINT2DD, 
														npoints_polyline_max);
	}
	else 
	{
		//convert to world coordinate system
		POINT* pPolylinePOINT = (POINT*) new POINT[npoints_polyline_max];
		npoints_polyline = pEdgeset->GetPolylinePoints(	pPOLYLINE, 
														pPolylinePOINT, 
														npoints_polyline_max);
		for(int i=0; i<npoints_polyline; i++)
		{
			pPolylinePOINT2DD[i] = pOW2View->MapPointFromViewToWorld(pPolylinePOINT[i]);
		}
		delete[] pPolylinePOINT;
	}
	int ntri_e = PolylineFill(pPolylinePOINT2DD, npoints_polyline, pEDGEFILLPARAM);
	delete[] pPolylinePOINT2DD;
	return ntri_e;
}

int CPointset::PolylineFill(POINT* pPolylinePOINT,
							int npoints_polyline,
							EDGEFILLPARAM* pEDGEFILLPARAM)
{
	POINT2DD* pPolylinePOINT2DD = new POINT2DD[npoints_polyline];
	for(int i=0; i<npoints_polyline; i++)
	{
		pPolylinePOINT2DD[i].x = pPolylinePOINT[i].x;
		pPolylinePOINT2DD[i].y = pPolylinePOINT[i].y;
	}
	int ntri_e = PolylineFill(	pPolylinePOINT2DD, npoints_polyline, pEDGEFILLPARAM);
	delete[] pPolylinePOINT2DD;
	return ntri_e;
}
int CPointset::PolylineFill(POINT2DD* pPolylinePOINT2DD,
							int npoints_polyline,
							EDGEFILLPARAM* pEDGEFILLPARAM)
{
	int ntri = 0;

	if(pPolylinePOINT2DD==NULL || pEDGEFILLPARAM==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	EDGEFILLPARAM myEDGEFILLPARAM;
	myEDGEFILLPARAM = *pEDGEFILLPARAM;

	IDTRI* pIDTRI_e = NULL; 
	IDTRI* pIDTRI_ie = NULL;
	IDTRI* pIDTRI_oe = NULL;
	IDTRI* pIDTRI_iebr = NULL;
	IDTRI* pIDTRI_oebr = NULL;

	IDVERTEX* pIDVERTEX1_e = NULL; 
	IDVERTEX* pIDVERTEX2_e = NULL; 
	IDVERTEX* pIDVERTEX_ie = NULL;
	IDVERTEX* pIDVERTEX_oe = NULL;
	IDVERTEX* pIDVERTEX_iebr = NULL;
	IDVERTEX* pIDVERTEX_oebr = NULL;

	if(pEDGEFILLPARAM->pIDTRI) pIDTRI_e = &(pEDGEFILLPARAM->pIDTRI[0]);
	if(pEDGEFILLPARAM->pIDTRI_ie) pIDTRI_ie = &(pEDGEFILLPARAM->pIDTRI_ie[0]);
	if(pEDGEFILLPARAM->pIDTRI_oe) pIDTRI_oe = &(pEDGEFILLPARAM->pIDTRI_oe[0]);
	if(pEDGEFILLPARAM->pIDTRI_iebr) pIDTRI_iebr = &(pEDGEFILLPARAM->pIDTRI_iebr[0]);
	if(pEDGEFILLPARAM->pIDTRI_oebr) pIDTRI_oebr = &(pEDGEFILLPARAM->pIDTRI_oebr[0]);
	
	if(pEDGEFILLPARAM->pIDVERTEX) pIDVERTEX1_e = &(pEDGEFILLPARAM->pIDVERTEX[0]);
	if(pEDGEFILLPARAM->pIDVERTEX2) pIDVERTEX2_e = &(pEDGEFILLPARAM->pIDVERTEX2[0]);
	if(pEDGEFILLPARAM->pIDVERTEX_ie) pIDVERTEX_ie = &(pEDGEFILLPARAM->pIDVERTEX_ie[0]);
	if(pEDGEFILLPARAM->pIDVERTEX_oe) pIDVERTEX_oe = &(pEDGEFILLPARAM->pIDVERTEX_oe[0]);
	if(pEDGEFILLPARAM->pIDVERTEX_iebr) pIDVERTEX_iebr = &(pEDGEFILLPARAM->pIDVERTEX_iebr[0]);
	if(pEDGEFILLPARAM->pIDVERTEX_oebr) pIDVERTEX_oebr = &(pEDGEFILLPARAM->pIDVERTEX_oebr[0]);

	int ntri_e = 0; 
	int ntri_e_prev = 0;
	int ntri_e_max_remaining = myEDGEFILLPARAM.ntri_max;
	int nvertex1_e = 0; 
	int nvertex1_e_prev = 0;
	int nvertex1_e_max_remaining = myEDGEFILLPARAM.nvertex_max;
	int nvertex2_e = 0; 
	int nvertex2_e_prev = 0;
	int nvertex2_e_max_remaining = myEDGEFILLPARAM.nvertex_max2;

	int ntri_ie = 0; 
	int ntri_ie_prev = 0;
	int ntri_ie_max_remaining = myEDGEFILLPARAM.ntri_ie_max;
	int nvertex_ie = 0; 
	int nvertex_ie_prev = 0;
	int nvertex_ie_max_remaining = myEDGEFILLPARAM.nvertex_ie_max;

	int ntri_oe = 0; 
	int ntri_oe_prev = 0;
	int ntri_oe_max_remaining = myEDGEFILLPARAM.ntri_oe_max;
	int nvertex_oe = 0; 
	int nvertex_oe_prev = 0;
	int nvertex_oe_max_remaining = myEDGEFILLPARAM.nvertex_oe_max;

	int ntri_iebr = 0;
	int ntri_iebr_prev = 0;
	int ntri_iebr_max_remaining = myEDGEFILLPARAM.ntri_iebr_max;
	int nvertex_iebr = 0; 
	int nvertex_iebr_prev = 0;
	int nvertex_iebr_max_remaining = myEDGEFILLPARAM.nvertex_iebr_max;

	int ntri_oebr = 0;
	int ntri_oebr_prev = 0;
	int ntri_oebr_max_remaining = myEDGEFILLPARAM.ntri_oebr_max;
	int nvertex_oebr = 0; 
	int nvertex_oebr_prev = 0;
	int nvertex_oebr_max_remaining = myEDGEFILLPARAM.nvertex_oebr_max;

	//for each polyline's line segment, call edgefill
	int nlines_polyline = npoints_polyline -1;
	for(int i=0; i<nlines_polyline; i++)
	{
		ntri_e_max_remaining = ntri_e_max_remaining - ntri_e_prev - 1;
		ntri_ie_max_remaining = ntri_ie_max_remaining - ntri_ie_prev - 1;
		ntri_oe_max_remaining = ntri_oe_max_remaining - ntri_oe_prev - 1;
		ntri_iebr_max_remaining = ntri_iebr_max_remaining - ntri_iebr_prev - 1;
		ntri_oebr_max_remaining = ntri_oebr_max_remaining - ntri_oebr_prev - 1;
		nvertex1_e_max_remaining = nvertex1_e_max_remaining - nvertex1_e_prev - 1;
		nvertex2_e_max_remaining = nvertex2_e_max_remaining - nvertex2_e_prev - 1;
		nvertex_ie_max_remaining = nvertex_ie_max_remaining - nvertex_ie_prev - 1;
		nvertex_oe_max_remaining = nvertex_oe_max_remaining - nvertex_oe_prev - 1;
		nvertex_iebr_max_remaining = nvertex_iebr_max_remaining - nvertex_iebr_prev - 1;
		nvertex_oebr_max_remaining = nvertex_oebr_max_remaining - nvertex_oebr_prev - 1;
		
		myEDGEFILLPARAM.ntri_max = ntri_e_max_remaining;
		myEDGEFILLPARAM.ntri_ie_max = ntri_ie_max_remaining;
		myEDGEFILLPARAM.ntri_oe_max = ntri_oe_max_remaining;
		myEDGEFILLPARAM.ntri_iebr_max = ntri_iebr_max_remaining;
		myEDGEFILLPARAM.ntri_oebr_max = ntri_oebr_max_remaining;
		myEDGEFILLPARAM.nvertex_max = nvertex1_e_max_remaining;
		myEDGEFILLPARAM.nvertex_max2 = nvertex2_e_max_remaining;
		myEDGEFILLPARAM.nvertex_ie_max = nvertex_ie_max_remaining;
		myEDGEFILLPARAM.nvertex_oe_max = nvertex_oe_max_remaining;
		myEDGEFILLPARAM.nvertex_iebr_max = nvertex_iebr_max_remaining;
		myEDGEFILLPARAM.nvertex_oebr_max = nvertex_oebr_max_remaining;

		myEDGEFILLPARAM.p_ntrifound = &ntri_e_prev;
		myEDGEFILLPARAM.p_ntrifound_ie = &ntri_ie_prev;
		myEDGEFILLPARAM.p_ntrifound_oe = &ntri_oe_prev;
		myEDGEFILLPARAM.p_ntrifound_iebr = &ntri_iebr_prev;
		myEDGEFILLPARAM.p_ntrifound_oebr = &ntri_oebr_prev;
		myEDGEFILLPARAM.p_nvertexfound = &nvertex1_e_prev;
		myEDGEFILLPARAM.p_nvertexfound2 = &nvertex2_e_prev;
		myEDGEFILLPARAM.p_nvertexfound_ie = &nvertex_ie_prev;
		myEDGEFILLPARAM.p_nvertexfound_oe = &nvertex_oe_prev;
		myEDGEFILLPARAM.p_nvertexfound_iebr = &nvertex_iebr_prev;
		myEDGEFILLPARAM.p_nvertexfound_oebr = &nvertex_oebr_prev;

		POINT2DD P1;
		POINT2DD P2;
		P1.x = pPolylinePOINT2DD[i].x;
		P1.y = pPolylinePOINT2DD[i].y;
		P2.x = pPolylinePOINT2DD[i+1].x;
		P2.y = pPolylinePOINT2DD[i+1].y;
		ntri_e_prev = EdgeFill(	P1, P2, &myEDGEFILLPARAM);	//idvertex outter edge to bounding rect limits

		if(ntri_e_prev>0) ntri_e = ntri_e + ntri_e_prev - 1; 
		if(ntri_ie_prev>0) ntri_ie = ntri_ie + ntri_ie_prev - 1; 
		if(ntri_oe_prev>0) ntri_oe = ntri_oe + ntri_oe_prev - 1; 
		if(ntri_iebr_prev>0) ntri_iebr = ntri_iebr + ntri_iebr_prev - 1; 
		if(ntri_oebr_prev>0) ntri_oebr = ntri_oebr + ntri_oebr_prev - 1; 
		if(nvertex1_e_prev>0) nvertex1_e = nvertex1_e + nvertex1_e_prev - 1; 
		if(nvertex2_e_prev>0) nvertex2_e = nvertex2_e + nvertex2_e_prev - 1; 
		if(nvertex_ie_prev>0) nvertex_ie = nvertex_ie + nvertex_ie_prev - 1; 
		if(nvertex_oe_prev>0) nvertex_oe = nvertex_oe + nvertex_oe_prev - 1; 
		if(nvertex_iebr_prev>0) nvertex_iebr = nvertex_iebr + nvertex_iebr_prev - 1; 
		if(nvertex_oebr_prev>0) nvertex_oebr = nvertex_oebr + nvertex_oebr_prev - 1; 

		if(pEDGEFILLPARAM->pIDTRI) pIDTRI_e = &(pEDGEFILLPARAM->pIDTRI[ntri_e]);
		if(pEDGEFILLPARAM->pIDTRI_ie) pIDTRI_ie = &(pEDGEFILLPARAM->pIDTRI_ie[ntri_ie]);
		if(pEDGEFILLPARAM->pIDTRI_oe) pIDTRI_oe = &(pEDGEFILLPARAM->pIDTRI_oe[ntri_oe]);
		if(pEDGEFILLPARAM->pIDTRI_iebr) pIDTRI_iebr = &(pEDGEFILLPARAM->pIDTRI_iebr[ntri_iebr]);
		if(pEDGEFILLPARAM->pIDTRI_oebr) pIDTRI_oebr = &(pEDGEFILLPARAM->pIDTRI_oebr[ntri_oebr]);
		
		if(pEDGEFILLPARAM->pIDVERTEX) pIDVERTEX1_e = &(pEDGEFILLPARAM->pIDVERTEX[nvertex1_e]);
		if(pEDGEFILLPARAM->pIDVERTEX2) pIDVERTEX2_e = &(pEDGEFILLPARAM->pIDVERTEX2[nvertex2_e]);
		if(pEDGEFILLPARAM->pIDVERTEX_ie) pIDVERTEX_ie = &(pEDGEFILLPARAM->pIDVERTEX_ie[nvertex_ie]);
		if(pEDGEFILLPARAM->pIDVERTEX_oe) pIDVERTEX_oe = &(pEDGEFILLPARAM->pIDVERTEX_oe[nvertex_oe]);
		if(pEDGEFILLPARAM->pIDVERTEX_iebr) pIDVERTEX_iebr = &(pEDGEFILLPARAM->pIDVERTEX_iebr[nvertex_iebr]);
		if(pEDGEFILLPARAM->pIDVERTEX_oebr) pIDVERTEX_oebr = &(pEDGEFILLPARAM->pIDVERTEX_oebr[nvertex_oebr]);

		myEDGEFILLPARAM.pIDTRI = pIDTRI_e;
		myEDGEFILLPARAM.pIDTRI_ie = pIDTRI_ie;
		myEDGEFILLPARAM.pIDTRI_oe = pIDTRI_oe;
		myEDGEFILLPARAM.pIDTRI_iebr = pIDTRI_iebr;
		myEDGEFILLPARAM.pIDTRI_oebr = pIDTRI_oebr;

		myEDGEFILLPARAM.pIDVERTEX = pIDVERTEX1_e;
		myEDGEFILLPARAM.pIDVERTEX2 = pIDVERTEX2_e;
		myEDGEFILLPARAM.pIDVERTEX_ie = pIDVERTEX_ie;
		myEDGEFILLPARAM.pIDVERTEX_oe = pIDVERTEX_oe;
		myEDGEFILLPARAM.pIDVERTEX_iebr = pIDVERTEX_iebr;
		myEDGEFILLPARAM.pIDVERTEX_oebr = pIDVERTEX_oebr;

	}
	//spi, march 2010, replacing i with nlines_polyline, begin
	/*
	if(pPolylinePOINT2DD[i].x==pPolylinePOINT2DD[0].x
		&& pPolylinePOINT2DD[i].y==pPolylinePOINT2DD[0].y)	
	*/
	if(pPolylinePOINT2DD[nlines_polyline].x==pPolylinePOINT2DD[0].x
		&& pPolylinePOINT2DD[nlines_polyline].y==pPolylinePOINT2DD[0].y)	
	//spi, march 2010, replacing i with nlines_polyline, end
	{
		//last point is excluded
		//npoints = npoints;
	}
	else
	{
		//re-include the last point
		ntri_e++;
	}
	/*
	p_npoint_ioe_collected[0] = ntri_ioe;
	p_npoint_iebr_collected[0] = ntri_iebr;
	p_npoint_oebr_collected[0] = ntri_oebr;
	*/
	if(pEDGEFILLPARAM->p_ntrifound) pEDGEFILLPARAM->p_ntrifound[0] = ntri_e;
	if(pEDGEFILLPARAM->p_nvertexfound) pEDGEFILLPARAM->p_nvertexfound[0] = nvertex1_e;
	if(pEDGEFILLPARAM->p_nvertexfound2) pEDGEFILLPARAM->p_nvertexfound2[0] = nvertex2_e;
	if(pEDGEFILLPARAM->p_ntrifound_ie) pEDGEFILLPARAM->p_ntrifound_ie[0] = ntri_ie;
	if(pEDGEFILLPARAM->p_ntrifound_oe) pEDGEFILLPARAM->p_ntrifound_oe[0] = ntri_oe;
	if(pEDGEFILLPARAM->p_ntrifound_iebr) pEDGEFILLPARAM->p_ntrifound_iebr[0] = ntri_iebr;
	if(pEDGEFILLPARAM->p_ntrifound_oebr) pEDGEFILLPARAM->p_ntrifound_oebr[0] = ntri_oebr;
	if(pEDGEFILLPARAM->p_nvertexfound_ie) pEDGEFILLPARAM->p_nvertexfound_ie[0] = nvertex_ie;
	if(pEDGEFILLPARAM->p_nvertexfound_oe) pEDGEFILLPARAM->p_nvertexfound_oe[0] = nvertex_oe;
	if(pEDGEFILLPARAM->p_nvertexfound_iebr) pEDGEFILLPARAM->p_nvertexfound_iebr[0] = nvertex_iebr;
	if(pEDGEFILLPARAM->p_nvertexfound_oebr) pEDGEFILLPARAM->p_nvertexfound_oebr[0] = nvertex_oebr;

	return ntri_e;
}

//to collect triangle and vertex inside the specific rectangle
int CPointset::RectFill(RECT2DD* pRECT2DD, 
						EDGEFILLPARAM* pEDGEFILLPARAM)
{
	if(pRECT2DD==NULL) 
	{
		ASSERT(FALSE);
		return -1;
	}
	
	if(((COIIIApp*)AfxGetApp())->m_pTraceSettings->bTPTriangleset==FALSE)
	{
		((COIIIApp*)AfxGetApp())->GetLogDocument()->DisableLog();
		//((COIIIApp*)AfxGetApp())->GetLogDocument()->DisableFileUpdating();
	}
	else
	{
		((COIIIApp*)AfxGetApp())->GetLogDocument()->EnableLog();
		//((COIIIApp*)AfxGetApp())->GetLogDocument()->EnableFileUpdating();
	}
	
	POINT2DD P1;
	POINT2DD P2;
	P1.x = pRECT2DD->left;
	P1.y = pRECT2DD->top;
	P2.x = pRECT2DD->right;
	P2.y = pRECT2DD->bottom;
	int ntri_e = EdgeFill(P1, P2, pEDGEFILLPARAM);

	//((COIIIApp*)AfxGetApp())->GetLogDocument()->EnableLog();
	//((COIIIApp*)AfxGetApp())->GetLogDocument()->DisableFileUpdating();

	int ntri_iebr = pEDGEFILLPARAM->p_ntrifound_iebr[0];
	int ntri_oebr = pEDGEFILLPARAM->p_ntrifound_oebr[0];

	/*
	IDTRI* pIDTRI_e = pEDGEFILLPARAM->pIDTRI;
	int nIDTRI_e = pEDGEFILLPARAM->p_ntrifound[0];

	int ntri_iebr = IEBRFill(	pIDTRI_e, 
								nIDTRI_e,
								pRECT2DD,
								pEDGEFILLPARAM->pIDTRI_iebr, 
								pEDGEFILLPARAM->ntri_iebr_max,
								pEDGEFILLPARAM->p_ntrifound_iebr,
								pEDGEFILLPARAM->pIDVERTEX_iebr,
								pEDGEFILLPARAM->nvertex_iebr_max,
								pEDGEFILLPARAM->p_nvertexfound_iebr);	

	int ntri_oebr = OEBRFill(	pIDTRI_e, 
								nIDTRI_e,
								pRECT2DD,
								pEDGEFILLPARAM->pIDTRI_oebr, 
								pEDGEFILLPARAM->ntri_oebr_max,
								pEDGEFILLPARAM->p_ntrifound_oebr,
								pEDGEFILLPARAM->pIDVERTEX_oebr,
								pEDGEFILLPARAM->nvertex_oebr_max,
								pEDGEFILLPARAM->p_nvertexfound_oebr);	
	*/
	return ntri_e + ntri_iebr + ntri_oebr;	
}

int CPointset::PolygonFill(	COWEdgeset* pEdgeset,
							POLYLINE* pPOLYLINE, 
							EDGEFILLPARAM* pEDGEFILLPARAM)
{
	int ntri = 0;
	//1) get idtri along polygon boundary, tag them all collected
	//2) get one idtri inside polygon
	//3) floodfill collecting untagged idtri
	ASSERT(FALSE);
	return ntri;
}

int CPointset::PointFill(	POINT2DD P1, 
							EDGEFILLPARAM* pEDGEFILLPARAM)
{
	ASSERT(pEDGEFILLPARAM->pIDTRI!=NULL
			&& pEDGEFILLPARAM->pIDVERTEX!=NULL
			&& pEDGEFILLPARAM->p_ntrifound!=NULL
			&& pEDGEFILLPARAM->p_itriseed);

	int idvertex = FindNearestNeighbor(	m_pPOINTSET,
										P1.x,
										P1.y,
										pEDGEFILLPARAM->p_itriseed);
	if(idvertex>-1)
	{
		//success
	}
	else
	{
		ASSERT(FALSE);
		//i.e. , if idvertex==-1, point might me outside convex hull
	}
	pEDGEFILLPARAM->p_ntrifound[0] = 0;
	pEDGEFILLPARAM->p_nvertexfound[0] = 0;

	int iresult = FindAllTriSurroundingVertex(	m_pPOINTSET,
												idvertex,
												pEDGEFILLPARAM->p_itriseed,
												pEDGEFILLPARAM->p_ntrifound,
												pEDGEFILLPARAM->pIDTRI,
												pEDGEFILLPARAM->pIDVERTEX);	
	if(iresult==1)
	{
		//success
	}
	else if(iresult==0)
	{
		//at least one triangle is invalid
		ASSERT(FALSE); //no problemo, as long as you do not try to compute voronoi area for all these points
	}
	else
	{
		//error
		ASSERT(FALSE);
	}
	pEDGEFILLPARAM->p_nvertexfound[0] = pEDGEFILLPARAM->p_ntrifound[0];
	return iresult;
}

//given that idtri1 and idtri2 are adjacent to idtri, the triangle of interest,
//this function returns the third adjacent triangle. 
int CPointset::GetThirdAdjTri(int idtri, int idtri1, int idtri2)
{
	int i=0;
	// find the index not corresponding to idtri1 nor idtri2 
	while(i<3 && (idtri1==m_pPOINTSET->nt[i][idtri] 
				|| idtri2==m_pPOINTSET->nt[i][idtri])) i++;
	if(i==3)
	{
		//not found
		ASSERT(FALSE);
		return -1;
	}
	return m_pPOINTSET->nt[i][idtri];
}

//given idvertex1 and idvertex2 are 2 vertices of idtri, 
//this function returns the third vertex of idtri
int CPointset::GetThirdVertex(int idtri, int idvertex1, int idvertex2)
{
	int i=0;
	// find the index not corresponding to ivertex1 nor ivertex2 
	while(i<3 && (idvertex1==m_pPOINTSET->vt[i][idtri] 
				|| idvertex2==m_pPOINTSET->vt[i][idtri])) i++;
	if(i==3)
	{
		//not found
		ASSERT(FALSE);
		return -1;
	}
	return m_pPOINTSET->vt[i][idtri];
}

void CPointset::NormalizeRect(RECT2DD* pRECT2DD)
{
	//normalized
	double fXmin;
	double fXmax;
	if(pRECT2DD->left>pRECT2DD->right) 
	{
		fXmin = pRECT2DD->right;
		fXmax = pRECT2DD->left;
	}
	else 
	{
		fXmin = pRECT2DD->left;
		fXmax = pRECT2DD->right;
	}
	double fYmin;
	double fYmax;
	if(pRECT2DD->top>pRECT2DD->bottom) 
	{
		fYmin = pRECT2DD->bottom;
		fYmax = pRECT2DD->top;
	}
	else 
	{
		fYmin = pRECT2DD->top;
		fYmax = pRECT2DD->bottom;
	}
	pRECT2DD->left = fXmin;
	pRECT2DD->right = fXmax;
	pRECT2DD->top = fYmin;
	pRECT2DD->bottom = fYmax;
	return;
}

BOOL CPointset::PointInRect(POINT2DD* pPOINT2DD, RECT2DD* pRECT2DD)
{
	double fX = pPOINT2DD->x;
	double fY = pPOINT2DD->y;
	return PointInRect(fX, fY, pRECT2DD);
}

BOOL CPointset::PointInRect(int idvertex, RECT2DD* pRECT2DD)
{
	double fX = m_pPOINTSET->px[idvertex];
	double fY = m_pPOINTSET->py[idvertex];
	return PointInRect(fX, fY, pRECT2DD);
}

BOOL CPointset::PointInRect(double fX, double fY, RECT2DD* pRECT2DD)
{
	ASSERT(pRECT2DD!=NULL);
	RECT2DD myRECT2DD;
	myRECT2DD = *pRECT2DD;
	if(pRECT2DD->left<=pRECT2DD->right && pRECT2DD->top<=pRECT2DD->bottom) 
	//if(pRECT2DD->left<=pRECT2DD->right && pRECT2DD->top>=pRECT2DD->bottom) //because in world coor
	{
		//pRECT2DD is normalized
	}
	else
	{
		//ASSERT(FALSE); //just to know, for debugging after initial fresh run
		NormalizeRect(&myRECT2DD);
	}

	if(fX>=myRECT2DD.left && fX<=myRECT2DD.right
		&& fY>=myRECT2DD.top && fY<=myRECT2DD.bottom)
	{
		return TRUE; //inside rect (or touching rect boundary)
	}
	return FALSE; //outside rect
}


BOOL CPointset::TriInRect(int idtri, RECT2DD* pRECT2DD)
{
	ASSERT(pRECT2DD!=NULL);
	RECT2DD myRECT2DD;
	myRECT2DD = *pRECT2DD;
	if(pRECT2DD->left<=pRECT2DD->right && pRECT2DD->top<=pRECT2DD->bottom) 
	//if(pRECT2DD->left<=pRECT2DD->right && pRECT2DD->top>=pRECT2DD->bottom) //because in world coor
	{
		//pRECT2DD is normalized
	}
	else
	{
		//ASSERT(FALSE); //just to know, for debugging after initial fresh run
		NormalizeRect(&myRECT2DD);
	}
	BOOL bResult[3];
	int isum =0;
	for(int i=0; i<3;i++)
	{
		int idvertex = m_pPOINTSET->vt[i][idtri];
		bResult[i] = PointInRect(idvertex, &myRECT2DD);
		isum += (int) bResult[i];
	}
	//if(bResult[0]==FALSE && bResult[1]==FALSE && bResult[2]==FALSE)
	if(isum<2)
	{
		//2 FALSE 1 TRUE, or 3 FALSE
		return FALSE; //no, idtri completely outside rect
	}
	return TRUE; //yes, idtri either totally inside or overlapping rect boundary
}

//get inner and outter side edge vertex 
int CPointset::GetInnerAndOutterEdge_TAV(	int idtri_e, int idtri_e_next, 
											int* pIDVERTEX_i, int* pIDVERTEX_o,
											int* pIDTRI_i, int* pIDTRI_o,
											int* prev_pIDVERTEX_i, int* prev_pIDVERTEX_o)
{
	if(pIDVERTEX_i==NULL || pIDVERTEX_o==NULL || pIDTRI_i==NULL || pIDTRI_o==NULL)
	{
		ASSERT(FALSE);
		return -1;
	}
	int idvertex1[3];
	int idvertex2[3];
	int idtri_adj[3];
	int index = -1;
	//search for adjacent triangle to idtri_e
	for (int i=0; i<=2; i++)
	{
		//jnd[] = {1,2,0} to pick next adjacent index (counter-clockwise)
		idvertex1[i] = m_pPOINTSET->vt[i][idtri_e];
		idvertex2[i] = m_pPOINTSET->vt[jnd[i]][idtri_e];
		idtri_adj[i] = GetAdjTri(m_pPOINTSET, idvertex1[i], idvertex2[i], idtri_e);
		if(idtri_adj[i]==idtri_e_next)
		{
			index = i;
		}
	}
	if(index==-1)
	{
		//not found, idtri_e_next is not adjacent to idtri_e 
		//ASSERT(FALSE);
		m_pFloodFill_IDTRIList.AddTail((void*)idtri_e);
		m_pFloodFill_nIDTRIList.AddTail((void*)1);
		m_pFloodFill_IDTRIList.AddTail((void*)idtri_e_next);
		m_pFloodFill_nIDTRIList.AddTail((void*)1);
		/*
		CString myString;
		myString.Format("Error in GetInnerAndOutterEdge_TAV(%d,%d)", idtri_e, idtri_e_next);
		((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText(myString);
		pIDVERTEX_i[0] = 0;
		pIDVERTEX_o[0] = 0;
		pIDTRI_i[0] = 0;
		pIDTRI_o[0] = 0;
		return -1;
		*/
		int _idvertex1[3];
		int _idvertex2[3];
		int _idtri_adj[30];
		int _index = -1;
		//now search for adjacent triangle to idtri_e_next
		for (int i=0; i<=2; i++)
		{
			//jnd[] = {1,2,0} to pick next adjacent index (counter-clockwise)
			_idvertex1[i] = m_pPOINTSET->vt[i][idtri_e_next];
			_idvertex2[i] = m_pPOINTSET->vt[jnd[i]][idtri_e_next];
			_idtri_adj[i] = GetAdjTri(m_pPOINTSET, _idvertex1[i], _idvertex2[i], idtri_e_next);
			if(_idtri_adj[i]==idtri_e)
			{
				//none of the adj tri found should be idtri_e
				ASSERT(FALSE);
			}
			//to find idtri adjacent to both idtri_e and idtri_e_next
			for(int j=0; j<=2; j++)
			{
				if(_idtri_adj[i]==idtri_adj[j])
				{
					_index = i;
				}
			}
		}
		if(_index==-1)
		{
			//there is more than one triangle between idtri_e and idtri_e_next 
			/*
			//ASSERT(FALSE);
			#ifdef _DEBUG
				CString myString;
				myString.Format("Error in GetInnerAndOutterEdge_TAV(%d,%d)\r\n", idtri_e, idtri_e_next);
				((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText(myString);
			#endif //_DEBUG
			pIDVERTEX_i[0] = 0;
			pIDVERTEX_o[0] = 0;
			pIDTRI_i[0] = 0;
			pIDTRI_o[0] = 0;
			return -1;
			*/
			//collect these triangles
			POINT2DD P;
			P = GetTriangleGeoCenter(idtri_e_next);
			int idtriseed = idtri_e;
			int ntrifound = 0;
			int iresult = FindTriContainingPoint_CAT(	m_pPOINTSET,
														P.x,
														P.y,
														&idtriseed,
														&ntrifound,
														_idtri_adj);
			ASSERT(_idtri_adj[ntrifound-1]==idtri_e_next);
			ASSERT(ntrifound>3);
			_index = ntrifound-1-1;
			#ifdef _DEBUG
				CString myString;
				myString.Format(L"Found %d triangle between idtri_e and idtri_e_next in GetInnerAndOutterEdge_TAV(%d,%d)\r\n", ntrifound, idtri_e, idtri_e_next);
				((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
			#endif //_DEBUG
			if(iresult<0)
			{
				//ASSERT(FALSE);
				#ifdef _DEBUG
					CString myString;
					myString.Format(L"Error in GetInnerAndOutterEdge_TAV(%d,%d)\r\n", idtri_e, idtri_e_next);
					((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
				#endif //_DEBUG
				pIDVERTEX_i[0] = 0;
				pIDVERTEX_o[0] = 0;
				pIDTRI_i[0] = 0;
				pIDTRI_o[0] = 0;
				return -1;
			}
		}
		//keep going droping idtri_e replacing it with _idtri_e which is adjacent to idtri_e_next
		int _idtri_e = _idtri_adj[_index];
		_index = -1;
		//now search for adjacent triangle to _idtri_e
		for (int i=0; i<=2; i++)
		{
			//jnd[] = {1,2,0} to pick next adjacent index (counter-clockwise)
			_idvertex1[i] = m_pPOINTSET->vt[i][_idtri_e];
			_idvertex2[i] = m_pPOINTSET->vt[jnd[i]][_idtri_e];
			_idtri_adj[i] = GetAdjTri(m_pPOINTSET, _idvertex1[i], _idvertex2[i], _idtri_e);
			if(_idtri_adj[i]==idtri_e_next)
			{
				_index = i;
			}
		}
		if(_index==-1)
		{
			ASSERT(FALSE);
			#ifdef _DEBUG
				CString myString;
				myString.Format(L"Error in GetInnerAndOutterEdge_TAV(%d,%d)\r\n", idtri_e, idtri_e_next);
				((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
			#endif //_DEBUG
			pIDVERTEX_i[0] = 0;
			pIDVERTEX_o[0] = 0;
			pIDTRI_i[0] = 0;
			pIDTRI_o[0] = 0;
			return -1;
		}
		pIDVERTEX_i[0] = _idvertex1[_index];
		pIDVERTEX_o[0] = _idvertex2[_index];
		//jrd[] = {2,0,1} to pick second next adjacent index (counter-clockwise)
		pIDTRI_i[0] = _idtri_adj[jrd[_index]]; //or idtri[jnd[jnd[index]]], one is the inner, the other the outter
		pIDTRI_o[0] = _idtri_adj[jnd[_index]]; 
		if(prev_pIDVERTEX_i) 
		{
			int _idvertex_e = GetThirdVertex(_idtri_e, pIDVERTEX_i[0], pIDVERTEX_o[0]);
			prev_pIDVERTEX_i[0] = GetThirdVertex(pIDTRI_i[0], pIDVERTEX_i[0], _idvertex_e);
		}
		if(prev_pIDVERTEX_o) 
		{
			int _idvertex_e = GetThirdVertex(_idtri_e, pIDVERTEX_i[0], pIDVERTEX_o[0]);
			prev_pIDVERTEX_o[0] = GetThirdVertex(pIDTRI_o[0], pIDVERTEX_o[0], _idvertex_e);
		}
		//return >=0; //idvertex_e_next
		//return -1; error
		CString myString;
		myString.Format(L"Called GetInnerAndOutterEdge_TAV(idtri_e %d, idtri_e_next %d), found idtri_e not adjacent to idtri_e_next, idtr_e replaced by _idtri_e %d\r\n", idtri_e, idtri_e_next, _idtri_e);
		((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
		int idvertex_e_next = GetThirdVertex(idtri_e_next, pIDVERTEX_i[0], pIDVERTEX_o[0]);
		return idvertex_e_next;
	}
	pIDVERTEX_i[0] = idvertex1[index];
	pIDVERTEX_o[0] = idvertex2[index];
	//jrd[] = {2,0,1} to pick second next adjacent index (counter-clockwise)
	pIDTRI_i[0] = idtri_adj[jrd[index]]; //or idtri[jnd[jnd[index]]], one is the inner, the other the outter
	pIDTRI_o[0] = idtri_adj[jnd[index]]; 

	//return >=0; //idvertex_e_next
	//return -1; error
	int idvertex_e_next = GetThirdVertex(idtri_e_next, pIDVERTEX_i[0], pIDVERTEX_o[0]);
	return idvertex_e_next;
}

void CPointset::RenderToLog_T(IDTRI* pIDTRI, int nIDTRI, const char* pszHeader, const char* pszFooter)
{
	CString myString;
	CString myString2;
	if(pszHeader==NULL) myString = "idtri: ";
	else myString = pszHeader;
	for(int i=0; i<nIDTRI; i++)
	{
		int ivalue = pIDTRI[i];
		myString2.Format(L"%d, ", ivalue);
		myString += myString2;
	}
	((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
	if(pszFooter==NULL) ((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText("\r\n");
	  else ((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText(pszFooter);
}

void CPointset::RenderToLog_Edge_TAV(	POINT2DD P1, 
										POINT2DD P2, 
										EDGEFILLPARAM* pEDGEFILLPARAM)
{
	CString myString;
	myString.Format(L"EdgeFill(P1,P2) collected %i triangles - P1(%i,%i) P2(%i,%i)\r\n", pEDGEFILLPARAM->p_ntrifound[0], (int)P1.x, (int)P1.y, (int)P2.x, (int)P2.y);
	((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
	CString myString2;
	myString = "idtri: ";
	for(int i=0; i<pEDGEFILLPARAM->p_ntrifound[0]; i++)
	{
		int ivalue = pEDGEFILLPARAM->pIDTRI[i];
		myString2.Format(L"%d, ", ivalue);
		myString += myString2;
	}
	if(pEDGEFILLPARAM->pIDVERTEX2==NULL 
		|| pEDGEFILLPARAM->p_nvertexfound2==NULL 
		|| pEDGEFILLPARAM->nvertex_max2==0)
	{
		myString += "    idvertex: ";
		for(int i=0; i<pEDGEFILLPARAM->p_nvertexfound[0]; i++)
		{
			int ivalue = pEDGEFILLPARAM->pIDVERTEX[i];
			myString2.Format(L"%d, ", ivalue);
			myString += myString2;
		}
	}
	else
	{
		myString += "    idvertex1: ";
		for(int i=0; i<pEDGEFILLPARAM->p_nvertexfound[0]; i++)
		{
			int ivalue = pEDGEFILLPARAM->pIDVERTEX[i];
			myString2.Format(L"%d, ", ivalue);
			myString += myString2;
		}
		myString += "    idvertex2: ";
		for(int i=0; i<pEDGEFILLPARAM->p_nvertexfound2[0]; i++)
		{
			int ivalue = pEDGEFILLPARAM->pIDVERTEX2[i];
			myString2.Format(L"%d, ", ivalue);
			myString += myString2;
		}
	}
	((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
	((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText("\r\n\r\n");
}

void CPointset::RenderToLog_InnerEdge_TAV(	POINT2DD P1, 
											POINT2DD P2, 
											EDGEFILLPARAM* pEDGEFILLPARAM)
{
	//for debugging, begin
	#ifdef _DEBUG
	CString myString;
	CString myString2;
	if(pEDGEFILLPARAM->pIDTRI_ie!=NULL)
	{
		int ntrifound = pEDGEFILLPARAM->p_ntrifound_ie[0];
		myString.Format(L"EdgeFill(P1,P2) collected %d inner triangles - P1(%d,%d) P2(%d,%d)\r\n", ntrifound, (int)P1.x, (int)P1.y, (int)P2.x, (int)P2.y);
		((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
		myString = "idtri_ie: ";
		for(int i=0; i<pEDGEFILLPARAM->p_ntrifound_ie[0]; i++)
		{
			int ivalue = pEDGEFILLPARAM->pIDTRI_ie[i];
			myString2.Format(L"%d, ", ivalue);
			myString += myString2;
		}
	}
	if(pEDGEFILLPARAM->pIDVERTEX_ie!=NULL)
	{
		myString += "    idvertex_ie: ";
		for(int i=0; i<pEDGEFILLPARAM->p_nvertexfound_ie[0]; i++)
		{
			int ivalue = pEDGEFILLPARAM->pIDVERTEX_ie[i];
			myString2.Format(L"%d, ", ivalue);
			myString += myString2;
		}
	}
	if(pEDGEFILLPARAM->pIDTRI_ie!=NULL
		|| pEDGEFILLPARAM->pIDVERTEX_ie!=NULL)
	{
		myString += "\r\n\r\n";
		((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
	}
	#endif //_DEBUG
	//for debugging, end
}

void CPointset::RenderToLog_OutterEdge_TAV(	POINT2DD P1, 
											POINT2DD P2, 
											EDGEFILLPARAM* pEDGEFILLPARAM)
{
	//for debugging, begin
	#ifdef _DEBUG
	CString myString;
	CString myString2;
	if(pEDGEFILLPARAM->pIDTRI_oe!=NULL)
	{
		int ntrifound = pEDGEFILLPARAM->p_ntrifound_oe[0];
		myString.Format(L"EdgeFill(P1,P2) collected %d outter triangles - P1(%d,%d) P2(%d,%d)\r\n", ntrifound, (int)P1.x, (int)P1.y, (int)P2.x, (int)P2.y);
		((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
		myString = "idtri_oe: ";
		for(int i=0; i<pEDGEFILLPARAM->p_ntrifound_oe[0]; i++)
		{
			int ivalue = pEDGEFILLPARAM->pIDTRI_oe[i];
			myString2.Format(L"%d, ", ivalue);
			myString += myString2;
		}
	}
	if(pEDGEFILLPARAM->pIDVERTEX_oe!=NULL)
	{
		myString += "    idvertex_oe: ";
		for(int i=0; i<pEDGEFILLPARAM->p_nvertexfound_oe[0]; i++)
		{
			int ivalue = pEDGEFILLPARAM->pIDVERTEX_oe[i];
			myString2.Format(L"%d, ", ivalue);
			myString += myString2;
		}
	}
	if(pEDGEFILLPARAM->pIDTRI_oe!=NULL
		|| pEDGEFILLPARAM->pIDVERTEX_oe!=NULL)
	{
		myString += "\r\n\r\n";
		((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
	}
	#endif //_DEBUG
	//for debugging, end
}

void CPointset::RenderToLog_IEBR_TAV(	POINT2DD P1, 
										POINT2DD P2, 
										EDGEFILLPARAM* pEDGEFILLPARAM)
{
	//for debugging, begin
	#ifdef _DEBUG
	CString myString;
	CString myString2;
	if(pEDGEFILLPARAM->pIDTRI_iebr!=NULL)
	{
		int ntrifound = pEDGEFILLPARAM->p_ntrifound_iebr[0];
		myString.Format(L"EdgeFill(P1,P2) collected %d iebr triangles - P1(%d,%d) P2(%d,%d)\r\n", ntrifound, (int)P1.x, (int)P1.y, (int)P2.x, (int)P2.y);
		((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
		myString = "idtri_iebr: ";
		for(int i=0; i<pEDGEFILLPARAM->p_ntrifound_iebr[0]; i++)
		{
			int ivalue = pEDGEFILLPARAM->pIDTRI_iebr[i];
			myString2.Format(L"%d, ", ivalue);
			myString += myString2;
		}
	}
	if(pEDGEFILLPARAM->pIDVERTEX_iebr!=NULL)
	{
		myString += "    idvertex_iebr: ";
		for(int i=0; i<pEDGEFILLPARAM->p_nvertexfound_iebr[0]; i++)
		{
			int ivalue = pEDGEFILLPARAM->pIDVERTEX_iebr[i];
			myString2.Format(L"%d, ", ivalue);
			myString += myString2;
		}
	}
	if(pEDGEFILLPARAM->pIDTRI_iebr!=NULL
		|| pEDGEFILLPARAM->pIDVERTEX_iebr!=NULL)
	{
		myString += "\r\n\r\n";
		((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
	}
	#endif //_DEBUG
	//for debugging, end
}

void CPointset::RenderToLog_OEBR_TAV(	POINT2DD P1, 
										POINT2DD P2, 
										EDGEFILLPARAM* pEDGEFILLPARAM)
{
	//for debugging, begin
	#ifdef _DEBUG
	CString myString;
	CString myString2;
	if(pEDGEFILLPARAM->pIDTRI_oebr!=NULL)
	{
		int ntrifound = pEDGEFILLPARAM->p_ntrifound_oebr[0];
		myString.Format(L"EdgeFill(P1,P2) collected %d oebr triangles - P1(%d,%d) P2(%d,%d)\r\n", ntrifound, (int)P1.x, (int)P1.y, (int)P2.x, (int)P2.y);
		((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
		myString = "idtri_oebr: ";
		for(int i=0; i<pEDGEFILLPARAM->p_ntrifound_oebr[0]; i++)
		{
			int ivalue = pEDGEFILLPARAM->pIDTRI_oebr[i];
			myString2.Format(L"%d, ", ivalue);
			myString += myString2;
		}
	}
	if(pEDGEFILLPARAM->pIDVERTEX_oebr!=NULL)
	{
		myString += "    idvertex_oebr: ";
		for(int i=0; i<pEDGEFILLPARAM->p_nvertexfound_oebr[0]; i++)
		{
			int ivalue = pEDGEFILLPARAM->pIDVERTEX_oebr[i];
			myString2.Format(L"%d, ", ivalue);
			myString += myString2;
		}
	}
	if(pEDGEFILLPARAM->pIDTRI_oebr!=NULL
		|| pEDGEFILLPARAM->pIDVERTEX_oebr!=NULL)
	{
		myString += "\r\n\r\n";
		((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
	}
	#endif //_DEBUG
	//for debugging, end
}


BOOL CPointset::ReadAllValuesFrom(FILE* pFILE)
{
	COWSet::ReadAllValuesFrom(pFILE);
	//2020july03, spi, begin
	//int inumread = -1;
	size_t inumread = 0;
	//2020july03, spi, end
	int npoints;
	inumread = fread(&npoints, sizeof(int), 1, pFILE);
	if(inumread!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(npoints>GetMaxNumberOfElements())
	{
		int new_maxnumberofelements = npoints + POINTSET_ADDITIONAL_NUMBEROFPOINTS;
		ReallocPointset(new_maxnumberofelements);
		ReallocPointsetStatistics(new_maxnumberofelements);
	}
	
	for(int i=0; i<npoints; i++)
	{
		double fX;
		double fY;
		double fIntensity;
		double fDimX;
		double fDimY;
		double fArea;
		inumread = fread(&fX, sizeof(double), 1, pFILE);
		if(inumread!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		inumread = fread(&fY, sizeof(double), 1, pFILE);
		if(inumread!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		inumread = fread(&fIntensity, sizeof(double), 1, pFILE);
		if(inumread!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		inumread = fread(&fDimX, sizeof(double), 1, pFILE);
		if(inumread!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		inumread = fread(&fDimY, sizeof(double), 1, pFILE);
		if(inumread!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		inumread = fread(&fArea, sizeof(double), 1, pFILE);
		if(inumread!=1)
		{
			//#include <errno.h>
			int idoserror = _doserrno;
			int ierror = errno;
			int ierror1 = ferror(pFILE);
			int ierror2 = feof(pFILE);
			ASSERT(FALSE);
			return FALSE;
		}
		//int inumbyte = fscanf(pFILE, "%f %f %f %f %f %f", &fX, &fY, &fIntensity, &fDimX, &fDimY, &fArea);

		m_pPOINTSET->px[i] = fX;
		m_pPOINTSET->py[i] = fY;
		double* pfStatistics = GetPointerToStatistics(i);
		pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_INTENSITY] = fIntensity;
		pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMX] = fDimX;
		pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMY] = fDimY;
		pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEAREA] = fArea;
		//update pointset min max info
		m_pPOINTSET->xmin=(m_pPOINTSET->xmin < m_pPOINTSET->px[i] ) ? m_pPOINTSET->xmin : m_pPOINTSET->px[i];
		m_pPOINTSET->ymin=(m_pPOINTSET->ymin < m_pPOINTSET->py[i] ) ? m_pPOINTSET->ymin : m_pPOINTSET->py[i];
		m_pPOINTSET->xmax=(m_pPOINTSET->xmax > m_pPOINTSET->px[i] ) ? m_pPOINTSET->xmax : m_pPOINTSET->px[i];
		m_pPOINTSET->ymax=(m_pPOINTSET->ymax > m_pPOINTSET->py[i] ) ? m_pPOINTSET->ymax : m_pPOINTSET->py[i];
	}

	/*
	double f1;
	double f2;
	double f3;
	double f4;
	inumread=fread(&f1, sizeof(double), 1, pFILE);
	inumread=fread(&f2, sizeof(double), 1, pFILE);
	inumread=fread(&f3, sizeof(double), 1, pFILE);
	inumread=fread(&f4, sizeof(double), 1, pFILE);
	*/

	/*
	inumread=fread(&(m_pPOINTSET->px[0]), sizeof(double), 1, pFILE);
	inumread=fread(&(m_pPOINTSET->px[1]), sizeof(double), 1, pFILE);
	inumread=fread(&(m_pPOINTSET->px[2]), sizeof(double), 1, pFILE);
	inumread=fread(&(m_pPOINTSET->px[3]), sizeof(double), 1, pFILE);
	*/
	
	/*
	double fValue;
	for(int i=0; i<npoints; i++)
	{
		inumread = fread(&fValue, sizeof(double), 1, pFILE);
		if(inumread!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		m_pPOINTSET->px[i] = fValue;
	}
	for(i=0; i<npoints; i++)
	{
		inumread = fread(&fValue, sizeof(double), 1, pFILE);
		if(inumread!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		m_pPOINTSET->py[i] = fValue;
	}
	*/
		//inumread=fread(m_pPOINTSET->px, sizeof(double), npoints, pFILE);
		//if(inumread!=npoints)
		//{
		//	ASSERT(FALSE);
		//	return FALSE;
		//}
	
	
	

	
	
	//inumread=fread(m_pPOINTSET->py, sizeof(double), npoints, pFILE);
	//if(inumread!=npoints)
	//{
	//	ASSERT(FALSE);
	//	return FALSE;
	//}
	
	m_pPOINTSET->npts = npoints;
	return TRUE;
}

BOOL CPointset::WriteAllValuesTo(FILE* pFILE)
{
	COWSet::WriteAllValuesTo(pFILE);

	//2020july03, spi, begin
	//int inumwritten = -1;
	size_t inumwritten = 0;
	//2020july03, spi, end
	int npoints = m_pPOINTSET->npts;
	inumwritten = fwrite(&npoints, sizeof(int), 1, pFILE);
	if(inumwritten!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	

	/*
	double f1 = 1.0;
	double f2 = 2.0;
	double f3 = 3.0;
	double f4 = 4.0;
	inumwritten = fwrite(&f1, sizeof(double), 1, pFILE);
	inumwritten = fwrite(&f2, sizeof(double), 1, pFILE);
	inumwritten = fwrite(&f3, sizeof(double), 1, pFILE);
	inumwritten = fwrite(&f4, sizeof(double), 1, pFILE);
	*/

	/*
	inumwritten = fwrite(&(m_pPOINTSET->px[0]), sizeof(double), 1, pFILE);
	inumwritten = fwrite(&(m_pPOINTSET->px[1]), sizeof(double), 1, pFILE);
	inumwritten = fwrite(&(m_pPOINTSET->px[2]), sizeof(double), 1, pFILE);
	inumwritten = fwrite(&(m_pPOINTSET->px[3]), sizeof(double), 1, pFILE);
	*/

	for(int i=0; i<npoints; i++)
	{
		double fX = m_pPOINTSET->px[i];
		double fY = m_pPOINTSET->py[i];
		double* pfStatistics = GetPointerToStatistics(i);
		double fIntensity = pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_INTENSITY];
		double fDimX = pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMX];
		double fDimY = pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMY];
		double fArea = pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEAREA];
		inumwritten = fwrite(&fX, sizeof(double), 1, pFILE);
		if(inumwritten!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		inumwritten = fwrite(&fY, sizeof(double), 1, pFILE);
		if(inumwritten!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		inumwritten = fwrite(&fIntensity, sizeof(double), 1, pFILE);
		if(inumwritten!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		inumwritten = fwrite(&fDimX, sizeof(double), 1, pFILE);
		if(inumwritten!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		inumwritten = fwrite(&fDimY, sizeof(double), 1, pFILE);
		if(inumwritten!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		inumwritten = fwrite(&fArea, sizeof(double), 1, pFILE);
		if(inumwritten!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		//int inumbyte = fprintf(pFILE, "%f\t%f\t%f\t%f\t%f\t%f\r\n", fX, fY, fIntensity, fDimX, fDimY, fArea);
	}
	/*
	int j=0;
	double fValue;
	for(int i=0; i<npoints; i++)
	{
		j++; if(j==200) 
		{ 
			fflush(pFILE); 
			j=0; 
		}
		fValue = (double) m_pPOINTSET->px[i]; 
		inumwritten = fwrite((void*)&fValue, sizeof(double), 1, pFILE);
		if(inumwritten!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	fflush(pFILE);
	j=0;
	for(i=0; i<npoints; i++)
	{
		j++; if(j==200) { fflush(pFILE); j=0; }
		fValue = (double) m_pPOINTSET->py[i]; 
		inumwritten = fwrite((void*)&fValue, sizeof(double), 1, pFILE);
		if(inumwritten!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	fflush(pFILE);
	*/

	/*
	inumwritten = fwrite(m_pPOINTSET->px, sizeof(double), npoints, pFILE);
	if(inumwritten!=npoints)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	*/
	/*
	for(i=0; i<npoints; i++)
	{
		double fValue = m_pPOINTSET->py[i]; 
		inumwritten = fwrite(&fValue, sizeof(double), 1, pFILE);
		if(inumwritten!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	*/
	/*
	inumwritten = fwrite(m_pPOINTSET->py, sizeof(double), npoints, pFILE);
	if(inumwritten!=npoints)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	*/
	return TRUE;
}

RECT CPointset::GetTriangleBoundingRect(int idtri, COW2View* pOW2View, int nei)
{
	RECT2DD myRECT2DD = GetTriangleBoundingRect(idtri, nei);
	return pOW2View->MapRectFromWorldToView(myRECT2DD);
}

RECT2DD CPointset::GetTriangleBoundingRect(int idtri, int nei)
{
	double x, y , xmin, xmax, ymin, ymax;

	//for idtri alone
	for (int i=0; i<=2; i++)
	{
		x = m_pPOINTSET->px[m_pPOINTSET->vt[i][idtri]];
		y = m_pPOINTSET->py[m_pPOINTSET->vt[i][idtri]];
		if(i==0)
		{
			xmin = x;
			xmax = x;
			ymin = y;
			ymax = y;
		}
		else 
		{
			if(x<xmin) xmin = x;
			  else if(x>xmax) xmax = x;
			if(y<ymin) ymin = y;
			  else if(y>ymax) ymax = y;
		}
	}

	if(nei==1)
	{
		int nIDVERTEX;
		IDVERTEX pIDVERTEX[30];
		int iresult = FindAllNeighborSurroundingTri(m_pPOINTSET, idtri, nei, &nIDVERTEX, pIDVERTEX);
		
		if(iresult>-1)
		{
			for(int i=0; i<nIDVERTEX; i++)
			{
				x = m_pPOINTSET->px[pIDVERTEX[i]];
				y = m_pPOINTSET->py[pIDVERTEX[i]];
				if(x<xmin) xmin = x;
				  else if(x>xmax) xmax = x;
				if(y<ymin) ymin = y;
				  else if(y>ymax) ymax = y;
			}
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	RECT2DD myRECT2DD;
	myRECT2DD.left = xmin;
	myRECT2DD.right = xmax;
	myRECT2DD.top = ymin;
	myRECT2DD.bottom = ymax;
	return myRECT2DD;
}

POINT CPointset::GetTriangleGeoCenter(int idtri, COW2View* pOW2View)
{
	POINT2DD myPOINT2DD = GetTriangleGeoCenter(idtri);
	return pOW2View->MapPointFromWorldToView(myPOINT2DD);
}

POINT2DD CPointset::GetTriangleGeoCenter(int idtri)
{
	double x = 0.0;
	double y = 0.0;
	for (int i=0; i<=2; i++)
	{
		//jnd[] = {1,2,0} to pick next adjacent index (counter-clockwise)
		x = x + m_pPOINTSET->px[m_pPOINTSET->vt[i][idtri]];
		y = y + m_pPOINTSET->py[m_pPOINTSET->vt[i][idtri]];
	}
	x = x/3.0;
	y = y/3.0;
	POINT2DD P;
	P.x = x;
	P.y = y;
	return P;
}

void CPointset::GetTrianglePOINTS(int idtri, POINT2DD* pPOINT2DD)
{
	for (int i=0; i<=2; i++)
	{
		pPOINT2DD[i].x = m_pPOINTSET->px[m_pPOINTSET->vt[i][idtri]];
		pPOINT2DD[i].y = m_pPOINTSET->py[m_pPOINTSET->vt[i][idtri]];
	}
	return;
}

void CPointset::GetTrianglePOINTS(int idtri, POINT* pPOINT, COW2View* pOW2View)
{
	POINT2DD myPOINT2DD;
	for (int i=0; i<=2; i++)
	{
		myPOINT2DD.x =  m_pPOINTSET->px[m_pPOINTSET->vt[i][idtri]];
		myPOINT2DD.y = m_pPOINTSET->py[m_pPOINTSET->vt[i][idtri]];
		pPOINT[i] = pOW2View->MapPointFromWorldToView(myPOINT2DD);
	}
	return;
}



