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

#ifndef _OWPOINTSET_H
#define _OWPOINTSET_H



#define IDTRI		int
#define IDVERTEX	int
#define IDPOINT		int




#define POINTSET_DRAW_LABEL						1		//1
#define POINTSET_DRAW_POINTSETLABEL				2		//2 combined with 1
#define POINTSET_DRAW_CLASSSETLABEL				4		//4 combined with 1

#define POINTSET_DRAW_POINTSET					8		//8
#define POINTSET_DRAW_POINTSET_AS_ELLIPSE		(16+8)		//16 combined with 8
#define POINTSET_DRAW_POINTSET_AS_CROSS			(32+8)		//32 combined with 8
#define POINTSET_DRAW_POINTSET_AS				(64+8)		//64 combined with 8

#define POINTSET_DRAW_TRIANGLESET_LABEL			128
#define POINTSET_DRAW_TRIANGLESET_CENTER_LABEL	256

#define POINTSET_DRAW_TRIANGLESET				512
#define POINTSET_DRAW_TRIANGLESET_AS_3VERTEX	(1024+512)	//1024 + 512
#define POINTSET_DRAW_TRIANGLESET_AS_POLYGON	(2048+512)	//2048 + 512

#define POINTSET_DRAW_VORONOISET_LABEL			4096
#define POINTSET_DRAW_VORONOISET_				(8192+4096)	//8192 + 4096  

#define POINTSET_DRAW_VORONOISET				16384	
#define POINTSET_DRAW_VORONOISET_AS_NVERTEX		(32768+16384)	//32768 + 16384
#define POINTSET_DRAW_VORONOISET_AS_POLYGON		(65536+16384)	//65536 + 16384

//pointset dynamic update flag
#define POINTSET_TIN_NOUPDATE	1
#define POINTSET_TIN_UPDATE		2

#define POINTSET_TIN_NONORMALIZE	1
#define POINTSET_TIN_NORMALIZE		2

#define POINTSET_ADDPOINT_NOXMARGIN			1
#define POINTSET_ADDPOINT_XMARGIN			2
#define POINTSET_ADDPOINT_DONTDISPLAYVORO	4


#define POINTSET_ALL	-1
#define POINTSET_NONE	-2

//2020july04, spi, begin
#define VORONOISET_COLOR_ALL		0
#define VORONOISET_COLOR_HUMANSKIN	1
#define VORONOISET_COLOR_RED		2
#define VORONOISET_COLOR_GREEN		4
#define VORONOISET_COLOR_BLUE		8
#define VORONOISET_COLOR_YELLOW		16
#define VORONOISET_COLOR_ORANGE		32

#define VORONOISET_COLOR_MAP_NONE		0
#define VORONOISET_COLOR_MAP_HUMANSKIN	1
#define VORONOISET_COLOR_MAP_RED		2
#define VORONOISET_COLOR_MAP_GREEN		4
#define VORONOISET_COLOR_MAP_BLUE		8
#define VORONOISET_COLOR_MAP_YELLOW		16
#define VORONOISET_COLOR_MAP_ORANGE		32
//2020july04, spi, end


typedef struct OIFIILIB_API tagPOINT2DD 
{ // pt  

    double x;
    double y;
} POINT2DD;

typedef struct OIFIILIB_API tagRECT2DD 
{ // pt  

    double left;
    double top;
	double right;
	double bottom;
} RECT2DD;

typedef struct OIFIILIB_API tagCUBE2DD
{
	double left;
	double top;
	double right;
	double bottom;
	double neardepth;
	double fardepth;
} CUBE2DD;


//2022jan04, spi, begin
//this is to be used only with audiospi and videospi apps
POINT OIFIILIB_API MapPointFromWorldToView(POINT2DD pt2DD);
POINT2DD OIFIILIB_API MapPointFromViewToWorld(POINT pt);
//2022jan04, spi, end


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//class COWSet
//
//this is the 'would be' based class of pixelset, pointset, segmentset and 
//edgeset classes.
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
class OIFIILIB_API COWSet : public CObject
{
public:
	//id of this set
	int m_idset;
	//type of 2D world set	 
	int m_idOW2type;
	//euclidian bounding cube
	CUBE2DD m_euclidianCUBE2DD;
	//latlong center coordinate (bounding rect center)
	POINT2DD m_latlong_centerPOINT2DD;
	//latlong projection
	CUBE2DD m_latlong_projCUBE2DD;
	//sky model id and basic parameter
	int m_idskymodel;
	CUBE2DD m_skymodelCUBE2DD; //double triplet for sun angle: azimuth, zenith, time
	//ground dem model id
	int m_idgrounddemmodel;
	CUBE2DD m_grounddemmodelCUBE2DD; //double triplet not used
	//cover dem model id
	int m_idcoverdemmodel;
	CUBE2DD m_coverdemmodelCUBE2DD; //double triplet not used

public:
	COWSet();
	~COWSet();
	void DefaultInitialization();
	BOOL ReadAllValuesFrom(FILE* pFile);
	BOOL WriteAllValuesTo(FILE* pFile);
};


//structure EDGEFILLPARAM
class OIFIILIB_API EDGEFILLPARAM
{
public:
	IDTRI* pIDTRI; //allocated by caller to collect all intermediate points between P1 and P2 
	int ntri_max; //maximum number of IDTRI allocated by caller
	int* p_ntrifound;
	int* p_itriseed;
	IDVERTEX* pIDVERTEX; //allocated by caller to collect all intermediate points between P1 and P2 
	int nvertex_max; //maximum number of IDVERTEX allocated by caller
	int* p_nvertexfound;
	IDVERTEX* pIDVERTEX2; //allocated by caller to collect all intermediate points between P1 and P2
	int nvertex_max2; //maximum number of IDVERTEX allocated by caller
	int* p_nvertexfound2;
	IDTRI* pIDTRI_ie;		//idtri inner edge
	int ntri_ie_max;			//idtri inner edge
	int* p_ntrifound_ie;	//idtri inner edge
	IDVERTEX* pIDVERTEX_ie;//idvertex inner edge
	int nvertex_ie_max;		//idvertex inner edge
	int* p_nvertexfound_ie;//idvertex inner edge
	IDTRI* pIDTRI_oe;		//idtri outter edge
	int ntri_oe_max;			//idtri outter edge
	int* p_ntrifound_oe;	//idtri outter edge
	IDVERTEX* pIDVERTEX_oe;//idvertex outter edge
	int nvertex_oe_max;		//idvertex outter edge
	int* p_nvertexfound_oe;//idvertex outter edge
	IDTRI* pIDTRI_iebr;		//idtri inner edge to bounding rect limits
	int ntri_iebr_max;			//idtri inner edge to bounding rect limits
	int* p_ntrifound_iebr;		//idtri inner edge to bounding rect limits
	IDVERTEX* pIDVERTEX_iebr;	//idvertex inner edge to bounding rect limits
	int nvertex_iebr_max;			//idvertex inner edge to bounding rect limits
	int* p_nvertexfound_iebr;	//idvertex inner edge to bounding rect limits
	IDTRI* pIDTRI_oebr;		//idtri outter edge to bounding rect limits
	int ntri_oebr_max;			//idtri outter edge to bounding rect limits
	int* p_ntrifound_oebr;		//idtri outter edge to bounding rect limits
	IDVERTEX* pIDVERTEX_oebr;	//idvertex outter edge to bounding rect limits
	int nvertex_oebr_max;			//idvertex outter edge to bounding rect limits
	int* p_nvertexfound_oebr;	//idvertex outter edge to bounding rect limits

	EDGEFILLPARAM();

	/*
	EDGEFILLPARAM()
	{
		//default -1 are mandatory, must be set by user before using the EdgeFill(), 
		//RectEdgeFill(), PolylineFill() or PolygonFill() COWPoinset member functions.
		pIDTRI = -1;		//allocated by caller to collect all intermediate points between P1 and P2 
		ntri_max = -1;		//maximum number of IDTRI allocated by caller
		p_ntrifound = -1;
		p_itriseed = -1;
		pIDVERTEX = -1;		//allocated by caller to collect all intermediate points between P1 and P2 
		nvertex_max = -1;	//maximum number of IDVERTEX allocated by caller
		p_nvertexfound = -1;
		
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
	*/
};

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//class CPointset
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

//TODO - major oiii architecture change, backup the project in separate directory.
//TODO - remove #include "pointset.h" from the code and try to move all calls/reference
//       to pointset.h data structure and pointset.c code (in the cpp here).
//     - use the new CPointset class to provide c++ interface to pointset.h
//	   - when compile and run stable, move out the c++ layer (CPointset and COWPointset)
//       to their own source file owpointset.h/.cpp

class OIFIILIB_API CPointset : public COWSet
{
	//data members
public:
	int m_maxnumberofelements;
	POINTSET* m_pPOINTSET;
	double m_fDX; //use to store uncertainty on px[] points
	double m_fDY; //use to store uncertainty on py[] points

	CPtrList m_pFloodFill_Pasted_IDTRIList;
	CPtrList m_pFloodFill_Pasted_nIDTRIList;
	CPtrList m_pFloodFill_IDTRIList;
	CPtrList m_pFloodFill_nIDTRIList;
	CPtrList m_pDropped_IDTRIList;
public:
	//creation, allocation and deallocation
	CPointset();
	//CPointset(int maxnumberofelements);
	CPointset(int maxnumberofelements, int numberofstatsperelement);
	CPointset(POINTSET* pPOINTSET)
	{
		m_pPOINTSET = pPOINTSET;
	}
	~CPointset();
	void DefaultInitialization();

	int GetMaxNumberOfElements() 
	{ 
		ASSERT(m_pPOINTSET->maxnumberofelements==m_maxnumberofelements);
		return m_pPOINTSET->maxnumberofelements;
	}
	void MinimalAlloc(int additionalnumberofelements=20);

	class CPointset* NewPointsetCopy() 
	{ 
		return (CPointset*) new CPointset(::NewPointsetCopy(m_pPOINTSET)); 
	}
	int NewPointsetStatistics(int maxnumberofelements, int statperelement)
	{
		return ::NewPointsetStatistics(m_pPOINTSET, maxnumberofelements, statperelement); 
	}
	int ReallocPointset(int new_maxnumberofelements)
	{
		int iresult = ::ReallocPointset(m_pPOINTSET, new_maxnumberofelements);
		if(iresult) m_maxnumberofelements = new_maxnumberofelements;
		return iresult;
	}
	int ReallocPointsetStatistics(int new_maxnumberofelements)
	{
		return ::ReallocPointsetStatistics(m_pPOINTSET, new_maxnumberofelements);
	}

	////////////
	//points
	////////////
public:
	int GetNumberOfPoints() { return m_pPOINTSET->npts; }
	void SetNumberOfPoints(int npoints) { m_pPOINTSET->npts = npoints; }
	int GetSizeX() {return (int)(m_pPOINTSET->xmax - m_pPOINTSET->xmin + 0.5);}
	int GetSizeY() {return (int)(m_pPOINTSET->ymax - m_pPOINTSET->ymin + 0.5);}

	RECT GetBoundingRect() 
	{ 
		RECT myRect;
		myRect.left = (int)(m_pPOINTSET->xmin - 0.5); 
		myRect.top = (int)(m_pPOINTSET->ymin - 0.5);
		myRect.right = (int)(m_pPOINTSET->xmax + 0.5);
		myRect.bottom = (int)(m_pPOINTSET->ymax + 0.5);
		return myRect; 
	}
	RECT2DD GetWorldRect() 
	{ 
		RECT2DD myRECT2DD;
		myRECT2DD.left = m_pPOINTSET->xmin; 
		myRECT2DD.top = m_pPOINTSET->ymin;
		myRECT2DD.right = m_pPOINTSET->xmax;
		myRECT2DD.bottom = m_pPOINTSET->ymax;
		return myRECT2DD; 
	}
	
	//add points
	int AddPointFrame(int nX, int nY, double dfXmin, double dfYmin, double dfXmax, double dfYmax);
	int AddPointSerie(int n, double dfX1, double dfY1, double dfX2, double dfY2, int iMARGINflag=POINTSET_ADDPOINT_NOXMARGIN);
	int AddPoint(double fX, double fY, int iTINupdateflag=POINTSET_TIN_NOUPDATE, int* p_itriseed=NULL, double fIntensity=-1.0, double fTreeDimX=-1.0, double fTreeDimY=-1.0, double fTreeArea=-1.0);
	int MovePoint(int idpoint, double fX, double fY);
	BOOL IsSiteAvailable(double fX, double fY);

	//move points
	void TranslatePointset(double x_offset, double y_offset)
	{
		::TranslatePointset(m_pPOINTSET, x_offset, y_offset);
		return;
	}

	////////////
	//statistics
	////////////
	double* GetPointerToStatistics() { return m_pPOINTSET->pfStatistics; }
	int GetNumberOfStatPerPoint() { return m_pPOINTSET->nStatPerPoint; }

	double* GetPointerToStatistics(int ielement)
	{
		return GetPointsetPointerToStatistics(m_pPOINTSET, ielement);
	}

	GLOBALSTATISTICSINFO* GetPointerToGlobalStatisticsInfo() 
	{ 
		return m_pPOINTSET->pGlobalStatisticsInfo; 
	}

	double GetStatisticsValue(int idvertex, int idstatchannel)
	{
		double* pDOUBLE = GetPointsetPointerToStatistics(m_pPOINTSET, idvertex);
		return pDOUBLE[idstatchannel];
	}
	BOOL SetStatisticsValue(int idvertex, int idstatchannel, double fValue)
	{
		double* pDOUBLE = GetPointsetPointerToStatistics(m_pPOINTSET, idvertex);
		pDOUBLE[idstatchannel] = fValue;
		return TRUE;
	}

	//////////
	//file i/o
	//////////
	BOOL OpenFileAsXYI(const char* lpszPathName);
	BOOL SaveFileAsXYI(const char* lpszPathName);
	BOOL OpenFileAsXYZ(const char* lpszPathName);
	BOOL ReadAllValuesFrom(FILE* pFile);
	BOOL WriteAllValuesTo(FILE* pFile);

	///////////////
	//build TIN
	///////////////
	BOOL BuildTINAndComputeStatistics(int iNORMALIZEflag=POINTSET_TIN_NORMALIZE);
	int GetNumberOfTriangles() { return m_pPOINTSET->ntri; }
	int InvalidTri(int idtri) { return ::InvalidTri(m_pPOINTSET, idtri); }


	///////////////
	//draw
	///////////////
	//2022jan04, spi, begin
	/*
	BOOL DrawPointset(class COW2View* pOW2View,
		CDC* pDC,
		int idpoint = POINTSET_ALL,
		int idrawingflag = POINTSET_DRAW_POINTSET_AS_ELLIPSE);
	*/
	BOOL DrawPointset(	class COW2View* pOW2View, 
						class CDC* pDC, 
						int idpoint=POINTSET_ALL, 
						int idrawingflag=POINTSET_DRAW_POINTSET_AS_ELLIPSE,
						HDC hdc=NULL); //non-null to be used by audiospi and videospi apps when pOW2View and pDC are null

	BOOL DrawPointset(	class COW2View* pOW2View, 
						class CDC* pDC, 
						class CRect* pViewRect, //CRect* pViewRect=NULL, 
						int idrawingflag=POINTSET_DRAW_POINTSET_AS_ELLIPSE);

	/*
	void DrawPointsetObject(class COW2View* pOW2View, 
							class CDC* pDC, 
							int idpointsetobject);
	*/
	void DrawPointsetObject(class COW2View* pOW2View,
							class CDC* pDC,
							int idpointsetobject); //,
							//HDC hdc=NULL); //todo: XOR drawing directly to hdc, like spitext.exe does, when used by audiospi and videospi apps hdc should be non-null but pOW2View and pDC should be null)

	/*
	BOOL DrawTriangleset(	class COW2View* pOW2View, 
							CDC* pDC, 
							int idtri=POINTSET_ALL, 
							int idrawingflag = POINTSET_DRAW_TRIANGLESET_AS_3VERTEX);
	*/
	BOOL DrawTriangleset(class COW2View* pOW2View,
							class CDC* pDC,
							int idtri=POINTSET_ALL,
							int idrawingflag=POINTSET_DRAW_TRIANGLESET_AS_3VERTEX,
							HDC hdc=NULL); //when used by audiospi and videospi apps hdc should be non-null but pOW2View and pDC should be null

	/*
	BOOL DrawTriangleset(	class COW2View* pOW2View, 
							class CDC* pDC,  
							RECT* pViewRECT=NULL,
							int idrawingflag = POINTSET_DRAW_TRIANGLESET_AS_3VERTEX);
	*/
	BOOL DrawTriangleset(class COW2View* pOW2View,
							class CDC* pDC,
							RECT* pViewRECT=NULL,
							int idrawingflag=POINTSET_DRAW_TRIANGLESET_AS_3VERTEX,
							HDC hdc=NULL, //when used by audiospi and videospi apps hdc should be non-null but pOW2View and pDC should be null
							int* p_itriseed=NULL); //used when pOW2View is NULL and hdc is used instead 
	/*
	BOOL DrawTriangleset(	class COW2View* pOW2View, 
							CDC* pDC,  
							IDTRI* pIDTRI,
							int ntrifound,
							int idrawingflag = POINTSET_DRAW_TRIANGLESET_AS_3VERTEX);
	*/
	BOOL DrawTriangleset(class COW2View* pOW2View,
							class CDC* pDC,
							IDTRI* pIDTRI,
							int ntrifound,
							int idrawingflag=POINTSET_DRAW_TRIANGLESET_AS_3VERTEX,
							HDC hdc=NULL); //when used by audiospi and videospi apps hdc should be non-null but pOW2View and pDC should be null)
	//2022jan04, spi, end
	BOOL DrawVoronoiset(class COW2View* pOW2View,
						class CDC* pDC,
						int idpoint = POINTSET_ALL,
						CRect* pViewRect = NULL,
						int idrawingflag = POINTSET_DRAW_VORONOISET_AS_NVERTEX,
						class fipWinImage* pfipWinImage = NULL,
						float portionofvoronoipolygonstofill = 0.0f, //0.0 for none, 1.0 for all, 0.5 for 50%, etc.
						int inverseprob_onpixelfill = 1, //); //1 for all, 2 for 50%, 3 for 33.33%, 4 for 25%, 5 for 20%, etc.
						//2020july04, spi, begin
						int voronoisetedgesfilterflag=VORONOISET_COLOR_ALL, int voronoisetpointsfilterflag=VORONOISET_COLOR_ALL,
						class COW2Doc* pAlternateColorsOW2Doc=NULL, int voronoisetedgesmapperflag=VORONOISET_COLOR_MAP_NONE, int voronoisetpointsmapperflag=VORONOISET_COLOR_MAP_NONE);
						//2020july04, spi, end

	
	//////////
	//search
	//////////

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
	int EdgeFill(	struct tagPOINT2DD P1, 
					struct tagPOINT2DD P2, 
					IDTRI* pIDTRI, //allocated by caller to collect all intermediate points between P1 and P2 
					int ntri_max, //maximum number of IDTRI allocated by caller
					int* p_ntrifound,
					int* p_itriseed,
					IDVERTEX* pIDVERTEX, //allocated by caller to collect all intermediate points between P1 and P2 
					int nvertex_max, //maximum number of IDVERTEX allocated by caller
					int* p_nvertexfound,
					IDVERTEX* pIDVERTEX2, //allocated by caller to collect all intermediate points between P1 and P2 
					int nvertex_max2, //maximum number of IDVERTEX allocated by caller
					int* p_nvertexfound2);

	//collect all triangles along an edge (a line segment) defined by between P1 and P2 
	//note: when pIDVERTEX2 is NULL, pIDVERTEX will contain all idvertex of the collected
	//      edge triangle pIDTRI. when pIDVERTEX2 is not NULL, idvertex are sorted between 
	//      pIDVERTEX and pIDVERTEX2 according to which (P1,P2) edge side each idvertex lies
	//      (inner side in pIDVERTEX and outter side in pIDVERTEX2, with inner and outter
	//      being defined as follow: outter side is above the line segment (P1,P2) going
	//      from P1 towards P2 and inner is below going from P2 towards P1). see owedgeset.h
	//      for inner and outter definition. 
	int EdgeFill(	struct tagPOINT2DD P1, 
					struct tagPOINT2DD P2, 
					class EDGEFILLPARAM* pEDGEFILLPARAM);	//idvertex outter edge to bounding rect limits

	int InnerEdgeFill(	IDTRI* pIDTRI_e, 
						int nIDTRI_e, 
						IDTRI* pIDTRI_ie, 
						int ntri_ie_max,
						int* p_ntrifound_ie,
						IDVERTEX* pIDVERTEX_ie=NULL,
						int nvertex_ie_max=0,
						int* p_nvertexfound_ie=NULL,
						struct tagRECT2DD* pRECT2DD=NULL);	

	int OutterEdgeFill(	IDTRI* pIDTRI_e, 
						int nIDTRI_e, 
						IDTRI* pIDTRI_oe, 
						int ntri_oe_max,
						int* p_ntrifound_oe,
						IDVERTEX* pIDVERTEX_oe=NULL,
						int nvertex_oe_max=NULL,
						int* p_nvertexfound_oe=NULL,
						struct tagRECT2DD* pRECT2DD=NULL);	

	int IEBRFill(	IDTRI* pIDTRI_e, 
					int nIDTRI_e,
					struct tagRECT2DD* pRECT2DD,
					IDTRI* pIDTRI_iebr, 
					int ntri_iebr_max,
					int* p_ntrifound_iebr,
					IDVERTEX* pIDVERTEX_iebr=NULL,
					int nvertex_iebr_max=0,
					int* p_nvertexfound_iebr=NULL);	

	int OEBRFill(	IDTRI* pIDTRI_e, 
					int nIDTRI_e,
					struct tagRECT2DD* pRECT2DD,
					IDTRI* pIDTRI_oebr, 
					int ntri_oebr_max,
					int* p_ntrifound_oebr,
					IDVERTEX* pIDVERTEX_oebr=NULL,
					int nvertex_oebr_max=0,
					int* p_nvertexfound_oebr=NULL);	

	int RectEdgeFill(	struct tagRECT2DD* pRECT2DD, 
						class EDGEFILLPARAM* pEDGEFILLPARAM);

	int PolylineFill(	class COWEdgeset* pEdgeset,
						struct tagPOLYLINE* pPOLYLINE, 
						class EDGEFILLPARAM* pEDGEFILLPARAM,
						class COW2View* pOW2View=NULL);

	int PolylineFill(	struct tagPOINT* pPolylinePOINT,
						int npoints_polyline,
						class EDGEFILLPARAM* pEDGEFILLPARAM);

	int PolylineFill(	struct tagPOINT2DD* pPolylinePOINT2DD,
						int npoints_polyline,
						class EDGEFILLPARAM* pEDGEFILLPARAM);

	int RectFill(	struct tagRECT2DD* pRECT2DD, 
					class EDGEFILLPARAM* pEDGEFILLPARAM);

int PolygonFill(class COWEdgeset* pEdgeset,
					struct tagPOLYLINE* pPOLYLINE, 
					class EDGEFILLPARAM* pEDGEFILLPARAM);

	int PointFill(	struct tagPOINT2DD P1, 
					class EDGEFILLPARAM* pEDGEFILLPARAM);

	//used by EdgeFill()
private:
	int GetThirdAdjTri(int idtri, int idtri1, int idtri2);

	int GetThirdVertex(int idtri, int idvertex1, int idvertex2);

	int GetInnerAndOutterEdge_TAV(	int idtri_e, int idtri_e_next, 
									int* pIDVERTEX_i, int* pIDVERTEX_o,
									int* pIDTRI_i, int* pIDTRI_o,
									int* prev_pIDVERTEX_i=NULL, int* prev_pIDVERTEX_o=NULL);
public:
	void NormalizeRect(struct tagRECT2DD* pRECT2DD);

	BOOL PointInRect(struct tagPOINT2DD* pPOINT2DD, struct tagRECT2DD* pRECT2DD);
	BOOL PointInRect(double fX, double fY, struct tagRECT2DD* pRECT2DD);
	BOOL PointInRect(int idvertex, struct tagRECT2DD* pRECT2DD);

	BOOL TriInRect(int idtri, struct tagRECT2DD* pRECT2DD);

private:
	void RenderToLog_T(	IDTRI* pIDTRI, 
						int nIDTRI, 
						const char* pszHeader=NULL, 
						const char* pszFooter=NULL);

	void RenderToLog_Edge_TAV(	struct tagPOINT2DD P1, 
								struct tagPOINT2DD P2, 
								class EDGEFILLPARAM* pEDGEFILLPARAM);

	void RenderToLog_InnerEdge_TAV(	struct tagPOINT2DD P1, 
									struct tagPOINT2DD P2, 
									class EDGEFILLPARAM* pEDGEFILLPARAM);

	void RenderToLog_OutterEdge_TAV(struct tagPOINT2DD P1, 
									struct tagPOINT2DD P2, 
									class EDGEFILLPARAM* pEDGEFILLPARAM);

	void RenderToLog_IEBR_TAV(	struct tagPOINT2DD P1, 
								struct tagPOINT2DD P2, 
								class EDGEFILLPARAM* pEDGEFILLPARAM);

	void RenderToLog_OEBR_TAV(	struct tagPOINT2DD P1, 
								struct tagPOINT2DD P2, 
								class EDGEFILLPARAM* pEDGEFILLPARAM);
public:
	struct tagRECT2DD GetTriangleBoundingRect(int idtri, int nei=0);
	RECT GetTriangleBoundingRect(int idtri, COW2View* pOW2View, int nei=0);
	struct tagPOINT2DD GetTriangleGeoCenter(int idtri);
	POINT GetTriangleGeoCenter(int idtri, COW2View* pOW2View);
	void GetTrianglePOINTS(int idtri, struct tagPOINT2DD* pPOINT2DD);
	void GetTrianglePOINTS(int idtri, POINT* pPOINT, COW2View* pOW2View);
};








///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//class COWPointset
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
class OIFIILIB_API COWPointset  : public CPointset
{
public:
	COWPointset();
	//COWPointset(int maxnumberofelements);
	COWPointset(int maxnumberofelements, int numberofstatsperelement);
	COWPointset(POINTSET* pPOINTSET);
	COWPointset(class COWPointset* pPointset);
	class COWPointset* NewPointsetCopy(); 
	~COWPointset();
	void DefaultInitialization();

	BOOL ReadAllValuesFrom(FILE* pFile);
	BOOL WriteAllValuesTo(FILE* pFile);
};


#endif //_OWPOINTSET_H