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

// OW2AGdoc.cpp : implementation of the COW2AGDoc class
//

#include "stdafx.h"
#include "oifiilib.h" //spi 2014

#include "c_pointset.h"
#include "owpointset.h"

#include "OWpixelset.h"

#include "OWdoc.h"
#include "OW2doc.h"
#include "OW2AGdoc.h"

#include "Owview.h"
#include "Ow2view.h"
#include "OW2AGview.h"

#include <string>
#include "portaudio.h"

#include "sndfile.h"
//#include "spiws_WavSet.h" //spi 2014
#include "spiwavsetlib.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COW2Doc

IMPLEMENT_DYNCREATE(COW2AGDoc, COW2Doc)

BEGIN_MESSAGE_MAP(COW2AGDoc, COW2Doc)
	//{{AFX_MSG_MAP(COW2AGDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(COW2AGDoc, COW2Doc)
	//{{AFX_DISPATCH_MAP(COW2AGDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// COW2AGDoc construction/destruction

COW2AGDoc::COW2AGDoc()
{
	m_idpointstart=-1;
	m_idpointstartrow;
	m_nCol = -1;
	m_pWavsetList = new CPtrList;
}

COW2AGDoc::~COW2AGDoc()
{	
	if (!m_pWavsetList->IsEmpty())
	{
		for(POSITION pos=m_pWavsetList->GetHeadPosition(); pos!=NULL;)
		{
			WavSet* pWavset = (WavSet*) m_pWavsetList->GetNext(pos);
			delete pWavset;
		}
	}
	if(m_pWavsetList) delete m_pWavsetList;
}

int COW2AGDoc::GetNumberOfRow()
{
	//2020july03, spi, begin
	//return m_pWavsetList->GetCount();
	return (int)(m_pWavsetList->GetCount());	
	//2020july03, spi, end
}

int COW2AGDoc::GetNumberOfCol()
{
	return m_nCol;
}

void COW2AGDoc::SetNumberOfCol(int nCol)
{
	m_nCol = nCol;
}

BOOL COW2AGDoc::IsSupportedPixelset(const char* filename)
{
	//this filename's extension
    char*  pCharFileExt = strrchr((char*)filename,'.');
	if(pCharFileExt==NULL)
	{
		//no valid file extension
		ASSERT(FALSE);
		return FALSE;
	}
	CStringA pszFileExt = _strupr( pCharFileExt );
	if(pszFileExt==".") 
	{
		//no valid file extension
		ASSERT(FALSE);
		return FALSE;
	}
	if(IsFileExtensionOfPixelsetType((const char*)pszFileExt)>0) return TRUE;
	  else return FALSE;
}

BOOL COW2AGDoc::ReplacePixelset(const char* filename)
{
	//1) delete pointset
	if(m_pPointset)
	{
		//don't bother saving current state, but keep pixelset and all wavsets
		delete m_pPointset;
		m_pPointset = NULL;
	}
	//2) delete existing pixelset
	if(m_pPixelset!=NULL) 
	{
		delete m_pPixelset;
		m_pPixelset=NULL;
	}
	//3) load in new pixelset
	CString myString(filename);
	m_pPixelset=OpenFileAsPixelset(myString);

	POSITION pos = GetFirstViewPosition();
	COW2AGView* pFirstView = (COW2AGView*) GetNextView(pos);
	pFirstView-> m_WorldRect = GetLargestWorldRect();
	pFirstView->m_ViewRect = GetViewingWorldRect();	
	pFirstView->m_itriseed=0;
	SetNumberOfCol(-1);

	//4) re-insert pointset
	if(m_pPointset==NULL && !m_pWavsetList->IsEmpty())
	{
		//add global controls and for each wavset, add row ...
		/*
		UpdateRows(POINTSET_TIN_NOUPDATE);
		UpdateTIN();
		*/
		UpdateRows(POINTSET_TIN_UPDATE);

	}

	pFirstView->SetScrollInfo();
	pFirstView->Invalidate(TRUE);
	if(m_pPixelset)
	{
		UpdateAllViews(NULL);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL COW2AGDoc::IsSupportedWavset(const char* filename)
{
	SF_INFO mySF_INFO;
	SNDFILE* pSNDFILE = sf_open(filename, SFM_READ, &mySF_INFO);
	if(pSNDFILE==NULL) 
	{
		//can't open that file
		return FALSE;
	}
	else 
	{
		sf_close(pSNDFILE);
		//todo, further implement in order to restrict to wav file format "really" fully supported by our app. 
		return TRUE;
	}
}

BOOL COW2AGDoc::AddWavset(const char* filename)
{
	//1) check if filename already in the wavset list
	WavSet* pWavset = GetWavsetFromFilename(filename);
	if(pWavset!=NULL) return FALSE;

	//2) if not, add it to the list
	return AddRow(filename);
}

WavSet* COW2AGDoc::GetWavsetFromFilename(const char* filename)
{
	WavSet* pWavset = NULL;
	if (!m_pWavsetList->IsEmpty())
	{
		for(POSITION pos=m_pWavsetList->GetHeadPosition(); pos!=NULL;)
		{
			pWavset = (WavSet*) m_pWavsetList->GetNext(pos);
			CStringA myString(pWavset->GetName());
			if(myString.CompareNoCase(filename)==0)
			{
				//match
				return pWavset;
			}
		}
		pWavset=NULL;
	}
	return pWavset;
}

BOOL COW2AGDoc::AddRow(const char* filename)
{
	//1) get actual number of row
	int nRowBefore=GetNumberOfRow();

	//2) add wavset to the list
	WavSet* pWavset = new WavSet;
	pWavset->ReadWavFile(filename);
	m_pWavsetList->AddTail(pWavset);

	//3) if required, initialize frame and add global controls
	if(m_pPointset==NULL)
	{
		BOOL bResult = AddFrameAndGlobalControls();
		if(bResult==FALSE) return FALSE;
	}

	//4) add row for this filename
	return AddRow(nRowBefore, POINTSET_TIN_UPDATE);
}

BOOL COW2AGDoc::AddFrameAndGlobalControls(int iflagTINUpdate)
{
	//0) get area for global controls
	double restrictedarea_inratio=(double)OW2AGDOC_CONTROL_RECTANGULARAREASIZE_INPERCENT/100.00; 

	if(m_pPointset!=NULL) 
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if(m_pPointset==NULL)
	{
		//1.5.1) alloc pointset
		m_pPointset = new COWPointset(POINTSET_INITIAL_NUMBEROFPOINTS, POINTSET_DEFAULT_NUMBEROFSTAT);
		if(m_pPointset==NULL) return FALSE;
		int nX=20;
		int nY=20;
		/*
		if(m_pPixelset)
		{
			CRect myImageRect=m_pPixelset->GetCropRect();
			nX=myImageRect.Width()/8;
			nY=myImageRect.Height()/8;
			if(nX<10) nX=10;
			if(nY<10) nX=10;
		}
		*/
		CRect myRect;
		myRect = GetWorldRect();
		//1.5.2 add rectangular point frames
		double dfXmin;
		double dfYmin;
		double dfXmax;
		double dfYmax;
		//double dfXwidth;
		//double dfYwidth;
		//1.5.3) add first point frame (100% world rect)
		dfXmin=myRect.left;
		dfYmin=myRect.top;
		dfXmax=myRect.right;
		dfYmax=myRect.bottom;		
		m_pPointset->AddPointFrame(nX, nY, dfXmin, dfYmin, dfXmax, dfYmax);
		//1.5.4) add first point serie (to be used as global control, i.e. volume, tempo, number of time division, etc.)
		/* */
		dfXmin=dfXmin+4.0;
		dfXmax=dfXmax-4.0;
		ASSERT(dfXmin!=dfXmax);
		/* */
		double dfY = (1.00-restrictedarea_inratio)*(dfYmax-dfYmin); //20%
		m_pPointset->AddPointSerie(nX, dfXmin, dfY, dfXmax, dfY, POINTSET_ADDPOINT_XMARGIN);
		m_idpointstart=m_pPointset->GetNumberOfPoints();
		dfY = (1.00-restrictedarea_inratio/2)*(dfYmax-dfYmin); //middle of the restristed area
		m_pPointset->AddPointSerie(OW2AGDOC_CONTROL_TOTALNUMBEROFCONTROL, dfXmin, dfY, dfXmax, dfY, POINTSET_ADDPOINT_XMARGIN);
		//1.5.5) set defaults
		double dfRatio=0.0;
		dfRatio=ComputeControlRatio(m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_VOLUME, OW2AGDOC_CONTROL_VOLUME_DEFAULT);
		m_pPointset->SetStatisticsValue(m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_VOLUME, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL,dfRatio);
		dfRatio=ComputeControlRatio(m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_TEMPO, OW2AGDOC_CONTROL_TEMPO_DEFAULT);
		m_pPointset->SetStatisticsValue(m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_TEMPO, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL,dfRatio);
		dfRatio=ComputeControlRatio(m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGNUMERATOR, OW2AGDOC_CONTROL_TIMESIGNUMERATOR_DEFAULT);
		m_pPointset->SetStatisticsValue(m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGNUMERATOR, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL,dfRatio);
		dfRatio=ComputeControlRatio(m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGDENOMINATOR, OW2AGDOC_CONTROL_TIMESIGDENOMINATOR_DEFAULT);
		m_pPointset->SetStatisticsValue(m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGDENOMINATOR, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL,dfRatio);
		dfRatio=ComputeControlRatio(m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_MEASURESINDISPLAY, OW2AGDOC_CONTROL_MEASURESINDISPLAY_DEFAULT);
		m_pPointset->SetStatisticsValue(m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_MEASURESINDISPLAY, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL,dfRatio);
		dfRatio=ComputeControlRatio(m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_NUMBEROFSTEPSPERBEAT, OW2AGDOC_CONTROL_NUMBEROFSTEPSPERBEAT_DEFAULT);
		m_pPointset->SetStatisticsValue(m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_NUMBEROFSTEPSPERBEAT, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL,dfRatio);
		m_idpointstartrow=m_pPointset->GetNumberOfPoints();
		UpdateNumberOfColFromControls();
		if(iflagTINUpdate==POINTSET_TIN_UPDATE) UpdateTIN();
	}
	return TRUE;
}

BOOL COW2AGDoc::AddRow(int nRowBefore, int iflagTINUpdate)
{
	if(m_pPointset==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//0) get area for global controls
	double restrictedarea_inratio=(double)OW2AGDOC_CONTROL_RECTANGULARAREASIZE_INPERCENT/100.00; 
	/*
	//1) get actual number of row
	int nRowBefore=GetNumberOfRow();
	*/
	int nX = GetNumberOfCol(); 
	CRect myRect;
	myRect = GetWorldRect();
	myRect.top = restrictedarea_inratio*(myRect.bottom-myRect.top);
	double dfX1=myRect.left;
	double dfX2=myRect.right;
	/* */
	dfX1=dfX1+4.0;
	dfX2=dfX2-4.0;
	ASSERT(dfX1!=dfX2);
	/* */
	//double dfYStepOld=(myRect.bottom-myRect.top)/(nRowBefore+1);
	double dfYStepNew=(myRect.bottom-myRect.top)/(nRowBefore+2);
	//2) translate previous rows
	if(nRowBefore>0)
	{
		for(int i=0; i<nRowBefore; i++)
		{
			//double dfY=(i+1)*dfYStepNew;
			double dfY=(myRect.bottom-myRect.top)-((i+1)*dfYStepNew);
			//double dfY=(myRect.top)+((i+1)*dfYStepNew);
			for(int k=0; k<m_nCol; k++)
			{
				//m_pPointset->m_pPOINTSET->py[m_idpointstartrow+i*m_nCol+k]=dfY;
				m_pPointset->MovePoint(m_idpointstartrow+i*m_nCol+k, m_pPointset->m_pPOINTSET->px[m_idpointstartrow+i*m_nCol+k], dfY);
			}
		}
	}

	//3.1) add new point row to the pointset
	int idpointseriestart = m_pPointset->GetNumberOfPoints();
	//double dfY=(nRowBefore+1)*dfYStepNew;
	//double dfY=(myRect.top)+((nRowBefore+1)*dfYStepNew);
	double dfY=(myRect.bottom-myRect.top)-((nRowBefore+1)*dfYStepNew);
	m_pPointset->AddPointSerie(nX, dfX1, dfY, dfX2, dfY, POINTSET_ADDPOINT_XMARGIN);
	//3.2) set states
	for(int i=0; i<nX; i++)
	{
		int idpoint=idpointseriestart+i;
		double dfValue=0.0; //0%
		if(idpoint==idpointseriestart) dfValue=1.0; //100%
		m_pPointset->SetStatisticsValue(idpoint, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL,dfValue); //the fourth user-defined statistics is used as a volume level
	}

	if(iflagTINUpdate==POINTSET_TIN_UPDATE) 
	{
		UpdateTIN();
		//5) update all views
		UpdateAllViews(NULL);
	}
	return TRUE;
}

BOOL COW2AGDoc::RemoveAllLastPointsFrom(int idpointstartrow, int iflagTINUpdate) //idpointstartrow included
{
	if(m_pPointset==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	m_pPointset->m_pPOINTSET->npts = idpointstartrow;
	if(iflagTINUpdate==POINTSET_TIN_UPDATE) UpdateTIN();
	return TRUE;
}

BOOL COW2AGDoc::UpdateRows(int iflagTINUpdate)
{
	if(m_pPointset==NULL)
	{
		BOOL bResult = AddFrameAndGlobalControls();
		if(bResult==FALSE) return FALSE;
	}
	//add one row, for each wavset in place
	//2020july03, spi, begin
	//int nrowtracks = m_pWavsetList->GetCount();
	int nrowtracks = (int)(m_pWavsetList->GetCount());
	//2020july03, spi, end
	for(int i=0; i<nrowtracks; i++)
	{
		/*
		AddRow();
		*/
		AddRow(i);
	}
	if(iflagTINUpdate==POINTSET_TIN_UPDATE) UpdateTIN();
	return TRUE;
}

BOOL COW2AGDoc::UpdateTIN()
{
	//4) update TIN
	m_pPointset->BuildTINAndComputeStatistics(POINTSET_TIN_NONORMALIZE);

	//4.5) prevent some voronoi polygons to be displayed (for points used to separate global controls from track volume controls)
	for(int i=0; i<m_idpointstart; i++)
	{
		//skip drawing when previously "computed/or tagged" voronoi area is negative
		m_pPointset->SetStatisticsValue(i, POINTSET_OFFSETSTAT_VOROAREA, POINTSET_TAGFORINVALIDVOROAREA);
	}
	return TRUE;
}

double COW2AGDoc::GetControlUserValue(int idpoint)
{
	double dfUserValue=0.0;
	if(m_pPointset==NULL)
	{
		ASSERT(FALSE);
		return dfUserValue;
	}
	if(OW2AGDOC_CONTROL_TOTALNUMBEROFCONTROL<1)
	{
		ASSERT(FALSE);
		return dfUserValue;
	}
	if(idpoint<m_idpointstart || idpoint>(m_idpointstart+OW2AGDOC_CONTROL_TOTALNUMBEROFCONTROL-1))
	{
		ASSERT(FALSE);
		return dfUserValue;
	}
	//convert from value between 0.0 and 1.0
	double dfValue = m_pPointset->GetStatisticsValue(idpoint, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL);
	dfUserValue = ComputeControlUserValue(idpoint, dfValue);
	return dfUserValue;
}

double COW2AGDoc::ComputeControlUserValue(int idpoint, double fRatio)
{
	double dfUserValue=0.0;
	if(m_pPointset==NULL)
	{
		ASSERT(FALSE);
		return dfUserValue;
	}
	if(OW2AGDOC_CONTROL_TOTALNUMBEROFCONTROL<1)
	{
		ASSERT(FALSE);
		return dfUserValue;
	}
	if(idpoint<m_idpointstart || idpoint>(m_idpointstart+OW2AGDOC_CONTROL_TOTALNUMBEROFCONTROL-1))
	{
		ASSERT(FALSE);
		return dfUserValue;
	}
	if(fRatio<0.0 || fRatio>1.0)
	{
		ASSERT(FALSE);
		return dfUserValue;
	}
	double dfMinValue;
	double dfMaxValue;
	if((idpoint-m_idpointstart)==OW2AGDOC_CONTROL_IDPOINTOFFSET_VOLUME)
	{
		dfMinValue=OW2AGDOC_CONTROL_VOLUME_MIN;
		dfMaxValue=OW2AGDOC_CONTROL_VOLUME_MAX;
	}
	else if((idpoint-m_idpointstart)==OW2AGDOC_CONTROL_IDPOINTOFFSET_TEMPO)
	{
		dfMinValue=OW2AGDOC_CONTROL_TEMPO_MIN;
		dfMaxValue=OW2AGDOC_CONTROL_TEMPO_MAX;
	}
	else if((idpoint-m_idpointstart)==OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGNUMERATOR)
	{
		dfMinValue=OW2AGDOC_CONTROL_TIMESIGNUMERATOR_MIN;
		dfMaxValue=OW2AGDOC_CONTROL_TIMESIGNUMERATOR_MAX;
	}
	else if((idpoint-m_idpointstart)==OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGDENOMINATOR)
	{
		dfMinValue=OW2AGDOC_CONTROL_TIMESIGDENOMINATOR_MIN;
		dfMaxValue=OW2AGDOC_CONTROL_TIMESIGDENOMINATOR_MAX;
	}
	else if((idpoint-m_idpointstart)==OW2AGDOC_CONTROL_IDPOINTOFFSET_MEASURESINDISPLAY)
	{
		dfMinValue=OW2AGDOC_CONTROL_MEASURESINDISPLAY_MIN;
		dfMaxValue=OW2AGDOC_CONTROL_MEASURESINDISPLAY_MAX;
	}
	else if((idpoint-m_idpointstart)==OW2AGDOC_CONTROL_IDPOINTOFFSET_NUMBEROFSTEPSPERBEAT)
	{
		dfMinValue=OW2AGDOC_CONTROL_NUMBEROFSTEPSPERBEAT_MIN;
		dfMaxValue=OW2AGDOC_CONTROL_NUMBEROFSTEPSPERBEAT_MAX;
	}
	else
	{
		ASSERT(FALSE);
		return dfUserValue;
	}
	//convert to user meaningfull value 
	dfUserValue = fRatio*(dfMaxValue-dfMinValue)+dfMinValue;
	/* 
	if((idpoint-m_idpointstart)==OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGDENOMINATOR)
	{
		dfUserValue = pow(2.0,dfUserValue);
	}
	*/
	return dfUserValue;
	//m_pPointset->SetStatisticsValue(idpoint, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL, dfValue);
}

double COW2AGDoc::ComputeControlRatio(int idpoint, double dfUserValue)
{
	double dfRatio=0.0;
	if(m_pPointset==NULL)
	{
		ASSERT(FALSE);
		return dfRatio;
	}
	if(OW2AGDOC_CONTROL_TOTALNUMBEROFCONTROL<1)
	{
		ASSERT(FALSE);
		return dfRatio;
	}
	if(idpoint<m_idpointstart || idpoint>(m_idpointstart+OW2AGDOC_CONTROL_TOTALNUMBEROFCONTROL-1))
	{
		ASSERT(FALSE);
		return dfRatio;
	}
	double dfMinValue;
	double dfMaxValue;
	if((idpoint-m_idpointstart)==OW2AGDOC_CONTROL_IDPOINTOFFSET_VOLUME)
	{
		dfMinValue=OW2AGDOC_CONTROL_VOLUME_MIN;
		dfMaxValue=OW2AGDOC_CONTROL_VOLUME_MAX;
	}
	else if((idpoint-m_idpointstart)==OW2AGDOC_CONTROL_IDPOINTOFFSET_TEMPO)
	{
		dfMinValue=OW2AGDOC_CONTROL_TEMPO_MIN;
		dfMaxValue=OW2AGDOC_CONTROL_TEMPO_MAX;
	}
	else if((idpoint-m_idpointstart)==OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGNUMERATOR)
	{
		dfMinValue=OW2AGDOC_CONTROL_TIMESIGNUMERATOR_MIN;
		dfMaxValue=OW2AGDOC_CONTROL_TIMESIGNUMERATOR_MAX;
	}
	else if((idpoint-m_idpointstart)==OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGDENOMINATOR)
	{
		dfMinValue=OW2AGDOC_CONTROL_TIMESIGDENOMINATOR_MIN;
		dfMaxValue=OW2AGDOC_CONTROL_TIMESIGDENOMINATOR_MAX;
	}
	else if((idpoint-m_idpointstart)==OW2AGDOC_CONTROL_IDPOINTOFFSET_MEASURESINDISPLAY)
	{
		dfMinValue=OW2AGDOC_CONTROL_MEASURESINDISPLAY_MIN;
		dfMaxValue=OW2AGDOC_CONTROL_MEASURESINDISPLAY_MAX;
	}
	else if((idpoint-m_idpointstart)==OW2AGDOC_CONTROL_IDPOINTOFFSET_NUMBEROFSTEPSPERBEAT)
	{
		dfMinValue=OW2AGDOC_CONTROL_NUMBEROFSTEPSPERBEAT_MIN;
		dfMaxValue=OW2AGDOC_CONTROL_NUMBEROFSTEPSPERBEAT_MAX;
	}
	else
	{
		ASSERT(FALSE);
		return dfRatio;
	}
	if(dfUserValue<dfMinValue || dfUserValue>dfMaxValue)
	{
		ASSERT(FALSE);
		return dfRatio;
	}
	if(dfMaxValue-dfMinValue<=0.0)
	{
		ASSERT(FALSE);
		return dfRatio;
	}
	//convert to value between 0.0 and 1.0
	dfRatio = (dfUserValue-dfMinValue)/(dfMaxValue-dfMinValue);
	return dfRatio;
	//m_pPointset->SetStatisticsValue(idpoint, POINTSET_EXTRA_NUMBEROFSTAT+OW2AGDOC_XYZI_LEVEL, dfValue);
}

void COW2AGDoc::UpdateNumberOfColFromControls()
{
	int prev_nCol=GetNumberOfCol();
	double dfMEASURESINDISPLAY=GetControlUserValue(m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_MEASURESINDISPLAY);
	double dfTIMESIGNUMERATOR=GetControlUserValue(m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGNUMERATOR);
	double dfNUMBEROFSTEPSPERBEAT=GetControlUserValue(m_idpointstart+OW2AGDOC_CONTROL_IDPOINTOFFSET_NUMBEROFSTEPSPERBEAT);
	int nCol=((int)(dfMEASURESINDISPLAY+0.5))*((int)(dfTIMESIGNUMERATOR+0.5))*((int)(dfNUMBEROFSTEPSPERBEAT+0.5));
	SetNumberOfCol(nCol);
	/* */
	if(prev_nCol!=-1 && nCol!=prev_nCol)
	{
		//1) don't bother saving current state, but keep pixelset and all wavsets
		//2) delete all points with idpoint greater than or equal to m_idpointstartrow
		RemoveAllLastPointsFrom(m_idpointstartrow);
		//3) for each wavset, add row ...
		UpdateRows();
	}
	/* */
}
