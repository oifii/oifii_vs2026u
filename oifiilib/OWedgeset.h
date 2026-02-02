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

#ifndef _OWEDGESET_H
#define _OWEDGESET_H


#define EDGESET_SMALLEST_WORLDRECT_ROI_ID	0
#define EDGESET_LARGEST_WORLDRECT_ROI_ID	1
#define EDGESET_VIEWING_RECT_ROI_ID			2
#define EDGESET_SELECTION_RECT_ROI_ID		3

#define EDGESET_ALL		-1

#define ROISET_ALL		-1

#define ROISET_DRAWAS_NOTSELECTED	0
#define ROISET_DRAWAS_SELECTED		1


///////////////////////////////////////////////////////////////////////////
//class COWGeoPrimitive
//
//this is the 'would be' based class of pixelset, pointset, segmentset and 
//edgeset geometric primitive classes. for edgeset library, we start with
//line and polyline. it will remain to be done for pixelset, pointset and 
//segmentset. more todo. todo. todos es on the way. 
///////////////////////////////////////////////////////////////////////////
class OIFIILIB_API COWGeoPrimitive
{
protected:

public:
	COWGeoPrimitive();
	~COWGeoPrimitive();
	void DefaultInitialization();
};



/////////////////////////////////////////////////////////////////////////////////////////////
//class COWEdgeset
//
//for efficient storage and retreival of vectorial elements (line and polyline primitive)
//each single instance of COWEdgeset can be used as a vectorial COW2View overlay 
/////////////////////////////////////////////////////////////////////////////////////////////

typedef struct OIFIILIB_API tagINTEGERPOINT
{
	int iX;	//integer X
	int iY;	//integer Y
	int iDX;//integer Delta X (for managing uncertainty on X, i.e. screen resolution or rounding)
	int iDY;//integer Delta Y (for managing uncertainty on Y, i.e. screen resolution or rounding)

	int id_info;	//logical id, could be a pointer in memory or database record id
} INTEGERPOINT;

typedef struct OIFIILIB_API tagFLOATPOINT
{
	double fX;	//X
	double fY;	//Y
	double fDX;//Delta X (for managing uncertainty on X, i.e. screen resolution, rounding, successive processing or transform)
	double fDY;//Delta Y (for managing uncertainty on Y, i.e. screen resolution, rounding, successive processing or transform)

	int id_info;	//logical id, could be a pointer in memory or database record id
} FLOATPOINT;

typedef struct OIFIILIB_API tagLINE
{
	int idpointP1;	//line segment starting point id, pointset library point id (physical id)
	int idpointP2;	//line segment ending point id, pointset library point id (physical id)
	int idlineNL;
	double fLineWidth;	//width of this line segment or if zero, radial error on P1 and P2 can be used
	double fLineWeight;	//weight of this line segment, zero for locally or temporarely disjoint

	int id_info;	//logical id, could be a pointer in memory or database record id
} LINE;

typedef struct OIFIILIB_API tagPOLYLINE
{
	int idlineL0;	//starting polyline's line segment id (physical id)
	int idlineLN;	//ending polyline's line segment id (physical id)
	int idpointPN;  //ending point id, this polyline is a closed polygon when idpointPN!=-1 &&  
					//idpointPN==idpointP1[idlineL0] (physical ids)			
	RECT BoundingRect;//bounding rectangle for this polyline/shape
	int idshape;    //shape type id (physical id)

	int id_info;	//logical id, could be a pointer in memory or database record id
} POLYLINE;


#define EDGESET_SHAPETYPE_RECTANGLE		0
#define EDGESET_SHAPETYPE_SQUARE		1
#define EDGESET_SHAPETYPE_LINE			2
#define EDGESET_SHAPETYPE_ELLIPSE		3
#define EDGESET_SHAPETYPE_CIRCLE		4
#define EDGESET_SHAPETYPE_POINT			5
#define EDGESET_SHAPETYPE_TRAPEZE		6
#define EDGESET_SHAPETYPE_TRIANGLE		7
#define EDGESET_SHAPETYPE_POLYGON		8
#define EDGESET_SHAPETYPE_3D_PARALLEBIPEDE	9
#define EDGESET_SHAPETYPE_3D_CUBE		10
#define EDGESET_SHAPETYPE_3D_ELLIPSOID	11
#define EDGESET_SHAPETYPE_3D_SPHERE		12
#define EDGESET_SHAPETYPE_3D_CONE		13
#define EDGESET_SHAPETYPE_3D_PYRAMID	14
#define EDGESET_SHAPETYPE_3D_UNKNOWN	15


class OIFIILIB_API COWEdgeset : public COWSet
{
protected:

	//internal pointset
	BOOL m_bINTEGERPOINT;
	BOOL m_bFLOATPOINT;
	CPtrList m_ListINTEGERPOINT;
	CPtrList m_ListFLOATPOINT;

	//external pointset
	POINTSET* m_pPointset;
	int m_idset_pointset;
	BOOL m_bDual;

	CPtrList m_ListLINE; //list of LINE*
	CPtrList m_ListPOLYLINE; //list of POLYLINE*
		   
public:
	COWEdgeset(); //will alloc its own internal pointset
	COWEdgeset(BOOL bIntegerMode); //will refer to this pointset
	COWEdgeset(class COWPointset* pPointset, BOOL bDual); //will refer to this pointset's dual representation set of points (voronoi edges)
	~COWEdgeset();
	void DefaultInitialization();
	void ClearAll();
	void DeleteAndRemoveAll();


	BOOL ReadAllValuesFrom(FILE* pFILE);
	BOOL WriteAllValuesTo(FILE* pFILE);

	//to add 1 integer point, returns point id
	int AddPoint(	int iX,	//integer X
					int iY,	//integer Y
					int iDX = 0, //integer Delta X (for managing uncertainty on X, i.e. screen resolution or rounding)
					int iDY = 0 ,//integer Delta Y (for managing uncertainty on Y, i.e. screen resolution or rounding)
					int id_info = -1);	//logical id, could be a pointer in memory or database record id

	int AddPoint(INTEGERPOINT* pINTEGERPOINT);	

	//to add 1 floating point, returns point id
	int AddPoint(	double fX,	//integer X
					double fY,	//integer Y
					double fDX = 0.0, //integer Delta X (for managing uncertainty on X, i.e. screen resolution or rounding)
					double fDY = 0.0,//integer Delta Y (for managing uncertainty on Y, i.e. screen resolution or rounding)
					int id_info = -1);	//logical id, could be a pointer in memory or database record id

	int AddPoint(FLOATPOINT* pFLOATPOINT);	

	//to add 1 line segment, returns line segment id 
	int AddLine(int idpointP1, 
				int idpointP2, 
				int idlineNL = -1,
				double fLineWidth = -1.0,
				double fLineWeight = 1.0,	
				int id_info = -1); 

	int AddLine(LINE* pLINE);	

	//to add 1 chain of line segments, returns polyline id
	int AddPolyline(int* idpointP, 
					int nP, 
					int idpointPN=-1, //when -1, last point not connected
					RECT* pBoundingRect=NULL,
					int idshape = EDGESET_SHAPETYPE_LINE,
					int id_info = -1); 

	//to add one polyline  (when point and line segments already exist), return polyline id
	int AddPolyline(int idlineL0, 
					int idlineLN, 
					int idpointPN=-1, //when -1, last point not connected
					RECT* pBoundingRect=NULL,
					int idshape = EDGESET_SHAPETYPE_LINE,
					int id_info = -1); 	

	int AddPolyline(POLYLINE* pPOLYLINE);	

	int GetPolylineNumberOfPoints(POLYLINE* pPOLYLINE);

	int GetPolylinePoints(	POLYLINE* pPOLYLINE, 
							POINT2DD* pPOINT2DD, //allocated by caller
							int npoint_max, //maximum number of POINT structure allocated by caller
							int npoint_offset=0, //=0
							POINT2DD* pDeltaPOINT2DD=NULL); //=NULL)

	int GetPolylinePoints(	POLYLINE* pPOLYLINE, 
							POINT* pPOINT, 
							int npoint_max, 
							int npoint_offset=0, 
							POINT* pDeltaPOINT=NULL);

	BOOL DeletePolyline(int idpolyline);

	int GetNumberOfPolyline();

	INTEGERPOINT* FindIntegerPoint(int idpoint);
	FLOATPOINT* FindFloatPoint(int idpoint);
	LINE* FindLine(int idline);
	POLYLINE* FindPolyline(int idpolyline);

	int TranslatePolyline(int idpolyline, int iX_offset, int iY_offset);
	int TranslatePolyline(int idpolyline, double fX_offset, double fY_offset);

	int AddLineToPolyline(int idpolyline, RECT* pRect, int iDX=0, int iDY=0);

	int IsTouchingPolylineEdge(CPoint pt);
	int IsTouchingPolylineBoundingRect(CPoint pt);

	int EdgeFill(	POINT P1, 
					POINT P2, 
					POINT* pEdgePOINT, //allocated by caller to collect all intermediate points between P1 and P2 (given the grid step)
					int npoint_max, //maximum number of POINT structure allocated by caller
					int iGridStep,
					POINT* pInnerEdgePOINT=NULL,
					POINT* pOutterEdgePOINT=NULL,
					int npoint_ioe_max=0,
					int* p_npoint_ioe_collected=NULL,
					POINT* pInnerEBRPOINT=NULL,
					int npoint_iebr_max=0,
					int* p_npoint_iebr_collected=NULL, //=0,
					POINT* pOutterEBRPOINT=NULL,
					int npoint_oebr_max=0,
					int* p_npoint_oebr_collected=NULL); //=0,

	//basic EdgeFill with iGridStep=1
	int EdgeFill(	POINT P1, 
					POINT P2, 
					POINT* pEdgePOINT, //allocated by caller to collect all intermediate points between P1 and P2 (given the grid step)
					int npoint_max); //maximum number of POINT structure allocated by caller

	int EstimateRectFillMemoryRequirement(	RECT* pRECT,
											int* p_npoint_e_max,
											int* p_npoint_ioe_max,
											int* p_npoint_br_max);

	int RectEdgeFill(	RECT pRECT, 
						POINT* pEdgePOINT, //allocated by caller to collect all intermediate points between P1 and P2 (given the grid step)
						int npoint_max); //maximum number of POINT structure allocated by caller

	int EstimatePolylineFillMemoryRequirement(	POLYLINE* pPOLYLINE,
												int* npoint_e_max,
												int* npoint_ioe_max,
												int* npoint_iebr_max,
												int* npoint_oebr_max);
	int PolylineFill(	POLYLINE* pPOLYLINE, 							
						POINT* pEdgePOINT, //allocated by caller to collect all intermediate points between Pi and Pi+1 segments (given the grid step)
						int npoint_max, //maximum number of POINT structure allocated by caller
						int iGridStep=1,
						POINT* pInnerEdgePOINT=NULL,
						POINT* pOutterEdgePOINT=NULL,
						int npoint_ioe_max=0,
						int* p_npoint_ioe_collected=NULL,
						POINT* pInnerEBRPOINT=NULL,
						int npoint_iebr_max=0,
						int* p_npoint_iebr_collected=NULL,
						POINT* pOutterEBRPOINT=NULL,
						int npoint_oebr_max=0,
						int* p_npoint_oebr_collected=NULL);

	int PolygonFill(POLYLINE* pPOLYLINE, 							
					POINT* pPerimeterPOINT, //allocated by caller to collect all intermediate points between Pi and Pi+1 segments (given the grid step)
					int npoint_per_max, //maximum number of POINT structure allocated by caller
					POINT* pInteriorPOINT,
					int npoint_int_max,
					int iGridStep=1);
};



/////////////////////////////////////////////////////////////////////////////////////////////
//class COWRoiset
//
//region of interest vectorial overlay, user defined with mouse tools, mouse moves and clicks 
/////////////////////////////////////////////////////////////////////////////////////////////



class OIFIILIB_API COWRoiset  : public COWEdgeset
{
protected:
	//single roi selection support for now
	int m_idroi_selected;

	CPtrList* m_pListROI;

public:
	COWRoiset();
	~COWRoiset();
	void DefaultInitialization();
	BOOL ReadAllValuesFrom(FILE* pFILE);
	BOOL WriteAllValuesTo(FILE* pFILE);

	int AddRectangularROI(RECT* pRect);
	int AddRectangularROI(	double fX_left, 
							double fY_top, 
							double fX_right, 
							double fY_bottom);

	int AddPolylinearROI(RECT* pRect, int iDX=1, int iDY=1);
	int AddLineToPolylinearROI(int idroi, RECT* pRect, int iDX=1, int iDY=1);

	BOOL DeleteROI(int idroi);

	int GetNumberOfUserROI(); //user defined
	int GetNumberOfROI(); //total (default + user defined)

	int ModifyRectangularROI(int idpolyline, RECT* pRect);
	int ModifyRectangularROI(	int idroi,
								double fX_left, 
								double fY_top, 
								double fX_right, 
								double fY_bottom);

	int TranslateRectangularROI(int idpolyline, int iX_offset, int iY_offset);
	int TranslateRectangularROI(int idpolyline, double fX_offset, double fY_offset);

	//single roi selection support for now
	int SelectROI(int idroi);
	int SetSelectedROI(int idroi) { return SelectROI(idroi); }
	int GetSelectedROI() { return m_idroi_selected; }

	RECT GetRectROI(int idroi);
	POLYLINE* GetInfoROI(int idroi) { return FindPolyline(idroi); }

	//capture roi
	int IsTouchingROI(CPoint pt);

	//draw roi
	void DrawUserROI(class COW2View* pOW2View, CDC* pDC, int idroi=ROISET_ALL, int iselected=ROISET_DRAWAS_NOTSELECTED);
	void DrawRectangularROI(COW2View* pOW2View, CDC* pDC, POLYLINE* pPOLYLINE, int iselected=ROISET_DRAWAS_NOTSELECTED); 
	void DrawPolylinearROI(COW2View* pOW2View, CDC* pDC, POLYLINE* pPOLYLINE, int iselected=ROISET_DRAWAS_NOTSELECTED); 
	void DrawBoundingRectROI(COW2View* pOW2View, CDC* pDC, POLYLINE* pPOLYLINE, int iselected=ROISET_DRAWAS_NOTSELECTED, int iborders=TRUE);
};


//////////////////////////////////////////////////////////////////////////////////////
//class COWMapobjectset
//
//map objects vectorial overlay, user defined with mouse tools, mouse moves and clicks 
//////////////////////////////////////////////////////////////////////////////////////
class OIFIILIB_API COWMapobjectset  : public COWEdgeset
{
protected:

public:
	COWMapobjectset();
	~COWMapobjectset();
	void DefaultInitialization();
};


//////////////////////////////////////////////////////////////////////////////////////
//class COWImagefeatureset
//
//image features vectorial overlay, computer defined with oiii analysis functionality 
//////////////////////////////////////////////////////////////////////////////////////
class OIFIILIB_API COWImagefeatureset  : public COWEdgeset
{
protected:

public:
	COWImagefeatureset();
	~COWImagefeatureset();
	void DefaultInitialization();
};

//////////////////////////////////////////////////////////////////////////////////////
//class COWFusionset
//
//computer defined with oiii segmentation functionality 
//////////////////////////////////////////////////////////////////////////////////////
class OIFIILIB_API COWFusionset : public COWEdgeset
{
protected:

public:
	COWFusionset();
	~COWFusionset();
	void DefaultInitialization();
};

//////////////////////////////////////////////////////////////////////////////////////
//class COWPartitionset
//
//computer defined with oiii segmentation functionality 
//////////////////////////////////////////////////////////////////////////////////////
class OIFIILIB_API COWPartitionset : public COWEdgeset
{
protected:

public:
	COWPartitionset();
	~COWPartitionset();
	void DefaultInitialization();
};



//spi, nov 2018, begin
/////////////////////////////////////////////////////////////////////////////////////////////
//class COWPolygonset
//
//initially created to temporarily store voronoi polygons 
/////////////////////////////////////////////////////////////////////////////////////////////


const int maxpointsperpolygon = 2048;

class OIFIILIB_API COWPolygonset : public COWEdgeset
{
protected:

	CPtrList* m_pListPolygon;

	POINT p[maxpointsperpolygon]; //vertices
	int v = -1; //number of vertices
	int inter[maxpointsperpolygon], x, y;
	int c, xmin, ymin, xmax, ymax;

	void ints(float z); 
	void sort(int z, class COW2View* pOW2View, class fipWinImage* pfipWinImage, int inverseprob_onpixelfill, //);
	//2020july04, spi, begin
			int voronoisetedgesfilterflag=VORONOISET_COLOR_ALL, int voronoisetpointsfilterflag=VORONOISET_COLOR_ALL, 
			class COW2Doc* pAlternateColorsOW2Doc=NULL, 
			int voronoisetedgesmapperflag=VORONOISET_COLOR_MAP_NONE, int voronoisetpointsmapperflag=VORONOISET_COLOR_MAP_NONE);
	//2020july04, spi, end


public:
	COWPolygonset();
	~COWPolygonset();
	void DefaultInitialization();

	int AddPolygon(POINT* pPOINT, int nCount);

	int GetNumberOfPolygon();

	void DoScanlineFillForPolygon(int idpolygon, class COW2View* pOW2View, class fipWinImage* pfipWinImage, int inverseprob_onpixelfill=1,//);
	//2020july04, spi, begin
		int voronoisetedgesfilterflag = VORONOISET_COLOR_ALL, int voronoisetpointsfilterflag = VORONOISET_COLOR_ALL,
		class COW2Doc* pAlternateColorsOW2Doc = NULL,
		int voronoisetedgesmapperflag = VORONOISET_COLOR_MAP_NONE, int voronoisetpointsmapperflag = VORONOISET_COLOR_MAP_NONE);
	//2020july04, spi, end
	void DoScanlineFillForPolygon(POINT* pPOINT, int nCount, class COW2View* pOW2View, class fipWinImage* pfipWinImage, int inverseprob_onpixelfill = 1, //);
	//2020july04, spi, begin
		int voronoisetedgesfilterflag = VORONOISET_COLOR_ALL, int voronoisetpointsfilterflag = VORONOISET_COLOR_ALL,
		class COW2Doc* pAlternateColorsOW2Doc = NULL,
		int voronoisetedgesmapperflag = VORONOISET_COLOR_MAP_NONE, int voronoisetpointsmapperflag = VORONOISET_COLOR_MAP_NONE);
	//2020july04, spi, end


};

//spi, nov 2018, end
#endif //_OWEDGESET_H
