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

#ifdef OWGRAPH_INTERNAL
	#define OW_AFX_EXT_OR_INT_CLASS
#else
	#ifndef OWGRAPH_EXTERNAL
	#define OWGRAPH_EXTERNAL
	#endif
	#define OW_AFX_EXT_OR_INT_CLASS		AFX_EXT_CLASS 
	//#pragma message( "Forcing owgraph library to be used as an MFC EXT DLL " __FILE__ )
#endif


/////////////////////////////////////////////////////////////////////////////
// private.h : private definitions for all classes of graph library 
//                                                    
// This class is a part of offworld graph library. 
//
// This source code is only intended as a supplement to the
// Offworld Graph Library and related electronic documentation 
// provided with the library. This source code is provided as
// is without any garantee from Offworld Technologies Corp.
//
// Offworld cannot be held responsable for any system corruption
// and cannot be held responsable for any lost of data that could
// be directly or indirectly caused by the use of this source code.
/////////////////////////////////////////////////////////////////////////////
#ifndef _PRIVATE_H
#define _PRIVATE_H

//used by graphmdi.cpp and graview.cpp
#define ID_GRAPHVIEW_STATUSBAR	10010

#define ID_VIEW_ZOOM_BASE               300
#define ID_VIEW_ZOOM_IN_11              301
#define ID_VIEW_ZOOM_IN_21              302
#define ID_VIEW_ZOOM_IN_31              303
#define ID_VIEW_ZOOM_IN_41              304
#define ID_VIEW_ZOOM_IN_51              305
#define ID_VIEW_ZOOM_IN_61              306
#define ID_VIEW_ZOOM_IN_71              307
#define ID_VIEW_ZOOM_IN_81              308
#define ID_VIEW_ZOOM_OUT_11             317
#define ID_VIEW_ZOOM_OUT_12             318
#define ID_VIEW_ZOOM_OUT_13             319
#define ID_VIEW_ZOOM_OUT_14             320
#define ID_VIEW_ZOOM_OUT_15             321
#define ID_VIEW_ZOOM_OUT_16             322
#define ID_VIEW_ZOOM_OUT_17             323
#define ID_VIEW_ZOOM_OUT_18             324
#define ID_VIEW_TOFIT                   332
#define ID_VIEW_ZOOM                    333
#define ID_BASE_POPUP_GRAPHMENU0        350
#define ID_BASE_POPUP_GRAPHMENU1        351
#define ID_BASE_POPUP_GRAPHMENU2        352
#define ID_BASE_POPUP_GRAPHMENU3        353
#define ID_BASE_POPUP_GRAPHMENU4        354
#define ID_BASE_POPUP_GRAPHMENU5        355
#define ID_BASE_POPUP_GRAPHMENU6        356
#define ID_BASE_POPUP_GRAPHMENU7        357
#define ID_BASE_POPUP_GRAPHMENU8        358
#define ID_BASE_POPUP_GRAPHMENU9        359
#define ID_BASE_POPUP_GRAPHMENU10       360
#define ID_BASE_POPUP_GRAPHMENU11       361
#define ID_BASE_POPUP_GRAPHMENU12       362
#define ID_BASE_POPUP_GRAPHMENU13       363
#define ID_BASE_POPUP_GRAPHMENU14       364
#define ID_BASE_POPUP_GRAPHMENU15       365
#define ID_BASE_POPUP_GRAPHMENU16       366
#define ID_BASE_POPUP_GRAPHMENU17       367
#define ID_BASE_POPUP_GRAPHMENU18       368
#define ID_BASE_POPUP_GRAPHMENU19       369
#define ID_BASE_POPUP_GRAPHMENU20       370

#define ID_VIEW_AS_LINEGRAPH            408
#define ID_VIEW_AS_VBARGRAPH            409
#define ID_VIEW_AS_HBARGRAPH            410
#define ID_VIEW_FIT_ALL                 411
#define ID_VIEW_FIT_ONEVALUEPERPIXEL    412
#define ID_VIEW_GRAPH                   413
#define ID_VIEW_GRAPHDATA               414
#define ID_OPTIONS_AUTORESCALING        415

//COWGraphSet::m_ldatatype can be one of these values                   
#define LONGTYPE  0                   
#define FLOATTYPE 1     
#define DOUBLETYPE 2     

//if these runtime resource objects are required elsewhere, these macro 
//id definitions could be eventualy moved to resource.h (or equivalent).                
#define ID_VBXGRAPHCONTROL 		10001
#define ID_TEXT1                10002
#define ID_TEXT2   				10003
           
//macros used to set COWGraphView::ldisplaymode flag
#define FITINWINDOW      0
#define ONEDATAPERPIXEL  1 

//macros used to set COWGraphView::lgraphtype flag
#define GRAPH_LINE									1
#define GRAPH_VBAR									2
#define GRAPH_HBAR									4
#define GRAPH_3DVBAR								8
#define GRAPH_3DHBAR								16
#define GRAPH_VGANTT								32
#define GRAPH_HGANTT								64
//poirier, march 27 1997, for oiii
#define GRAPH_SCATTER								128
//poirier, oct 1997, for isera group
#define GRAPH_PILED									256
#define GRAPH_REVERSE_CHANNEL_DISPLAY_ORDER			512
#define GRAPH_FORFIRSTCHANNEL_OUTLINE_BAR_TOP_ONLY	1024

//macro used by COWGraphView              
#define TICKS_NONE		0
#define TICKS_VERT		1
#define TICKS_HORZ		2
#define TICKS_BOTH		3

//macro used by COWGraphView              
#define GRID_VERT		1
#define GRID_HORZ		2
//be carefull if you add extra here, 
//it should be 4, 8, 16, ...
 
//macro used by COWGraphDoc, with function GenerateTabbedTextBuffer(int memflag), memflag can be:
#define USING_LOCAL_HANDLE   1
#define USING_GLOBAL_HANDLE  2

//macro used by COWGraphView, when displaying a subset of the graph data (ex.: when not
//enough pixel to display all values) 
#define DISPLAY_FIT_MAX_VALUE	   	0
#define DISPLAY_FIT_MIN_VALUE	    1
#define DISPLAY_FIT_AVERAGE_VALUE	2
#define DISPLAY_FIT_FIRST_VALUE  	3

//macro used by COWGraphView, when displaying bar graph
#define DISPLAY_ORDER_NONE				0
#define DISPLAY_ORDER_MAX_VALUE_FIRST	1

//
#define GRAPH_DEFAULT_COLOR		-1

#endif //_PRIVATE_H


/////////////////////////////////////////////////////////////////////////////
// graphset.h : definition of the GraphSet class
//                                                    
// This class is a part of offworld graph library. 
//
// This source code is only intended as a supplement to the
// Offworld Graph Library and related electronic documentation 
// provided with the library. This source code is provided as
// is without any garantee from Offworld Technologies Corp.
//
// Offworld cannot be held responsable for any system corruption
// and cannot be held responsable for any lost of data that could
// be directly or indirectly caused by the use of this source code.
//
//
//
// Purpose:
//
// COWGraphSet class is used by the COWGraphDoc to manage
// long or float arrays.  More precicely, it provides
// a level of abstation between the array of data and
// the GraphDoc (that manages list of data set)
/////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHSET_H
#define _GRAPHSET_H           
    
class OW_AFX_EXT_OR_INT_CLASS COWGraphSet : public CObject
{
private: 
	int m_ldatatype;
	int m_ldatasize;
	int m_lchannelid;
	int m_lgraphsetcolor;
	char m_psz_graphsetname[80];
	BOOL m_bvisible;
	int far* m_lp_longarray;
	double far* m_lp_floatarray;
	double far* m_lp_doublearray;
    int m_loffsetbetweentwoconsecutivevalues;	//non-zero when using lp_doublearray, 
												//should be equal to the number of allocated 
												//stat channels in pointset or segmentset
public:
	COWGraphSet();  
    //allocate new data set
	COWGraphSet(int far* lp_longarray,	 
			  int lchannelsize, 
			  LPCSTR psz_channelname=NULL, 
			  int lchannelid=0,
			  COLORREF crRGBColor=GRAPH_DEFAULT_COLOR);
	//allocate new data set
	COWGraphSet(double far* lp_floatarray, 
			  int lchannelsize, 
			  LPCSTR psz_channelname=NULL, 
			  int lchannelid=0,
			  COLORREF crRGBColor=GRAPH_DEFAULT_COLOR); 
	//by default does not allocate any data
	COWGraphSet(double far* lp_dfBasePointer,
			  int lOffsetBetweenTwoConsecutiveValues,
			  int lchannelsize, 
			  LPCSTR psz_channelname=NULL, 
			  int lchannelid=0,
			  BOOL bReallocateAsFloatArray=FALSE,
			  COLORREF crRGBColor=GRAPH_DEFAULT_COLOR); 
	void Initialize(void);		  
	~COWGraphSet();
	
	int   GetValueAsLong(int index);
	double  GetValueAsFloat(int index);
private:
	  //always called by GetValueAsFloat() for now
	double GetValueAsDouble(int index);
public:
	//LPCSTR GetValueAsString(int); 
	CString GetValueAsString(int index);
	
	void SetValue(int, int);
	void SetValue(double, int); 
	BOOL SetAllValues(int far*, int, int);
	BOOL SetAllValues(double far*, int, int);
	BOOL AddValue(double); 
	BOOL AddValue(int);
    BOOL AddExtraValues(int far*, int);        
    BOOL AddExtraValues(double far*, int);        
    
    LPCSTR GetGraphSetName(void);
    int GetDataType();
	void SetDataType(int ldatatype);
	int GetDataSize();
	int GetChannelID();
	int GetColor();
	BOOL m_bUseColorTable;

	BOOL IsVisible();
	void SetVisibility(BOOL);

	double GetYMin();
	double GetYMax();
	double GetAverage();
	double GetStddev();	
};
#endif //_GRAPHSET_H


/////////////////////////////////////////////////////////////////////////////
// graphdoc.h : definition file for the class GraphDoc
//
// This class is a part of offworld graph library. 
//
// This source code is only intended as a supplement to the
// Offworld Graph Library and related electronic documentation 
// provided with the library. This source code is provided as
// is without any garantee from Offworld Technologies Corp.
//
// Offworld cannot be held responsable for any system corruption
// and cannot be held responsable for any lost of data that could
// be directly or indirectly caused by the use of this source code.
//
//
//
// Purpose:
//
// This GraphDoc class can be directly use by any of the image class to 
// receive graph data and maintain viewtype, coloringtype and labeling
// of the various sets of data received.
//
// In order to manage data sets (arrays of long or float) efficently, this
// class uses GraphSet object and keep these in a ObList object.  The 
// GraphDoc class will therefore have the functionality to manipulate this
// list of GraphSets.	
//
//
//
//
// Limitations: 
//
//		-at the moment, can not use more than 20 channels at a time, would be easy to 
//       extend.  there is no storage size restriction other than the available ram, but
//		 in the grapview.cpp, when drawing line or bar chart, it uses the channel number to
//       select pens and brushes.
//      -vertical bar chart display fully supported, but not horizontal bar, would be easy
//       to support, but we should make sure vertical bar display mode behave correctly before
//       supporting this mode.                         
//
//
// Tests TODO:
//
//      -test with negative values.
// 		-more test with floating point arrays.
//	                                                                     
//
/////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHDOC_H
#define _GRAPHDOC_H

class OW_AFX_EXT_OR_INT_CLASS COWGraphDoc : public CDocument
{
	DECLARE_SERIAL(COWGraphDoc)
// Attributes
public:    
    CObArray m_GraphSetList; //array containing the graph sets
    int m_numberofvisiblesets; //should always be maintained by COWGraphDoc 
    int m_maxnumberofpointsperset; //should always be maintained by COWGraphDoc
    int m_oldymax; //when autorescaling enabled, it is maintained by COWGraphView  
    BOOL m_addlabels; //add labels when copying data to clipboard ( also when displaying ).
    BOOL m_autorescaling; //default TRUE, it rescale everytime a graphset is added or removed
    
    CString m_XAxisLabelString; //displayed in the left-most statusbar pane
    CString m_YAxisLabelString; //displayed in the right-most statusbar pane

	//poirier, march 27 1997
	BOOL m_bMapIndexToFloatDISPLAYFLOAT;
	double m_dfMapIndexToFloatMINFLOAT;
	double m_dfMapIndexToFloatMAXFLOAT;
	int m_iMapIndexToFloatNUMBEROFLEVEL;

	//poirier, march 27 1997
	int m_lgraphtype;
	int m_lviewgrid;

	COLORREF m_crDefaultBackgroundColor;
	COLORREF m_crDefaultTextColor;
	BOOL m_bUseDefaultMultiColorTable;
	BOOL m_bUseDefaultGrayColorTable;

	int m_iPenWidthForFirstChannelBarTop;
public:
	void FixMaxNumberOfPointPerSet(int nMaxNumPoint);
	void FixPROFILEMinMaxXYValue(double fMinY, double fMaxY);
	void FixHISTOMinMaxXYValue(double fMinY, double fMaxY);
	void FixSCATTERMinMaxXYValue(double fMinX, double fMaxX, double fMinY, double fMaxY);
	int m_PreFixed_maxnumberofpointsperset;
	double m_PreFixed_PROFILEdfMinValueY;
	double m_PreFixed_PROFILEdfMaxValueY;
	double m_PreFixed_HISTOdfMinValueY;
	double m_PreFixed_HISTOdfMaxValueY;
	double m_PreFixed_SCATTERdfMinValueX;
	double m_PreFixed_SCATTERdfMaxValueX;
	double m_PreFixed_SCATTERdfMinValueY;
	double m_PreFixed_SCATTERdfMaxValueY;
private:    
    CDocument* m_pImageDoc; // used with line profile
// Implementation
public:
	COWGraphDoc();  // constructor used by dynamic creation
	~COWGraphDoc();

	void MapIndexToFloatValuesOnDisplay(BOOL bEnable);
	BOOL MapIndexToFloatValuesOnDisplay(double dfMin, double dfMax, int iNumberOfLevel);
    
    void AttachImageDoc(CDocument* pImageDoc);
    void DetachImageDoc();
    
	void AddGraphSet(int far* lp_longarray, 
					 int ldatasize,
					 LPCSTR psz_channelname=NULL, 
					 int lchannelid=0,
					 BOOL bVisible=TRUE);  
					 
	void AddGraphSet(double far* lp_floatarray, 
					 int ldatasize, 
					 LPCSTR psz_channelname=NULL, 
					 int lchannelid=0,
					 BOOL bVisible=TRUE); 

	//it copies the input double float data (that are interlaced) 
	//as a contiguous float vector in the newly created graphset, if
	//flag bReallocateAsFloatArray is TRUE. 
	//it takes data directly from the supplied input double 
	//float pointer (without allocating and copying when 
	//bReallocateAsFloatArray is FALSE, the default). 
	void AddGraphSet(	double* lp_dfBasePointer,
						int lOffsetBetweenTwoConsecutiveValues,
						int lchannelsize, 
						LPCSTR psz_channelname=NULL, 
						int lchannelid=0,
						BOOL bReallocateAsFloatArray=FALSE); 
	
	int AddValueToGraphSet(double, int);
	
	int RemoveGraphSet(int);   
	void RemoveAllGraphSet(); 
	
	double GetYMin(void);
	double GetYMin(int);
	double GetYMax(void);	
	double GetYMax(int);
	int   GetXMax(void);
	
	void SetGraphType(int iGraphType);
	//you can combine GRID_HORZ with GRID_VERT or 0 for none
	void SetViewGrid(int iFlag);

	int GetNumberOfVisibleSets();
	int GetNumberOfSets();

	void SetTitle(LPCSTR lpszString);
	void SetXAxisLabel(LPCSTR lpszString);
	void SetYAxisLabel(LPCSTR lpszString);
	void SetPenWidthForFirstChannelBarTopRendering(int iPenWidth);
    
    void SetAutoRescaling(BOOL bFlag); 	//if TRUE, it rescale everytime a graphset is added or removed
    									//otherwise, no rescale is performed
private:	
	int SearchForGraphSetIndex(int);
	
public: 
	void ModifyVisibilityOfGraphSet(int index); 
	void GetVisibilityOfAllGraphSets(BOOL* pBOOL, int* nBOOL);
	void SetVisibilityOfAllGraphSets(BOOL* pBOOL, int nBOOL);

	HGLOBAL GenerateTabbedTextBuffer(void); //used by CopyGraphSetsToClipboardVerticaly()  
	BOOL CopyGraphSetsToClipboardVerticaly(void); //copy data to clipboard, final version 
	                                        
	//assumes tabbedtext 1 channel per row, used only at development-time for testing.
    BOOL PasteGraphSetsFromClipboard(void); //to receive Excel data from clipboard    

	void SetDefaultGraphBackgroundAndTextColor(	COLORREF crBackGroundColor = RGB(255,255,255),
												COLORREF crTextColor = RGB(0,0,0));
	void UseDefaultGraphMultiColorTable();
	void UseDefaultGraphGrayColorTable();

protected:                                                                   
	virtual void Serialize(CArchive& ar);	// overridden for document i/o
	virtual	BOOL OnNewDocument();

	// Generated message map functions
protected:
	//{{AFX_MSG(COWGraphDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnFileSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif //_GRAPHDOC_H


/////////////////////////////////////////////////////////////////////////////
// graphmdi.h : implementation file for the class COWGraphMDIChildWnd
//
// This class is a part of offworld graph library. 
//
// This source code is only intended as a supplement to the
// Offworld Graph Library and related electronic documentation 
// provided with the library. This source code is provided as
// is without any garantee from Offworld Technologies Corp.
//
// Offworld cannot be held responsable for any system corruption
// and cannot be held responsable for any lost of data that could
// be directly or indirectly caused by the use of this source code.
//
//
//
// Purpose:
//
// This GraphMDIChildWnd class has been created because we want to have
// a CStatusBar inside the graphic output window.  The CStatusBar cannot
// be attach to a CView, it must be attach to a CMDIChildWnd or CMDIFrameWnd.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHMDI_H
#define _GRAPHMDI_H

class OW_AFX_EXT_OR_INT_CLASS COWGraphMDIChildWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(COWGraphMDIChildWnd)
protected:
	COWGraphMDIChildWnd();			// protected constructor used by dynamic creation
// Attributes
public:
	CStatusBar	m_stBarImgFrm;
	CFont m_fontStatusBar;
	int m_nPanes;
// Operations
public:
	void CalcFrameSize( CSize* sizeDoc );
	void SetFrameSize( CSize sizeGraphWindow );
// Implementation
protected:
	virtual ~COWGraphMDIChildWnd();
	// Generated message map functions
	//{{AFX_MSG(COWGraphMDIChildWnd)
	afx_msg void OnViewStatusBar();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif //_GRAPHMDI_H


/////////////////////////////////////////////////////////////////////////////
// graphvw.h : definition file for the class COWGraphView
//
// This class is a part of offworld graph library. 
//
// This source code is only intended as a supplement to the
// Offworld Graph Library and related electronic documentation 
// provided with the library. This source code is provided as
// is without any garantee from Offworld Technologies Corp.
//
// Offworld cannot be held responsable for any system corruption
// and cannot be held responsable for any lost of data that could
// be directly or indirectly caused by the use of this source code.
//
//
//
// Specifications:
//
//      -2D drawing relying on WIN32 GDI through the use of MFC.
//
//      -Copy and Paste from clipboard are supported for graphic
//       and data (MetaFile for graphic, TabbedText for data).
//
//		-Graphic export to MS-Word as well as data in MS-Excel 
//		 tested and supported.
//
//      -Printing is supported (WYSIWYG).  
//
//
// Purpose:
//
// This GraphView class can render the COWGraphSet contained in COWGraphDoc 
// as a Line, VBar, HBar, Graphs.  
//
// Drawing routine are all included in this COWGraphView object.
/////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHVIEW_H
#define _GRAPHVIEW_H

typedef struct tagVBARINFO 
{ 
	int nPos;
	int nOffset;
	int nHeight;
} VBARINFO;

//structure shared by DrawBarGraph(), ComputeBarGraphRenderingAttributes()
//as well as by COWGraphView::OnSize ()
typedef struct tagGRAPHRENDERINGATTRIBUTES
{
	CPoint ptOrigin; 
	int nMaxValue;
	CSize sizePixelsPerTick;
	class COWGraphDoc* pDoc;
	int nBarWidth; 
	int nHorzTicks; 
	int nVertTicks;
	int nCharHeight;
	int nDataSetWidth;
	int nSpaceWidth;
	BOOL bSkipDrawing;
} GRAPHRENDERINGATTRIBUTES;

           
class OW_AFX_EXT_OR_INT_CLASS COWGraphView : public CScrollView
{
	DECLARE_DYNCREATE(COWGraphView)
protected:
	COWGraphView(); // protected constructor used by dynamic creation
// Attributes
private:
	int m_ldisplaymode; //could be FITINWINDOW or ONEDATAPERPIXEL
	int m_lgraphtype; //see private.h for possible values
private: 
	int m_ldisplayfit; //see private.h for possible values
	int m_ldisplayorder; //see private.h for possible values

    BOOL m_bFitDataIn; //flag to tell if data should be fitted in  
    BOOL m_bStatusBarVisible; //flag to tell if status bar is visible          
    int m_lGrid; //flag to tell if grid (horz and/or vert) is present 
    BOOL m_bInterpolationDesired; //flag to tell if interpolation should be displayed in status bar
    							//when cursor moves on top of GRAPH_LINE and more pixel than values.                   

    //previous VerticalBarPosition must be store, because it must be restored on
    //mousemove.    
    BOOL m_bVBarVisible; //to store user selected choice on wether to hightlight or not the bar

    int m_nPrevVBarPos;    //0 if invisible else it's the position in pixel of prev. bar
    int m_nPrevVBarOffset; //0, 1, 2, ... for channel 1, 2, 3, ...
    int m_nPrevVBarHeight; //height in pixel of the previous highlighted bar  
    
    BOOL m_bUpdate; //flag to tell view to update draw or not
     
    //graph scale-related member variables, should be valid when bUpdate is FALSE,
    //that is right after drawing has been refresh
	CPoint m_ptGraphOrigin;  
	CSize m_sizeGraph;  
	CSize m_sizeText;
	CSize m_sizePixelsPerTick; 
	int m_nXValuePerTick;
	int m_nYValuePerTick;  
    int m_nBarWidth;
    
    //this zoom factor can be user-selected 
    int m_zoomfactor; 
     
    int m_numberofvalueperpixel;
    int m_nremainder; 
    
    BOOL m_HasFocus;

public:
	double m_PROFILEdfMinValueY;
	double m_PROFILEdfMaxValueY;

	double m_HISTOdfMinValueY;
	double m_HISTOdfMaxValueY;

	//poirier, march 27 1997
	double m_SCATTERdfMinValueX;
	double m_SCATTERdfMaxValueX;
	double m_SCATTERdfMinValueY;
	double m_SCATTERdfMaxValueY;
private:
	double m_SCATTERdfRangeX;
	double m_SCATTERdfRangeY;

	COLORREF m_crBackgroundColor;
	COLORREF m_crTextColor;
	BOOL m_bUseDefaultMultiColorTable;
	BOOL m_bUseDefaultGrayColorTable;

// Operations
public:	  
	//public because also called by COWGraphMDIChildWnd.
	virtual void OnUpdateMenu(void);  	     
// Implementation  
protected:
	virtual ~COWGraphView();
	virtual	void OnDraw(CDC* pDC);		// overridden to draw this view
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo=NULL);       
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnInitialUpdate();
	void SetScrollSizes(void); 
	void OnViewAsLineGraph(void);   
	void OnViewAsVBarGraph(void); 
	void OnViewAsHBarGraph(void); 
	void OnViewFitAll(void);
	void OnViewFitOneValuePerPixel(void); 
	void OnUpdateViewFitOneValuePerPixel(CCmdUI* pCmdUI);
	void OnUpdateViewStatusBar(CCmdUI* pCmdUI);
    void OnViewStatusBar(void); 
    void OnOptionAutoRescaling(void);
    void OnUpdateOptionAutoRescaling(CCmdUI* pCmdUI);
    
	CRect GetDisplayRect(CDC* pDC);     
	
	void DrawVBar(CDC* pDC, int nOffsetPos, int nBarOffset=0, int nBarHeight=0);
	VBARINFO GetNextVBarInfo(CPoint point);  
	
	CString GetGraphSetName(COWGraphSet* p_currentGraphSet, int index=0);
	CString GetVisibleGraphSetValues(CPoint point);
	
	void PlotScatterGraph(CDC* pDC);
	void PlotLineGraph(CDC* pDC);
public:
	struct tagGRAPHRENDERINGATTRIBUTES ComputeBarGraphRenderingAtributes(CDC* pDC, BOOL bVert);
	COLORREF GetGraphSetColor(class COWGraphSet* pGraphSet, int nDefaultIndex);

protected:
	void PlotBarGraph(CDC* pDC, BOOL bVert);
	int GetTickValue(int nMaxValue, int nNumDiv); 
	
	void PlotCaptions(CDC* pDC,
					  CPoint ptOrigin,
					  CSize sizePixelsPerTick,
					  int nValue,
					  int nValuePerTick,
					  int nNumTicks,
					  int nCharHeight,
					  BOOL bVert,
					  BOOL bAlpha);  
					  
	void PlotAxes(CDC* pDC, 
			 	  CPoint ptOrigin, 
			 	  CSize sizePixelsPerTick,
			 	  int nHorzTicks, 
			 	  int nVertTicks, 
			 	  int nDrawTicks); 
			 
	int GetGraphMetrics(CDC* pDC, 
						int nHDiv, 
						int nVDiv,
						CSize& sizeGraph, 
						CSize& sizePixelsPerTick, 
						CPoint& ptOrigin);
							
	BOOL OnPreparePrinting(CPrintInfo* pInfo);
	void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	void Print();
	void Preview();
	void HandlePopupMenu(CPoint point); //to handle popup menu displayed on mouse_right_click		              
protected:
	//{{AFX_MSG(COWGraphView)
	afx_msg BOOL OnSetVisibility(UINT nID); 
	afx_msg BOOL OnViewZoomIn(UINT nID);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message); 
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnViewGraphData();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrint();
	afx_msg void OnFileNew();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point); 
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); 
	afx_msg void OnSetFocus(CWnd* pWnd); 
	afx_msg void OnKillFocus(CWnd* pWnd); 
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif //_GRAPHVIEW_H


/////////////////////////////////////////////////////////////////////////////
// datamdi.h : implementation file for the class COWGraphDataMDIChildWnd
//			   generated from graphmdi.h, then modified.
//
// This class is a part of offworld graph library. 
//
// This source code is only intended as a supplement to the
// Offworld Graph Library and related electronic documentation 
// provided with the library. This source code is provided as
// is without any garantee from Offworld Technologies Corp.
//
// Offworld cannot be held responsable for any system corruption
// and cannot be held responsable for any lost of data that could
// be directly or indirectly caused by the use of this source code.
//
//
//
// Purpose:
//
// This GraphDataMDIChildWnd class has been created because we want to have
// a CStatusBar inside the graphic data output window.  The CStatusBar cannot
// be attach to a CView, it must be attach to a CMDIChildWnd or CMDIFrameWnd.
// 
// We also need this class to respond on MDI_ACTIVATE and refresh the menu.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef _DATAMDI_H
#define _DATAMDI_H

class OW_AFX_EXT_OR_INT_CLASS COWGraphDataMDIChildWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(COWGraphDataMDIChildWnd)
protected:
	COWGraphDataMDIChildWnd();			// protected constructor used by dynamic creation
// Attributes
public:
protected:
// Operations
public:
	void SetFrameSize( CSize sizeGraphWindow );
// Implementation
protected:
	virtual ~COWGraphDataMDIChildWnd();
	// Generated message map functions
	//{{AFX_MSG(COWGraphDataMDIChildWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct); 
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};    
#endif //_DATAMDI_H



/////////////////////////////////////////////////////////////////////////////
// dataview.h : definition file for class COWGraphDataView
//
// This class is a part of offworld graph library. 
//
// This source code is only intended as a supplement to the
// Offworld Graph Library and related electronic documentation 
// provided with the library. This source code is provided as
// is without any garantee from Offworld Technologies Corp.
//
// Offworld cannot be held responsable for any system corruption
// and cannot be held responsable for any lost of data that could
// be directly or indirectly caused by the use of this source code.
//
//
//
// Purpose:
//
// This COWGraphDataView has been added to enable user to see its graph data. 
// Copy to clipboard is supported using tabbedtext format, so it can be pasted
// to MS-Excel or MS-Word.  Default printing is supported (no pagination)
/////////////////////////////////////////////////////////////////////////////
#ifndef _DATAVIEW_H
#define _DATAVIEW_H

class OW_AFX_EXT_OR_INT_CLASS COWGraphDataView : public CEditView
{
	DECLARE_DYNCREATE(COWGraphDataView)
protected:
	COWGraphDataView();			// protected constructor used by dynamic creation
// Attributes
private:
    HLOCAL m_hTabbedTextBuffer; //buffer holding the displayed text.         
    int m_nEachTabStop; //to allow user to select tab
// Operations
public:
// Implementation
protected:
	virtual ~COWGraphDataView();
	virtual	void OnDraw(CDC* pDC);		// overridden to draw this view   
    virtual void OnInitialUpdate(void); 
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);   
protected:
	//{{AFX_MSG(COWGraphDataView)
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnViewStatusBar(); 
	afx_msg void OnViewGraph();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif //_DATAVIEW_H



