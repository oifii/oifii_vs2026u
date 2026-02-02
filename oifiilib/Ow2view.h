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

// OW2view.h : interface of the COW2View class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _OW2VIEW_H
#define _OW2VIEW_H



#define OW2VIEW_USE_APP_TEMPINFO		0
#define OW2VIEW_IGNORE_APP_TEMPINFO		1

#define OW2VIEW_PENSIZE					0
#define OW2VIEW_PENSIZE_PRINTING		1
#define OW2VIEW_POINTSET_COLOR			RGB(0, 0, 255)
#define OW2VIEW_TRIANGLESET_COLOR		RGB(0, 255, 0)
#define OW2VIEW_TRIANGLESET_COLOR_DARK	RGB(0, 200, 0)
#define OW2VIEW_VORONOISET_COLOR		RGB(255, 0, 0)
#define OW2VIEW_SEGMENTSET_COLOR		RGB(255, 0, 0)  //RGB(255, 255, 0) yellow


class OIFIILIB_API COW2View : public COWView
{
/* //spi 2014
protected: // create from serialization only
*/
public: // create from serialization only
	COW2View();
	COW2View(HWND hwnd, class COWDocument* pCOWDoc); //spi 2014
	void Init(); //spi 2014
	DECLARE_DYNCREATE(COW2View)

// Attributes
public:
	BOOL m_bMode_ROIFloating;
	BOOL m_bMode_ROIRect;
	BOOL m_bMode_ROIPolyline;
	int m_idroiROIPolylineDrag;
	BOOL m_bMode_ROIFreehand;
	int m_idroiROIFreehandDrag;

	BOOL IsMOUSENearestNeighborsModeEnabled();
	BOOL m_bOnMouseMoveLocalExtractionON;
	void LocalTextureExtraction(CRect selectionrect);
	/*
	void EraseSelectionRectAndDisableROIRectMode(CWnd* pCalledFromWnd=NULL);
	*/
	void EnableMOUSENearestNeighborsMode();
	void DisableMOUSENearestNeighborsMode();
	void ROIFloatingMode_EraseSelectionRect();
	void EnableROIFloatingMode();
	void DisableROIFloatingMode();
	WORD m_wROIFloatingModeToggledBack;

	void EnableROIRectMode();
	void DisableROIRectMode();
	void EnableROIPolylineMode();
	void DisableROIPolylineMode();
	void EnableROIFreehandMode();
	void DisableROIFreehandMode();


	//poirier, begin, selection rect, imported and adapted from OISS
private:
	CPoint m_prev_logPoint;
	CRect m_PrevSelectionRect; //to store selection rect once it is drawn, so it can be xor erased
	int m_SelectionRectWidth; //from mouse location x,y, selection width and height fully define a given selection rectangle 
	int m_SelectionRectHeight; //from mouse location x,y, selection width and height fully define a given selection rectangle 
	int m_SelectionRectWidthMin;
	int m_SelectionRectWidthMax;
	int m_SelectionRectHeightMin;
	int m_SelectionRectHeightMax;
	CPoint m_TempPoint;
	WORD m_wInflatingTimerOn;
	WORD m_nInflatingTimerID;
	WORD m_wDeflatingTimerOn;
	WORD m_nDeflatingTimerID;
	void InflateSelectionRect();
	void DeflateSelectionRect();
	WORD m_wDelayingOpenSettingsDlgTimerOn;
	WORD m_nDelayingOpenSettingsDlgTimerID;
	void OpenSettingsDlg();

	CBitmap m_savedBitmap;
	BOOL DrawSelectionRect(CDC* pDC, CRect* lprc);
	BOOL EraseSelectionRect(CDC* pDC, CRect* lprc);
	int m_selection_rect_thickness_inpixel;
	//poirier, end, selection rect, imported and adapted from OISS

public:
	//poirier, sept 2001, begin
	WORD m_wTransSpectralFilterTimerOn;
	WORD m_nTransSpectralFilterTimerID;
	int m_nTransSpectralFilterIndex;

	/* now in the doc
	COWRoiset* m_pROIset;
	*/
private:
	void InitialUpdateEDGESET_ROI();
	void UpdateROI(int idroi);
	void SelectROI(int idroi);
	void DeleteROI(int idroi);
public:
	void InvalidateROI(int idroi, BOOL bEraseBackGround=FALSE); 

public:
	COW2View* m_pZoomedView;
	//poirier, sept 2001, end
public:

	//CDC  m_MemDC;
	BOOL m_bRendered;
	BOOL m_bBusyPainting;

	HBITMAP m_hDDB;
	int m_itriseed;
	int m_idPointsetObject;
	int m_numneighborfound;
	int m_pArrayNeighbor[200];
	int m_idPrevObject;
	int m_idStartSegment;
	//int m_indexSegmentHistoryList;
	//int* m_pSegmentHistoryList;
	int m_indexHistoryList;
	int m_pidsegHistoryList[1000];
	int m_piNumSegForFinalPartitionList[1000];
	int m_inumberofcollectedsegment;
	int m_imaxhistorylistsize;
	int m_idHistorySegment;
	int m_idHistorySegmentPartition;

	int m_idPrevPointsetObjectUsedWithLocalGraph;

	BOOL m_bShiftKeyIsDown;
// Operations
public:
	//2012july22, poirier, begin
	BITMAP GetDDB(HBITMAP hDDB);
	void DisplayDDB(HBITMAP hDDB, HDC hDC, long ix_offset, long iy_offset);
	HBITMAP DIBtoDDBitmap(BYTE* pDIB);
	DWORD GetPaletteSize(BITMAPINFOHEADER* pBMIH);
	DWORD CalcNumColors(BITMAPINFOHEADER* pBMIH);
	//2012july22, poirier, end
	class COW2Doc* GetDocument();
	POINT MapPointFromWorldToView(POINT2DD myPoint2DD);
	POINT2DD MapPointFromViewToWorld(POINT myPoint);
	RECT MapRectFromWorldToView(RECT2DD myRECT2DD);
	RECT2DD MapRectFromViewToWorld(RECT myRECT);

	HANDLE CreateDIBFromRender2D();
	void Render2D(class COW2Doc* pDoc, CDC* pDC);
	void DrawSegmentPolygonsPartition(	CDC* pDC,
										struct tagPOINTSET* pPointset, 
										struct tagSEGMENTSET* pSegmentSet,
										int iNumberOfSegmentForFinalPartition,
										int iDrawMode=-1);
	void DrawSegmentPolygon(CDC* pDC,
							struct tagPOINTSET* pPointset,
							struct tagSEGMENTSET* pSegmentset,
							int idsegment, //to be drawn
							int idvertex,	 //any one vertex inside the segment to be drawn
							int iNumberOfSegmentForFinalPartition,
							BOOL bSelectPen=TRUE,
							int iDrawMode=-1);

	void UpdatePartition(int iPrevNumSeg, int iNumberOfSegmentForFinalPartition);

	void DrawInterSegmentFrontier(	CDC* pDC,
									struct tagPOINTSET* pPointset,
									struct tagSEGMENTSET* pSegmentset,
									int idsegment1, //frontier is between idsegment1 and 2
									int idsegment2, 
									int iNumberOfSegmentForFinalPartition,
									BOOL bSelectPen/*=TRUE*/,
									int iDrawMode/*=-1*/);

	void StartDrawingInterSegmentFrontierEdge(	CDC* pDC,
												struct tagPOINTSET* pPointset,
												struct tagSEGMENTSET* pSegmentset,
												int idsegment1, //frontier is between idsegment1 and 2
												int idsegment2,
												int iNumberOfSegmentForFinalPartition,
												struct tagEDGE* p_myEdge, 
												struct tagEDGE StartSearchEdge);

	void ComboSetText(LPCTSTR);
	virtual double GetZoomFactor();	
    void ZoomIn(RECT* pRECT);
    void ZoomIn(CRect &rect);
    void ZoomIn(CPoint *point = NULL, double delta = 1.25);

    void MyChangeStatusBar(int,LPCTSTR);

    void DrawRubberRect(CDC* pDC, CRect &rect);
    void DrawRubberEllipse(CDC* pDC, CRect &rect);
	void DrawRubberLine(CDC* pDC, CRect &rect);
	void DrawRubberFillRect(CDC* pDC, CRect &rect);

	void DrawTransparentMask(CDC* pDC, CRect &rect);

	virtual void ZoomFactorChanged(void);

	int LookUpKeyInIntList(int* pIntList, int isizelist, int iKey);
	void HandlePopupMenu(CPoint point);
	void DPtoLP(CClientDC* pDC, CPoint* pPoint);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COW2View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnInitialUpdate();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COW2View();
	void  EmptyView();

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COW2View)
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct); //spi 2014
protected:
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnZoomPercent();
	afx_msg void OnUpdateZoomPercent(CCmdUI* pCmdUI);
	afx_msg void OnZoomPointin();
	afx_msg void OnUpdateZoomPointin(CCmdUI* pCmdUI);
	afx_msg void OnZoomPointout();
	afx_msg void OnUpdateZoomPointout(CCmdUI* pCmdUI);
	afx_msg void OnZoomRectin();
	afx_msg void OnUpdateZoomRectin(CCmdUI* pCmdUI);
public:
	afx_msg void OnZoomZoomnormal();
protected:
	afx_msg void OnUpdateZoomZoomnormal(CCmdUI* pCmdUI);
	afx_msg void OnZoomZoomoff();
	afx_msg void OnUpdateZoomZoomoff(CCmdUI* pCmdUI);
	afx_msg void OnZoomZoomtofit();
	afx_msg void OnUpdateZoomZoomtofit(CCmdUI* pCmdUI);
	afx_msg void OnZoomp10();
	afx_msg void OnUpdateZoomp10(CCmdUI* pCmdUI);
	afx_msg void OnZoomp100();
	afx_msg void OnUpdateZoomp100(CCmdUI* pCmdUI);
	afx_msg void OnZoomp150();
	afx_msg void OnUpdateZoomp150(CCmdUI* pCmdUI);
	afx_msg void OnZoomp200();
	afx_msg void OnUpdateZoomp200(CCmdUI* pCmdUI);
	afx_msg void OnZoomp400();
	afx_msg void OnUpdateZoomp400(CCmdUI* pCmdUI);
	afx_msg void OnZoomp50();
	afx_msg void OnUpdateZoomp50(CCmdUI* pCmdUI);
	afx_msg void OnZoomp75();
	afx_msg void OnUpdateZoomp75(CCmdUI* pCmdUI);
	afx_msg void OnZoomp800();
	afx_msg void OnUpdateZoomp800(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnComboSelChange();
	afx_msg void OnComboEditChange();
	afx_msg void OnComboEditUpdate();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcMouseMove( UINT nHitTest, CPoint point );
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnAnalysisLocalextraction();
	afx_msg void OnUpdateAnalysisLocalextraction(CCmdUI* pCmdUI);
	afx_msg void OnAnalysisLaunchscenario();
	afx_msg void OnUpdateAnalysisLaunchscenario(CCmdUI* pCmdUI);
	afx_msg void OnAnalysisLocalclassification();
	afx_msg void OnUpdateAnalysisLocalclassification(CCmdUI* pCmdUI);
	afx_msg void OnAnalysisLocalsegmentation();
	afx_msg void OnUpdateAnalysisLocalsegmentation(CCmdUI* pCmdUI);
	afx_msg void OnAnalysisTextureextraction();
	afx_msg void OnUpdateAnalysisTextureextraction(CCmdUI* pCmdUI);
	afx_msg void OnAnalysisTexturesegmentation();
	afx_msg void OnUpdateAnalysisTexturesegmentation(CCmdUI* pCmdUI);
	afx_msg void OnTransLaunchscenario();
	afx_msg void OnUpdateTransLaunchscenario(CCmdUI* pCmdUI);
	afx_msg void OnTransSpatialConfinedstretch();
	afx_msg void OnUpdateTransSpatialConfinedstretch(CCmdUI* pCmdUI);
	afx_msg void OnTransSpatialRotation();
	afx_msg void OnUpdateTransSpatialRotation(CCmdUI* pCmdUI);
	afx_msg void OnTransSpatialStretch();
	afx_msg void OnUpdateTransSpatialStretch(CCmdUI* pCmdUI);
	afx_msg void OnTransSpectralInvert();
	afx_msg void OnUpdateTransSpectralInvert(CCmdUI* pCmdUI);
	afx_msg void OnTransTemporalEvolve();
	afx_msg void OnUpdateTransTemporalEvolve(CCmdUI* pCmdUI);
	afx_msg void OnTransformationRepeatinsequence();
	afx_msg void OnUpdateTransformationRepeatinsequence(CCmdUI* pCmdUI);
	afx_msg void OnTransSpatialEdgedetect();
	afx_msg void OnUpdateTransSpatialEdgedetect(CCmdUI* pCmdUI);
	afx_msg void OnTransSpatialFlatdetect();
	afx_msg void OnUpdateTransSpatialFlatdetect(CCmdUI* pCmdUI);
	afx_msg void OnTransSpatialMaxdetect();
	afx_msg void OnUpdateTransSpatialMaxdetect(CCmdUI* pCmdUI);
	afx_msg void OnTransSpatialSharpen();
	afx_msg void OnUpdateTransSpatialSharpen(CCmdUI* pCmdUI);
	afx_msg void OnTransSpectralEqualize();
	afx_msg void OnUpdateTransSpectralEqualize(CCmdUI* pCmdUI);
	afx_msg void OnTransSpectralHsi();
	afx_msg void OnUpdateTransSpectralHsi(CCmdUI* pCmdUI);
	afx_msg void OnTransSpectralStretch();
	afx_msg void OnUpdateTransSpectralStretch(CCmdUI* pCmdUI);
	afx_msg void OnViewNewZoomedview();
	afx_msg void OnAnalysisScaleSelection();
	afx_msg void OnUpdateViewNewZoomedview(CCmdUI* pCmdUI);
	afx_msg void OnTransSpectralFilter();
	afx_msg void OnRoiRect();
	afx_msg void OnRoiPolyline();
	afx_msg void OnRoiFloating();
	afx_msg void OnRoiFreehand();
	afx_msg void OnEditClear();
	afx_msg void OnZoomp1000();
	afx_msg void OnZoomp1500();
	afx_msg void OnZoomp2000();
	afx_msg void OnZoomp2500();
	afx_msg void OnZoomp3000();
	afx_msg void OnSettingsProfile();
	afx_msg void OnSettingsHisto();
	afx_msg void OnSettingsScatter();
	afx_msg void OnDestroy();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnEditPaste();
	afx_msg void OnTransSpectralConverttomono();
	afx_msg void OnUpdateTransSpectralConverttomono(CCmdUI* pCmdUI);
	afx_msg void OnTransSpectralConverttomonoSet();
	afx_msg void OnUpdateTransSpectralConverttomonoSet(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditCopy();
};

/* //spi 2014
#ifndef _DEBUG  // debug version in OW2view.cpp
inline COW2Doc* COW2View::GetDocument()
   { return (COW2Doc*)m_pDocument; }
#endif
*/


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

class OIFIILIB_API COW2ZoomedView : public COW2View
{
public:
	DECLARE_DYNCREATE(COW2ZoomedView)

protected: 
	BOOL bFirstTime;
	class COW2View* m_pOW2View;

	//{{AFX_VIRTUAL(COW2ZoomedView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:
	COW2ZoomedView();
	~COW2ZoomedView();
	void AttachParentView(COW2View* pView);
	void DetachParentView();

	void SetWindowPosition();

protected:
	//{{AFX_MSG(COW2ZoomedView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif //_OW2VIEW_H
