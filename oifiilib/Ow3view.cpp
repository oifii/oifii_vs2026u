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

// ow3view.cpp : implementation of the COW3View class
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
#include "OW2doc.h"
#include "OWview.h"

#include <gl\gl.h>
#include <gl\glu.h>
#include "ow3view.h"

#include "oifii_propsettings.h"
#include "oifii_propsheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

unsigned char threeto8[8] = 
{
	0, 0111>>1, 0222>>1, 0333>>1, 0444>>1, 0555>>1, 0666>>1, 0377
};

unsigned char twoto8[4] = 
{
	0, 0x55, 0xaa, 0xff
};

unsigned char oneto8[2] = 
{
	0, 255
};

static int defaultOverride[13] = 
{
	0, 3, 24, 27, 64, 67, 88, 173, 181, 236, 247, 164, 91
};

static PALETTEENTRY defaultPalEntry[20] = 
{
	{ 0,   0,   0,    0 },
	{ 0x80,0,   0,    0 },
	{ 0,   0x80,0,    0 },
	{ 0x80,0x80,0,    0 },
	{ 0,   0,   0x80, 0 },
	{ 0x80,0,   0x80, 0 },
	{ 0,   0x80,0x80, 0 },
	{ 0xC0,0xC0,0xC0, 0 },

	{ 192, 220, 192,  0 },
	{ 166, 202, 240,  0 },
	{ 255, 251, 240,  0 },
	{ 160, 160, 164,  0 },

	{ 0x80,0x80,0x80, 0 },
	{ 0xFF,0,   0,    0 },
	{ 0,   0xFF,0,    0 },
	{ 0xFF,0xFF,0,    0 },
	{ 0,   0,   0xFF, 0 },
	{ 0xFF,0,   0xFF, 0 },
	{ 0,   0xFF,0xFF, 0 },
	{ 0xFF,0xFF,0xFF, 0 }
};

/////////////////////////////////////////////////////////////////////////////
// COW3View

IMPLEMENT_DYNCREATE(COW3View, COWView)

BEGIN_MESSAGE_MAP(COW3View, COWView)
	//{{AFX_MSG_MAP(COW3View)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COW3View construction/destruction

COW3View::COW3View()
{
	// TODO: add construction code here
	m_play = FALSE;

	/*
	m_xTranslate = 0.0f;
	m_yTranslate = 0.0f;
	m_zTranslate = -100.0f;
	m_xScale = 1.0f;
	m_yScale = 1.0f;
	m_zScale = 1.0f;
	m_xRotate = 0.0f;
	m_yRotate = 0.0f;
	m_zRotate = 0.0f;
	*/

	m_ix_init = 0;
	m_iy_init = 0;
	m_ix_translation = 0;
	m_iy_translation = 0;

}

COW3View::~COW3View()
{
}

/////////////////////////////////////////////////////////////////////////////
// COW3View drawing

void COW3View::OnDraw(CDC* pDC)
{
	COW2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
    DrawScene();
}

/////////////////////////////////////////////////////////////////////////////
// COW3View diagnostics

#ifdef _DEBUG
void COW3View::AssertValid() const
{
	CView::AssertValid();
}

void COW3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COW2Doc* COW3View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COW2Doc)));
	return (COW2Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COW3View message handlers


int COW3View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COWView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    HWND        hWnd = GetSafeHwnd();
    HDC         hdc = ::GetDC(hWnd);
	HGLRC		hrc;
	float		fMaxObjSize, fAspect;
	float		fNearPlane, fFarPlane;

    bSetupPixelFormat(hdc);
    hrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hrc);

    GetClientRect(&m_oldRect);
    //glClearDepth(1.0f);
	glClearDepth(0.0f);

	/* march 97, does not work with this!!!
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	*/

	fAspect = (GLfloat)m_oldRect.right/m_oldRect.bottom;
	fNearPlane = 3.0f;
	fFarPlane = 7.0f;
	//fMaxObjSize = 3.0f;
	fMaxObjSize = 3.0f;
	m_fRadius = fNearPlane + fMaxObjSize / 2.0f;


	return 0;
}

void COW3View::OnDestroy() 
{
	CView::OnDestroy();
	
    HDC     hdc;
	HGLRC	hrc;

	//fev 97
	//KillTimer(1);

    hdc = ::wglGetCurrentDC();
	hrc = ::wglGetCurrentContext();

    ::wglMakeCurrent(NULL,  NULL);

    if(hrc)
        ::wglDeleteContext(hrc);
    if(hdc)
        ::ReleaseDC(GetSafeHwnd(), hdc);
}


void COW3View::OnInitialUpdate() 
{
	COWView::OnInitialUpdate();
	
	COW2Doc* pDoc = (COW2Doc*) GetDocument();
	ASSERT(pDoc!=NULL);
	/* march 97
	if(pDoc->m_pPixelset!=NULL)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		GLsizei width, height;
		width = pDoc->m_pPixelset->GetWidth();
		height = pDoc->m_pPixelset->GetHeight();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(	GL_TEXTURE_2D, 0, 1, 
						width, height, 0, 
						GL_LUMINANCE, 
						GL_UNSIGNED_BYTE,
						(GLvoid*)pDoc->m_pPixelset->m_pData);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (float)GL_DECAL);
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_FLAT);
	}
	*/

	//initialize view settings from document	
	m_pViewDataSettings->CopyAllValuesFrom(pDoc->m_pViewDataSettings);
	m_pProjectionSettings->CopyAllValuesFrom(pDoc->m_pProjectionSettings);
	m_pTransformationSettings->CopyAllValuesFrom(pDoc->m_pTransformationSettings);	
	return;
}



void COW3View::OnSize(UINT nType, int cx, int cy) 
{

	CView::OnSize(nType, cx, cy);
	
    if(cy > 0)
    {   
		//fev 97
		/*
		HDC     hdc;
		HGLRC	hrc;
		hdc = ::wglGetCurrentDC();
		hrc = ::wglGetCurrentContext();
		wglMakeCurrent(hdc, hrc);
		*/

        if((m_oldRect.right > cx) || (m_oldRect.bottom > cy))
            RedrawWindow();
        m_oldRect.right = cx;
        m_oldRect.bottom = cy;


        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
		if(m_pProjectionSettings->bVVSOrthogonal)
		{
			glOrtho(m_pProjectionSettings->fVVSLeft, 
					m_pProjectionSettings->fVVSRight,
					m_pProjectionSettings->fVVSBottom, 
					m_pProjectionSettings->fVVSTop, 
					m_pProjectionSettings->fVVSNear, 
					m_pProjectionSettings->fVVSFar);
		}
		if(m_pProjectionSettings->bVVSPerspective)
		{
			glFrustum(	m_pProjectionSettings->fVVSLeft, 
						m_pProjectionSettings->fVVSRight,
						m_pProjectionSettings->fVVSBottom, 
						m_pProjectionSettings->fVVSTop, 
						m_pProjectionSettings->fVVSNear, 
						m_pProjectionSettings->fVVSFar);
		}
		if(m_pProjectionSettings->bLAUseIt)
		{
			ASSERT(m_pProjectionSettings->bLAIgnore==FALSE);
			gluLookAt(	m_pProjectionSettings->fLAEyeX, 
						m_pProjectionSettings->fLAEyeY, 
						m_pProjectionSettings->fLAEyeZ, 		
						m_pProjectionSettings->fLACenterX, 
						m_pProjectionSettings->fLACenterY, 
						m_pProjectionSettings->fLACenterZ,
						m_pProjectionSettings->fLAUpX, 
						m_pProjectionSettings->fLAUpY, 
						m_pProjectionSettings->fLAUpZ);

		}

        glViewport(0, 0, cx, cy);
		m_pTransformationSettings->fVTX = 0.0;
		m_pTransformationSettings->fVTY = 0.0;
		m_pTransformationSettings->fVTWidth = (double) cx;
		m_pTransformationSettings->fVTHeight = (double) cy;
		if( ((COIIIApp*)AfxGetApp())->IsSettingsSheetVisible())
		{
			//update info
			CSettingsSheet* pSettingsSheet = ((COIIIApp*)AfxGetApp())->GetSettingsSheet();
			if(pSettingsSheet) pSettingsSheet->PostMessage(OWM_SETTINGSSHEET_UPDATEINFO); //we must add a pointer to the class containing all info

		}

    }
}


BOOL COW3View::PreCreateWindow(CREATESTRUCT& cs) 
{
    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	
	return CView::PreCreateWindow(cs);
}

void COW3View::DrawScene(void)
{
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    static BOOL     bBusy = FALSE;
    static float    wAngleY = 45.0f; 	//10.0f
    static float    wAngleX = 45.0f;	//1.0f
    static float    wAngleZ = 45.0f;	//5.0f

    if(bBusy) return;
    bBusy = TRUE;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

	glTranslated(	m_pTransformationSettings->fMTTranslateX, 
					m_pTransformationSettings->fMTTranslateY, 
					m_pTransformationSettings->fMTTranslateZ);

	glScaled(	m_pTransformationSettings->fMTScaleX, 
				m_pTransformationSettings->fMTScaleY, 
				m_pTransformationSettings->fMTScaleZ);

    glRotated(m_pTransformationSettings->fMTRotateX, 1.0, 0.0, 0.0);
	glRotated(m_pTransformationSettings->fMTRotateY, 0.0, 1.0, 0.0);
    glRotated(m_pTransformationSettings->fMTRotateZ, 0.0, 0.0, 1.0);


	//
	//warning, all polygon vertices must be specified in counter-clockwise order
	//
	/*
   	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.7f, 0.7f, 0.7f);
	glBegin(GL_QUADS); //front
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 400.0f, 300.0f);
        glVertex3f(0.0f, 0.0f, 300.0f);
		glVertex3f(400.0f, 0.0f, 300.0f);
        glVertex3f(400.0f, 400.0f, 300.0f);
	glEnd();
    glBegin(GL_QUADS); //right
		glColor3f(0.0f, 1.0f, 1.0f);
		glVertex3f(400.0f, 400.0f, 300.0f);
		glVertex3f(400.0f, 0.0f, 300.0f);
		glVertex3f(400.0f, 0.0f, -100.0f);
        glVertex3f(400.0f, 400.0f, -100.0f);
    glEnd();
	glBegin(GL_QUADS); //back
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(400.0f, 400.0f, -100.0f);
        glVertex3f(400.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f, -100.0f);
        glVertex3f(0.0f, 400.0f, -100.0f);
	glEnd();
    glBegin(GL_QUADS); //left
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 400.0f, -100.0f);
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f, 300.0f);
        glVertex3f(0.0f, 400.0f, 300.0f);
    glEnd();
    glBegin(GL_QUADS); //bottom
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 300.0f);
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(400.0f, 0.0f, -100.0f);
        glVertex3f(400.0f, 0.0f, 300.0f);
    glEnd();
    glBegin(GL_QUADS); //top
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 400.0f, -100.0f);
		glVertex3f(0.0f, 400.0f, 300.0f);
		glVertex3f(400.0f, 400.0f, 300.0f);
        glVertex3f(400.0f, 400.0f, -100.0f);
    glEnd();
	*/
	
	if(m_pViewDataSettings->bDrawPixelset)
	{
		DrawPixelset();
	}
	
	if(m_pViewDataSettings->bDrawPointset)
	{
		int idstatchannelDimX = POINTSET_EXTRA_NUMBEROFSTAT + POINTSET_XYZI_TREEDIMX;
		int idstatchannelDimY = POINTSET_EXTRA_NUMBEROFSTAT + POINTSET_XYZI_TREEDIMY;
		DrawPointset(idstatchannelDimX, idstatchannelDimY);
	}//end of m_bDrawPoint
	
	if(m_pViewDataSettings->bDrawTriangleset)
	{
		int idstatchannelMappedStat = m_pViewDataSettings->idstatchannelMappedToTriHeight;
		double fRangeForMappedStat = m_pViewDataSettings->fTriangleset_zrange;
		DrawTriangleset(idstatchannelMappedStat, fRangeForMappedStat);
	}
		
	if(m_pViewDataSettings->bDrawVoronoiset)
	{
		DrawVoronoiset();
	}
	
	if(m_pViewDataSettings->bDrawSegmentset)
	{
		DrawSegmentset(m_iNumberOfSegmentForFinalPartition);	
	}
	
    glPopMatrix();

    glFinish();
    SwapBuffers(wglGetCurrentDC());

    bBusy = FALSE;
	return;
}

BOOL COW3View::DrawPixelset()
{
	/*
	if(m_hDDB>0)
	{
		HDC hDC = pDC->GetSafeHdc();
		IMG_display_ddb(m_hDDB, hDC, m_ViewOffset.x, m_ViewOffset.y);
	}
	*/

	/* march 97
	ASSERT(pDoc->m_pPixelset!=NULL);
	float fWidth, fHeight;
	fWidth =float(pDoc->m_pPixelset->GetWidth());
	fHeight =float(pDoc->m_pPixelset->GetHeight());

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, fHeight, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(fWidth, fHeight, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(fWidth, 0.0f, 0.0f);
	glEnd();
	*/
	return TRUE;
}

BOOL COW3View::DrawPointset(int idstatchannelDimX, int idstatchannelDimY)
{
 	COW2Doc* pDoc = (COW2Doc*) GetDocument();
	ASSERT(pDoc!=NULL);
	ASSERT(pDoc->m_pPointset!=NULL);
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

	double dfOffsetZ = m_pViewDataSettings->fPointset_zoffset;

	//blue 
	glColor3f(0.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBegin(GL_QUADS);
		for (i=0; i<pDoc->m_pPointset->GetNumberOfPoints(); i++)
		{   
			//iStatBaseOffset = i*nStatPerPoint+POINTSET_EXTRA_NUMBEROFSTAT;
			iStatBaseOffset = i*nStatPerPoint;
			dfTreePosX = pDoc->m_pPointset->m_pPOINTSET->px[i];
			dfTreePosY = pDoc->m_pPointset->m_pPOINTSET->py[i];
			//dfTreeDimX = pfStatistics[iStatBaseOffset+POINTSET_XYZI_TREEDIMX];
			//dfTreeDimY = pfStatistics[iStatBaseOffset+POINTSET_XYZI_TREEDIMY]; 
			dfTreeDimX = pfStatistics[iStatBaseOffset+idstatchannelDimX];
			dfTreeDimY = pfStatistics[iStatBaseOffset+idstatchannelDimY]; 

			//glVertex3d(dfTreePosX, dfTreePosY, 0.0);
			double dfHalfX = dfTreeDimX/2.0;
			double dfHalfY = dfTreeDimY/2.0;
			//glRectd(dfTreePosX-dfHalfX, dfTreePosY-dfHalfY, dfTreePosX+dfHalfX, dfTreePosY+dfHalfY);		 
		    glVertex3d(dfTreePosX-dfHalfX, dfTreePosY-dfHalfY, dfOffsetZ);
		    glVertex3d(dfTreePosX+dfHalfX, dfTreePosY-dfHalfY, dfOffsetZ);
		    glVertex3d(dfTreePosX+dfHalfX, dfTreePosY+dfHalfY, dfOffsetZ);
		    glVertex3d(dfTreePosX-dfHalfX, dfTreePosY+dfHalfY, dfOffsetZ);
		}
    glEnd();	 
	return TRUE;
}

BOOL COW3View::DrawTriangleset(int idstatchannelMappedStat, double fRangeForMappedStat)
{
 	COW2Doc* pDoc = (COW2Doc*) GetDocument();
	ASSERT(pDoc!=NULL);
	ASSERT(pDoc->m_pPointset!=NULL);
	int i,k, ivertex;

	double* pfStatistics;
	int nStatPerPoint;

  	//green 
	glColor3f(0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	double dfOffsetZ = m_pViewDataSettings->fTriangleset_zoffset;

	double dfMin, dfMax;
	pDoc->GetMinMaxForStatChannel(idstatchannelMappedStat, dfMin, dfMax);
	if(dfMin<0.0 && idstatchannelMappedStat==POINTSET_OFFSETSTAT_VOROAREA)
	{
		dfMin = 0.0;
	}
	double dfScaleFactor;
	if(dfMax==dfMin)
	{
		ASSERT(FALSE);
		dfScaleFactor = 0.0;
	}
	else
	{
		dfScaleFactor = (fRangeForMappedStat)/(dfMax - dfMin);
	}

	pfStatistics = pDoc->GetPointerToStatistics();
	nStatPerPoint = pDoc->GetNumberOfStatPerPoint(); 

	double dfOldZ, dfNewZ;
	glBegin(GL_TRIANGLES);
		for (i=0; i<=pDoc->m_pPointset->GetNumberOfTriangles(); i++)
		{
			//
			//Pour chacun des triangles valides
			//
			if( !pDoc->m_pPointset->InvalidTri(i) )
			{
				
				for(k=0; k<3; k++)
				{
					//obtenir les coordonnees d'un sommet du triangle
				    ivertex = pDoc->m_pPointset->m_pPOINTSET->vt[k][i];
					//obtenir la statistique que l'on veut projetter
					if(dfScaleFactor>0.0)
					{
						int iStatBaseOffset = ivertex*nStatPerPoint;
						dfOldZ = pfStatistics[iStatBaseOffset+idstatchannelMappedStat];
						//to avoid drawing to negative z value (voronoi area, density, voronoi average, voronoi area variance)
						if(dfOldZ<0) dfOldZ = dfOffsetZ;
					}
					else
					{
						//dfNewZ will be equal to dfOffsetZ
						dfOldZ = dfMin;
					}
					dfNewZ = (dfOldZ - dfMin)*dfScaleFactor + dfOffsetZ;
					//dessiner un sommet du triangle
					glVertex3d(pDoc->m_pPointset->m_pPOINTSET->px[ivertex], pDoc->m_pPointset->m_pPOINTSET->py[ivertex], dfNewZ);
				}
			}
		}
	glEnd();
	return TRUE;
}

BOOL COW3View::DrawVoronoiset()
{
 	COW2Doc* pDoc = (COW2Doc*) GetDocument();
	ASSERT(pDoc!=NULL);
	ASSERT(pDoc->m_pPointset!=NULL);
	int ivertex,j;

	int numtrifound, numneighborfound, itriseed;
	int p_arraytri[200];
	int p_arrayneighbor[200];

  	//red 
	glColor3f(1.0f, 0.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	double dfOffsetZ = m_pViewDataSettings->fVoronoiset_zoffset;

	// the triangle seed must be an existing triangle index 
	itriseed = 0;
	for (ivertex=0; ivertex<(pDoc->m_pPointset->GetNumberOfPoints()); ivertex++)
	{
		glBegin(GL_POLYGON);
			//
			//Pour chacun des points qui n'ont aucun triangle adjacents
			//touchant directement la frontiere convexe ...
			//
			if( FindAllValidTriSurroundingVertex( 	pDoc->m_pPointset->m_pPOINTSET,
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
					glVertex3d(pDoc->m_pPointset->m_pPOINTSET->ctx[p_arraytri[j]], pDoc->m_pPointset->m_pPOINTSET->cty[p_arraytri[j]], dfOffsetZ);
				}
			}
			else
			{
			}
		glEnd();
	}
	return TRUE;
}

BOOL COW3View::DrawSegmentset(int iNumberOfSegmentForFinalPartition)
{
	/*
 	COW2Doc* pDoc = (COW2Doc*) GetDocument();
	ASSERT(pDoc!=NULL);
	ASSERT(pDoc->m_pPointset!=NULL);
	ASSERT(pDoc->m_pSegmentset!=NULL);
	if(m_iNumberOfSegmentForFinalPartition<0)
	{
		//default value, initial number of segment
		m_iNumberOfSegmentForFinalPartition = pDoc->m_pSegmentset->iInitialNumberOfSegment;
	}
	int iColor = 0;
	DrawSegmentPolygonsPartition(	pDC,
									pDoc->m_pPointset, 
									pDoc->m_pSegmentset,
									iNumberOfSegmentForFinalPartition,
									iColor);
	*/
	return TRUE;
}

BOOL COW3View::bSetupPixelFormat(HDC hDC)
{
    static PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
        1,                              // version number
        PFD_DRAW_TO_WINDOW |            // support window
          PFD_SUPPORT_OPENGL |          // support OpenGL
          PFD_DOUBLEBUFFER,             // double buffered
        PFD_TYPE_RGBA,                  // RGBA type
        24,                             // 24-bit color depth
        0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // no alpha buffer
        0,                              // shift bit ignored
        0,                              // no accumulation buffer
        0, 0, 0, 0,                     // accum bits ignored
        32,                             // 32-bit z-buffer
        0,                              // no stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main layer
        0,                              // reserved
        0, 0, 0                         // layer masks ignored
    };
    int pixelformat;

    if ( (pixelformat = ChoosePixelFormat(hDC, &pfd)) == 0 )
    {
        MessageBox(L"ChoosePixelFormat failed");
        return FALSE;
    }

    if (SetPixelFormat(hDC, pixelformat, &pfd) == FALSE)
    {
        MessageBox(L"SetPixelFormat failed");
        return FALSE;
    }

    CreateRGBPalette(hDC);

    return TRUE;
}

unsigned char COW3View::ComponentFromIndex(int i, UINT nbits, UINT shift)
{
    unsigned char val;

    val = (unsigned char) (i >> shift);
    switch (nbits) {

    case 1:
        val &= 0x1;
        return oneto8[val];
    case 2:
        val &= 0x3;
        return twoto8[val];
    case 3:
        val &= 0x7;
        return threeto8[val];

    default:
        return 0;
    }
}


void COW3View::CreateRGBPalette(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd;
    LOGPALETTE *pPal;
    int n, i;
    CDC dc;

    n = ::GetPixelFormat(hDC);
    ::DescribePixelFormat(hDC, n, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    if (pfd.dwFlags & PFD_NEED_PALETTE)
    {
        n = 1 << pfd.cColorBits;
        pPal = (PLOGPALETTE)LocalAlloc(LMEM_FIXED, sizeof(LOGPALETTE) +
                n * sizeof(PALETTEENTRY));
        pPal->palVersion = 0x300;
        pPal->palNumEntries = n;
        for (i=0; i<n; i++)
        {
            pPal->palPalEntry[i].peRed =
                    ComponentFromIndex(i, pfd.cRedBits, pfd.cRedShift);
            pPal->palPalEntry[i].peGreen =
                    ComponentFromIndex(i, pfd.cGreenBits, pfd.cGreenShift);
            pPal->palPalEntry[i].peBlue =
                    ComponentFromIndex(i, pfd.cBlueBits, pfd.cBlueShift);
            pPal->palPalEntry[i].peFlags = 0;
        }

        /* fix up the palette to include the default GDI palette */
        if ((pfd.cColorBits == 8)                           &&
            (pfd.cRedBits   == 3) && (pfd.cRedShift   == 0) &&
            (pfd.cGreenBits == 3) && (pfd.cGreenShift == 3) &&
            (pfd.cBlueBits  == 2) && (pfd.cBlueShift  == 6)
           )
        {
			for (i = 1 ; i <= 12 ; i++)
                pPal->palPalEntry[defaultOverride[i]] = defaultPalEntry[i];
        }

        dc.Attach(hDC);

		CPalette	*pOldPal;

        m_cPal.CreatePalette(pPal);
        LocalFree(pPal);
        pOldPal = dc.SelectPalette(&m_cPal, FALSE);
        dc.RealizePalette();
        dc.Detach();
    }
}

BOOL COW3View::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void COW3View::OnMouseMove(UINT nFlags, CPoint point) 
{
	CSettingsSheet* pSettingsSheet = NULL;

	if( (nFlags&MK_LBUTTON)==MK_LBUTTON )
	{
		/*
		m_xRotate = float((point.y-m_iy_init)+m_ix_translation);
		m_yRotate = float((point.x-m_ix_init)+m_iy_translation);
		*/
		m_pTransformationSettings->fMTRotateX = (double) float((point.y-m_iy_init)+m_ix_translation);
		m_pTransformationSettings->fMTRotateY = (double) float((point.x-m_ix_init)+m_iy_translation);
		/*
		if( ((COIIIApp*)AfxGetApp())->IsSettingsSheetVisible())
		{
			//update info
			((COIIIApp*)AfxGetApp())->GetSettingsSheet();
			pSettingsSheet->PostMessage(OWM_SETTINGSSHEET_UPDATEINFO); //we must add a pointer to the class containing all info

		}
		*/
	}
	
	COWView::OnMouseMove(nFlags, point);
}


void COW3View::OnLButtonUp(UINT nFlags, CPoint point) 
{
	/*
	m_ix_translation = int(m_xRotate);
	m_iy_translation = int(m_yRotate);
	*/
	m_ix_translation = int(m_pTransformationSettings->fMTRotateX);
	m_ix_translation = int(m_pTransformationSettings->fMTRotateY);

	COWView::OnLButtonUp(nFlags, point);

	if( ((COIIIApp*)AfxGetApp())->IsSettingsSheetVisible())
	{
		//update info
		CSettingsSheet* pSettingsSheet = ((COIIIApp*)AfxGetApp())->GetSettingsSheet();
		if(pSettingsSheet) pSettingsSheet->PostMessage(OWM_SETTINGSSHEET_UPDATEINFO); //we must add a pointer to the class containing all info

	}
	Invalidate(FALSE);
	return;
}

void COW3View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_ix_init = point.x;
	m_iy_init = point.y;
	
	COWView::OnLButtonDown(nFlags, point);
}

