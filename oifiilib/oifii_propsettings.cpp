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

/////////////////////////////////////////////////////////////////////////////
//oiffii_propsettings.h file
//
//contain implementation of property page settings classes
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "oifiilib.h" //spi 2014
#include "oifii_app.h"

#include <gl\gl.h>
#include <gl\glu.h>
#include "oifii_propsettings.h"

#include "c_pointset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// behasett.cpp
//
// CBehaviorSettings implementation
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CBehaviorSettings, CObject)

CBehaviorSettings::CBehaviorSettings()
{
	SetDataMembersToDefault();
}

CBehaviorSettings::~CBehaviorSettings()
{
}

void CBehaviorSettings::SetDataMembersToDefault()
{
	bMBHighlightNearestPoint = FALSE;
	bMBHighlightNearestNeighbors = FALSE;
	bMBHighlightSegment = FALSE;

	bMBLocalExtraction = FALSE;
	bMBLocalSegmentation = FALSE;
	bMBLocalClassification = FALSE;

	bIBSelfTraining = FALSE;
	bIBSelfExtractionSegmentationValidation = FALSE;
}

BOOL CBehaviorSettings::CopyAllValuesFrom(CBehaviorSettings* pOther)
{
	if(pOther)
	{
		bMBHighlightNearestPoint = pOther->bMBHighlightNearestPoint;
		bMBHighlightNearestNeighbors = pOther->bMBHighlightNearestNeighbors;
		bMBHighlightSegment = pOther->bMBHighlightSegment;

		bMBLocalExtraction = pOther->bMBLocalExtraction;
		bMBLocalSegmentation = pOther->bMBLocalSegmentation;
		bMBLocalClassification = pOther->bMBLocalClassification;

		bIBSelfTraining = FALSE;
		bIBSelfExtractionSegmentationValidation = FALSE;
	}
	else
	{
		SetDataMembersToDefault();
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// classett.cpp : implementation file
//
// CClassificationSettings property page
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "oifii_app.h"
//#include "classett.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

IMPLEMENT_DYNCREATE(CClassificationSettings, CObject)

CClassificationSettings::CClassificationSettings()
{
	SetDataMembersToDefault();
}

CClassificationSettings::~CClassificationSettings()
{
}

void CClassificationSettings::SetDataMembersToDefault()
{
	bCTSupervised = FALSE;
	bCTNonsupervised = TRUE;
}

BOOL CClassificationSettings::CopyAllValuesFrom(CClassificationSettings* pOther)
{
	if(pOther)
	{
		bCTSupervised = pOther->bCTSupervised;
		bCTNonsupervised = pOther->bCTNonsupervised;
	}
	else
	{
		SetDataMembersToDefault();
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// extrsett.cpp : implementation file
//
// CExtractionSettings property page
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


//#include "stdafx.h"
//#include "oifii_app.h"
//#include "extrsett.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/


IMPLEMENT_DYNCREATE(CExtractionSettings, CObject)

CExtractionSettings::CExtractionSettings()
{
	SetDataMembersToDefault();
}

CExtractionSettings::~CExtractionSettings()
{
}

void CExtractionSettings::SetDataMembersToDefault()
{
	bTCDbyLocalMax = TRUE;
	iTCDThreshold = 80;
	iTCDScale = 3;
	bTCCArea = TRUE;
	bTCCDensity = TRUE;
	bTCCHeight = FALSE;
	bTCCAverageIntensity = TRUE;
	bTCCSpectralVariability = FALSE;

	bEZRoads = FALSE;
	bEZManMade = FALSE;
	bEZForestation = FALSE;
	bEZDeforestation = FALSE;

	bAllowEZTraining = FALSE;
}

BOOL CExtractionSettings::CopyAllValuesFrom(CExtractionSettings* pOther)
{
	if(pOther)
	{
		bTCDbyLocalMax = pOther->bTCDbyLocalMax;
		iTCDThreshold = pOther->iTCDThreshold;
		iTCDScale = pOther->iTCDScale;
		bTCCArea = pOther->bTCCArea;
		bTCCDensity = pOther->bTCCDensity;
		bTCCHeight = pOther->bTCCHeight;
		bTCCAverageIntensity = pOther->bTCCAverageIntensity;
		bTCCSpectralVariability = pOther->bTCCSpectralVariability;

		bEZRoads = pOther->bEZRoads;
		bEZManMade = pOther->bEZManMade;
		bEZForestation = pOther->bEZForestation;
		bEZDeforestation = pOther->bEZDeforestation;

		bAllowEZTraining = pOther->bAllowEZTraining;
	}
	else
	{
		SetDataMembersToDefault();
	}
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// fussett.cpp : implementation file
//
// CFusionSettings property page
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "oifii_app.h"
//#include "fussett.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

IMPLEMENT_DYNCREATE(CFusionSettings, CObject)

CFusionSettings::CFusionSettings()
{
	SetDataMembersToDefault();
}

CFusionSettings::~CFusionSettings()
{
}

void CFusionSettings::SetDataMembersToDefault()
{
	bSegTypeHSO = TRUE;
	bSegTypeHRG = FALSE;
	bSegTypeMixedHSOHRG = FALSE;

	idstatchannelUsedAsSimilarityCriteria = 0;
	bUseExclusionThreshold = FALSE;
	fExclusionThreshold = 0.0f;

	bUsePreClassificationInfo = FALSE;
	idstatchannelUsedAsPreClassificationInfo = 0;
}

BOOL CFusionSettings::CopyAllValuesFrom(CFusionSettings* pOther)
{
	if(pOther)
	{
		bSegTypeHSO = pOther->bSegTypeHSO;
		bSegTypeHRG = pOther->bSegTypeHRG;
		bSegTypeMixedHSOHRG = pOther->bSegTypeMixedHSOHRG;

		idstatchannelUsedAsSimilarityCriteria = pOther->idstatchannelUsedAsSimilarityCriteria;
		bUseExclusionThreshold = pOther->bUseExclusionThreshold;
		fExclusionThreshold = pOther->fExclusionThreshold;

		bUsePreClassificationInfo = pOther->bUsePreClassificationInfo;
		idstatchannelUsedAsPreClassificationInfo = pOther->idstatchannelUsedAsPreClassificationInfo;
	}
	else
	{
		SetDataMembersToDefault();
	}
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// projsett.cpp file
//
// CProjectionSettings class implementation
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


//#include "stdafx.h"
//#include "oifii_app.h"

/*
#include <gl\gl.h>
#include <gl\glu.h>
*/
//#include "projsett.h"


/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

IMPLEMENT_DYNCREATE(CProjectionSettings, CObject)

CProjectionSettings::CProjectionSettings()
{
	SetDataMembersToDefault();
}

CProjectionSettings::~CProjectionSettings()
{
}

void CProjectionSettings::SetDataMembersToDefault()
{
	fVVSLeft = -200.0;
	fVVSTop = 600.0;
	fVVSRight = 600.0;
	fVVSBottom = -200.0;
	fVVSNear = -400.0;
	fVVSFar = 800.0;

	bVVSPerspective = FALSE;
	bVVSOrthogonal = TRUE;

	fLAEyeX = 10.0;
	fLAEyeY = 10.0;
	fLAEyeZ = -10.0;
	fLACenterX = 5.0;
	fLACenterY = 5.0;
	fLACenterZ = 5.0;
	fLAUpX = 0.0;
	fLAUpY = 1.0;
	fLAUpZ = 0.0;

	bLAIgnore = TRUE;
	bLAUseIt = FALSE;
}

BOOL CProjectionSettings::CopyAllValuesFrom(CProjectionSettings* pOther)
{
	if(pOther)
	{
		fVVSLeft = pOther->fVVSLeft;
		fVVSTop = pOther->fVVSTop;
		fVVSRight = pOther->fVVSRight;
		fVVSBottom = pOther->fVVSBottom;
		fVVSNear = pOther->fVVSNear;
		fVVSFar = pOther->fVVSFar;

		bVVSPerspective = pOther->bVVSPerspective;
		bVVSOrthogonal = pOther->bVVSOrthogonal;

		fLAEyeX = pOther->fLAEyeX;
		fLAEyeY = pOther->fLAEyeY;
		fLAEyeZ = pOther->fLAEyeZ;
		fLACenterX = pOther->fLACenterX;
		fLACenterY = pOther->fLACenterY;
		fLACenterZ = pOther->fLACenterZ;
		fLAUpX = pOther->fLAUpX;
		fLAUpY = pOther->fLAUpY;
		fLAUpZ = pOther->fLAUpZ;

		bLAIgnore = pOther->bLAIgnore;
		bLAUseIt = pOther->bLAUseIt;
	}
	else
	{
		SetDataMembersToDefault();
	}
	return TRUE;
}




/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// tracsett.cpp 
//
// CTraceSettings property page
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


//#include "stdafx.h"
//#include "oifii_app.h"
//#include "tracsett.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/


IMPLEMENT_DYNCREATE(CTraceSettings, CObject)

CTraceSettings::CTraceSettings()
{
	bTPPixelset = FALSE;
	bTPPointset = FALSE;
	bTPTriangleset = FALSE;
	bTPVoronoiset = FALSE;
	bTPSegmentset = FALSE;
#ifdef _DEBUG
	bTPPixelset = TRUE;
	bTPPointset = TRUE;
	bTPTriangleset = TRUE;
	bTPVoronoiset = TRUE;
	bTPSegmentset = TRUE;
#endif //_DEBUG

	bTLAStat1 = FALSE;
	bTLAStat2 = FALSE;
	bTLAStat3 = FALSE;
	bTLAStat4 = FALSE;

	idchannelTLAStat1 = -1;
	idchannelTLAStat2 = -1;
	idchannelTLAStat3 = -1;
	idchannelTLAStat4 = -1;
}

CTraceSettings::~CTraceSettings()
{
}

BOOL CTraceSettings::CopyAllValuesFrom(CTraceSettings* pOther)
{
	if(pOther)
	{
		bTPPixelset = pOther->bTPPixelset;
		bTPPointset = pOther->bTPPointset;
		bTPTriangleset = pOther->bTPTriangleset;
		bTPVoronoiset = pOther->bTPVoronoiset;
		bTPSegmentset = pOther->bTPSegmentset;

		bTLAStat1 = pOther->bTLAStat1;
		bTLAStat2 = pOther->bTLAStat2;
		bTLAStat3 = pOther->bTLAStat3;
		bTLAStat4 = pOther->bTLAStat4;

		idchannelTLAStat1 = pOther->idchannelTLAStat1;
		idchannelTLAStat2 = pOther->idchannelTLAStat2;
		idchannelTLAStat3 = pOther->idchannelTLAStat3;
		idchannelTLAStat4 = pOther->idchannelTLAStat4;
	}
	else
	{
		bTPPixelset = FALSE;
		bTPPointset = FALSE;
		bTPTriangleset = FALSE;
		bTPVoronoiset = FALSE;
		bTPSegmentset = FALSE;

		bTLAStat1 = FALSE;
		bTLAStat2 = FALSE;
		bTLAStat3 = FALSE;
		bTLAStat4 = FALSE;

		idchannelTLAStat1 = -1;
		idchannelTLAStat2 = -1;
		idchannelTLAStat3 = -1;
		idchannelTLAStat4 = -1;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// transett.cpp file
//
// CTransformationSettings class implementation
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


//#include "stdafx.h"
//#include "oifii_app.h"

/*
#include <gl\gl.h>
#include <gl\glu.h>

#include "transett.h"
*/

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

IMPLEMENT_DYNCREATE(CTransformationSettings, CObject)

CTransformationSettings::CTransformationSettings()
{
	SetDataMembersToDefault();
}

CTransformationSettings::~CTransformationSettings()
{
}

void CTransformationSettings::SetDataMembersToDefault()
{
	fVTX = 0.0; 
	fVTY = 0.0; 
	fVTWidth = 10.0; 
	fVTHeight = 10.0; 

	fMTTranslateX = -100.0;  //0.0
	fMTTranslateY = 60.0; //0.0
	fMTTranslateZ = 0.0; 
	fMTScaleX = 1.2; 	//1.0
	fMTScaleY = 1.2; 	//1.0
	fMTScaleZ = 1.0; 
	fMTRotateX = -60.0; //-30 
	fMTRotateY = -10.0; 	//40
	fMTRotateZ = -50.0; //0

	bMTIgnore = TRUE;
	bMTUseIt = FALSE;
	bMTAbsolute = TRUE;
	bMTRelative = FALSE;
	return;
}

BOOL CTransformationSettings::CopyAllValuesFrom(CTransformationSettings* pOther)
{
	if(pOther)
	{
		fVTX = pOther->fVTX; 
		fVTY = pOther->fVTY; 
		fVTWidth = pOther->fVTWidth; 
		fVTHeight = pOther->fVTHeight; 

		fMTTranslateX = pOther->fMTTranslateX; 
		fMTTranslateY = pOther->fMTTranslateY; 
		fMTTranslateZ = pOther->fMTTranslateZ; 
		fMTScaleX = pOther->fMTScaleX; 
		fMTScaleY = pOther->fMTScaleY; 
		fMTScaleZ = pOther->fMTScaleZ; 
		fMTRotateX = pOther->fMTRotateX; 
		fMTRotateY = pOther->fMTRotateY; 
		fMTRotateZ = pOther->fMTRotateZ; 

		bMTIgnore = pOther->bMTIgnore;
		bMTUseIt = pOther->bMTUseIt;
		bMTAbsolute = pOther->bMTAbsolute;
		bMTRelative = pOther->bMTRelative;
	}
	else
	{
		SetDataMembersToDefault();	
	}
	return TRUE;
}




/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// viewsett.cpp file
//
// CViewDataSettings class implementation
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


//#include "stdafx.h"
//#include "oifii_app.h"
//#include "viewsett.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/


IMPLEMENT_DYNCREATE(CViewDataSettings, CObject)

CViewDataSettings::CViewDataSettings()
{
	SetDataMembersToDefault();
}

CViewDataSettings::~CViewDataSettings()
{
}

void CViewDataSettings::SetDataMembersToDefault()
{
	bDrawPixelset = FALSE;
	bDrawPointset = FALSE;
	bDrawTriangleset = FALSE;
	bDrawVoronoiset = FALSE; 
	bDrawSegmentset = FALSE;

	bDrawPointsetLabels = FALSE;
	bDrawTrianglesetLabels = FALSE;
	bDrawTriCenterLabels = FALSE;
	bDrawSegmentsetLabels = FALSE;

	bDrawClasssetLabels = FALSE;

	fPixelset_zoffset = 0.0f;
	fPointset_zoffset = -100.0f;
	fTriangleset_zoffset = 100.0f;
	fTriangleset_zrange = 20.0f;
	fVoronoiset_zoffset = 300.0f;
	fSegmentset_zoffset = 400.0f;

	idstatchannelMappedToTriHeight = 0;
}

BOOL CViewDataSettings::CopyAllValuesFrom(CViewDataSettings* pOther)
{
	if(pOther)
	{
		bDrawPixelset = pOther->bDrawPixelset;
		bDrawPointset = pOther->bDrawPointset;
		bDrawTriangleset = pOther->bDrawTriangleset;
		bDrawVoronoiset = pOther->bDrawVoronoiset; 
		bDrawSegmentset = pOther->bDrawSegmentset;

		bDrawPointsetLabels = pOther->bDrawPointsetLabels;
		bDrawTrianglesetLabels = pOther->bDrawTrianglesetLabels;
		bDrawTriCenterLabels = pOther->bDrawTriCenterLabels;
		bDrawSegmentsetLabels = pOther->bDrawSegmentsetLabels;

		bDrawClasssetLabels = pOther->bDrawClasssetLabels;

		fPixelset_zoffset = pOther->fPixelset_zoffset;
		fPointset_zoffset = pOther->fPointset_zoffset;
		fTriangleset_zoffset = pOther->fTriangleset_zoffset;
		fTriangleset_zrange = pOther->fTriangleset_zrange;
		fVoronoiset_zoffset = pOther->fVoronoiset_zoffset;
		fSegmentset_zoffset = pOther->fSegmentset_zoffset;

		idstatchannelMappedToTriHeight = pOther->idstatchannelMappedToTriHeight;
	}
	else
	{
		SetDataMembersToDefault();	
	}

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// profile settings implementation file
//
// CProfileSettings property page
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "oifii_app.h"
//#include "profsett.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

IMPLEMENT_DYNCREATE(CProfileSettings, CObject)

CProfileSettings::CProfileSettings()
{
	SetDataMembersToDefault();
}

CProfileSettings::~CProfileSettings()
{
}

void CProfileSettings::SetDataMembersToDefault()
{
	//poirier, sept 2001, begin	
	bImagePixels = TRUE;
	bImageFeatures = FALSE;	
	//poirier, sept 2001, end
	bGlobal = TRUE;
	bLocal = FALSE;
	iNeighborOrder = 1;
	bSegment = FALSE;
	idSegment = 0;
	idchannelStat = 0;
	bAutoScale = TRUE;
}

BOOL CProfileSettings::CopyAllValuesFrom(CProfileSettings* pOther)
{
	if(pOther)
	{
		bImagePixels = pOther->bImagePixels;
		bImageFeatures = pOther->bImageFeatures;
		bGlobal = pOther->bGlobal;
		bLocal = pOther->bLocal;
		iNeighborOrder = pOther->iNeighborOrder;
		bSegment = pOther->bSegment;
		idSegment = pOther->idSegment;
		idchannelStat = pOther->idchannelStat;
		bAutoScale = pOther->bAutoScale;
	}
	else
	{
		SetDataMembersToDefault();
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// histo settings implementation file
//
// CTraceSettings property page
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


//#include "stdafx.h"
//#include "oifii_app.h"
//#include "histosett.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

IMPLEMENT_DYNCREATE(CHistoSettings, CObject)

CHistoSettings::CHistoSettings()
{
	SetDataMembersToDefault();
}

CHistoSettings::~CHistoSettings()
{
}

void CHistoSettings::SetDataMembersToDefault()
{
	//poirier, sept 2001, begin	
	bImagePixels = TRUE;
	bImageFeatures = FALSE;	
	//poirier, sept 2001, end

	bGlobal = TRUE;
	bLocal = FALSE;
	iNeighborOrder = 1;
	bSegment = FALSE;
	idSegment = 0;
	idchannelStat = 0;
	bAutoScale = TRUE;
}

BOOL CHistoSettings::CopyAllValuesFrom(CHistoSettings* pOther)
{
	if(pOther)
	{
		bImagePixels = pOther->bImagePixels;
		bImageFeatures = pOther->bImageFeatures;
		bGlobal = pOther->bGlobal;
		bLocal = pOther->bLocal;
		iNeighborOrder = pOther->iNeighborOrder;
		bSegment = pOther->bSegment;
		idSegment = pOther->idSegment;
		idchannelStat = pOther->idchannelStat;
		bAutoScale = pOther->bAutoScale;
	}
	else
	{
		SetDataMembersToDefault();
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// scatter settins implementation file
//
// CScatterSettings property page
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "oifii_app.h"
//#include "scattersett.h"

//#include "pointset.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/


IMPLEMENT_DYNCREATE(CScatterSettings, CObject)

CScatterSettings::CScatterSettings()
{
	SetDataMembersToDefault();
}

CScatterSettings::~CScatterSettings()
{
}

void CScatterSettings::SetDataMembersToDefault()
{
	//poirier, sept 2001, begin	
	bImagePixels = TRUE;
	bImageFeatures = FALSE;	
	//poirier, sept 2001, end

	bGlobal = TRUE;
	bLocal = FALSE;
	iNeighborOrder = 1;
	bSegment = FALSE;
	idSegment = 0;
	idchannelStat1 = POINTSET_OFFSETSTAT_VOROAREA; //POINTSET_OFFSETSTAT_NEIGHAVERAGEVOROAREA;
	idchannelStat2 = POINTSET_OFFSETSTAT_VORODENSITY; //POINTSET_OFFSETSTAT_NEIGHAVERAGEINTENSITY;
	bAutoScale = TRUE;
}

BOOL CScatterSettings::CopyAllValuesFrom(CScatterSettings* pOther)
{
	if(pOther)
	{
		bGlobal = pOther->bGlobal;
		bLocal = pOther->bLocal;
		iNeighborOrder = pOther->iNeighborOrder;
		bSegment = pOther->bSegment;
		idSegment = pOther->idSegment;
		idchannelStat1 = pOther->idchannelStat1;
		idchannelStat2 = pOther->idchannelStat2;
		bAutoScale = pOther->bAutoScale;
	}
	else
	{
		SetDataMembersToDefault();
	}
	return TRUE;
}





///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

BOOL CClassificationSettings::ReadAllValuesFrom(FILE* pFILE)
{
	fread(&bCTSupervised, sizeof(BOOL), 1, pFILE);
	fread(&bCTNonsupervised, sizeof(BOOL), 1, pFILE);
	return TRUE;
}

BOOL CClassificationSettings::WriteAllValuesTo(FILE* pFILE)
{
	fwrite(&bCTSupervised, sizeof(BOOL), 1, pFILE);
	fwrite(&bCTNonsupervised, sizeof(BOOL), 1, pFILE);
	return TRUE;
}

BOOL CBehaviorSettings::ReadAllValuesFrom(FILE* pFILE)
{
	fread(&bMBHighlightNearestPoint, sizeof(BOOL), 1, pFILE);
	fread(&bMBHighlightNearestNeighbors, sizeof(BOOL), 1, pFILE);
	fread(&bMBHighlightSegment, sizeof(BOOL), 1, pFILE);

	fread(&bMBLocalExtraction, sizeof(BOOL), 1, pFILE);
	fread(&bMBLocalSegmentation, sizeof(BOOL), 1, pFILE);
	fread(&bMBLocalClassification, sizeof(BOOL), 1, pFILE);

	fread(&bIBSelfTraining, sizeof(BOOL), 1, pFILE);
	fread(&bIBSelfExtractionSegmentationValidation, sizeof(BOOL), 1, pFILE);
	return TRUE;
}

BOOL CBehaviorSettings::WriteAllValuesTo(FILE* pFILE)
{
	fwrite(&bMBHighlightNearestPoint, sizeof(BOOL), 1, pFILE);
	fwrite(&bMBHighlightNearestNeighbors, sizeof(BOOL), 1, pFILE);
	fwrite(&bMBHighlightSegment, sizeof(BOOL), 1, pFILE);

	fwrite(&bMBLocalExtraction, sizeof(BOOL), 1, pFILE);
	fwrite(&bMBLocalSegmentation, sizeof(BOOL), 1, pFILE);
	fwrite(&bMBLocalClassification, sizeof(BOOL), 1, pFILE);

	fwrite(&bIBSelfTraining, sizeof(BOOL), 1, pFILE);
	fwrite(&bIBSelfExtractionSegmentationValidation, sizeof(BOOL), 1, pFILE);
	return TRUE;
}

BOOL CExtractionSettings::ReadAllValuesFrom(FILE* pFILE)
{
	fread(&bTCDbyLocalMax, sizeof(BOOL), 1, pFILE);
	fread(&iTCDThreshold, sizeof(int), 1, pFILE);
	fread(&iTCDScale, sizeof(int), 1, pFILE);
	fread(&bTCCArea, sizeof(BOOL), 1, pFILE);
	fread(&bTCCDensity, sizeof(BOOL), 1, pFILE);
	fread(&bTCCHeight, sizeof(BOOL), 1, pFILE);
	fread(&bTCCAverageIntensity, sizeof(BOOL), 1, pFILE);
	fread(&bTCCSpectralVariability, sizeof(BOOL), 1, pFILE);
	fread(&bEZRoads, sizeof(BOOL), 1, pFILE);
	fread(&bEZManMade, sizeof(BOOL), 1, pFILE);
	fread(&bEZForestation, sizeof(BOOL), 1, pFILE);
	fread(&bAllowEZTraining, sizeof(BOOL), 1, pFILE);
	return TRUE;
}

BOOL CExtractionSettings::WriteAllValuesTo(FILE* pFILE)
{
	fwrite(&bTCDbyLocalMax, sizeof(BOOL), 1, pFILE);
	fwrite(&iTCDThreshold, sizeof(int), 1, pFILE);
	fwrite(&iTCDScale, sizeof(int), 1, pFILE);
	fwrite(&bTCCArea, sizeof(BOOL), 1, pFILE);
	fwrite(&bTCCDensity, sizeof(BOOL), 1, pFILE);
	fwrite(&bTCCHeight, sizeof(BOOL), 1, pFILE);
	fwrite(&bTCCAverageIntensity, sizeof(BOOL), 1, pFILE);
	fwrite(&bTCCSpectralVariability, sizeof(BOOL), 1, pFILE);
	fwrite(&bEZRoads, sizeof(BOOL), 1, pFILE);
	fwrite(&bEZManMade, sizeof(BOOL), 1, pFILE);
	fwrite(&bEZForestation, sizeof(BOOL), 1, pFILE);
	fwrite(&bAllowEZTraining, sizeof(BOOL), 1, pFILE);
	return TRUE;
}

BOOL CFusionSettings::ReadAllValuesFrom(FILE* pFILE)
{
	fread(&bSegTypeHSO, sizeof(BOOL), 1, pFILE);
	fread(&bSegTypeHRG, sizeof(BOOL), 1, pFILE);
	fread(&bSegTypeMixedHSOHRG, sizeof(BOOL), 1, pFILE);
	fread(&idstatchannelUsedAsSimilarityCriteria, sizeof(int), 1, pFILE);
	fread(&bUseExclusionThreshold, sizeof(BOOL), 1, pFILE);
	fread(&fExclusionThreshold, sizeof(double), 1, pFILE);
	fread(&bUsePreClassificationInfo, sizeof(BOOL), 1, pFILE);
	fread(&idstatchannelUsedAsPreClassificationInfo, sizeof(int), 1, pFILE);
	return TRUE;
}

BOOL CFusionSettings::WriteAllValuesTo(FILE* pFILE)
{
	fwrite(&bSegTypeHSO, sizeof(BOOL), 1, pFILE);
	fwrite(&bSegTypeHRG, sizeof(BOOL), 1, pFILE);
	fwrite(&bSegTypeMixedHSOHRG, sizeof(BOOL), 1, pFILE);
	fwrite(&idstatchannelUsedAsSimilarityCriteria, sizeof(int), 1, pFILE);
	fwrite(&bUseExclusionThreshold, sizeof(BOOL), 1, pFILE);
	fwrite(&fExclusionThreshold, sizeof(double), 1, pFILE);
	fwrite(&bUsePreClassificationInfo, sizeof(BOOL), 1, pFILE);
	fwrite(&idstatchannelUsedAsPreClassificationInfo, sizeof(int), 1, pFILE);
	return TRUE;
}

BOOL CTraceSettings::ReadAllValuesFrom(FILE* pFILE)
{
	fread(&bTPPixelset, sizeof(BOOL), 1, pFILE);
	fread(&bTPPointset, sizeof(BOOL), 1, pFILE);
	fread(&bTPTriangleset, sizeof(BOOL), 1, pFILE);
	fread(&bTPVoronoiset, sizeof(BOOL), 1, pFILE);
	fread(&bTPSegmentset, sizeof(BOOL), 1, pFILE);

	fread(&bTLAStat1, sizeof(BOOL), 1, pFILE);
	fread(&bTLAStat2, sizeof(BOOL), 1, pFILE);
	fread(&bTLAStat3, sizeof(BOOL), 1, pFILE);
	fread(&bTLAStat4, sizeof(BOOL), 1, pFILE);

	fread(&idchannelTLAStat1, sizeof(int), 1, pFILE);
	fread(&idchannelTLAStat2, sizeof(int), 1, pFILE);
	fread(&idchannelTLAStat3, sizeof(int), 1, pFILE);
	fread(&idchannelTLAStat4, sizeof(int), 1, pFILE);
	return TRUE;
}

BOOL CTraceSettings::WriteAllValuesTo(FILE* pFILE)
{
	fwrite(&bTPPixelset, sizeof(BOOL), 1, pFILE);
	fwrite(&bTPPointset, sizeof(BOOL), 1, pFILE);
	fwrite(&bTPTriangleset, sizeof(BOOL), 1, pFILE);
	fwrite(&bTPVoronoiset, sizeof(BOOL), 1, pFILE);
	fwrite(&bTPSegmentset, sizeof(BOOL), 1, pFILE);

	fwrite(&bTLAStat1, sizeof(BOOL), 1, pFILE);
	fwrite(&bTLAStat2, sizeof(BOOL), 1, pFILE);
	fwrite(&bTLAStat3, sizeof(BOOL), 1, pFILE);
	fwrite(&bTLAStat4, sizeof(BOOL), 1, pFILE);

	fwrite(&idchannelTLAStat1, sizeof(int), 1, pFILE);
	fwrite(&idchannelTLAStat2, sizeof(int), 1, pFILE);
	fwrite(&idchannelTLAStat3, sizeof(int), 1, pFILE);
	fwrite(&idchannelTLAStat4, sizeof(int), 1, pFILE);
	return TRUE;
}




BOOL CTransformationSettings::ReadAllValuesFrom(FILE* pFILE)
{
	fread(&fVTX, sizeof(GLdouble), 1, pFILE);
	fread(&fVTY, sizeof(GLdouble), 1, pFILE);
	fread(&fVTWidth, sizeof(GLdouble), 1, pFILE);
	fread(&fVTHeight, sizeof(GLdouble), 1, pFILE);

	fread(&fMTTranslateX, sizeof(GLdouble), 1, pFILE);
	fread(&fMTTranslateY, sizeof(GLdouble), 1, pFILE);
	fread(&fMTTranslateZ, sizeof(GLdouble), 1, pFILE);
	fread(&fMTScaleX, sizeof(GLdouble), 1, pFILE);
	fread(&fMTScaleY, sizeof(GLdouble), 1, pFILE);
	fread(&fMTScaleZ, sizeof(GLdouble), 1, pFILE);
	fread(&fMTRotateX, sizeof(GLdouble), 1, pFILE);
	fread(&fMTRotateY, sizeof(GLdouble), 1, pFILE);
	fread(&fMTRotateZ, sizeof(GLdouble), 1, pFILE);

	fread(&bMTIgnore, sizeof(BOOL), 1, pFILE);
	fread(&bMTUseIt, sizeof(BOOL), 1, pFILE);
	fread(&bMTAbsolute, sizeof(BOOL), 1, pFILE);
	fread(&bMTRelative, sizeof(BOOL), 1, pFILE);
	return TRUE;
}

BOOL CTransformationSettings::WriteAllValuesTo(FILE* pFILE)
{
	fwrite(&fVTX, sizeof(GLdouble), 1, pFILE);
	fwrite(&fVTY, sizeof(GLdouble), 1, pFILE);
	fwrite(&fVTWidth, sizeof(GLdouble), 1, pFILE);
	fwrite(&fVTHeight, sizeof(GLdouble), 1, pFILE);

	fwrite(&fMTTranslateX, sizeof(GLdouble), 1, pFILE);
	fwrite(&fMTTranslateY, sizeof(GLdouble), 1, pFILE);
	fwrite(&fMTTranslateZ, sizeof(GLdouble), 1, pFILE);
	fwrite(&fMTScaleX, sizeof(GLdouble), 1, pFILE);
	fwrite(&fMTScaleY, sizeof(GLdouble), 1, pFILE);
	fwrite(&fMTScaleZ, sizeof(GLdouble), 1, pFILE);
	fwrite(&fMTRotateX, sizeof(GLdouble), 1, pFILE);
	fwrite(&fMTRotateY, sizeof(GLdouble), 1, pFILE);
	fwrite(&fMTRotateZ, sizeof(GLdouble), 1, pFILE);

	fwrite(&bMTIgnore, sizeof(BOOL), 1, pFILE);
	fwrite(&bMTUseIt, sizeof(BOOL), 1, pFILE);
	fwrite(&bMTAbsolute, sizeof(BOOL), 1, pFILE);
	fwrite(&bMTRelative, sizeof(BOOL), 1, pFILE);
	return TRUE;
}


BOOL CViewDataSettings::ReadAllValuesFrom(FILE* pFILE)
{
	fread(&bDrawPixelset, sizeof(BOOL), 1, pFILE);
	fread(&bDrawPointset, sizeof(BOOL), 1, pFILE);
	fread(&bDrawTriangleset, sizeof(BOOL), 1, pFILE);
	fread(&bDrawVoronoiset, sizeof(BOOL), 1, pFILE);
	fread(&bDrawSegmentset, sizeof(BOOL), 1, pFILE);

	fread(&bDrawPointsetLabels, sizeof(BOOL), 1, pFILE);
	fread(&bDrawTrianglesetLabels, sizeof(BOOL), 1, pFILE);
	fread(&bDrawTriCenterLabels, sizeof(BOOL), 1, pFILE);
	fread(&bDrawSegmentsetLabels, sizeof(BOOL), 1, pFILE);
	fread(&bDrawClasssetLabels, sizeof(BOOL), 1, pFILE);

	fread(&fPixelset_zoffset, sizeof(double), 1, pFILE);
	fread(&fPointset_zoffset, sizeof(double), 1, pFILE);
	fread(&fTriangleset_zoffset, sizeof(double), 1, pFILE);
	fread(&fTriangleset_zrange, sizeof(double), 1, pFILE);
	fread(&fVoronoiset_zoffset, sizeof(double), 1, pFILE);
	fread(&fSegmentset_zoffset, sizeof(double), 1, pFILE);

	fread(&idstatchannelMappedToTriHeight, sizeof(int), 1, pFILE);
	return TRUE;
}

BOOL CViewDataSettings::WriteAllValuesTo(FILE* pFILE)
{
	fwrite(&bDrawPixelset, sizeof(BOOL), 1, pFILE);
	fwrite(&bDrawPointset, sizeof(BOOL), 1, pFILE);
	fwrite(&bDrawTriangleset, sizeof(BOOL), 1, pFILE);
	fwrite(&bDrawVoronoiset, sizeof(BOOL), 1, pFILE);
	fwrite(&bDrawSegmentset, sizeof(BOOL), 1, pFILE);

	fwrite(&bDrawPointsetLabels, sizeof(BOOL), 1, pFILE);
	fwrite(&bDrawTrianglesetLabels, sizeof(BOOL), 1, pFILE);
	fwrite(&bDrawTriCenterLabels, sizeof(BOOL), 1, pFILE);
	fwrite(&bDrawSegmentsetLabels, sizeof(BOOL), 1, pFILE);
	fwrite(&bDrawClasssetLabels, sizeof(BOOL), 1, pFILE);

	fwrite(&fPixelset_zoffset, sizeof(double), 1, pFILE);
	fwrite(&fPointset_zoffset, sizeof(double), 1, pFILE);
	fwrite(&fTriangleset_zoffset, sizeof(double), 1, pFILE);
	fwrite(&fTriangleset_zrange, sizeof(double), 1, pFILE);
	fwrite(&fVoronoiset_zoffset, sizeof(double), 1, pFILE);
	fwrite(&fSegmentset_zoffset, sizeof(double), 1, pFILE);

	fwrite(&idstatchannelMappedToTriHeight, sizeof(int), 1, pFILE);
	return TRUE;
}


BOOL CProjectionSettings::ReadAllValuesFrom(FILE* pFILE)
{
	fread(&fVVSLeft, sizeof(GLdouble), 1, pFILE);
	fread(&fVVSTop, sizeof(GLdouble), 1, pFILE);
	fread(&fVVSRight, sizeof(GLdouble), 1, pFILE);
	fread(&fVVSBottom, sizeof(GLdouble), 1, pFILE);
	fread(&fVVSNear, sizeof(GLdouble), 1, pFILE);
	fread(&fVVSFar, sizeof(GLdouble), 1, pFILE);

	fread(&bVVSPerspective, sizeof(BOOL), 1, pFILE);
	fread(&bVVSOrthogonal, sizeof(BOOL), 1, pFILE);

	fread(&fLAEyeX, sizeof(GLdouble), 1, pFILE);
	fread(&fLAEyeY, sizeof(GLdouble), 1, pFILE);
	fread(&fLAEyeZ, sizeof(GLdouble), 1, pFILE);
	fread(&fLACenterX, sizeof(GLdouble), 1, pFILE);
	fread(&fLACenterY, sizeof(GLdouble), 1, pFILE);
	fread(&fLACenterZ, sizeof(GLdouble), 1, pFILE);
	fread(&fLAUpX, sizeof(GLdouble), 1, pFILE);
	fread(&fLAUpY, sizeof(GLdouble), 1, pFILE);
	fread(&fLAUpZ, sizeof(GLdouble), 1, pFILE);

	fread(&bLAIgnore, sizeof(BOOL), 1, pFILE);
	fread(&bLAUseIt, sizeof(BOOL), 1, pFILE);
	return TRUE;
}

BOOL CProjectionSettings::WriteAllValuesTo(FILE* pFILE)
{
	fwrite(&fVVSLeft, sizeof(GLdouble), 1, pFILE);
	fwrite(&fVVSTop, sizeof(GLdouble), 1, pFILE);
	fwrite(&fVVSRight, sizeof(GLdouble), 1, pFILE);
	fwrite(&fVVSBottom, sizeof(GLdouble), 1, pFILE);
	fwrite(&fVVSNear, sizeof(GLdouble), 1, pFILE);
	fwrite(&fVVSFar, sizeof(GLdouble), 1, pFILE);

	fwrite(&bVVSPerspective, sizeof(BOOL), 1, pFILE);
	fwrite(&bVVSOrthogonal, sizeof(BOOL), 1, pFILE);

	fwrite(&fLAEyeX, sizeof(GLdouble), 1, pFILE);
	fwrite(&fLAEyeY, sizeof(GLdouble), 1, pFILE);
	fwrite(&fLAEyeZ, sizeof(GLdouble), 1, pFILE);
	fwrite(&fLACenterX, sizeof(GLdouble), 1, pFILE);
	fwrite(&fLACenterY, sizeof(GLdouble), 1, pFILE);
	fwrite(&fLACenterZ, sizeof(GLdouble), 1, pFILE);
	fwrite(&fLAUpX, sizeof(GLdouble), 1, pFILE);
	fwrite(&fLAUpY, sizeof(GLdouble), 1, pFILE);
	fwrite(&fLAUpZ, sizeof(GLdouble), 1, pFILE);

	fwrite(&bLAIgnore, sizeof(BOOL), 1, pFILE);
	fwrite(&bLAUseIt, sizeof(BOOL), 1, pFILE);
	return TRUE;
}

BOOL CProfileSettings::ReadAllValuesFrom(FILE* pFILE)
{
	fread(&bImagePixels, sizeof(BOOL), 1, pFILE);
	fread(&bImageFeatures, sizeof(BOOL), 1, pFILE);
	fread(&bGlobal, sizeof(BOOL), 1, pFILE);
	fread(&bLocal, sizeof(BOOL), 1, pFILE);

	fread(&iNeighborOrder, sizeof(int), 1, pFILE);
	fread(&bSegment, sizeof(BOOL), 1, pFILE);
	fread(&idSegment, sizeof(int), 1, pFILE);
	fread(&idchannelStat, sizeof(int), 1, pFILE);
	fread(&bAutoScale, sizeof(BOOL), 1, pFILE);
	return TRUE;
}

BOOL CProfileSettings::WriteAllValuesTo(FILE* pFILE)
{
	fwrite(&bImagePixels, sizeof(BOOL), 1, pFILE);
	fwrite(&bImageFeatures, sizeof(BOOL), 1, pFILE);
	fwrite(&bGlobal, sizeof(BOOL), 1, pFILE);
	fwrite(&bLocal, sizeof(BOOL), 1, pFILE);

	fwrite(&iNeighborOrder, sizeof(int), 1, pFILE);
	fwrite(&bSegment, sizeof(BOOL), 1, pFILE);
	fwrite(&idSegment, sizeof(int), 1, pFILE);
	fwrite(&idchannelStat, sizeof(int), 1, pFILE);
	fwrite(&bAutoScale, sizeof(BOOL), 1, pFILE);
	return TRUE;
}

BOOL CHistoSettings::ReadAllValuesFrom(FILE* pFILE)
{
	fread(&bImagePixels, sizeof(BOOL), 1, pFILE);
	fread(&bImageFeatures, sizeof(BOOL), 1, pFILE);
	fread(&bGlobal, sizeof(BOOL), 1, pFILE);
	fread(&bLocal, sizeof(BOOL), 1, pFILE);

	fread(&iNeighborOrder, sizeof(int), 1, pFILE);
	fread(&bSegment, sizeof(BOOL), 1, pFILE);
	fread(&idSegment, sizeof(int), 1, pFILE);
	fread(&idchannelStat, sizeof(int), 1, pFILE);
	fread(&bAutoScale, sizeof(BOOL), 1, pFILE);
	return TRUE;
}

BOOL CHistoSettings::WriteAllValuesTo(FILE* pFILE)
{
	fwrite(&bImagePixels, sizeof(BOOL), 1, pFILE);
	fwrite(&bImageFeatures, sizeof(BOOL), 1, pFILE);
	fwrite(&bGlobal, sizeof(BOOL), 1, pFILE);
	fwrite(&bLocal, sizeof(BOOL), 1, pFILE);

	fwrite(&iNeighborOrder, sizeof(int), 1, pFILE);
	fwrite(&bSegment, sizeof(BOOL), 1, pFILE);
	fwrite(&idSegment, sizeof(int), 1, pFILE);
	fwrite(&idchannelStat, sizeof(int), 1, pFILE);
	fwrite(&bAutoScale, sizeof(BOOL), 1, pFILE);
	return TRUE;
}

BOOL CScatterSettings::ReadAllValuesFrom(FILE* pFILE)
{
	fread(&bImagePixels, sizeof(BOOL), 1, pFILE);
	fread(&bImageFeatures, sizeof(BOOL), 1, pFILE);
	fread(&bGlobal, sizeof(BOOL), 1, pFILE);
	fread(&bLocal, sizeof(BOOL), 1, pFILE);

	fread(&iNeighborOrder, sizeof(int), 1, pFILE);
	fread(&bSegment, sizeof(BOOL), 1, pFILE);
	fread(&idSegment, sizeof(int), 1, pFILE);
	fread(&idchannelStat1, sizeof(int), 1, pFILE);
	fread(&idchannelStat2, sizeof(int), 1, pFILE);
	fread(&bAutoScale, sizeof(BOOL), 1, pFILE);
	return TRUE;
}

BOOL CScatterSettings::WriteAllValuesTo(FILE* pFILE)
{
	fwrite(&bImagePixels, sizeof(BOOL), 1, pFILE);
	fwrite(&bImageFeatures, sizeof(BOOL), 1, pFILE);
	fwrite(&bGlobal, sizeof(BOOL), 1, pFILE);
	fwrite(&bLocal, sizeof(BOOL), 1, pFILE);

	fwrite(&iNeighborOrder, sizeof(int), 1, pFILE);
	fwrite(&bSegment, sizeof(BOOL), 1, pFILE);
	fwrite(&idSegment, sizeof(int), 1, pFILE);
	fwrite(&idchannelStat1, sizeof(int), 1, pFILE);
	fwrite(&idchannelStat2, sizeof(int), 1, pFILE);
	fwrite(&bAutoScale, sizeof(BOOL), 1, pFILE);
	return TRUE;
}

