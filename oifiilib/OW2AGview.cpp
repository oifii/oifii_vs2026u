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

// OW2AGview.cpp : implementation of the COW2AGView class
//

#include "stdafx.h"
#include "oifiilib.h" //spi 2014

#include "c_pointset.h"
#include "owpointset.h"

#include "OWView.h"
#include "OW2View.h"
#include "OW2AGview.h"

#include "OWdoc.h"
#include "OW2doc.h"
#include "OW2AGdoc.h"

#include <gl\gl.h>
#include <gl\glu.h>
#include "oifii_propsettings.h"

#include <float.h>
#include <math.h>

//2012august23, poirier, begin
#include "portaudio.h"
#include "pa_asio.h"
#include <map>
#include <string>
using namespace std;
//2012august23, poirier, end

//2013july02, poirier, begin
//#include "spiws_wavset.h" //spi 2014
#include "spiwavsetlib.h"
//2013july02, poirier, end

#include "OW2mdi.h"
#include "OW2AGmdi.h"

#include "oifii_mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COW2View

IMPLEMENT_DYNCREATE(COW2AGView, COW2View)

BEGIN_MESSAGE_MAP(COW2AGView, COW2View)
	//{{AFX_MSG_MAP(COW2AGView)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_EDIT_CLEAR, &COW2AGView::OnEditClear)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COW2View construction/destruction

COW2AGView::COW2AGView()
{
}

COW2AGView::~COW2AGView()
{
}



void COW2AGView::OnInitialUpdate()
{
	COW2View::OnInitialUpdate();

	//ensure voronoi polygon will be drawn
	m_pViewDataSettings->bDrawVoronoiset=TRUE;

    //Register our view as a drop target.
    m_droptarget.Register(this);
}


DROPEFFECT COW2AGView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	// TODO: Add your specialized code here and/or call the base class
	//DROPEFFECT dwEffect = DROPEFFECT_MOVE;
	DROPEFFECT dwEffect = DROPEFFECT_COPY;
	return dwEffect;
	//return COW2View::OnDragEnter(pDataObject, dwKeyState, point);
}


void COW2AGView::OnDragLeave()
{
	// TODO: Add your specialized code here and/or call the base class

	COW2View::OnDragLeave();
}


DROPEFFECT COW2AGView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	// TODO: Add your specialized code here and/or call the base class
	//DROPEFFECT dwEffect = DROPEFFECT_MOVE;
	DROPEFFECT dwEffect = DROPEFFECT_COPY;
	return dwEffect;
	//return COW2View::OnDragOver(pDataObject, dwKeyState, point);
}


BOOL COW2AGView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	BOOL bRet;
    //Read the CF_HDROP data and put the files in the main window's list.
    bRet = ReadHdropData(pDataObject);

	return TRUE;
	//return COW2View::OnDrop(pDataObject, dropEffect, point);
}


//ReadHdropData() reads CF_HDROP data from the passed-in data object, and 
//puts all dropped files/folders into the document's list control.
BOOL COW2AGView::ReadHdropData(COleDataObject* pDataObject)
{
	HGLOBAL     hg;
	HDROP       hdrop;
	UINT        uNumFiles;
	TCHAR       szNextFile [MAX_PATH];
	//SHFILEINFO  rFileInfo;
	COW2AGDoc* pDoc = ((COW2AGDoc*)GetDocument());
	//int         nIndex = list.GetItemCount();
	//HANDLE      hFind;
	//WIN32_FIND_DATA rFind;
	//TCHAR       szFileLen [64];

    //Get the HDROP data from the data object.
    hg=pDataObject->GetGlobalData(CF_HDROP);
    if(NULL==hg)
    {
        return FALSE;
    }
    hdrop=(HDROP)GlobalLock(hg);
    if(NULL==hdrop)
    {
        GlobalUnlock(hg);
        return FALSE;
    }

    //Get the # of files being dropped.
    uNumFiles = DragQueryFile(hdrop,-1,NULL,0);
    for(UINT uFile=0; uFile<uNumFiles; uFile++)
    {
        //Get the next filename from the HDROP info.
        if (DragQueryFile(hdrop,uFile,szNextFile,MAX_PATH)>0)
		{
			CStringA myStringA(szNextFile);
			//if file is audio
			if (pDoc->IsSupportedWavset(myStringA) == TRUE)
			{
				//if the filename is already in the list, skip it.
				pDoc->AddWavset(myStringA);
			}
			//if file is an image
			else if (pDoc->IsSupportedPixelset(myStringA) == TRUE)
			{
				pDoc->ReplacePixelset(myStringA);
			}
			else
			{
			}
        }
    }

    return TRUE;
}


void COW2AGView::OnDraw(CDC* pDC)
{
	COW2View::OnDraw(pDC);
	COW2AGDoc* pDoc = (COW2AGDoc*)GetDocument();
	if(pDoc->m_pPointset!=NULL && pDoc->m_idpointstart>0)
	{
		
		CBrush* pMyOldBrush = NULL;
		CBrush myBrush1;
		myBrush1.CreateSolidBrush(RGB(150,150,150));
		pMyOldBrush = pDC->SelectObject(&myBrush1); 
		
		/*
		// Create a hatched bit pattern.
		WORD HatchBits[8] = { 0x11, 0x22, 0x44, 0x88, 0x11,
			0x22, 0x44, 0x88 };
		// Use the bit pattern to create a bitmap.
		CBitmap bm;
		BITMAP myBITMAP;
		CBrush* pMyOldBrush = NULL;
		CBrush myBrush1;
		if(pDoc->m_pPixelset==NULL)
		{
			bm.CreateBitmap(8,8,1,1, HatchBits);
		}
		else
		{
			myBITMAP=GetDDB(m_hDDB);
			bm.CreateBitmapIndirect(&myBITMAP);
		}
		// Create a pattern brush from the bitmap.
		myBrush1.CreatePatternBrush(&bm);
		pMyOldBrush = pDC->SelectObject(&myBrush1); 
		*/

		//red thin pen
		CPen* pPen = new CPen;
		CPen* pOldPen;
		pPen->CreatePen(PS_SOLID, OW2VIEW_PENSIZE, RGB(255, 0, 0));
    	pOldPen = pDC->SelectObject(pPen);
		/*
		//no brush
    	CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH); 
		*/

		POINT pPointVoronoiPolygon[100];
		int numpointvoronoipolygon=0;

		for (int idpoint=pDoc->m_idpointstart; idpoint<pDoc->m_pPointset->GetNumberOfPoints(); idpoint++)
		{
			POINT2DD myPoint2DD;
			myPoint2DD.x = pDoc->m_pPointset->m_pPOINTSET->px[idpoint];
			myPoint2DD.y = pDoc->m_pPointset->m_pPOINTSET->py[idpoint];
			double dfValue = pDoc->m_pPointset->GetStatisticsValue(idpoint, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL); //the fourth user-defined statistics is used as a volume level
			/*
			if(dfValue>0.0)
			{
				//floodfill the voronoi region (at this stage the voronoi region should already be drawn by the base class)
				//POINT2DD myPoint2DD = MapPointFromViewToWorld(logPoint);
				POINT myPOINT = MapPointFromWorldToView(myPoint2DD);
				pDC->FloodFill(myPOINT.x, myPOINT.y, OW2VIEW_VORONOISET_COLOR);
			}
			*/
			if(dfValue>0.0)
			{
				//redraw each voronoi polygon but sliced in "y" according to the volume level
				if( GetVoronoiPolygon(idpoint, pPointVoronoiPolygon, &numpointvoronoipolygon) == TRUE)
				{
					if(dfValue==1.0)
					{
						//////////////////////////////////////////////
						//redraw the voronoi polygon completely filled
						//////////////////////////////////////////////
						pDC->Polygon(pPointVoronoiPolygon, numpointvoronoipolygon); //+1 for the last point 
					}
					else
					{
						///////////////////////////////////////
						//floodfill part of the voronoi polygon
						///////////////////////////////////////
						POINT pPointOutputPolygon[100];
						int numpointoutputpolygon;
						SlicePolygon(pPointVoronoiPolygon, numpointvoronoipolygon, dfValue, pPointOutputPolygon, &numpointoutputpolygon);
						/*
						//draw the voronoi polygon
						pDC->Polygon(pPointOutputPolygon, numpointoutputpolygon);
						*/
						//for now, draw the intersection line
						if(numpointoutputpolygon==2)
						{
							pDC->MoveTo(pPointOutputPolygon[0]);
							pDC->LineTo(pPointOutputPolygon[1]);
						}
						POINT myPOINT;
						myPOINT.x=(pPointOutputPolygon[0].x+pPointOutputPolygon[1].x)/2;
						myPOINT.y=((pPointOutputPolygon[0].y+pPointOutputPolygon[1].y)/2)+1; //one pixel below the intersection line's center
						//2013july08, poirier, begin
						/*
						//pDC->FloodFill(myPOINT.x, myPOINT.y, OW2VIEW_VORONOISET_COLOR);
						long xmin, xmax, ymin, ymax;
						int idpointymin;
						GetPolygonMinMax(pPointVoronoiPolygon, numpointvoronoipolygon, &xmin, &xmax, &ymin, &ymax, &idpointymin);
						if(myPOINT.y < ymax) pDC->FloodFill(myPOINT.x, myPOINT.y, OW2VIEW_VORONOISET_COLOR);
						*/
						pDC->FloodFill(myPOINT.x, myPOINT.y, OW2VIEW_VORONOISET_COLOR);
						//2013july08, poirier, end
					}
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

		}
		pDC->SelectObject(pMyOldBrush); 
		pDC->SelectObject(pOldPen);
		if(pPen) delete pPen;
	}
}

int COW2AGView::GetVoronoiPolygon(int ivertex, POINT* pPointOutputPolygon, int* p_numpointoutputpolygon)
{
	COW2AGDoc* pDoc = (COW2AGDoc*)GetDocument();
	if(pDoc->m_pPointset==NULL || pPointOutputPolygon==NULL || p_numpointoutputpolygon==NULL)
	{
		ASSERT(FALSE);
		return 0;
	}

	int numtrifound, numneighborfound;//, itriseed;
	int p_arraytri[200];
	int p_arrayneighbor[200];
	if( FindAllValidTriSurroundingVertex( 	pDoc->m_pPointset->m_pPOINTSET,
							       			ivertex,
							       			&m_itriseed,
							       			&numtrifound,
							       			p_arraytri,
							       			&numneighborfound,
							       			p_arrayneighbor) == TRUE)
	{
		//if all surrounding triangles are valid,		 
		// build voronoi polygon using each adjtri's center 
		ASSERT(numtrifound<200); //development-time error, if allowed, define p_arraytri[n] with n>200
		for(int j=0; j<numtrifound; j++)
		{
			POINT2DD myPoint2DD;
			myPoint2DD.x = pDoc->m_pPointset->m_pPOINTSET->ctx[p_arraytri[j]];
			myPoint2DD.y = pDoc->m_pPointset->m_pPOINTSET->cty[p_arraytri[j]];
			pPointOutputPolygon[j] = MapPointFromWorldToView( myPoint2DD);
		}
		// To close the polygon, the last point is equal to the first point 
		pPointOutputPolygon[numtrifound].x = pPointOutputPolygon[0].x;
		pPointOutputPolygon[numtrifound].y = pPointOutputPolygon[0].y;
		*p_numpointoutputpolygon = numtrifound+1;
		return 1;
	}
	*p_numpointoutputpolygon=0;
	return 0; 
}

void COW2AGView::GetPolygonMinMax(POINT* pPointInputPolygon, int numpointinputpolygon, long* p_xmin, long* p_xmax, long* p_ymin, long* p_ymax, int* p_idpointymin)
{
	*p_xmin=LONG_MAX;
	*p_xmax=LONG_MIN;
	*p_ymin=LONG_MAX;
	*p_ymax=LONG_MIN;
	*p_idpointymin=-1;
	for(int i=0; i<numpointinputpolygon; i++)
	{
		if(pPointInputPolygon[i].y<*p_ymin)
		{
			*p_ymin=pPointInputPolygon[i].y;
			*p_idpointymin=i;
		}
		if(pPointInputPolygon[i].y>*p_ymax)
		{
			*p_ymax=pPointInputPolygon[i].y;
		}
		if(pPointInputPolygon[i].x<*p_xmin)
		{
			*p_xmin=pPointInputPolygon[i].x;
		}
		if(pPointInputPolygon[i].x>*p_xmax)
		{
			*p_xmax=pPointInputPolygon[i].x;
		}
	}
}

void COW2AGView::SlicePolygon(POINT* pPointInputPolygon, int numpointinputpolygon, double dfValue, POINT* pPointOutputPolygon, int* p_numpointoutputpolygon)
{
	if(dfValue>1.0)
	{
		ASSERT(FALSE);
		return;
	}
	else if(dfValue==1.0)
	{
		for(int i=0; i<numpointinputpolygon; i++)
		{
			pPointOutputPolygon[i]=pPointInputPolygon[i];
		}
		*p_numpointoutputpolygon = numpointinputpolygon;
	}
	else 
	{
		//1) find ymin and ymax (as well as xmin and xmax)
		long xmin, xmax, ymin, ymax;
		int idpointymin;
		GetPolygonMinMax(pPointInputPolygon, numpointinputpolygon, &xmin, &xmax, &ymin, &ymax, &idpointymin);

		//2) find two points where level line intersect polygon
		/*
		//   and build the two new polygons
		POINT pPointOutputPolygon1[100];
		int numpointoutputpolygon1;
		POINT pPointOutputPolygon2[100];
		int numpointoutputpolygon2;
		*/

		double pfX[100]; //we only need 2, but for safety
		double pfY[100]; //we only need 2, but for safety
		double x3, y3, x4, y4; //level line segment
		x3=xmin;
		x4=xmax;
		//y3=ymin+(ymax-ymin)*dfValue;
		y3=ymax-(ymax-ymin)*dfValue;
		y4=y3;
		int idintersect=0;
		for(int i=0; i<numpointinputpolygon-1; i++)
		{
			double x1, y1, x2, y2; //polygon segment
			x1=pPointInputPolygon[i].x;
			y1=pPointInputPolygon[i].y;
			x2=pPointInputPolygon[i+1].x;
			y2=pPointInputPolygon[i+1].y;
			if(LineSegmentsIntersect(	x1, y1, x2, y2, //line segment 1
										x3, y3, x4, y4, //line segment 2
										&pfX[idintersect], &pfY[idintersect])==1)
			{
				idintersect++;
				if(idintersect>2)
				{
					//LineSegmentsIntersect has found a third intersecting point?
					//OK, that happens because of intersection on a vertex.
					//will have to prevent duplicate points in the output.
					//ASSERT(FALSE);
				}
			}

		}
		/*
		//3) select polygon and copy polygon points to output
		int idstart=idpointymin;
		int idend=idpointymin-1;
		if(idend<0) idend=numpointinputpolygon-2;
		*/
		//for NOW, only copy intersection points to output (avoid duplicates)
		pPointOutputPolygon[0].x=(long)pfX[0];
		pPointOutputPolygon[0].y=(long)pfY[0];
		*p_numpointoutputpolygon=1;
		for(int i=1; i<idintersect; i++)
		{
			//if(pPointOutputPolygon[i].x==pPointOutputPolygon[0].x && pPointOutputPolygon[i].y==pPointOutputPolygon[0].y)
			//if(pfX[i]==pPointOutputPolygon[0].x && pfY[i]==pPointOutputPolygon[0].y)
			if( (((long)pfX[i])==pPointOutputPolygon[0].x && ((long)pfY[i])==pPointOutputPolygon[0].y)
				//|| ((i==2)&&(((long)pfX[i])==pPointOutputPolygon[1].x && ((long)pfY[i])==pPointOutputPolygon[1].y)) )
				|| ((i>1)&&(((long)pfX[i])==pPointOutputPolygon[1].x && ((long)pfY[i])==pPointOutputPolygon[1].y)) )
			{
				//skip duplicate
				continue;
			}
			else
			{
				//pPointOutputPolygon[i].x=pfX[1];
				//pPointOutputPolygon[i].y=pfY[1];
				pPointOutputPolygon[1].x=(long)pfX[i];
				pPointOutputPolygon[1].y=(long)pfY[i];
				ASSERT(*p_numpointoutputpolygon!=2); 
				//if this assert fails, we are overwriting onto the second output point.
				//that means more than 2 intersecting points were found (excluding duplicates),
				//this case was not forseen by this implementation. 
				*p_numpointoutputpolygon=2;
			}
		}
	}
}

void COW2AGView::OnLButtonDown(UINT nFlags, CPoint point)
{
    CClientDC dc(this);
    OnPrepareDC(&dc);
    CPoint logPoint = point;
	this->DPtoLP(&dc, &logPoint);

	POINT2DD myPoint2DD = MapPointFromViewToWorld(logPoint);
	int idpoint=-1;

	COW2AGDoc* pDoc = (COW2AGDoc*)GetDocument();
	if(pDoc->m_pPointset!=NULL && pDoc->m_idpointstartrow>0)
	{
		if(!(nFlags&MK_SHIFT))
		{
			////////////////////////////////////////////////////////////////////////////////
			//turn wavset on/off for the time division (time division is the voronoi region)
			////////////////////////////////////////////////////////////////////////////////

			//get nearest pointset object (idvertex, also equals the idstartsegment)
			idpoint = pDoc->GetNearestPointsetObject(myPoint2DD.x, myPoint2DD.y, &m_itriseed); 
			if(idpoint>=pDoc->m_idpointstartrow)
			{
				double dfRatio = pDoc->m_pPointset->GetStatisticsValue(idpoint, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL); //the fourth user-defined statistics is used as a volume level
				if(dfRatio>0.0)
				{
					dfRatio=0.0; //0%
				}
				else
				{
					dfRatio=1.0; //100%
				}
				pDoc->m_pPointset->SetStatisticsValue(idpoint, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL, dfRatio);
				Invalidate();
				UpdateStatusBarInfo(idpoint, dfRatio, FALSE);
			}
		}
		else if(nFlags&MK_SHIFT)
		{
			////////////////////////////////////////////////////////////////////////////////////////
			//turn wavset volume up/down for the time division (time division is the voronoi region)
			////////////////////////////////////////////////////////////////////////////////////////
			//get nearest pointset object (idvertex, also equals the idstartsegment)
			idpoint = pDoc->GetNearestPointsetObject(myPoint2DD.x, myPoint2DD.y, &m_itriseed); 
			if(idpoint>=pDoc->m_idpointstart)
			{
				/*
				double dfValue=0.50;
				*/
				POINT pPointVoronoiPolygon[100];
				int numpointvoronoipolygon=0;
				if( GetVoronoiPolygon(idpoint, pPointVoronoiPolygon, &numpointvoronoipolygon) == TRUE)
				{
					
					long xmin, xmax, ymin, ymax;
					int idpointymin;
					GetPolygonMinMax(pPointVoronoiPolygon, numpointvoronoipolygon, &xmin, &xmax, &ymin, &ymax, &idpointymin);
					double dfRatio = 0.0;
					/*
					POINT originPOINT;
					POINT2DD originPoint2DD;
					originPoint2DD.x=0;
					originPoint2DD.y=400;
					originPOINT= MapPointFromWorldToView(originPoint2DD);
					*/
					//if(ymax!=ymin) dfValue = (point.y-ymin)*1.0/(ymax-ymin);
					//if(ymax!=ymin) dfValue = abs(ymax-point.y)*1.0/abs(ymax-ymin);
					if(ymax!=ymin) dfRatio = abs(ymax-logPoint.y)*1.0/abs(ymax-ymin);
					
					/*
					POINT minPOINT, maxPOINT;
					int idpointymin;
					GetPolygonMinMax(pPointVoronoiPolygon, numpointvoronoipolygon, &minPOINT.x, &maxPOINT.y, &minPOINT.y, &maxPOINT.y, &idpointymin);
					double dfValue = 0.0;
					POINT2DD minPoint2DD = MapPointFromViewToWorld(minPOINT);
					POINT2DD maxPoint2DD = MapPointFromViewToWorld(maxPOINT);
					if(maxPoint2DD.y!=minPoint2DD.y) dfValue = abs(maxPoint2DD.y-myPoint2DD.y)*1.0/abs(maxPoint2DD.y-minPoint2DD.y);
					*/
					if(dfRatio>=0.0 && dfRatio<=1.0)
					{
						pDoc->m_pPointset->SetStatisticsValue(idpoint, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL, dfRatio);
					}
					else
					{
						//ASSERT(FALSE);
						if(dfRatio>1.0) dfRatio=1.0;
						if(dfRatio<0.0) dfRatio=0.0;
						pDoc->m_pPointset->SetStatisticsValue(idpoint, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL, dfRatio);
					}
					pDoc->UpdateNumberOfColFromControls();
					Invalidate();
					UpdateStatusBarInfo(idpoint, dfRatio);
				}
			}
		}
	}
	COW2View::OnLButtonDown(nFlags, point);
}


void COW2AGView::OnMouseMove(UINT nFlags, CPoint point)
{
	COW2View::OnMouseMove(nFlags, point);

	CClientDC dc(this);
    OnPrepareDC(&dc);
    CPoint logPoint = point;
	this->DPtoLP(&dc, &logPoint);

	POINT2DD myPoint2DD = MapPointFromViewToWorld(logPoint);
	int idpoint=-1;
	COW2AGDoc* pDoc = (COW2AGDoc*)GetDocument();
	if(pDoc->m_pPointset!=NULL && pDoc->m_idpointstart>0)
	{
		//get nearest pointset object (idvertex, also equals the idstartsegment)
		idpoint = pDoc->GetNearestPointsetObject(myPoint2DD.x, myPoint2DD.y, &m_itriseed); 
		if(idpoint>=pDoc->m_idpointstart)
		{
			double dfRatio = 0.0;
			POINT pPointVoronoiPolygon[100];
			int numpointvoronoipolygon=0;
			if( GetVoronoiPolygon(idpoint, pPointVoronoiPolygon, &numpointvoronoipolygon) == TRUE)
			{
				long xmin, xmax, ymin, ymax;
				int idpointymin;
				GetPolygonMinMax(pPointVoronoiPolygon, numpointvoronoipolygon, &xmin, &xmax, &ymin, &ymax, &idpointymin);
				if(ymax!=ymin) dfRatio = abs(ymax-logPoint.y)*1.0/abs(ymax-ymin);

				if(dfRatio>=0.0 && dfRatio<=1.0)
				{
					//pDoc->m_pPointset->SetStatisticsValue(idpoint, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL, dfRatio);
				}
				else
				{
					//ASSERT(FALSE);
					if(dfRatio>1.0) dfRatio=1.0;
					if(dfRatio<0.0) dfRatio=0.0;
					//pDoc->m_pPointset->SetStatisticsValue(idpoint, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL, dfRatio);
				}
			}
			UpdateStatusBarInfo(idpoint, dfRatio);
		}
		else
		{
			/*
			TCHAR buf[256];
			sprintf_s(buf,256,_T("         "));
			MyChangeStatusBar(1,buf);
			sprintf_s(buf,256,_T("         "));
			MyChangeStatusBar(2,buf);
			sprintf_s(buf,256,_T("         "));
			MyChangeStatusBar(3,buf);
			*/
		}
	}

}

void COW2AGView::UpdateStatusBarInfo(int idpoint, double dfRatio, int iflagUpdateThirdPane)
{
	COW2AGDoc* pDoc = (COW2AGDoc*)GetDocument();
	if(pDoc->m_pPointset==NULL)
	{
		ASSERT(FALSE);
		return;
	}
	TCHAR buf[256];
	if(idpoint==pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_VOLUME)
	{
		swprintf_s(buf,256,_T("Global Volume"));
		MyChangeStatusBar(1,buf);
		double dfValue = pDoc->GetControlUserValue(pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_VOLUME); 
		swprintf_s(buf,256,_T("Actual: %.0f"),dfValue); //presented as integer percent
		MyChangeStatusBar(2,buf);
		dfValue = pDoc->ComputeControlUserValue(pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_VOLUME, dfRatio); 
		swprintf_s(buf,256,_T("To be: %.0f"),dfValue); //presented as integer percent
		MyChangeStatusBar(3,buf);
	}
	else if(idpoint==pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_TEMPO)
	{
		swprintf_s(buf,256,_T("Global Tempo"));
		MyChangeStatusBar(1,buf);
		double dfValue = pDoc->GetControlUserValue(pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_TEMPO); 
		swprintf_s(buf,256,_T("Actual: %.0f"),dfValue); //presented as integer
		MyChangeStatusBar(2,buf);
		dfValue = pDoc->ComputeControlUserValue(pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_TEMPO, dfRatio); 
		swprintf_s(buf,256,_T("To be: %.0f"),dfValue); //presented as integer
		MyChangeStatusBar(3,buf);
	}
	else if(idpoint==pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGNUMERATOR)
	{
		swprintf_s(buf,256,_T("Global Timesignature Numerator"));
		MyChangeStatusBar(1,buf);
		double dfValue = pDoc->GetControlUserValue(pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGNUMERATOR); 
		swprintf_s(buf,256,_T("Actual: %.0f"),dfValue); //presented as integer
		MyChangeStatusBar(2,buf);
		dfValue = pDoc->ComputeControlUserValue(pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGNUMERATOR, dfRatio); 
		swprintf_s(buf,256,_T("To be: %.0f"),dfValue); //presented as integer
		MyChangeStatusBar(3,buf);
	}
	else if(idpoint==pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGDENOMINATOR)
	{
		swprintf_s(buf,256,_T("Global Timesignature Denominator"));
		MyChangeStatusBar(1,buf);
		double dfValue = pDoc->GetControlUserValue(pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGDENOMINATOR); 
		swprintf_s(buf,256,_T("Actual: %.0f"),dfValue); //presented as integer
		MyChangeStatusBar(2,buf);
		dfValue = pDoc->ComputeControlUserValue(pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGDENOMINATOR, dfRatio); 
		swprintf_s(buf,256,_T("To be: %.0f"),dfValue); //presented as integer
		MyChangeStatusBar(3,buf);
	}
	else if(idpoint==pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_MEASURESINDISPLAY)
	{
		swprintf_s(buf,256,_T("Global Number of Measures in Display"));
		MyChangeStatusBar(1,buf);
		double dfValue = pDoc->GetControlUserValue(pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_MEASURESINDISPLAY); 
		swprintf_s(buf,256,_T("Actual: %.0f"),dfValue); //presented as integer
		MyChangeStatusBar(2,buf);
		dfValue = pDoc->ComputeControlUserValue(pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_MEASURESINDISPLAY, dfRatio); 
		swprintf_s(buf,256,_T("To be: %.0f"),dfValue); //presented as integer
		MyChangeStatusBar(3,buf);
	}
	else if(idpoint==pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_NUMBEROFSTEPSPERBEAT)
	{
		swprintf_s(buf,256,_T("Global Number of Steps per Beat"));
		MyChangeStatusBar(1,buf);
		double dfValue = pDoc->GetControlUserValue(pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_NUMBEROFSTEPSPERBEAT); 
		swprintf_s(buf,256,_T("Actual: %.0f"),dfValue); //presented as integer
		MyChangeStatusBar(2,buf);
		dfValue = pDoc->ComputeControlUserValue(pDoc->m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_NUMBEROFSTEPSPERBEAT, dfRatio); 
		swprintf_s(buf,256,_T("To be: %.0f"),dfValue); //presented as integer
		MyChangeStatusBar(3,buf);
	}
	else if(idpoint>=pDoc->m_idpointstartrow)
	{
		//these are the local volumes
		swprintf_s(buf,256,_T("On/Off or Volume"));
		MyChangeStatusBar(1,buf);
		double dfRatioValue = pDoc->m_pPointset->GetStatisticsValue(idpoint, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL);
		swprintf_s(buf,256,_T("Actual: %.0f"),dfRatioValue*100.0); //presented as integer percent
		MyChangeStatusBar(2,buf);
		if(iflagUpdateThirdPane)
		{
			swprintf_s(buf,256,_T("To be: %.0f"),dfRatio*100.0); //presented as integer percent
			MyChangeStatusBar(3,buf);
		}
	}
}

BOOL COW2AGView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	BOOL bResult = COW2View::OnScroll(nScrollCode, nPos, bDoScroll); 
	COW2AGDoc* pDoc = (COW2AGDoc*)GetDocument();
	if(pDoc->m_pPointset!=NULL && pDoc->m_idpointstart>0)
	{
		//for NOW, that we are using floodfill, must redraw onscroll change
		Invalidate();
	}
	return bResult;
}

void COW2AGView::MyChangeStatusBar(int nPane,LPCTSTR text)
{
    CWinApp* pApp = AfxGetApp() ;
    CMainFrame* pFrame = (CMainFrame*)pApp->m_pMainWnd ;
	ASSERT(pFrame->IsKindOf(RUNTIME_CLASS(CMainFrame))) ;
    pFrame->MyChangePane(nPane,text);  
	/*
	COW2AGMDI* pOW2AGMDI=(COW2AGMDI*)GetParent();
    pOW2AGMDI->MyChangePane(nPane,text);  
	*/
}        


void COW2AGView::OnEditClear()
{
	//1) delete pointset
	COW2View::OnEditClear();

	COW2AGDoc* pDoc = (COW2AGDoc*)GetDocument();
	//2) empty wavset list
	if (!pDoc->m_pWavsetList->IsEmpty())
	{
		for(POSITION pos=pDoc->m_pWavsetList->GetHeadPosition(); pos!=NULL;)
		{
			WavSet* pWavset = (WavSet*) pDoc->m_pWavsetList->GetNext(pos);
			delete pWavset;
		}
	}
	pDoc->m_pWavsetList->RemoveAll();
	//3) update variables, reset row to zero, etc.
	m_WorldRect = pDoc->GetLargestWorldRect();
	m_ViewRect = pDoc->GetViewingWorldRect();	
	m_itriseed=0;
	pDoc->SetNumberOfCol(-1);
	pDoc->m_idpointstart=-1;
}
