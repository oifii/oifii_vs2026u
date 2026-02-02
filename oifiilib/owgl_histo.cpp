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

#include "resource.h"

#include "c_pointset.h"
#include "c_segmentset.h"

#include "OWpointset.h"

#include "OW2mdi.h"
#include "OWdoc.h"
#include "OW2doc.h"

#include "OWview.h"
#include "OW2view.h"

#include "gl_owgraph.h"
#include "owgl_histovw.h"
#include "owgl_histo.h"

#include <gl\gl.h>
#include <gl\glu.h>
#include "oifii_propsettings.h"
#include "oifii_proppage.h"

#include "OWpixelset.h"
#include "OWedgeset.h"

#include "logdoc.h"
#include "oifii_app.h"

#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


  
IMPLEMENT_DYNCREATE(CHistogram, CObject)


CHistogram::CHistogram()
{ 
	//do not use this constructor, for the moment (therefore no dynamic creation allowed)
}

CHistogram::~CHistogram()
{   
	//m_pHistoView and m_pHistoDoc are automaticaly deleted by the parent frame wnd
}


///////////////////////
//to create a histogram
///////////////////////
CHistogram::CHistogram(	CRect rectHistoView, 
						COW2Doc* pOW2Doc, 
						CWnd* pParentWnd,
						int iPointType,
						int iPointCollectionType,
						int idROI,
						int idVertex,
						int iNumNeighbor,
						int idSegment, 
						int idstatchannel,
						int iNumberOfLevel)
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

	CreateContext(rectHistoView, pOW2Doc, pParentWnd);
	//MapStatChannelIntoHisto(pOW2Doc, idstatchannel, iNumberOfLevel);
	m_pHistoDoc->SetTitle("Histogram");
	m_pHistoDoc->SetViewGrid(GRID_HORZ);
	m_pHistoDoc->SetXAxisLabel("DN Level");
	m_pHistoDoc->SetYAxisLabel("Count");
	//Update(pOW2Doc, idstatchannel, iNumberOfLevel);
	Update(	pOW2Doc,
			iPointType,
			iPointCollectionType,
			idROI,
			idVertex, 
			iNumNeighbor,
			idSegment, 
			idstatchannel, 
			iNumberOfLevel);

}


/////////////////////
//to update histogram
/////////////////////
BOOL CHistogram::Update(COW2Doc* pDoc, 
						int iPointType,
						int iPointCollectionType,
						int idROI,
						int idVertex, 
						int iNumNeighbor,
						int idSegment, 
						int idchannelStat, 
						int iNumberOfLevel)
{
	//on update, store previous graphset visibility attributes
	if(m_pHistoDoc->GetNumberOfSets()>0)
	{
		m_pHistoDoc->GetVisibilityOfAllGraphSets(m_pBOOL, &m_nBOOL);
	}

	m_pHistoDoc->RemoveAllGraphSet();
	//histogram
	if(pDoc==NULL) return FALSE;
	//MapStatChannelIntoHisto(pDoc, idchannelStat, iNumberOfLevel);
	MapStatChannelIntoHisto(pDoc, 
							iPointType,
							iPointCollectionType,
							idROI,
							idVertex, 
							iNumNeighbor, 
							idSegment, 
							idchannelStat, 
							iNumberOfLevel); //used with global histo only

	return TRUE;
}




void CHistogram::CreateContext(CRect rectHistoView, COW2Doc* pOW2Doc, CWnd* pParentWnd)
{
	CRuntimeClass* pHistoDocClass = RUNTIME_CLASS(COWGraphDoc);
	m_pHistoDoc = (COWGraphDoc*) (pHistoDocClass->CreateObject());
    ASSERT(m_pHistoDoc);
    
	CRuntimeClass* pHistoViewClass = RUNTIME_CLASS(CHistogramView);
	m_pHistoView = (CHistogramView*) pHistoViewClass->CreateObject();
    ASSERT(m_pHistoView);
          
	CCreateContext context;
	context.m_pNewViewClass		= pHistoViewClass;
	context.m_pCurrentDoc		= m_pHistoDoc;
	context.m_pNewDocTemplate	= NULL;
	context.m_pLastView			= NULL;
	if(pParentWnd->IsKindOf(RUNTIME_CLASS(CPropertySheet))) context.m_pCurrentFrame	= NULL;
	  else if(pParentWnd->IsKindOf(RUNTIME_CLASS(CView))) context.m_pCurrentFrame = pParentWnd->GetParentFrame();
	  else if(pParentWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd))) context.m_pCurrentFrame	= (CFrameWnd*) pParentWnd;

    if (!m_pHistoView->Create(NULL, NULL, WS_CHILD|(TRUE ? WS_VISIBLE:0),rectHistoView,
		pParentWnd, AFX_IDW_PANE_FIRST+1, &context))
	{
	    	TRACE0("Warning: couldn't create view for frame\n");
	    	return;
	}
	// commented out since integration into CPropertySheet in OIII
	//pParentWnd->GetParentFrame()->SetActiveView(m_pHistoView);

	// get data from ow2doc
    // get histogram from specified image
	// TODO: add your code in here if you want this class to automatically
	//       get the data from the image document
	return;
}

//iNumberOfLevel would be 256 for a gray image histogram (0-255)
BOOL CHistogram::MapStatChannelIntoHisto(	COW2Doc* pOW2Doc, 
											int iPointType,
											int iPointCollectionType,
											int idROI,
											int idVertex, 
											int iNumNeighbor, 
											int idSegment, 
											int iStatChannel, 
											int iNumberOfLevel) //used with global histo only
{
	BOOL bResult = FALSE;

	if(pOW2Doc==NULL) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	ASSERT(pOW2Doc->IsKindOf(RUNTIME_CLASS(COW2Doc)));
	CHistoPage* pHistoPage = (CHistoPage*) m_pHistoView->GetParent();
	ASSERT(pHistoPage->IsKindOf(RUNTIME_CLASS(CHistoPage)));



	if(idVertex==-1 && iNumNeighbor==-1 && idSegment==-1 && idROI==-1)
	{
		idROI = EDGESET_LARGEST_WORLDRECT_ROI_ID;
	}

	////////////////////////////
	//histogragram from pixelset
	////////////////////////////
	if(iPointType==HISTO_IMAGEPIXELS && idROI!=-1)
	{
		bResult = TRUE;

		//set graph type
		m_pHistoDoc->SetGraphType(GRAPH_VBAR);

		//get roi bounding rect
		CRect myRect = pOW2Doc->m_pROIset->GetRectROI(idROI);

		//get roi info
		POLYLINE* pPOLYLINE = pOW2Doc->m_pROIset->GetInfoROI(idROI);

		if(iPointCollectionType==HISTO_BR_INTERIOR)
		{
			//bounding rect interior
			ASSERT(pOW2Doc->m_pPixelset->IsBIIPresent());
			int nchannels = pOW2Doc->m_pPixelset->GetPixelsetNumberOfChannel();

			const char* pPSZ[] = {"Red", "Green", "Blue", "Gray"};
			ASSERT(nchannels<=4); //otherwise revise the line above, for the least!
			
			int nlevels = 256;
			int* pINT = (int*) new int[nlevels];
			int j_start = 0;
			int j_end = nchannels;
			if(iStatChannel>=j_start && iStatChannel<nchannels)
			{
				//add one channel, the selected one
				pOW2Doc->m_pPixelset->GetHistogramChannel(iStatChannel, pINT, &myRect);//iStatChannel, pnHisto)); //iStatChannel used as channel of the image
				//add the histogram vector to the graph document
				m_pHistoDoc->AddGraphSet(pINT, nlevels, "gray");
			}
			else
			{
				for(int j=0; j<j_end; j++)
				{
					//add one channel, the selected one
					pOW2Doc->m_pPixelset->GetHistogramChannel(j, pINT, &myRect);//iStatChannel, pnHisto)); //iStatChannel used as channel of the image
					//add the histogram vector to the graph document
					m_pHistoDoc->AddGraphSet(pINT, nlevels, pPSZ[j]);
				}
			}
			delete[] pINT;
			m_pHistoDoc->MapIndexToFloatValuesOnDisplay(FALSE);
			m_pHistoDoc->SetVisibilityOfAllGraphSets(m_pBOOL, m_nBOOL);
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

			//get histogram
			ASSERT(pOW2Doc->m_pPixelset->IsBIIPresent());
			int nchannels = pOW2Doc->m_pPixelset->GetPixelsetNumberOfChannel();

			const char* pPSZ[] = {"Red", "Green", "Blue", "Gray"};
			ASSERT(nchannels<=4); //otherwise revise the line above, for the least!
			
			int nlevels = 256;
			int* pINT = (int*) new int[nlevels];
			int j_start = 0;
			int j_end = nchannels;
			if(iStatChannel>=j_start && iStatChannel<nchannels)
			{
				//add one channel, the selected one
				pOW2Doc->m_pPixelset->GetHistogramChannel(iStatChannel, pINT, pPOINT, npoints, nlevels);//iStatChannel, pnHisto)); //iStatChannel used as channel of the image
				//add the histogram vector to the graph document
				m_pHistoDoc->AddGraphSet(pINT, nlevels, "gray");
			}
			else
			{
				for(int j=0; j<j_end; j++)
				{
					//add one channel, the selected one
					pOW2Doc->m_pPixelset->GetHistogramChannel(j, pINT, pPOINT, npoints, nlevels);//iStatChannel, pnHisto)); //iStatChannel used as channel of the image
					//add the histogram vector to the graph document
					m_pHistoDoc->AddGraphSet(pINT, nlevels, pPSZ[j]);
				}
			}
			delete[] pINT;
			m_pHistoDoc->MapIndexToFloatValuesOnDisplay(FALSE);
			m_pHistoDoc->SetVisibilityOfAllGraphSets(m_pBOOL, m_nBOOL);

			if(pEdgePOINT) delete[] pEdgePOINT;
			if(pInnerEdgePOINT) delete[] pInnerEdgePOINT;
			if(pOutterEdgePOINT) delete[] pOutterEdgePOINT;
			if(pInnerEBRPOINT) delete[] pInnerEBRPOINT;
			if(pOutterEBRPOINT) delete[] pOutterEBRPOINT;

			if(pTempPOINT) delete[] pTempPOINT;

		}
	}



	////////////////////////////
	//histogragram from pointset
	////////////////////////////
	if(pHistoPage->m_pHistoSettings->bImageFeatures==TRUE)  
	{
		if(pOW2Doc->m_pPointset==NULL)
		{
			//request for image feature histo BUT NO STATISTICS PRESENT, switch to pixel histo
			ASSERT(FALSE);
			return FALSE; 
		}

		if(idVertex==-1 && iNumNeighbor==-1 && idSegment==-1 
			&& idROI!=-1 && idROI!=EDGESET_LARGEST_WORLDRECT_ROI_ID)
		{
			COW2View* pOW2View = NULL;
			POSITION pos = pOW2Doc->GetFirstViewPosition(); 
			while(pos)
			{
				CView* pView = pOW2Doc->GetNextView(pos); 
				if(pView->IsKindOf(RUNTIME_CLASS(COW2View)))
				{
					pOW2View = (COW2View*)pView;
					break;
				}
			}

			RECT myRECT = pOW2Doc->m_pROIset->GetRectROI(idROI);
			RECT2DD myROIRECT = pOW2View->MapRectFromViewToWorld(myRECT);	
			pOW2Doc->m_pPointset->NormalizeRect(&myROIRECT);

			RECT2DD myWorldRect = pOW2Doc->m_pPointset->GetWorldRect();
			if(pOW2Doc->m_pPointset->PointInRect(myWorldRect.left, myWorldRect.top, &myROIRECT)
				&& pOW2Doc->m_pPointset->PointInRect(myWorldRect.right, myWorldRect.top, &myROIRECT)
				&& pOW2Doc->m_pPointset->PointInRect(myWorldRect.right, myWorldRect.bottom, &myROIRECT)
				&& pOW2Doc->m_pPointset->PointInRect(myWorldRect.left, myWorldRect.bottom, &myROIRECT))
			{
				//specified rect is fully embedded within pointset world rect
				idROI=EDGESET_LARGEST_WORLDRECT_ROI_ID;
			}
		}
 		if(idVertex==-1 && iNumNeighbor==-1 && idSegment==-1 
			&& (idROI==-1 || idROI==EDGESET_LARGEST_WORLDRECT_ROI_ID) )
		{
			//////////////////////////
			//global histogram request

			//get statistics
			double* pdfBaseStatistics = pOW2Doc->GetPointerToStatistics();
			if(pdfBaseStatistics==NULL)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			//compute scale factor used when mapping to histogram
			int iNumberOfPoint = pOW2Doc->GetNumberOfPoint();
			int iNumberOfStatPerPoint = pOW2Doc->GetNumberOfStatPerPoint();
			double dfMin, dfMax;
			pOW2Doc->GetMinMaxForStatChannel(iStatChannel, dfMin, dfMax);
			double dfNumberOfLevel = (double) iNumberOfLevel;
			double dfScaleFactor = (dfNumberOfLevel-1.0)/(dfMax - dfMin);
			//allocate histogram vector
			int* plArray = (int*) new int[iNumberOfLevel];
			if(plArray==NULL) 
			{
				ASSERT(FALSE);
				return FALSE;
			}
			//reset histogram vector
			for(int i=0; i<iNumberOfLevel; i++) 
			{
				plArray[i] = 0;
			}
			//compute the distribution of the statistic channel
			for(int i=0; i<iNumberOfPoint; i++)
			{
				//get value
				int iStatOffset = i*iNumberOfStatPerPoint;
				double dfValue = pdfBaseStatistics[iStatOffset+iStatChannel];
				if(	!((iStatChannel==POINTSET_OFFSETSTAT_VORODENSITY || 
					iStatChannel==POINTSET_OFFSETSTAT_VOROAREA  ||
					iStatChannel==POINTSET_OFFSETSTAT_NEIGHAVERAGEVOROAREA ||
					iStatChannel==POINTSET_OFFSETSTAT_NEIGHVARIANCEVOROAREA) &&
					dfValue<0.0))
				{
					//map it, that is find the corresponding index
					int index = (int) ((dfValue-dfMin)*dfScaleFactor + 0.5); //+0.5 for round off
					//increment count
					plArray[index]++;
				}
			}

			//set graph type
			m_pHistoDoc->SetGraphType(GRAPH_VBAR);	//GRAPH_LINE	 //GRAPH_VBAR
			//2) add the histogram vector to the graph document
			m_pHistoDoc->AddGraphSet((int*)plArray, iNumberOfLevel, pszStatChannelNames[iStatChannel]);
			//3) tell graph lib to display float values instead of index
			m_pHistoDoc->MapIndexToFloatValuesOnDisplay(dfMin, dfMax, iNumberOfLevel);

			if(plArray) delete[] plArray;

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

			if(iPointCollectionType==HISTO_BR_INTERIOR)
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
			else if(iPointCollectionType==HISTO_SBR_INTERIOR
				|| iPointCollectionType==HISTO_SBR_INNER_EDGE_HALF
				|| iPointCollectionType==HISTO_SBR_OUTTER_EDGE_HALF
				|| iPointCollectionType==HISTO_EDGE_ALONG
				|| iPointCollectionType==HISTO_EDGE_INNER
				|| iPointCollectionType==HISTO_EDGE_OUTTER
				|| iPointCollectionType==HISTO_EDGE_AIO)
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

				if(iPointCollectionType==HISTO_SBR_INTERIOR)
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
				else if(iPointCollectionType==HISTO_SBR_INNER_EDGE_HALF)
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
				else if(iPointCollectionType==HISTO_SBR_OUTTER_EDGE_HALF)
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
				else if(iPointCollectionType==HISTO_EDGE_ALONG)
				{
				}	
				else if(iPointCollectionType==HISTO_EDGE_INNER)
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
				else if(iPointCollectionType==HISTO_EDGE_OUTTER)
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
				else if(iPointCollectionType==HISTO_EDGE_AIO)
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


				if(iPointCollectionType==HISTO_SBR_INTERIOR)
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
				else if(iPointCollectionType==HISTO_SBR_INNER_EDGE_HALF)
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
				else if(iPointCollectionType==HISTO_SBR_OUTTER_EDGE_HALF)
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
				else if(iPointCollectionType==HISTO_EDGE_ALONG)
				{
					p_npoints[0] = myEDGEFILLPARAM.p_nvertexfound[0]; //along edge
					p_npoints[1] = 0; 
					p_npoints[2] = 0; 
					npoints = p_npoints[0];
					pIDVERTEX[0] = myEDGEFILLPARAM.pIDVERTEX;
					pIDVERTEX[1] = NULL;
					pIDVERTEX[2] = NULL;
				}	
				else if(iPointCollectionType==HISTO_EDGE_INNER)
				{
					p_npoints[0] = myEDGEFILLPARAM.p_nvertexfound_ie[0]; //inner edge
					p_npoints[1] = 0; 
					p_npoints[2] = 0; 
					npoints = p_npoints[0];
					pIDVERTEX[0] = myEDGEFILLPARAM.pIDVERTEX_ie;
					pIDVERTEX[1] = NULL;
					pIDVERTEX[2] = NULL;
				}				
				else if(iPointCollectionType==HISTO_EDGE_OUTTER)
				{
					p_npoints[0] = myEDGEFILLPARAM.p_nvertexfound_oe[0]; //outter edge
					p_npoints[1] = 0; 
					p_npoints[2] = 0; 
					npoints = p_npoints[0];
					pIDVERTEX[0] = myEDGEFILLPARAM.pIDVERTEX_oe;
					pIDVERTEX[1] = NULL;
					pIDVERTEX[2] = NULL;
				}				
				else if(iPointCollectionType==HISTO_EDGE_AIO)
				{
					p_npoints[0] = myEDGEFILLPARAM.p_nvertexfound[0]; //edge
					p_npoints[1] = myEDGEFILLPARAM.p_nvertexfound_ie[0]; //inner edge 
					p_npoints[2] = myEDGEFILLPARAM.p_nvertexfound_oe[0]; //outter edge 
					npoints = p_npoints[0] + p_npoints[1] + p_npoints[2];
					pIDVERTEX[0] = myEDGEFILLPARAM.pIDVERTEX;
					pIDVERTEX[1] = myEDGEFILLPARAM.pIDVERTEX_ie;
					pIDVERTEX[2] = myEDGEFILLPARAM.pIDVERTEX_oe;
				}
				else
				{
					ASSERT(FALSE);
					p_npoints[0] = 0; 
					p_npoints[1] = 0; 
					p_npoints[2] = 0; 
					npoints = 0;
					pIDVERTEX[0] = NULL;
					pIDVERTEX[1] = NULL;
					pIDVERTEX[2] = NULL;
				}
			}
			ASSERT(pOW2Doc->m_pPointset->m_pPOINTSET->pfStatistics);
			int nchannels = pOW2Doc->m_pPointset->GetNumberOfStatPerPoint();

			ASSERT(nchannels<=(4+9)); //otherwise revise the line above, for the least!
			
			double* pDOUBLE = NULL;
			if(npoints) pDOUBLE = (double*) new double[npoints];

			int j_start = 0;
			int j_end = nchannels;
			if(iStatChannel>=j_start && iStatChannel<nchannels)
			{
				//add one channel, the selected one
				//get vertex statistic
				int index = 0;
				int k=0; while(k<3 && p_npoints[k]>0)
				{
					for(int i=0; i<p_npoints[k]; i++)
					{
						pDOUBLE[index] = pOW2Doc->m_pPointset->GetStatisticsValue(pIDVERTEX[k][i], iStatChannel);
						index++;
					}
					//index = index + p_npoints[k];
					k++;
				}
				//map to histogram
				const int inumhistolevel = 256;
				int p_iHistogram[inumhistolevel];

				double fMin, fMax;
				pOW2Doc->MapStatisticsVectorIntoHistogram(	pDOUBLE,
															npoints,
															p_iHistogram,
															inumhistolevel,
															&fMin,
															&fMax);


				//set graph type
				m_pHistoDoc->SetGraphType(GRAPH_VBAR);
				//2) add the histogram vector to the graph document
				m_pHistoDoc->AddGraphSet((int*)p_iHistogram, inumhistolevel, pszStatChannelNames[iStatChannel]);
				//3) tell graph lib to display float values instead of index
				m_pHistoDoc->MapIndexToFloatValuesOnDisplay(fMin, fMax, inumhistolevel);

			}
			else
			{
				//add all channels
				for(int j=0; j<nchannels; j++)
				{
					int index = 0;
					int k=0; while(k<3 && p_npoints[k]>0)
					{
						for(int i=0; i<p_npoints[k]; i++)
						{
							pDOUBLE[index] = pOW2Doc->m_pPointset->GetStatisticsValue(pIDVERTEX[k][i], iStatChannel);
							index++;
						}
						//index = index + p_npoints[k];
						k++;
					}
					//map to histogram
					const int inumhistolevel = 256;
					int p_iHistogram[inumhistolevel];

					double fMin, fMax;
					pOW2Doc->MapStatisticsVectorIntoHistogram(	pDOUBLE,
																npoints,
																p_iHistogram,
																inumhistolevel,
																&fMin,
																&fMax);


					//set graph type
					m_pHistoDoc->SetGraphType(GRAPH_VBAR);
					//2) add the histogram vector to the graph document
					m_pHistoDoc->AddGraphSet((int*)p_iHistogram, inumhistolevel, pszStatChannelNames[iStatChannel]);
					//3) tell graph lib to display float values instead of index
					m_pHistoDoc->MapIndexToFloatValuesOnDisplay(fMin, fMax, inumhistolevel);

				}
			}
			m_pHistoDoc->SetVisibilityOfAllGraphSets(m_pBOOL, m_nBOOL);
			if(pDOUBLE) delete[] pDOUBLE;


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

			bResult = TRUE;
		}
		else if(idVertex!=-1 && iNumNeighbor>-1 && idSegment==-1)
		{
			/////////////////////////
			//local histogram request

			//get circular neighborhood profile
 			int ifinalnumfound;
			int p_ifinalarrayneighbor[200];
			double pfStatistics[200];
			BOOL bResult = pOW2Doc->GetCircularNeighborhoodProfileAroundVertex(	idVertex, 
																				iNumNeighbor,
																				iStatChannel,
																				&ifinalnumfound,																			p_ifinalarrayneighbor,
																				pfStatistics);
			if(bResult==FALSE || ifinalnumfound==0)
			{
				return FALSE;
			}

			//map to histogram
			int inumhistolevel = 4;
			int p_iHistogram[4];

			double fMin, fMax;
			pOW2Doc->MapStatisticsVectorIntoHistogram(	pfStatistics,
														ifinalnumfound,
														p_iHistogram,
														inumhistolevel,
														&fMin,
														&fMax);


			//set graph type
			m_pHistoDoc->SetGraphType(GRAPH_VBAR);
			//2) add the histogram vector to the graph document
			m_pHistoDoc->AddGraphSet((int*)p_iHistogram, inumhistolevel, pszStatChannelNames[iStatChannel]);
			//3) tell graph lib to display float values instead of index
			m_pHistoDoc->MapIndexToFloatValuesOnDisplay(fMin, fMax, inumhistolevel);

		}
		else if(idSegment!=-1)
		{
			//histogram for given segment request
		}
	}
	return TRUE;
}

