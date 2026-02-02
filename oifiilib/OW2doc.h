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

//spi, 2020june19 modif, begin
//#include "freeimageplus.h"
//spi, 2020june19 modif, end

/////////////////////////////////////////////////////////////////////////////
// OW2doc.h : interface of the COW2Doc class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _OW2DOC_H
#define _OW2DOC_H



#define OW2TYPE_ENDOFFILE	-1

#define OW2TYPE_OWSET		0
#define OW2TYPE_PIXELSET	1
#define OW2TYPE_POINTSET	2
#define OW2TYPE_SEGMENTSET	3
#define OW2TYPE_CLASSSET	4
#define OW2TYPE_ROISET		5

#define OW2DOC_MAXPATHLENGTH		2048
#define OW2DOC_SAVEPOINTSET_ASXYI	1
//#define OW2DOC_SAVEPOINTSET_ASXYI	0

#define OW2DOC_NEWDOCUMENT_SIZEX	800
#define OW2DOC_NEWDOCUMENT_SIZEY	600



class OIFIILIB_API COW2Doc : public COWDocument
{
public: // create from serialization only
	COW2Doc();
	COW2Doc(HWND hwnd); //spi 2014, new constructor
	void Init();
	DECLARE_DYNCREATE(COW2Doc)

	virtual ~COW2Doc();
	void InitDocument(); //constructor calls it
	void EmptyDocument(); //destructor calls it
	BOOL OnNewDocumentFromAppTempInfo();

// Attributes
public:
	int iTempAverageLineCount;
	float fTempAverageLineColor_R;
	float fTempAverageLineColor_G;
	float fTempAverageLineColor_B;
	int iTempAveragePolygonCount;
	float fTempAveragePolygonColor_R;
	float fTempAveragePolygonColor_G;
	float fTempAveragePolygonColor_B;

	BOOL m_bLocalTextureExtrationStarted;
	double m_fViewInitZoomFactor;

	////////pixelset///////////
	////////pixelset///////////
	////////pixelset///////////
	char m_pszPixelsetPathName[OW2DOC_MAXPATHLENGTH];
	class COWPixelset* m_pPixelset; //image, rectangular raster data, multiple channels 
									//(forest cover loaded from file as input data source
									//or generated as an output from ULooPS method given
									//individual trees forest model supplied)
	long IsFileExtensionOfPixelsetType(const char* pszFileExtention);

	////////pointset///////////
	////////pointset///////////
	////////pointset///////////
//	struct tagPOINTSET* m_pPointset;	//image feature primitives (global set of structural 
	char m_pszPointsetPathName[OW2DOC_MAXPATHLENGTH];
	class COWPointset* m_pPointset;		//texture vectors), extracted from treeprimitivesfinder 
										//OIII automated method.

	////////segmentset/////////
	////////segmentset/////////
	////////segmentset/////////
	char m_pszSegmentsetPathName[OW2DOC_MAXPATHLENGTH];
	struct tagSEGMENTSET* m_pSegmentset;//image feature primitives clusters (global set of  
										//texture vectors), HSO segmentation partitions set 
										//computed by treeshapesinterpreter OIII automated method.
										//poirier, sept 2001, Offworld Technologies R&D improvements.

	////////segmentation results/////////	
	////////segmentation results/////////	
	////////segmentation results/////////										
	char m_pszFusionsetPathName[OW2DOC_MAXPATHLENGTH];
	char m_pszPartitionsetPathName[OW2DOC_MAXPATHLENGTH];
	class COWFusionset* m_pFusionset;		//COWFusionset derived from COWEdgeset,
	class COWPartitionset* m_pPartitionset; //COWPartitionset derived from COWEdgeset,
											//the goal is to provide a data structure that
											//will facilitate visualization of the evolution 
											//going from a given partition to another partition. 

	////////image features set///////////
	////////image features set///////////
	////////image features set///////////
	char m_pszImagefeaturesetPathName[OW2DOC_MAXPATHLENGTH];
	class COWImagefeatureset* m_pImagefeatureset;	//class derived from edgeset
													////automated measures, not editable by user.
													//final partition obtained from segmentation
													//(automated passes) and possibly some
													//optional user cues and pushes of additional
													//information, i.e. providing one or more sets 
													//of clusters defining each a class pattern. 

	/*
	////////edgeset////////////
	////////edgeset////////////
	////////edgeset////////////
	class COWEdgeset* m_pEdgeset;	//may not be used, to be decided

	////////classset///////////
	////////classset///////////
	////////classset///////////
	struct tagSEGMENTSET* m_pClassset; //may not be used, to be decided
	*/

	////////classset///////////
	////////classset///////////
	////////classset///////////
	char m_pszClasssetPathName[OW2DOC_MAXPATHLENGTH];
	class COWClassset* m_pClassset;		//class derived from edgeset
													 

	////////map objects set///////////
	////////map objects set///////////
	////////map objects set///////////
	char m_pszMapobjectsetPathName[OW2DOC_MAXPATHLENGTH];
	class COWMapobjectset* m_pMapobjectset;	//class derived from edgeset
											//user editable, user measures or user provided 
											//local forest inventory sample parsel or annotations.


	////////roi set///////////
	////////roi set///////////
	////////roi set///////////
	char m_pszRoisetPathName[OW2DOC_MAXPATHLENGTH];
	class COWRoiset* m_pROIset;	//class derived from edgeset. regions of interest, user selected areas
	void SelectROI(int idroi);


	//view of interest (to save more user settings)
	class COWView* m_pViewOfInterest;
	double m_VOI_dfZoomFactor;
	RECT  m_VOI_ViewRECT; 
	SIZE  m_VOI_WindowSIZE; 
	POINT m_VOI_ViewOffsetPOINT;  
	RECT  m_VOI_ZoomedViewRECT;

	class COWView* GetViewOfInterest() { return m_pViewOfInterest; }
	void SetViewOfInterest(class COWView* pOWView) { m_pViewOfInterest = pOWView; }

	////////file i/o//////////
	////////file i/o//////////
	////////file i/o//////////
	////////file i/o//////////
	BOOL OpenFileAs2DWorldDocument(LPCTSTR lpszPathName);
	BOOL SaveFileAs2DWorldDocument(LPCTSTR lpszPathName);
	virtual class COWPixelset* OpenFileAsPixelset(LPCTSTR lpszPathName); 
	class COWPointset* OpenFileAsPointset(LPCTSTR lpszPathname);
	struct tagSEGMENTSET* OpenFileAsSegmentset(LPCTSTR lpszPathName);
	class COWClassset* OpenFileAsClassset(LPCTSTR lpszPathname);
	class COWRoiset* OpenFileAsRoiset(LPCTSTR lpszPathname);

	BOOL SaveFileAsPixelset(class COWPixelset* pPixelset, LPCTSTR lpszPathName); 
	BOOL SaveFileAsPointset(class COWPointset* pPointset, LPCTSTR lpszPathname);
	BOOL SaveFileAsSegmentset(struct tagSEGMENTSET* pSegmentset, LPCTSTR lpszPathName);
	BOOL SaveFileAsClassset(class COWClassset* pClassset, LPCTSTR lpszPathname);
	BOOL SaveFileAsRoiset(class COWRoiset* pRoiset, LPCTSTR lpszPathname);
	
	//2020july04, spi, begin
	//spi, avril 2015, begin
	BOOL SaveVoronoiDiagramColoredWithPixelsetAsImagefile(LPCTSTR lpszPathname, class COW2View* pOW2View, float portionofvoronoipolygonstofill = 0.0f, int inverseprob_onpixelfill = 1,
		int ibackgroundfill = 0, int ired = 0, int igreen = 0, int iblue = 0, int imagechannel = 0, class COW2Doc* pBackgroundColorsOW2Doc=NULL,
		/*
			float portionofvoronoipolygonstofill=0.0f, //0.0 for none, 1.0 for all, 0.5 for 50%, etc.
			int inverseprob_onpixelfill=1); //1 for all, 2 for 50%, 3 for 33.33%, 4 for 25%, 5 for 20%, etc.
		*/
		//spi, avril 2015, end
		/*
			int ibackgroundfill=0), //defaults to 0 for solid black background like early vorotrans, 1 for specifying solid color or 2 for selecting specific image channel or using all three RGB
			int ired=0,  //only used if ibackgroundfill is 1, defaults to 0, must be between 0 and 255
			int igreen=0, //only used if ibackgroundfill is 1, defaults to 0, must be between 0 and 255
			int iblue, //only used if ibackgroundfill is 1, defaults to 0, must be between 0 and 255
			int imagechannel=0, //only used if ibackgroundfill is 2, defaults to 0 for all color channels or can be 1,2,3 for specifying red, green and blue channels or can be 4 for specifying gray intensity levels
			class COW2Doc* pBackgroundColorsOW2Doc=NULL, //defaults to NULL for not used, only used if ibackgroundfill is 3 and if contains valid pixelset  
		*/
		int voronoisetedgesfilterflag= VORONOISET_COLOR_ALL, int voronoisetpointsfilterflag= VORONOISET_COLOR_ALL, //these flags default to VORONOISET_COLOR_ALL but can combine values i.e. VORONOISET_COLOR_HUMANSKIN | VORONOISET_COLOR_RED, etc.
		class COW2Doc* pAlternateColorsOW2Doc=NULL, //defaults to NULL so no color mapping applied, if is a valid pointer to an alternate pixelset color mapping will be executed using the mapper flags
		int voronoisetedgesmapperflag= VORONOISET_COLOR_MAP_NONE, int voronoisetpointsmapperflag= VORONOISET_COLOR_MAP_NONE); //these flags default to VORONOISET_COLOR_MAP_NONE but can combine values i.e. VORONOISET_COLOR_HUMANSKIN | VORONOISET_COLOR_GREEN, etc.
	//2020july04, spi, end

	////////settings dlg related//////////////////
	class CExtractionSettings*		m_pExtractionSettings;
	class CFusionSettings*			m_pFusionSettings;
	class CClassificationSettings*	m_pClassificationSettings;
	class CProfileSettings*			m_pProfileSettings;	  //profile
	class CHistoSettings*			m_pHistoSettings; //histo
	class CScatterSettings*			m_pScatterSettings; //scatter graph
	class CViewDataSettings*		m_pViewDataSettings;
	class CProjectionSettings*		m_pProjectionSettings;
	class CTransformationSettings*	m_pTransformationSettings;
	class CTraceSettings*			m_pTraceSettings;
	class CBehaviorSettings*		m_pBehaviorSettings;

private:
	struct _IplImage* temp_pIplImage;
	//spi, 2020june19 modif, begin
	//fipWinImage myfipWinImage; //now, allocation is local again  //free image library utility class to manipulate images
	//spi, 2020june19 modif, end

// Operations
public:
	void TransSpectralFilter(int iIplFilterIndex);

public:
	//BOOL CopyPointset(struct tagPOINTSET* pPointset);
	BOOL CopyPointset(class COWPointset* pPointset);
	BOOL OffsetPointsetRect(double x_offset, double y_offset);
	CRect GetPointsetRect();
	//add points
	int AddPoint(double fX, double fY, int iTINupdateflag=POINTSET_TIN_NOUPDATE, int* p_itriseed=NULL, double fIntensity=-1.0, double fTreeDimX=-1.0, double fTreeDimY=-1.0, double fTreeArea=-1.0);

   	double* GetPointerToStatistics();
	int GetNumberOfStatPerPoint();
	int GetNumberOfPoint();
	int GetSizeX();
	int GetSizeY();

	RECT GetWorldRect();
	RECT GetLargestWorldRect();
	RECT GetSmallestWorldRect();
	RECT GetViewingWorldRect();
	RECT GetSelectionWorldRect();

	BOOL GetMinMaxForStatChannel(int idstatchannel, double& pdfMin, double& pdfMax);
	//BOOL BuildTINAndComputeStatistics(); 
	int GetPixelsetNumberOfChannel();
	int GetPixelsetIntensity(int idpixelsetchannel, int ioffsetx, int ioffsety);
	int GetNearestPointsetObject(double dfX, double dfY, int* p_itriseed=NULL);
	CString GetPointsetObjectStatAsString(int idpointsetobject, int idstatchannel=-1);
	double GetPointsetObjectStatAsDouble(int idpointsetobject, int idstatchannel);
	BOOL GetCircularNeighborhoodProfileAroundVertex(int idvertex, 
													int iorder,
													int idstatchannel,
													int* p_numvertexfound,
													int* p_idvertexlist,
													double* pfStatistics);

	BOOL GetSegmentEdgeProfile( int idvertex, //any vertex in the segment
								int idsegment, //id of the segment
								int iNumberOfSegmentForFinalPartition,
								int iorder,	//size of neighborhood 
								int* p_itriseed,
								int idstatchannel,
								int* p_numedgefound,
								int* p_idinvertexlist,
								int* p_idoutvertexlist,
								double* pdfStatistics);

	BOOL GetFusionSequenceProfile( int idfusionstart,
								   int idfusionend,
								   int iFusionInfoType,
								   double* pdfStatistics);

	BOOL GetMinMaxForStatisticsVector(	double* pfStatistics, 
										int inumstatistics, 
										double* fMin, 
										double* fMax);

	BOOL MapStatisticsVectorIntoHistogram(	double* pfStatistics,
											int inumstatistics,
											int* p_iHistogram,
											int inumhistolevel,
											double* pfMin,
											double* pfMax);

	//i.e. store classified tree local max intensity into POINTSET_OFFSETSTAT_CLASSTREEINTENSITY pointset statistic
	BOOL UpdateAllClassification(int istatoffset, int iNumberOfSegmentForFinalClassification);
	BOOL UpdateClassification(int istatoffset, int idvertex, int iNumberOfSegmentForFinalClassification);

	BOOL CreateDocumentFromAppTempInfo();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COW2Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL CreateSettingsAndInitFromAppDefault();

// Generated message map functions
public:
	//{{AFX_MSG(COW2Doc)
	afx_msg void OnAnalysisTextureextraction();
	afx_msg void OnUpdateAnalysisTextureextraction(CCmdUI* pCmdUI);
	afx_msg void OnAnalysisTexturesegmentation();
	afx_msg void OnUpdateAnalysisTexturesegmentation(CCmdUI* pCmdUI);
	afx_msg void OnViewNew2dview();
	afx_msg void OnViewNew3dview();
	afx_msg void OnViewNewgraphview();
	afx_msg void OnViewNewGraphdataview();
	afx_msg void OnAnalysisGlobalclassification();
	afx_msg void OnUpdateAnalysisGlobalclassification(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewNew2dview(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewNew3dview(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewNewgraphview(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewGraphdataview(CCmdUI* pCmdUI);
	afx_msg void OnTransSpatialRotation();
	afx_msg void OnTransSpatialStretch();
	afx_msg void OnUpdateTransSpatialRotation(CCmdUI* pCmdUI);
	afx_msg void OnTransSpatialConfinedstretch();
	afx_msg void OnUpdateTransSpatialConfinedstretch(CCmdUI* pCmdUI);
	afx_msg void OnTransLaunchscenario();
	afx_msg void OnUpdateTransLaunchscenario(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTransSpatialStretch(CCmdUI* pCmdUI);
	afx_msg void OnTransSpectralInvert();
	afx_msg void OnUpdateTransSpectralInvert(CCmdUI* pCmdUI);
	afx_msg void OnTransTemporalEvolve();
	afx_msg void OnUpdateTransTemporalEvolve(CCmdUI* pCmdUI);
	afx_msg void OnTransformationRepeatinsequence();
	afx_msg void OnUpdateTransformationRepeatinsequence(CCmdUI* pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnTransSpectralFilter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(COW2Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
};

CString GetPathOnly(const char* lpszPathName);
CString GetFilenameOnly(const char* lpszPathName);
CString GetExtensionOnly(const char*lpszPathName);

#endif //_OW2DOC_H
