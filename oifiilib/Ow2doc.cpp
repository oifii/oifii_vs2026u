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

// OW2doc.cpp : implementation of the COW2Doc class
//

#include "stdafx.h"
#include "oifiilib.h" //spi 2014
#include "oifii_app.h"

#include "c_xyzi.h"
#include "logdoc.h"

//accusoft image libraries
/*
#include "imglib.h"
#include "img_err.h"
#include "accuipl.h"
#include "ipl_errs.h"
*/
#include "ow_iff.h"


#include "OWpixelset.h" //offworld c++ image library

#include "c_pointset.h" //offworld c pointset library
#include "c_segmentset.h" //offworld c segmentset library

#include "OWpointset.h" //offworld c++ pointset library (interface sitting on pointset.h)
#include "OWsegmentset.h" //offworld c++ segmentset library (interface sitting on segmentset.h)
#include "OWclassset.h" //offworld c++ classset library (interface sitting on segmentset.h)
#include "OWedgeset.h" //offworld edgeset library (for line, polyline, polygone  and shape)


#include "OWdoc.h"
#include "OW2doc.h"
#include "OW2mdi.h"
#include "OWview.h"
#include "OW2view.h"

#include <gl\gl.h>
#include <gl\glu.h>
#include "OW3view.h"

#include "c_debug.h" //header, part of segmentset library
#include "c_fileio.h" //header, part of segmentset library

#include "oifii_gotodlg.h"

#include "oifii_propsettings.h"
#include "oifii_propsheet.h"

#include "limits.h"

#include "gl_owgraph.h"

//2012august23, poirier, begin
#include "portaudio.h"
#include "pa_asio.h"
#include <map>
#include <string>
using namespace std;
//2012august23, poirier, end
#include "oifii_mainfrm.h"

//2012august15, poirier, begin
#include "FreeImagePlus.h"
//2012august15, poirier, end

/* //spi, 2020july15, kicking out oiio dependencies for now
//2020june20, spi, begin
#define NOMINMAX
#undef min
#undef max
#ifdef _M_X64
#include <OpenImageIO/imageio.h>
#endif
#include "utility.h"
//2020june20, spi, end
*/

//2020july04, spi, begin
#include "utility.h"
//2020july04, spi, end

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define OW2DOC_MAXFILENAMELENGTH	512




//should be set accordinly with macro definition list, bottom of file imglib.h
static char* OFF_image_type_stringtable[] =
{    
	"TIFF_UNCOMPRESSED", 
	"BMP_UNCOMPRESSED", 	
	"PCX",  	
	"TARGA", 
	"GIF", 
	"WPG",
	"WMF",
	"TIFF_HUFFMAN",
	"TIFF_G3_FAX",
	"TIFF_LZW",
	"TIFF_G4_FAX",
	"DCX",
	"BMP_COMPRESSED",
	"JPEG",
	"EPS",
	"PICT",
	"TIFF_PACK",
	"TIFF_2D",
	"CALS",
	"LASER_DATA",
	"XBM",
	"MACPAINT",
	"GX2",
	"KOFAX",
	"IOCA",
	"ICON",
	"IFF_ILBM",
	"CLIP",
	"IMG",
	"BROOK_TROUT",
	"MSP",
	"CUT",
	"TARGA16",
	"CCITT_G3",
	"CCITT_G4",
	"XPM",
	"XWD",
	"RAST",
	"ASCII",
	"PHOTOCD",
	"TIFF_JPEG",
	"PHOTOSHOP",
	"IMNET",
	"MAXFILTERS"
};

static char* OFF_image_fileextension_stringtable[] =
{    
	".TIF", 
	".BMP", 	
	".PCX",  	
	".TGA", //targa
	".GIF", 
	".WPG", //word perfect graphic
	".WMF", //windows meta file
	".TIF",
	".TIF",
	".TIF",
	".TIF",
	".DCX",
	".BMP",
	".JPG",
	".EPS",
	".PIC",
	".TIF",
	".TIF",
	".CAL",
	".LAS",
	".XBM",
	".TIF", //MACPAINT always TIF?
	".GX2",
	".FAX", //kofax
	".IOC", //IOCA
	".ICO", //ICON
	".IFF", //IFF_ILBM
	".CLI", //CLIP
	".IMG",
	".BRO", //BROOK_TROUT
	".MSP",
	".CUT",
	".TGA", //TARGAT 16BIT
	".CG3", //CCITT_G3
	".CG4", //CCITT_G4
	".XPM",
	".XWD",
	".RAS", //RAST
	".ASC", //ASCII
	".PHO", //PHOTOCD
	".TIF", //TIFF_JPEG
	".PSD", //PHOTOSHOP
	".IMN", //IMNET
	".MAX"  //MAXFILTERS
};

/////////////////////////////////////////////////////////////////////////////
// COW2Doc

IMPLEMENT_DYNCREATE(COW2Doc, COWDocument)

BEGIN_MESSAGE_MAP(COW2Doc, COWDocument)
	//{{AFX_MSG_MAP(COW2Doc)
	ON_COMMAND(ID_ANALYSIS_TEXTUREEXTRACTION, OnAnalysisTextureextraction)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_TEXTUREEXTRACTION, OnUpdateAnalysisTextureextraction)
	ON_COMMAND(ID_ANALYSIS_TEXTURESEGMENTATION, OnAnalysisTexturesegmentation)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_TEXTURESEGMENTATION, OnUpdateAnalysisTexturesegmentation)
	ON_COMMAND(ID_VIEW_NEW2DVIEW, OnViewNew2dview)
	ON_COMMAND(ID_VIEW_NEW3DVIEW, OnViewNew3dview)
	ON_COMMAND(ID_VIEW_NEWGRAPHVIEW, OnViewNewgraphview)
	ON_COMMAND(ID_VIEW_GRAPHDATAVIEW, OnViewNewGraphdataview)
	ON_COMMAND(ID_ANALYSIS_GLOBALCLASSIFICATION, OnAnalysisGlobalclassification)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_GLOBALCLASSIFICATION, OnUpdateAnalysisGlobalclassification)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NEW2DVIEW, OnUpdateViewNew2dview)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NEW3DVIEW, OnUpdateViewNew3dview)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NEWGRAPHVIEW, OnUpdateViewNewgraphview)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRAPHDATAVIEW, OnUpdateViewGraphdataview)
	ON_COMMAND(ID_TRANS_SPATIAL_ROTATION, OnTransSpatialRotation)
	ON_COMMAND(ID_TRANS_SPATIAL_STRETCH, OnTransSpatialStretch)
	ON_UPDATE_COMMAND_UI(ID_TRANS_SPATIAL_ROTATION, OnUpdateTransSpatialRotation)
	ON_COMMAND(ID_TRANS_SPATIAL_CONFINEDSTRETCH, OnTransSpatialConfinedstretch)
	ON_UPDATE_COMMAND_UI(ID_TRANS_SPATIAL_CONFINEDSTRETCH, OnUpdateTransSpatialConfinedstretch)
	ON_COMMAND(ID_TRANS_LAUNCHSCENARIO, OnTransLaunchscenario)
	ON_UPDATE_COMMAND_UI(ID_TRANS_LAUNCHSCENARIO, OnUpdateTransLaunchscenario)
	ON_UPDATE_COMMAND_UI(ID_TRANS_SPATIAL_STRETCH, OnUpdateTransSpatialStretch)
	ON_COMMAND(ID_TRANS_SPECTRAL_INVERT, OnTransSpectralInvert)
	ON_UPDATE_COMMAND_UI(ID_TRANS_SPECTRAL_INVERT, OnUpdateTransSpectralInvert)
	ON_COMMAND(ID_TRANS_TEMPORAL_EVOLVE, OnTransTemporalEvolve)
	ON_UPDATE_COMMAND_UI(ID_TRANS_TEMPORAL_EVOLVE, OnUpdateTransTemporalEvolve)
	ON_COMMAND(ID_TRANSFORMATION_REPEATINSEQUENCE, OnTransformationRepeatinsequence)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORMATION_REPEATINSEQUENCE, OnUpdateTransformationRepeatinsequence)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_TRANS_SPECTRAL_FILTER, OnTransSpectralFilter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(COW2Doc, COWDocument)
	//{{AFX_DISPATCH_MAP(COW2Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// COW2Doc construction/destruction

COW2Doc::COW2Doc(HWND hwnd)//spi 2014
{
	m_parentHWND=hwnd;
	Init();
}

COW2Doc::COW2Doc()
{
	m_parentHWND=NULL; //spi 2014
	Init();
}

void COW2Doc::Init()
{
	InitDocument();

	m_pExtractionSettings = NULL;
	m_pFusionSettings = NULL;
	m_pClassificationSettings = NULL;
	m_pProfileSettings = NULL;
	m_pHistoSettings = NULL;
	m_pScatterSettings = NULL;
	m_pViewDataSettings = NULL;
	m_pProjectionSettings = NULL;
	m_pTransformationSettings = NULL;
	m_pTraceSettings = NULL;
	m_pBehaviorSettings = NULL;

	EnableAutomation();
	AfxOleLockApp();
}

void COW2Doc::InitDocument()
{
	m_bLocalTextureExtrationStarted = FALSE;
	m_fViewInitZoomFactor = 1.0; //100%

	m_pPixelset = NULL;
	strcpy_s(m_pszPixelsetPathName, 2048, "");

	m_pPointset = NULL;
	strcpy_s(m_pszPointsetPathName, 2048, "");

	m_pSegmentset = NULL;
	strcpy_s(m_pszSegmentsetPathName, 2048, "");

	m_pFusionset = NULL;
	strcpy_s(m_pszFusionsetPathName, 2048, "");

	m_pPartitionset = NULL;
	strcpy_s(m_pszPartitionsetPathName, 2048, "");

	m_pImagefeatureset = NULL;
	strcpy_s(m_pszImagefeaturesetPathName, 2048, "");

	m_pClassset = NULL;
	strcpy_s(m_pszClasssetPathName, 2048, "");

	m_pMapobjectset = NULL;	
	strcpy_s(m_pszMapobjectsetPathName, 2048, "");

	//poirier, sept 2001, begin
	temp_pIplImage = NULL;
	m_pROIset = new COWRoiset;
	if(m_pROIset==NULL)
	{
		ASSERT(FALSE);
	}
	strcpy_s(m_pszRoisetPathName, 2048, "");
	SelectROI(EDGESET_LARGEST_WORLDRECT_ROI_ID);

	m_pViewOfInterest = NULL;
	m_VOI_dfZoomFactor = 0.0;
	m_VOI_ViewRECT.left = 0; 
	m_VOI_ViewRECT.top = 0; 
	m_VOI_ViewRECT.right = 0; 
	m_VOI_ViewRECT.bottom = 0; 
	m_VOI_WindowSIZE.cx = 0; 
	m_VOI_WindowSIZE.cy = 0; 
	m_VOI_ViewOffsetPOINT.x =0;  
	m_VOI_ViewOffsetPOINT.y =0;  
	m_VOI_ZoomedViewRECT.left = 0; 
	m_VOI_ZoomedViewRECT.top = 0; 
	m_VOI_ZoomedViewRECT.right = 0; 
	m_VOI_ZoomedViewRECT.bottom = 0; 
	//poirier, sept 2001, end


	//poirier dec 98, added to store pointset pre-classification
	m_pClassset = NULL;

	return;
}

COW2Doc::~COW2Doc()
{	
	EmptyDocument();

	//poirier dec 98, added to store pointset pre-classification
	//if(m_pClassset) DeleteClassSet(m_pClassset);

	if(m_pExtractionSettings) delete m_pExtractionSettings;
	if(m_pFusionSettings) delete m_pFusionSettings;
	if(m_pClassificationSettings) delete m_pClassificationSettings;
	if(m_pProfileSettings) delete m_pProfileSettings;
	if(m_pHistoSettings) delete m_pHistoSettings;
	if(m_pScatterSettings) delete m_pScatterSettings;
	if(m_pViewDataSettings) delete m_pViewDataSettings;
	if(m_pProjectionSettings) delete m_pProjectionSettings;
	if(m_pTransformationSettings) delete m_pTransformationSettings;
	if(m_pTraceSettings) delete m_pTraceSettings;
	if(m_pBehaviorSettings) delete m_pBehaviorSettings;

	AfxOleUnlockApp();
}

void COW2Doc::EmptyDocument()
{
	//input data, raster or vector
	if(m_pPixelset) delete m_pPixelset;	//if allocated with new
	/*
	if(m_pPointset) DeletePointset(m_pPointset);
	*/
	if(m_pPointset) delete m_pPointset;

	//segmentation related output
	if(m_pSegmentset) DeleteSegmentSet(m_pSegmentset);
	if(m_pFusionset) delete m_pFusionset;
	if(m_pPartitionset) delete m_pPartitionset;
	if(m_pImagefeatureset) delete m_pImagefeatureset;
	
	//user editable, classes sample sets, mapobjects and roi sets
	if(m_pClassset) delete m_pClassset;
	if(m_pMapobjectset) delete m_pMapobjectset;	
	if(m_pROIset) delete m_pROIset;


	return;
}

//poirier, sept 2001, begin
void COW2Doc::SelectROI(int idroi)
{
	if(temp_pIplImage)
	{
		OW_IPL_iplDeallocateImage(temp_pIplImage);
	}

	if(m_pROIset==NULL)
	{
		ASSERT(FALSE);
	}
	m_pROIset->SelectROI(idroi);



}

//poirier, sept 2001, end

/////////////////////////////////////////////////////////////////////////////
// COW2Doc serialization

void COW2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// COW2Doc diagnostics

#ifdef _DEBUG
void COW2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void COW2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

BOOL COW2Doc::CreateSettingsAndInitFromAppDefault()
{
	ASSERT(m_pExtractionSettings==NULL);
	m_pExtractionSettings = (CExtractionSettings*) new CExtractionSettings;
	ASSERT(m_pExtractionSettings!=NULL);
	m_pFusionSettings = (CFusionSettings*) new CFusionSettings;
	ASSERT(m_pFusionSettings!=NULL);
	m_pClassificationSettings = (CClassificationSettings*) new CClassificationSettings;
	ASSERT(m_pClassificationSettings!=NULL);
	m_pProfileSettings = (CProfileSettings*) new CProfileSettings;
	ASSERT(m_pProfileSettings!=NULL);
	m_pHistoSettings = (CHistoSettings*) new CHistoSettings;
	ASSERT(m_pHistoSettings!=NULL);
	m_pScatterSettings = (CScatterSettings*) new CScatterSettings;
	ASSERT(m_pScatterSettings!=NULL);
	m_pViewDataSettings = (CViewDataSettings*) new CViewDataSettings;
	ASSERT(m_pViewDataSettings!=NULL);
	m_pProjectionSettings = (CProjectionSettings*) new CProjectionSettings;
	ASSERT(m_pProjectionSettings!=NULL);
	m_pTransformationSettings = (CTransformationSettings*) new CTransformationSettings;
	ASSERT(m_pTransformationSettings!=NULL);
	m_pTraceSettings = (CTraceSettings*) new CTraceSettings;
	ASSERT(m_pTraceSettings!=NULL);
	m_pBehaviorSettings = (CBehaviorSettings*) new CBehaviorSettings;
	ASSERT(m_pBehaviorSettings!=NULL);

	if(m_pExtractionSettings==NULL || m_pFusionSettings==NULL || m_pClassificationSettings==NULL
		|| m_pProfileSettings==NULL || m_pHistoSettings==NULL || m_pScatterSettings==NULL
		|| m_pViewDataSettings==NULL || m_pProjectionSettings==NULL || m_pTransformationSettings==NULL
		|| m_pTraceSettings==NULL || m_pBehaviorSettings==NULL)
	{
		return FALSE;
	}

	if(m_parentHWND==NULL) //spi 2014
	{
		//copy content of all settings instances
		//... from application defaults
		COIIIApp* pOIIIApp = (COIIIApp*)AfxGetApp();
		m_pExtractionSettings->CopyAllValuesFrom(pOIIIApp->m_pExtractionSettings);
		m_pFusionSettings->CopyAllValuesFrom(pOIIIApp->m_pFusionSettings);
		m_pClassificationSettings->CopyAllValuesFrom(pOIIIApp->m_pClassificationSettings);
		m_pProfileSettings->CopyAllValuesFrom(pOIIIApp->m_pProfileSettings);
		m_pHistoSettings->CopyAllValuesFrom(pOIIIApp->m_pHistoSettings);
		m_pScatterSettings->CopyAllValuesFrom(pOIIIApp->m_pScatterSettings);
		m_pTraceSettings->CopyAllValuesFrom(pOIIIApp->m_pTraceSettings);
		m_pBehaviorSettings->CopyAllValuesFrom(pOIIIApp->m_pBehaviorSettings);
		m_pViewDataSettings->CopyAllValuesFrom(pOIIIApp->m_pViewDataSettings);
		m_pProjectionSettings->CopyAllValuesFrom(pOIIIApp->m_pProjectionSettings);
		m_pTransformationSettings->CopyAllValuesFrom(pOIIIApp->m_pTransformationSettings);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// COW2Doc commands
BOOL COW2Doc::OnNewDocument()
{
	//test, begin
	CDocument* pDoc = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->GetActiveDocument();
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	//get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
	//get the active view attached to the active MDI child window.
	CView *pView = (CView*) pChild->GetActiveView();
	//test, end


	if(!CreateSettingsAndInitFromAppDefault())
		return FALSE;	
	if (!COWDocument::OnNewDocument())
		return FALSE;

	if(((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pPixelset==NULL
	   && ((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pDoc==NULL)
	{
		// case when file menu new document 
		// caller of new document must set these temporary app global data members
		//CRect worldrect(0,0,400,400);
		CRect worldrect(0,0,OW2DOC_NEWDOCUMENT_SIZEX,OW2DOC_NEWDOCUMENT_SIZEY);
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_worldrect = worldrect;
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_bRawData = FALSE;
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_rectRawData.SetRectEmpty(); //only used if bRawData==TRUE
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pDoc = NULL;
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pPixelset = NULL;	
	}
	return CreateDocumentFromAppTempInfo();
}

BOOL COW2Doc::OnNewDocumentFromAppTempInfo()
{
	//test, begin
	CDocument* pDoc = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->GetActiveDocument();
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	//get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
	//get the active view attached to the active MDI child window.
	CView *pView = (CView*) pChild->GetActiveView();
	//test, end

	if(!CreateSettingsAndInitFromAppDefault())
		return FALSE;
	if (!COWDocument::OnNewDocument())
		return FALSE;

	/* caller of new document must set these temporary app global data members
	CRect worldrect(0,0,400,400);
	((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_worldrect = worldrect;
	((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_bRawData = FALSE;
	((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_rectRawData.SetRectEmpty(); //only used if bRawData==TRUE
	((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pDoc = NULL;
	((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pPixelset = NULL;
	*/
	return CreateDocumentFromAppTempInfo();
}

BOOL COW2Doc::CreateDocumentFromAppTempInfo()
{
	char pszCharBuffer[255];
	char pszMessage[255];
 	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();

	//0) get temporary document creation variable
	m_fViewInitZoomFactor = ((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_initzoomfactor;
	CRect worldrect = ((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_worldrect;
	BOOL bRawData = ((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_bRawData;
	CRect rectRawData = ((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_rectRawData;
	COW2Doc* pSrcDoc = ((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pDoc;
	COWPixelset* pPixelset = ((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pPixelset;

	if(pPixelset==NULL)
	{
		if(bRawData==FALSE && pSrcDoc==NULL)
		{
			if(worldrect.IsRectEmpty()) 
			{
				//default rect
				m_fViewInitZoomFactor = 1.0;
				//CRect rect(0,0,400,400);
				CRect rect(0,0,OW2DOC_NEWDOCUMENT_SIZEX,OW2DOC_NEWDOCUMENT_SIZEY);
				worldrect = rect;
				sprintf_s(pszCharBuffer, 255, "default empty image (%d,%d,%d,%d", rect.left, rect.top, rect.right, rect.bottom);
			}
			else
			{
				//selection rect 
				strcpy_s(pszCharBuffer, 255, "empty image with supplied rect");
			}
		}
		else if(pSrcDoc)
		{
			if(worldrect.IsRectEmpty()) 
			{
				//get rect from pixelset
				worldrect = pSrcDoc->GetWorldRect();
				if(bRawData) strcpy_s(pszCharBuffer, 255, "pixelset rect");
				  else strcpy_s(pszCharBuffer, 255, "image handle rect");
			}
			else
			{
				//get rect from selection
				if(bRawData) strcpy_s(pszCharBuffer, 255, "pixelset selection rect");
				  else strcpy_s(pszCharBuffer, 255, "image handle selection rect");
			}
			pPixelset = pSrcDoc->m_pPixelset;
		}
		wsprintfA(pszMessage, "Creating pixelset from %s ... ", pszCharBuffer);
		pLogDocument->AddText(pszMessage);
		//m_pPixelset = new COWPixelset(pPixelset, &worldrect);
		m_pPixelset = new COWPixelset(	pPixelset, 
										&worldrect, 
										-1, 
										PIXELSET_IGNORE_APP_TEMPINFO, 
										0); //ivalue -1 would be for uninitialized buffer
	}
	else if(pPixelset)
	{
		strcpy_s(pszCharBuffer, 255, "Attaching new pixelset image buffer ... ");
		pLogDocument->AddText(pszMessage);
		m_pPixelset = pPixelset;
	}

	//2012august15, poirier, begin
	/*
	//AllocIFF() if not already allocated
	if(m_pPixelset->IsBIIPresent() && !m_pPixelset->IsIFFPresent()) 
	{
		m_pPixelset->AllocIFF(m_pPixelset);
	}
	if(!m_pPixelset->IsIFFPresent())
	{
		pLogDocument->AddText("error\r\nERROR creating pixelset.\r\n");
		return FALSE; //error return safely
	}
	*/
	if( (m_pPixelset->IsIFFPresent() && !m_pPixelset->IsBIIPresent())
		|| (m_pPixelset->IsDIBPresent() && !m_pPixelset->IsBIIPresent()) )
	{
		m_pPixelset->AllocBII(m_pPixelset);
		m_pPixelset->UpdateBIIFrom(m_pPixelset->GetIFFHandle());
	}
	//AllocDIB() if not already allocated
	if(m_pPixelset->IsBIIPresent() && !m_pPixelset->IsDIBPresent()) 
	{
		m_pPixelset->AllocDIB(m_pPixelset);
		m_pPixelset->UpdateDIBFromBII();
	}

	if(!m_pPixelset->IsDIBPresent())
	{
		pLogDocument->AddText("error\r\nERROR creating pixelset.\r\n");
		return FALSE; //error return safely
	}
	//2012august15, poirier, end
	pLogDocument->AddText("done.\r\n");

	//set the view parameter directly into this document's m_pViewDataSettings 
	m_pViewDataSettings->bDrawPixelset = TRUE;

	//2012july22, poirier, begin
	//reset temporary document creation variable
	//((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_initzoomfactor;
	//((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_worldrect;
	//((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_bRawData;
	//((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_rectRawData;
	((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pDoc = NULL;
	((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pPixelset = NULL;
	//2012july22, poirier, end
	return TRUE;
}


BOOL COW2Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if(!CreateSettingsAndInitFromAppDefault())
		return FALSE;

	if(m_parentHWND==NULL) //spi 2014
	{
		if (!COWDocument::OnOpenDocument(lpszPathName))
			return FALSE; //error, return safely
	}
	CLogDocument* pLogDocument=NULL;
	if(m_parentHWND==NULL) //spi 2014
	{
 		pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	}
    char* pszFileExtension = strrchr((char*)lpszPathName,'.');
	//pszFileExtension = _strupr( pszFileExtension );
	if (pszFileExtension != NULL && (strcmp(pszFileExtension, ".OW2") == 0 || strcmp(pszFileExtension, ".ow2") == 0))
	{
		if(m_parentHWND==NULL) pLogDocument->AddText("Opening file as a 2D World Document (.ow2)\r\n");
		return OpenFileAs2DWorldDocument(lpszPathName);
	}

	//try to load file as a pixelset
	m_pPixelset = OpenFileAsPixelset(lpszPathName);
	if (m_pPixelset)
	{
		CStringA myString(lpszPathName);
		::strcpy(m_pszPixelsetPathName, (const char*)myString);
	}
	if(m_pPixelset==NULL)
	{
		//try to load file as a pointset
		m_pPointset = OpenFileAsPointset(lpszPathName);
		
		if(m_pPointset)
		{
			CStringA myString(lpszPathName);
			::strcpy(m_pszPointsetPathName, (const char*)myString);
			//build TIN
			BOOL bResult = m_pPointset->BuildTINAndComputeStatistics();
			if(bResult)
			{
				/*
				//2012july24,poirier, begin
				//get first view and set initial view attribute
				POSITION pos = GetFirstViewPosition();
				COW2View* pFirstView = (COW2View*) GetNextView( pos );
				if(pFirstView!=NULL) 
				{
					pFirstView->m_pViewDataSettings->bDrawPointset = TRUE;
				}
				*/
				m_pViewDataSettings->bDrawPointset = TRUE;
				//2012july24,poirier, end
			}
		}
	}

	if(m_pPixelset==NULL && m_pPointset==NULL)
	{
		//load segments (fusion tree)
		m_pSegmentset = OpenFileAsSegmentset(lpszPathName);
		if (m_pSegmentset)
		{
			CStringA myString(lpszPathName);
			::strcpy(m_pszSegmentsetPathName, (const char*)myString);
		}
	}

	if(m_pPixelset==NULL && m_pPointset==NULL && m_pSegmentset==NULL)
	{
		//load classes definition 
		m_pClassset = OpenFileAsClassset(lpszPathName);
		if (m_pSegmentset)
		{
			CStringA myString(lpszPathName);
			::strcpy(m_pszClasssetPathName, myString);
		}
	}

	return TRUE;
}

BOOL COW2Doc::OpenFileAs2DWorldDocument(LPCTSTR lpszOW2PathName)
{
	//2020july03, spi, begin
	//int inumread = -1;
	size_t inumread = 0;
	//2020july03, spi, end
 	CLogDocument* pLogDocument = NULL; //spi 2014
	if(m_parentHWND==NULL) pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();

	FILE* pFILE = NULL;
	CStringA myStringA(lpszOW2PathName);
	if( (pFILE = fopen((const char*)myStringA, "r")) == NULL )
	{
		if(m_parentHWND==NULL)
		{
			CString myString;
			myString.Format(L"     Error, cannot open %s as 2D World Document file (oiii version 1.04a) )\r\n", lpszOW2PathName);
			pLogDocument->AddTextW(myString);
		}
		return FALSE; //error, return safely
	}
	//load 2D World Document specific

	//load ow2 file format signature
	double dfsignature=0.0;
	inumread = fread(&dfsignature, sizeof(double), 1, pFILE);
	if(inumread!=1 || dfsignature!=1.0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//load view of interest settings
	inumread = fread(&m_VOI_dfZoomFactor, sizeof(double), 1, pFILE);
	if(inumread!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	inumread = fread(&m_VOI_ViewRECT, sizeof(RECT), 1, pFILE);
	if(inumread!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	inumread = fread(&m_VOI_WindowSIZE, sizeof(SIZE), 1, pFILE);
	if(inumread!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	inumread = fread(&m_VOI_ViewOffsetPOINT, sizeof(POINT), 1, pFILE);
	if(inumread!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	inumread = fread(&m_VOI_ZoomedViewRECT, sizeof(RECT), 1, pFILE);
	if(inumread!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//load settings
	m_pExtractionSettings->ReadAllValuesFrom(pFILE);
	m_pFusionSettings->ReadAllValuesFrom(pFILE);
	m_pClassificationSettings->ReadAllValuesFrom(pFILE);
	m_pProfileSettings->ReadAllValuesFrom(pFILE);
	m_pHistoSettings->ReadAllValuesFrom(pFILE);
	m_pScatterSettings->ReadAllValuesFrom(pFILE);
	m_pViewDataSettings->ReadAllValuesFrom(pFILE);
	m_pProjectionSettings->ReadAllValuesFrom(pFILE);
	m_pTransformationSettings->ReadAllValuesFrom(pFILE);
	m_pTraceSettings->ReadAllValuesFrom(pFILE);
	m_pBehaviorSettings->ReadAllValuesFrom(pFILE);

	//load sets
	int numberofsetsloaded=0;
	int numberofsetsthatcouldnotbeloaded=0;
	char pszPathName[2048];
	int idOW2type;
	while(fread(&idOW2type, sizeof(int), 1, pFILE)==1 && idOW2type!=OW2TYPE_ENDOFFILE)
	{
		//read filename
		int nchar;
		inumread = fread(&nchar, sizeof(int), 1, pFILE);
		if(inumread!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		inumread = fread(pszPathName, sizeof(char)*nchar, 1, pFILE);
		if(inumread!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		pszPathName[nchar] = '\0';

		switch (idOW2type)
		{
		case OW2TYPE_PIXELSET:
		{
								 //load pixelset
								 CString myString(pszPathName);
								 m_pPixelset = OpenFileAsPixelset(myString);
								 if (m_pPixelset != NULL) numberofsetsloaded++;
								 else numberofsetsthatcouldnotbeloaded++;
								 //m_pPixelset->ReadAllValuesFrom(pFILE);
								 break;
		}
		case OW2TYPE_POINTSET:
		{
								 //load pointset
								 CString myString(pszPathName);
								 m_pPointset = OpenFileAsPointset(myString);
								 if (m_pPointset != NULL) numberofsetsloaded++;
								 else numberofsetsthatcouldnotbeloaded++;
								 //m_pPointset->ReadAllValuesFrom(pFILE);
								 if (m_pPointset)
								 {
									 //build TIN
									 BOOL bResult = m_pPointset->BuildTINAndComputeStatistics();
									 if (bResult)
									 {
										 //get first view and set initial view attribute
										 POSITION pos = GetFirstViewPosition();
										 COW2View* pFirstView = (COW2View*)GetNextView(pos);
										 if (pFirstView != NULL) pFirstView->m_pViewDataSettings->bDrawPointset = TRUE;
									 }
								 }
								 break;
		}
		case OW2TYPE_SEGMENTSET:
		{
								   //load segmentset
								   CString myString(pszPathName);
								   m_pSegmentset = OpenFileAsSegmentset(myString);
								   if (m_pSegmentset != NULL) numberofsetsloaded++;
								   else numberofsetsthatcouldnotbeloaded++;
								   //m_pSegmentset->ReadAllValuesFrom(pFILE);
								   break;
		}
		case OW2TYPE_CLASSSET:
		{
								 //load classset
								 CString myString(pszPathName);
								 m_pClassset = OpenFileAsClassset(myString);
								 if (m_pClassset != NULL) numberofsetsloaded++;
								 else numberofsetsthatcouldnotbeloaded++;
								 //m_pClassset->ReadAllValuesFrom(pFILE);
								 break;
		}
		case OW2TYPE_ROISET:
		{
							   //load ROIs
							   CString myString(pszPathName);
							   m_pROIset = OpenFileAsRoiset(myString);
							   if (m_pROIset != NULL) numberofsetsloaded++;
							   else numberofsetsthatcouldnotbeloaded++;
							   //m_pROIset->ReadAllValuesFrom(pFILE);
							   int idroi_selected = m_pROIset->GetSelectedROI();
							   if (idroi_selected > 3)
							   {
								   if (m_parentHWND == NULL) //spi 2014
								   {
									   //get first view and set initial view attribute
									   POSITION pos = GetFirstViewPosition();
									   COW2View* pFirstView = (COW2View*)GetNextView(pos);
									   if (pFirstView != NULL) pFirstView->InvalidateROI(idroi_selected);
								   }
							   }
							   break;
		}
		}
	}
	::fclose(pFILE);
	if(numberofsetsloaded==0 || numberofsetsthatcouldnotbeloaded>0) 
	{
		ASSERT(FALSE);
		if(m_parentHWND==NULL) //spi 2014
		{
			CString myString;
			myString.Format(L"     Error, cannot open %s as 2D World Document file (oiii version 1.04a) )\r\n",lpszOW2PathName);
			pLogDocument->AddTextW(myString);
		}
		return FALSE;
	}
	return TRUE;
}

BOOL COW2Doc::SaveFileAs2DWorldDocument(LPCTSTR lpszPathName)
{
	//2020july03, spi, begin
	//int inumwritten = -1;
	size_t inumwritten = 0;
	//2020july03, spi, end
	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();

	FILE* pFILE = NULL;
	CStringA myStringA(lpszPathName);
	if ((pFILE = fopen(myStringA, "w")) == NULL)
	{
		CString myString;
		myString.Format(L"     Error, cannot open %s as 2D World Document file (filename may already exist) )\r\n",lpszPathName);
		pLogDocument->AddTextW(myString);
		return FALSE; //error, return safely
	}
	//2012jluly26, poirier, begin
	CString myPath = GetPathOnly(myStringA);
	CString myFilename = GetFilenameOnly(myStringA);
	CString myExtension = GetExtensionOnly(myStringA);
	//2012jluly26, poirier, end
	
	//save 2D World Document specific

	int idOW2type = OW2TYPE_ENDOFFILE;
	if(pFILE)
	{
		//save ow2 file format signature
		double dfsignature=1.0;
		inumwritten = fwrite(&dfsignature, sizeof(double), 1, pFILE);
		if(inumwritten!=1)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		//save view of interest settings
		if(m_pViewOfInterest)
		{
			//get settings into this document
			m_VOI_dfZoomFactor = m_pViewOfInterest->GetZoomFactor();
			m_VOI_ViewRECT.left = m_pViewOfInterest->m_ViewRect.left; 
			m_VOI_ViewRECT.top = m_pViewOfInterest->m_ViewRect.top; 
			m_VOI_ViewRECT.right = m_pViewOfInterest->m_ViewRect.right; 
			m_VOI_ViewRECT.bottom = m_pViewOfInterest->m_ViewRect.bottom; 
			m_VOI_WindowSIZE.cx = m_pViewOfInterest->m_WindowSize.cx;  
			m_VOI_WindowSIZE.cy = m_pViewOfInterest->m_WindowSize.cy;  
			m_VOI_ViewOffsetPOINT.x = m_pViewOfInterest->m_ViewOffset.x;   
			m_VOI_ViewOffsetPOINT.y = m_pViewOfInterest->m_ViewOffset.y; 
			m_pViewOfInterest->GetClientRect(&m_VOI_ZoomedViewRECT);
			m_pViewOfInterest->ViewDPtoLP(&m_VOI_ZoomedViewRECT);
			m_pViewDataSettings->CopyAllValuesFrom(m_pViewOfInterest->m_pViewDataSettings);
			m_pProjectionSettings->CopyAllValuesFrom(m_pViewOfInterest->m_pProjectionSettings);
			m_pTransformationSettings->CopyAllValuesFrom(m_pViewOfInterest->m_pTransformationSettings);


			//write settings
			inumwritten = fwrite(&m_VOI_dfZoomFactor, sizeof(double), 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			inumwritten = fwrite(&m_VOI_ViewRECT, sizeof(RECT), 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			inumwritten = fwrite(&m_VOI_WindowSIZE, sizeof(SIZE), 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			inumwritten = fwrite(&m_VOI_ViewOffsetPOINT, sizeof(POINT), 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			inumwritten = fwrite(&m_VOI_ZoomedViewRECT, sizeof(RECT), 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
		}

		//save settings
		m_pExtractionSettings->WriteAllValuesTo(pFILE);
		m_pFusionSettings->WriteAllValuesTo(pFILE);
		m_pClassificationSettings->WriteAllValuesTo(pFILE);
		m_pProfileSettings->WriteAllValuesTo(pFILE);
		m_pHistoSettings->WriteAllValuesTo(pFILE);
		m_pScatterSettings->WriteAllValuesTo(pFILE);
		m_pViewDataSettings->WriteAllValuesTo(pFILE);
		m_pProjectionSettings->WriteAllValuesTo(pFILE);
		m_pTransformationSettings->WriteAllValuesTo(pFILE);
		m_pTraceSettings->WriteAllValuesTo(pFILE);
		m_pBehaviorSettings->WriteAllValuesTo(pFILE);

		//save pixelset
		if(m_pPixelset) 
		{
			//write file type
			idOW2type = OW2TYPE_PIXELSET;
			inumwritten = fwrite(&idOW2type, sizeof(int), 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			//write filename size and filename string
			//2012jluly26, poirier, begin
			/*
			//int nchar = strlen(m_pszPixelsetPathName);
			int nchar = strlen(m_pszPixelsetPathName);
			if(nchar==0)
			{
				char* pszFileExtension = strrchr((char*)lpszPathName,'.');
				int nPath = strlen(lpszPathName);
				int nExt = strlen(pszFileExtension);
				int nName = nPath - nExt;
				memcpy(m_pszPixelsetPathName, lpszPathName, nName);
				char NullChar = '\0';
				memcpy(&(m_pszPixelsetPathName[nName]), &NullChar, 1); //terminate string with null char
				strcat(m_pszPixelsetPathName, ".pix");
				nchar = strlen(m_pszPixelsetPathName);
			}
			fwrite(&nchar, sizeof(int), 1, pFILE);
			fwrite(m_pszPixelsetPathName, sizeof(char)*nchar, 1, pFILE);
			*/
			CString myFullfilename="";
			if(!myPath.IsEmpty()) myFullfilename=myPath+"\\";
			myFullfilename=myFullfilename+myFilename+".pix";
			CStringA myStringA(myFullfilename);
			strcpy(m_pszPixelsetPathName, myStringA);
			//2020july03, spi, begin
			//int nchar = strlen(m_pszPixelsetPathName);
			size_t nchar = strlen(m_pszPixelsetPathName);
			//2020july03, spi, end
			inumwritten = fwrite(&nchar, sizeof(int), 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			inumwritten = fwrite(m_pszPixelsetPathName, sizeof(char)*nchar, 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			//2012jluly26, poirier, end
			//write pixelset
			CString myString(m_pszPixelsetPathName);
			SaveFileAsPixelset(m_pPixelset, myString);
			//write COW2Set
			//m_pPixelset->WriteAllValuesTo(pFILE);
		}

		//save pointset
		if(m_pPointset) 
		{
			//write file type
			idOW2type = OW2TYPE_POINTSET;
			inumwritten = fwrite(&idOW2type, sizeof(int), 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			//write filename size and filename string
			//2012jluly26, poirier, begin
			/*
			int nchar = strlen(m_pszPointsetPathName);
			if(nchar==0)
			{
				char* pszFileExtension = strrchr((char*)lpszPathName,'.');
				int nPath = strlen(lpszPathName);
				int nExt = strlen(pszFileExtension);
				int nName = nPath - nExt;
				memcpy(m_pszPointsetPathName, lpszPathName, nName);
				char NullChar = '\0';
				memcpy(&(m_pszPointsetPathName[nName]), &NullChar, 1); //terminate string with null char
				strcat(m_pszPointsetPathName, ".poi");
				nchar = strlen(m_pszPointsetPathName);
			}
			fwrite(&nchar, sizeof(int), 1, pFILE);
			fwrite(m_pszPointsetPathName, sizeof(char)*nchar, 1, pFILE);
			*/
			CString myFullfilename="";
			if(!myPath.IsEmpty()) myFullfilename=myPath+"\\";
			if(OW2DOC_SAVEPOINTSET_ASXYI)
			{
				myFullfilename=myFullfilename+myFilename+".xyi";
			}
			else
			{
				myFullfilename=myFullfilename+myFilename+".poi";
			}
			CStringA myStringA(myFullfilename);
			strcpy(m_pszPointsetPathName, myStringA);
			//2020july03, spi, begin
			//int nchar = strlen(m_pszPointsetPathName);
			size_t nchar = strlen(m_pszPointsetPathName);
			//2020july03, spi, end
			inumwritten = fwrite(&nchar, sizeof(int), 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			inumwritten = fwrite(m_pszPointsetPathName, sizeof(char)*nchar, 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			//2012jluly26, poirier, end
			//write pointset
			CString myString(m_pszPointsetPathName);
			SaveFileAsPointset(m_pPointset, myString);
			//write COW2Set
			//m_pPointset->WriteAllValuesTo(pFILE);
		}
		
		//save segmentset
		if(m_pSegmentset) 
		{
			//write file type
			idOW2type = OW2TYPE_SEGMENTSET;
			inumwritten = fwrite(&idOW2type, sizeof(int), 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			//write filename size and filename string
			//2012jluly26, poirier, begin
			/*
			int nchar = strlen(m_pszSegmentsetPathName);
			if(nchar==0)
			{
				char* pszFileExtension = strrchr((char*)lpszPathName,'.');
				int nPath = strlen(lpszPathName);
				int nExt = strlen(pszFileExtension);
				int nName = nPath - nExt;
				memcpy(m_pszSegmentsetPathName, lpszPathName, nName);
				char NullChar = '\0';
				memcpy(&(m_pszSegmentsetPathName[nName]), &NullChar, 1); //terminate string with null char
				strcat(m_pszSegmentsetPathName, ".seg");
				nchar = strlen(m_pszSegmentsetPathName);
			}
			fwrite(&nchar, sizeof(int), 1, pFILE);
			fwrite(m_pszSegmentsetPathName, sizeof(char)*nchar, 1, pFILE);
			*/
			CString myFullfilename="";
			if(!myPath.IsEmpty()) myFullfilename=myPath+"\\";
			myFullfilename=myFullfilename+myFilename+".seg";
			CStringA myStringA(myFullfilename);
			strcpy(m_pszSegmentsetPathName, myStringA);
			//2020july03, spi, begin
			//int nchar = strlen(m_pszSegmentsetPathName);
			size_t nchar = strlen(m_pszSegmentsetPathName);
			//2020july03, spi, end
			inumwritten = fwrite(&nchar, sizeof(int), 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			inumwritten = fwrite(m_pszSegmentsetPathName, sizeof(char)*nchar, 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			//2012jluly26, poirier, end
			//write segmentset
			CString myString(m_pszSegmentsetPathName);
			SaveFileAsSegmentset(m_pSegmentset, myString);
			//write COW2Set
			//m_pSegmentset->WriteAllValuesTo(pFILE);
		}

		//save classset
		if(m_pClassset) 
		{
			//write file type
			idOW2type = OW2TYPE_CLASSSET;
			inumwritten = fwrite(&idOW2type, sizeof(int), 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			//write filename size and filename string
			//2012jluly26, poirier, begin
			/*
			int nchar = strlen(m_pszClasssetPathName);
			if(nchar==0)
			{
				char* pszFileExtension = strrchr((char*)lpszPathName,'.');
				int nPath = strlen(lpszPathName);
				int nExt = strlen(pszFileExtension);
				int nName = nPath - nExt;
				memcpy(m_pszClasssetPathName, lpszPathName, nName);
				char NullChar = '\0';
				memcpy(&(m_pszClasssetPathName[nName]), &NullChar, 1); //terminate string with null char
				strcat(m_pszClasssetPathName, ".cla");
				nchar = strlen(m_pszClasssetPathName);
			}
			fwrite(&nchar, sizeof(int), 1, pFILE);
			fwrite(m_pszClasssetPathName, sizeof(char)*nchar, 1, pFILE);
			*/
			CString myFullfilename="";
			if(!myPath.IsEmpty()) myFullfilename=myPath+"\\";
			myFullfilename=myFullfilename+myFilename+".cla";
			CStringA myStringA(myFullfilename);
			strcpy(m_pszClasssetPathName, myStringA);
			//2020july03, spi, begin
			//int nchar = strlen(m_pszClasssetPathName);
			size_t nchar = strlen(m_pszClasssetPathName);
			//2020july03, spi, end
			inumwritten = fwrite(&nchar, sizeof(int), 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			inumwritten = fwrite(m_pszClasssetPathName, sizeof(char)*nchar, 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			//2012jluly26, poirier, end
			//write classset
			CString myString(m_pszClasssetPathName);
			SaveFileAsClassset(m_pClassset, myString);
			//write COW2Set
			//m_pClassset->WriteAllValuesTo(pFILE);
		}

		//save ROIs
		if(m_pROIset) 
		{
			//write file type
			idOW2type = OW2TYPE_ROISET;
			inumwritten = fwrite(&idOW2type, sizeof(int), 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			//write filename size and filename string
			//2012jluly26, poirier, begin
			/*			
			int nchar = strlen(m_pszRoisetPathName);
			if(nchar==0)
			{
				char* pszFileExtension = strrchr((char*)lpszPathName,'.');
				int nPath = strlen(lpszPathName);
				int nExt = strlen(pszFileExtension);
				int nName = nPath - nExt;
				memcpy(m_pszRoisetPathName, lpszPathName, nName);
				char NullChar = '\0';
				memcpy(&(m_pszRoisetPathName[nName]), &NullChar, 1); //terminate string with null char
				strcat(m_pszRoisetPathName, ".roi");
				nchar = strlen(m_pszRoisetPathName);
			}
			fwrite(&nchar, sizeof(int), 1, pFILE);
			fwrite(m_pszRoisetPathName, sizeof(char)*nchar, 1, pFILE);
			*/
			CString myFullfilename="";
			if(!myPath.IsEmpty()) myFullfilename=myPath+"\\";
			myFullfilename=myFullfilename+myFilename+".roi";
			CStringA myStringA(myFullfilename);
			strcpy(m_pszRoisetPathName, myStringA);
			//2020july03, spi, begin
			//int nchar = strlen(m_pszRoisetPathName);
			size_t nchar = strlen(m_pszRoisetPathName);
			//2020july03, spi, end
			inumwritten = fwrite(&nchar, sizeof(int), 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			inumwritten = fwrite(m_pszRoisetPathName, sizeof(char)*nchar, 1, pFILE);
			if(inumwritten!=1)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			//2012jluly26, poirier, end
			//write roiset
			CString myString(m_pszRoisetPathName);
			SaveFileAsRoiset(m_pROIset, myString);
			//write COW2Set
			//m_pROIset->WriteAllValuesTo(pFILE);
		}
	}
	idOW2type = OW2TYPE_ENDOFFILE;
	inumwritten = fwrite(&idOW2type, sizeof(int), 1, pFILE);	
	if(inumwritten!=1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(pFILE) ::fclose(pFILE);
	return TRUE;
}

COWPixelset* COW2Doc::OpenFileAsPixelset(LPCTSTR lpszPathName) 
{
	COWPixelset* pPixelset = NULL;

 	CLogDocument* pLogDocument = NULL; //spi 2014
	if(m_parentHWND==NULL) pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();


	//1) if file is a raster image file
 	char pszMessage[1024];
	char pszCharBuf[1024];
	CStringA myStringA(lpszPathName);
	strcpy(pszCharBuf, myStringA);

	int iType;
	if(OIII_USE_OWIFFACCUSOFT==1)
	{
		iType = OW_IFF_IMGLOW_get_filetype(pszCharBuf);
	}
	if((OIII_USE_OWIFFACCUSOFT==1 && iType!=FORMAT_NOT_ALLOWED) ||
		OIII_USE_FREEIMAGE==1 || OIII_USE_OPENCV==1 || OIII_USE_OPENIMAGEIO==1) 
	{
		//accusoft supported raster file format
		pPixelset = new COWPixelset;
		//2012august15, poirier, begin
		if(OIII_USE_OWIFFACCUSOFT==1)
		{
			if(m_parentHWND==NULL)
			{
				char* pChar = OFF_image_type_stringtable[iType];
				wsprintfA(pszMessage, "Loading pixelset as %s ... ", pChar);
				pLogDocument->AddText(pszMessage);
			}
			int imghandle = OW_IFF_IMG_decompress_bitmap(pszCharBuf);
			if(imghandle<0) 
			{
				if(m_parentHWND==NULL) pLogDocument->AddText("error\r\nERROR loading pixelset.\r\n");
				delete pPixelset;
				return NULL; //error return safely
			}
			pPixelset->SetIFFHandle(imghandle);
			if(m_parentHWND==NULL) pLogDocument->AddText("done.\r\n");
			pPixelset->AllocDIB(imghandle);
			pPixelset->UpdateDIBFrom(imghandle);
		}
		else if(OIII_USE_FREEIMAGE==1)
		{
			if (m_parentHWND == NULL)
			{
				wsprintfA(pszMessage, "Loading pixelset ... ");
				pLogDocument->AddText(pszMessage);
			}
			//spi, 2020june19 modif, begin
			fipWinImage myfipWinImage; //now, local allocation again //now allocted in the ow2doc.h
			BOOL bResult;
			if (myfipWinImage.identifyFIF(pszCharBuf)==FIF_JPEG)
			{
				bResult = myfipWinImage.load(pszCharBuf, JPEG_EXIFROTATE);
			}
			else
			{
				bResult = myfipWinImage.load(pszCharBuf);
			}
			//spi, 2020june19 modif, end
			if(bResult==FALSE)
			{
				if(m_parentHWND==NULL) pLogDocument->AddText("error\r\nERROR loading pixelset.\r\n");
				delete pPixelset;
				return NULL;
			}
			if(m_parentHWND==NULL) pLogDocument->AddText("done.\r\n");
			//spi, nov 2018, begin
			/*
			BITMAPINFOHEADER* pBITMAPINFOHEADER = myfipWinImage.getInfoHeader();
			pPixelset->AllocDIB(pBITMAPINFOHEADER);
			pPixelset->UpdateDIBFrom(pBITMAPINFOHEADER);
			*/
			const BITMAPINFOHEADER* pBITMAPINFOHEADER=myfipWinImage.getInfoHeader();
			pPixelset->AllocDIB(const_cast<BITMAPINFOHEADER*>(pBITMAPINFOHEADER));
			pPixelset->UpdateDIBFrom(const_cast<BITMAPINFOHEADER*>(pBITMAPINFOHEADER));
			myfipWinImage.clear();
			//spi, nov 2018, end
		}
		//2020june20 modif, spi, begin
#ifdef _M_X64
		else if (OIII_USE_OPENCV==1)
		{
			/* //todo
			if (m_parentHWND == NULL)
			{
				wsprintfA(pszMessage, "Loading pixelset ... ");
				pLogDocument->AddText(pszMessage);
			}
			Mat img = imread(pszCharBuf, IMREAD_COLOR);
			if (m_parentHWND == NULL) pLogDocument->AddText("done.\r\n");
			*/
		}
		else if (OIII_USE_OPENIMAGEIO==1)
		{
			/* //spi commented out 2020july15, kicking out oiio dependencies for now
			if (m_parentHWND == NULL)
			{
				wsprintfA(pszMessage, "Loading pixelset ... ");
				pLogDocument->AddText(pszMessage);
			}
			wstring ws = lpszPathName;
			string myfilename = utf8_encode(ws); //ws2s(ws);
			auto in = OIIO::ImageInput::open(myfilename);
			if (!in)
			{
				if (m_parentHWND == NULL) pLogDocument->AddText("error\r\nERROR opening image file.\r\n");
				delete pPixelset;
				return NULL;
			}
			const OIIO::ImageSpec &spec = in->spec();
			int xres = spec.width;
			int yres = spec.height;
			int nchannels = spec.nchannels;
			if (nchannels != 1 && nchannels != 3)
			{
				if (m_parentHWND == NULL) pLogDocument->AddText("error\r\nERROR image file should either have 1 (gray) or 3 channels (rgb).\r\n");
				delete pPixelset;
				return NULL;
			}
			if (pPixelset->AllocDIB(nchannels * 8, spec.width, spec.height) == TRUE)
			{
				std::vector<unsigned char> pixels(xres*yres*nchannels);
				if(in->read_image(OIIO::TypeDesc::UINT8, &pixels[0])==FALSE)
				{
					if (m_parentHWND == NULL) pLogDocument->AddText("error\r\nERROR reading image file.\r\n");
					delete pPixelset;
					return NULL;
				}

				unsigned char* pDIBData = (unsigned char*)pPixelset->GetDIBDataPointer();
				//memcpy(pDIBData, &pixels[0], xres*yres*nchannels);
				memcpy(pDIBData, &pixels[0], xres*yres*nchannels);
				
			}
			else
			{
				if (m_parentHWND == NULL) pLogDocument->AddText("error\r\nERROR allocating pixelset.\r\n");
				delete pPixelset;
				return NULL;
			}
			in->close();
			if (m_parentHWND == NULL) pLogDocument->AddText("done.\r\n");
			*/
		}
#endif //_M_X64
		//2020june20 modif, spi, end
		//2012august15, poirier, end

		//set the view parameter directly into this document's m_pViewDataSettings 
		m_pViewDataSettings->bDrawPixelset = TRUE;

		/*
		//poirier, sept 2001, begin
		if(pPixelset->IsIFFPresent() && !(pPixelset->IsBIIPresent()) )
		{
			pLogDocument->AddText("     Allocating pixelset buffer ... ");
			if(pPixelset->AllocBII(pPixelset->GetIFFHandle())==FALSE)
			{
				pLogDocument->AddText("Error\r\nError, not enough memory to alloc BII buffer, can't perform texture extraction\r\n");
				delete pPixelset;
				return NULL; //error
			}
			pPixelset->UpdateBIIFrom(pPixelset->GetIFFHandle());
			pLogDocument->AddText("done.\r\n");
		}
		//poirier, sept 2001, end
		*/
		if(pPixelset->IsDIBPresent() && !(pPixelset->IsBIIPresent()) )
		{
			if(m_parentHWND==NULL) pLogDocument->AddText("     Allocating pixelset buffer ... ");
			if(pPixelset->AllocBII(pPixelset->GetDIBPointer())==FALSE)
			{
				if(m_parentHWND==NULL) pLogDocument->AddText("Error\r\nError, not enough memory to alloc BII buffer, can't perform texture extraction\r\n");
				delete pPixelset;
				return NULL; //error
			}
			pPixelset->UpdateBIIFrom(pPixelset->GetDIBPointer());
			if(m_parentHWND==NULL) pLogDocument->AddText("done.\r\n");
		}
		
		return pPixelset;
	}

    char* pszFileExtension = strrchr((char*)lpszPathName,'.');
	pszFileExtension = _strupr( pszFileExtension );
 	if(pszFileExtension!=NULL  &&  strcmp(pszFileExtension, ".RAW") == 0)
	//if (pszFileExtension != NULL && (strcmp(pszFileExtension, ".RAW") == 0 || strcmp(pszFileExtension, ".raw") == 0))
	{
		if(m_parentHWND==NULL) pLogDocument->AddText("Sorry, RAW format not supported yet.\r\n");
		delete pPixelset;
		return NULL; //error
	}

	if(OIII_USE_OWIFFACCUSOFT==1)
	{
		//
		//0) if file is a text file
		//
 		if(pszFileExtension!=NULL  &&  strcmp(pszFileExtension, ".TXT") == 0)
		{
			if(m_parentHWND==NULL) pLogDocument->AddText("Importing text file as an image ...");
			pPixelset = new COWPixelset;

			ASCIITEXTATTR attributes;
			int ret = OW_IFF_IMGLOW_get_ascii_attributes(&attributes);
			attributes.asciiFlags = 0xFFFFFFFF;	// determines which fields to use
			attributes.asciiXDpi =	72;	//horizontal dots per inch 	
			attributes.asciiYDpi = 72;	//vertical dots per inch   	
			RECT marginRECT;
			marginRECT.left = 500; //margins (1/1000 inches)
			marginRECT.right = 500; //margins (1/1000 inches)
			marginRECT.top = 500; //margins (1/1000 inches)
			marginRECT.bottom = 500; //margins (1/1000 inches)
			attributes.asciiMargin = marginRECT;
			attributes.asciiTabStop = 5;	// # of chars between tab stops   
			attributes.asciiPageWidth = 8500;	// width of page (1/1000 inches)	 
			attributes.asciiPageHeight = 11000; // height of page (1/1000 inches)  
			attributes.asciiPointSize = 12;	// point size of the font			  
			attributes.asciiCharsPerLine = 80; // number of characters per line	  
			attributes.asciiLinesPerPage = 60; // number of lines per page		 
			attributes.asciiWeight = 0; // normal = 0, bold = 1			 	  
			attributes.asciiItalic = 0; // normal = 0, italic = 1			  
			strcpy(attributes.asciiTypeFace, "ARIAL"); // name of the font to use			 
			ret = OW_IFF_IMGLOW_set_ascii_attributes(&attributes);
			int npages = OW_IFF_IMGLOW_get_pages((char*)lpszPathName);

			int imghandle = OW_IFF_IMG_import_ascii((char*)lpszPathName, npages);
			if(pPixelset!=NULL && imghandle>-1)
			{
				pPixelset->SetIFFHandle(imghandle);
			}
			else
			{
				if(m_parentHWND==NULL) pLogDocument->AddText("error on import.\r\n");
				delete pPixelset;
				return NULL; //error
			}
			if(m_parentHWND==NULL) pLogDocument->AddText("done.\r\n");
			m_pViewDataSettings->bDrawPixelset = TRUE;

			return pPixelset;
		}
	}
	//2012july24, poirier, begin
	/*
	ASSERT(FALSE);
	delete pPixelset;
	*/
	//2012july24, poirier, end
	return NULL;
}

BOOL COW2Doc::SaveFileAsPixelset(COWPixelset* pPixelset, LPCTSTR lpszPathName) 
{
	ASSERT(pPixelset);
 	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();

	//2012july26, poirier, begin
	/*
	FILE* pFILE = NULL;
	if( (pFILE = fopen(lpszPathName, "w")) == NULL )
	{
		CString myString;
		myString.Format("     Error, cannot open %s within 2D World Document file (file may exist and be locked for writing) )\r\n",lpszPathName);
		pLogDocument->AddText(myString);
		return FALSE; //error, return safely
	}
	//pPixelset->WriteAllValuesTo(pFILE);
	if(pFILE) fclose(pFILE);
	*/
	CStringA myStringA(lpszPathName);
	if (pPixelset->IsIFFPresent())
	{
		OW_IFF_IMG_save_bitmap(pPixelset->GetIFFHandle(), const_cast<char*>((const char*)myStringA), BMP_UNCOMPRESSED);
	}
	else if(m_pPixelset->IsDIBPresent())
	{
		m_pPixelset->SaveDIB(const_cast<char*>((const char*)myStringA));
	}
	else if(m_pPixelset->IsBIIPresent())
	{
		ASSERT(FALSE);
	}
	else
	{
		ASSERT(FALSE);
	}
	//2012july26, poirier, end

	return TRUE;
}

COWPointset* COW2Doc::OpenFileAsPointset(LPCTSTR lpszPathName)
{
 	CLogDocument* pLogDocument = NULL; //spi 2014
	if(m_parentHWND==NULL) pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	//
	//1) if file is a pointset
	//
	if(m_parentHWND==NULL) pLogDocument->AddText("Will now load pointset, construct TIN and compute statistics\r\n");
	CTime InitialTime = CTime::GetCurrentTime();


	//COWPointset* pPointset = new COWPointset(POINTSET_INITIAL_NUMBEROFPOINTS);
	COWPointset* pPointset = new COWPointset(POINTSET_INITIAL_NUMBEROFPOINTS, POINTSET_DEFAULT_NUMBEROFSTAT);
    if(pPointset==NULL)
    {
		if(m_parentHWND==NULL) pLogDocument->AddText("     Fatal error, pPointset is NULL\r\n");
		return NULL; //error, return safely
    }
	/*
	//if(pPointset->NewPointsetStatistics(pPointset->GetMaxNumberOfElements(), 4) ==FALSE)
	if(pPointset->NewPointsetStatistics(pPointset->GetMaxNumberOfElements(), POINTSET_DEFAULT_NUMBEROFSTAT) ==FALSE)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	*/
	CStringA myStringA(lpszPathName);
	int i,ii;
    char* pszFileExtension = strrchr((char*)lpszPathName,'.');
	pszFileExtension = _strupr( pszFileExtension );
    if( pszFileExtension!=NULL && (i=strcmp(pszFileExtension, ".POI"))==0)
    {
		FILE* pFILE = NULL;
		if ((pFILE = fopen(myStringA, "r")) == NULL)
		{
			if(m_parentHWND==NULL)
			{
				CString myString;
				myString.Format(L"     Error, cannot open %s within 2D World Document file (file may have been moved) )\r\n",lpszPathName);
				pLogDocument->AddTextW(myString);
			}
			delete pPointset;
			return NULL; //error, return safely
		}
		if(pPointset->ReadAllValuesFrom(pFILE)==FALSE)
		{
			if(m_parentHWND==NULL)
			{
				CString myString;
				myString.Format(L"     Error, cannot read %s within 2D World Document file (cannot parse file) )\r\n",lpszPathName);
				pLogDocument->AddTextW(myString);
			}
			fclose(pFILE);
			delete pPointset;
			return NULL;
		}
		if(pFILE) fclose(pFILE);
    }
    // check if .xyi file-format.  note: .nbr are actualy of type .xyi 
    // nbr file format are created by Voronoi for Windows 1.0 CRG App. 
    else if( pszFileExtension != NULL && ( (i=strcmp(pszFileExtension, ".XYI")) == 0 ||
	  (ii=strcmp(pszFileExtension, ".NBR")) == 0) )
    {
		pPointset->OpenFileAsXYI(myStringA);
    }
	else if(pszFileExtension != NULL &&	 strcmp(pszFileExtension, ".XYZ") == 0)
	{
		// it is a .xyz file format
		pPointset->OpenFileAsXYZ(myStringA);
	}
	else
	{
		ASSERT(FALSE);
		//if unsupported file extension
    	if(m_parentHWND==NULL) pLogDocument->AddText("development-time error, should not attempt to read unsupported file type");
		/*
		DeletePointset(pPointset);
		*/
		delete pPointset;
		return NULL; //error, but return safely
	}

	char pszCharBuf[1024];
	wsprintfA(pszCharBuf, "     nombre de points lus = %d\r\n",pPointset->GetNumberOfPoints());
	if(m_parentHWND==NULL) pLogDocument->AddText(pszCharBuf);

	CTime FinalTime = CTime::GetCurrentTime();
	CTimeSpan DiffTime = FinalTime - InitialTime;
    wsprintfA(pszCharBuf, "XYZI file successfully read in %d min and %d sec.\r\n", DiffTime.GetMinutes(), DiffTime.GetSeconds());
	if(m_parentHWND==NULL) pLogDocument->AddText(pszCharBuf);

	return pPointset;
}

BOOL COW2Doc::SaveFileAsPointset(COWPointset* pPointset, LPCTSTR lpszPathName) 
{
	ASSERT(pPointset);
 	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	CStringA myStringA(lpszPathName);
	int i, ii;
    char* pszFileExtension = strrchr((char*)lpszPathName,'.');
	pszFileExtension = _strupr( pszFileExtension );
    if( pszFileExtension!=NULL && (i=strcmp(pszFileExtension, ".POI"))==0)
    {
		FILE* pFILE = NULL;
		if( (pFILE = fopen(myStringA, "w")) == NULL )
		{
			CString myString;
			myString.Format(L"     Error, cannot open %s within 2D World Document file (file may have been moved) )\r\n",lpszPathName);
			pLogDocument->AddTextW(myString);
			return FALSE; //error, return safely
		}
		pPointset->WriteAllValuesTo(pFILE);
		if(pFILE) fclose(pFILE);
	}
    else if( pszFileExtension != NULL && ( (i=strcmp(pszFileExtension, ".XYI")) == 0 ||
	  (ii=strcmp(pszFileExtension, ".NBR")) == 0) )
    {
		pPointset->SaveFileAsXYI(myStringA);
    }
	else if(pszFileExtension != NULL &&	 strcmp(pszFileExtension, ".XYZ") == 0)
	{
		// it is a .xyz file format
		//pPointset->SaveFileAsXYZ(lpszPathName);
	}
	else
	{
		ASSERT(FALSE);
		//if unsupported file extension
    	pLogDocument->AddText("development-time error, should not attempt to read unsupported file type");
		return FALSE; //error, but return safely
	}
	return TRUE;
}

SEGMENTSET* COW2Doc::OpenFileAsSegmentset(LPCTSTR lpszPathName)
{
	SEGMENTSET* pSEGMENTSET = NULL;

 	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();

	CStringA myStringA(lpszPathName);
	//
	//1) if file is a segment set
	//
    char* pszFileExtension = strrchr((char*)lpszPathName,'.');
	pszFileExtension = _strupr( pszFileExtension );
	if(pszFileExtension!=NULL  &&  strcmp(pszFileExtension, ".SET") == 0)
	{
		pLogDocument->AddText("Loading segmentset... ");
		//segmentset 
		char pszCharBuf[1024];
		strcpy(pszCharBuf, myStringA);
		pSEGMENTSET = OpenSegmentSet(pszCharBuf);
		if(pSEGMENTSET==NULL)
		{
			pLogDocument->AddText("error.\r\n");
			return NULL;
		}
		pLogDocument->AddText("done.\r\n");

		//now, set file extension to .xyi, to load corresponding pointset
		strcpy(pszFileExtension, ".XYI");
	}
	return pSEGMENTSET;
}


BOOL COW2Doc::SaveFileAsSegmentset(SEGMENTSET* pSegmentset, LPCTSTR lpszPathName)
{
	ASSERT(pSegmentset);
 	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();

	CStringA myStringA(lpszPathName);
	FILE* pFILE = NULL;
	if( (pFILE = fopen(myStringA, "w")) == NULL )
	{
		CString myString;
		myString.Format(L"     Error, cannot open %s within 2D World Document file (file may have been moved) )\r\n",lpszPathName);
		pLogDocument->AddTextW(myString);
		return FALSE; //error, return safely
	}
	//pClassset->WriteAllValuesTo(pFILE);
	char pszCharBuf[OW2DOC_MAXPATHLENGTH];
	strcpy(pszCharBuf, myStringA);
	SaveSegmentSet(pSegmentset, pszCharBuf);
	if(pFILE) fclose(pFILE);
	return TRUE;
}

COWClassset* COW2Doc::OpenFileAsClassset(LPCTSTR lpszPathName)
{
	ASSERT(FALSE);
	COWClassset* pClassset = NULL;

 	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();

	CStringA myStringA(lpszPathName);
	FILE* pFILE = NULL;
	if( (pFILE = fopen(myStringA, "r")) == NULL )
	{
		CString myString;
		myString.Format(L"     Error, cannot open %s within 2D World Document file (file may have been moved) )\r\n",lpszPathName);
		pLogDocument->AddTextW(myString);
		return FALSE; //error, return safely
	}
	if(m_pClassset)
	{
		pClassset = m_pClassset;
	}
	else
	{
		pClassset = (COWClassset*) new COWClassset;
	}
	//pClassset->ReadAllValuesFrom(pFILE);
	if(pFILE) fclose(pFILE);
	return pClassset;
}

BOOL COW2Doc::SaveFileAsClassset(COWClassset* pClassset, LPCTSTR lpszPathName)
{
	ASSERT(pClassset);
 	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();

	CStringA myStringA(lpszPathName);
	FILE* pFILE = NULL;
	if( (pFILE = fopen(myStringA, "w")) == NULL )
	{
		CString myString;
		myString.Format(L"     Error, cannot open %s within 2D World Document file (file may have been moved) )\r\n",lpszPathName);
		pLogDocument->AddTextW(myString);
		return FALSE; //error, return safely
	}
	//pClassset->WriteAllValuesTo(pFILE);
	if(pFILE) fclose(pFILE);
	return TRUE;
}

COWRoiset* COW2Doc::OpenFileAsRoiset(LPCTSTR lpszPathName)
{
	COWRoiset* pROIset = NULL;
 	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();

	CStringA myStringA(lpszPathName);
	FILE* pFILE = NULL;
	if( (pFILE = fopen(myStringA, "r")) == NULL )
	{
		CString myString;
		myString.Format(L"     Error, cannot open %s within 2D World Document file (file may have been moved) )\r\n",lpszPathName);
		pLogDocument->AddTextW(myString);
		return FALSE; //error, return safely
	}
	if(m_pROIset)
	{
		pROIset = m_pROIset;
	}
	else
	{
		pROIset = (COWRoiset*) new COWRoiset;
	}
	pROIset->ReadAllValuesFrom(pFILE);
	if(pFILE) fclose(pFILE);
	return pROIset;
}

BOOL COW2Doc::SaveFileAsRoiset(COWRoiset* pROIset, LPCTSTR lpszPathName)
{
	ASSERT(pROIset);
 	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();

	CStringA myStringA(lpszPathName);
	FILE* pFILE = NULL;
	if( (pFILE = fopen(myStringA, "w")) == NULL )
	{
		CString myString;
		myString.Format(L"     Error, cannot open %s within 2D World Document file (file may have been moved) )\r\n",lpszPathName);
		pLogDocument->AddTextW(myString);
		return FALSE; //error, return safely
	}
	pROIset->WriteAllValuesTo(pFILE);
	if(pFILE) fclose(pFILE);
	return TRUE;
}

double* COW2Doc::GetPointerToStatistics()
{
	double* pfStatistics = NULL;

	//has a pointset been loaded?
	if(m_pPointset != NULL)
	{
		//have the statistic been computed?  
		pfStatistics = m_pPointset->GetPointerToStatistics();
		if(pfStatistics)
		{
			//before segmentation, statistic are stored on pointset's side
			return pfStatistics;
		}
		else
		{
			//after segmentation, statistic are stored on segset's side
			if(m_pSegmentset != NULL)
			{
				return m_pSegmentset->pfStatistics;
			}
		}
	}
	//ASSERT(FALSE); //cannot find statistics!
	return NULL;
}


int COW2Doc::GetNumberOfStatPerPoint()
{
	//has a pointset been loaded?
	if(m_pPointset != NULL)
	{
		//have the statistic been computed?  
		if(m_pPointset->GetPointerToStatistics())
		{
			//before segmentation, statistic are stored on pointset's side
			return m_pPointset->GetNumberOfStatPerPoint();
		}
		else
		{
			//after segmentation, statistic are stored on segset's side
			if(m_pSegmentset != NULL)
			{
				return m_pSegmentset->nStatPerSegment;
			}
		}
	}
	ASSERT(FALSE); //cannot find statistics!
	return 0;
}

int COW2Doc::GetNumberOfPoint()
{
	//has a pointset been loaded?
	if(m_pPointset != NULL)
	{
		//have the statistic been computed?  
		if(m_pPointset->GetPointerToStatistics())
		{
			//before segmentation, statistic are stored on pointset's side
			return m_pPointset->GetNumberOfPoints();
		}
		else
		{
			//after segmentation, statistic are stored on segset's side
			if(m_pSegmentset != NULL)
			{
				return m_pSegmentset->iInitialNumberOfSegment;
			}
		}
	}
	ASSERT(FALSE); //cannot find statistics!
	return 0;
}


int COW2Doc::GetSizeX()
{
	if(m_pPointset) return m_pPointset->GetSizeX();
	  else return 0; 
}

int COW2Doc::GetSizeY()
{
	if(m_pPointset) return m_pPointset->GetSizeY();
	  else return 0; 
}

RECT COW2Doc::GetWorldRect()
{
	return GetLargestWorldRect();
}

RECT COW2Doc::GetSmallestWorldRect()
{
	//default 1 unit (i.e. like 1 x 1 pixel size)
	CRect myRect(0,0,1,1);

	if(m_pPixelset)
	{
		//check the continuous world
		
		//if georeference, we may want to get this info here
	}
	else if(m_pPointset)
	{ 
		//check the discrete world

		//get smallest map object size of feature size
		//in the pointset, the smallest dimension in x and y of a given point

		//actually: pointset stores px[] and py[]
		//todo: to pointset add dx[] and dy[] to store delta (errors)
		/*
		myRect.left = (LONG)(m_pPointset->xmin - 0.5);
		myRect.top = (LONG)(m_pPointset->ymin - 0.5);
		myRect.right = (LONG)(m_pPointset->xmax + 0.5);
		myRect.bottom = (LONG)(m_pPointset->ymax + 0.5);
		*/
	} 
	else if(m_pSegmentset)
	{
		//check the segmented world

		//get smallest segment bounding rect

	}
	return myRect; 
}

RECT COW2Doc::GetLargestWorldRect()
{
	//2012august17, poirier, begin
	/*
	RECT myRect;
	if(m_pPixelset)
	{
		//check the continuous world
		myRect = m_pPixelset->GetCropRect();
	}
	else if(m_pPointset)
	{ 
		//check the discrete world
		myRect = m_pPointset->GetBoundingRect();
	} 
	else if(m_pSegmentset)
	{
		//check the segmented world
	}
	return myRect; 
	*/
	
	RECT myRECT;
	CRect myRect;
	CRect myRect1;
	CRect myRect2;
	if(m_pPixelset)
	{
		//check the continuous world
		myRect1 = m_pPixelset->GetCropRect();
		myRECT = myRect1;
	}
	if(m_pPointset && m_pPointset->m_pPOINTSET)
	{ 
		//check the discrete world
		myRect2 = m_pPointset->GetBoundingRect();
		myRECT = myRect2;
	} 
	if(m_pPixelset && m_pPointset && m_pPointset->m_pPOINTSET)
	{
		myRect.UnionRect(&myRect1,&myRect2);
		myRECT = myRect;
	}
	else if(m_pPixelset==NULL && m_pPointset==NULL)
	{
		//no pixelset and no pointset
		ASSERT(FALSE);
		//todo
		//check for segmentset
	}
	return myRECT; 
	//2012august17, poirier, end
}

RECT COW2Doc::GetViewingWorldRect()
{
	//if .ow2 document is a large database, only selection would be loaded
	RECT myRect = GetWorldRect();
	return myRect;
}

RECT COW2Doc::GetSelectionWorldRect()
{
	//if .ow2 document is a large database, only selection would be loaded
	RECT myRect = GetViewingWorldRect();
	return myRect;
}

//this function returns the min and max for the caller's supplied pfStastistics vector 
BOOL COW2Doc::GetMinMaxForStatisticsVector(double* pfStatistics, 
										   int inumstatistics, 
										   double* pfMin, 
										   double* pfMax)
{
	if(pfStatistics==NULL || pfMin==NULL || pfMax==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	pfMin[0] = (double)MAXDBL;
	pfMax[0] = (double)MINDBL;
	for(int i=0; i<inumstatistics; i++)
	{
		if(pfStatistics[i]<pfMin[0]) pfMin[0] = pfStatistics[i];
		  else if(pfStatistics[i]>pfMax[0]) pfMax[0] = pfStatistics[i];
	}
	return TRUE;
}

//returns the dfMin and dfMax values computed by FindGlobalMinMax() in pointset module
BOOL COW2Doc::GetMinMaxForStatChannel(int idstatchannel, double& dfMin, double& dfMax)
{
	if(m_pPointset==NULL || idstatchannel<0 || idstatchannel>=GetNumberOfStatPerPoint())
	{
		dfMin = -1.0;
		dfMax = -1.0;
		ASSERT(FALSE);
		return FALSE;
	}
	dfMin = m_pPointset->GetPointerToGlobalStatisticsInfo()[idstatchannel].fMin;
	dfMax = m_pPointset->GetPointerToGlobalStatisticsInfo()[idstatchannel].fMax;


/* //TODO, temporarely using directly pGlobalStatisticsInfo[]
	pdfMin* = MAXDBL;
	pdfMax* = MINDBL;

	double* pdfBase = GetPointerToStatistics();
    double* pdfStat;
    int iOffset = idstatchannel; 
	if(pdfBase==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	for(iOffset=idstatchannel; iOffset; GetNumberOfStatPerPoint()++)
	{
	}

*/
	return TRUE;
}

BOOL COW2Doc::CopyPointset(COWPointset* pPointset)
{
	//0) duplicate
	#ifdef _DEBUG
		CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
		pLogDocument->AddText("Duplicating pointset\r\n");
	#endif

	//poirier, sept 2001, begin
	if(pPointset==NULL)
	{
		#ifdef _DEBUG
			pLogDocument->AddText("No texture vector found, will delete pointset");
		#endif
	}
	else if(pPointset->GetNumberOfPoints()<3)
	{
		#ifdef _DEBUG
			pLogDocument->AddText("Not enough texture vector found, will delete pointset");
		#endif		
			//DeletePointset(pPointset);
			delete pPointset;
			pPointset = NULL;
			return FALSE;
	}
	//poirier, sept 2001, end
	else
	{
		//delete existing pointset
		if(m_pPointset!=NULL)
		{
			//DeletePointset(m_pPointset);
			delete m_pPointset;
			m_pPointset = NULL;
		}
		//copy newly obtain pointset
		//m_pPointset = pPointset->NewPointsetCopy();
		m_pPointset = new COWPointset(pPointset); 
	}
	return TRUE;
}

//approximate, rounded off, used to invalidate pointset display
CRect COW2Doc::GetPointsetRect()
{
	CRect myPointsetRect(0,0,0,0);
	if(m_pPointset==NULL)
	{
		ASSERT(FALSE);
	}
	else
	{
		/*
		myPointsetRect.left = (int) m_pPointset->xmin;
		myPointsetRect.right = (int) m_pPointset->xmax;
		myPointsetRect.top = (int) m_pPointset->ymax;
		myPointsetRect.bottom = (int) m_pPointset->ymin;
		*/
		myPointsetRect = m_pPointset->GetBoundingRect();

	}
	return myPointsetRect;
}

BOOL COW2Doc::OffsetPointsetRect(double x_offset, double y_offset)
{
	if(m_pPointset==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	m_pPointset->TranslatePointset(x_offset, y_offset);
	return TRUE;
}

//add points
int COW2Doc::AddPoint(double fX, double fY, int iTINupdateflag/*=POINTSET_TIN_NOUPDATE*/, int* p_itriseed/*=NULL*/, double fIntensity, double fTreeDimX, double fTreeDimY, double fTreeArea)
{
	if(m_pPointset==NULL)
	{
		//0) allocate pointset
		//check if initial alloc needed
		if(m_pPointset==NULL)
		{
			/*
			m_pPOINTSET = NewPointset(m_maxnumberofelements);
			if(m_pPOINTSET==NULL)
			{
				ASSERT(FALSE);
				return;
			}
			NewPointsetStatistics(m_maxnumberofelements, POINTSET_DEFAULT_NUMBEROFSTAT);
			*/
			//m_pPointset = new COWPointset(POINTSET_INITIAL_NUMBEROFPOINTS);
			m_pPointset = new COWPointset(POINTSET_INITIAL_NUMBEROFPOINTS, POINTSET_DEFAULT_NUMBEROFSTAT);
			if(m_pPointset==NULL) return FALSE;
		}
		/*
		int nX=2;
		int nY=2;
		*/
		int nX=10;
		int nY=10;
		CRect myRect;
		myRect = GetWorldRect();
		//1.0 add rectangular point frames
		double dfXmin;
		double dfYmin;
		double dfXmax;
		double dfYmax;
		double dfXwidth;
		double dfYwidth;
		
		//1.1) add first point frame (100% world rect)
		dfXmin=myRect.left;
		dfYmin=myRect.top;
		dfXmax=myRect.right;
		dfYmax=myRect.bottom;
		
		m_pPointset->AddPointFrame(nX, nY, dfXmin, dfYmin, dfXmax, dfYmax);
			
		/*
		//1.2) add second point frame (50% world rect, centered)
		dfXwidth=0.50*(myRect.right-myRect.left);
		dfYwidth=0.50*(myRect.bottom-myRect.top);
		dfXmin=myRect.left+(myRect.right-myRect.left)/2.0-dfXwidth/2.0;
		dfYmin=myRect.top+(myRect.bottom-myRect.top)/2.0-dfYwidth/2.0;
		dfXmax=dfXmin+dfXwidth;
		dfYmax=dfYmin+dfYwidth;
		m_pPointset->AddPointFrame(nX, nY, dfXmin, dfYmin, dfXmax, dfYmax);
		*/
		
		/*
		//1.3) add third point frame (25% world rect, centered)		
		dfXwidth=0.25*(myRect.right-myRect.left);
		dfYwidth=0.25*(myRect.bottom-myRect.top);
		dfXmin=myRect.left+(myRect.right-myRect.left)/2.0-dfXwidth/2.0;
		dfYmin=myRect.top+(myRect.bottom-myRect.top)/2.0-dfYwidth/2.0;
		dfXmax=dfXmin+dfXwidth;
		dfYmax=dfYmin+dfYwidth;
		m_pPointset->AddPointFrame(nX, nY, dfXmin, dfYmin, dfXmax, dfYmax);
		*/

		/*
		//add one triangle
		double dfX=(dfXmin+dfXmax)/3.0;
		double dfY=(dfYmin+dfYmax)/3.0;
		m_pPointset->AddPoint(dfX, dfY, POINTSET_TIN_NOUPDATE, NULL);
		dfX=2*(dfXmin+dfXmax)/3.0;
		dfY=(dfYmin+dfYmax)/2.0;
		m_pPointset->AddPoint(dfX, dfY, POINTSET_TIN_NOUPDATE, NULL);
		dfX=(dfXmin+dfXmax)/3.0;
		dfY=2*(dfYmin+dfYmax)/3.0;
		m_pPointset->AddPoint(dfX, dfY, POINTSET_TIN_NOUPDATE, NULL);
		*/

		//2) build TIN and compute stats
		/*
		m_pPointset->BuildTINAndComputeStatistics();
		//3) add point
		return m_pPointset->AddPoint(fX, fY, iTINupdateflag, p_itriseed);
		*/
		//2022may19, spi note, after adding the point frame, tin is built before adding the point in order to enable validation within AddPoint(), i.e. to detect in which triangle user wants to add the point 
		//2022may19, spi note, that provoque 2 consecutive tin builds
		m_pPointset->BuildTINAndComputeStatistics();
	}
	//////////////
	//3) add point
	//////////////
	if(m_pPointset->AddPoint(fX, fY, iTINupdateflag, p_itriseed, fIntensity, fTreeDimX, fTreeDimY, fTreeArea)>-1)
	{
		//get first view and set initial view attribute
		POSITION pos = GetFirstViewPosition();
		COW2View* pFirstView = (COW2View*) GetNextView( pos );
		if(pFirstView!=NULL) pFirstView->m_pViewDataSettings->bDrawPointset = TRUE;
		//update all views
		UpdateAllViews(NULL);
		return TRUE;
	}
	else
	{
		CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
		pLogDocument->AddText("AddPoint() canceled.\r\n");
		return FALSE;
	}
}

void COW2Doc::OnAnalysisTextureextraction() 
{
	CLogDocument* pLogDocument = NULL;
	if(m_parentHWND==NULL) //spi 2014
	{
		pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
		if(m_bLocalTextureExtrationStarted)
		{
			pLogDocument->DisableViewUpdating();
		}
		pLogDocument->AddText("Will now perform texture extraction\r\n");
	}

	if(m_pPixelset==NULL || m_pPointset!=NULL) 
	{
		ASSERT(FALSE); //inconsistent
		if(m_parentHWND==NULL) //spi 2014
		{
			if(m_bLocalTextureExtrationStarted)
			{
				pLogDocument->EnableViewUpdating();
			}
		}
		return;
	}
	
	//if(m_pPixelset->m_imghandle>-1 && m_pPixelset->m_pData==NULL)
	if(m_pPixelset->IsIFFPresent() && !(m_pPixelset->IsBIIPresent()) )
	{
		if(m_parentHWND==NULL) //spi 2014
		{
			pLogDocument->AddText("     Allocating pixelset buffer ... ");
		}
		//poirier, sept 2001, begin
		//version2
		if(m_pPixelset->AllocBII(m_pPixelset->GetIFFHandle())==FALSE)
		/*
		if(m_pPixelset->AllocImgBufferFromAccusoftImgHandle()==FALSE)
		*/
		//poirier, sept 2001, end
		{
			if(m_parentHWND==NULL) //spi 2014
			{
				if(m_bLocalTextureExtrationStarted)
				{
					pLogDocument->EnableViewUpdating();
				}
				pLogDocument->AddText("Error\r\nError, not enough memory to alloc BII buffer, can't perform texture extraction\r\n");
			}
			return;
		}
		m_pPixelset->UpdateBIIFrom(m_pPixelset->GetIFFHandle());
		if(m_parentHWND==NULL) //spi 2014
		{
			pLogDocument->AddText("done.\r\n");
		}
	}
	#ifdef _DEBUG
		/* //at last, was poping all of these for testing
		CRect rect1(this->GetWorldRect()); //whole image
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_worldrect= rect1; //selection rect
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_bRawData = FALSE; //FALSE, will use doc pixelset imghandle
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pDoc = this; 
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pPixelset = NULL;
		((COIIIApp*)AfxGetApp())->m_pOW2DocTemplate->OpenDocumentFile(NULL);

		CRect rect12(0,0,40,40); //image sub rect
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_worldrect= rect12; //selection rect
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_bRawData = FALSE; //FALSE, will use doc pixelset imghandle
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pDoc = this; 
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pPixelset = NULL;
		((COIIIApp*)AfxGetApp())->m_pOW2DocTemplate->OpenDocumentFile(NULL);

		CRect rect13(12,0,40,40); //image sub rect
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_worldrect= rect13; //selection rect
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_bRawData = FALSE; //FALSE, will use doc pixelset imghandle
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pDoc = this; 
		((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_pPixelset = NULL;
		((COIIIApp*)AfxGetApp())->m_pOW2DocTemplate->OpenDocumentFile(NULL);
		*/
	
		/*
		pLogDocument->AddText("     Rendering pixelset buffer ...");
		CString myTextBuffer;
		m_pPixelset->RenderPixelsetDataAsTextBuffer(&myTextBuffer);
		pLogDocument->AddText("\r\n");
		pLogDocument->AddText((const char*)myTextBuffer);
		pLogDocument->AddText("done.\r\n");
		return;
		*/
	#endif //_DEBUG

	//2012july25, poirier, begin
	//m_pPointset = (COWPointset*) new CPointset(POINTSET_INITIAL_NUMBEROFPOINTS);
	m_pPointset = (COWPointset*) new COWPointset(POINTSET_INITIAL_NUMBEROFPOINTS, POINTSET_DEFAULT_NUMBEROFSTAT);
	//2012july25, poirier, end
	if(m_pPointset==NULL) 
	{
		if(m_parentHWND==NULL) //spi 2014
		{
			if(m_bLocalTextureExtrationStarted)
			{
				pLogDocument->EnableViewUpdating();
			}
			pLogDocument->AddText("Error\r\nError, not enough memory to allocate pointset\r\n");
		}
		return;
	}
	//2012august15, poirier, begin
	ASSERT(m_pPixelset->IsBIIPresent()); //BII must be allocated
	//2012august15, poirier, end

	if(m_parentHWND==NULL) //spi 2014
	{
		pLogDocument->AddText("     Extracting texture from pixelset ...");
	}
	//2020august09, spi, begin
	//m_pPixelset->SetTCDThreshold(m_pExtractionSettings->iTCDThreshold);
	//m_pPixelset->SetTCDScale(m_pExtractionSettings->iTCDScale);
	if (m_parentHWND == NULL) 
	{
		//do this only when using oifii from oiii (mfc gui)
		m_pPixelset->SetTCDThreshold(m_pExtractionSettings->iTCDThreshold);
		m_pPixelset->SetTCDScale(m_pExtractionSettings->iTCDScale);
	}
	//2020august09, spi, end
	m_pPixelset->ExtractLocalMax(m_pPointset);
	if(m_parentHWND==NULL) //spi 2014
	{
		pLogDocument->AddText("done.\r\n");
	}
	/*
	//temporary for testing
	pLogDocument->AddText("     Erasing BII pixelset ...");
	if(m_pPixelset->FillBII(255)) pLogDocument->AddText("done.\r\n");
	  else pLogDocument->AddText("Error\r\nError, FillBII() failed\r\n");	
	*/

	/*
	//temporary for testing
	pLogDocument->AddText("     Updating IFF pixelset from BII pixelset ...");
	if(m_pPixelset->UpdateIFFFromBII()) pLogDocument->AddText("done.\r\n");
	  else pLogDocument->AddText("Error\r\nError, UpdateIFFFromBII() failed\r\n");	
	*/
	

	//testing buffer allocation, begin
	#ifdef _DEBUG
	//assuming IFF and BII buffers are present at start
	//more tempporary code
	//AfxMessageBox("hello, keep testing ?");
	/*
	BITMAPINFOHEADER* pDIB = m_pPixelset->GetIFFHeaderPointer();
	m_pPixelset->AllocIPL(pDIB);
	m_pPixelset->UpdateIPLFrom(pDIB);
	m_pPixelset->UpdateIFFFrom(m_pPixelset->GetIPLPointer());
	*/
	/*
	m_pPixelset->UpdateIFFFromBII();
	*/
	/* 
	//poirier, oct 2001, begin
	//from IFF to IPL to BII to IFF 
	BITMAPINFOHEADER* pDIB = m_pPixelset->GetIFFHeaderPointer();
	m_pPixelset->AllocIPL(pDIB);
	m_pPixelset->UpdateIPLFrom(pDIB);
	m_pPixelset->FreeBII();
	m_pPixelset->AllocBII(m_pPixelset->GetIPLPointer());
	m_pPixelset->UpdateBIIFrom(m_pPixelset->GetIPLPointer());
	m_pPixelset->UpdateIFFFromBII();
	m_pPixelset->FreeIPL();
	//poirier, oct 2001, end
	*/

	/*
	//from IFF to DIB to BII to IFF
	BITMAPINFOHEADER* pDIB = m_pPixelset->GetIFFHeaderPointer();
	m_pPixelset->AllocDIB(pDIB);
	m_pPixelset->UpdateDIBFrom(pDIB);
	m_pPixelset->FreeBII();
	m_pPixelset->AllocBII(pDIB);
	m_pPixelset->UpdateBIIFrom(pDIB);
	m_pPixelset->UpdateIFFFromBII();
	m_pPixelset->FreeDIB();
	*/
	#endif //_DEBUG
	//testing buffer allocation, end


	/* //poirier dec 98, let's keep image buffer around
	pLogDocument->AddText("     Freeing pixelset buffer ...");
	m_pPixelset->FreeImgBuffer();
	pLogDocument->AddText("done.\r\n");
	*/

	if(m_pPointset->GetNumberOfPoints()<3) 
	{
		if(m_parentHWND==NULL) //spi 2014
		{
			if(m_bLocalTextureExtrationStarted)
			{
				pLogDocument->EnableViewUpdating();
			}
			pLogDocument->AddText("Not enough texture vector found, will delete pointset");
		}
		//DeletePointset(m_pPointset);
		delete m_pPointset;
		m_pPointset = NULL;
		return;
	}

	//build TIN
	m_pPointset->BuildTINAndComputeStatistics();

	if(m_parentHWND==NULL) //spi 2014
	{
		//get first view and set initial view attribute
		POSITION pos = GetFirstViewPosition();
		COW2View* pFirstView = (COW2View*) GetNextView( pos );
		if(pFirstView!=NULL) pFirstView->m_pViewDataSettings->bDrawPointset = TRUE;

		if(m_bLocalTextureExtrationStarted)
		{
			pLogDocument->EnableViewUpdating();
		}

		//update all views
		UpdateAllViews(NULL);
		//if(pFirstView!=NULL) pFirstView->Invalidate(FALSE);
	}
	else
	{
		//::InvalidateRect(m_parentHWND, NULL, false);
	}
	return;	
}

void COW2Doc::OnUpdateAnalysisTextureextraction(CCmdUI* pCmdUI) 
{
	if(m_pPixelset!=NULL && m_pPointset==NULL) pCmdUI->Enable(TRUE);
	  else pCmdUI->Enable(FALSE);

}

void COW2Doc::OnAnalysisTexturesegmentation() 
{
	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	pLogDocument->AddText("Will now perform texture segmentation\r\n");

	ASSERT(m_pPointset!=NULL);
    // if pointset loaded, it is assumed triangle network is built 
    if( (m_pPointset!=NULL) && (m_pSegmentset==NULL) )
    {
		printf("\n");
		pLogDocument->AddText("     allocating segmentset ... ");
		m_pSegmentset = NewSegmentSet(m_pPointset->GetNumberOfPoints(), (m_pPointset->GetNumberOfPoints())*2-1);
		pLogDocument->AddText("done.\r\n");
	
		pLogDocument->AddText("     initializing segmentset from pointset ... ");
		InitializeSegmentSet(m_pSegmentset, m_pPointset->m_pPOINTSET);
		pLogDocument->AddText("done.\r\n");

		#ifdef _DEBUG	
			pLogDocument->AddText("     dumping to disk, segset info ...");
			DumpInfoSegmentSetToFile(m_pSegmentset, "segset0.dat");
			pLogDocument->AddText("done.\r\n");
	
			pLogDocument->AddText("     dumping to disk, segment list info ...");
			DumpInfoSegmentToFile(m_pSegmentset, "segment0.dat");
			pLogDocument->AddText("done.\r\n");
		#endif //_DEBUG

		pLogDocument->AddText("     building fusion tree ... ");
		BuildFusionTree(m_pSegmentset);
		pLogDocument->AddText("done.\r\n");
	
		#ifdef _DEBUG	
			pLogDocument->AddText("     dumping to disk, segset info (after tree built) ...");
			DumpInfoSegmentSetToFile(m_pSegmentset, "segset2.dat");
			pLogDocument->AddText("done.\r\n");

			pLogDocument->AddText("     dumping to disk, segment list info (after tree built) ...");
			DumpInfoSegmentToFile(m_pSegmentset, "segment2.dat");
			pLogDocument->AddText("done.\r\n");
	
			pLogDocument->AddText("     dumping to disk, fusion tree info ...");
			DumpInfoFusionToFile(m_pSegmentset, "fusion2.dat");
			pLogDocument->AddText("done.\r\n");
		#endif //_DEBUG

		pLogDocument->AddText("     saving segmentset(*.set, *.seg, *.nei, *.fus) to disk ...");
		CStringA myStringA = GetPathName();
		char pszCharBuf[1024];
		strcpy(pszCharBuf, (const char*)myStringA);
		SaveSegmentSet(m_pSegmentset, pszCharBuf);
		pLogDocument->AddText("done.\r\n");
	    
		
		pLogDocument->AddText("Texture segmentation successfully terminated\r\n");
		//update goto partition dlg
		COIIIApp* myApp = (COIIIApp*)AfxGetApp();
		if(myApp->m_pGotoPartitionDlg!=NULL)
		{
			myApp->m_pGotoPartitionDlg->PostMessage(OWM_GOTODLG_UPDATEINFO);
		}
	}
	else
	{
		ASSERT(FALSE); //development-time error
	}
	//pView = new COW2View;
	//AddView(pView)
	//UpdateAllViews(NULL);
	return;
}

void COW2Doc::OnUpdateAnalysisTexturesegmentation(CCmdUI* pCmdUI) 
{
	if(m_pPointset!=NULL && m_pSegmentset==NULL) pCmdUI->Enable(TRUE);
	  else pCmdUI->Enable(FALSE);
}


void COW2Doc::OnFileSave() 
{
	OnFileSaveAs();
}

void COW2Doc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void COW2Doc::OnFileSaveAs() 
{
    //1) get actual document path and file name
	CString stringPathName("");
	CString stringFileName("");
	if(GetPathName() == "")
	{
		//if new unsaved file, get current directory
		stringPathName = COWDocument::GetCurrentDirectory();
		//get current and document's title
	   	stringFileName = "log.txt";
	}
	else
	{   
		stringPathName = COWDocument::GetPathNameOnly();
		stringFileName = COWDocument::GetFileNameOnly(); 
	} 

	OPENFILENAME ofn;           
	memset(&ofn, 0, sizeof(OPENFILENAME));	//set structure to zero
	//char* pszFilter[]= {"2D World File (*.ow2)\0*.ow2;\03D World File (*.ow3)\0*.ow3\0Bitmap Image File (*.bmp)\0*.bmp;\0Tiff Image File (*.tif)\0*.tif;\0Jpeg Image File (*.jpg)\0*.jpg;\0Photoshop Image (*.psd)\0*.psd;\0Tab Text Files (*.txt)\0*.txt;\0Segmentation Files (*.seg)\0*.seg;\0All Files (*.*)\0*.*\0\0"};
	WCHAR* pszFilter[]= {L"2D World File (*.ow2)\0*.ow2\0Bitmap Image (*.bmp)\0*.bmp\0Tiff Image (*.tif)\0*.tif\0Jpeg Image (*.jpg)\0*.jpg\0Photoshop Image (*.psd)\0*.psd\0Tab Text Files (*.txt)\0*.txt\0Segmentation Files (*.seg)\0*.seg\0All Files (*.*)\0*.*\0\0"};

	WCHAR PathName[OW2DOC_MAXFILENAMELENGTH];
	WCHAR FileName[OW2DOC_MAXFILENAMELENGTH];			
	WCHAR FileTitle[OW2DOC_MAXFILENAMELENGTH];

	wcscpy(PathName, stringPathName);
	wcscpy(FileName, stringFileName);

	FileTitle[0]='\0';
   	ofn.lStructSize       = sizeof (OPENFILENAME);
   	ofn.hwndOwner         = AfxGetApp()->m_pMainWnd->GetSafeHwnd();
   	ofn.lpstrFilter       = pszFilter[0];
   	ofn.lpstrFile         = FileName;
   	ofn.nMaxFile          = OW2DOC_MAXFILENAMELENGTH; //sizeof (FileName);
   	ofn.lpstrFileTitle    = FileTitle;
   	ofn.nMaxFileTitle     = OW2DOC_MAXFILENAMELENGTH; //sizeof (FileTitle);
   	ofn.lpstrInitialDir   = PathName; 
   	ofn.lpstrTitle		  = L"File Save As";
   	ofn.Flags             = OFN_HIDEREADONLY | OFN_SHOWHELP | OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT| OFN_PATHMUSTEXIST;

    if (GetSaveFileName(&ofn))
 	{
		//on ok
		OnSaveDocument(ofn.lpstrFile);
   	}
	return; //user canceled, return safely
}

void COW2Doc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

long COW2Doc::IsFileExtensionOfPixelsetType(const char* pszFileExtention)
{
	long iImageType = 0; //FALSE;

	if(pszFileExtention!=NULL)
	{
		//check if one of the accusoft supported image file format
		BOOL bAccusoftImageType = FALSE;
		long accu_img_type=0;
		while(strcmp(OFF_image_fileextension_stringtable[accu_img_type],pszFileExtention) 
			&& accu_img_type<MAXFILTERS)
		{
			accu_img_type++;
		}
		if(accu_img_type==MAXFILTERS) 
		{
			bAccusoftImageType = FALSE;
		}
		else
		{
			bAccusoftImageType = TRUE;
			iImageType = accu_img_type;
		}

		//check if one of the offworld supported image file type
		if(bAccusoftImageType==FALSE)
		{
			BOOL bOffworldImageType = FALSE;
			//todo
			long iOffworldImageType = 0;
			iImageType = iOffworldImageType;
		}
	}
	return iImageType;
}

BOOL COW2Doc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CStringA pathnameStringA(lpszPathName);
	//this filename's extension
    char*  pCharFileExt = strrchr((char*)lpszPathName,'.');
	if (pCharFileExt == NULL)
	{
		//no valid file extension
		ASSERT(FALSE);
		return FALSE;
	}
	CString pszFileExt = _strupr( pCharFileExt );
	CStringA myStringA(pszFileExt);
	if (pszFileExt == ".")
	{
		//no valid file extension
		ASSERT(FALSE);
		return FALSE;
	}

	//save as a 2D world document
	if(pszFileExt==".OW2")
	{
		BOOL bResult = SaveFileAs2DWorldDocument(lpszPathName);
		if(bResult) SetModifiedFlag(FALSE); 
		return bResult;
	}
	//save as pixelset to disk
	else if (IsFileExtensionOfPixelsetType((const char*)myStringA))
	{ 
		if(pszFileExt==".OW2")
		{
			ASSERT(FALSE);
		}
		if(pszFileExt==".TXT")
		{
			ASSERT(FALSE);
		}


		//poirier, may 2003, begin
		//before saving, merge all visible layers into image buffer
		//that means, we want to save the DIB attached to the view 

		//get active view
		CView* pView = NULL;
		CMDIChildWnd* pMDIChildWnd = (CMDIChildWnd*)((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->GetActiveFrame();
		if(pMDIChildWnd) pView = pMDIChildWnd->GetActiveView();
		COWView* pActiveOWView = NULL;
		if(pView->IsKindOf(RUNTIME_CLASS(COWView))) pActiveOWView = (COWView*)pView;
		BOOL bViewPoint = pActiveOWView->m_pViewDataSettings->bDrawPointset && m_pPointset!=NULL;
		BOOL bViewPointLabels = pActiveOWView->m_pViewDataSettings->bDrawPointsetLabels && m_pPointset!=NULL;
		BOOL bViewTri = pActiveOWView->m_pViewDataSettings->bDrawTriangleset && m_pPointset!=NULL;
		BOOL bViewTriLabels = pActiveOWView->m_pViewDataSettings->bDrawTrianglesetLabels && m_pPointset!=NULL;
		BOOL bViewVoro = pActiveOWView->m_pViewDataSettings->bDrawVoronoiset && m_pPointset!=NULL;

		if(m_pPointset!=NULL || m_pSegmentset!=NULL)
		{
			HANDLE hDIB = ((COW2View*)pActiveOWView)->CreateDIBFromRender2D();
			if(hDIB)
			{
				//////////////////////////////////////////
				//save DIB to disk
				/*
				CFile m_file;
				if(!m_file.Open(fname,CFile::modeWrite |
							  CFile::modeCreate,NULL))
				return FALSE;
				BITMAPFILEHEADER hdr;
				LPBITMAPINFOHEADER lpbi;
				lpbi=(LPBITMAPINFOHEADER ) hDIB;
				int nColors= 1 << lpbi->biBitCount;
				hdr.bfType= ((WORD) ('M' << 8) | 'B');
				hdr.bfSize=sizeof(hdr) + GlobalSize(hDIB);
				hdr.bfReserved1=0;
				hdr.bfReserved2=0;
				hdr.bfOffBits=(DWORD) sizeof(hdr) + nColors * sizeof(RGBQUAD);
				m_file.Write(&hdr,sizeof(hdr));
				m_file.Write(lpbi,GlobalSize(hDIB));
				m_file.Close();
				*/
				CFile m_file;
				if(!m_file.Open(L"test.bmp",CFile::modeWrite | CFile::modeCreate,NULL)) 
					return FALSE;
				BITMAPFILEHEADER myBIH;
				LPBITMAPINFOHEADER pDIB = (LPBITMAPINFOHEADER) hDIB;
				int nColors= 1 << pDIB->biBitCount;
				myBIH.bfType= ((WORD) ('M' << 8) | 'B');
				//2020july03, spi, begin
				//myBIH.bfSize = sizeof(myBIH) + GlobalSize(pDIB);
				myBIH.bfSize=(DWORD)(sizeof(myBIH) + GlobalSize(pDIB));
				//2020july03, spi, end
				myBIH.bfReserved1=0;
				myBIH.bfReserved2=0;
				myBIH.bfOffBits=(DWORD) sizeof(myBIH) + nColors * sizeof(RGBQUAD);
				m_file.Write(&myBIH,sizeof(myBIH));
				//2020july03, spi, begin
				//m_file.Write(pDIB, GlobalSize(hDIB));
				m_file.Write(pDIB, static_cast<UINT>(GlobalSize(hDIB)));
				//2020july03, spi, end
				m_file.Close();
				//////////////////////////////////////////

				GlobalFree(hDIB);
			}
		}
		//there is only the pixelset present and request to saveas pixelset 
		//(case where pixelset is converted to a different file format)
		else if(m_pPixelset!=NULL && bViewPoint==FALSE && bViewPointLabels==FALSE
				&& bViewTri==FALSE && bViewTriLabels==FALSE && bViewVoro==FALSE)
		{
			//todo, check the view to find out if all pixelset's channels are visible  
			//in this implementation, alwayssave all channels

			if(OIII_USE_OWIFFACCUSOFT==1)
			{
				//check if one of the accusoft supported image file format
				long accu_img_type = IsFileExtensionOfPixelsetType((const char*)myStringA);
				if(accu_img_type>0)
				{
					//if accusoft image buffer present
					if(m_pPixelset->IsIFFPresent())
					{
						char pFilename[1024];
						strcpy(pFilename, pathnameStringA);
						int success = OW_IFF_IMG_save_bitmap(m_pPixelset->GetIFFHandle(), pFilename, accu_img_type);
					}
				}
			}
			if(OIII_USE_FREEIMAGE==1)
			{
				if(m_pPixelset->IsDIBPresent())
				{
					fipWinImage myfipWinImage;
					HGLOBAL myHGLOBAL = m_pPixelset->GetDIBCopy();
					myfipWinImage.copyFromHandle(myHGLOBAL);
					ASSERT(FALSE); //further implement, it is a saveas, we must select the filetype in here
					FREE_IMAGE_FORMAT myFREE_IMAGE_FORMAT = myfipWinImage.identifyFIF(pathnameStringA);
					BOOL bResult = myfipWinImage.save(pathnameStringA);
					GlobalFree(myHGLOBAL);
				}
			}
		}
		
		//poirier, may 2003, end

	}

	/* //poirier, may 2002
	//save pointset to disk	
	if(m_pPointset)
	{
	}

	//save segmentset(*.set, *.seg, *.nei, *.fus) to disk
	if(m_pSegmentset)
	{
		char pszCharBuf[1024];
		strcpy(pszCharBuf, (const char*)lpszPathName);
		SaveSegmentSet(m_pSegmentset, pszCharBuf);
	}
	//return COWDocument::OnSaveDocument(lpszPathName);
	SetModifiedFlag(FALSE); 
	*/
	return TRUE;
}

void COW2Doc::OnViewNew2dview() 
{
	//get active view
	CView* pView = NULL;
	CMDIChildWnd* pMDIChildWnd = (CMDIChildWnd*)((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->GetActiveFrame();
	if(pMDIChildWnd) pView = pMDIChildWnd->GetActiveView();
	COWView* pActiveOWView = NULL;
	if(pView->IsKindOf(RUNTIME_CLASS(COWView))) pActiveOWView = (COWView*)pView;

	//use 2d doc template
	COW2MDI* pNewMDIFrame = (COW2MDI*) (((COIIIApp*)AfxGetApp())->m_pOW2DocTemplate->CreateNewFrame(this, NULL));
	if(pNewMDIFrame==NULL) return;
	((COIIIApp*)AfxGetApp())->m_pOW2DocTemplate->InitialUpdateFrame(pNewMDIFrame, this);

	//get pointer to newly created view
	COWView* pOWView = NULL;
	POSITION pos = GetFirstViewPosition();
	COWView* pFirstOWView = (COWView*) GetNextView(pos);
	COWView* pPrevLastOWView = pFirstOWView;
	COWView* pLastOWView = NULL;
	while(pos)
	{
		pPrevLastOWView = pLastOWView;
		pLastOWView = (COWView*) GetNextView(pos);
	}

	//last view is the newly created view
	if(pActiveOWView)
	{
		pLastOWView->m_pViewDataSettings->CopyAllValuesFrom(pActiveOWView->m_pViewDataSettings);
	}
	return;
}

void COW2Doc::OnViewNew3dview() 
{
	//get active view
	CView* pView = NULL;
	CMDIChildWnd* pMDIChildWnd = (CMDIChildWnd*)((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->GetActiveFrame();
	if(pMDIChildWnd) pView = pMDIChildWnd->GetActiveView();
	COWView* pActiveOWView = NULL;
	if(pView->IsKindOf(RUNTIME_CLASS(COWView))) pActiveOWView = (COWView*)pView;

	//use 3d doc template
	COW2MDI* pNewMDIFrame = (COW2MDI*) (((COIIIApp*)AfxGetApp())->m_pOW3DocTemplate->CreateNewFrame(this, NULL));
	if(pNewMDIFrame==NULL) return;
	((COIIIApp*)AfxGetApp())->m_pOW3DocTemplate->InitialUpdateFrame(pNewMDIFrame, this);

	//get pointer to newly created view
	COWView* pOWView = NULL;
	POSITION pos = GetFirstViewPosition();
	COWView* pFirstOWView = (COWView*) GetNextView(pos);
	COWView* pPrevLastOWView = pFirstOWView;
	COWView* pLastOWView = NULL;
	while(pos)
	{
		pPrevLastOWView = pLastOWView;
		pLastOWView = (COWView*) GetNextView(pos);
	}

	//last view is the newly created view
	if(pActiveOWView)
	{
		pLastOWView->m_pViewDataSettings->CopyAllValuesFrom(pActiveOWView->m_pViewDataSettings);
	}
	return;
}

int COW2Doc::GetPixelsetNumberOfChannel()
{
	//validate
	if(m_pPixelset==NULL || m_pPixelset->GetBIIPointer()==NULL)
	{
		ASSERT(FALSE);
		return -1;
	}
	//return m_pPixelset->m_numberofchannel;
	return m_pPixelset->GetPixelsetNumberOfChannel();
}

int COW2Doc::GetPixelsetIntensity(int idpixelsetchannel, int ioffsetx, int ioffsety)
{
	if(m_pPixelset==NULL || !(m_pPixelset->IsBIIPresent()) )
	{
		ASSERT(FALSE);
		return -1;
	}
	//validate idpixelsetchannel
	int iNumChannel = GetPixelsetNumberOfChannel();
	if(idpixelsetchannel<0 || idpixelsetchannel>=iNumChannel)
	{
		return -1;
	}

	//validate ioffsetx and ioffsety
	CRect myRect = GetWorldRect();
	if(ioffsetx<myRect.left || ioffsetx>myRect.right ||ioffsety<myRect.top || ioffsety>myRect.bottom)
	{
		return -1;
	}
	//2012august17, poirier, begin
	/*
	int iIntensity = m_pPixelset->GetPixelValue(ioffsetx, ioffsety, idpixelsetchannel);
	return iIntensity;
	*/
	int iIntensity = -1;
	RECT myRECT = m_pPixelset->GetCropRect();
	if(ioffsetx<myRECT.right && ioffsety<myRECT.bottom)
	{
		iIntensity = m_pPixelset->GetPixelValue(ioffsetx, ioffsety, idpixelsetchannel);
	}
	return iIntensity;
	//2012august17, poirier, end
}

//2022may18, spi, begin
//major revision to this function
//major vorogui bug was to have getnearestpointsetobject() return directly result from FindNearestNeighbor(m_pPOINTSET, dfX, dfY, p_itriseed),
//which was always returning "the nearest neighboring vertex" of the triangle in which point (dfX,dfY) resides".
//the problem was that the nearest pointset object can also often be a vextex of one of the adjacent triangles.
int COW2Doc::GetNearestPointsetObject(double dfX, double dfY, int* p_itriseed/*=NULL*/) 
{
	//validate ioffsetx and ioffsety
	CRect myRect = GetWorldRect();
	if(m_pPointset==NULL)
	{
		ASSERT(FALSE);
		return -1;
	}
	if(dfX<(double)myRect.left || dfX>(double)myRect.right ||
		dfY<(double)myRect.top || dfY>(double)myRect.bottom)
	{
		return -1;
	}

	int iTriSeed = 0;
	if(p_itriseed==NULL) p_itriseed = &iTriSeed; 
	//2022may18, spi, begin
	//int idObject = FindNearestNeighbor(m_pPointset->m_pPOINTSET, dfX, dfY, p_itriseed);
	int idObject = FindNearestNeighbor(m_pPointset->m_pPOINTSET, dfX, dfY, p_itriseed, POINTSET_ADJACENTTRIFLAG_FIRSTADJTRI);
	//2022may18, spi, end
	/*
	if(idObject<0) 
	{
		ASSERT(FALSE);
		return -1;
	}
	*/
	return idObject;
}
//2022may18, spi, end

CString COW2Doc::GetPointsetObjectStatAsString(int idpointsetobject, int idstatchannel/*=-1*/)
{
	CString myString = "";
	//validate idpointsetobject
	if(m_pPointset==NULL || idpointsetobject<0 || idpointsetobject>=GetNumberOfPoint())
	{
		ASSERT(FALSE);
		myString = "dev-time error";
		return myString;
	}

	double* pfStatistics = GetPointerToStatistics();
	int nStatPerPoint = GetNumberOfStatPerPoint(); 
	double dfFloat;

	//validate idstatchannel
	if(idstatchannel<-1 || idstatchannel>=nStatPerPoint)
	{
		ASSERT(FALSE);
		myString = "dev-time error";
		return myString;
	}

	int iStatBaseOffset = idpointsetobject*nStatPerPoint;
	if(idstatchannel!=-1)
	{
		dfFloat = pfStatistics[iStatBaseOffset+idstatchannel];
		//myString.Format(L"%s=%.2f", pszStatChannelNames[idstatchannel], dfFloat);
		myString.Format(L"%s=%.2f", pwcharStatChannelNames[idstatchannel], dfFloat);
	}
	else
	{
		CString myTempString;
		for(int i=0; i<nStatPerPoint; i++)
		{
			//2022jan03, spi, begin
			if (1) //for debugging
			{
				if(i==0)
				{
					myTempString.Format(L"%d", idpointsetobject);
					myString = "id=" + myTempString + ", ";
				}
			}
			//2022jan03, spi, end
			dfFloat = pfStatistics[iStatBaseOffset+i];
			myTempString.Format(L"S%d=%.2f", i, dfFloat);
			myString += myTempString;
			if(i!=nStatPerPoint-1) myString += ", ";
		}
	}
	return myString;
}

double COW2Doc::GetPointsetObjectStatAsDouble(int idpointsetobject, int idstatchannel)
{
	double dfReturnValue = 0.0;
	if(m_pPointset==NULL || idpointsetobject<0 || idpointsetobject>=GetNumberOfPoint())
	{
		ASSERT(FALSE);
		return dfReturnValue;
	}
	double* pfStatistics = GetPointerToStatistics();
	int nStatPerPoint = GetNumberOfStatPerPoint(); 
	//validate idstatchannel
	if(idstatchannel<-1 || idstatchannel>=nStatPerPoint)
	{
		ASSERT(FALSE);
		return dfReturnValue;
	}
	int iStatBaseOffset = idpointsetobject*nStatPerPoint;
	if(idstatchannel!=-1)
	{
		dfReturnValue = pfStatistics[iStatBaseOffset+idstatchannel];
	}
	else
	{
		ASSERT(FALSE);
		return dfReturnValue;
	}
	return dfReturnValue;
}

BOOL COW2Doc::GetCircularNeighborhoodProfileAroundVertex(int idvertex, 
														 int iorder,
														 int idstatchannel,
														 int* p_numvertexfound,
														 int* p_idvertexlist,
														 double* pfStatistics)
{
	if(p_numvertexfound==NULL || p_idvertexlist==NULL || idvertex<0 || idstatchannel<0)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//circular neighborhood profile
	POSITION pos = GetFirstViewPosition();
	CView* pFirstView;
	do
	{
		pFirstView = GetNextView( pos );
	} while(!pFirstView->IsKindOf(RUNTIME_CLASS(COW2View)));
	COW2View* pOW2View = NULL;
	int itriseed = 0;
	if(pFirstView->IsKindOf(RUNTIME_CLASS(COW2View)))
	{
		pOW2View = (COW2View*) pFirstView;
		itriseed = pOW2View->m_itriseed;
	}
	//collect vertices circularly around input vertex
	BOOL bIsFullySurrounded = FindAllNeighborSurroundingVertex(	m_pPointset->m_pPOINTSET,
																idvertex,
																iorder, //iOrder
																&itriseed,
																p_numvertexfound,
																p_idvertexlist);
	if(p_numvertexfound[0]<1)
	{
		//ASSERT(FALSE);
		return FALSE;
	}
	if(pfStatistics!=NULL)
	{
		//for each vertex, get statistics
		double* pdfStatistics = GetPointerToStatistics();
		int nStatPerPoint = GetNumberOfStatPerPoint(); 
		int istatoffset;
		for(int i=0; i<p_numvertexfound[0]; i++)
		{
			istatoffset = p_idvertexlist[i]*nStatPerPoint+idstatchannel;
			pfStatistics[i] = (double) pdfStatistics[istatoffset];
		}
	}
	return bIsFullySurrounded;
}


BOOL COW2Doc::GetSegmentEdgeProfile(int idvertex, //any vertex in the segment
									int idsegment, //id of the segment
									int iNumberOfSegmentForFinalPartition,
									int iorder,	//size of neighborhood 
									int* p_itriseed,
									int idstatchannel,
									int* p_numedgefound,
									int* p_idinvertexlist,
									int* p_idoutvertexlist,
									double* pdfStatistics)
{
	//collects 
	return TRUE;
}
																		
BOOL COW2Doc::GetFusionSequenceProfile(int idfusionstart,
									   int idfusionend,
									   int iFusionInfoType,
									   double* pdfStatistics)
{
	return TRUE;
}

//caller supplies the statistics and an allocated array of int to receive histogram
BOOL COW2Doc::MapStatisticsVectorIntoHistogram(	double* pfStatistics,
												int inumstatistics,
												int* p_iHistogram,
												int inumhistolevel,
												double* pfMin,
												double* pfMax)
{
	if(pfStatistics==NULL || p_iHistogram==NULL || inumhistolevel<1 
		|| pfMin==NULL || pfMax==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(inumhistolevel>inumstatistics)
	{
		//ASSERT(FALSE);
	}
	 

	//reset histogram vector
	for(int i=0; i<inumhistolevel; i++) 
	{
		p_iHistogram[i] = 0;
	}

	//compute scale factor used when mapping to histogram
	GetMinMaxForStatisticsVector(pfStatistics, inumstatistics, pfMin, pfMax);
	double fNumberOfLevel = (double) inumhistolevel;
	double fScaleFactor = (fNumberOfLevel-1.0f)/(pfMax[0] - pfMin[0]);

	//compute the distribution of the statistic channel
	for(int i=0; i<inumstatistics; i++)
	{
		//get value
		double fValue = pfStatistics[i];
		{
			//map it, that is find the corresponding index
			int index = (int) ((fValue-pfMin[0])*fScaleFactor + 0.5); //+0.5 for round off
			//increment count
			p_iHistogram[index]++;
		}
	}

	return TRUE;
}

void COW2Doc::OnViewNewgraphview() 
{
	// 0) make sure the statistics have already been computed
	double* pdfGlobalStatistics = GetPointerToStatistics();
	if(pdfGlobalStatistics==NULL) 
	{
		ASSERT(FALSE); //should not be able to call this function if stat not computed yet
		return;
	}
	int nStatPerPoint = GetNumberOfStatPerPoint();
	int nNumPoints = GetNumberOfPoint();

    // 1) create a new set doc-frame-view for the graphic
    COWGraphDoc* pGraphDoc = (COWGraphDoc*) ((COIIIApp*)AfxGetApp())->m_pGraphViewTemplate->OpenDocumentFile(NULL); 
	//pGraphDoc->SetTitle("Global Profile of all measured statistics for " + GetTitle());
	CStringA myTitleA(GetTitle());
	pGraphDoc->SetTitle("Global Profile of all measured statistics for " + myTitleA);
    pGraphDoc->SetXAxisLabel("X");
    pGraphDoc->SetYAxisLabel("Stat");
    pGraphDoc->SetGraphType(GRAPH_VBAR); //|GRAPH_PILED
	pGraphDoc->SetDefaultGraphBackgroundAndTextColor(RGB(0,0,0), RGB(255,255,255));

	// 2) copy the data to the graphdoc, one channel at a time
	if(nNumPoints>168) nNumPoints=168;
	int nMaxChannel = nStatPerPoint;
	if(nMaxChannel>5) nMaxChannel=5;
	for (int iOffset=0; iOffset<nMaxChannel; iOffset++)    // for each band
	{   
		pGraphDoc->AddGraphSet(	(double*)&(pdfGlobalStatistics[iOffset]), 
								nStatPerPoint,
								nNumPoints,
								NULL, //graphsetname
								0,
								TRUE); //TRUE for allocating a floattype array 
    }   
    
	//modif to activate image view after histogram creation
	POSITION pos = GetFirstViewPosition();
	CView* pFirstView = GetNextView( pos );
	ASSERT(pFirstView!=NULL);
	CFrameWnd* pFrameWnd = pFirstView->GetParentFrame();
	ASSERT(pFrameWnd);
	pFrameWnd->SetActiveView(pFirstView);
	return;
}

void COW2Doc::OnViewNewGraphdataview() 
{
	// 0) make sure the statistics have already been computed
	double* pdfGlobalStatistics = GetPointerToStatistics();
	if(pdfGlobalStatistics==NULL) 
	{
		ASSERT(FALSE); //should not be able to call this function if stat not computed yet
		return;
	}
	int nStatPerPoint = GetNumberOfStatPerPoint();
	int nNumPoints = GetNumberOfPoint();

    // 1) create a new set doc-frame-view for the graphic
    COWGraphDoc* pGraphDoc = (COWGraphDoc*) ((COIIIApp*)AfxGetApp())->m_pGraphDataViewTemplate->OpenDocumentFile(NULL); 
	//pGraphDoc->SetTitle("Global Profile of all measured statistics for " + GetTitle());
	CStringA myTitleA(GetTitle());
	pGraphDoc->SetTitle("Global Profile of all measured statistics for " + myTitleA);
    pGraphDoc->SetXAxisLabel("X");
    pGraphDoc->SetYAxisLabel("Stat");
	int iTypeFlag = GRAPH_VBAR;
    pGraphDoc->SetGraphType(iTypeFlag);
	pGraphDoc->SetDefaultGraphBackgroundAndTextColor(RGB(0,0,0), RGB(255,255,255));

	// 2) copy the data to the graphdoc, one channel at a time
	if(nNumPoints>168) nNumPoints=168;
	int nMaxChannel = nStatPerPoint;
	if(nMaxChannel>5) nMaxChannel=5;
	for (int iOffset=0; iOffset<nMaxChannel; iOffset++)    // for each band
	{   
		pGraphDoc->AddGraphSet(	(double*)&(pdfGlobalStatistics[iOffset]), 
								nStatPerPoint,
								nNumPoints,
								NULL, //graphsetname
								0,
								TRUE); //TRUE for allocating a floattype array 
    }   
    
	/*
	//modif to activate image view after histogram creation
	POSITION pos = GetFirstViewPosition();
	CView* pFirstView = GetNextView( pos );
	ASSERT(pFirstView!=NULL);
	CFrameWnd* pFrameWnd = pFirstView->GetParentFrame();
	ASSERT(pFrameWnd);
	pFrameWnd->SetActiveView(pFirstView);
	*/
	return;
}

void COW2Doc::OnAnalysisGlobalclassification() 
{
	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	pLogDocument->AddText("Will now perform pointset pre-classification (intensity)\r\n");

	ASSERT(m_pPointset!=NULL);
    // if pointset loaded, it is assumed triangle network is built 
    if( (m_pPointset!=NULL) && (m_pClassset==NULL) )
    {
		printf("\n");
		pLogDocument->AddText("     allocating classset and initializing from pointset ... ");
		//poirier, sept 2001, c++ interface, begin
		m_pClassset = new COWClassset(m_pPointset->m_pPOINTSET, m_pPointset->GetNumberOfPoints(), (m_pPointset->GetNumberOfPoints())*2-1);
		ASSERT(m_pClassset);
		/* //c interface
		m_pClassset = NewClassSet(m_pPointset->npts, (m_pPointset->npts)*2-1);
		*/
		//poirier, sept 2001, end
		pLogDocument->AddText("done.\r\n");
	
		//poirier, sept 2001, c++ interface, begin
		/* pLogDocument->AddText("     initializing classset from pointset ... ");
		//c interface
		InitializeClassSet(m_pClassset, m_pPointset);
		pLogDocument->AddText("done.\r\n");
		*/
		//poirier, sept 2001, end

		/*
		#ifdef _DEBUG	
			pLogDocument->AddText("     dumping to disk, classset info ...");
			DumpInfoSegmentSetToFile(m_pClassset, "classset0.dat");
			pLogDocument->AddText("done.\r\n");
	
			pLogDocument->AddText("     dumping to disk, class list info ...");
			DumpInfoSegmentToFile(m_pClassset, "class0.dat");
			pLogDocument->AddText("done.\r\n");
		#endif //_DEBUG
		*/

		pLogDocument->AddText("     building class fusion tree ... ");
		m_pClassset->BuildClassFusionTree();
		/*
		BuildClassFusionTree(m_pClassset);
		*/
		pLogDocument->AddText("done.\r\n");

		/*
		#ifdef _DEBUG	
			pLogDocument->AddText("     dumping to disk, classset info (after tree built) ...");
			DumpInfoSegmentSetToFile(m_pClassset, "classset2.dat");
			pLogDocument->AddText("done.\r\n");

			pLogDocument->AddText("     dumping to disk, class list info (after tree built) ...");
			DumpInfoSegmentToFile(m_pClassset, "class2.dat");
			pLogDocument->AddText("done.\r\n");
	
			pLogDocument->AddText("     dumping to disk, class fusion tree info ...");
			DumpInfoFusionToFile(m_pClassset, "classfusion2.dat");
			pLogDocument->AddText("done.\r\n");
		#endif //_DEBUG
		*/

		pLogDocument->AddText("Pointset pre-classification successfully terminated\r\n");

		//set default number of class in the current COWView
		CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd; //Get App frame wnd
		CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame(); //Get the active MDI child window.
		COWView* pOWView = (COWView*) pChild->GetActiveView(); // Get the active view attached to the active MDI child window.
		if(pOWView->IsKindOf(RUNTIME_CLASS(COWView)))
		{
			pOWView->m_iNumberOfSegmentForFinalClassification = 10;
		}

		//update goto classification dlg
		COIIIApp* myApp = (COIIIApp*)AfxGetApp();
		if(myApp->m_pGotoClassificationDlg!=NULL)
		{
			myApp->m_pGotoClassificationDlg->PostMessage(OWM_GOTODLG_UPDATEINFO);
		}

	}
	else
	{
		ASSERT(FALSE); //development-time error
	}
	return;
}

void COW2Doc::OnUpdateAnalysisGlobalclassification(CCmdUI* pCmdUI) 
{
	if(m_pPointset!=NULL && m_pClassset==NULL) pCmdUI->Enable(TRUE);
	  else pCmdUI->Enable(FALSE);
}

//i.e. store classified tree local max intensity into POINTSET_OFFSETSTAT_CLASSTREEINTENSITY pointset statistic
BOOL COW2Doc::UpdateAllClassification(int istatoffset, int iNumberOfSegmentForFinalClassification)
{
	//validate idpointsetobject
	if(m_pPointset==NULL || m_pClassset==NULL 
		|| iNumberOfSegmentForFinalClassification<1 || iNumberOfSegmentForFinalClassification>m_pPointset->GetNumberOfPoints()
		|| istatoffset<0 || istatoffset>(POINTSET_DEFAULT_NUMBEROFSTAT+POINTSET_EXTRA_NUMBEROFSTAT))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	double* pfStatistics = m_pPointset->GetPointerToStatistics();
	for(int idvertex=0; idvertex<m_pPointset->GetNumberOfPoints(); idvertex++)
	{
		int idclass = GetOwnerParentClassThreshold(m_pClassset->GetSegmentsetPointer(), idvertex, iNumberOfSegmentForFinalClassification);
		pfStatistics[idvertex*(m_pPointset->GetNumberOfStatPerPoint())+istatoffset] = (double)idclass;
	}
	return TRUE;
}
BOOL COW2Doc::UpdateClassification(int istatoffset, int idvertex, int iNumberOfSegmentForFinalClassification)
{
	//validate idpointsetobject
	if(m_pPointset==NULL || m_pClassset==NULL 
		|| iNumberOfSegmentForFinalClassification<1 || iNumberOfSegmentForFinalClassification>m_pPointset->GetNumberOfPoints()
		|| istatoffset<0 || istatoffset>(POINTSET_DEFAULT_NUMBEROFSTAT+POINTSET_EXTRA_NUMBEROFSTAT))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	double* pfStatistics = m_pPointset->GetPointerToStatistics();
	int idclass = GetOwnerParentClassThreshold(m_pClassset->GetSegmentsetPointer(), idvertex, iNumberOfSegmentForFinalClassification);
	pfStatistics[idvertex*(m_pPointset->GetNumberOfStatPerPoint())+istatoffset] = (double)idclass;
	return TRUE;
}

void COW2Doc::OnUpdateViewNew2dview(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void COW2Doc::OnUpdateViewNew3dview(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void COW2Doc::OnUpdateViewNewgraphview(CCmdUI* pCmdUI) 
{
	double* pdfGlobalStatistics = GetPointerToStatistics();
	if(pdfGlobalStatistics==NULL) pCmdUI->Enable(FALSE);
	  else pCmdUI->Enable(TRUE);
}

void COW2Doc::OnUpdateViewGraphdataview(CCmdUI* pCmdUI) 
{
	double* pdfGlobalStatistics = GetPointerToStatistics();
	if(pdfGlobalStatistics==NULL) pCmdUI->Enable(FALSE);
	  else pCmdUI->Enable(TRUE);
}

void COW2Doc::OnTransSpatialRotation() 
{
	//rotate viewing buffer
	/*
	int i = IMG_rotate_bitmap(m_pPixelset->m_imghandle, 90);
	*/
	//update all views
	UpdateAllViews(NULL);
}

void COW2Doc::OnTransSpatialStretch() 
{
	/*
	int width = 200;
	int height = 200;
	int i = IMG_resize_bitmap(m_pPixelset->m_imghandle, width, height);
	*/
	//int i = IMG_resize_bitmap_interp(m_pPixelset->m_imghandle, width, height);

	//update all views
	UpdateAllViews(NULL);
}

void COW2Doc::OnUpdateTransSpatialRotation(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void COW2Doc::OnTransSpatialConfinedstretch() 
{
	//m_pPixelset->OFF_confined_stretch();
	
	//update all views
	UpdateAllViews(NULL);
}

void COW2Doc::OnUpdateTransSpatialConfinedstretch(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void COW2Doc::OnTransLaunchscenario() 
{
	// TODO: Add your command handler code here
	
}

void COW2Doc::OnUpdateTransLaunchscenario(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void COW2Doc::OnUpdateTransSpatialStretch(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void COW2Doc::OnTransSpectralInvert() 
{
	// TODO: Add your command handler code here
	
}

void COW2Doc::OnUpdateTransSpectralInvert(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void COW2Doc::OnTransTemporalEvolve() 
{
	// TODO: Add your command handler code here
	
}

void COW2Doc::OnUpdateTransTemporalEvolve(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void COW2Doc::OnTransformationRepeatinsequence() 
{
	// TODO: Add your command handler code here
	
}

void COW2Doc::OnUpdateTransformationRepeatinsequence(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}




void COW2Doc::OnTransSpectralFilter() 
{
	/* //moved to COW2View::OnTransSpectralFilter() 
	if(m_wTransSpectralFilterTimerOn==FALSE)
	{
		m_wTransSpectralFilterTimerOn = SetTimer(m_nTransSpectralFilterTimerID, 1000);
	}
	else
	{
		KillTimer(m_nTransSpectralFilterTimerID);
		m_wTransSpectralFilterTimerOn = FALSE;

	}
	*/
	return;
}

void COW2Doc::TransSpectralFilter(int iIplFilterIndex)
{
	if(iIplFilterIndex==-1)
	{
		//free temporary IPL buffer
		if(temp_pIplImage) 
		{
			OW_IPL_iplDeallocateImage(temp_pIplImage);
			temp_pIplImage = NULL;
		}
		//update IFF (display buffer)
		IplImage* src_pIplImage = m_pPixelset->GetIPLPointer();
		if(src_pIplImage)
		{
			if(m_pPixelset->IsIFFPresent()) m_pPixelset->UpdateIFFFrom(src_pIplImage);
			if(m_pPixelset->IsDIBPresent()) m_pPixelset->UpdateDIBFrom(src_pIplImage);
			//update views
			UpdateAllViews(NULL);
		}
		return;
	}

	if(m_pPixelset->IsIFFPresent() || m_pPixelset->IsDIBPresent())
	{
		//free BII
		if(m_pPixelset->IsBIIPresent())
		{
			m_pPixelset->FreeBII();
		}

		//first time called
		if(iIplFilterIndex==0 && temp_pIplImage==NULL)
		{
			//alloc IPL and update IPL from IFF buffer (alloc and copy)
			BITMAPINFOHEADER* pDIB = NULL;
			if(m_pPixelset->IsIFFPresent()) pDIB = m_pPixelset->GetIFFHeaderPointer();
			if(m_pPixelset->IsDIBPresent()) pDIB = m_pPixelset->GetDIBPointer();
			m_pPixelset->AllocIPL(pDIB);
			m_pPixelset->UpdateIPLFrom(pDIB);

			//create new temporary IPL output buffer
			IplImage* src_pIplImage = m_pPixelset->GetIPLPointer();
			temp_pIplImage = m_pPixelset->CreateIPLFrom(src_pIplImage);
		}

		IplImage* src_pIplImage = m_pPixelset->GetIPLPointer();
		IplImage* dest_pIplImage = temp_pIplImage;

		/*
		typedef enum {
			IPL_PREWITT_3x3_V=0,
				IPL_PREWITT_3x3_H,
				IPL_SOBEL_3x3_V,   //vertical 
				IPL_SOBEL_3x3_H,   //horizontal 
				IPL_LAPLACIAN_3x3,
				IPL_LAPLACIAN_5x5,
				IPL_GAUSSIAN_3x3,
				IPL_GAUSSIAN_5x5,
				IPL_HIPASS_3x3,
				IPL_HIPASS_5x5,
				IPL_SHARPEN_3x3
		} IplFilter;
		
		IplFilter myIplFilter = IPL_PREWITT_3x3_V;//from 0 to 10 
		IplFilter myIplFilter = IPL_PREWITT_3x3_V;
		OW_IPL_iplFixedFilter(src_pIplImage, dest_pIplImage, myIplFilter);
		*/
		OW_IPL_iplFixedFilter(src_pIplImage, dest_pIplImage, (IplFilter)iIplFilterIndex);

		//update IPL (input processing buffer)
		//m_pPixelset->UpdateIPLFrom(dest_pIplImage);

		//update IFF (display buffer)
		if(m_pPixelset->IsIFFPresent()) m_pPixelset->UpdateIFFFrom(dest_pIplImage);
		if(m_pPixelset->IsDIBPresent()) m_pPixelset->UpdateDIBFrom(src_pIplImage);
		//update views
		UpdateAllViews(NULL);
	}
	return;
}

CString GetPathOnly(const char* lpszPathName)
{
	CString myString(lpszPathName);
	int pos = myString.ReverseFind('\\');
	if(pos==-1) return "";
	return myString.Mid(0,pos);
}

CString GetFilenameOnly(const char* lpszPathName)
{
	CString myString(lpszPathName);
	int pos = myString.ReverseFind('\\');
	if(pos==-1) return myString;
	int pos2 = myString.ReverseFind('.');
	if(pos2==-1) return myString.Mid(pos+1);
	return myString.Mid(pos+1, pos2-1-pos);
}

CString GetExtensionOnly(const char*lpszPathName)
{
	CString myString(lpszPathName);
	int pos = myString.ReverseFind('.');
	if(pos==-1)
	{
		ASSERT(FALSE);
		return "";
	}
	return myString.Mid(pos);
}

//spi, avril 2015, begin
BOOL COW2Doc::SaveVoronoiDiagramColoredWithPixelsetAsImagefile(LPCTSTR lpszPathname, COW2View* pOW2View, float portionofvoronoipolygonstofill, int inverseprob_onpixelfill,
	//2020july04, spi, begin
	int ibackgroundfill, int ired, int igreen, int iblue, int imagechannel, COW2Doc* pBackgroundColorsOW2Doc, //for possible values, see header or implementation here below
	int voronoisetedgesfilterflag, int voronoisetpointsfilterflag, //for possible values, see header or implementation here below
	COW2Doc* pAlternateColorsOW2Doc, int voronoisetedgesmapperflag, int voronoisetpointsmapperflag) //for possible values, see header or implementation here below
	//2020july04, spi, end
{

	CStringA myStringA(lpszPathname);
	if(m_pPixelset && m_pPointset && OIII_USE_FREEIMAGE==1)
	{
		if(m_pPixelset->IsDIBPresent())
		{
			//2020july16, spi, begin
			//allocate and compute HSV color components
			//note: required nowadays with hybrid colored vorotrans because color filtering and color mapping are performed based on HSV color space
			if (m_pPixelset->IsBIIPresent() && m_pPixelset->IsColor())
			{
				//this double the size of the whole image buffer, doubling the number of channel of an RGB image
				m_pPixelset->ReallocBIIforHSVandUpdateBIIComputingHSV();
			}
			//2020july16, spi, end

			//1) initialize image
			fipWinImage myfipWinImage;
			HGLOBAL myHGLOBAL = m_pPixelset->GetDIBCopy();
			myfipWinImage.copyFromHandle(myHGLOBAL);
			//ASSERT(FALSE); //further implement, it is a saveas, we must select the filetype in here
			FREE_IMAGE_FORMAT myFREE_IMAGE_FORMAT = myfipWinImage.identifyFIF(myStringA);
			//2020july04, spi, begin
			if (ibackgroundfill == 0 || ibackgroundfill == 1)
			{
				//2) erase image with solid color
				int width = myfipWinImage.getWidth();
				int height = myfipWinImage.getHeight();
				RGBQUAD myRGBQUAD;
				myRGBQUAD.rgbBlue = 0;
				myRGBQUAD.rgbGreen = 0;
				myRGBQUAD.rgbRed = 0;
				if (ibackgroundfill == 1)
				{
					if (ired < 0) ired = 0;
					if (ired > 255) ired = 255;
					if (igreen < 0) igreen = 0;
					if (igreen > 255) igreen = 255;
					if (iblue < 0) iblue = 0;
					if (iblue > 255) iblue = 255;
					myRGBQUAD.rgbBlue = iblue;
					myRGBQUAD.rgbGreen = igreen;
					myRGBQUAD.rgbRed = ired;
				}
				for (int iy = 0; iy < height; iy++)
				{
					for (int ix = 0; ix < width; ix++)
					{
						myfipWinImage.setPixelColor(ix, iy, &myRGBQUAD);
					}
				}
			}
			else if(ibackgroundfill==2)
			{
				if (imagechannel == 0)
				{
					//do nothing, it will use all image colors
				}
				else if (imagechannel == 1)
				{
					//turn background, our starting image buffer, to the red channel of the image
					int width = myfipWinImage.getWidth();
					int height = myfipWinImage.getHeight();
					RGBQUAD myRGBQUAD;
					myRGBQUAD.rgbBlue = 0;
					myRGBQUAD.rgbGreen = 0;
					myRGBQUAD.rgbRed = 0;
					RGBQUAD myredRGBQUAD;
					myredRGBQUAD.rgbBlue = 0;
					myredRGBQUAD.rgbGreen = 0;
					myredRGBQUAD.rgbRed = 0;
					int luma = 0;
					for (int iy = 0; iy < height; iy++)
					{
						for (int ix = 0; ix < width; ix++)
						{
							myfipWinImage.getPixelColor(ix, iy, &myRGBQUAD);
							myredRGBQUAD.rgbBlue = 0;
							myredRGBQUAD.rgbGreen = 0;
							myredRGBQUAD.rgbRed = myRGBQUAD.rgbRed;
							myfipWinImage.setPixelColor(ix, iy, &myredRGBQUAD);
						}
					}
				}
				else if (imagechannel == 2)
				{
					//turn background, our starting image buffer, to the green channel of the image
					int width = myfipWinImage.getWidth();
					int height = myfipWinImage.getHeight();
					RGBQUAD myRGBQUAD;
					myRGBQUAD.rgbBlue = 0;
					myRGBQUAD.rgbGreen = 0;
					myRGBQUAD.rgbRed = 0;
					RGBQUAD mygreenRGBQUAD;
					mygreenRGBQUAD.rgbBlue = 0;
					mygreenRGBQUAD.rgbGreen = 0;
					mygreenRGBQUAD.rgbRed = 0;
					int luma = 0;
					for (int iy = 0; iy < height; iy++)
					{
						for (int ix = 0; ix < width; ix++)
						{
							myfipWinImage.getPixelColor(ix, iy, &myRGBQUAD);
							mygreenRGBQUAD.rgbBlue = 0;
							mygreenRGBQUAD.rgbGreen = myRGBQUAD.rgbGreen;
							mygreenRGBQUAD.rgbRed = 0;
							myfipWinImage.setPixelColor(ix, iy, &mygreenRGBQUAD);
						}
					}
				}
				else if (imagechannel == 3)
				{
					//turn background, our starting image buffer, to the blue channel of the image
					int width = myfipWinImage.getWidth();
					int height = myfipWinImage.getHeight();
					RGBQUAD myRGBQUAD;
					myRGBQUAD.rgbBlue = 0;
					myRGBQUAD.rgbGreen = 0;
					myRGBQUAD.rgbRed = 0;
					RGBQUAD myblueRGBQUAD;
					myblueRGBQUAD.rgbBlue = 0;
					myblueRGBQUAD.rgbGreen = 0;
					myblueRGBQUAD.rgbRed = 0;
					int luma = 0;
					for (int iy = 0; iy < height; iy++)
					{
						for (int ix = 0; ix < width; ix++)
						{
							myfipWinImage.getPixelColor(ix, iy, &myRGBQUAD);
							myblueRGBQUAD.rgbBlue = myRGBQUAD.rgbBlue;
							myblueRGBQUAD.rgbGreen = 0;
							myblueRGBQUAD.rgbRed = 0;
							myfipWinImage.setPixelColor(ix, iy, &myblueRGBQUAD);
						}
					}
				}
				else if (imagechannel == 4)
				{
					//turn background, our starting image buffer, to the intensity (rgb to luma) channel of the image
					int width = myfipWinImage.getWidth();
					int height = myfipWinImage.getHeight();
					RGBQUAD myRGBQUAD;
					myRGBQUAD.rgbBlue = 0;
					myRGBQUAD.rgbGreen = 0;
					myRGBQUAD.rgbRed = 0;
					RGBQUAD myintensityRGBQUAD;
					myintensityRGBQUAD.rgbBlue = 0;
					myintensityRGBQUAD.rgbGreen = 0;
					myintensityRGBQUAD.rgbRed = 0;
					BYTE luma = 0;
					for (int iy = 0; iy < height; iy++)
					{
						for (int ix = 0; ix < width; ix++)
						{
							myfipWinImage.getPixelColor(ix, iy, &myRGBQUAD);
							//https://stackoverflow.com/questions/596216/formula-to-determine-brightness-of-rgb-color
							//one approx formula is Y = (R + R + B + G + G + G) / 6
							luma = (BYTE)(((int)(myRGBQUAD.rgbRed) + (int)(myRGBQUAD.rgbRed) + (int)(myRGBQUAD.rgbBlue) + (int)(myRGBQUAD.rgbGreen) + (int)(myRGBQUAD.rgbGreen) + (int)(myRGBQUAD.rgbGreen)) / 6);
							//luma = (0.2126*R + 0.7152*G + 0.0722*B)
							//luma = (BYTE)(0.2126*(double)(myRGBQUAD.rgbRed) + 0.7152*(double)(myRGBQUAD.rgbGreen) + 0.0722*(double)(myRGBQUAD.rgbBlue));
							myintensityRGBQUAD.rgbBlue = luma;
							myintensityRGBQUAD.rgbGreen = luma;
							myintensityRGBQUAD.rgbRed = luma;
							myfipWinImage.setPixelColor(ix, iy, &myintensityRGBQUAD);
						}
					}
				}
			}
			else if (ibackgroundfill == 3 && pBackgroundColorsOW2Doc!=NULL && pBackgroundColorsOW2Doc->m_pPixelset!=NULL
					&& pBackgroundColorsOW2Doc->m_pPixelset->IsDIBPresent())
			{
				//no need to duplicate background image buffer
				//fipWinImage mybackgroundfipWinImage;
				//HGLOBAL mybackgroundHGLOBAL = pBackgroundColorsOW2Doc->m_pPixelset->GetDIBCopy();
				//mybackgroundfipWinImage.copyFromHandle(mybackgroundHGLOBAL);

				//erase image with supplied background colors
				int width = myfipWinImage.getWidth();
				int height = myfipWinImage.getHeight();
				RGBQUAD myRGBQUAD;
				myRGBQUAD.rgbBlue = 0;
				myRGBQUAD.rgbGreen = 0;
				myRGBQUAD.rgbRed = 0;
				RGBQUAD myblackRGBQUAD;
				myblackRGBQUAD.rgbBlue = 0;
				myblackRGBQUAD.rgbGreen = 0;
				myblackRGBQUAD.rgbRed = 0;
				//int idice = random_int(1, 4);
				int idice = random_int(1, inverseprob_onpixelfill);
				for (int iy = 0; iy < height; iy++)
				{
					for (int ix = 0; ix < width; ix++)
					{
						//get color
						//mybackgroundfipWinImage.getPixelColor(ix, iy, &myRGBQUAD);
						myRGBQUAD.rgbBlue = pBackgroundColorsOW2Doc->m_pPixelset->GetPixelValue(ix, iy, 2);
						myRGBQUAD.rgbGreen = pBackgroundColorsOW2Doc->m_pPixelset->GetPixelValue(ix, iy, 1);
						myRGBQUAD.rgbRed = pBackgroundColorsOW2Doc->m_pPixelset->GetPixelValue(ix, iy, 0);
						//set color
						//idice = random_int(1,4);
						idice = random_int(1, inverseprob_onpixelfill);
						if (idice == 1)
						{
							if (!myfipWinImage.setPixelColor(ix, height -1 - iy, &myRGBQUAD))
							{
								break;
							}
						}
						else
						{
							if (!myfipWinImage.setPixelColor(ix, height - 1 - iy, &myblackRGBQUAD))
							{
								break;
							}
						}
					}
				}
				//GlobalFree(mybackgroundHGLOBAL);
			}
			//2020july04, spi, end
			//3) draw voronoi diagram colored with pixelset
			/*
			POSITION pos = GetFirstViewPosition();
			if (pos != NULL)
			{
				CView* pView = GetNextView(pos);
				if(pView->IsKindOf(RUNTIME_CLASS(COW2View)))
				{
					COW2View* pOW2View = (COW2View*)pView; 
					m_pPointset->DrawVoronoiset(pOW2View, NULL, POINTSET_ALL, NULL, POINTSET_DRAW_VORONOISET_AS_NVERTEX, &myfipWinImage);
				}
			}
			*/
			//2020july04, spi, begin
			//m_pPointset->DrawVoronoiset(pOW2View, NULL, POINTSET_ALL, NULL, POINTSET_DRAW_VORONOISET_AS_NVERTEX, &myfipWinImage, portionofvoronoipolygonstofill, inverseprob_onpixelfill);
			m_pPointset->DrawVoronoiset(pOW2View, NULL, POINTSET_ALL, NULL, POINTSET_DRAW_VORONOISET_AS_NVERTEX, &myfipWinImage, portionofvoronoipolygonstofill, inverseprob_onpixelfill,
				voronoisetedgesfilterflag, voronoisetpointsfilterflag, 
				pAlternateColorsOW2Doc, voronoisetedgesmapperflag, voronoisetpointsmapperflag);
			//2020july04, spi, end

			//4) save image
			BOOL bResult = myfipWinImage.save(myStringA);
			GlobalFree(myHGLOBAL);
		}
	}
	return TRUE;
}
//spi, avril 2015, end
