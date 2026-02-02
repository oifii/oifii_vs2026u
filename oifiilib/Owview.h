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
// OWview.h : interface of the COWView class
//
/////////////////////////////////////////////////////////////////////////////
#ifndef _OWVIEW_H
#define _OWVIEW_H


//user defined message
#define	OWM_VIEW_UPDATEINFO		WM_USER+3
#define	OWM_DOC_UPDATEINFO		WM_USER+4


//four input modes:
//  NORMAL - Draw a cloud
//  ZOOMINPOINT - Zoom in on the point, cursor is an 'in arrow'
//  ZOOMOUTPOINT - Zoom in on the point, cursor is an 'out arrow'
//  ZOOMRECT - Zoom in based on a rubber-banding rectangle.


typedef enum {NORMAL,ZOOMINPOINT,ZOOMOUTPOINT,ZOOMRECT,DRAWELLIPSE,SELECTELLIPSE} OWViewMode;	 //activated with mouse in toolbar or menu

class OIFIILIB_API COWView : public CScrollView //CView //SECPanView  //SECPanView
{
protected: // create from serialization only
	COWView();
	COWView(HWND hwnd, class COWDocument* pCOWDoc); //spi 2014
	void Init();
	DECLARE_DYNCREATE(COWView)

// Attributes
public:
	HWND m_parentHWND;  //spi 2014                                                             
	class COWDocument* m_pCOWDoc; //spi 2014

    enum {OW_ZOOMOFF, OW_ZOOMPANOVER, OW_ZOOMNORMAL, OW_ZOOMFIT};

	//used in combinaison with m_pViewDataSettings->bDisplayPixelset
	BOOL m_bViewPixelset_DisplayRed;
	BOOL m_bViewPixelset_DisplayGreen;
	BOOL m_bViewPixelset_DisplayBlue;
	BOOL m_bViewPixelset_DisplayIntensity;
	BOOL m_bViewPixelset_DisplayHue;
	BOOL m_bViewPixelset_DisplaySaturation;
	void UpdateViewPixelsetDisplayFlags(); //from draw pixelset flag
	void Update_ViewDataSettings_DrawPixelsetFlag(); //from sub pixelset display flags
	void GetPixelsetChannelVisibility(int* pINT, int nINT);

	class CViewDataSettings*		m_pTempViewDataSettings;
	class CViewDataSettings*		m_pViewDataSettings;
	class CProjectionSettings*		m_pProjectionSettings;
	class CTransformationSettings*	m_pTransformationSettings;

	int m_iNumberOfSegmentForFinalPartition;
	int m_iNumberOfSegmentForFinalClassification; //poirier dec 98, added for classification

    OWViewMode m_owviewMode;   //Current 'mode'
    BOOL m_bCaptured;	//Is the mouse captured?
    CRect m_rectDrag;			//The dragging rectangle for zoomrect mode.
    CRect m_rectEllipseDrag;	//The dragging rectangle for DRAWELLIPSE mode.
    CRect m_rectROIRectDrag;	//The dragging rectangle for roi rect mode (when dropping a rect roi).
    CRect m_rectROIPolylineDrag;	//The dragging rectangle for roi polyline mode (when dropping polyline, line segment by line segment).
    CRect m_rectROIFreehandDrag;	//The dragging rectangle for roi polyline mode (when dropping polyline, line segment by line segment).
    BOOL m_bCenter;
    //Cursors for Zooming UI.
    HCURSOR m_hZoomInPointCursor;   
    HCURSOR m_hZoomOutPointCursor;
    HCURSOR m_hZoomRectCursor;
    HCURSOR m_hZoomRectDragCursor;

    HCURSOR m_hDrawEllipseCursor;
    HCURSOR m_hDrawEllipseDragCursor;

    HCURSOR m_hROIRectCursor;    
    HCURSOR m_hROIRectDragCursor; 
    HCURSOR m_hROIPolylineCursor;    
    HCURSOR m_hROIPolylineDragCursor; 
	HCURSOR m_hROIFreehandCursor;
	HCURSOR m_hROIFreehandDragCursor;

public:
	//for mapping coor from world to view (and vis-versa)
	CRect  m_WorldRect;	//full map
	CRect  m_ViewRect; //portion of map viewed
	CSize  m_WindowSize; //actual display window size
	CPoint m_ViewOffset;  //between viewrect and client window origin

private:
	double m_dfZoomFactor;
	double m_dfZoomFactorMin;
	double m_dfZoomFactorMax;

    UINT m_zoomMode;		// The current zoom mode: normal/off/zoom to fit
    CSize m_szOrigTotalDev;  // Original total size in device units
    CSize m_szOrigPageDev;   // Original page scroll size in device units
    CSize m_szOrigLineDev;   // Original line scroll size in device units

// Operations
public:
	BOOL HasToDrawPixel();
	BOOL HasToDrawOverlay();

	void SetScrollInfo();
	void SetScrollSizes(int nMapMode,
						SIZE sizeTotal, 
						const SIZE& sizePage = sizeDefault,
						const SIZE& sizeLine = sizeDefault);

    void CenterOnLogicalPoint(CPoint ptCenter);
    CPoint GetLogicalCenterPoint(void);
    
    // Zooming functions
    UINT GetZoomMode() {return m_zoomMode;};
    void SetZoomMode(UINT zoomMode);
	double GetZoomFactor();
    
    void ZoomIn(CRect &rect);
    void ZoomIn(CPoint *point = NULL, double delta = 1.25);
    void ZoomOut(CPoint *point = NULL, double delta = 1.25);
    void ZoomOriginal(CPoint *point = NULL);
    void ZoomFit();
    
    //Arguments slightly different, so people can just enter percent
    //and have point as default instead of having to specify null point.
    void ZoomPercent(int nPercent = 25,CPoint *point = NULL);
    
    // Zooming utility functions
    void ViewDPtoLP (LPPOINT lpPoints, int nCount = 1);
	void ViewDPtoLP(RECT* pRECT);
    void ViewLPtoDP (LPPOINT lpPoints, int nCount = 1);
    void ClientToDevice(CPoint &point);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COWView)
	public:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnEraseBkgnd(CDC* pDC);
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COWView();

	// Overrideables
protected:
    // Override this to get zoom scale change notifications.
    virtual void ZoomFactorChanged(void) {};
    virtual void ZoomOutOfRange(BOOL bUnderFlow, double dfAttempt);
public:
	virtual void SetZoomFactor(double dfZoomFactor);
protected:
	virtual void SetZoomFactorMinMax(double dfZoomFactorMin, double dfZoomFactorMax);
protected:
	void GetZoomFactorMinMax(double* pfZoomFactorMin, double* pfZoomFactorMax);
public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
protected:
    void PersistRatio(const CSize &orig, CSize &dest, CPoint &remainder);
    void ReCalcBars(void);
    void UpdateViewport(CPoint * ptNewCenter);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	//virtual class COW2Doc* COWView::GetDocument();
#endif
	virtual class COW2Doc* COWView::GetDocument();

protected:
	BOOL IsActiveView();

// Generated message map functions
protected:
	//{{AFX_MSG(COWView)
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewPixelset();
	afx_msg void OnUpdateViewPixelset(CCmdUI* pCmdUI);
	afx_msg void OnViewPointset();
	afx_msg void OnUpdateViewPointset(CCmdUI* pCmdUI);
	afx_msg void OnViewPointsetLabels();
	afx_msg void OnUpdateViewPointsetLabels(CCmdUI* pCmdUI);
	afx_msg void OnViewTriangleset();
	afx_msg void OnUpdateViewTriangleset(CCmdUI* pCmdUI);
	afx_msg void OnViewVoronoiset();
	afx_msg void OnUpdateViewVoronoiset(CCmdUI* pCmdUI);
	afx_msg void OnViewSegmentset();
	afx_msg void OnUpdateViewSegmentset(CCmdUI* pCmdUI);
	afx_msg void OnViewGotopartition();
	afx_msg void OnUpdateViewGotopartition(CCmdUI* pCmdUI);
 	afx_msg LRESULT OnUpdateDocInfo(WPARAM wParam, LPARAM lParam);
 	afx_msg LRESULT OnUpdateViewInfo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewGotoclassification();
	afx_msg void OnUpdateViewGotoclassification(CCmdUI* pCmdUI);
	afx_msg void OnViewPixelsetBlue();
	afx_msg void OnUpdateViewPixelsetBlue(CCmdUI* pCmdUI);
	afx_msg void OnViewPixelsetGreen();
	afx_msg void OnUpdateViewPixelsetGreen(CCmdUI* pCmdUI);
	afx_msg void OnViewPixelsetHue();
	afx_msg void OnUpdateViewPixelsetHue(CCmdUI* pCmdUI);
	afx_msg void OnViewPixelsetIntensity();
	afx_msg void OnUpdateViewPixelsetIntensity(CCmdUI* pCmdUI);
	afx_msg void OnViewPixelsetRed();
	afx_msg void OnUpdateViewPixelsetRed(CCmdUI* pCmdUI);
	afx_msg void OnViewPixelsetSaturation();
	afx_msg void OnUpdateViewPixelsetSaturation(CCmdUI* pCmdUI);
	afx_msg void OnViewPointsetCross();
	afx_msg void OnViewPointsetEllipse();
	afx_msg void OnViewPointsetMark();
	afx_msg void OnViewTrianglesetLabels();
	afx_msg void OnViewTriangleset3vertices();
	afx_msg void OnViewTrianglesetCenterpoints();
	afx_msg void OnViewTrianglesetPolygon();
	afx_msg void OnViewVoronoisetLabels();
	afx_msg void OnUpdateViewPointsetCross(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPointsetEllipse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPointsetMark(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewTriangleset3vertices(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewTrianglesetCenterpoints(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewTrianglesetLabels(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewTrianglesetPolygon(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewVoronoisetLabels(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in OW2view.cpp
inline class COW2Doc* COWView::GetDocument() { return (class COW2Doc*)m_pDocument; }
#endif

#endif// _OWVIEW_H
