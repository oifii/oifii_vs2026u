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

// OW2view.cpp : implementation of the COW2View class
//

#include "stdafx.h"
#include "oifiilib.h" //spi 2014
#include "oifii_app.h"

#include "OWpixelset.h" //offworld c++ pixelset library

#include "c_pointset.h" //offworld c pointset library
#include "c_segmentset.h" //offworld c segmentset library

#include "OWpointset.h" //offworld c++ pointset library (interface sitting on pointset.h)
#include "OWsegmentset.h" //offworld c++ segmentset library (interface sitting on segmentset.h)
#include "OWclassset.h" //offworld c++ classset library (interface sitting on segmentset.h)
#include "OWedgeset.h" //offworld edgeset library (for line, polyline, polygone  and shape)

#include "OWdoc.h"
#include "logdoc.h"
#include "OW2doc.h"
#include "OW2mdi.h"
#include "OWview.h"
#include "OW2view.h"

//2012august23, poirier, begin
#include "portaudio.h"
#include "pa_asio.h"
#include <map>
#include <string>
using namespace std;
//2012august23, poirier, end
#include "oifii_mainfrm.h"

#include "oifii_gotodlg.h"
#include "oifii_proppage.h"


/*
#include "imglib.h"
#include "img_err.h"
*/
#include "ow_iff.h"

#include <gl\gl.h>
#include <gl\glu.h>
#include "oifii_propsettings.h"
#include "oifii_propsheet.h"

#include <wingdi.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COW2View

IMPLEMENT_DYNCREATE(COW2View, COWView)

BEGIN_MESSAGE_MAP(COW2View, COWView)
	//{{AFX_MSG_MAP(COW2View)
	ON_WM_CREATE()
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_ZOOM_PERCENT, OnZoomPercent)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_PERCENT, OnUpdateZoomPercent)
	ON_COMMAND(ID_ZOOM_POINTIN, OnZoomPointin)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_POINTIN, OnUpdateZoomPointin)
	ON_COMMAND(ID_ZOOM_POINTOUT, OnZoomPointout)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_POINTOUT, OnUpdateZoomPointout)
	ON_COMMAND(ID_ZOOM_RECTIN, OnZoomRectin)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_RECTIN, OnUpdateZoomRectin)
	ON_COMMAND(ID_ZOOM_ZOOMNORMAL, OnZoomZoomnormal)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_ZOOMNORMAL, OnUpdateZoomZoomnormal)
	ON_COMMAND(ID_ZOOM_ZOOMOFF, OnZoomZoomoff)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_ZOOMOFF, OnUpdateZoomZoomoff)
	ON_COMMAND(ID_ZOOM_ZOOMTOFIT, OnZoomZoomtofit)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_ZOOMTOFIT, OnUpdateZoomZoomtofit)
	ON_COMMAND(ID_ZOOMP_10, OnZoomp10)
	ON_UPDATE_COMMAND_UI(ID_ZOOMP_10, OnUpdateZoomp10)
	ON_COMMAND(ID_ZOOMP_100, OnZoomp100)
	ON_UPDATE_COMMAND_UI(ID_ZOOMP_100, OnUpdateZoomp100)
	ON_COMMAND(ID_ZOOMP_150, OnZoomp150)
	ON_UPDATE_COMMAND_UI(ID_ZOOMP_150, OnUpdateZoomp150)
	ON_COMMAND(ID_ZOOMP_200, OnZoomp200)
	ON_UPDATE_COMMAND_UI(ID_ZOOMP_200, OnUpdateZoomp200)
	ON_COMMAND(ID_ZOOMP_400, OnZoomp400)
	ON_UPDATE_COMMAND_UI(ID_ZOOMP_400, OnUpdateZoomp400)
	ON_COMMAND(ID_ZOOMP_50, OnZoomp50)
	ON_UPDATE_COMMAND_UI(ID_ZOOMP_50, OnUpdateZoomp50)
	ON_COMMAND(ID_ZOOMP_75, OnZoomp75)
	ON_UPDATE_COMMAND_UI(ID_ZOOMP_75, OnUpdateZoomp75)
	ON_COMMAND(ID_ZOOMP_800, OnZoomp800)
	ON_UPDATE_COMMAND_UI(ID_ZOOMP_800, OnUpdateZoomp800)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
    ON_CBN_SELCHANGE(IDW_COMBO,OnComboSelChange)
	ON_CBN_EDITCHANGE(IDW_COMBO,OnComboEditChange)
	ON_CBN_EDITUPDATE(IDW_COMBO,OnComboEditUpdate)
	ON_WM_KEYUP()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_ANALYSIS_LOCALEXTRACTION, OnAnalysisLocalextraction)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_LOCALEXTRACTION, OnUpdateAnalysisLocalextraction)
	ON_COMMAND(ID_ANALYSIS_LAUNCHSCENARIO, OnAnalysisLaunchscenario)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_LAUNCHSCENARIO, OnUpdateAnalysisLaunchscenario)
	ON_COMMAND(ID_ANALYSIS_LOCALCLASSIFICATION, OnAnalysisLocalclassification)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_LOCALCLASSIFICATION, OnUpdateAnalysisLocalclassification)
	ON_COMMAND(ID_ANALYSIS_LOCALSEGMENTATION, OnAnalysisLocalsegmentation)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_LOCALSEGMENTATION, OnUpdateAnalysisLocalsegmentation)
	ON_COMMAND(ID_ANALYSIS_TEXTUREEXTRACTION, OnAnalysisTextureextraction)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_TEXTUREEXTRACTION, OnUpdateAnalysisTextureextraction)
	ON_COMMAND(ID_ANALYSIS_TEXTURESEGMENTATION, OnAnalysisTexturesegmentation)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_TEXTURESEGMENTATION, OnUpdateAnalysisTexturesegmentation)
	ON_COMMAND(ID_TRANS_LAUNCHSCENARIO, OnTransLaunchscenario)
	ON_UPDATE_COMMAND_UI(ID_TRANS_LAUNCHSCENARIO, OnUpdateTransLaunchscenario)
	ON_COMMAND(ID_TRANS_SPATIAL_CONFINEDSTRETCH, OnTransSpatialConfinedstretch)
	ON_UPDATE_COMMAND_UI(ID_TRANS_SPATIAL_CONFINEDSTRETCH, OnUpdateTransSpatialConfinedstretch)
	ON_COMMAND(ID_TRANS_SPATIAL_ROTATION, OnTransSpatialRotation)
	ON_UPDATE_COMMAND_UI(ID_TRANS_SPATIAL_ROTATION, OnUpdateTransSpatialRotation)
	ON_COMMAND(ID_TRANS_SPATIAL_STRETCH, OnTransSpatialStretch)
	ON_UPDATE_COMMAND_UI(ID_TRANS_SPATIAL_STRETCH, OnUpdateTransSpatialStretch)
	ON_COMMAND(ID_TRANS_SPECTRAL_INVERT, OnTransSpectralInvert)
	ON_UPDATE_COMMAND_UI(ID_TRANS_SPECTRAL_INVERT, OnUpdateTransSpectralInvert)
	ON_COMMAND(ID_TRANS_TEMPORAL_EVOLVE, OnTransTemporalEvolve)
	ON_UPDATE_COMMAND_UI(ID_TRANS_TEMPORAL_EVOLVE, OnUpdateTransTemporalEvolve)
	ON_COMMAND(ID_TRANSFORMATION_REPEATINSEQUENCE, OnTransformationRepeatinsequence)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORMATION_REPEATINSEQUENCE, OnUpdateTransformationRepeatinsequence)
	ON_COMMAND(ID_TRANS_SPATIAL_EDGEDETECT, OnTransSpatialEdgedetect)
	ON_UPDATE_COMMAND_UI(ID_TRANS_SPATIAL_EDGEDETECT, OnUpdateTransSpatialEdgedetect)
	ON_COMMAND(ID_TRANS_SPATIAL_FLATDETECT, OnTransSpatialFlatdetect)
	ON_UPDATE_COMMAND_UI(ID_TRANS_SPATIAL_FLATDETECT, OnUpdateTransSpatialFlatdetect)
	ON_COMMAND(ID_TRANS_SPATIAL_MAXDETECT, OnTransSpatialMaxdetect)
	ON_UPDATE_COMMAND_UI(ID_TRANS_SPATIAL_MAXDETECT, OnUpdateTransSpatialMaxdetect)
	ON_COMMAND(ID_TRANS_SPATIAL_SHARPEN, OnTransSpatialSharpen)
	ON_UPDATE_COMMAND_UI(ID_TRANS_SPATIAL_SHARPEN, OnUpdateTransSpatialSharpen)
	ON_COMMAND(ID_TRANS_SPECTRAL_EQUALIZE, OnTransSpectralEqualize)
	ON_UPDATE_COMMAND_UI(ID_TRANS_SPECTRAL_EQUALIZE, OnUpdateTransSpectralEqualize)
	ON_COMMAND(ID_TRANS_SPECTRAL_HSI, OnTransSpectralHsi)
	ON_UPDATE_COMMAND_UI(ID_TRANS_SPECTRAL_HSI, OnUpdateTransSpectralHsi)
	ON_COMMAND(ID_TRANS_SPECTRAL_STRETCH, OnTransSpectralStretch)
	ON_UPDATE_COMMAND_UI(ID_TRANS_SPECTRAL_STRETCH, OnUpdateTransSpectralStretch)
	ON_COMMAND(ID_VIEW_NEWSELECTIONZOOMVIEW, OnViewNewZoomedview)
	ON_COMMAND(ID_ANALYSIS_SCALE_SELECTION, OnAnalysisScaleSelection)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NEWSELECTIONZOOMVIEW, OnUpdateViewNewZoomedview)
	ON_COMMAND(ID_TRANS_SPECTRAL_FILTER, OnTransSpectralFilter)
	ON_COMMAND(ID_ROI_RECT, OnRoiRect)
	ON_COMMAND(ID_ROI_POLYLINE, OnRoiPolyline)
	ON_COMMAND(ID_ROI_FLOATING, OnRoiFloating)
	ON_COMMAND(ID_ROI_FREEHAND, OnRoiFreehand)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_ZOOMP_1000, OnZoomp1000)
	ON_COMMAND(ID_ZOOMP_1500, OnZoomp1500)
	ON_COMMAND(ID_ZOOMP_2000, OnZoomp2000)
	ON_COMMAND(ID_ZOOMP_2500, OnZoomp2500)
	ON_COMMAND(ID_ZOOMP_3000, OnZoomp3000)
	ON_COMMAND(ID_SETTINGS_PROFILE, OnSettingsProfile)
	ON_COMMAND(ID_SETTINGS_HISTO, OnSettingsHisto)
	ON_COMMAND(ID_SETTINGS_SCATTER, OnSettingsScatter)
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_TRANS_SPECTRAL_CONVERTTOMONO, OnTransSpectralConverttomono)
	ON_UPDATE_COMMAND_UI(ID_TRANS_SPECTRAL_CONVERTTOMONO, OnUpdateTransSpectralConverttomono)
	ON_COMMAND(ID_TRANS_SPECTRAL_CONVERTTOMONO_SET, OnTransSpectralConverttomonoSet)
	ON_UPDATE_COMMAND_UI(ID_TRANS_SPECTRAL_CONVERTTOMONO_SET, OnUpdateTransSpectralConverttomonoSet)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_EDIT_COPY, &COW2View::OnEditCopy)
	END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COW2View construction/destruction

COW2View::COW2View(HWND hwnd, COWDocument* pCOWDoc) : COWView(hwnd, pCOWDoc) //spi 2014
{
	//m_parentHWND = hwnd;
	Init();
}

COW2View::COW2View()
{
	Init();
}

void COW2View::Init()
{
	//m_parentHWND=NULL; //spi 2014
	m_bMode_ROIFloating = FALSE;
	m_bMode_ROIRect = FALSE;
	m_bMode_ROIPolyline = FALSE;
	m_idroiROIPolylineDrag = -1;
	m_bMode_ROIFreehand = FALSE;
	m_idroiROIFreehandDrag = -1;

	m_bOnMouseMoveLocalExtractionON = FALSE;

	//poirier, begin, selection rect, imported and adapted from OISS
	m_SelectionRectWidth = 800/10;//800/5;
	m_SelectionRectHeight = 600/10;//600/5;
	m_SelectionRectWidthMin = 800/10;
	m_SelectionRectWidthMax = 800/2;
	m_SelectionRectHeightMin = 600/10;
	m_SelectionRectHeightMax = 600/2;
	m_wInflatingTimerOn = 0;
	m_nInflatingTimerID = 96;
	m_wDeflatingTimerOn = 0;
	m_nDeflatingTimerID = 69;
	m_selection_rect_thickness_inpixel = 0;
	//poirier, end, selection rect, imported and adapted from OISS

	//poirier, sept 2001, begin
	m_wDelayingOpenSettingsDlgTimerOn = 0;
	m_nDelayingOpenSettingsDlgTimerID = 99;

	m_wTransSpectralFilterTimerOn = 0;
	m_nTransSpectralFilterTimerID = 101;
	m_nTransSpectralFilterIndex = 0;
	//poirier, sept 2001, end

	//poirier, sept 2001, begin
	/* now in the doc
	m_pROIset = new COWRoiset;
	if(m_pROIset==NULL)
	{
		ASSERT(FALSE);
	}
	*/
	m_pZoomedView = NULL;
	m_wROIFloatingModeToggledBack = FALSE;
	//poirier, sept 2001, end

	m_WorldRect.left = 0;
	m_WorldRect.top = 0;
	m_WorldRect.right = 100;
	m_WorldRect.bottom = 100;
	m_ViewRect = m_WorldRect;
	m_WindowSize.cx = 140;
	m_WindowSize.cy = 140; 
	m_ViewOffset.x = 20;	//relative offset with view window origin
	m_ViewOffset.y = 20;	//relative offset with view window origin
	//m_fZoomFactor = 1.0;

	m_bRendered = FALSE;
	m_bBusyPainting = FALSE;

	m_hDDB = NULL;
	m_itriseed = 0;
	m_idPointsetObject = -1;
	m_numneighborfound = -1;
	m_idPrevObject = -1;
	m_idStartSegment = -1;
	//m_indexSegmentHistoryList = -1;
	//m_pSegmentHistoryList = NULL;
	m_indexHistoryList = -1;
	//m_pidsegHistoryList
	m_inumberofcollectedsegment = 0;
	m_imaxhistorylistsize = 1000;
	m_idHistorySegment = -1;
	m_idHistorySegmentPartition = -1;

	m_idPrevPointsetObjectUsedWithLocalGraph = -1;
	//shift key down is used for disabling temporarely the local graph updates
	m_bShiftKeyIsDown = FALSE;
}

COW2View::~COW2View()
{
	EmptyView();
}

void COW2View::EmptyView()
{
	//poirier, sept 2001, begin
	/* now in the doc
	if(m_pROIset) delete m_pROIset;
	*/
	if(m_pZoomedView)
	{
		((COW2ZoomedView*)m_pZoomedView)->DetachParentView();
		m_pZoomedView = NULL;
	}
	//poirier, sept 2001, end

	if(m_hDDB!=NULL) DeleteObject(m_hDDB);
	//if(m_pSegmentHistoryList) delete[] m_pSegmentHistoryList;
	if(m_savedBitmap.m_hObject!=NULL) 
	{
		m_savedBitmap.DeleteObject();
		m_savedBitmap.m_hObject = NULL;
	}
}

double COW2View::GetZoomFactor()
{
	return COWView::GetZoomFactor();
}

void COW2View::OnInitialUpdate() 
{
	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
    
	//get world rect
    m_WorldRect = pDoc->GetLargestWorldRect();
	//set view rect
	m_ViewRect = pDoc->GetViewingWorldRect();

	//poirier, sept 2001, begin
	/* now m_pROIset is only in the document
	*/
	InitialUpdateEDGESET_ROI();

	CRect myRect(pDoc->m_VOI_ZoomedViewRECT);
	if(myRect.IsRectEmpty()==FALSE)
	{
		m_ViewRect = myRect;
		/*
		m_WindowSize.cx = m_ViewRect.Width(); //with zoom factor 1.0 
		m_WindowSize.cy = m_ViewRect.Height(); //with zoom factor 1.0
		*/
		SetZoomFactor(pDoc->m_VOI_dfZoomFactor);
		m_WindowSize.cx = pDoc->m_VOI_WindowSIZE.cx; 
		m_WindowSize.cy = pDoc->m_VOI_WindowSIZE.cy; 
		m_ViewOffset.x = pDoc->m_VOI_ViewOffsetPOINT.x;  
		m_ViewOffset.y = pDoc->m_VOI_ViewOffsetPOINT.y; 
		//ZoomIn(pDoc->m_VOI_ZoomedViewRECT); //not here
		/* //done within COWView::OnInitialUpdate()
		m_pViewDataSettings->CopyAllValuesFrom(pDoc->m_pViewDataSettings);
		m_pProjectionSettings->CopyAllValuesFrom(pDoc->m_pProjectionSettings);
		m_pTransformationSettings->CopyAllValuesFrom(pDoc->m_pTransformationSettings);
		*/
	}
	else
	{
		/*
		SetZoomFactor(pDoc->m_fViewInitZoomFactor);
		m_WindowSize.cx = (int)(m_ViewRect.Width()*pDoc->m_fViewInitZoomFactor);
		m_WindowSize.cy = (int)(m_ViewRect.Height()*pDoc->m_fViewInitZoomFactor);
		*/
		
		SetZoomFactor(1.0);
		m_WindowSize.cx = m_ViewRect.Width();
		m_WindowSize.cy = m_ViewRect.Height();
		
		m_ViewOffset.x = 0;
		m_ViewOffset.y = 0;
	}

	/*
	if(this->IsKindOf(RUNTIME_CLASS(COW2ZoomedView)))
	{
		//for zoomed view
		if(m_WindowSize.cx>400) m_WindowSize.cx = 400;
		if(m_WindowSize.cy>300) m_WindowSize.cy = 300;
	}
	*/

	if(m_WindowSize.cx>0 && m_WindowSize.cy>0)
	{
		if(m_parentHWND==NULL) //spi 2014
		{
			// get the document parent frame window
			COW2MDI* pMDIFrame = (COW2MDI*)GetParentFrame();
			pMDIFrame->SetFrameSizeAccordToDocumentSize(m_WindowSize);
		}
		else
		{
			//spi 2014
			//todo: might have to do something in here
			//2021dec28, spi, begin
			//ASSERT(FALSE);
			//2021dec28, spi, end
		}
	}

	/*
	// adjust the size of the frame according to the size of the image
	CRect myRect;
	AfxGetApp()->m_pMainWnd->GetClientRect((LPRECT)myRect);
	if(myRect.Width()<m_ViewRect.Width() || myRect.Height()<m_ViewRect.Height())
	{
		//not enough room to display, scale down
		m_WindowSize.cx = myRect.Width();
		m_WindowSize.cy = myRect.Height();
		//m_fZoomFactor = m_WindowSize.cx/m_ViewRect.Width();
		SetZoomFactor(((double)(m_WindowSize.cx)/(double)m_ViewRect.Width()));
		double fNewZoomFactor = (double)m_WindowSize.cy/(double)m_ViewRect.Height();
	   	//if( fNewZoomFactor < m_fZoomFactor)
	   	if( fNewZoomFactor < GetZoomFactor())
		{
			//y determines the final zoomfactor
			//m_ViewOffset.x = (LONG) (m_WindowSize.cx - (m_fZoomFactor * m_ViewRect.Width()));
			m_ViewOffset.x = (LONG) (m_WindowSize.cx - (GetZoomFactor() * m_ViewRect.Width()));
			//m_fZoomFactor = fNewZoomFactor;
			SetZoomFactor(fNewZoomFactor);
		}
		else
		{
			//x determines the final zoomfactor
 			//m_ViewOffset.y = (LONG) (m_WindowSize.cy - (m_fZoomFactor * m_ViewRect.Height()));
 			m_ViewOffset.y = (LONG) (m_WindowSize.cy - (GetZoomFactor() * m_ViewRect.Height()));
		}
	}
	else
	{
		m_WindowSize.cx = m_ViewRect.Width();
		m_WindowSize.cy = m_ViewRect.Height();
		//m_fZoomFactor = 1.0;
		SetZoomFactor(1.0);
		m_ViewOffset.x = 0;
		m_ViewOffset.y = 0;
	}
	*/



	// create DDB for eventual display
	if(pDoc->m_pPixelset && 
		m_bViewPixelset_DisplayRed==FALSE &&
		m_bViewPixelset_DisplayGreen==FALSE &&
		m_bViewPixelset_DisplayBlue==FALSE &&
		m_bViewPixelset_DisplayHue==FALSE &&
		m_bViewPixelset_DisplaySaturation==FALSE &&
 		m_bViewPixelset_DisplayIntensity==FALSE)
	{
		//2012july22, poirier, begin
		/*
		int imghandle = pDoc->m_pPixelset->GetIFFHandle();
		ASSERT(imghandle>-1); 
		m_hDDB = OW_IFF_IMG_dib_to_ddb(imghandle, m_WindowSize.cx, m_WindowSize.cy);
		*/
		m_hDDB = DIBtoDDBitmap((BYTE*) (pDoc->m_pPixelset->GetDIBPointer()));
		//2012july22, poirier, end
	}
	else if(pDoc->m_pPixelset && 
		m_bViewPixelset_DisplayRed==FALSE &&
		m_bViewPixelset_DisplayGreen==FALSE &&
		m_bViewPixelset_DisplayBlue==FALSE &&
		m_bViewPixelset_DisplayHue==FALSE &&
		m_bViewPixelset_DisplaySaturation==FALSE &&
		m_bViewPixelset_DisplayIntensity==TRUE)
	{
		//display intensity components
		
	}

	COWView::OnInitialUpdate();

	CRect myZoomedRect(pDoc->m_VOI_ZoomedViewRECT);
	if(myZoomedRect.IsRectEmpty()==FALSE)
	{
		ZoomIn(myZoomedRect); 
	}
}

HBITMAP COW2View::DIBtoDDBitmap(BYTE* pDIB)
{
	HBITMAP myHBITMAP;
	BITMAPINFOHEADER* pBMIH;
	BYTE* pBits;
	
	CPalette myPalette;
	CPalette* pOldPalette;
	CClientDC myDC(NULL);

	// initilize the pointer to the BITMAPINFOHEADER and to the bits
	pBMIH = (BITMAPINFOHEADER*)pDIB;
	pBits = pDIB + pBMIH->biSize + GetPaletteSize(pBMIH);

	// We also need a DC to hold our bitmap.  Also make sure that 
	//  the global palette is selected into the DC.

	/*
	HDC hDC = ::GetDC(NULL);
	ASSERT(hDC);
	*/

	/*
	HPALETTE hOldPalette = ::SelectPalette(hDC, (HPALETTE)CBaseView::m_globPal.GetGlobPalette()->GetSafeHandle(), FALSE);	
	::RealizePalette(hDC);
	*/
	// Create and select a logical palette if needed
	int nColors = CalcNumColors(pBMIH);
	if( nColors <= 256 && myDC.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;
		pLP->palNumEntries = nColors;
		for( int i=0; i < nColors; i++)
		{
			pLP->palPalEntry[i].peRed = ((BITMAPINFO*)pDIB)->bmiColors[i].rgbRed;
			pLP->palPalEntry[i].peGreen = ((BITMAPINFO*)pDIB)->bmiColors[i].rgbGreen;
			pLP->palPalEntry[i].peBlue = ((BITMAPINFO*)pDIB)->bmiColors[i].rgbBlue;
			pLP->palPalEntry[i].peFlags = 0;
		}
		myPalette.CreatePalette(pLP);
		delete[] pLP;
		// Select and realize the palette
		pOldPalette = myDC.SelectPalette(&myPalette, FALSE);
		myDC.RealizePalette();
	}
	else
	{
		//ASSERT(FALSE);
	}
	// initialize the bitmap handle
	myHBITMAP = ::CreateDIBitmap( myDC.GetSafeHdc(),
		    	        		  pBMIH,
		       			     	  CBM_INIT,
		            			  pBits,
		            			  (BITMAPINFO*)pDIB,
		            		 	  DIB_RGB_COLORS );
	ASSERT(myHBITMAP!=NULL);	
	/*
	if(hOldPalette)
		 ::SelectPalette(hDC, hOldPalette, FALSE);

	::ReleaseDC(NULL, hDC);	
	*/
	if (myPalette.GetSafeHandle())
		myDC.SelectPalette(pOldPalette,FALSE);
	return myHBITMAP;
}

DWORD COW2View::GetPaletteSize(BITMAPINFOHEADER* pBMIH)
{
	return CalcNumColors(pBMIH)*sizeof(RGBQUAD);

}

DWORD COW2View::CalcNumColors( BITMAPINFOHEADER* pBMIH)
{
    if( !pBMIH )
	{
    	ASSERT(FALSE);
	}    
  	if (pBMIH->biClrUsed != 0)
		return pBMIH->biClrUsed;

    switch (pBMIH->biBitCount)	// biBitCount - bits per pixel 
    {
    case 1:
            return 2;	// number of entryes in the color table
    case 4:
            return 16;
    case 8:
            return 256;
    default:			
            return 0;	// a 24 biBitCount DIB has no color table
    }
}

void COW2View::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CView::CalcWindowRect(lpClientRect, nAdjustType);
}

/////////////////////////////////////////////////////////////////////////////
// COW2View drawing

void COW2View::OnDraw(CDC* pDC)
{
	COW2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	/*
	//render if required
	if(!m_bRendered)
	{
		Render2D(pDoc, pDC);
	}
	*/
	Render2D(pDoc, pDC);

	/*
	//bitblt the memory DC to the screen DC
	CRect mySrcRect( 0, 0, pDoc->GetSizeX(), pDoc->GetSizeY());
	pDC->BitBlt(mySrcRect.left, 
				mySrcRect.top, 
				mySrcRect.Width(), 
				mySrcRect.Height(), 
				&m_MemDC, 
				0, //x offset in dest DC
				0, //y offset in dest DC
				SRCCOPY );
	*/
	return;
}


/////////////////////////////////////////////////////////////////////////////
// COW2View message handlers


HANDLE COW2View::CreateDIBFromRender2D()
{
	COW2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CDC* pDC = GetDC();
	CBitmap MemoryBitmap;
	CBitmap* pOldBitmap = NULL;
	CRect myWorldRect(pDoc->GetWorldRect());
	if(MemoryBitmap.CreateCompatibleBitmap(pDC, myWorldRect.Width(), myWorldRect.Height())!=NULL)
	{
		CDC MemoryDC;
		if(MemoryDC.CreateCompatibleDC(pDC)!=NULL)
		{
			pOldBitmap = MemoryDC.SelectObject(&MemoryBitmap);
			Render2D(pDoc, &MemoryDC);
			//pOldBitmap = MemoryDC.SelectObject(&MemoryBitmap);
			//MemoryDC.BitBlt(0, 0, myWorldRect.Width(), myWorldRect.Height(), &MemoryDC, 0, 0, SRCCOPY); 
			MemoryDC.SelectObject(pOldBitmap);
			//query BITMAPINFO
			BITMAPINFO myBI;
			myBI.bmiHeader.biBitCount = 0;
			GetDIBits(MemoryDC.GetSafeHdc(), (HBITMAP)MemoryBitmap.GetSafeHandle(), 0, myWorldRect.Height(), NULL, &myBI, DIB_RGB_COLORS); 
			// Allocate enough memory to hold bitmap infoheader and color table (here always 24bits, no color table)
			HANDLE hDIB = GlobalAlloc(GMEM_FIXED,myBI.bmiHeader.biSizeImage+sizeof(myBI));
			LPBITMAPINFOHEADER pDIB = (LPBITMAPINFOHEADER)hDIB;
			memcpy(pDIB, &myBI, sizeof(BITMAPINFOHEADER));
			GetDIBits(MemoryDC.GetSafeHdc(), (HBITMAP)MemoryBitmap.GetSafeHandle(), 0, myWorldRect.Height(), pDIB, &myBI, DIB_RGB_COLORS);
			return hDIB;
		}
	}
	ReleaseDC(pDC);
	return NULL; //error, but returns safely
}

//2012august13, poirier, begin
BITMAP COW2View::GetDDB(HBITMAP hDDB)
{
	BITMAP myBITMAP;
	GetObject(hDDB, sizeof(BITMAP), &myBITMAP);
	return myBITMAP;
}
//2012august13, poirier, end

void COW2View::DisplayDDB(HBITMAP hDDB, HDC hDC, long ix_offset, long iy_offset)
{
	BITMAP myBITMAP;
	GetObject(hDDB, sizeof(BITMAP), &myBITMAP);
	HDC myMemHDC = CreateCompatibleDC(hDC);
	HGDIOBJ prevHBITMAP = SelectObject(myMemHDC, hDDB);
	BitBlt(hDC, ix_offset, iy_offset, myBITMAP.bmWidth, myBITMAP.bmHeight, myMemHDC, 0, 0, SRCCOPY); 
	SelectObject(myMemHDC, prevHBITMAP);
	DeleteDC(myMemHDC);
}

void COW2View::Render2D(COW2Doc* pDoc, CDC* pDC)
{
	/*
	if(m_MemDC.CreateCompatibleDC(pDC) == FALSE)
	{
		m_bRendered = FALSE;
		ASSERT(FALSE);
		return; 
	}
	*/

	m_bBusyPainting = TRUE;

	//create font with proper size
	CFont myfont; 
	CFont* p_oldfont = NULL; 
	
	CSize sizeText = pDC->GetTextExtent(L"0", 1);
	
	int ifont_height = sizeText.cy; 
	int ifont_width = sizeText.cx;
	
	double fzoomfactor = GetZoomFactor();
	if(fzoomfactor<5.00) 
	{
		ifont_width = 0; //will automatically select font size
		ifont_height = 2;
	}
	else
	{
		ifont_width = 0; //will automatically select font size
		ifont_height = 1;

	}
	/*
#ifdef _DEBUG
	CString myString;
	myString.Format("text_ext_size: height=%d, width=%d\r\n", sizeText.cy, sizeText.cx);
	((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText(myString);
	myString.Format("fontsize: height=%d, width=%d\r\n", ifont_height, ifont_width);
	((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText(myString);
#endif //_DEBUG
	*/
	if(myfont.CreateFont(ifont_height, ifont_width, 0, 0, FW_THIN, 0, 0, 0, 0, OUT_TT_PRECIS, 
	   					 CLIP_DEFAULT_PRECIS, PROOF_QUALITY, TMPF_TRUETYPE, L"Arial"))
	{
		//if no error, select this font
		p_oldfont = pDC->SelectObject(&myfont);
	}


   	if(m_pViewDataSettings->bDrawPixelset && !pDC->IsPrinting())
   	//if(m_pViewDataSettings->bDrawPixelset)
	{
		//ASSERT(pDoc->m_pPixelset!=NULL);
		if(m_hDDB>0)
		{
			HDC hDC = pDC->GetSafeHdc();
			int nOldBackgroundMode = pDC->SetBkMode(TRANSPARENT);
			//2012july22, poirier, begin
			/*
			OW_IFF_IMG_display_ddb(m_hDDB, hDC, m_ViewOffset.x, m_ViewOffset.y);
			*/
			DisplayDDB(m_hDDB, hDC, m_ViewOffset.x, m_ViewOffset.y);
			//2012july22, poirier, end
			pDC->SetBkMode(nOldBackgroundMode);
		}
	}

	if(pDoc->m_pPointset!=NULL && (m_pViewDataSettings->bDrawPointset
		|| m_pViewDataSettings->bDrawPointsetLabels
		|| m_pViewDataSettings->bDrawClasssetLabels) )
	{
		CPen* pPen = new CPen;
		CPen* pOldPen;
		int nWidth=OW2VIEW_PENSIZE;
		//if(pDC->IsPrinting()) nWidth=OW2VIEW_PENSIZE_PRINTING;
		COLORREF crPOINTSETCOLOR = OW2VIEW_POINTSET_COLOR;
		if(pDC->IsPrinting()) crPOINTSETCOLOR = RGB(150, 150, 150); //pale gray when printing
		//if(pDC->IsPrinting()) crPOINTSETCOLOR = RGB(100, 100, 100); //dark gray when printing
		//if(pDC->IsPrinting()) crPOINTSETCOLOR = RGB(255, 255, 255); //white when printing
		//if(pDC->IsPrinting()) crPOINTSETCOLOR = RGB(0, 0, 0); //black when printing
		pPen->CreatePen(PS_SOLID, nWidth, crPOINTSETCOLOR);
    	pOldPen = pDC->SelectObject(pPen);
		//no brush
    	CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);
		//text color
		COLORREF crOldTextColor = pDC->SetTextColor(crPOINTSETCOLOR);
		int nOldBackgroundMode = pDC->SetBkMode(TRANSPARENT);

		//drawing flag
		int idrawingflag = 0;
		if(m_pViewDataSettings->bDrawClasssetLabels) idrawingflag = idrawingflag | POINTSET_DRAW_CLASSSETLABEL;
		if(m_pViewDataSettings->bDrawPointsetLabels) idrawingflag = idrawingflag | POINTSET_DRAW_POINTSETLABEL;
		if(m_pViewDataSettings->bDrawPointset) 
		{
			idrawingflag = idrawingflag | POINTSET_DRAW_POINTSET;
			idrawingflag = idrawingflag | POINTSET_DRAW_POINTSET_AS_ELLIPSE;
		}

		//draw all points or a subrect
		CRect myRect = pDoc->m_pPointset->GetBoundingRect();
		CRect myViewRect = myRect;
		int idroi = -1;
		if(pDoc->m_pROIset)
		{
			idroi = pDoc->m_pROIset->GetSelectedROI();
			myViewRect = pDoc->m_pROIset->GetRectROI(idroi);
		}
		if(idroi<4)
		{
			//draw all points
			pDoc->m_pPointset->DrawPointset(this, pDC, POINTSET_ALL, idrawingflag);
		}
		else
		{
			//draw all points within a sub rectangle (i.e. view rect)
			pDoc->m_pPointset->DrawPointset(this, pDC, &myViewRect, idrawingflag);
		}

	  	pDC->SelectObject(pOldBrush); 
		pDC->SelectObject(pOldPen);
		if(pPen) delete pPen;
		pDC->SetTextColor(crOldTextColor);
		pDC->SetBkMode(nOldBackgroundMode);		 
		//poirier, sept 2001, end
	}//end of m_bDrawPoint


	//poirier, oct 2001, debugging code, begin
	BOOL bForceDraw = FALSE;
	int idroi = -1;
	if(pDoc->m_pROIset)
	{
		idroi = pDoc->m_pROIset->GetSelectedROI();
	}
	if(idroi>3 && pDoc->m_pPointset!=NULL) bForceDraw = TRUE;
	//poirier, oct 2001, debugging code, end

	if( (m_pViewDataSettings->bDrawTriangleset || m_pViewDataSettings->bDrawTrianglesetLabels
		|| bForceDraw) && pDoc->m_pPointset!=NULL)
	{
		//green thin pen
		CPen* pPen = new CPen;
		CPen* pOldPen;
		int nWidth=OW2VIEW_PENSIZE;
		//if(pDC->IsPrinting()) nWidth=OW2VIEW_PENSIZE_PRINTING;
		COLORREF crTRIANGLESETCOLOR = OW2VIEW_TRIANGLESET_COLOR;
		//if(pDC->IsPrinting()) crTRIANGLESETCOLOR = RGB(255, 255, 255); //white when printing
		if(pDC->IsPrinting()) crTRIANGLESETCOLOR = RGB(0, 0, 0); //black when printing
		pPen->CreatePen(PS_SOLID, nWidth, crTRIANGLESETCOLOR);
    	pOldPen = pDC->SelectObject(pPen);
		//no brush
    	CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH); 
		//text color
		COLORREF crOldTextColor = pDC->SetTextColor(crTRIANGLESETCOLOR);
		int nOldBackgroundMode = pDC->SetBkMode(TRANSPARENT);


		int idrawingflag = 0;
		if(m_pViewDataSettings->bDrawTrianglesetLabels) idrawingflag = idrawingflag | POINTSET_DRAW_TRIANGLESET_LABEL;
		if(m_pViewDataSettings->bDrawTriangleset || bForceDraw) idrawingflag = idrawingflag | POINTSET_DRAW_TRIANGLESET;

		//draw all triangles
		CRect myRect = pDoc->m_pPointset->GetBoundingRect();
		CRect myViewRect = myRect;
		int idroi = -1;
		if(pDoc->m_pROIset)
		{
			idroi = pDoc->m_pROIset->GetSelectedROI();
			myViewRect = pDoc->m_pROIset->GetRectROI(idroi);
		}
		if(idroi<4)
		{
			/*
			if(this->IsKindOf(RUNTIME_CLASS(COW2ZoomedView)))
			{
				RECT myRECT;
				if(pDC->GetClipBox(&myRECT)!=NULLREGION)
				{
					pDoc->m_pPointset->DrawTriangleset(this, pDC, &myRECT, idrawingflag);
				}
			}
			else
			{
				pDoc->m_pPointset->DrawTriangleset(this, pDC, POINTSET_ALL, idrawingflag);
			}
			*/
			pDoc->m_pPointset->DrawTriangleset(this, pDC, POINTSET_ALL, idrawingflag);			
		}
		else
		{
			//draw all triangles within a sub rectangle (i.e. view rect)
			pDoc->m_pPointset->DrawTriangleset(this, pDC, &myViewRect, idrawingflag);
		}

		//for some triangle, color interior (floodfill)
		CBrush* pMyOldBrush = NULL;
		CBrush myBrush1;
		myBrush1.CreateSolidBrush(RGB(0,150,0));
		CBrush myBrush2;
		myBrush2.CreateSolidBrush(RGB(0,180,0));
		CBrush myBrush3;
		myBrush3.CreateSolidBrush(RGB(210,0,0));

		POSITION pos = pDoc->m_pPointset->m_pFloodFill_Pasted_IDTRIList.GetHeadPosition();
		POSITION pos2 = pDoc->m_pPointset->m_pFloodFill_Pasted_nIDTRIList.GetHeadPosition();
		while(pos2)
		{
			int ntri = (int)pDoc->m_pPointset->m_pFloodFill_nIDTRIList.GetNext(pos2);

			if(ntri==1) pMyOldBrush = pDC->SelectObject(&myBrush3); 
			else if(pMyOldBrush!=&myBrush1 && pMyOldBrush!=&myBrush2) 
			{
				pOldBrush = pDC->SelectObject(&myBrush1);
			}
			else if(pMyOldBrush==&myBrush1)
			{
				pOldBrush = pDC->SelectObject(&myBrush2);
			}
			else if(pMyOldBrush==&myBrush2)
			{
				pOldBrush = pDC->SelectObject(&myBrush1);
			}
			
			//POSITION pos = pDoc->m_pPointset->m_pFloodFill_IDTRIList.GetHeadPosition();
			int ncount = 0;
			while(pos && ncount!=ntri)
			{
				int idtri = (int)pDoc->m_pPointset->m_pFloodFill_Pasted_IDTRIList.GetNext(pos);
				//draw idtri
				POINT pPOINT[3];
				pDoc->m_pPointset->GetTrianglePOINTS(idtri, pPOINT, this);
				pDC->MoveTo(pPOINT[0].x, pPOINT[0].y);
				pDC->LineTo(pPOINT[1].x, pPOINT[1].y);
				pDC->LineTo(pPOINT[2].x, pPOINT[2].y);
				pDC->LineTo(pPOINT[0].x, pPOINT[0].y);
				//fill idtri
				POINT P = pDoc->m_pPointset->GetTriangleGeoCenter(idtri, this);
				pDC->FloodFill(P.x, P.y, OW2VIEW_TRIANGLESET_COLOR);
				ncount++;
			}
		}

		pos = pDoc->m_pPointset->m_pFloodFill_IDTRIList.GetHeadPosition();
		pos2 = pDoc->m_pPointset->m_pFloodFill_nIDTRIList.GetHeadPosition();
		while(pos2)
		{
			int ntri = (int)pDoc->m_pPointset->m_pFloodFill_nIDTRIList.GetNext(pos2);

			if(ntri==1) pMyOldBrush = pDC->SelectObject(&myBrush3); 
			else if(pMyOldBrush!=&myBrush1 && pMyOldBrush!=&myBrush2) 
			{
				pOldBrush = pDC->SelectObject(&myBrush1);
			}
			else if(pMyOldBrush==&myBrush1)
			{
				pOldBrush = pDC->SelectObject(&myBrush2);
			}
			else if(pMyOldBrush==&myBrush2)
			{
				pOldBrush = pDC->SelectObject(&myBrush1);
			}
			
			//POSITION pos = pDoc->m_pPointset->m_pFloodFill_IDTRIList.GetHeadPosition();
			int ncount = 0;
			while(pos && ncount!=ntri)
			{
				int idtri = (int)pDoc->m_pPointset->m_pFloodFill_IDTRIList.GetNext(pos);
				//draw idtri
				POINT pPOINT[3];
				pDoc->m_pPointset->GetTrianglePOINTS(idtri, pPOINT, this);
				pDC->MoveTo(pPOINT[0].x, pPOINT[0].y);
				pDC->LineTo(pPOINT[1].x, pPOINT[1].y);
				pDC->LineTo(pPOINT[2].x, pPOINT[2].y);
				pDC->LineTo(pPOINT[0].x, pPOINT[0].y);
				//fill idtri
				POINT P = pDoc->m_pPointset->GetTriangleGeoCenter(idtri, this);
				pDC->FloodFill(P.x, P.y, OW2VIEW_TRIANGLESET_COLOR);
				ncount++;
			}
		}

		//draw some triangle red
		pDC->SelectObject(pOldPen);
		if(pPen) delete pPen;
		CPen myPen;
		myPen.CreatePen(PS_SOLID, OW2VIEW_PENSIZE, RGB(255,0,0));
    	pOldPen = pDC->SelectObject(&myPen);

		pos = pDoc->m_pPointset->m_pDropped_IDTRIList.GetHeadPosition();
		while(pos)
		{
			int idtri = (int)pDoc->m_pPointset->m_pDropped_IDTRIList.GetNext(pos);
			POINT pPOINT[3];
			pDoc->m_pPointset->GetTrianglePOINTS(idtri, pPOINT, this);
			pDC->MoveTo(pPOINT[0].x, pPOINT[0].y);
			pDC->LineTo(pPOINT[1].x, pPOINT[1].y);
			pDC->LineTo(pPOINT[2].x, pPOINT[2].y);
			pDC->LineTo(pPOINT[0].x, pPOINT[0].y);
		}

	  	pDC->SelectObject(pOldBrush); 
		pDC->SelectObject(pOldPen);
		//if(pPen) delete pPen;
		pDC->SetTextColor(crOldTextColor);
		pDC->SetBkMode(nOldBackgroundMode);
	}





	if(m_pViewDataSettings->bDrawVoronoiset && pDoc->m_pPointset!=NULL)
	{
		//red thin pen
		CPen* pPen = new CPen;
		CPen* pOldPen;
		pPen->CreatePen(PS_SOLID, OW2VIEW_PENSIZE, RGB(255, 0, 0));
    	pOldPen = pDC->SelectObject(pPen);
		//no brush
    	CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH); 


		//draw all voronoi regions
		CRect myRect = pDoc->m_pPointset->GetBoundingRect();
		CRect myViewRect = myRect;
		if(pDoc->m_pROIset)
		{
			int idroi = pDoc->m_pROIset->GetSelectedROI();
			myViewRect = pDoc->m_pROIset->GetRectROI(idroi);
		}
		if(myViewRect==myRect)
		{
			pDoc->m_pPointset->DrawVoronoiset(this, pDC, POINTSET_ALL);
		}
		else
		{
			//draw all triangles within a sub rectangle (i.e. view rect)
			pDoc->m_pPointset->DrawVoronoiset(this, pDC, POINTSET_ALL, &myViewRect);
		}

	  	pDC->SelectObject(pOldBrush); 
		pDC->SelectObject(pOldPen);
		if(pPen) delete pPen;
	}




	if(m_pViewDataSettings->bDrawSegmentset && pDoc->m_pPointset!=NULL && pDoc->m_pSegmentset!=NULL)
	{
		if(m_iNumberOfSegmentForFinalPartition<0)
		{
			//default value, initial number of segment
			m_iNumberOfSegmentForFinalPartition = pDoc->m_pSegmentset->iInitialNumberOfSegment;
		}
		int iColor = 0;
		DrawSegmentPolygonsPartition(	pDC,
										pDoc->m_pPointset->m_pPOINTSET, 
										pDoc->m_pSegmentset,
										m_iNumberOfSegmentForFinalPartition);
	}

	if(p_oldfont) pDC->SelectObject(p_oldfont);
	m_bRendered = TRUE;
	m_bBusyPainting = FALSE;

	//poirier, begin, selection rect, imported and adapted from OISS
	if(!m_PrevSelectionRect.IsRectEmpty()) EraseSelectionRect(pDC,&m_PrevSelectionRect);	 //Erase
	m_PrevSelectionRect.SetRectEmpty();
	//poirier, end, selection rect, imported and adapted from OISS

	//poirier, sept 2001, begin
	if(pDoc->m_pROIset)
	{
		//red thin pen
		CPen* pPen = new CPen;
		CPen* pOldPen;
		pPen->CreatePen(PS_SOLID, OW2VIEW_PENSIZE, RGB(255, 0, 0));
    	pOldPen = pDC->SelectObject(pPen);
		//no brush
    	CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH); 

		//draw all user roi
		pDoc->m_pROIset->DrawUserROI(this, pDC, ROISET_ALL);
		//draw selected user roi
		int idroi = pDoc->m_pROIset->GetSelectedROI();
		if(idroi>3)
		{
			pDoc->m_pROIset->DrawUserROI(this, pDC, idroi, ROISET_DRAWAS_SELECTED);
		}
		//undo pen selection and allocation
	  	pDC->SelectObject(pOldBrush); 
		pDC->SelectObject(pOldPen);
		if(pPen) delete pPen;
	}
	//poirier, sept 2001, end

	return;
}

int COW2View::LookUpKeyInIntList(int* pIntList, int isizelist, int iKey)
{
	int index = -1;
	do
	{
		index++;
		if(index==isizelist)
		{
			//break
			return -1;
		}
	} while(pIntList[index]!=iKey);
	return index;
}


//////////////////////////////////////////////  
//draw_segment_polygons_partition()
//
//display a partition, creating a polygon
//for each present segment (since microstation
//polygon are limited to 101 vertices, we 
//create many polylines instead).
//////////////////////////////////////////////
void COW2View::DrawSegmentPolygonsPartition(CDC* pDC,
											POINTSET* pPointset, 
											SEGMENTSET* pSegmentset,
											int iNumberOfSegmentForFinalPartition,
											int iDrawMode/*=-1*/)
{                             
	CLogDocument* pLogDocument = NULL;
	char pszBuffer[1024];
	if(m_parentHWND==NULL) //spi 2014
	{
		pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	
		#ifdef _DEBUG
			wsprintfA(pszBuffer, "Will now create partition based on hierarchical segmentation tree\r\n", iNumberOfSegmentForFinalPartition);
			pLogDocument->AddText(pszBuffer);
		#endif //_DEBUG 
	}
	//MSElement LineElement;
    //Dpoint3d pPointList[105];
    //int Color, iBaseLevel, iLevel;
    
    //Dpoint3d 	Pnt[10];
    //MSElement	TextElement;
    //char text[100];
    //float sizex, sizey;
    //TextSizeParam txSize;

    int i;
    //int idseg, idowner, idsegFinal, idsegStart, idsegEnd;    
    int ivertex;
    
    int* pidsegList;
    int* pivertexList;



    //Color = 0; //0 is white in SVGA 16 colors mode 
    
    //sizex = 1; // 0.2 
    //sizey = 1.5; // 0.3 
    //MasterToUors_distance(&sizex);
    //MasterToUors_distance(&sizey);
    //txSize.mode = TXT_BY_TILE_SIZE; // TXT_BY_TILE_SIZE | TXT_NO_TRANSFORM 
    //txSize.size.width = sizex;
    //txSize.size.height = sizey;    
    
    //0.1) validate iNumberOfSegmentForFinalPartition 
    if(iNumberOfSegmentForFinalPartition<1 || iNumberOfSegmentForFinalPartition>pPointset->npts)
    {          
		if(m_parentHWND==NULL) //spi 2014
		{
			wsprintfA(pszBuffer, "     invalid iNumberOfSegmentForFinalPartition (%i) in view.mc, draw_segment_labels_partition()\r\n", iNumberOfSegmentForFinalPartition);
			pLogDocument->AddText(pszBuffer);
		}
		ASSERT(FALSE);
    	return;
    } 
    
	/*
    //0.2) validate iOffsetLevel 
    iBaseLevel = 30;    
    iLevel = iBaseLevel+iOffsetLevel;
    if(iLevel<iBaseLevel || iLevel>63)
    {          
    	printf("invalid iOffsetLevel (%i) in view.mc, draw_segment_polygons_partition()\n", iOffsetLevel);
    	return;
    }
    */
     
    //1) collect the id of the segment present in the final partition  
    pidsegList = (int*) malloc(iNumberOfSegmentForFinalPartition*sizeof(int));
    pivertexList = (int*) malloc(iNumberOfSegmentForFinalPartition*sizeof(int));
    if(pidsegList==NULL || pivertexList==NULL)
    {
		if(m_parentHWND==NULL) //spi 2014
		{
    		pLogDocument->AddText("problem allocating pidsegList or pivertexList in view.mc\r\n");
			ASSERT(FALSE);
		}
    	return;
    } 
	if(!GetRemainingSegmentListForFinalPartition( pSegmentset, pidsegList, iNumberOfSegmentForFinalPartition ))
	  return; //error, abort attempt to draw

    //2) get one vertex corresponding to each final segment 
    for(i=0; i<iNumberOfSegmentForFinalPartition; i++)
    {
		ivertex = GetOneVertexFromSegment(pSegmentset, pidsegList[i]);
		pivertexList[i] = ivertex;
    }
    
 	CPen pen;
	CPen* pOldPen;
	COLORREF crSEGMENTSETCOLOR, crOldTextColor;
	CBrush* pOldBrush;
	int nOldBackgroundMode;
	//3.1) create gdi objects
	//white thin pen
	crSEGMENTSETCOLOR = OW2VIEW_SEGMENTSET_COLOR;
	if(pDC->IsPrinting())
	{
		crSEGMENTSETCOLOR = RGB(0, 0, 0); //black when printing
	}
	pen.CreatePen(PS_SOLID, OW2VIEW_PENSIZE, crSEGMENTSETCOLOR);
	pOldPen = pDC->SelectObject(&pen);
	//no brush
	pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH); 
	//text color
	crOldTextColor = pDC->SetTextColor(crSEGMENTSETCOLOR);
	nOldBackgroundMode = pDC->SetBkMode(TRANSPARENT);

   //3.2) for each final segment ... draw a polygon 
    for(i=0; i<iNumberOfSegmentForFinalPartition; i++)
    {   
		DrawSegmentPolygon(	pDC, 
							pPointset,
							pSegmentset,
							pidsegList[i],	//to be drawn
							pivertexList[i],//any one vertex inside the segment to be drawn
							iNumberOfSegmentForFinalPartition,
							FALSE);	//to speed up, pen has already been selected
	}
	//3.3) select oldpen and modes
  	pDC->SelectObject(pOldBrush); 
	pDC->SelectObject(pOldPen);
	pDC->SetTextColor(crOldTextColor);
	pDC->SetBkMode(nOldBackgroundMode);		 

    //4) freeing the previously allocated list 
    if(pidsegList) free(pidsegList); 
    if(pivertexList) free(pivertexList);
    return;
}    

//provided idsegment and idvertex, any one vertex in the segment,
//this function finds one segment edge and start drawing polygone
void COW2View::DrawSegmentPolygon(CDC* pDC,
								  POINTSET* pPointset,
								  SEGMENTSET* pSegmentset,
								  int idsegment, //to be drawn
								  int idvertex,	 //any one vertex inside the segment to be drawn
								  int iNumberOfSegmentForFinalPartition,
								  BOOL bSelectPen/*=TRUE*/,
								  int iDrawMode/*=-1*/)
{
	POINT2DD pPoint2DD; 
	POINT pPointList[2005];

    EDGE myEdge, StartEdge, myNextEdge;
	CPen pen;
	CPen* pOldPen;
	COLORREF crSEGMENTSETCOLOR, crOldTextColor;
	CBrush* pOldBrush;
	int nOldBackgroundMode;
	int nOldDrawingMode;

	int j;
	CLogDocument* pLogDocument = NULL;
	if(m_parentHWND==NULL) //spi 2014
	{
		pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	}
	if(iDrawMode==R2_NOTXORPEN)
	{
		// Save the device context for modes
		pDC->SaveDC();
		//R2_NOTMASKPEN is interesting used with mouse left down button, when user merges manualy the segment
		nOldDrawingMode = pDC->SetROP2(R2_NOTXORPEN); //R2_NOTMASKPEN, //R2_NOTXORPEN //XOR mode, always have to erase what's drawn.
    
		pen.CreatePen(PS_SOLID,  0, RGB(0, 0, 0)); // 0 width = 1 device unit
		pOldPen = pDC->SelectObject(&pen);
	}
	else if(bSelectPen)
	{
		//2.5) create gdi objects
		//white thin pen
		crSEGMENTSETCOLOR = OW2VIEW_SEGMENTSET_COLOR;
		if(pDC->IsPrinting())
		{
			crSEGMENTSETCOLOR = RGB(0, 0, 0); //black when printing
		}
		pen.CreatePen(PS_SOLID, OW2VIEW_PENSIZE, crSEGMENTSETCOLOR);
		pOldPen = pDC->SelectObject(&pen);
		//no brush
		pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH); 
		//text color
		crOldTextColor = pDC->SetTextColor(crSEGMENTSETCOLOR);
		nOldBackgroundMode = pDC->SetBkMode(TRANSPARENT);
	}

	//3.1) find one segment edge, an edge is a pair of vertices, each vertices part of a diff. segment 
	GetOneSegmentEdge(	pPointset, 
						pSegmentset, 
						idsegment, 
						idvertex, 
						iNumberOfSegmentForFinalPartition,
						&myEdge);  
	if(myEdge.ivertexIn == -1 && myEdge.ivertexOut == -1)
	{   
		//development-time error 
		if(m_parentHWND==NULL) //spi 2014
		{
			CString myText;
			myText.Format(L"     error, edge was not found for idseg %i in ow2view.cpp\r\n", idsegment);
 			pLogDocument->AddTextW(myText);
		}
		ASSERT(FALSE);
		return;
	}
	else if(myEdge.ivertexIn != -1 && myEdge.ivertexOut == -1)
	{    
		//case for bounding segment, should only append once 
		
		//why do we get in here so often when partition with initial number of segment (maxnumseg)?
		//try to avoid this specific case
		//
		//I think, I got the answer, all the outbound points have voronoi regions that cannot be drawn,
		//and the first segments to be merge are all these points which are tagged by some float -9999.999 
		//value in order to cluster them all together early in the segmentation stage.
		if(iNumberOfSegmentForFinalPartition != pSegmentset->iInitialNumberOfSegment)
		{
			//for all other number of desired segment, trace 
			#ifdef _DEBUG
				if(m_parentHWND==NULL) //spi 2014
				{
    				pLogDocument->AddText("     got bounding segment, skip drawing contour for this one\r\n");
				}
			#endif //_DEBUG
		}
	}
	else 
	{
		//3.2) start drawing in microstation, at every 100 edge points draw a polyline (ms limitation) 
		StartEdge = myEdge;  //copy structure 
		pPoint2DD.x = pPointset->ctx[StartEdge.itriDualTriCCW1];
		pPoint2DD.y = pPointset->cty[StartEdge.itriDualTriCCW1];
		pPointList[0] = MapPointFromWorldToView( pPoint2DD);
		//pPointList[0].z = 0.0;
		//MasterToUors_point(&pPointList[0]);    		
		pPoint2DD.x = pPointset->ctx[StartEdge.itriDualTriCCW2];
		pPoint2DD.y = pPointset->cty[StartEdge.itriDualTriCCW2];
		pPointList[1] = MapPointFromWorldToView( pPoint2DD);
		//pPointList[1].z = 0.0;
		//MasterToUors_point(&pPointList[1]);    		
		GetNextSegmentEdge( pPointset, 
							pSegmentset, 
							idsegment, 
							&myEdge, 
							iNumberOfSegmentForFinalPartition,
							&myNextEdge);
		j=2;
		int iCount = 0;    		
		while(myNextEdge.itriDualTriCCW2!=StartEdge.itriDualTriCCW2)
		{
			iCount++;
			pPoint2DD.x = pPointset->ctx[myNextEdge.itriDualTriCCW2];
			pPoint2DD.y = pPointset->cty[myNextEdge.itriDualTriCCW2];
			pPointList[j] = MapPointFromWorldToView( pPoint2DD);
			//pPointList[j].z = 0.0;
			//MasterToUors_point(&pPointList[j]);
			//poirier, march 27 1997
			//if(j<100)	//limitation of microstation on polyline creation
			//if(j<1000)
			if(j<2000) //there is no limitation in windows, except for allocation of pPointList
			{
				j++;                               
				myEdge = myNextEdge; //copy structure 
				GetNextSegmentEdge( pPointset, 
									pSegmentset, 
									idsegment, 
									&myEdge, 
									iNumberOfSegmentForFinalPartition,
									&myNextEdge);
			}
			else
			{
				// we got 100 points but segment is not fully surrounded yet 
				
				//3.2.1) draw the polyline in microstation 
				/*
				mdlLineString_create ( &LineElement, NULL, pPointList, j);
				mdlElement_setSymbology ( &LineElement, &Color, NULL, NULL );
				mdlElement_setProperties (&LineElement, &iLevel, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
				mdlElement_add ( &LineElement );
				mdlElement_display ( &LineElement, NORMALDRAW );
				*/
				pDC->Polyline(pPointList, j);
				//3.2.2) reset j index to zero and keep going 
				j=0;
			}
			//to avoid getting stock in here, in cases where return Edge not valid
			if(iCount>pSegmentset->iInitialNumberOfSegment)
			{
				//development-time error, we get in trouble when generating final partition with 5 segments
				ASSERT(FALSE);
				break;		
			}							
		} //end of while 
		//3.3) draw the full polyline or the rest of the polyline in microstation 
		if(j>2)
		{
			pDC->Polyline(pPointList, j);
			if(m_pViewDataSettings->bDrawSegmentsetLabels)
			{
				//option 1)
				//get this segment's bounding box
				//get the bounding box center
				//verify that center is inside segment
				//if not inside segment, go up north and find first segment edge

				//option 2), the easiest for now
				//find the gravity center 
				//(le baricentre, calcule avec points du perimetre seulement,
				// n'est pas garanti de tomber a l'interieur du polygone)
				int iCenterX, iCenterY;
				double dfSumX = 0.0;
				double dfSumY = 0.0;
				for(int jj=0; jj<j; jj++)
				{
					dfSumX += (double)pPointList[jj].x;
					dfSumY += (double)pPointList[jj].y;
				}
				iCenterX = (int)(dfSumX/(double)j + 0.5);
				iCenterY = (int)(dfSumY/(double)j + 0.5);

 				//dessiner le numero du triangle
				WCHAR text[50];
				swprintf_s(text, 50,L"%d", idsegment);
				int ncount = wcslen(text);
				pDC->TextOut( iCenterX, iCenterY, text, ncount);

				//option 3), the ideal
				//(le centre de gravite de la plaque polygonale, calcule avec les points de l'interieur,
				// est garanti d'etre a l'interieur du polygone)

			}
		}
		else if(j=1)
		{
			pDC->MoveTo(pPointList[0]);
			pDC->LineTo(pPointList[1]);
		}
	}//end of for(each segment)	    	

	//free gdi objects
	if(iDrawMode==R2_NOTXORPEN)
	{
		pDC->SelectObject(pOldPen);
		pDC->SetROP2(nOldDrawingMode); 
		pDC->RestoreDC(-1);
	}
	else if(bSelectPen)
	{
  		pDC->SelectObject(pOldBrush); 
		pDC->SelectObject(pOldPen);
		pDC->SetTextColor(crOldTextColor);
		pDC->SetBkMode(nOldBackgroundMode);		 
	}
	return;
}

//partial partition drawing update
void COW2View::UpdatePartition(int iPrevNumSeg, int iNumberOfSegmentForFinalPartition)
{
	ASSERT(iNumberOfSegmentForFinalPartition==iPrevNumSeg-1);
	COW2Doc* pDoc = GetDocument();
	CClientDC dc(this);
	OnPrepareDC(&dc);  // set up mapping mode and viewport origin
	//dc.DPtoLP(&point);
	int idsegment1;
	int idsegment2;
	GetLastSegmentPairMerged(	pDoc->m_pSegmentset, 
								iNumberOfSegmentForFinalPartition, 
								&idsegment1,
								&idsegment2);

	#ifdef _DEBUG
		int idvertex1 = GetOneVertexFromSegment(pDoc->m_pSegmentset, idsegment1);
		int idvertex2 = GetOneVertexFromSegment(pDoc->m_pSegmentset, idsegment2);
		DrawSegmentPolygon(&dc,
						  pDoc->m_pPointset->m_pPOINTSET,
						  pDoc->m_pSegmentset,
						  idsegment1, //to be drawn
						  idvertex1,	 //any one vertex inside the segment to be drawn
						  iPrevNumSeg);
		DrawSegmentPolygon(&dc,
						  pDoc->m_pPointset->m_pPOINTSET,
						  pDoc->m_pSegmentset,
						  idsegment2, //to be drawn
						  idvertex2,	 //any one vertex inside the segment to be drawn
						  iPrevNumSeg);
	#endif
	DrawInterSegmentFrontier(	&dc,
								pDoc->m_pPointset->m_pPOINTSET,
								pDoc->m_pSegmentset,
								idsegment1, //frontier is between idsegment1 and 2
								idsegment2, 
								iPrevNumSeg, //supply the previous number of segment
								TRUE,
								-1);
	return;
}

//provided idsegment1, idsegment2 and idvertex, any one vertex in the segment,
//this function finds the frontier between the segments and will draw it
//it is usefull for erasing frontier between two segments being merge, to update partition
//visualization
void COW2View::DrawInterSegmentFrontier(CDC* pDC,
										POINTSET* pPointset,
										SEGMENTSET* pSegmentset,
										int idsegment1, //frontier is between idsegment1 and 2
										int idsegment2,
										int iNumberOfSegmentForFinalPartition,
										BOOL bSelectPen/*=TRUE*/,
										int iDrawMode/*=-1*/)
{
    EDGE myEdge, myNextEdge, StartSearchEdge;
	CPen pen;
	CPen* pOldPen;
	COLORREF crSEGMENTSETCOLOR, crOldTextColor;
	CBrush* pOldBrush;
	int nOldBackgroundMode;
	int nOldDrawingMode;

	CLogDocument* pLogDocument = NULL;
	if(m_parentHWND==NULL) //spi 2014
	{
		pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	}
	if(iDrawMode==R2_NOTXORPEN)
	{
		// Save the device context for modes
		pDC->SaveDC();
		nOldDrawingMode = pDC->SetROP2(R2_NOTMASKPEN); //R2_NOTXORPEN //XOR mode, always have to erase what's drawn.
    
		pen.CreatePen(PS_SOLID,  0, RGB(0, 0, 0)); // 0 width = 1 device unit
		pOldPen = pDC->SelectObject(&pen);
	}
	else if(bSelectPen)
	{
		//2.5) create gdi objects
		//white thin pen
		crSEGMENTSETCOLOR = RGB(255,0,0); //OW2VIEW_SEGMENTSET_COLOR;
		if(pDC->IsPrinting())
		{
			crSEGMENTSETCOLOR = RGB(0, 0, 0); //black when printing
		}
		pen.CreatePen(PS_SOLID, OW2VIEW_PENSIZE, crSEGMENTSETCOLOR);
		pOldPen = pDC->SelectObject(&pen);
		//no brush
		pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH); 
		//text color
		crOldTextColor = pDC->SetTextColor(crSEGMENTSETCOLOR);
		nOldBackgroundMode = pDC->SetBkMode(TRANSPARENT);
	}

	//3.1) find one segment edge, an edge is a pair of vertices, each vertices part of a diff. segment 
	int idvertex = GetOneVertexFromSegment(pSegmentset, idsegment1);

	GetOneSegmentEdge(	pPointset, 
						pSegmentset, 
						idsegment1, 
						idvertex, 
						iNumberOfSegmentForFinalPartition,
						&myEdge);
	
	BOOL bCleanUpAndExit = FALSE;
	if(myEdge.ivertexIn == -1 && myEdge.ivertexOut == -1)
	{   
		//development-time error 
		if(m_parentHWND==NULL) //spi 2014
		{
			CString myText;
			myText.Format(L"     error, edge was not found for idseg %i in ow2view.cpp\r\n", idsegment1);
 			pLogDocument->AddTextW(myText);
		}
		ASSERT(FALSE);
		return;
	}
	else if(myEdge.ivertexIn != -1 && myEdge.ivertexOut == -1)
	{    
		//case for bounding segment, should only append once 
		
		//why do we get in here so often when partition with initial number of segment (maxnumseg)?
		//try to avoid this specific case
		//
		//I think, I got the answer, all the outbound points have voronoi regions that cannot be drawn,
		//and the first segments to be merge are all these points which are tagged by some float -9999.999 
		//value in order to cluster them all together early in the segmentation stage.
		if(iNumberOfSegmentForFinalPartition != pSegmentset->iInitialNumberOfSegment)
		{
			//for all other number of desired segment, trace 
			if(m_parentHWND==NULL) //spi 2014
			{
				#ifdef _DEBUG
    				pLogDocument->AddText("     got bounding segment, skip drawing contour for this one\r\n");
				#endif //_DEBUG
			}
		}
	}
	else 
	{
		//3.2) start searching for first edge found to belong to the frontier between idsegment1 and idsegment2
		StartSearchEdge = myEdge;
		int idebugcounter=0;
		while(!IsInterSegmentFrontierEdge(	pPointset, 
											pSegmentset, 
											idsegment1,
											idsegment2,
											iNumberOfSegmentForFinalPartition, 
											&myEdge))
		{
			idebugcounter++;
			GetNextSegmentEdge( pPointset, 
								pSegmentset, 
								idsegment1, 
								&myEdge, 
								iNumberOfSegmentForFinalPartition,
								&myNextEdge);
			if(myNextEdge.itriDualTriCCW2==StartSearchEdge.itriDualTriCCW2)
			{
				//end up on initial edge without having found a frontier, are idseg1 and idseg2 really adjacent segment (in partition iNumberOfSegmentForFinalPartition)?
				//note: this assertion fail in a few cases, after inspection, it seems two segments that are not adjacent
				//      are being merge, their neighbor list shows as if they were realy adjacent neighbors
				//      -solution, inspect segment history to find where things started to go wrong
				//      -maybe neighbor lists are sometimes not updated properly?
				ASSERT(FALSE);
				//return;
				bCleanUpAndExit = TRUE;
				break; //break the while loop
			}
			myEdge = myNextEdge;
		}
		if(!bCleanUpAndExit)
		{
			//3.3) start drawing frontier
			StartDrawingInterSegmentFrontierEdge(	pDC,
													pPointset,
													pSegmentset,
													idsegment1, //frontier is between idsegment1 and 2
													idsegment2,
													iNumberOfSegmentForFinalPartition,
													&myEdge, 
													StartSearchEdge);

			//4) if was stop, because looking counter clockwise and found end of frontier ...
			//   keep going counter clockwise to find if there is a remaining portion of the
			//   frontier to be drawn
			BOOL bBreakWhileLoop = FALSE;
			myNextEdge = myEdge;
			while(myNextEdge.itriDualTriCCW2!=StartSearchEdge.itriDualTriCCW2)
			{
				myEdge = myNextEdge;
				while(!IsInterSegmentFrontierEdge(	pPointset, 
													pSegmentset, 
													idsegment1,
													idsegment2,
													iNumberOfSegmentForFinalPartition, 
													&myEdge))
				{
					GetNextSegmentEdge( pPointset, 
										pSegmentset, 
										idsegment1, 
										&myEdge, 
										iNumberOfSegmentForFinalPartition,
										&myNextEdge);
					if(myNextEdge.itriDualTriCCW2==StartSearchEdge.itriDualTriCCW2)
					{
						//there was no remaining part of frontier to be drawn
						bBreakWhileLoop = TRUE;	//to break the other while loop
						break; //break this while loop
					}
					myEdge = myNextEdge;
				}
				if(bBreakWhileLoop) break;
				//ok, we are back on the frontier to be drawn
				StartDrawingInterSegmentFrontierEdge(	pDC,
														pPointset,
														pSegmentset,
														idsegment1, //frontier is between idsegment1 and 2
														idsegment2,
														iNumberOfSegmentForFinalPartition,
														&myEdge, 
														StartSearchEdge);
				myNextEdge = myEdge;

			}
		}//end if(!bCleanUpAndExit)
	}//end if	    	

	//free gdi objects
	if(iDrawMode==R2_NOTXORPEN)
	{
		pDC->SelectObject(pOldPen);
		pDC->SetROP2(nOldDrawingMode); 
		pDC->RestoreDC(-1);
	}
	else if(bSelectPen)
	{
  		pDC->SelectObject(pOldBrush); 
		pDC->SelectObject(pOldPen);
		pDC->SetTextColor(crOldTextColor);
		pDC->SetBkMode(nOldBackgroundMode);		 
	}
	return;
}


void COW2View::StartDrawingInterSegmentFrontierEdge(CDC* pDC,
													POINTSET* pPointset,
													SEGMENTSET* pSegmentset,
													int idsegment1, //frontier is between idsegment1 and 2
													int idsegment2,
													int iNumberOfSegmentForFinalPartition,
													EDGE* p_myEdge, //provide StartEdge as input, will return EndEdge on output
													EDGE StartSearchEdge)
{
	POINT2DD pPoint2DD; 
	POINT pPointList[2005];

	//3.3) start drawing edges that are frontier between idsegment1 and idsegment2
	EDGE StartEdge, myNextEdge;

	StartEdge = p_myEdge[0];  //copy structure 
	pPoint2DD.x = pPointset->ctx[StartEdge.itriDualTriCCW1];
	pPoint2DD.y = pPointset->cty[StartEdge.itriDualTriCCW1];
	pPointList[0] = MapPointFromWorldToView( pPoint2DD);
	//pPointList[0].z = 0.0;
	//MasterToUors_point(&pPointList[0]);    		
	pPoint2DD.x = pPointset->ctx[StartEdge.itriDualTriCCW2];
	pPoint2DD.y = pPointset->cty[StartEdge.itriDualTriCCW2];
	pPointList[1] = MapPointFromWorldToView( pPoint2DD);
	//pPointList[1].z = 0.0;
	//MasterToUors_point(&pPointList[1]);    		
	GetNextSegmentEdge( pPointset, 
						pSegmentset, 
						idsegment1, 
						p_myEdge, 
						iNumberOfSegmentForFinalPartition,
						&myNextEdge);
	int j=2;
	int iCount = 0;    		
	while(myNextEdge.itriDualTriCCW2!=StartSearchEdge.itriDualTriCCW2
		  && IsInterSegmentFrontierEdge(pPointset, 
										pSegmentset, 
										idsegment1,
										idsegment2,
										iNumberOfSegmentForFinalPartition, 
										&myNextEdge))
	{
		iCount++;
		pPoint2DD.x = pPointset->ctx[myNextEdge.itriDualTriCCW2];
		pPoint2DD.y = pPointset->cty[myNextEdge.itriDualTriCCW2];
		pPointList[j] = MapPointFromWorldToView( pPoint2DD);
		//pPointList[j].z = 0.0;
		//MasterToUors_point(&pPointList[j]);
		//poirier, march 27 1997
		//if(j<100)	//limitation of microstation on polyline creation
		//if(j<1000)
		if(j<2000) //there is no limitation in windows, except for allocation of pPointList
		{
			j++;                               
			p_myEdge[0] = myNextEdge; //copy structure 
			GetNextSegmentEdge( pPointset, 
								pSegmentset, 
								idsegment1, 
								p_myEdge, 
								iNumberOfSegmentForFinalPartition,
								&myNextEdge);
		}
		else
		{
			// we got 100 points but segment is not fully surrounded yet 
			
			//3.2.1) draw the polyline in microstation 
			/*
			mdlLineString_create ( &LineElement, NULL, pPointList, j);
			mdlElement_setSymbology ( &LineElement, &Color, NULL, NULL );
			mdlElement_setProperties (&LineElement, &iLevel, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
			mdlElement_add ( &LineElement );
			mdlElement_display ( &LineElement, NORMALDRAW );
			*/
			pDC->Polyline(pPointList, j);
			//3.2.2) reset j index to zero and keep going 
			j=0;
		}
		//to avoid getting stock in here, in cases where return Edge not valid
		if(iCount>pSegmentset->iInitialNumberOfSegment)
		{
			//development-time error, we get in trouble when generating final partition with 5 segments
			ASSERT(FALSE);
			break;		
		}							
	} //end of while 
	//3.3) draw the full polyline or the rest of the polyline in microstation 
	if(j>2)
	{
		pDC->Polyline(pPointList, j);
	}
	else if(j=1)
	{
		pDC->MoveTo(pPointList[0]);
		pDC->LineTo(pPointList[1]);
	}
	p_myEdge[0] = myNextEdge;
	return;
}


void COW2View::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	COW2Doc* pDoc = (COW2Doc*)GetDocument();
		
	// refresh DDB for eventual display
	if(pDoc->m_pPixelset)
	{
		if(pDoc->m_pPixelset->IsBIIPresent())
		{
			int ichannel_flag = 0;
			if(m_bViewPixelset_DisplayBlue) ichannel_flag = ichannel_flag | PIXELSET_BLUE;
			if(m_bViewPixelset_DisplayGreen) ichannel_flag = ichannel_flag | PIXELSET_GREEN;
			if(m_bViewPixelset_DisplayRed) ichannel_flag = ichannel_flag | PIXELSET_RED;
			//2012july22, poirier, begin
			//pDoc->m_pPixelset->UpdateIFFFromBII(ichannel_flag);
			pDoc->m_pPixelset->UpdateDIBFromBII(ichannel_flag);
			//2012july22, poirier, end
		}
		//2012july22, poirier, begin
		//int imghandle = pDoc->m_pPixelset->GetIFFHandle();
		//ASSERT(imghandle>-1); 
		//2012july22, poirier, end
		ASSERT(m_hDDB!=NULL);
		if(m_hDDB!=NULL) DeleteObject(m_hDDB);
		//2012july22, poirier, begin
		//m_hDDB = OW_IFF_IMG_dib_to_ddb(imghandle, m_WindowSize.cx, m_WindowSize.cy);
		m_hDDB = DIBtoDDBitmap((BYTE*) (pDoc->m_pPixelset->GetDIBPointer()));
		//2012july22, poirier, end

	}
	if(m_parentHWND==NULL) //spi 2014
	{
		//invalidate whole view
		Invalidate(FALSE);
	}
	else
	{
		::InvalidateRect(m_parentHWND, NULL, false);
	}
	return;
}



POINT COW2View::MapPointFromWorldToView( POINT2DD myPoint2DD)
{
	POINT myPoint;
	//invert y axis
	myPoint.x = (int)(myPoint2DD.x); //+0.5);
	myPoint.y = (int)(m_WorldRect.bottom - myPoint2DD.y); // +0.5);
	return myPoint;
}

POINT2DD COW2View::MapPointFromViewToWorld( POINT myPoint)
{
	POINT2DD myPoint2DD;
	//invert y axis
	myPoint2DD.x = (double)(myPoint.x);
	myPoint2DD.y = (double)(m_WorldRect.bottom - myPoint.y);
	return myPoint2DD;
}

RECT COW2View::MapRectFromWorldToView(RECT2DD myRECT2DD)
{
	RECT myRECT;
	//invert y axis
	myRECT.left = (int)(myRECT2DD.left); //+0.5);
	myRECT.right = (int)(myRECT2DD.right); //+0.5);
	myRECT.top = (int)(m_WorldRect.bottom - myRECT2DD.top); //+0.5);
	myRECT.bottom = (int)(m_WorldRect.bottom - myRECT2DD.bottom); //+0.5);
	return myRECT;
}

RECT2DD COW2View::MapRectFromViewToWorld( RECT myRECT)
{
	RECT2DD myRECT2DD;
	myRECT2DD.left = (double)(myRECT.left);
	myRECT2DD.right = (double)(myRECT.right);
	//invert y axis
	myRECT2DD.top = (double)(m_WorldRect.bottom - myRECT.top);
	myRECT2DD.bottom = (double)(m_WorldRect.bottom - myRECT.bottom);
	return myRECT2DD;
}

/* //spi 2014
#ifdef _DEBUG // non-debug version is inline
*/
	COW2Doc* COW2View::GetDocument() 
	{
		if(m_parentHWND==NULL) //spi 2014
		{
			ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COW2Doc)));
			return (COW2Doc*)m_pDocument;
		}
		else
		{
			//note: for the time being, only COW2Doc* can be used
			ASSERT(m_pCOWDoc->IsKindOf(RUNTIME_CLASS(COW2Doc)));
			return (COW2Doc*)m_pCOWDoc;
		}
	}
/* //spi 2014
#endif //_DEBUG
*/

BOOL COW2View::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

int COW2View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{

	if (COWView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	/*
	CDC* pDC = GetDC();
	m_MemoryDC.CreateCompatibleDC(pDC);
	ReleaseDC(pDC);
	*/

	return 0;
}


void COW2View::ZoomIn(RECT* pRECT)
{
	CRect myRect(pRECT);
	ZoomIn(myRect);
	//ZoomFactorChanged();
}

void COW2View::ZoomIn(CRect &rect)
{
	COWView::ZoomIn(rect);
	//ZoomFactorChanged();
}

void COW2View::ZoomIn(CPoint *point, double delta)
{
	COWView::ZoomIn(point, delta); 
	ZoomFactorChanged();
	return;
}
void COW2View::OnZoomIn()
{
	ZoomIn(); 	
	return;
}

//poirier, sept 2001, begin

void COW2View::OnZoomOut() 
{
    ZoomOut(); //COWView
}

void COW2View::OnZoomPercent() 
{
    //Grab the zoom menu
    CMenu * pPopupMenu =
	AfxGetApp()->m_pMainWnd->GetMenu()->GetSubMenu(3); 
    
    ASSERT(pPopupMenu != NULL);
    
    //Grab the percent menu
    CMenu * pSubMenu = pPopupMenu->GetSubMenu(2);
    if (pSubMenu == NULL) 
	{ //When maximized, things move!
		//That didn't work, so now try one over.
		pPopupMenu =
			AfxGetApp()->m_pMainWnd->GetMenu()->GetSubMenu(4); 
		
		ASSERT(pPopupMenu != NULL);
		
		//Grab the percent menu
		pSubMenu = pPopupMenu->GetSubMenu(2);
		ASSERT (pSubMenu !=NULL);
		TRACE0("Could not locate the percent submenu!!");
    }
    
    CPoint ptCurrent;
    ::GetCursorPos(&ptCurrent);
    
    if (pSubMenu)//See assertion above.
	pSubMenu->TrackPopupMenu(TPM_LEFTBUTTON|TPM_LEFTALIGN,
				 ptCurrent.x,ptCurrent.y,this,NULL);  
	
}

void COW2View::OnUpdateZoomPercent(CCmdUI* /*pCmdUI*/) 
{
}

void COW2View::OnZoomPointin() 
{
    m_owviewMode = ZOOMINPOINT;
    OnSetCursor(NULL,HTCLIENT,0);
}

void COW2View::OnUpdateZoomPointin(CCmdUI* /*pCmdUI*/) 
{
}

void COW2View::OnZoomPointout() 
{
    m_owviewMode = ZOOMOUTPOINT;
    OnSetCursor(NULL,HTCLIENT,0);
}

void COW2View::OnUpdateZoomPointout(CCmdUI* /*pCmdUI*/) 
{
}

void COW2View::OnZoomRectin() 
{
    m_owviewMode = ZOOMRECT;
    OnSetCursor(NULL,HTCLIENT,0);
}

void COW2View::OnUpdateZoomRectin(CCmdUI* /*pCmdUI*/) 
{
}

void COW2View::OnZoomZoomnormal() 
{
    if (GetZoomMode() != OW_ZOOMNORMAL)
	{ 
		SetZoomMode(OW_ZOOMNORMAL); 
		ZoomOriginal();  
    }
    Invalidate(TRUE);  //Force a redraw        
}

void COW2View::OnUpdateZoomZoomnormal(CCmdUI* pCmdUI) 
{
    if (GetZoomMode() == OW_ZOOMNORMAL) 
		pCmdUI->SetCheck(TRUE);
    else
		pCmdUI->SetCheck(FALSE);    
}

void COW2View::OnZoomZoomoff() 
{
    if (GetZoomMode() != OW_ZOOMOFF)
	{ 
		ZoomOriginal();  
		SetZoomMode(OW_ZOOMOFF); 
    }
    Invalidate(TRUE);  //Force a redraw        
}

void COW2View::OnUpdateZoomZoomoff(CCmdUI* pCmdUI) 
{
    if (GetZoomMode() == OW_ZOOMOFF) 
		pCmdUI->SetCheck(TRUE);
    else
		pCmdUI->SetCheck(FALSE);    	
}

void COW2View::OnZoomZoomtofit() 
{
    if (GetZoomMode() != OW_ZOOMFIT)
	{ 
		SetZoomMode(OW_ZOOMFIT); 
		ZoomFit();  
    }
    Invalidate(TRUE);  //Force a redraw    
}

void COW2View::OnUpdateZoomZoomtofit(CCmdUI* pCmdUI) 
{
    if (GetZoomMode() == OW_ZOOMFIT) 
		pCmdUI->SetCheck(TRUE);
    else
		pCmdUI->SetCheck(FALSE);    
}

void COW2View::OnZoomp10() 
{
	SetZoomMode(OW_ZOOMNORMAL); 
    ZoomPercent(10); //COWView
	OnZoomZoomnormal(); 
}

void COW2View::OnUpdateZoomp10(CCmdUI* /*pCmdUI*/) 
{
}

void COW2View::OnZoomp50() 
{
	SetZoomMode(OW_ZOOMNORMAL); 
    ZoomPercent(50); //COWView
	OnZoomZoomnormal(); 
}

void COW2View::OnUpdateZoomp50(CCmdUI* /*pCmdUI*/) 
{
}

void COW2View::OnZoomp75() 
{
	SetZoomMode(OW_ZOOMNORMAL); 
    ZoomPercent(75); //COWView
	OnZoomZoomnormal(); 
}

void COW2View::OnUpdateZoomp75(CCmdUI* /*pCmdUI*/) 
{
}

void COW2View::OnZoomp100() 
{
	SetZoomMode(OW_ZOOMNORMAL); 
    ZoomPercent(100); //COWView
	OnZoomZoomnormal(); 
}

void COW2View::OnUpdateZoomp100(CCmdUI* /*pCmdUI*/) 
{
}

void COW2View::OnZoomp150() 
{
	SetZoomMode(OW_ZOOMNORMAL); 
	ZoomPercent(150); //COWView
	OnZoomZoomnormal(); 
}

void COW2View::OnUpdateZoomp150(CCmdUI* /*pCmdUI*/) 
{
}

void COW2View::OnZoomp200() 
{
	SetZoomMode(OW_ZOOMNORMAL); 
	ZoomPercent(200); //COWView
	OnZoomZoomnormal(); 
}

void COW2View::OnUpdateZoomp200(CCmdUI* /*pCmdUI*/) 
{
}

void COW2View::OnZoomp400() 
{
	SetZoomMode(OW_ZOOMNORMAL); 
    ZoomPercent(400); //COWView
	OnZoomZoomnormal(); 
}

void COW2View::OnUpdateZoomp400(CCmdUI* /*pCmdUI*/) 
{
}

void COW2View::OnZoomp800() 
{
	SetZoomMode(OW_ZOOMNORMAL); 
    ZoomPercent(800); //COWView
	OnZoomZoomnormal(); 
}

void COW2View::OnUpdateZoomp800(CCmdUI* /*pCmdUI*/) 
{
}

void COW2View::OnComboEditUpdate()
{
}

void COW2View::OnComboEditChange()
{
	//poirier, begin, selection rect, imported and adapted from OISS
    CClientDC dc(this);
    OnPrepareDC(&dc);
	if(!m_PrevSelectionRect.IsRectEmpty()) EraseSelectionRect(&dc,&m_PrevSelectionRect);	 //Erase
	m_PrevSelectionRect.SetRectEmpty();
	//poirier, end, selection rect, imported and adapted from OISS	

	//return gets filtered out, useless code!
    CMainFrame * pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;    
	WCHAR pBuffer[100];
    pFrame->m_wndToolBar.m_comboBox.GetWindowText(pBuffer, 100);

	//char* pChar = strchr(pBuffer, '\r');
	//if (pChar == NULL) return;
	CStringA myStringA(pBuffer);
	int index=myStringA.Find('\r');
	if (index == -1) return;

	int nPercent = _wtol(pBuffer);
	double dPercent = (double) nPercent;

	double fZoomFactorMin, fZoomFactorMax;
	GetZoomFactorMinMax(&fZoomFactorMin, &fZoomFactorMax);
	if(dPercent<fZoomFactorMin) 
	{
		nPercent = (int) fZoomFactorMin;
	}
	else if(dPercent>fZoomFactorMax)
	{
		nPercent = (int) fZoomFactorMax;
	}

	SetZoomMode(OW_ZOOMNORMAL); 
    ZoomPercent(nPercent);
	OnZoomZoomnormal(); 
	return;
}

void COW2View::OnComboSelChange()
{
    CMainFrame * pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
    
    int nSelection = pFrame->m_wndToolBar.m_comboBox.GetCurSel();
    int nPercent = 100;
    
    switch (nSelection)
	{
		case 0:
		nPercent = 10;
		break;
		case 1:
		nPercent = 20;
		break;
		case 2:
		nPercent = 40;
		break;
		case 3:
		nPercent = 80;
		break;
		case 4:
		nPercent = 100;
		break;
		case 5:
		nPercent = 200;
		break;
		case 6:
		nPercent = 300;
		break;
		case 7:
		nPercent = 400;
		break;
		case 8:
		nPercent = 600;
		break;
		case 9:
		nPercent = 800;
		break;	
		case 10:
		nPercent = 1000;
		break;	
		case 11:
		nPercent = 1500;
		break;	
		case 12:
		nPercent = 2000;
		break;	
		case 13:
		nPercent = 2500;
		break;	
		case 14:
		nPercent = 3000;
		break;	
    }

	//poirier, begin, selection rect, imported and adapted from OISS
    CClientDC dc(this);
    OnPrepareDC(&dc);
	if(!m_PrevSelectionRect.IsRectEmpty()) EraseSelectionRect(&dc,&m_PrevSelectionRect);	 //Erase
	m_PrevSelectionRect.SetRectEmpty();
	//poirier, end, selection rect, imported and adapted from OISS	

	SetZoomMode(OW_ZOOMNORMAL); 
    ZoomPercent(nPercent);
	OnZoomZoomnormal(); 

}

void COW2View::ComboSetText(LPCTSTR text)
{
    CMainFrame * pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
    CComboBox * pCombo = &pFrame->m_wndToolBar.m_comboBox;

    pCombo->SetWindowText(text);
}

void COW2View::ZoomFactorChanged(void) //COW2View overriden virtual
{
    TCHAR buffer[56];
    
	if(m_parentHWND==NULL) //spi 2014
	{
		//_stprintf(buffer,_T("Scale: %.3f"),GetZoomFactor()); //SECZoomView
		swprintf_s(buffer,56,_T("Scale: %.3f"),GetZoomFactor()); //SECZoomView
		MyChangeStatusBar(3,buffer);
		//_stprintf(buffer,_T("Percent: %.3f"),GetZoomFactor() * 100.0f);
		swprintf_s(buffer,56,_T("Percent: %.3f"),GetZoomFactor() * 100.0f);
		MyChangeStatusBar(2,buffer);
		//_stprintf(buffer,_T("%.0f"),GetZoomFactor() * 100.0f);
		swprintf_s(buffer,56,_T("%.0f"),GetZoomFactor() * 100.0f);
		ComboSetText(buffer);
	}
	//poirier, sept 2001, begin
	if(GetDocument()->m_pROIset!=NULL)
	{
		//update roi rect coordinate
		UpdateROI(EDGESET_VIEWING_RECT_ROI_ID);
		//if this roi was selected
		if(GetDocument()->m_pROIset->GetSelectedROI()==EDGESET_VIEWING_RECT_ROI_ID)
		{
			//to trigger the graph update
			SelectROI(EDGESET_VIEWING_RECT_ROI_ID);
		}
	}
	//poirier, sept 2001, end

	/*
	//Have to call the SECPanView version to get overview window updated right.
    SECPanView::ZoomLevelChanged(); //SECPanView
	*/
}

void COW2View::MyChangeStatusBar(int nPane,LPCTSTR text)
{
    CWinApp* pApp = AfxGetApp() ;
    CMainFrame* pFrame = (CMainFrame*)pApp->m_pMainWnd ;
    ASSERT(pFrame->IsKindOf(RUNTIME_CLASS(CMainFrame))) ;
    pFrame->MyChangePane(nPane,text);  
}        

void COW2View::DPtoLP(CClientDC* pDC, CPoint* pPoint)
{
	//poirier, oct 2001, begin
	CPoint pt0(0,0);
	CPoint pt1(1,1);
	pDC->LPtoDP(&pt0);
	pDC->LPtoDP(&pt1);
	CSize devsizeOneLogicalUnit(pt1.x-pt0.x, pt1.y-pt0.y);
	pPoint->Offset((int)(-0.5*devsizeOneLogicalUnit.cx), (int)(-0.5*devsizeOneLogicalUnit.cy));
	if(pPoint->x<0) pPoint->x = 0;
	if(pPoint->y<0) pPoint->y = 0;
	//poirier, oct 2001, end
    pDC->DPtoLP(pPoint);
}

void COW2View::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(!m_bRendered || m_bBusyPainting==TRUE)
	{
		if(m_bMode_ROIFloating==TRUE)
		{
			//poirier, begin, selection rect, imported and adapted from OISS
			CClientDC dc(this);
			OnPrepareDC(&dc);
			if(!m_PrevSelectionRect.IsRectEmpty()) EraseSelectionRect(&dc,&m_PrevSelectionRect);	 //Erase
			m_PrevSelectionRect.SetRectEmpty();
			//poirier, end, selection rect, imported and adapted from OISS	
		}
		return;
	}

    //nFlags; //UNUSED
    
    CClientDC dc(this);
    OnPrepareDC(&dc);
    CPoint logPoint = point;
	this->DPtoLP(&dc, &logPoint);
	/*
    CClientDC dc(this);
    OnPrepareDC(&dc);
	//poirier, oct 2001, begin
	CPoint pt0(0,0);
	CPoint pt1(1,1);
	dc.LPtoDP(&pt0);
	dc.LPtoDP(&pt1);
	CSize devsizeOneLogicalUnit(pt1.x-pt0.x, pt1.y-pt0.y);
	logPoint.Offset((int)(-0.5*devsizeOneLogicalUnit.cx), (int)(-0.5*devsizeOneLogicalUnit.cy));
	if(logPoint.x<0) logPoint.x = 0;
	if(logPoint.y<0) logPoint.y = 0;
	//poirier, oct 2001, end
    dc.DPtoLP(&logPoint);
	*/

    TCHAR buf[256];
	//_stprintf(buf,_T("Log: (%d,%d) Dev: (%d,%d)"),
	swprintf_s(buf,256,_T("Log: (%d,%d) Dev: (%d,%d)"),
	    logPoint.x,logPoint.y,point.x,point.y);
    MyChangeStatusBar(1,buf);

    COW2Doc* pDoc = (COW2Doc*)GetDocument();
	CRect myWorldRect = pDoc->GetWorldRect();

    //If we're in rectangle mode, do rubber-banding
    if (m_owviewMode == ZOOMRECT && m_bCaptured)
	{
		DrawRubberRect(&dc,m_rectDrag);	 //Erase
		m_rectDrag.right = logPoint.x;
		m_rectDrag.bottom = logPoint.y;
		DrawRubberRect(&dc,m_rectDrag);	// Draw
    }
    else if (m_owviewMode == DRAWELLIPSE && m_bCaptured)
	{
		DrawRubberEllipse(&dc,m_rectEllipseDrag);	 //Erase
		/*
		#ifdef _DEBUG
		COW2Doc* pDoc = GetDocument();
		CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
		CString myString;
		myString.Format("DrawRubberEllipse(%d,%d,%d,%d);\r\n",m_rectEllipseDrag.left,m_rectEllipseDrag.top,m_rectEllipseDrag.right,m_rectEllipseDrag.bottom);
		pLogDocument->AddText(myString);
		#endif //_DEBUG
		*/
		m_rectEllipseDrag.right = logPoint.x;
		m_rectEllipseDrag.bottom = logPoint.y;
		DrawRubberEllipse(&dc,m_rectEllipseDrag);	// Draw
		/*
		#ifdef _DEBUG
		//COW2Doc* pDoc = GetDocument();
		//CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
		//CString myString;
		myString.Format("DrawRubberEllipse(%d,%d,%d,%d);\r\n",m_rectEllipseDrag.left,m_rectEllipseDrag.top,m_rectEllipseDrag.right,m_rectEllipseDrag.bottom);
		pLogDocument->AddText(myString);
		#endif //_DEBUG
		*/
    }
    else if (m_owviewMode==NORMAL && m_bMode_ROIRect && m_bCaptured)
	{
		DrawRubberRect(&dc,m_rectROIRectDrag);	 //Erase
		m_rectROIRectDrag.right = logPoint.x;
		m_rectROIRectDrag.bottom = logPoint.y;
		DrawRubberRect(&dc,m_rectROIRectDrag);	// Draw
    }
    else if(m_owviewMode==NORMAL && m_bMode_ROIPolyline && m_bCaptured)
	{
		DrawRubberLine(&dc,m_rectROIPolylineDrag);	 //Erase
		m_rectROIPolylineDrag.right = logPoint.x;
		m_rectROIPolylineDrag.bottom = logPoint.y;
		DrawRubberLine(&dc,m_rectROIPolylineDrag);	// Draw
    }
	else if(m_owviewMode==NORMAL && m_bMode_ROIFreehand && m_bCaptured)
	{
		//if mouse moving onto a new pixel ...
		if(logPoint.x!=m_rectROIFreehandDrag.left
			|| logPoint.y!=m_rectROIFreehandDrag.top)
		{
			//DrawRubberRect(&dc,m_rectROIFreehandDrag);	 //Erase
			m_rectROIFreehandDrag.right = logPoint.x;
			m_rectROIFreehandDrag.bottom = logPoint.y;
			/*
			DrawRubberRect(&dc,m_rectROIFreehandDrag);	// Draw
			*/
			
			//drop a new polyline roi
			COW2Doc* pDoc = GetDocument();
			if(pDoc!=NULL && pDoc->m_pROIset)
			{
				//1) add new roi at these coordinate
				if(m_idroiROIFreehandDrag==-1) 
				{
					m_idroiROIFreehandDrag = pDoc->m_pROIset->AddPolylinearROI(&m_rectROIFreehandDrag);
					/*
					CRect myRect(m_rectROIFreehandDrag);
					myRect.InflateRect(1,1);
					InvalidateRect(&myRect, FALSE);
					*/
					DrawRubberFillRect(&dc,m_rectROIFreehandDrag);	// Draw
				}
				else
				{
					pDoc->m_pROIset->AddLineToPolylinearROI(m_idroiROIFreehandDrag, &m_rectROIFreehandDrag);
					/*
					CRect myRect(m_rectROIFreehandDrag);
					myRect.InflateRect(1,1);
					InvalidateRect(&myRect, FALSE);
					*/
					DrawRubberFillRect(&dc,m_rectROIFreehandDrag);	// Draw
				}
			}
			//update freehand drag rect for next onmousemove()
			m_rectROIFreehandDrag.left = m_rectROIFreehandDrag.right;
			m_rectROIFreehandDrag.top = m_rectROIFreehandDrag.bottom;
		}

    }
	else if(m_owviewMode!=ZOOMINPOINT && m_owviewMode!=ZOOMOUTPOINT && m_owviewMode!=ZOOMRECT
		&& IsActiveView())
	{
		/*
		if(m_wROIFloatingModeToggledBack==TRUE)
		{
			if(pDoc->m_pROIset)
			{
				int idroi = pDoc->m_pROIset->GetSelectedROI();
				pDoc->m_pROIset->TranslateRectangularROI(idroi, logPoint.x-m_prev_logPoint.x, logPoint.y-m_prev_logPoint.y);
				m_prev_logPoint.x = logPoint.x;
				m_prev_logPoint.y = logPoint.y;
				//pDoc->m_pROIset->ModifyRectangularROI(idroi, &thisSelectionRect);
				//pDoc->m_pROIset->SelectROI(EDGESET_SELECTION_RECT_ROI_ID);
			}
		}
		else 
		*/
		if(m_bMode_ROIFloating==TRUE)
		{
			//////////////////////////////////////////////////
			//00) display a rectangular selection in this view
			//////////////////////////////////////////////////
			//poirier, begin, selection rect, imported and adapted from OISS
			CRect thisSelectionRect(logPoint.x-m_SelectionRectWidth/2, 
									logPoint.y-m_SelectionRectHeight/2, 
									logPoint.x+m_SelectionRectWidth/2, 
									logPoint.y+m_SelectionRectHeight/2);
			if(!m_PrevSelectionRect.IsRectEmpty()) 
			{
				//erase
				EraseSelectionRect(&dc,&m_PrevSelectionRect);
			}
			DrawSelectionRect(&dc,&thisSelectionRect);
			m_PrevSelectionRect = thisSelectionRect;
			//poirier, end

			//poirier, sept 2001, begin
			//1) update selection rect in CPixelset (ow2doc image buffer) ...
			CRect rect(thisSelectionRect);
			if(rect.left%4!=0)
			{
				rect.left += 4 - rect.left%4;
			}
			if(rect.Width()%4!=0)
			{
				rect.right -= rect.Width()%4;
			}
			((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_worldrect= rect; //adjusted selection rect (pixel width and height set to multiples of 4 to facilitate creation of new image buffer)
			//2) update of the graph (histo, scatter, etc.) will be triggered by OnLButtondown() when holding the shift key
			//set selection rect for profile, histo and scatter graph update
			if(pDoc->m_pROIset)
			{
				pDoc->m_pROIset->ModifyRectangularROI(EDGESET_SELECTION_RECT_ROI_ID, &thisSelectionRect);
				pDoc->m_pROIset->SelectROI(EDGESET_SELECTION_RECT_ROI_ID);
			}

			//3) update the zoomed view if present and if shift key is down
			if(m_pZoomedView && (nFlags&MK_SHIFT))
			{
				//if( (m_PrevSelectionRect.Width()!=thisSelectionRect.Width())
				//	|| (m_PrevSelectionRect.Height()!=thisSelectionRect.Height()) )
				//{
					m_pZoomedView->ZoomIn(thisSelectionRect);
				//}
				//else
				//{
				//	//m_pZoomedView->CenterOnLogicalPoint(thisSelectionRect.CenterPoint());
				//	CPoint myPoint = thisSelectionRect.CenterPoint();
				//	m_pZoomedView->UpdateViewport(&myPoint);
				//}
				/*
				m_pZoomedView->Invalidate();   
				m_pZoomedView->UpdateWindow();
				*/

			}
			//poirier, sept 2001, end
		}

		BOOL bLocalGraphUpdateRequired; 
		bLocalGraphUpdateRequired = (((COIIIApp*)AfxGetApp())->IsSettingsSheetGraphUpdateRequired());
		if(bLocalGraphUpdateRequired && pDoc->m_pPointset==NULL && pDoc->m_pPixelset!=NULL)
		{
			//case where pixelset local graph update is required
			if(m_bMode_ROIFloating==TRUE && (nFlags&MK_SHIFT) )
			{
				//gather context info
				globalstructCONTEXTINFO.idROI = -1;
				if(pDoc->m_pROIset)
				{
					globalstructCONTEXTINFO.idROI = pDoc->m_pROIset->GetSelectedROI();
				}
				globalstructCONTEXTINFO.idVertex = -1;
				globalstructCONTEXTINFO.idSegment = -1;
				/*
				pDoc->m_pPixelset->SetHistoChannelDirty();
				*/
				//2020july03, spi, begin
				//((COIIIApp*)AfxGetApp())->m_pSettingsSheet->PostMessage(OWM_MOUSEMOVED_UPDATELOCALGRAPH, (DWORD)&globalstructCONTEXTINFO);
				((COIIIApp*)AfxGetApp())->m_pSettingsSheet->PostMessage(OWM_MOUSEMOVED_UPDATELOCALGRAPH, (LONG_PTR)&globalstructCONTEXTINFO);
				//2020july03, spi, end
			}
		}
		else if(pDoc->m_pPointset!=NULL)
		{
			//case where pointset local graph update is required

			//////////////////////////////////////////////
			//0) get nearest pointset object if later required
			//////////////////////////////////////////////
			POINT2DD myPoint2DD = MapPointFromViewToWorld(logPoint);
			int idPointsetObject=-1;
			if(!m_bBusyPainting && (pDoc->m_pBehaviorSettings->bMBHighlightNearestPoint
				|| pDoc->m_pBehaviorSettings->bMBHighlightNearestNeighbors
				|| pDoc->m_pBehaviorSettings->bMBHighlightSegment) 
				|| bLocalGraphUpdateRequired)
			{
				//get nearest pointset object (idvertex, also equals the idstartsegment)
				idPointsetObject = pDoc->GetNearestPointsetObject(myPoint2DD.x, myPoint2DD.y, &m_itriseed); 
				//2012august09, poirier, begin
				if(idPointsetObject!=-1)
				{
					int iv=1;
				}
				//2012august09, poirier, end
			}

			//////////////////////////////////////////////
			//1) update profile, histo or scatter graph if visible on screen
			//////////////////////////////////////////////
			if(bLocalGraphUpdateRequired && m_bShiftKeyIsDown) 
			{
				//to avoid flickering of local graph
				if(idPointsetObject!=m_idPrevPointsetObjectUsedWithLocalGraph)
				{
					m_idPrevPointsetObjectUsedWithLocalGraph = idPointsetObject;
					//gather context info
					globalstructCONTEXTINFO.idROI = -1;
					if(pDoc->m_pROIset)
					{
						globalstructCONTEXTINFO.idROI = pDoc->m_pROIset->GetSelectedROI();
					}
					globalstructCONTEXTINFO.idVertex = idPointsetObject;
					globalstructCONTEXTINFO.idSegment = -1;
					if(pDoc->m_pSegmentset!=NULL && m_iNumberOfSegmentForFinalPartition!=-1)
					{
						int idsegOwner = GetOwnerParentSegmentThreshold(pDoc->m_pSegmentset,	
																		idPointsetObject,
			   			   												m_iNumberOfSegmentForFinalPartition );
						globalstructCONTEXTINFO.idSegment = idsegOwner;
					}
					//post message
					//2020july03, spi, begin
					//((COIIIApp*)AfxGetApp())->m_pSettingsSheet->PostMessage(OWM_MOUSEMOVED_UPDATELOCALGRAPH, (DWORD)&globalstructCONTEXTINFO);
					((COIIIApp*)AfxGetApp())->m_pSettingsSheet->PostMessage(OWM_MOUSEMOVED_UPDATELOCALGRAPH, (LONG_PTR)&globalstructCONTEXTINFO);
					//2020july03, spi, end
				}
			}

			//////////////////////////////////////////////
			//2) update mouse location local drawing
			//////////////////////////////////////////////
			if(pDoc->m_pBehaviorSettings->bMBHighlightNearestPoint && !m_bBusyPainting)
			{
				if(pDoc->m_pPointset!=NULL && myWorldRect.PtInRect(logPoint))
				{
					/*
					POINT2DD myPoint2DD = MapPointFromViewToWorld(logPoint);
					int idPointsetObject = pDoc->GetNearestPointsetObject(myPoint2DD.x, myPoint2DD.y, &m_itriseed); 
					*/
					//to avoid flickering, do not erase and redraw if still capturing the same object
					if(idPointsetObject!=m_idPointsetObject)
					{
						//erase previously drawn object
						if(m_idPointsetObject>-1) pDoc->m_pPointset->DrawPointsetObject(this, &dc, m_idPointsetObject); //erase
						//draw new object
						m_idPointsetObject = idPointsetObject; 
						if(m_idPointsetObject>-1) pDoc->m_pPointset->DrawPointsetObject(this, &dc, m_idPointsetObject); //draw
					}
				}
			}
			if(pDoc->m_pBehaviorSettings->bMBHighlightNearestNeighbors && !m_bBusyPainting)
			{
				if(pDoc->m_pPointset!=NULL && myWorldRect.PtInRect(logPoint))
				{
					/*
					POINT2DD myPoint2DD = MapPointFromViewToWorld(logPoint);
					int idObject = pDoc->GetNearestPointsetObject(myPoint2DD.x, myPoint2DD.y, &m_itriseed); 
					*/
					int idObject = idPointsetObject;
					//to avoid flickering, do not erase and redraw if still capturing the same object
					if(idObject!=m_idPrevObject)
					{
						//erase previously drawn objects
						if(m_numneighborfound>0)
						{
							for (int i=0; i<m_numneighborfound; i++)
							{
								if(m_pArrayNeighbor[i]>-1) pDoc->m_pPointset->DrawPointsetObject(this, &dc, m_pArrayNeighbor[i]); //erase
							}
						}
						//if new valid object found, look for neighbors
						m_idPrevObject = idObject;
						if(idObject>-1) 
						{
							FindAllConsistentNeighborSurroundingVertex( pDoc->m_pPointset->m_pPOINTSET,
																		idObject,
																		&m_itriseed,
																		&m_numneighborfound,
																		m_pArrayNeighbor);
						}
						else
						{
							m_numneighborfound = -1;
						}
						//draw the new neighboring objects
						if(m_numneighborfound>0)
						{
							for (int i=0; i<m_numneighborfound; i++)
							{
								if(m_pArrayNeighbor[i]>-1) pDoc->m_pPointset->DrawPointsetObject(this, &dc, m_pArrayNeighbor[i]); //draw
							}
						}
					}//end of if(idObject==m_idPrevObject)
				}//end of if(PtInRect)
			}//end of if(bMBHighlightNearestNeighbors)
			if(pDoc->m_pBehaviorSettings->bMBHighlightSegment && !m_bBusyPainting)
			{
				//if segmentset not drawn yet, set m_iNumberOfSegmentForFinalPartition to default initial number of segment
				if(m_iNumberOfSegmentForFinalPartition==-1)
				{
					m_iNumberOfSegmentForFinalPartition = pDoc->m_pSegmentset->iInitialNumberOfSegment;
				}
				if(pDoc->m_pPointset!=NULL && myWorldRect.PtInRect(logPoint))
				{
					/*
					POINT2DD myPoint2DD = MapPointFromViewToWorld(logPoint);
					int idStartSegment = pDoc->GetNearestPointsetObject(myPoint2DD.x, myPoint2DD.y, &m_itriseed); 
					*/
					int idStartSegment = idPointsetObject;
					//to avoid flickering, do not erase and redraw if still capturing the same object
					if(idStartSegment!=m_idStartSegment)
					{
						//erase previously drawn object
						if(m_idStartSegment>-1)
						{
							//erase segment polygon
							int idvertex = m_idStartSegment;
							DrawSegmentPolygon(	&dc,
												pDoc->m_pPointset->m_pPOINTSET,
												pDoc->m_pSegmentset,
												m_idStartSegment, //to be drawn
												idvertex,	 //any one vertex inside the segment to be drawn
												m_iNumberOfSegmentForFinalPartition,
												FALSE,
												R2_NOTXORPEN);
						}
						m_idStartSegment = idStartSegment; 
						if(m_idStartSegment>-1) 
						{
							//draw new segment polygon
							int idvertex = m_idStartSegment;
							DrawSegmentPolygon(	&dc,
												pDoc->m_pPointset->m_pPOINTSET,
												pDoc->m_pSegmentset,
												m_idStartSegment, //to be drawn
												idvertex,	 //any one vertex inside the segment to be drawn
												m_iNumberOfSegmentForFinalPartition,
												FALSE,
												R2_NOTXORPEN);
							//get new segment history list
							m_indexHistoryList=-1;
							GetSegmentHistory(	pDoc->m_pSegmentset,
												m_idStartSegment,
												1, //m_iNumberOfSegmentForFinalPartition
												m_pidsegHistoryList,
												m_piNumSegForFinalPartitionList,
												&m_inumberofcollectedsegment,
												m_imaxhistorylistsize); //size of supplied historylist

						}
						else
						{
							m_indexHistoryList=-1;
							m_inumberofcollectedsegment = 0;
						}
					}
				}
			}//end of if(bMBHighlightSegment)
		}
		else
		{
			//no update required
			//this occurs when mouse is moving over the image
		}
	}

	/*
    if (IsPanning()) 
	{ //Are we in panning mode?
		ContinuePan(&logPoint);
    }
	*/
	
	//COWView::OnMouseMove(nFlags, point);
}


//	OnLButtonDown
//	
//	If we're in normal mode draw a cloud.
//	If we're in zoomin/zoomout, wait for lbuttonup.
//	If we're in zoomrect, start trackin', then wait for lbuttonup.
//	See OnLButtonUp for details and OnMouseMove for more tracking.
void COW2View::OnLButtonDown(UINT nFlags, CPoint point) 
{
    // CScrollView changes the viewport origin and mapping mode.
    // It's necessary to convert the point from device coordinates
    // to logical coordinates, such as are stored in the document.

    CPoint logPoint = point;
    CClientDC dc(this);
    OnPrepareDC(&dc);
	//poirier, oct 2001, begin
	CPoint pt0(0,0);
	CPoint pt1(1,1);
	dc.LPtoDP(&pt0);
	dc.LPtoDP(&pt1);
	CSize devsizeOneLogicalUnit(pt1.x-pt0.x, pt1.y-pt0.y);
	logPoint.Offset((int)(-0.5*devsizeOneLogicalUnit.cx), (int)(-0.5*devsizeOneLogicalUnit.cy));
	if(logPoint.x<0) logPoint.x = 0;
	if(logPoint.y<0) logPoint.y = 0;
	//poirier, oct 2001, end
    dc.DPtoLP(&logPoint);



    switch (m_owviewMode)
	{
	
    case NORMAL:		
		{
			//2012july23, poirier, begin
			if(nFlags&MK_CONTROL)
			{
				if(!(nFlags&MK_SHIFT))
				{
					m_owviewMode=DRAWELLIPSE;
					m_bCaptured = TRUE;
					SetCapture();
					OnSetCursor(NULL,HTCLIENT,0);// Get the drag rectangle cursor going.
					//Initialize the dragging circle.
					m_rectEllipseDrag.SetRect(logPoint.x,logPoint.y,logPoint.x,logPoint.y);
					/*				
					#ifdef _DEBUG
					COW2Doc* pDoc = GetDocument();
					CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
					CString myString;
					myString.Format("m_rectEllipseDrag.SetRect(%f,%f,%f,%f);\r\n",logPoint.x,logPoint.y,logPoint.x,logPoint.y);
					pLogDocument->AddText(myString);
					#endif //_DEBUG
					*/
					break;
				}
				else if(nFlags&MK_SHIFT)
				{
					//m_owviewMode=SELECTELLIPSE;
					//todo
				}
			}
			//2012july23, poirier, end
			if(m_bMode_ROIFloating==TRUE)
			{
				
				//poirier, sept 2001, begin
				//if shift key is held down
				if(nFlags&MK_SHIFT)
				{
					break;
				}
				//poirier, sept 2001, end
				

				//poirier, begin, selection rect, imported and adapted from OISS
				CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
				// Get the active MDI child window.
				CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
				// Get the active view attached to the active MDI child window.
				COW2View* pView = (COW2View*) pChild->GetActiveView();
				if(pView!=this)
				{
					return;
				}
				/*
				CPoint logPoint = point;
				CClientDC dc(this);
				OnPrepareDC(&dc);
				dc.DPtoLP(&logPoint);
				*/
				m_TempPoint = logPoint;//point;
				m_wDeflatingTimerOn = SetTimer(m_nDeflatingTimerID, 100, NULL); //100 milliseconds -> 10 updates a seconds
				//poirier, end, selection rect, imported and adapted from OISS
			}
			else if(m_bMode_ROIRect==TRUE)
			{
				//Start tracking rectangle (roi rect)
				m_bCaptured = TRUE;
				SetCapture();
				OnSetCursor(NULL,HTCLIENT,0);// Get the drag rectangle cursor going.
				/*
				CClientDC dc(this);
				OnPrepareDC(&dc);  // set up mapping mode and viewport origin
				dc.DPtoLP(&point);
				//Initialize the dragging rectangle.
				m_rectROIRectDrag.SetRect(point.x,point.y,point.x,point.y);
				*/
				//Initialize the dragging rectangle.
				m_rectROIRectDrag.SetRect(logPoint.x, logPoint.y, logPoint.x, logPoint.y);
			}
			else if(m_bMode_ROIPolyline==TRUE)
			{
				//first lbutton down
				if(m_bCaptured==FALSE)
				{
					//Start tracking first polyline's line segment (roi polyline)
					m_bCaptured = TRUE;
					SetCapture();
					OnSetCursor(NULL,HTCLIENT,0);// Get the drag rectangle cursor going.
					/*
					CClientDC dc(this);
					OnPrepareDC(&dc);  // set up mapping mode and viewport origin
					dc.DPtoLP(&point);
					//Initialize the dragging rectangle.
					m_rectROIPolylineDrag.SetRect(point.x,point.y,point.x,point.y);
					*/
					//Initialize the dragging rectangle.
					m_rectROIPolylineDrag.SetRect(logPoint.x, logPoint.y, logPoint.x, logPoint.y);
				}
				else
				{ 
					//second lbutton down
					if(m_idroiROIPolylineDrag==-1)
					{
						//m_bCaptured = FALSE;
						//ReleaseCapture();
						/*
						CClientDC dc(this);
						OnPrepareDC(&dc);  // set up mapping mode and viewport origin
						dc.DPtoLP(&point);
						m_rectROIPolylineDrag.right = point.x;
						m_rectROIPolylineDrag.bottom = point.y;
						*/
						m_rectROIPolylineDrag.right = logPoint.x;
						m_rectROIPolylineDrag.bottom = logPoint.y;
						//m_owviewMode = NORMAL;
						//DrawRubberLine(&dc,m_rectROIPolylineDrag); //Erase last line
						//CRect rect(m_rectROIPolylineDrag);
						//m_rectROIPolylineDrag.NormalizeRect();
						//m_bRendered = FALSE;
						//OnSetCursor(NULL,HTCLIENT,0); //go back to normal drag rectangle.

						//DisableROIRectMode(); //go back to normal mode with normal cursor

						//drop a new polyline roi
						COW2Doc* pDoc = GetDocument();
						if(pDoc!=NULL && pDoc->m_pROIset)
						{
							//1) add new roi at these coordinate
							m_idroiROIPolylineDrag = pDoc->m_pROIset->AddPolylinearROI(&m_rectROIPolylineDrag);
							/*
							//2) select new rect roi
							pDoc->m_pROIset->SelectROI(m_idroiROIPolylineDrag);
							SelectROI(m_idroiROIPolylineDrag);
							*/
							//3)update window
							CRect myRect = pDoc->m_pROIset->GetRectROI(m_idroiROIPolylineDrag);
							CClientDC dc(this);
							OnPrepareDC(&dc);
							dc.LPtoDP(&myRect);
							myRect.InflateRect(devsizeOneLogicalUnit.cx, devsizeOneLogicalUnit.cy);
							InvalidateRect(&myRect, FALSE);
						}
						/*
						//instead of disabling mode, set next line segment origin
						m_rectROIPolylineDrag.SetRect(point.x, point.y, point.x, point.y);
						*/
						//instead of disabling mode, set next line segment origin
						m_rectROIPolylineDrag.SetRect(logPoint.x, logPoint.y, logPoint.x, logPoint.y);
					}
					//third and subsequent lbutton down
					else
					{
						//drop a new line segment in the current polyline roi
						COW2Doc* pDoc = GetDocument();
						if(pDoc!=NULL && pDoc->m_pROIset)
						{
							/*
							CClientDC dc(this);
							OnPrepareDC(&dc);  // set up mapping mode and viewport origin
							dc.DPtoLP(&point);
							*/
							//1)
							m_rectROIPolylineDrag.right = logPoint.x;
							m_rectROIPolylineDrag.bottom = logPoint.y;
							//2) add new roi at these coordinate
							pDoc->m_pROIset->AddLineToPolylinearROI(m_idroiROIPolylineDrag, &m_rectROIPolylineDrag);
							//3) instead of disabling mode, set next line segment origin
							m_rectROIPolylineDrag.SetRect(logPoint.x, logPoint.y, logPoint.x, logPoint.y);
							//4) update window
							CRect myRect = pDoc->m_pROIset->GetRectROI(m_idroiROIPolylineDrag);
							//CClientDC dc(this);
							//OnPrepareDC(&dc);
							dc.LPtoDP(&myRect);
							myRect.InflateRect(devsizeOneLogicalUnit.cx, devsizeOneLogicalUnit.cy);
							InvalidateRect(&myRect, FALSE);
						}
					}
				}
			}
			else if(m_bMode_ROIFreehand==TRUE)
			{
				if(m_bCaptured==FALSE)
				{
					//Start tracking first polyline's line segment (roi polyline)
					m_bCaptured = TRUE;
					SetCapture();
					OnSetCursor(NULL,HTCLIENT,0);// Get the drag rectangle cursor going.
					/*
					CClientDC dc(this);
					OnPrepareDC(&dc);  // set up mapping mode and viewport origin
					dc.DPtoLP(&point);
					*/
					//Initialize the dragging rectangle.
					m_rectROIFreehandDrag.SetRect(logPoint.x, logPoint.y, logPoint.x, logPoint.y);
				}
			}
			else
			{
			}
		}
		break;
	

    case ZOOMINPOINT:  //Wait for lbuttonup.
    case ZOOMOUTPOINT:
		break;
	
    case ZOOMRECT: 
		//Start tracking rectangle (zoom rect)
		m_bCaptured = TRUE;
		SetCapture();
		OnSetCursor(NULL,HTCLIENT,0);// Get the drag rectangle cursor going.
		/*
		CClientDC dc(this);
		OnPrepareDC(&dc);  // set up mapping mode and viewport origin
		dc.DPtoLP(&point);
		*/
		//Initialize the dragging rectangle.
		m_rectDrag.SetRect(logPoint.x,logPoint.y,logPoint.x,logPoint.y);
		break;	
    }
	
	//COWView::OnLButtonDown(nFlags, point);
}

//	
//	OnLButtonUp
//	
//	We need to grab the point if we're in rect or point mode.
//
void COW2View::OnLButtonUp(UINT nFlags, CPoint point) 
{
    COW2Doc * pDoc = GetDocument();	
    CClientDC dc(this);
    OnPrepareDC(&dc);  // set up mapping mode and viewport origin
    dc.DPtoLP(&point);
    
    switch (m_owviewMode)
	{
		case ZOOMINPOINT:
			m_bRendered = FALSE; //poirier, august 25th 1999
			ZoomIn(&point); //COWView
			m_owviewMode = NORMAL;
			OnSetCursor(NULL,HTCLIENT,0);
			break;
		
		case ZOOMOUTPOINT:
			m_bRendered = FALSE; //poirier, august 25th 1999
			ZoomOut(&point);   //COWView
			m_owviewMode = NORMAL;
			OnSetCursor(NULL,HTCLIENT,0);
			break;
		
		case ZOOMRECT:
			if (m_bCaptured)
			{
				m_bCaptured = FALSE;
				ReleaseCapture();
				m_rectDrag.right = point.x;
				m_rectDrag.bottom = point.y;
				m_owviewMode = NORMAL;
				DrawRubberRect(&dc,m_rectDrag); //Erase last line
				CRect rect(m_rectDrag);
				m_rectDrag.NormalizeRect();
				m_bRendered = FALSE; //poirier, august 25th 1999
				OnSetCursor(NULL,HTCLIENT,0); //go back to normal drag rectangle.
				if(nFlags!=MK_SHIFT)
				{
					ZoomIn(m_rectDrag); //COWView
				}
				else
				{
					if(rect.left%4!=0)
					{
						rect.left += 4 - rect.left%4;
					}
					if(rect.Width()%4!=0)
					{
						rect.right -= rect.Width()%4;
					}
					ASSERT(rect.Width()%4==0);
					((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_framealwaysontop = FALSE;
					((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_initzoomfactor = GetZoomFactor();
					((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_worldrect= rect; //selection rect
					((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_bRawData = FALSE; //FALSE, will use doc pixelset imghandle
					((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pDoc = this->GetDocument(); 
					((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pPixelset = NULL;
					((COIIIApp*)AfxGetApp())->m_pOW2DocTemplate->OpenDocumentFile(NULL);
				}
			}
			break;			

		case DRAWELLIPSE:
			if (m_bCaptured)
			{
				m_bCaptured = FALSE;
				ReleaseCapture();
				//m_rectEllipseDrag.right = point.x;
				//m_rectEllipseDrag.bottom = point.y;
				m_owviewMode = NORMAL;
				DrawRubberEllipse(&dc,m_rectEllipseDrag); //Erase last line
				CRect rect(m_rectEllipseDrag);
				m_rectEllipseDrag.NormalizeRect();
				m_bRendered = TRUE; 
				OnSetCursor(NULL,HTCLIENT,0); //go back to normal drag rectangle.
				/*
				if(nFlags!=MK_SHIFT)
				{
					ZoomIn(m_rectDrag); //COWView
				}
				else
				{
					if(rect.left%4!=0)
					{
						rect.left += 4 - rect.left%4;
					}
					if(rect.Width()%4!=0)
					{
						rect.right -= rect.Width()%4;
					}
					ASSERT(rect.Width()%4==0);
					((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_framealwaysontop = FALSE;
					((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_initzoomfactor = GetZoomFactor();
					((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_worldrect= rect; //selection rect
					((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_bRawData = FALSE; //FALSE, will use doc pixelset imghandle
					((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pDoc = this->GetDocument(); 
					((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pPixelset = NULL;
					((COIIIApp*)AfxGetApp())->m_pOW2DocTemplate->OpenDocumentFile(NULL);
				}
				*/
				//pDoc->m_pPointset->AddPoint((m_rectEllipseDrag.right+m_rectEllipseDrag.left)/2.0, (m_rectEllipseDrag.top+m_rectEllipseDrag.bottom)/2.0, POINTSET_TIN_UPDATE, &m_itriseed);
				POINT myPoint;
				myPoint.x = (m_rectEllipseDrag.right+m_rectEllipseDrag.left)/2;
				myPoint.y = (m_rectEllipseDrag.top+m_rectEllipseDrag.bottom)/2;
				POINT2DD myPOINT2DD = MapPointFromViewToWorld(myPoint);
				RECT2DD myRECT2DD = MapRectFromViewToWorld(m_rectEllipseDrag);
				double fDimX = abs(myRECT2DD.right-myRECT2DD.left);
				double fDimY = abs(myRECT2DD.bottom-myRECT2DD.top);
				double fArea = fDimX*fDimY;
				if(fDimX==0.0 || fDimY==0.0)
				{
					fDimX=-1.0;
					fDimY=-1.0;
					fArea=-1.0;
				}
				pDoc->AddPoint(myPOINT2DD.x, myPOINT2DD.y, POINTSET_TIN_UPDATE, &m_itriseed, -1.0, fDimX , fDimY, fDimX*fDimY);
			}
			break;			

		//no zooming mode
		case NORMAL: 

			if(m_bMode_ROIFloating==TRUE)
			{
				//poirier, sept 2001, begin
				//if shift key is held down
				if(nFlags&MK_SHIFT)
				{
					//if dlg does not exist, pops new histo dialog (pop the whole settings property dialog selected on histo page)
					//delay poping it so the process can be aborted if mousedblclk message follows
					m_wDelayingOpenSettingsDlgTimerOn = SetTimer(m_nDelayingOpenSettingsDlgTimerID, 100, NULL); //100 milliseconds -> 10 updates a seconds
				}
				else
				//poirier, sept 2001, end
				//poirier, begin, selection rect, imported and adapted from OISS
				if(m_wDeflatingTimerOn) 
				{
					//kill inflating timer
					KillTimer(m_nDeflatingTimerID);
				}
				//poirier, end, selection rect, imported and adapted from OISS

				/* //was fine, but not droping roi anymore in this mode
				//poirier, sept 2001, begin
				if(m_wROIFloatingModeToggledBack==FALSE)
				{
					//drop a new rectangular roi
					if(pDoc->m_pROIset)
					{
						//1) get moving selection rect roi coordinate
						RECT myRECT = pDoc->m_pROIset->GetRectROI(EDGESET_SELECTION_RECT_ROI_ID);
						//2) add new roi at these coordinate
						int idroi_new = pDoc->m_pROIset->AddRectangularROI(&myRECT);
						//3) disable mode
						DisableROIFloatingMode();
						//4) select new rect roi
						//pDoc->m_pROIset->SelectROI(idroi_new);
						SelectROI(idroi_new);
					}
				}
				else
				{
					m_wROIFloatingModeToggledBack = FALSE;
				}
				//poirier, sept 2001, end
				*/
				
			}
			else if(m_bMode_ROIRect==TRUE)
			{
				if (m_bCaptured)
				{
					m_bCaptured = FALSE;
					ReleaseCapture();
					m_rectROIRectDrag.right = point.x;
					m_rectROIRectDrag.bottom = point.y;
					//m_owviewMode = NORMAL;
					DrawRubberRect(&dc,m_rectROIRectDrag); //Erase last line
					CRect rect(m_rectROIRectDrag);
					m_rectROIRectDrag.NormalizeRect();
					//m_bRendered = FALSE;
					//OnSetCursor(NULL,HTCLIENT,0); //go back to normal drag rectangle.

					DisableROIRectMode(); //go back to normal mode with normal cursor

					//drop a new rectangular roi
					if(pDoc->m_pROIset && m_rectROIRectDrag.Width()>0 && m_rectROIRectDrag.Height()>0)
					{
						//1) add new roi at these coordinate
						int idroi_new = pDoc->m_pROIset->AddRectangularROI(&m_rectROIRectDrag);
						//2) select new rect roi
						//pDoc->m_pROIset->SelectROI(idroi_new);
						SelectROI(idroi_new);
					}

				}
			}
			else if(m_bMode_ROIPolyline==TRUE)
			{
				//note: roi polyline disable occurs on esc or on right button up
			}
			else if(m_bMode_ROIFreehand==TRUE)
			{
				//disable freehand mode
				if (m_bCaptured)
				{
					m_bCaptured = FALSE;
					ReleaseCapture();
					//polyline is completed, update bounding rectangle
					pDoc->m_pROIset->AddLineToPolylinearROI(m_idroiROIFreehandDrag, NULL);
					DisableROIFreehandMode();
					//select roi
					SelectROI(m_idroiROIFreehandDrag);
					m_idroiROIFreehandDrag = -1;
				}
			}

			//normal mode (no zooming, no special roi mode)
			else
			{
				//poirier, sept 2001, begin
				//grab and select roi
				if(pDoc->m_pROIset)
				{
					//1) if clicking on roi, get roi id
					int idroi_selected = pDoc->m_pROIset->GetSelectedROI();
					int idroi = pDoc->m_pROIset->IsTouchingROI(point);
					if(idroi!=idroi_selected)
					{
						//1.1) roi was not selected
						if(idroi==-1) 
						{
							if(idroi_selected!=EDGESET_VIEWING_RECT_ROI_ID)
							{
								SelectROI(EDGESET_VIEWING_RECT_ROI_ID);
							}
						}
						else 
						{
							SelectROI(idroi);
						}
					}
					else
					{
						//1.2) roi was already selected
						if(nFlags&MK_SHIFT)
						{
							//poirier, sept 2001, begin
							//if clicking within an already selected roi ...
							//toggle back the local analysis mode on
							//floating selection rect is defined by that roi
							//grab and select roi
							COW2Doc* pDoc = GetDocument();
							if(pDoc!=NULL && pDoc->m_pROIset)
							{
								//1) if clicking on roi, get roi id
								int idroi_selected = pDoc->m_pROIset->GetSelectedROI();
								CPoint logPoint = point;
								CClientDC dc(this);
								OnPrepareDC(&dc);
								dc.DPtoLP(&logPoint);
								int idroi = pDoc->m_pROIset->IsTouchingROI(logPoint);
								if(idroi==idroi_selected && idroi!=-1)
								{
									
									CRect myRect = pDoc->m_pROIset->GetRectROI(idroi);
									//delete roi
									DeleteROI(idroi);
									//set floating selection rect with this roi
									m_SelectionRectWidth = myRect.Width();
									m_SelectionRectHeight = myRect.Height();
									
									//draw selection rect
									//m_PrevSelectionRect.SetRectEmpty();
									//DrawSelectionRect(&dc,&myRect); //ROISET_DRAWAS_SELECTED);

									//enable (toggle back) local analysis mode
									m_wROIFloatingModeToggledBack = TRUE; //will be set back to false within onlbuttonup()
									EnableROIFloatingMode();	
								}
							}
						}
						//poirier, sept 2001, end
					}
				}
				//poirier, sept 2001, end
			}
			
	} //end of switch
	COWView::OnLButtonUp(nFlags, point);
}

void COW2View::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	//poirier, sept 2001, begin
	if(m_wDelayingOpenSettingsDlgTimerOn)
	{
		KillTimer(m_nDelayingOpenSettingsDlgTimerID); 
	}
	//poirier, sept 2001, end

	if(m_bMode_ROIFloating)
	{
		CRect rect(this->GetDocument()->GetWorldRect()); //whole image
		if(!m_PrevSelectionRect.IsRectEmpty()) 
		{
			rect = m_PrevSelectionRect;
			CClientDC dc(this);
			OnPrepareDC(&dc);
			EraseSelectionRect(&dc,&m_PrevSelectionRect);//Erase
			m_PrevSelectionRect.SetRectEmpty();
		}

		int ix_align = 0;
		if(rect.left%4!=0)
		{
			ix_align = 4 - rect.left%4;
			rect.left += ix_align;
		}
		if(rect.Width()%4!=0)
		{
			rect.right -= rect.Width()%4;
		}
		ASSERT(rect.Width()%4==0);
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_framealwaysontop = TRUE; 
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_worldrect= rect; //selection rect
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_bRawData = FALSE; //FALSE, will use doc pixelset imghandle
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pDoc = GetDocument(); 
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pPixelset = NULL;

		//if shift key is held down
		if(nFlags&MK_SHIFT)
		{
			//ensure alternate mode is off
			m_bOnMouseMoveLocalExtractionON = FALSE;

			//pop a new doc, frame and view
			COW2Doc* pNewDoc = (COW2Doc*) ((COIIIApp*)AfxGetApp())->m_pOW2DocTemplate->OpenDocumentFile(NULL);
			/*
			COW2Doc* pNewDoc = (COW2Doc*) new COW2Doc();
			pNewDoc->OnNewDocumentFromAppTempInfo();
			*/

			if(pNewDoc)
			{
				pNewDoc->OnAnalysisTextureextraction();
				m_bMode_ROIFloating = FALSE;
			}
		}
		else
		{
			//output result into existing document
			/*
			if(m_bOnMouseMoveLocalExtractionON==FALSE) 
			{
				m_bOnMouseMoveLocalExtractionON = TRUE;
			}
			else 
			{
				m_bOnMouseMoveLocalExtractionON = FALSE;
				m_bMode_ROIFloating = FALSE;
			}
			*/
			LocalTextureExtraction(rect);
		}

		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_framealwaysontop = FALSE; 

	}

	COWView::OnLButtonDblClk(nFlags, point);
}

void COW2View::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bMode_ROIFloating==TRUE)
	{
		//poirier, sept 2001, begin
		//if shift key is held down
		if(nFlags&MK_SHIFT)
		{
			//prevent inflating because shift key held down
			//right click action will occur on mouserbuttonup - zoomed view will be poped
		}
		//poirier, sept 2001, ende
		else
		{
			//poirier, begin, selection rect, imported and adapted from OISS
			CPoint logPoint = point;
			CClientDC dc(this);
			OnPrepareDC(&dc);
			dc.DPtoLP(&logPoint);
			m_TempPoint = logPoint;//point;
			m_wInflatingTimerOn = SetTimer(m_nInflatingTimerID, 100, NULL); //100 milliseconds -> 10 updates a seconds
			//poirier, end, selection rect, imported and adapted from OISS
		}
	}

	COWView::OnRButtonDown(nFlags, point);
}

void COW2View::OnRButtonUp(UINT nFlags, CPoint point) 
{
    CPoint logPoint = point;
    CClientDC dc(this);
    OnPrepareDC(&dc);
    dc.DPtoLP(&logPoint);
    COW2Doc* pDoc = (COW2Doc*)GetDocument();
	CRect myWorldRect = pDoc->GetWorldRect();

	if(m_bMode_ROIFloating==TRUE)
	{
		//poirier, sept 2001, begin
		//if shift key is held down
		if(nFlags&MK_SHIFT)
		{
			//if no companion zoomed view exist, pops new zoomed view 
			OnViewNewZoomedview();
		}
		else
		//poirier, sept 2001, end

		//poirier, begin, selection rect, imported and adapted from OISS
		if(m_wInflatingTimerOn) 
		{
			//kill inflating timer
			KillTimer(m_nInflatingTimerID);
		}
		//poirier, end, selection rect, imported and adapted from OISS
	}
	else if(m_bMode_ROIRect==TRUE)
	{
	}
	else if(m_bMode_ROIPolyline==TRUE)
	{
		//disable on esc or on right button up
		if(m_bCaptured==TRUE)
		{
			m_bCaptured = FALSE;
			ReleaseCapture();
			/*
			m_rectROIPolylineDrag.right = point.x;
			m_rectROIPolylineDrag.bottom = point.y;
			*/
			DrawRubberLine(&dc,m_rectROIPolylineDrag); //Erase last line
			
			//polyline is completed, update bounding rectangle
			pDoc->m_pROIset->AddLineToPolylinearROI(m_idroiROIPolylineDrag, NULL);

			DisableROIPolylineMode();
			m_idroiROIPolylineDrag = -1;
		}
	}
	else if(m_bMode_ROIFreehand==TRUE)
	{
	}
	else if(pDoc->m_pBehaviorSettings->bMBHighlightSegment && !m_bBusyPainting)
	{
		if(pDoc->m_pPointset!=NULL && myWorldRect.PtInRect(logPoint))
		{
			/*
			POINT2DD myPoint2DD = MapPointFromViewToWorld(logPoint);
			int idStartSegment = pDoc->GetNearestPointsetObject(myPoint2DD.x, myPoint2DD.y, &m_itriseed); 
			*/
			m_indexHistoryList++;
			//to avoid flickering, do not erase and redraw if still capturing the same object
			if(m_indexHistoryList<m_inumberofcollectedsegment)
			{
				//erase previously drawn object
				if(m_idHistorySegment>-1)
				{
					//erase segment polygon
					int idvertex = m_idStartSegment;
					DrawSegmentPolygon(	&dc,
										pDoc->m_pPointset->m_pPOINTSET,
										pDoc->m_pSegmentset,
										m_idHistorySegment, //to be drawn
										idvertex,	 //any one vertex inside the segment to be drawn
										m_idHistorySegmentPartition, //m_iNumberOfSegmentForFinalPartition,
										FALSE,
										R2_NOTXORPEN);
				}
				m_idHistorySegment = m_pidsegHistoryList[m_inumberofcollectedsegment-1-m_indexHistoryList];
 				m_idHistorySegmentPartition = m_piNumSegForFinalPartitionList[m_inumberofcollectedsegment-1-m_indexHistoryList];
				if(m_idStartSegment>-1) 
				{
					//draw new segment polygon
					int idvertex = m_idStartSegment;
					DrawSegmentPolygon(	&dc,
										pDoc->m_pPointset->m_pPOINTSET,
										pDoc->m_pSegmentset,
										m_idHistorySegment, //to be drawn
										idvertex,	 //any one vertex inside the segment to be drawn
										m_idHistorySegmentPartition, //m_iNumberOfSegmentForFinalPartition,
										FALSE,
										R2_NOTXORPEN);
				}
			}
		}
	}//end of if(bMBHighlightSegment)
	else
	{
		//handle popup memu
		CRect myRect;
		GetClientRect ((LPRECT)&myRect); 
		if (myRect.PtInRect(point))
		{   
			//popup the floating menu
			HandlePopupMenu(point);
		}	
	}
	COWView::OnRButtonUp(nFlags, point);
}

void COW2View::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	if(m_bOnMouseMoveLocalExtractionON==TRUE) 
	{
		m_bOnMouseMoveLocalExtractionON = FALSE;
		m_bMode_ROIFloating = FALSE;
	}
	
	COWView::OnRButtonDblClk(nFlags, point);
}

void COW2View::OpenSettingsDlg()
{
	ASSERT(m_wDelayingOpenSettingsDlgTimerOn);
	//poirier, sept 2001, begin
	if(m_wDelayingOpenSettingsDlgTimerOn)
	{
		KillTimer(m_nDelayingOpenSettingsDlgTimerID); 
	}
	//poirier, sept 2001, end

	//pop a new histo dialog (pop the whole settings property dialog selected on histo page)
	//note: for local ROI graph, use bGlobal==FALSE && bLocal==FALSE && bSegment==FALSE
	COW2Doc* pDoc = GetDocument();
	if(pDoc==NULL) return;

	pDoc->m_pProfileSettings->bGlobal = FALSE;
	pDoc->m_pProfileSettings->bLocal = FALSE;
	pDoc->m_pProfileSettings->bSegment = FALSE;

	pDoc->m_pHistoSettings->bGlobal = FALSE;
	pDoc->m_pHistoSettings->bLocal = FALSE;
	pDoc->m_pHistoSettings->bSegment = FALSE;

	pDoc->m_pScatterSettings->bGlobal = FALSE;
	pDoc->m_pScatterSettings->bLocal = FALSE;
	pDoc->m_pScatterSettings->bSegment = FALSE;

	if(pDoc->m_pPointset!=NULL) 
	{
		pDoc->m_pProfileSettings->bImagePixels = FALSE;
		pDoc->m_pProfileSettings->bImageFeatures = TRUE;
		pDoc->m_pHistoSettings->bImagePixels = FALSE;
		pDoc->m_pHistoSettings->bImageFeatures = TRUE;
		pDoc->m_pScatterSettings->bImagePixels = FALSE;
		pDoc->m_pScatterSettings->bImageFeatures = TRUE;
	}
	else if(pDoc->m_pPixelset!=NULL)
	{
		pDoc->m_pProfileSettings->bImagePixels = TRUE;
		pDoc->m_pProfileSettings->bImageFeatures = FALSE;
		pDoc->m_pHistoSettings->bImagePixels = TRUE;
		pDoc->m_pHistoSettings->bImageFeatures = FALSE;
		pDoc->m_pScatterSettings->bImagePixels = TRUE;
		pDoc->m_pScatterSettings->bImageFeatures = FALSE;
	}
	else
	{
		ASSERT(FALSE);
	}
	if(pDoc->m_pPointset!=NULL || pDoc->m_pPixelset!=NULL)
	{
		((COIIIApp*)AfxGetApp())->OnSettingsHisto();
		COWPropertyPage* pOWPropertyPage = (COWPropertyPage*)(((COIIIApp*)AfxGetApp())->m_pSettingsSheet)->GetActivePage();
		pOWPropertyPage->UpdateSettingsFromDocToSheet();

		//gather context info
		globalstructCONTEXTINFO.idROI = -1;
		if(pDoc->m_pROIset)
		{
			globalstructCONTEXTINFO.idROI = pDoc->m_pROIset->GetSelectedROI();
		}
		globalstructCONTEXTINFO.idVertex = -1;
		globalstructCONTEXTINFO.idSegment = -1;
		//post message
		//2020july03, spi, begin
		//((COIIIApp*)AfxGetApp())->m_pSettingsSheet->PostMessage(OWM_MOUSEMOVED_UPDATELOCALGRAPH, (DWORD)&globalstructCONTEXTINFO);
		((COIIIApp*)AfxGetApp())->m_pSettingsSheet->PostMessage(OWM_MOUSEMOVED_UPDATELOCALGRAPH, (LONG_PTR)&globalstructCONTEXTINFO);
		//2020july03, spi, end
		((COIIIApp*)AfxGetApp())->m_pSettingsSheet->ShowWindow(SW_SHOW);
	}
	return; 
}



BOOL COW2View::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    //TODO, should this be COWView?
    if (nHitTest != HTCLIENT)
	return CScrollView::OnSetCursor(pWnd,nHitTest,message);
    
    switch (m_owviewMode)
	{	
		case ZOOMINPOINT:
			::SetCursor(m_hZoomInPointCursor);
			return TRUE;
		
		case ZOOMOUTPOINT:		
			::SetCursor(m_hZoomOutPointCursor);
			return TRUE;
		
		case ZOOMRECT:
			if (m_bCaptured) ::SetCursor(m_hZoomRectDragCursor);
				else ::SetCursor(m_hZoomRectCursor);
			return TRUE;
		
		case DRAWELLIPSE:
			if (m_bCaptured) ::SetCursor(m_hDrawEllipseDragCursor);
				else ::SetCursor(m_hDrawEllipseCursor);
			return TRUE;

		case NORMAL: 
			
			if(m_bMode_ROIFloating==TRUE)
			{
				//Restore to normal
				::SetCursor(::LoadCursor(NULL,IDC_ARROW));
			}
			else if(m_bMode_ROIRect==TRUE)
			{
				//::SetCursor(::LoadCursor(NULL,IDC_ROIRECT));
				if (m_bCaptured) ::SetCursor(m_hROIRectDragCursor);
				  else ::SetCursor(m_hROIRectCursor);
			}
			else if(m_bMode_ROIPolyline==TRUE)
			{
				//::SetCursor(::LoadCursor(NULL,IDC_ROIPOLYLINE));
				if (m_bCaptured) ::SetCursor(m_hROIPolylineDragCursor);
				  else ::SetCursor(m_hROIPolylineCursor);
			}
			else if(m_bMode_ROIFreehand==TRUE)
			{
				//::SetCursor(::LoadCursor(NULL,IDC_ROIPOLYLINE));
				if (m_bCaptured) ::SetCursor(m_hROIFreehandDragCursor);
				  else ::SetCursor(m_hROIFreehandCursor);
			}
			else
			{
				//Restore to normal
				::SetCursor(::LoadCursor(NULL,IDC_ARROW));
			}
			return TRUE;
    }
	
	return COWView::OnSetCursor(pWnd, nHitTest, message);
}


//	
//	DrawPointsetObject
//	
//	Draws the object using XOR pen so that
//	it erases if it draws on itself.
//


void COW2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar==16 && nFlags==42)
	{
		//shift key down
		m_bShiftKeyIsDown = TRUE;
	}
	
	COWView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void COW2View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar==VK_SHIFT && nFlags==49194)
	{
		//shift key up
		m_bShiftKeyIsDown = FALSE;
	}
	 
	//poirier, sept 2001, begin
	if( (nChar==VK_ESCAPE && nFlags==49153) 
		|| (nChar==VK_DELETE && nFlags==49491) ) 
	{
		//escape key is up
		if(m_owviewMode!=NORMAL)
		{
			//disable any zoom mode
			m_owviewMode = NORMAL;
			OnSetCursor(NULL,HTCLIENT,0);
		}
		else
		{
			//in the normal mode ...
			if(m_owviewMode==NORMAL && m_bMode_ROIFloating==TRUE)
			{
				//disable local analysismode
				DisableROIFloatingMode();
			}
			if(m_owviewMode==NORMAL && IsMOUSENearestNeighborsModeEnabled()==TRUE)
			{
				//disable local mouse nearest neighbor mode
				DisableMOUSENearestNeighborsMode();
				if(((COIIIApp*)AfxGetApp())->m_pSettingsSheet)
				{
					COWPropertyPage* pOWPropertyPage = (COWPropertyPage*)(((COIIIApp*)AfxGetApp())->m_pSettingsSheet)->GetActivePage();
					pOWPropertyPage->OnLocalROIcheck(); 
				}
			}
			else if(m_owviewMode==NORMAL && m_bMode_ROIFloating==FALSE)
			{
				//delete selected user roi (if any)
				COW2Doc* pDoc = GetDocument();
				if(pDoc!=NULL && pDoc->m_pROIset!=NULL)
				{
					int idroi = pDoc->m_pROIset->GetSelectedROI();
					if(idroi>3)
					{
						DeleteROI(idroi);
					}
				}

			}
		}
	}
	//poirier, sept 2001, end
	COWView::OnKeyUp(nChar, nRepCnt, nFlags);
}



//poirier, begin, selection rect, imported and adapted from OISS
void COW2View::InflateSelectionRect()
{
	CPoint point(m_TempPoint);
	//increase selection rect by 1%
	m_SelectionRectWidth += m_SelectionRectWidth/10;
	m_SelectionRectHeight += m_SelectionRectHeight/10;
	if(m_SelectionRectWidth>m_SelectionRectWidthMax)
	{
		m_SelectionRectWidth = m_SelectionRectWidthMax;
	}
	if(m_SelectionRectHeight>m_SelectionRectHeightMax)
	{
		m_SelectionRectHeight = m_SelectionRectHeightMax;
	}
	if(m_SelectionRectWidth==m_SelectionRectWidthMax
		&& m_SelectionRectHeight==m_SelectionRectHeightMax)
	{
		if(m_wInflatingTimerOn) 
		{
			//kill inflating timer
			KillTimer(m_nInflatingTimerID);
		}
	}
    CClientDC dc(this);
    OnPrepareDC(&dc);
	CRect thisSelectionRect(point.x-m_SelectionRectWidth/2, 
							point.y-m_SelectionRectHeight/2, 
							point.x+m_SelectionRectWidth/2, 
							point.y+m_SelectionRectHeight/2);
	if(!m_PrevSelectionRect.IsRectEmpty()) EraseSelectionRect(&dc,&m_PrevSelectionRect);	 //Erase
	DrawSelectionRect(&dc,&thisSelectionRect);	// Draw
	m_PrevSelectionRect = thisSelectionRect;
}

void COW2View::DeflateSelectionRect()
{
	CPoint point(m_TempPoint);
	//reduce selection rect by 1%
	m_SelectionRectWidth -= m_SelectionRectWidth/10;
	m_SelectionRectHeight -= m_SelectionRectHeight/10;
	if(m_SelectionRectWidth<m_SelectionRectWidthMin)
	{
		m_SelectionRectWidth = m_SelectionRectWidthMin;
	}
	if(m_SelectionRectHeight<m_SelectionRectHeightMin)
	{
		m_SelectionRectHeight = m_SelectionRectHeightMin;
	}
	if(m_SelectionRectWidth==m_SelectionRectWidthMin
		&& m_SelectionRectHeight==m_SelectionRectHeightMin)
	{
		if(m_wDeflatingTimerOn) 
		{
			//kill inflating timer
			KillTimer(m_nDeflatingTimerID);
		}
	}

    CClientDC dc(this);
    OnPrepareDC(&dc);
	CRect thisSelectionRect(point.x-m_SelectionRectWidth/2, 
							point.y-m_SelectionRectHeight/2, 
							point.x+m_SelectionRectWidth/2, 
							point.y+m_SelectionRectHeight/2);
	if(!m_PrevSelectionRect.IsRectEmpty()) EraseSelectionRect(&dc,&m_PrevSelectionRect);	 //Erase
	DrawSelectionRect(&dc,&thisSelectionRect);	// Draw
	m_PrevSelectionRect = thisSelectionRect;

	//poirier, sept 2001, begin
	COW2Doc* pDoc = GetDocument();
	if(pDoc!=NULL && pDoc->m_pROIset!=NULL)
	{
		pDoc->m_pROIset->ModifyRectangularROI(EDGESET_SELECTION_RECT_ROI_ID, &thisSelectionRect);
	}
	//poirier, sept 2001, end
}

void COW2View::OnTimer(UINT_PTR nIDEvent) 
{
	if(nIDEvent==m_nInflatingTimerID)
	{
		InflateSelectionRect();
	}
	else if(nIDEvent==m_nDeflatingTimerID)
	{
		DeflateSelectionRect();
	}
	else if(nIDEvent==m_nDelayingOpenSettingsDlgTimerID)
	{
		OpenSettingsDlg();
	}
	else if(nIDEvent==m_nTransSpectralFilterTimerID)
	{
		GetDocument()->TransSpectralFilter(m_nTransSpectralFilterIndex);
		m_nTransSpectralFilterIndex++;
		if(m_nTransSpectralFilterIndex==10) //max number of ipl fixed filters
		{
			KillTimer(m_nTransSpectralFilterTimerID);
			m_wTransSpectralFilterTimerOn = FALSE;
			m_nTransSpectralFilterIndex = -1;
			GetDocument()->TransSpectralFilter(m_nTransSpectralFilterIndex); //to free temporary IPL buffer
		}
	}
	else
	{
		COWView::OnTimer(nIDEvent);
	}
	
	//COWView::OnTimer(nIDEvent);
}
//poirier, end, selection rect, imported and adapted from OISS


BOOL COW2View::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	//poirier, begin, selection rect, imported and adapted from OISS
    CClientDC dc(this);
    OnPrepareDC(&dc);
	if(!m_PrevSelectionRect.IsRectEmpty()) EraseSelectionRect(&dc,&m_PrevSelectionRect);	 //Erase
	m_PrevSelectionRect.SetRectEmpty();
	//poirier, end, selection rect, imported and adapted from OISS

	//poirier, sept 2001, begin
	COW2Doc* pDoc = GetDocument();
	if(pDoc->m_pROIset!=NULL)
	{
		int idroi_selected = pDoc->m_pROIset->GetSelectedROI();
		//when no user roi selected
		if(idroi_selected<4)
		{
			//update view rect roi and select it
			UpdateROI(EDGESET_VIEWING_RECT_ROI_ID);
			//if this roi was selected
			if(GetDocument()->m_pROIset->GetSelectedROI()==EDGESET_VIEWING_RECT_ROI_ID)
			{
				//to trigger the graph update
				SelectROI(EDGESET_VIEWING_RECT_ROI_ID);
			}
		}
	}
	//poirier, sept 2001, end	
	return COWView::OnScroll(nScrollCode, nPos, bDoScroll);
}

BOOL COW2View::DrawSelectionRect(CDC* pDC, CRect* lprc)
{
	double fZoomFactor = GetZoomFactor();
	if( (fZoomFactor!=1.0 && fZoomFactor!=2.0 && fZoomFactor!=4.0
		&& fZoomFactor!=8.0 && fZoomFactor!=10.0 && fZoomFactor!=20.0)
		|| (fZoomFactor!=1.0 && HasToDrawOverlay()) )
	{
		//use DrawRubberRect() XOR drawing function
		DrawRubberRect(pDC, *lprc);
		return TRUE;
	}
	

	/* moved in OnCreate()
	ASSERT(m_MemoryDC.GetSafeHdc()==NULL);
	BOOL fReturn = m_MemoryDC.CreateCompatibleDC(pDC);
	*/
	BOOL fReturn = TRUE;

	//save image selection rect from pDC into memory dc
	/*
	fReturn &= m_MemoryDC.BitBlt(	lprc->left,
									lprc->top,
									lprc->Width(),
									lprc->Height(),
									pDC,
									lprc->left,
									lprc->top,
									SRCCOPY);
	*/
	/*
	if(m_hDDB>0)
	{
		HDC hDC = myMemoryDC.GetSafeHdc();
		IMG_display_ddb(m_hDDB, hDC, m_ViewOffset.x, m_ViewOffset.y);
	}
	*/
	//create compatible dc and bitmap
	CDC myMemoryDC;
	myMemoryDC.CreateCompatibleDC(pDC);
	//OnPrepareDC(&myMemoryDC);

	ASSERT(m_savedBitmap.m_hObject==NULL);
	if(m_savedBitmap.m_hObject!=NULL)
	{
		m_savedBitmap.DeleteObject();
	}
	m_savedBitmap.CreateCompatibleBitmap(pDC, lprc->Width(), lprc->Height() );
    ASSERT(m_savedBitmap.m_hObject != NULL);
	//bitblt compatible bitmap into memory dc
    CBitmap* pbmOld = myMemoryDC.SelectObject( &m_savedBitmap );
    myMemoryDC.BitBlt( 0, 0, lprc->Width(), lprc->Height(), 
						pDC, lprc->left - m_ViewOffset.x, 
							 lprc->top - m_ViewOffset.y, 
							 SRCCOPY ); //was 0,0
/*
#ifdef _DEBUG
	char buf[1024];
	sprintf(buf, "DrawSelectionRect x,y %d,%d  w,h %d %d\r\n", lprc->left, lprc->top, lprc->Width, lprc->Height());
    ((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText(buf);
#endif
*/
	//delete compatible dc
	myMemoryDC.SelectObject(pbmOld);
	myMemoryDC.DeleteDC();


	//draw selection rect
	CBrush myBrush;
	myBrush.CreateStockObject(NULL_BRUSH);
	CBrush* pOldBrush = pDC->SelectObject(&myBrush);
	//myMemoryDC.FillRect(&rect, &myBrush);

    CPen pen;
	CPen* pOldPen;
	int penwidth_inpixel = 1; 
	m_selection_rect_thickness_inpixel = penwidth_inpixel;
    pen.CreatePen(PS_SOLID,  penwidth_inpixel-1, RGB(255, 0, 0)); // 0 width = 1 device unit
    pOldPen = pDC->SelectObject(&pen);
	//draw rect boundary
    pDC->Rectangle(lprc);    
    pDC->SelectObject(pOldPen);
	pen.DeleteObject();

	pDC->SelectObject(pOldBrush);
	myBrush.DeleteObject();

	return fReturn;
}

BOOL COW2View::EraseSelectionRect(CDC* pDC, CRect* lprc)
{
	ASSERT(pDC);
	
	double fZoomFactor = GetZoomFactor();
	if( (fZoomFactor!=1.0 && fZoomFactor!=2.0 && fZoomFactor!=4.0
		&& fZoomFactor!=8.0 && fZoomFactor!=10.0 && fZoomFactor!=20.0)
		|| (fZoomFactor!=1.0 && HasToDrawOverlay()) )
	{
		//use DrawRubberRect() XOR drawing function
		DrawRubberRect(pDC, *lprc);
	}
	

	CDC myMemoryDC;
	myMemoryDC.CreateCompatibleDC(pDC);
	//OnPrepareDC(&myMemoryDC);

	CBitmap* pbmOld = myMemoryDC.SelectObject(&m_savedBitmap);

	BOOL fReturn;
	
	//erase top edge
	fReturn = pDC->BitBlt(	lprc->left,
							lprc->top,
							lprc->Width(),
							m_selection_rect_thickness_inpixel,
							&myMemoryDC,
							0, //lprc->left,
							0, //lprc->top,
							SRCCOPY);
	//erase left edge
	fReturn &= pDC->BitBlt(	lprc->left, 
							lprc->top,
							m_selection_rect_thickness_inpixel,
							lprc->Height(),
							&myMemoryDC,
							0, //lprc->left,
							0, //lprc->top,
							SRCCOPY);
	//erase right edge
	fReturn &= pDC->BitBlt(	lprc->right - m_selection_rect_thickness_inpixel,
							lprc->top,
							m_selection_rect_thickness_inpixel,
							lprc->Height(),
							&myMemoryDC,
							lprc->Width()-m_selection_rect_thickness_inpixel, //(lprc->right - 1),
							0, //lprc->top,
							SRCCOPY);
	//erase bottom edge
	fReturn &= pDC->BitBlt(	lprc->left,
							lprc->bottom - m_selection_rect_thickness_inpixel,
							lprc->Width(),
							m_selection_rect_thickness_inpixel,
							&myMemoryDC,
							0, //lprc->left,
							lprc->Height()-m_selection_rect_thickness_inpixel, //(lprc->bottom - 1),
							SRCCOPY);
	
	/*
	fReturn = pDC->BitBlt(	lprc->left - m_ViewOffset.x, 
							lprc->top - m_ViewOffset.y,
							lprc->Width(),
							lprc->Height(),
							&myMemoryDC,
							0,
							0,
							SRCCOPY);
	*/
/*
#ifdef _DEBUG
	char buf[1024];
	sprintf(buf, "EraseSelectionRect x,y %d,%d  w,h %d %d\r\n", lprc->left, lprc->top, lprc->Width, lprc->Height());
    ((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText(buf);
#endif
*/
	myMemoryDC.SelectObject(pbmOld);
	m_savedBitmap.DeleteObject();
	ASSERT(m_savedBitmap.m_hObject==NULL);
	myMemoryDC.DeleteDC();
	
	/* //to help debugging
    CPen pen;
	CPen* pOldPen;
    pen.CreatePen(PS_SOLID,  0, RGB(0, 255, 0)); // 0 width = 1 device unit
    pOldPen = pDC->SelectObject(&pen);
	//draw rect boundary
	pDC->MoveTo(lprc->left, lprc->top);
    pDC->LineTo(lprc->right, lprc->top);    
    pDC->SelectObject(pOldPen);
	pen.DeleteObject();
	*/
	return fReturn;
}

void COW2View::DrawTransparentMask(CDC* pDC, CRect &rect)
{
	COW2Doc* pDoc = GetDocument();
	ASSERT(pDoc);
	if(pDoc->m_pPixelset==NULL) return;

	/*
	//temporary, begin, until fix
	if(rect.left%4!=0)
	{
		rect.left += 4 - rect.TopLeft().x%4;
	}
	if(rect.Width()%4!=0)
	{
		rect.right -= 4 - rect.Width()%4;
	}
	//temporary, end
	//create accusoft imghandle
	COWPixelset* pPixelset = new COWPixelset(pDoc->m_pPixelset, &rect, PIXELSET_IGNORE_APP_TEMPINFO);
	int imghandle = pDoc->m_pPixelset->m_imghandle;
	ASSERT(imghandle!=-1);
	//if not monochrome, convert to monochrome
	int width, height, bits_per_pixel;
	IMG_bitmap_info(imghandle, &width, &height, &bits_per_pixel);
	if(bits_per_pixel!=8)
	{
		IMG_color_gray(imghandle);
	}
	*/

    CDC myMemoryDC;
	BOOL bRes = myMemoryDC.CreateCompatibleDC(pDC);
	CBrush myBrush;
	myBrush.CreateStockObject(BLACK_BRUSH);
	CBrush* pOldBrush = myMemoryDC.SelectObject(&myBrush);
	myMemoryDC.FillRect(&rect, &myBrush);
	myMemoryDC.SelectObject(pOldBrush);
	myBrush.DeleteObject();

	UINT crTransparent = RGB(0,0,0);
	int nXOriginDest = rect.left;
	int nYOriginDest = rect.top;
	int nWidthDest = rect.Width();
	int hHeightDest = rect.Height();
	int nXOriginSrc = nXOriginDest;
	int nYOriginSrc = nYOriginDest;
	int nWidthSrc = nWidthDest;
	int nHeightSrc = hHeightDest;
	/* //can't call this function VC98
	BOOL b = TransparentBlt(pDC->GetSafeHdc(), 
							nXOriginDest,
							nYOriginDest,
							nWidthDest,
							hHeightDest,
							myMemoryDC.GetSafeHdc(),
							nXOriginSrc,
							nYOriginSrc,
							nWidthSrc,
							nHeightSrc);
	*/
	myMemoryDC.DeleteDC();
}

//	
//	DrawRubberRect
//	
//	Draws a rectangle using XOR pen so that
//	it erases if it draws on itself.
//
void COW2View::DrawRubberRect(CDC * pDC, CRect &rect)
{
    CPen pen;
	CPen* pOldPen;
	int nOldDrawingMode;
    // Save the device context for modes
    pDC->SaveDC();
    nOldDrawingMode = pDC->SetROP2(R2_NOTXORPEN); //XOR mode, always have to erase what's drawn.
    
    pen.CreatePen(PS_DASH,  0, RGB(0, 0, 0)); // 0 width = 1 device unit
    pOldPen = pDC->SelectObject(&pen);

	/*
	//draw rect interior transparent
	DrawTransparentMask(pDC, rect);
	*/

	//draw rect boundary
    pDC->Rectangle(&rect);
    
    pDC->SelectObject(pOldPen);
    pDC->SetROP2(nOldDrawingMode); 
    pDC->RestoreDC(-1);
    return;
}

//	
//	DrawRubberEllipse
//	
//	Draws an ellipse using XOR pen so that
//	it erases if it draws on itself.
//
void COW2View::DrawRubberEllipse(CDC * pDC, CRect &rect)
{
    CPen pen;
	CPen* pOldPen;
	int nOldDrawingMode;
    // Save the device context for modes
    pDC->SaveDC();
    nOldDrawingMode = pDC->SetROP2(R2_NOTXORPEN); //XOR mode, always have to erase what's drawn.
    
    pen.CreatePen(PS_DASH,  0, RGB(0, 0, 0)); // 0 width = 1 device unit
    pOldPen = pDC->SelectObject(&pen);

	/*
	//draw rect interior transparent
	DrawTransparentMask(pDC, rect);
	*/

	//draw circle boundary
    pDC->Ellipse(&rect);
    
    pDC->SelectObject(pOldPen);
    pDC->SetROP2(nOldDrawingMode); 
    pDC->RestoreDC(-1);
    return;
}

//	
//	DrawRubberLine
//	
//	Draws a rectangle using XOR pen so that
//	it erases if it draws on itself.
//
void COW2View::DrawRubberLine(CDC * pDC, CRect &rect)
{
    CPen pen;
	CPen* pOldPen;
	int nOldDrawingMode;
    // Save the device context for modes
    pDC->SaveDC();
    nOldDrawingMode = pDC->SetROP2(R2_NOTXORPEN); //XOR mode, always have to erase what's drawn.
    
    pen.CreatePen(PS_DASH,  0, RGB(0, 0, 0)); // 0 width = 1 device unit
    pOldPen = pDC->SelectObject(&pen);

	/*
	//draw rect interior transparent
	DrawTransparentMask(pDC, rect);
	*/

	//draw rect boundary
	CPoint point1(rect.left, rect.top);
	CPoint point2(rect.right, rect.bottom);
    pDC->MoveTo(point1);
	pDC->LineTo(point2);
    
    pDC->SelectObject(pOldPen);
    pDC->SetROP2(nOldDrawingMode); 
    pDC->RestoreDC(-1);
    return;
}

void COW2View::DrawRubberFillRect(CDC* pDC, CRect &rect)
{
    CPen pen;
	CPen* pOldPen;
	//int nOldDrawingMode;
    // Save the device context for modes
    //pDC->SaveDC();
    //nOldDrawingMode = pDC->SetROP2(R2_NOTXORPEN); //XOR mode, always have to erase what's drawn.
    
    pen.CreatePen(PS_SOLID,  0, RGB(255, 0, 0)); // 0 width = 1 device unit
    pOldPen = pDC->SelectObject(&pen);
	//no brush
    CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH); 

	/*
	//draw rect interior transparent
	DrawTransparentMask(pDC, rect);
	*/

	//draw rect boundary
	CRect myRect(&rect);
	if(myRect.Width()==0) myRect.right++;
	else if(myRect.Height()==0) myRect.bottom++;

	ASSERT(myRect.Width()!=0 && myRect.Height()!=0);
    pDC->Rectangle(&myRect);

    pDC->SelectObject(pOldBrush);   
    pDC->SelectObject(pOldPen);
    //pDC->SetROP2(nOldDrawingMode); 
    //pDC->RestoreDC(-1);
    return;
}

void COW2View::InitialUpdateEDGESET_ROI()
{
	COW2Doc* pDoc = GetDocument();
	ASSERT(pDoc);

	//set smallest world rect
	if(pDoc->m_pROIset)
	{
		RECT rect = pDoc->GetSmallestWorldRect();
		pDoc->m_pROIset->ModifyRectangularROI(EDGESET_SMALLEST_WORLDRECT_ROI_ID, &rect);
		//set largest world rect
		rect = pDoc->GetLargestWorldRect();
		pDoc->m_pROIset->ModifyRectangularROI(EDGESET_LARGEST_WORLDRECT_ROI_ID, &rect);	
		//set viewing rect
		rect = pDoc->GetViewingWorldRect();
		pDoc->m_pROIset->ModifyRectangularROI(EDGESET_VIEWING_RECT_ROI_ID, &rect);
		//set selection rect
		rect = pDoc->GetSelectionWorldRect(); //initially, full viewing rect
		pDoc->m_pROIset->ModifyRectangularROI(EDGESET_SELECTION_RECT_ROI_ID, &rect);

		//select viewing rect roi
		pDoc->m_pROIset->SelectROI(EDGESET_VIEWING_RECT_ROI_ID);
	}
	
	
}

void COW2View::UpdateROI(int idroi)
{
	//poirier, sept 2001, begin
	//update offset point (view origin)
	CRect myClientRect;
	if(m_parentHWND==NULL) //spi 2014
	{
		GetClientRect(&myClientRect);

		CClientDC dc(this);
		OnPrepareDC(&dc);
		/*
		CPoint point(0,0);
		dc.DPtoLP(&point);
		CPoint myViewOffset(point);
		//update view rect
		CRect myViewRect(myClientRect);
		myViewRect.OffsetRect(myViewOffset.x, myViewOffset.y);
		*/
		CRect myViewRect(myClientRect);
		dc.DPtoLP(&myViewRect);
		if(!myViewRect.IsRectEmpty() && GetDocument()->m_pROIset!=NULL)
		{
			GetDocument()->m_pROIset->ModifyRectangularROI(idroi, &myViewRect);
		}
		/*
		#ifdef _DEBUG 
			char pTxt[256];
			wsprintf(pTxt, "\r\nView rect updated to (left, top, right, bottom): %d, %d, %d, %d\r\n", myViewRect.left, myViewRect.top, myViewRect.right, myViewRect.bottom);
			((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText(pTxt);
			wsprintf(pTxt, "View offset is (left, top): %d, %d\r\n", myViewOffset.x, myViewOffset.y);
			((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText(pTxt);
		#endif //_DEBUG
		*/
		//poirier, sept 2001, end
	}
	else
	{
		RECT myRECT;
		::GetClientRect(m_parentHWND, &myRECT);
		myClientRect=myRECT;
		//spi 2014
		//not implemented yet, translate code here above
		//2021dec28, spi, begin
		//quick and dirty temporary fix
		//ASSERT(FALSE);
		CRect myViewRect(myClientRect);
		if (!myViewRect.IsRectEmpty() && GetDocument()->m_pROIset != NULL)
		{
			GetDocument()->m_pROIset->ModifyRectangularROI(idroi, &myViewRect);
		}
		//2021dec28, spi, end
	}
}

void COW2View::SelectROI(int idroi) 
{
	if(m_parentHWND==NULL) //spi 2014
	{
		//select roi (in the document)
		COW2Doc* pDoc = GetDocument();
		if(pDoc->m_pROIset!=NULL)
		{
			/*
			int idroi_selected = pDoc->m_pROIset->GetSelectedROI();
			CRect rect_selected = pDoc->m_pROIset->GetRectROI(idroi_selected);
			CRect rect = pDoc->m_pROIset->GetRectROI(idroi);
			if(rect == rect_selected) 
			{
				return;
			}
			else
			{
			*/
				//if previous selected was a user roi
				int idroi_selected = pDoc->m_pROIset->GetSelectedROI();
				if(idroi_selected>3)
				{
					//invalidate to erase it	
					//InvalidateROI(idroi_selected, FALSE);
					if(m_pViewDataSettings->bDrawPixelset) InvalidateROI(idroi_selected, FALSE);
					  else InvalidateROI(idroi_selected, TRUE);
				}

				//select roi for the document
				pDoc->m_pROIset->SelectROI(idroi);

				//if user roi
				if(idroi>3)
				{
					//invalidate to draw
					InvalidateROI(idroi, FALSE);
				}
				//update view
				UpdateWindow();

				/*
				//update new document selection rect, adjusted selection rect (pixel width and height set to multiples of 4 to facilitate creation of new image buffer)
				CRect rect(pDoc->m_pROIset->GetRectROI(idroi));
				if(rect.left%4!=0) rect.left += 4 - rect.left%4;
				if(rect.Width()%4!=0) rect.right -= rect.Width()%4;
				((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_worldrect= rect; 
				((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pDoc = GetDocument(); 
				((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pPixelset = NULL;
				*/

				//select roi (for the profile, histo or scatter graph objects)
				globalstructCONTEXTINFO.idROI = idroi;
				globalstructCONTEXTINFO.idVertex = -1;
				globalstructCONTEXTINFO.idSegment = -1;


				//update all (dependent) views
				BOOL bLocalGraphUpdateRequired; 
				bLocalGraphUpdateRequired = (((COIIIApp*)AfxGetApp())->IsSettingsSheetGraphUpdateRequired());
				if(bLocalGraphUpdateRequired && (pDoc->m_pPointset!=NULL || pDoc->m_pPixelset!=NULL) )
				{
					//2020july03, spi, begin
					//((COIIIApp*)AfxGetApp())->m_pSettingsSheet->PostMessage(OWM_MOUSEMOVED_UPDATELOCALGRAPH, (DWORD)&globalstructCONTEXTINFO);
					((COIIIApp*)AfxGetApp())->m_pSettingsSheet->PostMessage(OWM_MOUSEMOVED_UPDATELOCALGRAPH, (LONG_PTR)&globalstructCONTEXTINFO);
					//2020july03, spi, end
				}
			/*
			}
			*/
		}
	}
	else
	{
		//spi 2014
		//todo: not implemented yet
		//2021dec28, spi, begin
		//ASSERT(FALSE);
		//2021dec28, spi end
	}

}

void COW2View::InvalidateROI(int idroi, BOOL bEraseBackGround) 
{
	if(m_parentHWND==NULL) //spi 2014
	{
		COW2Doc* pDoc = GetDocument();
		if(pDoc->m_pROIset==NULL)
		{
			ASSERT(FALSE);
			return;
		}

		//get roi bounding rect
		CRect myRect = pDoc->m_pROIset->GetRectROI(idroi);
	
		//in case roi is selected (or was selected)
		CSize sizeExtra(4,4); //4x4 logical units, for corners when selected
		/*
		int idroi_selected = pDoc->m_pROIset->GetSelectedROI();
		if(idroi==idroi_selected)
		{
		}
		*/

		if(m_pViewDataSettings->bDrawPointset ||
			m_pViewDataSettings->bDrawTriangleset || 
			m_pViewDataSettings->bDrawVoronoiset)
		{
			//for now, simply increase the rect by poinset typical voronoi region size
			sizeExtra.cx = 10;
			sizeExtra.cy = 10;
		}
		sizeExtra.cx = 10;
		sizeExtra.cy = 10;

		CClientDC dc(this);
		OnPrepareDC(&dc);
		dc.LPtoDP(&myRect);
		dc.LPtoDP(&sizeExtra);
		myRect.InflateRect(sizeExtra.cx,sizeExtra.cy);
		/*
		if(pDoc->m_pPointset!=NULL && m_pViewDataSettings->bDrawPixelset==TRUE)
		{
			InvalidateRect(&myRect, FALSE);
		}
		else
		{
			InvalidateRect(&myRect, TRUE); //erase background if there is no image to bitblt()
		}
		*/
		InvalidateRect(&myRect, bEraseBackGround);
	}
	else
	{
		//spi 2014
		//todo: not implemented yet
		ASSERT(FALSE);
	}
	return;
}

void COW2View::DeleteROI(int idroi)
{
	if(m_parentHWND==NULL) //spi 2014
	{
		if(idroi<4)
		{
			ASSERT(FALSE); //do not delete default roi
			return;
		}
		COW2Doc* pDoc = GetDocument();
		//invalidate, to erase
		CRect myRect = pDoc->m_pROIset->GetRectROI(idroi);
		myRect.InflateRect(4,4);
		CClientDC dc(this);
		OnPrepareDC(&dc);
		dc.LPtoDP(&myRect);
		//2021fev08, spi, begin
		//InvalidateRect(&myRect, FALSE);
		InvalidateRect(&myRect, TRUE);
		//before deleting roi, unselect roi (at the document level)
		int idroi_selected = pDoc->m_pROIset->GetSelectedROI();
		if (idroi_selected == idroi)
		{
			//select roi (at the document level)
			pDoc->m_pROIset->SelectROI(EDGESET_VIEWING_RECT_ROI_ID);
		}
		//2021fev08, spi, end

		//delete roi
		pDoc->m_pROIset->DeleteROI(idroi);

		//select next roi
		int nTotalROI = pDoc->m_pROIset->GetNumberOfROI();
		if(idroi==nTotalROI) idroi--; //last one
		if(idroi<4) idroi = EDGESET_VIEWING_RECT_ROI_ID; 
		SelectROI(idroi);
	
		//update view
		UpdateWindow();
	}
	else
	{
		//spi 2014
		//todo: not implemented yet
		ASSERT(FALSE);
	}
	return;
}


void COW2View::OnSize(UINT nType, int cx, int cy) 
{
	m_bRendered = FALSE;
	
	//poirier, begin, selection rect, imported and adapted from OISS
	CRect myClientRect;
	GetClientRect(&myClientRect);
	//CClientDC dc(this);
	//OnPrepareDC(&dc);
	//dc.DPtoLP(&myClientRect);
 	m_SelectionRectWidthMin = myClientRect.Width()/30; //was /10
	m_SelectionRectWidthMax = myClientRect.Width()/2;
	m_SelectionRectHeightMin = myClientRect.Height()/20; //was /10
	m_SelectionRectHeightMax = myClientRect.Height()/2;
	//if(m_SelectionRectWidth>m_SelectionRectWidthMax) m_SelectionRectWidth = m_SelectionRectWidthMax;
	//if(m_SelectionRectHeight>m_SelectionRectHeightMax) m_SelectionRectHeight = m_SelectionRectHeightMax;
	//poirier, end, selection rect, imported and adapted from OISS	
	
	//poirier, sept 2001, begin
	UpdateROI(EDGESET_VIEWING_RECT_ROI_ID);
	//if this roi was selected
	if(GetDocument()->m_pROIset->GetSelectedROI()==EDGESET_VIEWING_RECT_ROI_ID)
	{
		//to trigger the graph update
		//2021fev08, spi, begin
		//commented out to prevent full redraw
		//SelectROI(EDGESET_VIEWING_RECT_ROI_ID);
		//todo, trigger a graph update in some other way
		//2021fev08, spi, end
	}
	//poirier, sept 2001, end

	COWView::OnSize(nType, cx, cy);	
}

//poirier, sept 2001, begin
void COW2View::OnNcMouseMove( UINT nHitTest, CPoint point )
{
	/*
	EraseSelectionRectAndDisableROIFloatingMode(this);
	*/
	//give a change to the mouse pointer to make it inside the ow2view client area once
	//when getting from the menuitem to inside the view for the first time
	if(m_PrevSelectionRect.IsRectEmpty()) return;

	ROIFloatingMode_EraseSelectionRect();
	return;
}
//poirier, sept 2001, end

//to be called from outside this view
void COW2View::ROIFloatingMode_EraseSelectionRect()
{
	//ASSERT(m_bMode_ROIFloating);
	//poirier, begin, selection rect, imported and adapted from OISS
	CClientDC dc(this);
	OnPrepareDC(&dc);
	if(!m_PrevSelectionRect.IsRectEmpty()) EraseSelectionRect(&dc,&m_PrevSelectionRect);	 //Erase
	m_PrevSelectionRect.SetRectEmpty();
	//poirier, end, selection rect, imported and adapted from OISS	
}




/*
void COW2View::EraseSelectionRectAndDisableROIFloatingMode(CWnd* pCalledFromWnd)
{
	if(m_bMode_ROIFloating)
	{
		//if called from COW2View
		if(pCalledFromWnd==NULL || (pCalledFromWnd->IsKindOf(RUNTIME_CLASS(COW2View)) && (COW2View*)pCalledFromWnd==this))
		{
			//poirier, sept 2001, begin
			//give a change to the mouse pointer to make it inside the ow2view client area once
			//when getting from the menuitem to inside the view for the first time
			if(m_PrevSelectionRect.IsRectEmpty())
			{
				return;
			}
			//poirier, sept 2001, end
		}

		//poirier, begin, selection rect, imported and adapted from OISS
		CClientDC dc(this);
		OnPrepareDC(&dc);
		if(!m_PrevSelectionRect.IsRectEmpty()) EraseSelectionRect(&dc,&m_PrevSelectionRect);	 //Erase
		m_PrevSelectionRect.SetRectEmpty();
		//poirier, end, selection rect, imported and adapted from OISS	
	}
}
*/


void COW2View::LocalTextureExtraction(CRect selectionrect)
{
	//output result into existing document
	COW2Doc* pNewDoc = (COW2Doc*) new COW2Doc();
	pNewDoc->m_bLocalTextureExtrationStarted = TRUE;
	if(pNewDoc)
	{
		COW2Doc* pDoc = GetDocument();
		CRect erase_rectDevice(0,0,0,0);
		if(pDoc->m_pPointset!=NULL)
		{
			//store previous pointset rect to later erase it
			CClientDC dc(this);
			OnPrepareDC(&dc);
			CRect prev_pointsetrect(pDoc->GetPointsetRect());
			POINT2DD topleft_inworld;
			topleft_inworld.x = (double)prev_pointsetrect.TopLeft().x;
			topleft_inworld.y = (double)prev_pointsetrect.TopLeft().y;
			CPoint leftbottom_inview = MapPointFromWorldToView(topleft_inworld);
			POINT2DD bottomright_inworld;
			bottomright_inworld.x = (double)prev_pointsetrect.BottomRight().x;
			bottomright_inworld.y = (double)prev_pointsetrect.BottomRight().y;
			CPoint righttop_inview = MapPointFromWorldToView(bottomright_inworld);
			dc.LPtoDP(&leftbottom_inview);
			dc.LPtoDP(&righttop_inview);
			
			erase_rectDevice.left = leftbottom_inview.x;
			erase_rectDevice.top = righttop_inview.y;
			erase_rectDevice.right = righttop_inview.x;
			erase_rectDevice.bottom = leftbottom_inview.y;

			int extra = 10; //to inflate invalidated rect
			erase_rectDevice.InflateRect(extra,extra);
			/*
			erase_rectDevice.left = topleft_inview.x - extra;
			erase_rectDevice.top = topleft_inview.y - extra;
			erase_rectDevice.right = bottomright_inview.x + extra;
			erase_rectDevice.bottom = bottomright_inview.y + extra;
			*/
		}
		//compute new pointset
		CRect worldrect(pDoc->GetWorldRect());
		if(selectionrect!=worldrect)
		{
			pNewDoc->OnNewDocumentFromAppTempInfo();
			pNewDoc->OnAnalysisTextureextraction();
			if(pNewDoc->m_pPointset!=NULL)
			{
				BOOL b = pDoc->CopyPointset(pNewDoc->m_pPointset);
				CPoint point_inview(selectionrect.left, selectionrect.bottom);
				POINT2DD point_inworld = MapPointFromViewToWorld(point_inview);
				pDoc->OffsetPointsetRect(point_inworld.x, point_inworld.y);
				delete pNewDoc;
				pNewDoc = NULL;
				if(b)
				{
					//update view
					if(m_pViewDataSettings->bDrawPointset==FALSE &&
					m_pViewDataSettings->bDrawVoronoiset==FALSE &&
					m_pViewDataSettings->bDrawTriangleset==FALSE)
					{
						m_pViewDataSettings->bDrawPointset = TRUE;
					}
					//m_bMode_ROIFloating = FALSE;
					/*
					CClientDC dc(this);
					OnPrepareDC(&dc);
					CPoint topleft(selectionrect.TopLeft());
					CPoint bottomright(selectionrect.BottomRight());
					dc.LPtoDP(&topleft);
					dc.LPtoDP(&bottomright);
					int extra = 10; //to inflate invalidated rect
					CRect rectDevice(topleft.x - extra, topleft.y - extra, bottomright.x + extra, bottomright.y + extra);
					*/
					CClientDC dc(this);
					OnPrepareDC(&dc);
					CRect rectDevice(selectionrect);
					dc.LPtoDP(&rectDevice);
					rectDevice.InflateRect(10,10);
					InvalidateRect(erase_rectDevice); //erase
					InvalidateRect(rectDevice);
				}
			}
		}
	}
	if(pNewDoc)
	{
		pNewDoc->m_bLocalTextureExtrationStarted = FALSE;
		delete pNewDoc;
	}
}

void COW2View::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{	
	COW2Doc* pDoc = GetDocument();
	if(bActivate)
	{
		if(pActivateView && pActivateView->IsKindOf(RUNTIME_CLASS(COW2View))) 
		{
			double df = ((COW2View*)pActivateView)->GetZoomFactor()*100 + 0.5;
			int i = (int) df;
			CString myString;
			myString.Format(L"%d", i);
			ComboSetText(myString);

			UpdateROI(EDGESET_VIEWING_RECT_ROI_ID);
			/*
			//select roi for the document, not triggering graph update yet
			pDoc->SelectROI(EDGESET_VIEWING_RECT_ROI_ID);
			*/
			if(pDoc->m_pROIset->GetSelectedROI()==EDGESET_VIEWING_RECT_ROI_ID)
			{
				//to trigger the graph update
				SelectROI(EDGESET_VIEWING_RECT_ROI_ID);
			}
		}
	}
	BOOL bDontDoThisView = FALSE;
	CView* pView = this;
	if(pActivateView!=NULL)
	{
		pView = pActivateView;
		if(pView==this)
		{
			bDontDoThisView = TRUE;
		}
	}
	if(pView->IsKindOf(RUNTIME_CLASS(COW2View)))
	{
		COW2View* pOW2View = (COW2View*)pView;
		if(!pOW2View->m_PrevSelectionRect.IsRectEmpty()) 
		{
			CClientDC dc(pOW2View);
			pOW2View->OnPrepareDC(&dc);
			pOW2View->EraseSelectionRect(&dc,&pOW2View->m_PrevSelectionRect);//Erase
			pOW2View->InvalidateRect(pOW2View->m_PrevSelectionRect);
			pOW2View->m_PrevSelectionRect.SetRectEmpty();
		}
	}

	if(pDeactiveView!=NULL)
	{
		pView = pDeactiveView;
		if(pView==this)
		{
			bDontDoThisView = TRUE;
		}
	}
	if(pView->IsKindOf(RUNTIME_CLASS(COW2View)))
	{
		COW2View* pOW2View = (COW2View*)pView;
		if(!pOW2View->m_PrevSelectionRect.IsRectEmpty()) 
		{
			CClientDC dc(pOW2View);
			pOW2View->OnPrepareDC(&dc);
			pOW2View->EraseSelectionRect(&dc,&pOW2View->m_PrevSelectionRect);//Erase
			pOW2View->InvalidateRect(pOW2View->m_PrevSelectionRect);
			pOW2View->m_PrevSelectionRect.SetRectEmpty();
		}
	}
	
	if(!bDontDoThisView)
	{
		COW2View* pOW2View = this;
		if(!pOW2View->m_PrevSelectionRect.IsRectEmpty()) 
		{
			CClientDC dc(pOW2View);
			pOW2View->OnPrepareDC(&dc);
			pOW2View->EraseSelectionRect(&dc,&pOW2View->m_PrevSelectionRect);//Erase
			pOW2View->InvalidateRect(pOW2View->m_PrevSelectionRect);
			pOW2View->m_PrevSelectionRect.SetRectEmpty();
		}
	}
	COWView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void COW2View::OnAnalysisLocalextraction() 
{
	/*
	if(m_bMode_ROIFloating) 
	{
		//m_bMode_ROIFloating = FALSE;
		//m_bOnMouseMoveLocalExtractionON = FALSE;
		DisableROIFloatingMode();
	}
	else 
	{
		//m_bMode_ROIFloating = TRUE;
		//m_bOnMouseMoveLocalExtractionON = FALSE;
		EnableROIFloatingMode();
	}
	*/
}

BOOL COW2View::IsMOUSENearestNeighborsModeEnabled()
{
	COW2Doc* pDoc = GetDocument();
	if(pDoc->m_pBehaviorSettings->bMBHighlightNearestNeighbors==TRUE
		&& pDoc->m_pBehaviorSettings->bMBHighlightNearestPoint==TRUE)
	{
		return TRUE;
	}
	return FALSE;
}

void COW2View::EnableMOUSENearestNeighborsMode()
{
	COW2Doc* pDoc = GetDocument();
	pDoc->m_pBehaviorSettings->bMBHighlightNearestNeighbors = TRUE;
	pDoc->m_pBehaviorSettings->bMBHighlightNearestPoint = TRUE;
}

void COW2View::DisableMOUSENearestNeighborsMode()
{
	COW2Doc* pDoc = GetDocument();
	if(pDoc!=NULL)
	{
		pDoc->m_pBehaviorSettings->bMBHighlightNearestNeighbors = FALSE;
		pDoc->m_pBehaviorSettings->bMBHighlightNearestPoint = FALSE;
		//erase previously drawn object
		CClientDC dc(this);
		OnPrepareDC(&dc);
		if(m_idPointsetObject>-1 && pDoc->m_pPointset!=NULL) 
		{
			pDoc->m_pPointset->DrawPointsetObject(this, &dc, m_idPointsetObject); //erase
		}
		//erase previously drawn objects
		if(m_numneighborfound>0)
		{
			for (int i=0; i<m_numneighborfound; i++)
			{
				if(m_pArrayNeighbor[i]>-1) pDoc->m_pPointset->DrawPointsetObject(this, &dc, m_pArrayNeighbor[i]); //erase
			}
		}
		m_idPointsetObject = -1;
		m_numneighborfound = -1;
	}
}

void COW2View::EnableROIFloatingMode()
{
	m_bMode_ROIFloating = TRUE;
	m_bOnMouseMoveLocalExtractionON = FALSE;
	
	//SelectROI(EDGESET_SELECTION_RECT_ROI_ID);
}

void COW2View::DisableROIFloatingMode()
{
	m_bMode_ROIFloating = FALSE;
	m_bOnMouseMoveLocalExtractionON = FALSE;

	ROIFloatingMode_EraseSelectionRect();

	SelectROI(EDGESET_VIEWING_RECT_ROI_ID);
}

void COW2View::EnableROIRectMode()
{
	m_bMode_ROIRect = TRUE;
	m_bOnMouseMoveLocalExtractionON = FALSE;
	
	//SelectROI(EDGESET_SELECTION_RECT_ROI_ID);
}

void COW2View::DisableROIRectMode()
{
	m_bMode_ROIRect = FALSE;
	m_bOnMouseMoveLocalExtractionON = FALSE;

	SelectROI(EDGESET_VIEWING_RECT_ROI_ID);
}

void COW2View::EnableROIPolylineMode()
{
	m_bMode_ROIPolyline = TRUE;
	m_bOnMouseMoveLocalExtractionON = FALSE;
	
	//SelectROI(EDGESET_SELECTION_RECT_ROI_ID);
}

void COW2View::DisableROIPolylineMode()
{
	m_bMode_ROIPolyline = FALSE;
	m_bOnMouseMoveLocalExtractionON = FALSE;

	SelectROI(EDGESET_VIEWING_RECT_ROI_ID);
}

void COW2View::EnableROIFreehandMode()
{
	m_bMode_ROIFreehand = TRUE;
	m_bOnMouseMoveLocalExtractionON = FALSE;
	
	//SelectROI(EDGESET_SELECTION_RECT_ROI_ID);
}

void COW2View::DisableROIFreehandMode()
{
	m_bMode_ROIFreehand = FALSE;
	m_bOnMouseMoveLocalExtractionON = FALSE;

	SelectROI(EDGESET_VIEWING_RECT_ROI_ID);
}


void COW2View::OnUpdateAnalysisLocalextraction(CCmdUI* pCmdUI) 
{
	//pCmdUI->SetCheck(m_bMode_ROIFloating);
}

void COW2View::OnAnalysisLaunchscenario() 
{
	// TODO: Add your command handler code here
	
}

void COW2View::OnUpdateAnalysisLaunchscenario(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void COW2View::OnAnalysisLocalclassification() 
{
	// TODO: Add your command handler code here
	
}

void COW2View::OnUpdateAnalysisLocalclassification(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void COW2View::OnAnalysisLocalsegmentation() 
{
	// TODO: Add your command handler code here
	
}

void COW2View::OnUpdateAnalysisLocalsegmentation(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void COW2View::OnAnalysisTextureextraction() 
{
	GetDocument()->OnAnalysisTextureextraction();
}

void COW2View::OnUpdateAnalysisTextureextraction(CCmdUI* pCmdUI) 
{
	GetDocument()->OnUpdateAnalysisTextureextraction(pCmdUI) ;
}

void COW2View::OnAnalysisTexturesegmentation() 
{
	GetDocument()->OnAnalysisTexturesegmentation() ;
	
}

void COW2View::OnUpdateAnalysisTexturesegmentation(CCmdUI* pCmdUI) 
{
	GetDocument()->OnUpdateAnalysisTexturesegmentation(pCmdUI);
}

void COW2View::OnTransLaunchscenario() 
{
	// TODO: Add your command handler code here
	
}

void COW2View::OnUpdateTransLaunchscenario(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void COW2View::OnTransSpatialConfinedstretch() 
{
	// TODO: Add your command handler code here
	
}

void COW2View::OnUpdateTransSpatialConfinedstretch(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void COW2View::OnTransSpatialRotation() 
{
	// TODO: Add your command handler code here
	
}

void COW2View::OnUpdateTransSpatialRotation(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void COW2View::OnTransSpatialStretch() 
{
	// TODO: Add your command handler code here
	
}

void COW2View::OnUpdateTransSpatialStretch(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void COW2View::OnTransSpectralInvert() 
{
	// TODO: Add your command handler code here
	
}

void COW2View::OnUpdateTransSpectralInvert(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void COW2View::OnTransTemporalEvolve() 
{
	// TODO: Add your command handler code here
	
}

void COW2View::OnUpdateTransTemporalEvolve(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void COW2View::OnTransformationRepeatinsequence() 
{
	// TODO: Add your command handler code here
	
}

void COW2View::OnUpdateTransformationRepeatinsequence(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void COW2View::OnTransSpatialEdgedetect() 
{
	// TODO: Add your command handler code here
	
}

void COW2View::OnUpdateTransSpatialEdgedetect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void COW2View::OnTransSpatialFlatdetect() 
{
	// TODO: Add your command handler code here
	
}

void COW2View::OnUpdateTransSpatialFlatdetect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void COW2View::OnTransSpatialMaxdetect() 
{
	// TODO: Add your command handler code here
	
}

void COW2View::OnUpdateTransSpatialMaxdetect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void COW2View::OnTransSpatialSharpen() 
{
	// TODO: Add your command handler code here
	
}

void COW2View::OnUpdateTransSpatialSharpen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void COW2View::OnTransSpectralEqualize() 
{
	// TODO: Add your command handler code here
	
}

void COW2View::OnUpdateTransSpectralEqualize(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void COW2View::OnTransSpectralHsi() 
{
	// TODO: Add your command handler code here
	
}

void COW2View::OnUpdateTransSpectralHsi(CCmdUI* pCmdUI) 
{
	if(GetDocument()->m_pPixelset!=NULL)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void COW2View::OnTransSpectralStretch() 
{
	// TODO: Add your command handler code here
	
}

void COW2View::OnUpdateTransSpectralStretch(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}


void COW2View::OnAnalysisScaleSelection() 
{
	//enable user define a local scale selection rectangle
	//by clicking, dragging and releasing mouse
	
	//todo, here: turn local scale selection mode ON or OFF
	//toggle it


	//todo within mouse move scope, being
	//1) change cursor (display arrow with rect - much like rect zoom tool)

	//2) variables to update according to the specified local selection rectangle
	/*
	m_SelectionRectWidth = 800/10;//800/5;
	m_SelectionRectHeight = 600/10;//600/5;
	m_SelectionRectWidthMin = 800/10;
	m_SelectionRectWidthMax = 800/2;
	m_SelectionRectHeightMin = 600/10;
	m_SelectionRectHeightMax = 600/2;
	*/
	/*
	m_WorldRect.left = 0;
	m_WorldRect.top = 0;
	m_WorldRect.right = 100;
	m_WorldRect.bottom = 100;
	m_ViewRect = m_WorldRect;
	m_WindowSize.cx = 140;
	m_WindowSize.cy = 140; 
	m_ViewOffset.x = 20;	//relative offset with view window origin
	m_ViewOffset.y = 20;	//relative offset with view window origin
	*/
	//todo within mouse move scope, being
}





void COW2View::OnTransSpectralFilter() 
{
	if(m_wTransSpectralFilterTimerOn==FALSE)
	{
		SetTimer(m_nTransSpectralFilterTimerID, 1000, NULL);
		m_wTransSpectralFilterTimerOn = TRUE;
	}
	else
	{
		KillTimer(m_nTransSpectralFilterTimerID);
		m_wTransSpectralFilterTimerOn = FALSE;
		GetDocument()->TransSpectralFilter(-1); //to free temporary IPL buffer
	}
	return;
}

void COW2View::OnRoiFloating() 
{
	//set mode data member
	if(m_bMode_ROIFloating) 
	{
		DisableROIFloatingMode();
	}
	else 
	{
		EnableROIFloatingMode();
	}
	//set cursor
    m_owviewMode = NORMAL; //zoomnormal
    OnSetCursor(NULL,HTCLIENT,0);
}

void COW2View::OnRoiRect() 
{
	//set mode data member
	if(m_bMode_ROIRect) 
	{
		DisableROIRectMode();
	}
	else 
	{
		EnableROIRectMode();
	}	
	//set cursor
    m_owviewMode = NORMAL; //zoom normal
    OnSetCursor(NULL,HTCLIENT,0);
}

void COW2View::OnRoiPolyline() 
{
	if(m_bMode_ROIPolyline) 
	{
		DisableROIPolylineMode();
	}
	else 
	{
		EnableROIPolylineMode();
	}	
	//set cursor
    m_owviewMode = NORMAL; //zoom normal
    OnSetCursor(NULL,HTCLIENT,0);
}


void COW2View::OnRoiFreehand() 
{
	if(m_bMode_ROIFreehand) 
	{
		DisableROIFreehandMode();
	}
	else 
	{
		EnableROIFreehandMode();
	}	
	//set cursor
    m_owviewMode = NORMAL; //zoom normal
    OnSetCursor(NULL,HTCLIENT,0);
}

void COW2View::OnEditClear() 
{
	COW2Doc* pDoc = GetDocument();
	if(pDoc->m_pROIset)
	{
		int nUserROI = pDoc->m_pROIset->GetNumberOfUserROI();
		if(nUserROI>1)
		{
			//select one user roi (i.e. first one)
			int idroi = pDoc->m_pROIset->SelectROI(4);

			//clear all user roi
			while(idroi>3)
			{
				DeleteROI(idroi);
				idroi = pDoc->m_pROIset->GetSelectedROI();
			}
		}
	}
	if(pDoc->m_pPointset)
	{
		if(pDoc->m_pPointset->m_pFloodFill_Pasted_IDTRIList.GetCount()>0 ||
			pDoc->m_pPointset->m_pFloodFill_IDTRIList.GetCount()>0)
		{
			//this time, only clear floodfill idtri list
			pDoc->m_pPointset->m_pFloodFill_Pasted_IDTRIList.RemoveAll();
			pDoc->m_pPointset->m_pFloodFill_Pasted_nIDTRIList.RemoveAll();
			pDoc->m_pPointset->m_pFloodFill_IDTRIList.RemoveAll();
			pDoc->m_pPointset->m_pFloodFill_nIDTRIList.RemoveAll();
		}
		else
		{
			//clear pointset
			//DeletePointset(pDoc->m_pPointset);
			delete pDoc->m_pPointset;
			pDoc->m_pPointset = NULL;
		}
	}
	Invalidate();
	return;
}

void COW2View::OnZoomp1000() 
{
	SetZoomMode(OW_ZOOMNORMAL); 
    ZoomPercent(1000); //COWView
	OnZoomZoomnormal(); 
}

void COW2View::OnZoomp1500() 
{
	SetZoomMode(OW_ZOOMNORMAL); 
    ZoomPercent(1500); //COWView
	OnZoomZoomnormal(); 
}

void COW2View::OnZoomp2000() 
{
	SetZoomMode(OW_ZOOMNORMAL); 
    ZoomPercent(2000); //COWView
	OnZoomZoomnormal(); 
}

void COW2View::OnZoomp2500() 
{
	SetZoomMode(OW_ZOOMNORMAL); 
    ZoomPercent(2500); //COWView
	OnZoomZoomnormal(); 
}

void COW2View::OnZoomp3000() 
{
	SetZoomMode(OW_ZOOMNORMAL); 
    ZoomPercent(3000); //COWView
	OnZoomZoomnormal(); 
}

void COW2View::OnSettingsProfile() 
{
	COW2Doc* pDoc = GetDocument();
	if(pDoc->m_pROIset)
	{
		int idroi = pDoc->m_pROIset->GetSelectedROI();
		if(idroi>3)
		{
			//user roi selected, roi will be checked
			GetDocument()->m_pProfileSettings->bGlobal = FALSE; //global
			GetDocument()->m_pProfileSettings->bLocal = FALSE; //local around mouse location
			GetDocument()->m_pProfileSettings->bSegment = FALSE; //local segment
			((COIIIApp*)AfxGetApp())->OnSettingsProfile();	
			return;
		}
	}
	GetDocument()->m_pProfileSettings->bGlobal = TRUE; //global
	GetDocument()->m_pProfileSettings->bLocal = FALSE; //local around mouse location
	GetDocument()->m_pProfileSettings->bSegment = FALSE; //local segment

	((COIIIApp*)AfxGetApp())->OnSettingsProfile();	

	COWPropertyPage* pOWPropertyPage = (COWPropertyPage*)(((COIIIApp*)AfxGetApp())->m_pSettingsSheet)->GetActivePage();
	pOWPropertyPage->UpdateSettingsFromDocToSheet();
	return;
}

void COW2View::OnSettingsHisto() 
{
	COW2Doc* pDoc = GetDocument();
	if(pDoc->m_pROIset)
	{
		int idroi = pDoc->m_pROIset->GetSelectedROI();
		if(idroi>3)
		{
			//user roi selected, roi will be checked
			pDoc->m_pHistoSettings->bGlobal = FALSE; //global
			pDoc->m_pHistoSettings->bLocal = FALSE; //local around mouse location
			pDoc->m_pHistoSettings->bSegment = FALSE; //local segment
			((COIIIApp*)AfxGetApp())->OnSettingsHisto();	
			return;
		}
	}
	
	pDoc->m_pHistoSettings->bGlobal = TRUE;
	pDoc->m_pHistoSettings->bLocal = FALSE;
	pDoc->m_pHistoSettings->bSegment = FALSE;
	((COIIIApp*)AfxGetApp())->OnSettingsHisto();

	COWPropertyPage* pOWPropertyPage = (COWPropertyPage*)(((COIIIApp*)AfxGetApp())->m_pSettingsSheet)->GetActivePage();
	pOWPropertyPage->UpdateSettingsFromDocToSheet();
	return;
}

void COW2View::OnSettingsScatter() 
{
	COW2Doc* pDoc = GetDocument();
	if(pDoc->m_pROIset)
	{
		int idroi = pDoc->m_pROIset->GetSelectedROI();
		if(idroi>3)
		{
			//user roi selected, roi will be checked
			pDoc->m_pScatterSettings->bGlobal = FALSE; //global
			pDoc->m_pScatterSettings->bLocal = FALSE; //local around mouse location
			pDoc->m_pScatterSettings->bSegment = FALSE; //local segment
			((COIIIApp*)AfxGetApp())->OnSettingsScatter();
			return;
		}
	}
	pDoc->m_pScatterSettings->bGlobal = TRUE;
	pDoc->m_pScatterSettings->bLocal = FALSE;
	pDoc->m_pScatterSettings->bSegment = FALSE;
	((COIIIApp*)AfxGetApp())->OnSettingsScatter();

	COWPropertyPage* pOWPropertyPage = (COWPropertyPage*)(((COIIIApp*)AfxGetApp())->m_pSettingsSheet)->GetActivePage();
	pOWPropertyPage->UpdateSettingsFromDocToSheet();
	return;
}

void COW2View::OnDestroy() 
{
	COWView::OnDestroy();
	if(m_parentHWND==NULL) //spi 2014
	{
		COIIIApp* pApp = (COIIIApp*)AfxGetApp();
		if(pApp->m_pSettingsSheet!=NULL)
		{
			if(pApp->m_pSettingsSheet->GetAttachedView()==this)
			{
				//set it to null
				pApp->m_pSettingsSheet->SetAttachedView(NULL);
				pApp->m_pSettingsSheet->SetAttachedDoc(NULL);
			}
		}
	}
}


//////////////////////////////////////
// COW2View floating popup menu
//
// the popup menu is ised to set
// the graphsets visibility attributes
//////////////////////////////////////
void COW2View::HandlePopupMenu(CPoint point)
{   
	CMenu myMenu;
	CMenu* pMenu;

    // Get the menu for the popup from the resource file. 
    if (!myMenu.LoadMenu (IDR_OW2VIEW_POPUP)) return; 
	pMenu = myMenu.GetSubMenu(0);

	//set menu state
	COW2Doc* pDoc = (COW2Doc*)GetDocument();	
	ASSERT(pDoc!=NULL);
	if(pDoc->m_pPixelset!=NULL)
	{
		pMenu->EnableMenuItem(ID_VIEW_PIXELSET, MF_BYCOMMAND | MF_ENABLED);
		if(m_pViewDataSettings->bDrawPixelset) pMenu->CheckMenuItem(ID_VIEW_PIXELSET, MF_BYCOMMAND | MF_CHECKED);
		  else pMenu->CheckMenuItem(ID_VIEW_PIXELSET, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else
	{
		pMenu->EnableMenuItem(ID_VIEW_PIXELSET, MF_BYCOMMAND | MF_GRAYED);
	}
	if(pDoc->m_pPointset!=NULL)
	{
		//pointset
		pMenu->EnableMenuItem(ID_VIEW_POINTSET, MF_BYCOMMAND | MF_ENABLED);
		if(m_pViewDataSettings->bDrawPointset) pMenu->CheckMenuItem(ID_VIEW_POINTSET, MF_BYCOMMAND | MF_CHECKED);
		  else pMenu->CheckMenuItem(ID_VIEW_POINTSET, MF_BYCOMMAND | MF_UNCHECKED);
		//pointset labels
		pMenu->EnableMenuItem(ID_VIEW_POINTSET_LABELS, MF_BYCOMMAND | MF_ENABLED);
		if(m_pViewDataSettings->bDrawPointsetLabels) pMenu->CheckMenuItem(ID_VIEW_POINTSET_LABELS, MF_BYCOMMAND | MF_CHECKED);
		  else pMenu->CheckMenuItem(ID_VIEW_POINTSET_LABELS, MF_BYCOMMAND | MF_UNCHECKED);
		//triangleset
		pMenu->EnableMenuItem(ID_VIEW_TRIANGLESET, MF_BYCOMMAND | MF_ENABLED);
		if(m_pViewDataSettings->bDrawTriangleset) pMenu->CheckMenuItem(ID_VIEW_TRIANGLESET, MF_BYCOMMAND | MF_CHECKED);
		  else pMenu->CheckMenuItem(ID_VIEW_TRIANGLESET, MF_BYCOMMAND | MF_UNCHECKED);
		//trianglesset labels
		pMenu->EnableMenuItem(ID_VIEW_POINTSET_LABELS, MF_BYCOMMAND | MF_ENABLED);
		if(m_pViewDataSettings->bDrawTrianglesetLabels) pMenu->CheckMenuItem(ID_VIEW_TRIANGLESET_LABELS, MF_BYCOMMAND | MF_CHECKED);
		  else pMenu->CheckMenuItem(ID_VIEW_POINTSET_LABELS, MF_BYCOMMAND | MF_UNCHECKED);
		//voronoiset
		pMenu->EnableMenuItem(ID_VIEW_VORONOISET, MF_BYCOMMAND | MF_ENABLED);
		if(m_pViewDataSettings->bDrawVoronoiset) pMenu->CheckMenuItem(ID_VIEW_VORONOISET, MF_BYCOMMAND | MF_CHECKED);
		  else pMenu->CheckMenuItem(ID_VIEW_VORONOISET, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else 
	{
		pMenu->EnableMenuItem(ID_VIEW_POINTSET, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_VIEW_POINTSET_LABELS, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_VIEW_TRIANGLESET, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_VIEW_TRIANGLESET_LABELS, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_VIEW_VORONOISET, MF_BYCOMMAND | MF_GRAYED);
	}
	if(pDoc->m_pSegmentset!=NULL)
	{
		//segmentset
		pMenu->EnableMenuItem(ID_VIEW_SEGMENTSET, MF_BYCOMMAND | MF_ENABLED);
		if(m_pViewDataSettings->bDrawSegmentset) pMenu->CheckMenuItem(ID_VIEW_SEGMENTSET, MF_BYCOMMAND | MF_CHECKED);
		  else pMenu->CheckMenuItem(ID_VIEW_SEGMENTSET, MF_BYCOMMAND | MF_UNCHECKED);
		//segmentset labels
		pMenu->EnableMenuItem(ID_VIEW_SEGMENTSET_LABELS, MF_BYCOMMAND | MF_ENABLED);
		if(m_pViewDataSettings->bDrawSegmentsetLabels) pMenu->CheckMenuItem(ID_VIEW_SEGMENTSET_LABELS, MF_BYCOMMAND | MF_CHECKED);
		  else pMenu->CheckMenuItem(ID_VIEW_SEGMENTSET_LABELS, MF_BYCOMMAND | MF_UNCHECKED);
		//classset labels
		pMenu->EnableMenuItem(ID_VIEW_CLASSSET_LABELS, MF_BYCOMMAND | MF_ENABLED);
		if(m_pViewDataSettings->bDrawClasssetLabels) pMenu->CheckMenuItem(ID_VIEW_CLASSSET_LABELS, MF_BYCOMMAND | MF_CHECKED);
		  else pMenu->CheckMenuItem(ID_VIEW_CLASSSET_LABELS, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else 
	{
		pMenu->EnableMenuItem(ID_VIEW_SEGMENTSET, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_VIEW_SEGMENTSET_LABELS, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_VIEW_CLASSSET_LABELS, MF_BYCOMMAND | MF_GRAYED);
	}

    // Convert the mouse point to screen coordinates since that is what TrackPopup expects.
    ClientToScreen((LPPOINT)&point); 	
    // Draw and track the "floating" popup 
    pMenu->TrackPopupMenu ((TPM_LEFTALIGN || TPM_LEFTBUTTON), point.x, point.y, this, NULL);
    
    // Destroy the menu since we are done with it. 
    pMenu->DestroyMenu();
	return;
}

void COW2View::OnFileSave() 
{
	COW2Doc* pDoc = GetDocument();
	pDoc->SetViewOfInterest(this);
	pDoc->OnFileSave();
	return;	
}

void COW2View::OnFileSaveAs() 
{
	COW2Doc* pDoc = GetDocument();
	pDoc->SetViewOfInterest(this);
	pDoc->OnFileSaveAs();
	return;
}

//2012august16, poirier, begin
void COW2View::OnEditCopy()
{
	/////////////////////////////////////
	//for NOW, only copy DIB to clipboard
	/////////////////////////////////////
	COW2Doc* pDoc= (COW2Doc*)GetDocument();
	if( pDoc->m_pPixelset!=NULL &&  pDoc->m_pPixelset->IsDIBPresent())
	{
		// Clean clipboard of contents, and copy the DIB.
		if (OpenClipboard())
		{
			BeginWaitCursor();
			EmptyClipboard();
			SetClipboardData (CF_DIB, (HANDLE) pDoc->m_pPixelset->GetDIBCopy() );
			CloseClipboard();
			EndWaitCursor();
		}
	}
}
//2012august16, poirier, end

void COW2View::OnEditPaste() 
{
	int iresult = OpenClipboard();
	if(iresult)
	{
		//////////////////////////
		//check for clipboard text
		//////////////////////////
		HANDLE hText = GetClipboardData(CF_TEXT);
		if(hText)
		{
			//get clipboard text
			COW2Doc* pDoc = GetDocument();
			if(pDoc->m_pPointset)
			{
				//out of the pasted text, find idtri rect coordinate and zoom in
				char* pszText = (char*) GlobalLock(hText);
				int nText = GlobalSize(hText);
				char* pBuffer = (char*) new char[nText+1];
				memcpy(pBuffer, pszText, nText);
				pszText[nText] = '\0';

				char* pszStop;
				int idtri = strtol(pszText, &pszStop, 10);
				if(idtri>0) pDoc->m_pPointset->m_pFloodFill_Pasted_IDTRIList.AddTail((void*)idtri);
				int ntri = 1;
				while(pszStop[0]!='\0' && pszStop[1]!='\0' && pszStop[2]!='\0' 
					&& idtri!=0)
				{
					pszText = &(pszStop[2]); //2 because skipping a comma and a space ", "
					idtri = strtol(pszText, &pszStop, 10);
					if(idtri>0) pDoc->m_pPointset->m_pFloodFill_Pasted_IDTRIList.AddTail((void*)idtri);
					ntri++;
					//CRgn myRgn;
					//POINT pPOINT[3];
					//pDoc->m_pPointset->GetTrianglePOINTS(idtri, pPOINT);
					//myRgn.CreatePolygonRgn(pPOINT, 3, ALTERNATE);
					//InvalidateRgn(&myRgn, FALSE);
				}
				pDoc->m_pPointset->m_pFloodFill_Pasted_nIDTRIList.AddTail((void*)ntri);
				GlobalUnlock(hText);
				if(iresult) CloseClipboard();

				//idtri
				if(idtri>0 && ntri==1)
				{
					RECT2DD myRECT2DD = pDoc->m_pPointset->GetTriangleBoundingRect(idtri, 1);
					RECT myRECT = MapRectFromWorldToView(myRECT2DD);
					ZoomIn(&myRECT);	
				}
				else
				{
					if(HasToDrawPixel()) Invalidate(FALSE);
					  else Invalidate(TRUE);
				}
			

				//release clipboard text
				GlobalUnlock(hText);
			}
		}

		/////////////////////////
		//check for clipboard DIB
		/////////////////////////
		HANDLE hNewDIB = NULL;
		HANDLE hDIB = GetClipboardData(CF_DIB);
		if (hDIB)
		{
			BITMAPINFOHEADER* pDIB = (BITMAPINFOHEADER*) GlobalLock(hDIB);
			COW2Doc* pOW2Doc = (COW2Doc*)GetDocument();
			//try to copy the DIB
			CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
			COWPixelset* pPixelset = new COWPixelset;

			BeginWaitCursor();
			pLogDocument->AddText("     Initializing pixelset from clipboard DIB ... ");
			if(pPixelset->AllocBII(pDIB)==FALSE)
			{
				pLogDocument->AddText("Error\r\nError, not enough memory to alloc BII buffer, can't perform texture extraction\r\n");
				delete pPixelset;
				return; //error, but returns safely
			}
			pPixelset->UpdateBIIFrom(pDIB);
			pLogDocument->AddText("done.\r\n");
			EndWaitCursor();
			GlobalUnlock(hDIB);	


			//delete existing pixelset, pointset and segmentset
			pOW2Doc->EmptyDocument();
			pOW2Doc->InitDocument();

			//attach the new pointset
			pOW2Doc->m_pPixelset = pPixelset;
			
			//2012july22, poirier, begin
			/*
			//AllocIFF() if not already allocated
			if((pOW2Doc->m_pPixelset->IsBIIPresent() || 
				pOW2Doc->m_pPixelset->IsDIBPresent() ||
				pOW2Doc->m_pPixelset->IsIPLPresent()) && 
				!pOW2Doc->m_pPixelset->IsIFFPresent()) 
			{
				pOW2Doc->m_pPixelset->AllocIFF(pOW2Doc->m_pPixelset);
			}
			if(!pOW2Doc->m_pPixelset->IsIFFPresent())
			{
				pLogDocument->AddText("error\r\nERROR creating pixelset.\r\n");
				return; //error return safely
			}
			*/
			//AllocDIB() if not already allocated
			if((pOW2Doc->m_pPixelset->IsBIIPresent() || 
				pOW2Doc->m_pPixelset->IsIPLPresent() ||
				pOW2Doc->m_pPixelset->IsIFFPresent()) && 
				!pOW2Doc->m_pPixelset->IsDIBPresent()) 
			{
				pOW2Doc->m_pPixelset->AllocIFF(pOW2Doc->m_pPixelset);
			}
			if(!pOW2Doc->m_pPixelset->IsDIBPresent())
			{
				pLogDocument->AddText("error\r\nERROR creating pixelset.\r\n");
				return; //error return safely
			}
			//2012july22, poirier, end
			pLogDocument->AddText("done.\r\n");

			//update view and frame
			EmptyView();
			OnInitialUpdate();

			//set the view parameter directly into this document's m_pViewDataSettings 
			m_pViewDataSettings->bDrawPixelset = TRUE;
		}

		//release clipboard
		CloseClipboard();
	}
	return;
}





void COW2View::OnViewNewZoomedview() 
{
	//get active view
	CView* pView = NULL;
	CMDIChildWnd* pMDIChildWnd = (CMDIChildWnd*)((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->GetActiveFrame();
	if(pMDIChildWnd) pView = pMDIChildWnd->GetActiveView();
	COWView* pActiveOWView = NULL;
	if(pView->IsKindOf(RUNTIME_CLASS(COWView))) pActiveOWView = (COWView*)pView;
	ASSERT(pActiveOWView==this);

	if(m_pZoomedView==NULL)
	{
		//the selection is either the full view rect or the view's floating selection rect
		COW2Doc* pDoc = GetDocument();

		COW2MDI* pNewMDIFrame = (COW2MDI*) (((COIIIApp*)AfxGetApp())->m_pOW2Doc_ZoomedViewTemplate->CreateNewFrame(pDoc, NULL));
		if(pNewMDIFrame==NULL) return;
		((COIIIApp*)AfxGetApp())->m_pOW2Doc_ZoomedViewTemplate->InitialUpdateFrame(pNewMDIFrame, pDoc, FALSE);

		CView* pView = NULL;
		POSITION pos = pDoc->GetFirstViewPosition();
		while(pos)
		{
			pView = pDoc->GetNextView(pos);
			if(pView!=NULL && pView!=this && pView->IsKindOf(RUNTIME_CLASS(COW2ZoomedView)))
			{
				//got the zoomed view, attach this view to the zoomed view
				((COW2ZoomedView*)pView)->AttachParentView(this);

				//attach the zoomed view to this view
				m_pZoomedView  = (COW2ZoomedView*)pView;

				//pass temp view data settings pointer, so view data settings can be copied within oninitialupdate
				m_pZoomedView->m_pTempViewDataSettings = m_pViewDataSettings;

				//set new window position according to this reference view/mdi frame window
				((COW2ZoomedView*)pView)->SetWindowPosition();
				m_pZoomedView->OnInitialUpdate(); //because, here above, InitialUpdateFrame was called with FALSE

			}
		}
		

		//enable selection rect mode
		EnableROIFloatingMode();
		CRect myRect;
		GetWindowRect(&myRect);
		//BOOL b = SetCursorPos( (myRect.left+myRect.right)/2, (myRect.top+myRect.bottom)/2);
		POINT myPOINT;
		myPOINT.x = (myRect.left+myRect.right)/2;
		myPOINT.y = (myRect.top+myRect.bottom)/2;
		ScreenToClient(&myPOINT);
		LPARAM lParam = LOWORD(myPOINT.x) | HIWORD(myPOINT.y);
		WPARAM wParam = MK_SHIFT;
		//SelectROI(EDGESET_SELECTION_RECT_ROI_ID);

		if(pMDIChildWnd) pMDIChildWnd->PostMessage(WM_CHILDACTIVATE, (WPARAM)0, (LPARAM)0);
		if(pMDIChildWnd) PostMessage(WM_MOUSEMOVE, (WPARAM)wParam, (LPARAM)lParam);
	}
	return;
}

void COW2View::OnUpdateViewNewZoomedview(CCmdUI* pCmdUI) 
{
	if(m_pZoomedView==NULL) pCmdUI->Enable(TRUE);
	  else pCmdUI->Enable(FALSE);
	
}


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//  class COW2ZoomedView
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(COW2ZoomedView, COW2View)

BEGIN_MESSAGE_MAP(COW2ZoomedView, COW2View)
	//{{AFX_MSG_MAP(COW2ZoomedView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

COW2ZoomedView::COW2ZoomedView()
{
	m_pOW2View = NULL;
	bFirstTime = TRUE;
}
COW2ZoomedView::~COW2ZoomedView()
{
	if(m_pOW2View)
	{
		m_pOW2View->m_pZoomedView = NULL;
		m_pOW2View = NULL;
	}
}

void COW2ZoomedView::AttachParentView(COW2View* pView)
{
	m_pOW2View = pView;
}

void COW2ZoomedView::DetachParentView()
{
	m_pOW2View = NULL;
}


void COW2ZoomedView::OnInitialUpdate() 
{
	COW2View::OnInitialUpdate();
}

BOOL COW2ZoomedView::OnEraseBkgnd(CDC* pDC) 
{
	COW2Doc* pDoc = GetDocument();
	CRect myWorldRect = pDoc->GetWorldRect();
	CRect myRect;
	GetClientRect(&myRect);

	if(bFirstTime==TRUE) 
	{
		bFirstTime = FALSE;
	}
	else
	{
		if(HasToDrawPixel() && myRect.Width()<myWorldRect.Width()
			&& myRect.Height()<myWorldRect.Height()) return FALSE;
	}
	return COW2View::OnEraseBkgnd(pDC);
}

void COW2ZoomedView::SetWindowPosition()
{
	if(m_pOW2View==NULL) return;

	// get the parent frame window of the reference view
	RECT rect;
	COW2MDI* pMDIFrame = (COW2MDI*)m_pOW2View->GetParentFrame();
	pMDIFrame->GetWindowRect(&rect);
	// get the parent frame window of this zoomed view
	/*
	RECT rect2;
	COW2MDI* pMDIFrame = (COW2MDI*)GetParentFrame();
	pMDIFrame->GetWindowRect(&rect2);
	*/
	CRect rect1(rect);
	CRect rect2(rect);
	/*
	if(rect2.Width()>400) rect2.right = rect2.left+400;
	if(rect2.Height()>300) rect2.bottom = rect2.top +300;
	*/

	rect2.OffsetRect(rect1.Width(), 0);
	((COIIIApp*)AfxGetApp())->m_pMainWnd->ScreenToClient(&rect2);

	/*
	CRect myRect;
	((COIIIApp*)AfxGetApp())->m_pMainWnd->GetClientRect(&myRect);
	if(rect2.right>myRect.right) rect2.OffsetRect(myRect.right-rect2.right, 0);
	*/

	pMDIFrame = (COW2MDI*)GetParentFrame();
	pMDIFrame->MoveWindow(&rect2, TRUE); //repaint
	pMDIFrame->ShowWindow(SW_SHOWNOACTIVATE);

	/*
	// we need the coord. of the upper left corner of the frame window
	//   and we need them in App. client area coordinates
	CRect rectFrame;
	GetWindowRect(&rectFrame);
	((COIIIApp*)AfxGetApp())->m_pMainWnd->ScreenToClient(&rectFrame);
	
	// take the coordinates of the application client area
	CRect rectAppClient;
	((COIIIApp*)AfxGetApp())->m_pMainWnd->GetClientRect( &rectAppClient );
	*/

	/*
	pMDIFrame->SetWindowPos(&wndTop,
    						 rect.right,
    						 rect.top,
    						 rect.right-rect.left,
    						 rect.bottom-rect.top,
    						 SWP_NOZORDER);
	*/

}

int COW2ZoomedView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COW2View::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL COW2ZoomedView::PreCreateWindow(CREATESTRUCT& cs) 
{
	/*
	if(cs.style && WS_VISIBLE)
	{
		cs.style = cs.style & !WS_VISIBLE;   
	}
	*/
	return COW2View::PreCreateWindow(cs);
}


void COW2View::OnTransSpectralConverttomono() 
{
	int iModeFlag = PIXLESET_BUFFER_NEWWINDOW;
	iModeFlag |= PIXLESET_SPECTRAL_AVERAGE;

	//create single channel image averaging currently visible image channels 
	COW2Doc* pDoc = GetDocument();
	int nchannel = pDoc->m_pPixelset->GetPixelsetNumberOfChannel();
	int* pINT = (int*) new int[nchannel];
	GetPixelsetChannelVisibility(pINT, nchannel);
	COWPixelset* pNewPixelset = pDoc->m_pPixelset->Spectral_ConvertToMonochrome(pINT, 
																				nchannel, 
																				iModeFlag);

	if(iModeFlag&PIXLESET_BUFFER_INPLACE) 
	{
		ASSERT(pNewPixelset==(pDoc->m_pPixelset));
		//done
	}
	else if(iModeFlag&PIXLESET_BUFFER_NEWWINDOW)
	{
		//create doc,mdi and view
		CRect myRect;
		myRect.SetRectEmpty();
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_framealwaysontop = TRUE; 
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_worldrect= myRect; //selection rect
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_bRawData = FALSE; //FALSE, will use doc pixelset imghandle
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pDoc = NULL; 
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pPixelset = pNewPixelset;
		//pop a new doc, frame and view
		COW2Doc* pNewDoc = (COW2Doc*) ((COIIIApp*)AfxGetApp())->m_pOW2DocTemplate->OpenDocumentFile(NULL);

		//2012july22, poirier, begin
		/*
		if(pNewPixelset->IsIFFPresent())
		{
			pNewPixelset->UpdateIFFFromBII();
		}
		*/
		if(pNewPixelset->IsDIBPresent())
		{
			pNewPixelset->UpdateDIBFromBII();
		}
		//2012july22, poirier, end
	}
	else
	{
		ASSERT(FALSE);
	}
	delete[] pINT;
}

void COW2View::OnUpdateTransSpectralConverttomono(CCmdUI* pCmdUI) 
{
	COW2Doc* pDoc = GetDocument();
	if(pDoc->m_pPixelset && pDoc->m_pPixelset->GetPixelsetNumberOfChannel()>1) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
	
}

void COW2View::OnTransSpectralConverttomonoSet() 
{
	int iModeFlag = PIXLESET_BUFFER_NEWWINDOW;
	iModeFlag |= PIXLESET_SPECTRAL_PICKFIRST;

	//create single channel images splitting currently visible image channels 
	COW2Doc* pDoc = GetDocument();
	int nchannel = pDoc->m_pPixelset->GetPixelsetNumberOfChannel();
	int* pINT = (int*) new int[nchannel];
	GetPixelsetChannelVisibility(pINT, nchannel);
	//for each visible channel
	int* _pINT = (int*) new int[nchannel];
	for(int i=0; i<nchannel; i++)
	{
		if(pINT[i])
		{
			for(int j=0; j<nchannel; j++) 
			{
				if(j==i) _pINT[j] = pINT[i];
				  else _pINT[j] = 0;
			}
			COWPixelset* pNewPixelset = pDoc->m_pPixelset->Spectral_ConvertToMonochrome(_pINT, 
																						nchannel, 
																						iModeFlag);

			if(iModeFlag&PIXLESET_BUFFER_INPLACE) 
			{
				ASSERT(pNewPixelset==(pDoc->m_pPixelset));
				//done
			}
			else if(iModeFlag&PIXLESET_BUFFER_NEWWINDOW)
			{
				//create doc,mdi and view
				CRect myRect;
				myRect.SetRectEmpty();
				((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_framealwaysontop = TRUE; 
				((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_worldrect= myRect; //selection rect
				((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_bRawData = FALSE; //FALSE, will use doc pixelset imghandle
				((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pDoc = NULL; 
				((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pPixelset = pNewPixelset;
				//pop a new doc, frame and view
				COW2Doc* pNewDoc = (COW2Doc*) ((COIIIApp*)AfxGetApp())->m_pOW2DocTemplate->OpenDocumentFile(NULL);

				//2012july22, poirier, begin
				/*
				if(pNewPixelset->IsIFFPresent())
				{
					pNewPixelset->UpdateIFFFromBII();
				}
				*/
				if(pNewPixelset->IsDIBPresent())
				{
					pNewPixelset->UpdateDIBFromBII();
				}
				//2012july22, poirier, end

			}
			else
			{
				ASSERT(FALSE);
			}
		}
	}
	delete[] pINT;
	delete[] _pINT;
}

void COW2View::OnUpdateTransSpectralConverttomonoSet(CCmdUI* pCmdUI) 
{
	COW2Doc* pDoc = GetDocument();
	if(pDoc->m_pPixelset && pDoc->m_pPixelset->GetPixelsetNumberOfChannel()>1) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}


