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

#include "OWpixelset.h"
#include "OWpointset.h"
#include "OWedgeset.h"

#include "OW2mdi.h"
#include "OWdoc.h"
#include "OW2doc.h"

#include "OWview.h"
#include "OW2view.h"

#include "gl_owgraph.h"
#include "owgl_profilevw.h"
#include "owgl_profile.h"



#include <gl\gl.h>
#include <gl\glu.h>
#include "resource.h"
#include "oifii_propsettings.h"
#include "oifii_proppage.h"

#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


  
IMPLEMENT_DYNCREATE(CProfileGraph, CObject)


CProfileGraph::CProfileGraph()
{ 
	//do not use this constructor, for the moment (therefore no dynamic creation allowed)
}

CProfileGraph::~CProfileGraph()
{   
	//m_pProfileGraphView and m_pProfileGraphDoc are automaticaly deleted by the parent frame wnd
}



///////////////////////////
//to create a profile graph
///////////////////////////
CProfileGraph::CProfileGraph(	CRect rectProfileGraphView, 
								COW2Doc* pOW2Doc, 
								CWnd* pParentWnd,
								int iPointType,
								int iPointCollectionType,
								int idROI,
								int idVertex,
								int iNumNeighbor,
								int idSegment,
								int iStatChannel)
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

	CreateContext(rectProfileGraphView, pOW2Doc, pParentWnd);
	m_pProfileGraphDoc->SetTitle("Profile");
	m_pProfileGraphDoc->SetViewGrid(GRID_HORZ |GRID_VERT);
	m_pProfileGraphDoc->SetXAxisLabel("X");
	m_pProfileGraphDoc->SetYAxisLabel("Y");
	Update(	pOW2Doc, 
			iPointType,
			iPointCollectionType,
			idROI,
			idVertex, 
			iNumNeighbor,
			idSegment, 
			iStatChannel);
}


///////////////////
//to update profile
///////////////////
BOOL CProfileGraph::Update(	COW2Doc* pDoc, 
							int iPointType,
							int iPointCollectionType,
							int idROI,
							int idVertex, 
							int iNumNeighbor,
							int idSegment, 
							int iStatChannel)
{
	//on update, store previous graphset visibility attributes
	if(m_pProfileGraphDoc->GetNumberOfSets()>0)
	{
		m_pProfileGraphDoc->GetVisibilityOfAllGraphSets(m_pBOOL, &m_nBOOL);
	}

	m_pProfileGraphDoc->RemoveAllGraphSet();
	//profile
	if(pDoc==NULL) return FALSE;
	MapIntoProfile(	pDoc, 
					iPointType,
					iPointCollectionType,
					idROI,
					idVertex, 
					iNumNeighbor, 
					idSegment, 
					iStatChannel);
	return TRUE;
}



void CProfileGraph::CreateContext(CRect rectProfileGraphView, COW2Doc* pOW2Doc, CWnd* pParentWnd)
{
	CRuntimeClass* pProfileGraphDocClass = RUNTIME_CLASS(COWGraphDoc);
	m_pProfileGraphDoc = (COWGraphDoc*) (pProfileGraphDocClass->CreateObject());
    ASSERT(m_pProfileGraphDoc);
    
	CRuntimeClass* pProfileGraphViewClass = RUNTIME_CLASS(CProfileGraphView);
	m_pProfileGraphView = (CProfileGraphView*) pProfileGraphViewClass->CreateObject();
    ASSERT(m_pProfileGraphView);
          
	CCreateContext context;
	context.m_pNewViewClass		= pProfileGraphViewClass;
	context.m_pCurrentDoc		= m_pProfileGraphDoc;
	context.m_pNewDocTemplate	= NULL;
	context.m_pLastView			= NULL;
	if(pParentWnd->IsKindOf(RUNTIME_CLASS(CPropertySheet))) context.m_pCurrentFrame	= NULL;
	  else if(pParentWnd->IsKindOf(RUNTIME_CLASS(CView))) context.m_pCurrentFrame = pParentWnd->GetParentFrame();
	  else if(pParentWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd))) context.m_pCurrentFrame	= (CFrameWnd*) pParentWnd;

    if (!m_pProfileGraphView->Create(NULL, NULL, WS_CHILD|(TRUE ? WS_VISIBLE:0),rectProfileGraphView,
		pParentWnd, AFX_IDW_PANE_FIRST+1, &context))
	{
	    	TRACE0("Warning: couldn't create view for frame\n");
	    	return;
	}
	// commented out since integration into CPropertySheet in OIII
	//pParentWnd->GetParentFrame()->SetActiveView(m_pProfileGraphView);

	// get data from ow2doc
    // get ProfileGraph from specified image
	// TODO: add your code in here if you want this class to automatically
	//       get the data from the image document
	return;
}


BOOL CProfileGraph::MapIntoProfile(	COW2Doc* pOW2Doc, 
									int iPointType,
									int iPointCollectionType,
									int idROI,
									int idVertex, 
									int iNumNeighbor, 
									int idSegment, 
									int iStatChannel)
{
	BOOL bResult = FALSE;
	if(pOW2Doc==NULL) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	ASSERT(pOW2Doc->IsKindOf(RUNTIME_CLASS(COW2Doc)));

	if(idVertex==-1 && iNumNeighbor==-1 && idSegment==-1 && idROI==-1)
	{
		idROI = EDGESET_LARGEST_WORLDRECT_ROI_ID;
	}

	///////////////////////
	//profile from pixelset
	///////////////////////
	if(iPointType==PROFILE_IMAGEPIXELS && idROI!=-1)
	{
		bResult = TRUE;

		//set graph type
		m_pProfileGraphDoc->SetGraphType(GRAPH_LINE);
		//get roi bounding rect
		CRect myRect = pOW2Doc->m_pROIset->GetRectROI(idROI);

		if(iPointCollectionType==PROFILE_BR_XCENTER
			|| iPointCollectionType==PROFILE_BR_YCENTER)
		{
			POINT P1;
			POINT P2;
			if(iPointCollectionType==PROFILE_BR_XCENTER)
			{
				//bounding rect center x profile
				P1.x = myRect.left;
				P1.y = (myRect.top+myRect.bottom)/2;
				P2.x = myRect.right;
				P2.y = (myRect.top+myRect.bottom)/2;
			}
			else if(iPointCollectionType==PROFILE_BR_YCENTER)
			{
				//bounding rect center y profile
				P1.x = (myRect.left+myRect.right)/2;
				P1.y = myRect.top;
				P2.x = (myRect.left+myRect.right)/2;
				P2.y = myRect.bottom;
			}


			int npoint_max = 2 * ((abs(P2.x - P1.x) + abs(P2.y - P1.y))); //estimate of max possible number 
			POINT* pPOINT = (POINT*) new POINT[npoint_max];
			int npoints = pOW2Doc->m_pROIset->EdgeFill(	P1, 
														P2, 
														pPOINT, //allocated by caller to collect all intermediate points between P1 and P2 (given the grid step)
														npoint_max); //maximum number of POINT structure allocated by caller
			ASSERT(pOW2Doc->m_pPixelset->IsBIIPresent());
			int nchannels = pOW2Doc->m_pPixelset->GetPixelsetNumberOfChannel();

			const char* pPSZ[] = {"Red", "Green", "Blue", "Gray"};
			ASSERT(nchannels<=4); //otherwise revise the line above, for the least!
			
			int* pINT = (int*) new int[npoints];
			int j_start = 0;
			int j_end = nchannels;
			if(iStatChannel>=j_start && iStatChannel<nchannels)
			{
				//add one channel, the selected one
				//get pixel intensity
				for(int i=0; i<npoints; i++)
				{
					pINT[i] = pOW2Doc->m_pPixelset->GetPixelValue(pPOINT[i].x, pPOINT[i].y, iStatChannel);
				}
				//add the profile graph vector to the graph document
				m_pProfileGraphDoc->AddGraphSet(pINT, npoints, "Gray");
			}
			else
			{
				//add all channels
				for(int j=0; j<nchannels; j++)
				{
					//get pixel intensity
					for(int i=0; i<npoints; i++)
					{
						pINT[i] = pOW2Doc->m_pPixelset->GetPixelValue(pPOINT[i].x, pPOINT[i].y, j);
					}
					//add the profile graph vector to the graph document
					m_pProfileGraphDoc->AddGraphSet(pINT, npoints, pPSZ[j]);
				}
			}
			m_pProfileGraphDoc->MapIndexToFloatValuesOnDisplay(FALSE);
			m_pProfileGraphDoc->SetVisibilityOfAllGraphSets(m_pBOOL, m_nBOOL);
			delete[] pINT;
			delete[] pPOINT;
		}
		else if(iPointCollectionType==PROFILE_BR_PERIMETER
			|| iPointCollectionType==PROFILE_EDGE_ALONG
			|| iPointCollectionType==PROFILE_EDGE_INNER
			|| iPointCollectionType==PROFILE_EDGE_OUTTER)
		{
			if(iPointCollectionType==PROFILE_BR_PERIMETER) 
			{
				//bounding rect perimeter profile
				int npoint_max = 2 * (abs(myRect.right - myRect.left) + abs(myRect.top - myRect.top)); //estimate of max possible number 
				npoint_max = npoint_max + 2 * (abs(myRect.right - myRect.right) + abs(myRect.bottom - myRect.top)); //estimate of max possible number 
				npoint_max = npoint_max + 2 * (abs(myRect.left - myRect.right) + abs(myRect.bottom - myRect.bottom)); //estimate of max possible number 
				npoint_max = npoint_max + 2 * (abs(myRect.left - myRect.left) + abs(myRect.top - myRect.bottom)); //estimate of max possible number 
				POINT* pPOINT = (POINT*) new POINT[npoint_max];
				int npoints = pOW2Doc->m_pROIset->RectEdgeFill(	myRect, 
																pPOINT, //allocated by caller to collect all intermediate points between P1 and P2 (given the grid step)
																npoint_max); //maximum number of POINT structure allocated by caller

				ASSERT(pOW2Doc->m_pPixelset->IsBIIPresent());
				int nchannels = pOW2Doc->m_pPixelset->GetPixelsetNumberOfChannel();

				const char* pPSZ[] = {"Red", "Green", "Blue", "Gray"};
				ASSERT(nchannels<=4); //otherwise revise the line above, for the least!
				
				int* pINT = (int*) new int[npoints];
				int j_start = 0;
				int j_end = nchannels;
				if(iStatChannel>=j_start && iStatChannel<nchannels)
				{
					//add one channel, the selected one
					//get pixel intensity
					for(int i=0; i<npoints; i++)
					{
						pINT[i] = pOW2Doc->m_pPixelset->GetPixelValue(pPOINT[i].x, pPOINT[i].y, iStatChannel);
					}
					//add the profile graph vector to the graph document
					m_pProfileGraphDoc->AddGraphSet(pINT, npoints, "Gray");
				}
				else
				{
					//add all channels
					for(int j=0; j<nchannels; j++)
					{
						//get pixel intensity
						for(int i=0; i<npoints; i++)
						{
							pINT[i] = pOW2Doc->m_pPixelset->GetPixelValue(pPOINT[i].x, pPOINT[i].y, j);
						}
						//add the profile graph vector to the graph document
						m_pProfileGraphDoc->AddGraphSet(pINT, npoints, pPSZ[j]);
					}
				}
				m_pProfileGraphDoc->MapIndexToFloatValuesOnDisplay(FALSE);
				m_pProfileGraphDoc->SetVisibilityOfAllGraphSets(m_pBOOL, m_nBOOL);
				delete[] pINT;
				delete[] pPOINT;
			}
			else if(iPointCollectionType==PROFILE_EDGE_ALONG
				|| iPointCollectionType==PROFILE_EDGE_INNER
				|| iPointCollectionType==PROFILE_EDGE_OUTTER)
			{

				POLYLINE* pPOLYLINE = pOW2Doc->m_pROIset->GetInfoROI(idROI);

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

				if(iPointCollectionType==PROFILE_EDGE_ALONG) 
				{
					pInnerEdgePOINT = NULL;
					pOutterEdgePOINT = NULL;
					npoint_ioe_max = 0;

					pInnerEBRPOINT = NULL;
					pOutterEBRPOINT = NULL;
					npoint_iebr_max = 0;
					npoint_oebr_max = 0;

				}
				else if(iPointCollectionType==PROFILE_EDGE_INNER) 
				{
					pInnerEdgePOINT = (POINT*) new POINT[npoint_ioe_max];
					pOutterEdgePOINT = NULL;

					pInnerEBRPOINT = NULL;
					pOutterEBRPOINT = NULL;
					npoint_iebr_max = 0;
					npoint_oebr_max = 0;
				}
				else if(iPointCollectionType==PROFILE_EDGE_OUTTER) 
				{
					pInnerEdgePOINT = NULL;
					pOutterEdgePOINT = (POINT*) new POINT[npoint_ioe_max];

					pInnerEBRPOINT = NULL;
					pOutterEBRPOINT = NULL;
					npoint_iebr_max = 0;
					npoint_oebr_max = 0;
				}

				int npoints = pOW2Doc->m_pROIset->PolylineFill(	pPOLYLINE, 							
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
				POINT* pPOINT = NULL;
				if(iPointCollectionType==PROFILE_EDGE_ALONG)
				{
					npoints = npoints;
					pPOINT = pEdgePOINT;
				}
				else if(iPointCollectionType==PROFILE_EDGE_INNER)
				{
					npoints = npoint_ioe_collected;
					pPOINT = pInnerEdgePOINT;
				}
				else if(iPointCollectionType==PROFILE_EDGE_OUTTER)
				{
					npoints = npoint_ioe_collected;
					pPOINT = pOutterEdgePOINT;
				}
				else
				{
					ASSERT(FALSE);
					return FALSE;
				}

				ASSERT(pOW2Doc->m_pPixelset->IsBIIPresent());
				int nchannels = pOW2Doc->m_pPixelset->GetPixelsetNumberOfChannel();

				const char* pPSZ[] = {"Red", "Green", "Blue", "Gray"};
				ASSERT(nchannels<=4); //otherwise revise the line above, for the least!
				
				int* pINT = (int*) new int[npoints];
				int j_start = 0;
				int j_end = nchannels;
				if(iStatChannel>=j_start && iStatChannel<nchannels)
				{
					//add one channel, the selected one
					//get pixel intensity
					for(int i=0; i<npoints; i++)
					{
						pINT[i] = pOW2Doc->m_pPixelset->GetPixelValue(pPOINT[i].x, pPOINT[i].y, iStatChannel);
					}
					//add the profile graph vector to the graph document
					m_pProfileGraphDoc->AddGraphSet(pINT, npoints, "Gray");
				}
				else
				{
					//add all channels
					for(int j=0; j<nchannels; j++)
					{
						//get pixel intensity
						for(int i=0; i<npoints; i++)
						{
							pINT[i] = pOW2Doc->m_pPixelset->GetPixelValue(pPOINT[i].x, pPOINT[i].y, j);
						}
						//add the profile graph vector to the graph document
						m_pProfileGraphDoc->AddGraphSet(pINT, npoints, pPSZ[j]);
					}
				}
				m_pProfileGraphDoc->MapIndexToFloatValuesOnDisplay(FALSE);
				m_pProfileGraphDoc->SetVisibilityOfAllGraphSets(m_pBOOL, m_nBOOL);
				delete[] pINT;


				if(pEdgePOINT) delete[] pEdgePOINT;
				if(pInnerEdgePOINT) delete[] pInnerEdgePOINT;
				if(pOutterEdgePOINT) delete[] pOutterEdgePOINT;
				if(pInnerEBRPOINT) delete[] pInnerEBRPOINT;
				if(pOutterEBRPOINT) delete[] pOutterEBRPOINT;

			}
		}
		else 
		{
			ASSERT(FALSE);
			return FALSE;
		}

	}
	///////////////////////
	//profile from pointset
	///////////////////////
	else if(iPointType==PROFILE_IMAGEFEATURES && pOW2Doc->m_pPointset!=NULL)
	{
		if(idVertex==-1 && iNumNeighbor==-1 && idSegment==-1 && idROI!=-1)
		{
			if(idROI==EDGESET_LARGEST_WORLDRECT_ROI_ID)
			{
				//global
			}
			if(1)
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

				//collect npoints (vertex) for profile graph
				int npoints = 0;
				IDVERTEX* pIDVERTEX = NULL;


				//prepare EDGEFILLPARAM  
				EDGEFILLPARAM myEDGEFILLPARAM;

				if(iPointCollectionType==PROFILE_BR_XCENTER
					|| iPointCollectionType==PROFILE_BR_YCENTER)
				{
					POINT myP1;
					POINT myP2;
					if(iPointCollectionType==PROFILE_BR_XCENTER)
					{
						//bounding rect center x profile
						myP1.x = myRect.left;
						myP1.y = (myRect.top+myRect.bottom)/2;
						myP2.x = myRect.right;
						myP2.y = (myRect.top+myRect.bottom)/2;
					}
					else if(iPointCollectionType==PROFILE_BR_YCENTER)
					{
						//bounding rect center y profile
						myP1.x = (myRect.left+myRect.right)/2;
						myP1.y = myRect.top;
						myP2.x = (myRect.left+myRect.right)/2;
						myP2.y = myRect.bottom;
					}
					
					//convert to OW2Doc coordinate system
					POINT2DD P1 = pOW2View->MapPointFromViewToWorld(myP1);
					POINT2DD P2 = pOW2View->MapPointFromViewToWorld(myP2);

					//edge
					int nvertex_max = (int) (fabs(P2.x-P1.x) + fabs(P2.y-P1.y));
					int nvertex_max2 = nvertex_max;
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
					/*
					myEDGEFILLPARAM.pIDVERTEX2 = (int*) new int[nvertex_max2];	
					myEDGEFILLPARAM.nvertex_max2 = nvertex_max2;	
					myEDGEFILLPARAM.p_nvertexfound2 = &nvertexfound2;
					*/

					int ntri_e = pOW2Doc->m_pPointset->EdgeFill(P1, P2, &myEDGEFILLPARAM);	//idvertex outter edge to bounding rect limits

					if(iPointCollectionType==PROFILE_BR_XCENTER)
					{
						npoints = myEDGEFILLPARAM.p_nvertexfound[0];
						pIDVERTEX = myEDGEFILLPARAM.pIDVERTEX;
					}
					else if(iPointCollectionType==PROFILE_BR_YCENTER)
					{
						npoints = myEDGEFILLPARAM.p_nvertexfound[0];
						pIDVERTEX = myEDGEFILLPARAM.pIDVERTEX;
					}

				}
				else if(iPointCollectionType==PROFILE_BR_PERIMETER
						|| iPointCollectionType==PROFILE_EDGE_ALONG
						|| iPointCollectionType==PROFILE_EDGE_INNER
						|| iPointCollectionType==PROFILE_EDGE_OUTTER)
				{
					POLYLINE* pPOLYLINE = pOW2Doc->m_pROIset->GetInfoROI(idROI);


					if(iPointCollectionType==PROFILE_BR_PERIMETER)
					{
						//bounding rect perimeter profile
						myRect = pPOLYLINE->BoundingRect;

						//convert to OW2Doc coordinate system
						POINT myP1;
						POINT myP2;
						myP1.x = myRect.left;
						myP1.y = myRect.top;
						myP2.x = myRect.right;
						myP2.y = myRect.bottom;
						POINT2DD P1 = pOW2View->MapPointFromViewToWorld(myP1);
						POINT2DD P2 = pOW2View->MapPointFromViewToWorld(myP2);
						RECT2DD myRECT2DD;
						myRECT2DD.left = P1.x;
						myRECT2DD.top = P1.y;
						myRECT2DD.right = P2.x;
						myRECT2DD.bottom = P2.y;
						//edge
						int nvertex_max = 2 * (abs(myRect.right - myRect.left) + abs(myRect.top - myRect.top)); //estimate of max possible number 
						nvertex_max = nvertex_max + 2 * (abs(myRect.right - myRect.right) + abs(myRect.bottom - myRect.top)); //estimate of max possible number 
						nvertex_max = nvertex_max + 2 * (abs(myRect.left - myRect.right) + abs(myRect.bottom - myRect.bottom)); //estimate of max possible number 
						nvertex_max = nvertex_max + 2 * (abs(myRect.left - myRect.left) + abs(myRect.top - myRect.bottom)); //estimate of max possible number 
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

						myEDGEFILLPARAM.pIDVERTEX2 = NULL;	
						myEDGEFILLPARAM.nvertex_max2 = 0;	
						myEDGEFILLPARAM.p_nvertexfound2 = NULL;
						
						npoints = pOW2Doc->m_pPointset->RectEdgeFill(	&myRECT2DD, 
																		&myEDGEFILLPARAM); 
					}
					else 
					{
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

						myEDGEFILLPARAM.pIDVERTEX2 = NULL;	
						myEDGEFILLPARAM.nvertex_max2 = 0;	
						myEDGEFILLPARAM.p_nvertexfound2 = NULL;
						if(iPointCollectionType==PROFILE_EDGE_ALONG)
						{
							//edge, all done here above
						}
						else if(iPointCollectionType==PROFILE_EDGE_INNER)
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
						else if(iPointCollectionType==PROFILE_EDGE_OUTTER)
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
						int ntri_e = pOW2Doc->m_pPointset->PolylineFill(pOW2Doc->m_pROIset,
																		pPOLYLINE, 
																		&myEDGEFILLPARAM,
																		pOW2View);
					}

					if(iPointCollectionType==PROFILE_BR_PERIMETER)
					{
						npoints = myEDGEFILLPARAM.p_nvertexfound[0];
						pIDVERTEX = myEDGEFILLPARAM.pIDVERTEX;
					}
					else if(iPointCollectionType==PROFILE_EDGE_ALONG)
					{
						npoints = myEDGEFILLPARAM.p_nvertexfound[0];
						pIDVERTEX = myEDGEFILLPARAM.pIDVERTEX;
					}
					else if(iPointCollectionType==PROFILE_EDGE_INNER)
					{
						npoints = myEDGEFILLPARAM.p_nvertexfound_ie[0];
						pIDVERTEX = myEDGEFILLPARAM.pIDVERTEX_ie;
					}
					else if(iPointCollectionType==PROFILE_EDGE_OUTTER)
					{
						npoints = myEDGEFILLPARAM.p_nvertexfound_oe[0];
						pIDVERTEX = myEDGEFILLPARAM.pIDVERTEX_oe;
					}
				}
				ASSERT(pOW2Doc->m_pPointset->m_pPOINTSET->pfStatistics);
				int nchannels = pOW2Doc->m_pPointset->GetNumberOfStatPerPoint();

				ASSERT(nchannels<=(4+9)); //otherwise revise the line above, for the least!
				
				double* pDOUBLE = (double*) new double[npoints];
				int j_start = 0;
				int j_end = nchannels;
				if(iStatChannel>=j_start && iStatChannel<nchannels)
				{
					//add one channel, the selected one
					//get vertex statistic
					for(int i=0; i<npoints; i++)
					{
						pDOUBLE[i] = pOW2Doc->m_pPointset->GetStatisticsValue(pIDVERTEX[i], iStatChannel);
					}
					//add the profile graph vector to the graph document
					m_pProfileGraphDoc->AddGraphSet(pDOUBLE, npoints, pszStatChannelNames[iStatChannel]);
				}
				else
				{
					//add all channels
					for(int j=0; j<nchannels; j++)
					{
						//get vertex statistic
						for(int i=0; i<npoints; i++)
						{
							pDOUBLE[i] = pOW2Doc->m_pPointset->GetStatisticsValue(pIDVERTEX[i], iStatChannel);
						}
						//add the profile graph vector to the graph document
						m_pProfileGraphDoc->AddGraphSet(pDOUBLE, npoints, pszStatChannelNames[iStatChannel]);

					}
				}
				m_pProfileGraphDoc->MapIndexToFloatValuesOnDisplay(FALSE);
				m_pProfileGraphDoc->SetVisibilityOfAllGraphSets(m_pBOOL, m_nBOOL);
				delete[] pDOUBLE;


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
		}
		else if(idVertex==-1 && iNumNeighbor==-1 && idSegment==-1 && pOW2Doc->m_pSegmentset!=NULL)
		{
			//global fusion sequence profile
			if(iStatChannel<0 || iStatChannel>FUSION_MAXNUMBEROFINFOCHANNEL)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			int imaxnumberofstat = pOW2Doc->m_pSegmentset->iInitialNumberOfSegment - 1;
			double* pfTemp = (double*) new double[imaxnumberofstat];
			if(pfTemp==NULL) 
			{
				ASSERT(FALSE);
				return FALSE;
			}
			int iNumberOfProfileStat;
			bResult = GetFusionSequenceProfile(	pOW2Doc->m_pSegmentset,
												iStatChannel,
												pfTemp,
												&iNumberOfProfileStat,
												imaxnumberofstat);
			if(bResult==FALSE || iNumberOfProfileStat==0)
			{
				return FALSE;
			}

			//set graph type
			m_pProfileGraphDoc->SetGraphType(GRAPH_LINE);
			//2) add the ProfileGraph vector to the graph document
			m_pProfileGraphDoc->AddGraphSet((double*)pfTemp, iNumberOfProfileStat, pszFusionInfoChannelNames[iStatChannel]);
			m_pProfileGraphDoc->MapIndexToFloatValuesOnDisplay(FALSE);
			if(pfTemp) delete[] pfTemp;
		}
		else if(idVertex!=-1 && iNumNeighbor>-1 && idSegment==-1)
		{
			//circular neighborhood profile
 			int ifinalnumfound;
			int p_ifinalarrayneighbor[200];
			double pfStatistics[200];
			BOOL bResult = pOW2Doc->GetCircularNeighborhoodProfileAroundVertex(	idVertex, 
																				iNumNeighbor,
																				iStatChannel,
																				&ifinalnumfound,
																				p_ifinalarrayneighbor,
																				pfStatistics); 

			//set graph type
			m_pProfileGraphDoc->SetGraphType(GRAPH_VBAR);
			//2) add the ProfileGraph vector to the graph document
			m_pProfileGraphDoc->AddGraphSet((double*)pfStatistics, ifinalnumfound, pszFusionInfoChannelNames[iStatChannel]);
			m_pProfileGraphDoc->MapIndexToFloatValuesOnDisplay(FALSE);
		}
		else if(idSegment!=-1)
		{
			//segment edge profile
		}

	}
	else
	{
		ASSERT(FALSE);
		return FALSE; //statistics would not be allocated yet
	}


	return bResult;
}


/*
	//edge
	int nvertex_max = (int) (fabs(P2.x-P1.x) + fabs(P2.y-P1.y));
	int nvertex_max2 = nvertex_max;
	int ntri_max = 2*nvertex_max;
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

	myEDGEFILLPARAM.pIDVERTEX2 = (int*) new int[nvertex_max2];	
	myEDGEFILLPARAM.nvertex_max2 = nvertex_max2;	
	myEDGEFILLPARAM.p_nvertexfound2 = &nvertexfound2;

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

	//inner edge bounding rect
	int ntri_iebr_max = ntri_max * ntri_max;
	int nvertex_iebr_max = nvertex_max * nvertex_max;
	int ntrifound_iebr = 0;
	int nvertexfound_iebr = 0;
	myEDGEFILLPARAM.pIDTRI_iebr = (int*) new int[ntri_iebr_max];	
	myEDGEFILLPARAM.ntri_iebr_max = ntri_iebr_max;	
	myEDGEFILLPARAM.p_ntrifound_iebr = &ntrifound_iebr;
	myEDGEFILLPARAM.pIDVERTEX_iebr = (int*) new int[nvertex_iebr_max];
	myEDGEFILLPARAM.nvertex_iebr_max = nvertex_iebr_max;	
	myEDGEFILLPARAM.p_nvertexfound_iebr = &nvertexfound_iebr;
	
	//outter edge bounding rect
	int ntri_oebr_max = ntri_max * ntri_max;
	int nvertex_oebr_max = nvertex_max * nvertex_max;
	int ntrifound_oebr = 0;
	int nvertexfound_oebr = 0;
	myEDGEFILLPARAM.pIDTRI_oebr = (int*) new int[ntri_oebr_max];	
	myEDGEFILLPARAM.ntri_oebr_max = ntri_oebr_max;	
	myEDGEFILLPARAM.p_ntrifound_oebr = &ntrifound_oebr;
	myEDGEFILLPARAM.pIDVERTEX_oebr = (int*) new int[nvertex_oebr_max];
	myEDGEFILLPARAM.nvertex_oebr_max = nvertex_oebr_max;	
	myEDGEFILLPARAM.p_nvertexfound_oebr = &nvertexfound_oebr;

*/