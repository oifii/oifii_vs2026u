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
#include <windowsx.h>	// for GlobalAllocPtr()

#include "c_pointset.h"
#include "c_segmentset.h"

#include "OWpointset.h"
#include "OWedgeset.h"
#include "OWpixelset.h"

#include "OW2mdi.h"
#include "OWdoc.h"
#include "OW2doc.h"

#include "OWview.h"
#include "OW2view.h"

#include "gl_owgraph.h"
#include "owgl_scattervw.h"
#include "owgl_scatter.h"

#include "resource.h"
#include "oifii_proppage.h"
#include "oifii_app.h"
#include "logdoc.h"

#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

  
IMPLEMENT_DYNCREATE(CScatterGraph, CObject)


CScatterGraph::CScatterGraph()
{ 
	//do not use this constructor, for the moment (therefore no dynamic creation allowed)
}

CScatterGraph::~CScatterGraph()
{   
	//m_pScatterGraphView and m_pScatterGraphDoc are automaticaly deleted by the parent frame wnd
}


///////////////////////////
//to create a scatter graph
///////////////////////////
CScatterGraph::CScatterGraph(	CRect rectScatterGraphView, 
								COW2Doc* pOW2Doc, 
								CWnd* pParentWnd,
								int iPointType,
								int iPointCollectionType,
								int idROI,
								int idVertex,
								int iNumNeighbor,
								int idSegment, 
								int idstatchannel1,
								int idstatchannel2)
{
	m_pBOOL[0] = TRUE;
	m_pBOOL[1] = FALSE;
	m_pBOOL[2] = FALSE;
	m_pBOOL[3] = FALSE;
	m_pBOOL[4] = FALSE;
	m_pBOOL[5] = FALSE;
	m_pBOOL[6] = FALSE;
	m_pBOOL[7] = FALSE;
	m_pBOOL[8] = FALSE;
	m_pBOOL[9] = FALSE;
	m_nBOOL = 10;

	CreateContext(rectScatterGraphView, pOW2Doc, pParentWnd);
	//MapStatChannelIntoScatterGraph(pOW2Doc, idstatchannel, iNumberOfLevel);
	m_pScatterGraphDoc->SetTitle("Scatter Graph");
	m_pScatterGraphDoc->SetViewGrid(GRID_HORZ |GRID_VERT);
	m_pScatterGraphDoc->SetXAxisLabel("Stat1");
	m_pScatterGraphDoc->SetYAxisLabel("Stat2");
	Update(	pOW2Doc,
			iPointType,
			iPointCollectionType,
			idROI,
			idVertex, 
			iNumNeighbor,
			idSegment, 
			idstatchannel1, 
			idstatchannel2);
}



/////////////////////////
//to update scatter graph
/////////////////////////
BOOL CScatterGraph::Update(	COW2Doc* pDoc, 
							int iPointType,
							int iPointCollectionType,
							int idROI,
							int idVertex, 
							int iNumNeighbor,
							int idSegment, 
							int idchannelStat1, 
							int idchannelStat2)
{
	//on update, store previous graphset visibility attributes
	/*
	if(m_pScatterGraphDoc->GetNumberOfSets()>0)
	{
		m_pScatterGraphDoc->GetVisibilityOfAllGraphSets(m_pBOOL, &m_nBOOL);
	}
	*/

	m_pScatterGraphDoc->RemoveAllGraphSet();
	//scatter graph
	if(pDoc==NULL) return FALSE;
	MapStatChannelIntoScatterGraph(	pDoc, 
									iPointType,
									iPointCollectionType,
									idROI,
									idVertex, 
									iNumNeighbor, 
									idSegment, 
									idchannelStat1, 
									idchannelStat2);
	return TRUE;
}



void CScatterGraph::CreateContext(CRect rectScatterGraphView, COW2Doc* pOW2Doc, CWnd* pParentWnd)
{
	CRuntimeClass* pScatterGraphDocClass = RUNTIME_CLASS(COWGraphDoc);
	m_pScatterGraphDoc = (COWGraphDoc*) (pScatterGraphDocClass->CreateObject());
    ASSERT(m_pScatterGraphDoc);
    
	CRuntimeClass* pScatterGraphViewClass = RUNTIME_CLASS(CScatterGraphView);
	m_pScatterGraphView = (CScatterGraphView*) pScatterGraphViewClass->CreateObject();
    ASSERT(m_pScatterGraphView);
          
	CCreateContext context;
	context.m_pNewViewClass		= pScatterGraphViewClass;
	context.m_pCurrentDoc		= m_pScatterGraphDoc;
	context.m_pNewDocTemplate	= NULL;
	context.m_pLastView			= NULL;
	if(pParentWnd->IsKindOf(RUNTIME_CLASS(CPropertySheet))) context.m_pCurrentFrame	= NULL;
	  else if(pParentWnd->IsKindOf(RUNTIME_CLASS(CView))) context.m_pCurrentFrame = pParentWnd->GetParentFrame();
	  else if(pParentWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd))) context.m_pCurrentFrame	= (CFrameWnd*) pParentWnd;

    if (!m_pScatterGraphView->Create(NULL, NULL, WS_CHILD|(TRUE ? WS_VISIBLE:0),rectScatterGraphView,
		pParentWnd, AFX_IDW_PANE_FIRST+1, &context))
	{
	    	TRACE0("Warning: couldn't create view for frame\n");
	    	return;
	}
	// commented out since integration into CPropertySheet in OIII
	//pParentWnd->GetParentFrame()->SetActiveView(m_pScatterGraphView);

	// get data from ow2doc
    // get scatter graph data from specified document

	// TODO: add your code in here if you want this class to automatically
	//       get the data from the image document
	return;
}


BOOL CScatterGraph::MapStatChannelIntoScatterGraph(	COW2Doc* pOW2Doc, 
													int iPointType,
													int iPointCollectionType,
													int idROI,
													int idVertex, 
													int iNumNeighbor, 
													int idSegment, 
													int idchannelStat1, 
													int idchannelStat2)
{
	BOOL bResult = TRUE;
	//iNumberOfPoints is not used for now, but it is required to have an extra parameter
	//to distinguish from the various versions of constructor and update funtions 
	if(pOW2Doc==NULL) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	ASSERT(pOW2Doc->IsKindOf(RUNTIME_CLASS(COW2Doc)));
	//if(pOW2Doc->m_pPointset==NULL) return FALSE; //statistics would not be allocated yet


	if(idVertex==-1 && iNumNeighbor==-1 && idSegment==-1 && idROI==-1)
	{
		idROI = EDGESET_LARGEST_WORLDRECT_ROI_ID;
	}

	/////////////////////////////
	//scatter graph from pixelset
	/////////////////////////////

	if(iPointType==SCATTER_IMAGEPIXELS && idROI!=-1)
	{
		bResult = TRUE;

		//set graph type
		m_pScatterGraphDoc->SetGraphType(GRAPH_SCATTER);

		//get roi bounding rect
		CRect myRect = pOW2Doc->m_pROIset->GetRectROI(idROI);
		myRect.IntersectRect(&myRect, &pOW2Doc->m_pPixelset->GetCropRect());

		//get roi info
		POLYLINE* pPOLYLINE = pOW2Doc->m_pROIset->GetInfoROI(idROI);

		if(iPointCollectionType==SCATTER_BR_INTERIOR)
		{
			//bounding rect interior
			ASSERT(pOW2Doc->m_pPixelset->IsBIIPresent());
			int nchannels = pOW2Doc->m_pPixelset->GetPixelsetNumberOfChannel();

			const char* pPSZ[] = {"Red", "Green", "Blue", "Gray"};
			ASSERT(nchannels<=4); //otherwise revise the line above, for the least!
			
			int width = myRect.Width();
			int height = myRect.Height();
 			int npoints = width*height;
			//double* pfStat1 = new double[npoints];
			//double* pfStat2 = new double[npoints];
			int* pfStat1 = new int[npoints];
			int* pfStat2 = new int[npoints];
			for(int j=0; j<height; j++)
			{
				for(int i=0; i<width; i++)
				{
					pfStat1[j*width+i] = pOW2Doc->m_pPixelset->GetPixelValue(myRect.left+i, myRect.top+j, idchannelStat1);
					pfStat2[j*width+i] = pOW2Doc->m_pPixelset->GetPixelValue(myRect.left+i, myRect.top+j, idchannelStat2);
				}
			}
			//add the scatter graph vector to the graph document
			m_pScatterGraphDoc->AddGraphSet(pfStat1, npoints, pPSZ[idchannelStat1]);
			m_pScatterGraphDoc->AddGraphSet(pfStat2, npoints, pPSZ[idchannelStat2]);

			delete[] pfStat1;
			delete[] pfStat2;
			//m_pScatterGraphDoc->SetVisibilityOfAllGraphSets(m_pBOOL, m_nBOOL);
		}
		else 
		{
			int npoints = 0;
			POINT* pPOINT = NULL;
			POINT* pTempPOINT = NULL;

			//polyline
			int npoint_e_max = 0;
			int npoint_ioe_max = 0;
			int npoint_iebr_max = 0;
			int npoint_oebr_max = 0;
			int ireturn = pOW2Doc->m_pROIset->EstimatePolylineFillMemoryRequirement(pPOLYLINE,
																					&npoint_e_max,
																					&npoint_ioe_max,
																					&npoint_iebr_max,
																					&npoint_oebr_max);
			POINT* pEdgePOINT = (POINT*) new POINT[npoint_e_max];
			int iGridStep = 1;
			POINT* pInnerEdgePOINT = NULL;
			POINT* pOutterEdgePOINT = NULL;
			int npoint_ioe_collected = 0;
			POINT* pInnerEBRPOINT = NULL;
			POINT* pOutterEBRPOINT = NULL;
			int npoint_iebr_collected = 0;
			int npoint_oebr_collected = 0;

			if(iPointCollectionType==HISTO_SBR_INTERIOR
				|| iPointCollectionType==HISTO_SBR_INNER_EDGE_HALF
				|| iPointCollectionType==HISTO_SBR_OUTTER_EDGE_HALF)
			{
				//sub bounding rects
				if(iPointCollectionType==HISTO_SBR_INTERIOR)
				{
					pInnerEdgePOINT = NULL;
					pOutterEdgePOINT = NULL;
					npoint_ioe_max = 0;

					pInnerEBRPOINT = (POINT*) new POINT[npoint_iebr_max];
					pOutterEBRPOINT = (POINT*) new POINT[npoint_oebr_max];
				}
				else if(iPointCollectionType==HISTO_SBR_INNER_EDGE_HALF)
				{
					pInnerEdgePOINT = NULL;
					pOutterEdgePOINT = NULL;
					npoint_ioe_max = 0;

					pInnerEBRPOINT = (POINT*) new POINT[npoint_iebr_max];
					pOutterEBRPOINT = NULL;
				}
				else if(iPointCollectionType==HISTO_SBR_OUTTER_EDGE_HALF)
				{
					pInnerEdgePOINT = NULL;
					pOutterEdgePOINT = NULL;
					npoint_ioe_max = 0;

					pInnerEBRPOINT = NULL;
					pOutterEBRPOINT = (POINT*) new POINT[npoint_oebr_max];
				}
			}
			else if(iPointCollectionType==HISTO_EDGE_ALONG
					|| iPointCollectionType==HISTO_EDGE_INNER
					|| iPointCollectionType==HISTO_EDGE_OUTTER
					|| iPointCollectionType==HISTO_EDGE_AIO)
			{
				//edge
				if(iPointCollectionType==HISTO_EDGE_ALONG)
				{
					pInnerEdgePOINT = NULL;
					pOutterEdgePOINT = NULL;
					npoint_ioe_max = 0;

					pInnerEBRPOINT = NULL;
					pOutterEBRPOINT = NULL;
					npoint_iebr_max = 0;
					npoint_oebr_max = 0;
				}
				else if(iPointCollectionType==HISTO_EDGE_INNER)
				{
					pInnerEdgePOINT = (POINT*) new POINT[npoint_ioe_max];
					pOutterEdgePOINT = NULL;

					pInnerEBRPOINT = NULL;
					pOutterEBRPOINT = NULL;
					npoint_iebr_max = 0;
					npoint_oebr_max = 0;
				}
				else if(iPointCollectionType==HISTO_EDGE_OUTTER)
				{
					pInnerEdgePOINT = NULL;
					pOutterEdgePOINT = (POINT*) new POINT[npoint_ioe_max];

					pInnerEBRPOINT = NULL;
					pOutterEBRPOINT = NULL;
					npoint_iebr_max = 0;
					npoint_oebr_max = 0;
				}
				else if(iPointCollectionType==HISTO_EDGE_AIO)
				{
					pInnerEdgePOINT = (POINT*) new POINT[npoint_ioe_max];
					pOutterEdgePOINT = (POINT*) new POINT[npoint_ioe_max];

					pInnerEBRPOINT = NULL;
					pOutterEBRPOINT = NULL;
					npoint_iebr_max = 0;
					npoint_oebr_max = 0;
				}


			}
			else if(iPointCollectionType==HISTO_POLYGON_INTERIOR)
			{
				//polygon interior
				ASSERT(FALSE);
			}
			else
			{
				ASSERT(FALSE);
			}

			//collect points
			npoints = pOW2Doc->m_pROIset->PolylineFill(	pPOLYLINE, 							
														pEdgePOINT, //allocated by caller to collect all intermediate points between Pi and Pi+1 segments (given the grid step)
														npoint_e_max, //maximum number of POINT structure allocated by caller
														iGridStep,
														pInnerEdgePOINT,
														pOutterEdgePOINT,
														npoint_ioe_max,
														&npoint_ioe_collected,
														pInnerEBRPOINT,
														npoint_iebr_max,
														&npoint_iebr_collected,
														pOutterEBRPOINT,
														npoint_oebr_max,
														&npoint_oebr_collected);
			pPOINT = NULL;
			if(iPointCollectionType==HISTO_SBR_INTERIOR)
			{
				//merge the 3 vectors
				/*
				npoints = npoints + 2 * npoint_ioebr_collected;
				pPOINT = pEdgePOINT;
				pPOINT = pInnerEBRPOINT;
				pPOINT = pOutterEBRPOINT;
				*/
				pTempPOINT = (POINT*) new POINT[npoints + npoint_iebr_collected + npoint_oebr_collected];
				memcpy(pTempPOINT, pEdgePOINT, npoints*sizeof(POINT));
				memcpy(&(pTempPOINT[npoints]), pInnerEBRPOINT, npoint_iebr_collected*sizeof(POINT));
				memcpy(&(pTempPOINT[npoints+npoint_iebr_collected]), pOutterEBRPOINT, npoint_oebr_collected*sizeof(POINT));
				npoints = npoints + npoint_iebr_collected + npoint_oebr_collected;
				pPOINT = pTempPOINT;
			}
			else if(iPointCollectionType==HISTO_SBR_INNER_EDGE_HALF)
			{
				npoints = npoint_iebr_collected;
				pPOINT = pInnerEBRPOINT;
			}
			else if(iPointCollectionType==HISTO_SBR_OUTTER_EDGE_HALF)
			{
				npoints = npoint_oebr_collected;
				pPOINT = pOutterEBRPOINT;
			}
			else if(iPointCollectionType==HISTO_EDGE_ALONG)
			{
				npoints = npoints;
				pPOINT = pEdgePOINT;
			}
			else if(iPointCollectionType==HISTO_EDGE_INNER)
			{
				npoints = npoint_ioe_collected;
				pPOINT = pInnerEdgePOINT;
			}
			else if(iPointCollectionType==HISTO_EDGE_OUTTER)
			{
				npoints = npoint_ioe_collected;
				pPOINT = pOutterEdgePOINT;
			}
			else if(iPointCollectionType==HISTO_EDGE_AIO)
			{
				/*
				npoints = npoints + 2*npoint_ioe_collected;
				pPOINT = pAlongEdgePOINT;
				pPOINT = pInnerEdgePOINT;
				pPOINT = pOutterEdgePOINT;
				*/
				pTempPOINT = (POINT*) new POINT[npoints + 2 * npoint_ioe_collected];
				memcpy(pTempPOINT, pEdgePOINT, npoints*sizeof(POINT));
				memcpy(&(pTempPOINT[npoints]), pInnerEdgePOINT, npoint_ioe_collected*sizeof(POINT));
				memcpy(&(pTempPOINT[npoints+npoint_ioe_collected]), pOutterEdgePOINT, npoint_ioe_collected*sizeof(POINT));
				npoints = npoints + 2 * npoint_ioe_collected;
				pPOINT = pTempPOINT;
			}
			else
			{
				ASSERT(FALSE);
				return FALSE;
			}

			#ifdef _DEBUG
			if(npoints<100)
			{
				CString myString;
				((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText("Points for histogram\r\n");
				for(int ii=0; ii<npoints; ii++)
				{

					myString.Format(L"(x,y) (%d,%d)\r\n", pPOINT[ii]);
					((COIIIApp*)AfxGetApp())->GetLogDocument()->AddTextW(myString);
				}
			}
			#endif //_DEBUG

			const char* pPSZ[] = {"Red", "Green", "Blue", "Gray"};

			double* pfStat1 = new double[npoints];
			double* pfStat2 = new double[npoints];
			for(int iii=0; iii<npoints; iii++)
			{
				pfStat1[iii] = pOW2Doc->m_pPixelset->GetPixelValue(pPOINT[iii].x, pPOINT[iii].y, idchannelStat1);
				pfStat2[iii] = pOW2Doc->m_pPixelset->GetPixelValue(pPOINT[iii].x, pPOINT[iii].y, idchannelStat2);
			}
			//add the scatter graph vector to the graph document
			m_pScatterGraphDoc->AddGraphSet(pfStat1, npoints, pPSZ[idchannelStat1]);
			m_pScatterGraphDoc->AddGraphSet(pfStat2, npoints, pPSZ[idchannelStat2]);

			delete[] pfStat1;
			delete[] pfStat2;

			//m_pScatterGraphDoc->SetVisibilityOfAllGraphSets(m_pBOOL, m_nBOOL);

			if(pEdgePOINT) delete[] pEdgePOINT;
			if(pInnerEdgePOINT) delete[] pInnerEdgePOINT;
			if(pOutterEdgePOINT) delete[] pOutterEdgePOINT;
			if(pInnerEBRPOINT) delete[] pInnerEBRPOINT;
			if(pOutterEBRPOINT) delete[] pOutterEBRPOINT;

			if(pTempPOINT) delete[] pTempPOINT;

		}
	}
	else
	{

		/////////////////////////////
		//scatter graph from pointset
		/////////////////////////////

		//global
		double* pdfBaseStatistics = pOW2Doc->GetPointerToStatistics();
		if(pdfBaseStatistics==NULL)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		int iNumberOfPoint = pOW2Doc->GetNumberOfPoint();
		int iNumberOfStatPerPoint = pOW2Doc->GetNumberOfStatPerPoint();
		ASSERT(idchannelStat1>-1 && idchannelStat1<iNumberOfStatPerPoint);
		ASSERT(idchannelStat2>-1 && idchannelStat2<iNumberOfStatPerPoint);

		//for now, always allocate the maximum possible number of points
		double* pfArray1 = (double*) new double[iNumberOfPoint];
		if(pfArray1==NULL) 
		{
			ASSERT(FALSE);
			return FALSE;
		}
		double* pfArray2 = (double*) new double[iNumberOfPoint];
		if(pfArray2==NULL) 
		{
			ASSERT(FALSE);
			return FALSE;
		}

		int iReject=0;
		int ii = -1;
 		if(idVertex==-1 && iNumNeighbor==-1 && idSegment==-1
			&& (idROI==-1 || idROI==EDGESET_LARGEST_WORLDRECT_ROI_ID) )
		{
			//global scatter graph request

			//get the statistics from global channels
			for(int i=0; i<iNumberOfPoint; i++)
			{
				//get value
				int iStatOffset = i*iNumberOfStatPerPoint;
				double fValue1 = pdfBaseStatistics[iStatOffset+idchannelStat1]; 						
				double fValue2 = pdfBaseStatistics[iStatOffset+idchannelStat2]; 						

				if(fValue1<0 || fValue2<0)
				{
					//exceptions, voronoi area that cannot be computed are -9.99, do not include these points
					iReject++;
				}
				else
				{
					ii++;
					//set arrays
					pfArray1[ii] = fValue1;
					pfArray2[ii] = fValue2;
				}
			}
		}
		else if(idVertex==-1 && iNumNeighbor==-1 && idSegment==-1 && idROI!=-1)
		{
			//local roi, get roi bounding rect
			CRect myRect = pOW2Doc->m_pROIset->GetRectROI(idROI);
			
			//itriseed, default is 0
			int itriseed = 0;
			int* p_itriseed = &itriseed;
			//itriseed from COW2View (much better)
			COW2View* pOW2View = NULL;
			POSITION pos = pOW2Doc->GetFirstViewPosition(); 
			while(pos)
			{
				CView* pView = pOW2Doc->GetNextView(pos); 
				if(pView->IsKindOf(RUNTIME_CLASS(COW2View)))
				{
					pOW2View = (COW2View*)pView;
					p_itriseed = &(pOW2View->m_itriseed);
					break;
				}
			}

			//collect npoints (vertex) for histo graph
			/*
			int npoints = 0;
			IDVERTEX* pIDVERTEX = NULL;
			*/
			int npoints = 0;
			int p_npoints[3];
			IDVERTEX* pIDVERTEX[3];
			p_npoints[0] = 0;
			p_npoints[1] = 0;
			p_npoints[2] = 0;
			pIDVERTEX[0] = NULL;
			pIDVERTEX[1] = NULL;
			pIDVERTEX[2] = NULL;

			//prepare EDGEFILLPARAM  
			EDGEFILLPARAM myEDGEFILLPARAM;

			if(iPointCollectionType==SCATTER_BR_INTERIOR)
			{
				POINT myP1;
				POINT myP2;
				myP1.x =  myRect.left;
				myP1.y = myRect.top;
				myP2.x = myRect.right;
				myP2.y = myRect.bottom;

				//convert to OW2Doc coordinate system
				POINT2DD P1 = pOW2View->MapPointFromViewToWorld(myP1);
				POINT2DD P2 = pOW2View->MapPointFromViewToWorld(myP2);

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
				//EDGEFILLPARAM myEDGEFILLPARAM;
				myEDGEFILLPARAM.pIDTRI = (int*) new int[ntri_max];		
				myEDGEFILLPARAM.ntri_max = ntri_max;		
				myEDGEFILLPARAM.p_ntrifound = &ntrifound;
				myEDGEFILLPARAM.p_itriseed = p_itriseed;

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

				int ntri_interior = pOW2Doc->m_pPointset->RectFill(&myRECT2DD, &myEDGEFILLPARAM);

				//these 3 sets represent the rect interior 
				p_npoints[0] = myEDGEFILLPARAM.p_nvertexfound[0]; //edge
				p_npoints[1] = myEDGEFILLPARAM.p_nvertexfound_iebr[0]; //iebr
				p_npoints[2] = myEDGEFILLPARAM.p_nvertexfound_oebr[0]; //iebr
				npoints = p_npoints[0] + p_npoints[1] + p_npoints[2];
				pIDVERTEX[0] = myEDGEFILLPARAM.pIDVERTEX;
				pIDVERTEX[1] = myEDGEFILLPARAM.pIDVERTEX_iebr;
				pIDVERTEX[2] = myEDGEFILLPARAM.pIDVERTEX_oebr;
			}
			else if(iPointCollectionType==SCATTER_SBR_INTERIOR
				|| iPointCollectionType==SCATTER_SBR_INNER_EDGE_HALF
				|| iPointCollectionType==SCATTER_SBR_OUTTER_EDGE_HALF
				|| iPointCollectionType==SCATTER_EDGE_ALONG
				|| iPointCollectionType==SCATTER_EDGE_INNER
				|| iPointCollectionType==SCATTER_EDGE_OUTTER
				|| iPointCollectionType==SCATTER_EDGE_AIO)
			{
				POLYLINE* pPOLYLINE = pOW2Doc->m_pROIset->GetInfoROI(idROI);

				int nvertex_e_max = 0;
				int nvertex_ioe_max = 0;
				int nvertex_iebr_max = 0;
				int nvertex_oebr_max = 0;
				int ireturn = pOW2Doc->m_pROIset->EstimatePolylineFillMemoryRequirement(pPOLYLINE,
																						&nvertex_e_max,
																						&nvertex_ioe_max,
																						&nvertex_iebr_max,
																						&nvertex_oebr_max);
				//edge
				int nvertex_max = nvertex_e_max;
				int nvertex_max2 = 0;
				int ntri_max = nvertex_max;//2*nvertex_max;
				int ntrifound = 0;
				int nvertexfound = 0;
				int nvertexfound2 = 0;
				//EDGEFILLPARAM myEDGEFILLPARAM;
				myEDGEFILLPARAM.pIDTRI = (int*) new int[ntri_max];		
				myEDGEFILLPARAM.ntri_max = ntri_max;		
				myEDGEFILLPARAM.p_ntrifound = &ntrifound;
				myEDGEFILLPARAM.p_itriseed = p_itriseed;

				myEDGEFILLPARAM.pIDVERTEX = (int*) new int[nvertex_max];		
				myEDGEFILLPARAM.nvertex_max = nvertex_max;	
				myEDGEFILLPARAM.p_nvertexfound = &nvertexfound;
				/*
				myEDGEFILLPARAM.pIDVERTEX2 = (int*) new int[nvertex_max2];	
				myEDGEFILLPARAM.nvertex_max2 = nvertex_max2;	
				myEDGEFILLPARAM.p_nvertexfound2 = &nvertexfound2;
				*/

				if(iPointCollectionType==SCATTER_SBR_INTERIOR)
				{
					//edge, iebr and oebr

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
				}
				else if(iPointCollectionType==SCATTER_SBR_INNER_EDGE_HALF)
				{
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
				}
				else if(iPointCollectionType==SCATTER_SBR_OUTTER_EDGE_HALF)
				{
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
				}
				else if(iPointCollectionType==SCATTER_EDGE_ALONG)
				{
				}	
				else if(iPointCollectionType==SCATTER_EDGE_INNER)
				{
					//inner edge
					int ntri_ie_max = ntri_max;
					int nvertex_ie_max = nvertex_max;
					int ntrifound_ie = 0;
					int nvertexfound_ie = 0;
					myEDGEFILLPARAM.pIDTRI_ie = (int*) new int[ntri_ie_max];	
					myEDGEFILLPARAM.ntri_ie_max = ntri_ie_max;	
					myEDGEFILLPARAM.p_ntrifound_ie = &ntrifound_ie;
					myEDGEFILLPARAM.pIDVERTEX_ie = (int*) new int[nvertex_ie_max];
					myEDGEFILLPARAM.nvertex_ie_max = nvertex_ie_max;	
					myEDGEFILLPARAM.p_nvertexfound_ie = &nvertexfound_ie;
				}				
				else if(iPointCollectionType==SCATTER_EDGE_OUTTER)
				{
					//outter edge
					int ntri_oe_max = ntri_max;
					int nvertex_oe_max = nvertex_max;
					int ntrifound_oe = 0;
					int nvertexfound_oe = 0;
					myEDGEFILLPARAM.pIDTRI_oe = (int*) new int[ntri_oe_max];	
					myEDGEFILLPARAM.ntri_oe_max = ntri_oe_max;	
					myEDGEFILLPARAM.p_ntrifound_oe = &ntrifound_oe;
					myEDGEFILLPARAM.pIDVERTEX_oe = (int*) new int[nvertex_oe_max];
					myEDGEFILLPARAM.nvertex_oe_max = nvertex_oe_max;	
					myEDGEFILLPARAM.p_nvertexfound_oe = &nvertexfound_oe;
				}				
				else if(iPointCollectionType==SCATTER_EDGE_AIO)
				{
					//inner edge
					int ntri_ie_max = ntri_max;
					int nvertex_ie_max = nvertex_max;
					int ntrifound_ie = 0;
					int nvertexfound_ie = 0;
					myEDGEFILLPARAM.pIDTRI_ie = (int*) new int[ntri_ie_max];	
					myEDGEFILLPARAM.ntri_ie_max = ntri_ie_max;	
					myEDGEFILLPARAM.p_ntrifound_ie = &ntrifound_ie;
					myEDGEFILLPARAM.pIDVERTEX_ie = (int*) new int[nvertex_ie_max];
					myEDGEFILLPARAM.nvertex_ie_max = nvertex_ie_max;	
					myEDGEFILLPARAM.p_nvertexfound_ie = &nvertexfound_ie;

					//outter edge
					int ntri_oe_max = ntri_max;
					int nvertex_oe_max = nvertex_max;
					int ntrifound_oe = 0;
					int nvertexfound_oe = 0;
					myEDGEFILLPARAM.pIDTRI_oe = (int*) new int[ntri_oe_max];	
					myEDGEFILLPARAM.ntri_oe_max = ntri_oe_max;	
					myEDGEFILLPARAM.p_ntrifound_oe = &ntrifound_oe;
					myEDGEFILLPARAM.pIDVERTEX_oe = (int*) new int[nvertex_oe_max];
					myEDGEFILLPARAM.nvertex_oe_max = nvertex_oe_max;	
					myEDGEFILLPARAM.p_nvertexfound_oe = &nvertexfound_oe;
				}				
				
				int ntri_e = pOW2Doc->m_pPointset->PolylineFill(pOW2Doc->m_pROIset,
																pPOLYLINE, 
																&myEDGEFILLPARAM,
																pOW2View); //for view to world coordinate conversion


				if(iPointCollectionType==SCATTER_SBR_INTERIOR)
				{
					//these 3 sets represent the sum of all sub-bounding rect interior 
					p_npoints[0] = myEDGEFILLPARAM.p_nvertexfound[0]; //edge
					p_npoints[1] = myEDGEFILLPARAM.p_nvertexfound_iebr[0]; //iebr
					p_npoints[2] = myEDGEFILLPARAM.p_nvertexfound_oebr[0]; //oebr
					npoints = p_npoints[0] + p_npoints[1] + p_npoints[2];
					pIDVERTEX[0] = myEDGEFILLPARAM.pIDVERTEX;
					pIDVERTEX[1] = myEDGEFILLPARAM.pIDVERTEX_iebr;
					pIDVERTEX[2] = myEDGEFILLPARAM.pIDVERTEX_oebr;
				}
				else if(iPointCollectionType==SCATTER_SBR_INNER_EDGE_HALF)
				{
					//these 3 sets represent the sum of all inner edge sub-bounding rect interior 
					p_npoints[0] = myEDGEFILLPARAM.p_nvertexfound[0]; //edge
					p_npoints[1] = myEDGEFILLPARAM.p_nvertexfound_iebr[0]; //iebr
					p_npoints[2] = 0; //oebr
					npoints = p_npoints[0] + p_npoints[1];
					pIDVERTEX[0] = myEDGEFILLPARAM.pIDVERTEX;
					pIDVERTEX[1] = myEDGEFILLPARAM.pIDVERTEX_iebr;
					pIDVERTEX[2] = NULL;
				}
				else if(iPointCollectionType==SCATTER_SBR_OUTTER_EDGE_HALF)
				{
					//these 3 sets represent the sum of all sub-bounding rect interior 
					p_npoints[0] = myEDGEFILLPARAM.p_nvertexfound[0]; //edge
					p_npoints[1] = myEDGEFILLPARAM.p_nvertexfound_oebr[0]; //iebr
					p_npoints[2] = 0;
					npoints = p_npoints[0] + p_npoints[1];
					pIDVERTEX[0] = myEDGEFILLPARAM.pIDVERTEX;
					pIDVERTEX[1] = myEDGEFILLPARAM.pIDVERTEX_oebr;
					pIDVERTEX[2] = NULL;
				}
				else if(iPointCollectionType==SCATTER_EDGE_ALONG)
				{
					p_npoints[0] = myEDGEFILLPARAM.p_nvertexfound[0]; //along edge
					p_npoints[1] = 0; 
					p_npoints[2] = 0; 
					npoints = p_npoints[0];
					pIDVERTEX[0] = myEDGEFILLPARAM.pIDVERTEX;
					pIDVERTEX[1] = NULL;
					pIDVERTEX[2] = NULL;
				}	
				else if(iPointCollectionType==SCATTER_EDGE_INNER)
				{
					p_npoints[0] = myEDGEFILLPARAM.p_nvertexfound_ie[0]; //inner edge
					p_npoints[1] = 0; 
					p_npoints[2] = 0; 
					npoints = p_npoints[0];
					pIDVERTEX[0] = myEDGEFILLPARAM.pIDVERTEX_ie;
					pIDVERTEX[1] = NULL;
					pIDVERTEX[2] = NULL;
				}				
				else if(iPointCollectionType==SCATTER_EDGE_OUTTER)
				{
					p_npoints[0] = myEDGEFILLPARAM.p_nvertexfound_oe[0]; //outter edge
					p_npoints[1] = 0; 
					p_npoints[2] = 0; 
					npoints = p_npoints[0];
					pIDVERTEX[0] = myEDGEFILLPARAM.pIDVERTEX_oe;
					pIDVERTEX[1] = NULL;
					pIDVERTEX[2] = NULL;
				}				
				else if(iPointCollectionType==SCATTER_EDGE_AIO)
				{
					p_npoints[0] = myEDGEFILLPARAM.p_nvertexfound[0]; //edge
					p_npoints[1] = myEDGEFILLPARAM.p_nvertexfound_ie[0]; //inner edge 
					p_npoints[2] = myEDGEFILLPARAM.p_nvertexfound_oe[0]; //outter edge 
					npoints = p_npoints[0] + p_npoints[1] + p_npoints[2];
					pIDVERTEX[0] = myEDGEFILLPARAM.pIDVERTEX;
					pIDVERTEX[1] = myEDGEFILLPARAM.pIDVERTEX_ie;
					pIDVERTEX[2] = myEDGEFILLPARAM.pIDVERTEX_oe;
				}				
			}
			ASSERT(pOW2Doc->m_pPointset->m_pPOINTSET->pfStatistics);
			int nchannels = pOW2Doc->m_pPointset->GetNumberOfStatPerPoint();

			ASSERT(nchannels<=(4+9)); //otherwise revise the line above, for the least!
			
			int iReject = 0;
			double* pDOUBLE1 = (double*) new double[npoints];
			double* pDOUBLE2 = (double*) new double[npoints];
			int j_start = 0;
			int j_end = nchannels;
			if(idchannelStat1>=j_start && idchannelStat1<nchannels
				&& idchannelStat2>=j_start && idchannelStat2<nchannels)
			{
				//add one channel, the selected one
				//get vertex statistic
				int index = 0;
				int k=0; while(k<3 && p_npoints[k]>0)
				{
					for(int i=0; i<p_npoints[k]; i++)
					{
						double fValue1 = pOW2Doc->m_pPointset->GetStatisticsValue(pIDVERTEX[k][i], idchannelStat1);
						double fValue2 = pOW2Doc->m_pPointset->GetStatisticsValue(pIDVERTEX[k][i], idchannelStat2);
						if(fValue1<0.0 || fValue2<0.0)
						{
							iReject++;
						}
						else
						{
							pDOUBLE1[index] = fValue1;
							pDOUBLE2[index] = fValue2;
							index++;
						}
					}
					//index = index + p_npoints[k];
					k++;
				}
			}
			iNumberOfPoint = npoints;

			//set graph type
			m_pScatterGraphDoc->SetGraphType(GRAPH_SCATTER);
			//2) add the scatter graph vector to the graph document
			m_pScatterGraphDoc->AddGraphSet(pDOUBLE1, iNumberOfPoint-iReject, pszStatChannelNames[idchannelStat1]);
			m_pScatterGraphDoc->AddGraphSet(pDOUBLE2, iNumberOfPoint-iReject, pszStatChannelNames[idchannelStat2]);

			//m_pScatterGraphDoc->SetVisibilityOfAllGraphSets(m_pBOOL, m_nBOOL);
			delete[] pDOUBLE1;
			delete[] pDOUBLE2;

			if(myEDGEFILLPARAM.pIDTRI) delete[] myEDGEFILLPARAM.pIDTRI;
			if(myEDGEFILLPARAM.pIDVERTEX) delete[] myEDGEFILLPARAM.pIDVERTEX;
			if(myEDGEFILLPARAM.pIDVERTEX2) delete[] myEDGEFILLPARAM.pIDVERTEX2;
			if(myEDGEFILLPARAM.pIDTRI_ie) delete[] myEDGEFILLPARAM.pIDTRI_ie;
			if(myEDGEFILLPARAM.pIDVERTEX_ie) delete[] myEDGEFILLPARAM.pIDVERTEX_ie;
			if(myEDGEFILLPARAM.pIDTRI_oe) delete[] myEDGEFILLPARAM.pIDTRI_oe;
			if(myEDGEFILLPARAM.pIDVERTEX_oe) delete[] myEDGEFILLPARAM.pIDVERTEX_oe;
			if(myEDGEFILLPARAM.pIDTRI_iebr) delete[] myEDGEFILLPARAM.pIDTRI_iebr;
			if(myEDGEFILLPARAM.pIDVERTEX_iebr) delete[] myEDGEFILLPARAM.pIDVERTEX_iebr;
			if(myEDGEFILLPARAM.pIDTRI_oebr) delete[] myEDGEFILLPARAM.pIDTRI_oebr;
			if(myEDGEFILLPARAM.pIDVERTEX_oebr) delete[] myEDGEFILLPARAM.pIDVERTEX_oebr;

			return TRUE;
		}
		else if(idVertex!=-1 && iNumNeighbor>-1 && idSegment==-1)
		{
			//circular neighborhood scatter graph request

			//get circular profile
 			int ifinalnumfound;
			int p_ifinalarrayneighbor[200];
			BOOL bResult = pOW2Doc->GetCircularNeighborhoodProfileAroundVertex(	idVertex, 
																				iNumNeighbor,
																				0, //does not matter since we provide a NULL pfStatistics pointer 
																				&ifinalnumfound,
																				p_ifinalarrayneighbor,
																				NULL); //NULL if we do not want statistics to be collected as well
			iNumberOfPoint = ifinalnumfound;

			//get the statistics for the local scatter graph
			for(int i=0; i<iNumberOfPoint; i++)
			{
				//get value
				int iStatOffset = p_ifinalarrayneighbor[i]*iNumberOfStatPerPoint;
				double fValue1 = pdfBaseStatistics[iStatOffset+idchannelStat1]; 						
				double fValue2 = pdfBaseStatistics[iStatOffset+idchannelStat2]; 						

				if(fValue1<0 || fValue2<0)
				{
					//exceptions, voronoi area that cannot be computed are -9.99, do not include these points
					iReject++;
				}
				else
				{
					ii++;
					//set arrays
					pfArray1[ii] = fValue1;
					pfArray2[ii] = fValue2;
				}
			}
		}
		else if(idSegment!=-1)
		{
			//scatter graph for given segment request
		}
		else
		{
			//poirier, august 1999, begin, memory leak
			if(pfArray1) delete[] pfArray1;
			if(pfArray2) delete[] pfArray2;
			//poirier, august 1999, end
			return FALSE;
		}
		ASSERT(ii==iNumberOfPoint-iReject-1);

		//set graph type
		m_pScatterGraphDoc->SetGraphType(GRAPH_SCATTER);
		//2) add the scatter graph vector to the graph document
		m_pScatterGraphDoc->AddGraphSet(pfArray1, iNumberOfPoint-iReject, pszStatChannelNames[idchannelStat1]);
		m_pScatterGraphDoc->AddGraphSet(pfArray2, iNumberOfPoint-iReject, pszStatChannelNames[idchannelStat2]);

		if(pfArray1) delete[] pfArray1;
		if(pfArray2) delete[] pfArray2;
	}
	return TRUE;
}


