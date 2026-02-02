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
#ifndef _OIFII_PROPPAGE_H
#define _OIFII_PROPPAGE_H


#define PROFILE_IMAGEPIXELS		0		
#define PROFILE_IMAGEFEATURES	1

#define HISTO_IMAGEPIXELS		0		
#define HISTO_IMAGEFEATURES		1

#define SCATTER_IMAGEPIXELS		0		
#define SCATTER_IMAGEFEATURES	1


#define PROFILE_NUMBEROF_POINTCOLLECTIONNAME	6
#define PROFILE_BR_XCENTER			0
#define PROFILE_BR_YCENTER			1
#define PROFILE_BR_PERIMETER		2
#define PROFILE_EDGE_ALONG			3
#define PROFILE_EDGE_INNER			4
#define PROFILE_EDGE_OUTTER			5

#define HISTO_NUMBEROF_POINTCOLLECTIONNAME		9
#define HISTO_BR_INTERIOR			0				//object bounding rect
#define HISTO_SBR_INTERIOR			1				//line segment's bounding rect
#define HISTO_SBR_INNER_EDGE_HALF	2				//line segment's bounding rect
#define HISTO_SBR_OUTTER_EDGE_HALF	3				//line segment's bounding rect
#define HISTO_EDGE_ALONG			4
#define HISTO_EDGE_INNER			5
#define HISTO_EDGE_OUTTER			6
#define HISTO_EDGE_AIO				7				//along, inner and outter edge
#define HISTO_POLYGON_INTERIOR		8				//polygon interior (for closed object only)

#define SCATTER_NUMBEROF_POINTCOLLECTIONNAME	9
#define SCATTER_BR_INTERIOR				0				//bounding rect
#define SCATTER_SBR_INTERIOR			1				//sub bounding rect
#define SCATTER_SBR_INNER_EDGE_HALF		2				//sub bounding rect
#define SCATTER_SBR_OUTTER_EDGE_HALF	3				//sub bounding rect
#define SCATTER_EDGE_ALONG				4
#define SCATTER_EDGE_INNER				5
#define SCATTER_EDGE_OUTTER				6
#define SCATTER_EDGE_AIO				7				//along, inner and outter edge
#define SCATTER_POLYGON_INTERIOR		8				//polygon interior



/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// owpropag.h file
//
// COWPropertyPage implementation
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef _OWPROPAG_H
#define _OWPROPAG_H


class OIFIILIB_API COWPropertyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COWPropertyPage)

// Construction
public:
	class CSettingsSheet* m_pSettingsSheet;

	virtual BOOL UpdateSettingsFromDocToSheet();
	virtual BOOL UpdateSettingsFromSheetToDoc();
	virtual void OnLocalROIcheck() { return; }

	virtual BOOL UpdateGraph() 
	{ 
		return FALSE; 
	}

	COWPropertyPage();
	COWPropertyPage(int iIDD);
	~COWPropertyPage();

	BOOL FillComboBoxWithPointCollectionNames(CComboBox* pComboBox);
	BOOL FillComboBoxWithImageChannelNames(CComboBox* pComboBox);
	BOOL FillComboBoxWithStatisticsChannelNames(CComboBox* pComboBox);
};


#endif //_OWPROPAG_H

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// behapage.h file
//
// CBehaviorPage implementation
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


class OIFIILIB_API CBehaviorPage : public COWPropertyPage
{
	DECLARE_DYNCREATE(CBehaviorPage)

// Construction
public:
	class CBehaviorSettings* m_pBehaviorSettings;
	virtual BOOL UpdateSettingsFromDocToSheet();
	virtual BOOL UpdateSettingsFromSheetToDoc();

	CBehaviorPage();
	~CBehaviorPage();

// Dialog Data
	//{{AFX_DATA(CBehaviorPage)
	enum { IDD = IDD_BEHAVIORSETTINGS };
	CButton	m_MBHighlightSegmentCheck;
	CButton	m_MBHighlightNearestNeighborsCheck;
	CButton	m_MBHighlightNearestPointCheck;
	CButton	m_MBLocalSegmentationCheckbox;
	CButton	m_MBLocalExtractionCheckbox;
	CButton	m_MBLocalClassificationCheckbox;
	CButton	m_IBSelfTrainingCheckbox;
	CButton	m_IBSelfExtracSegValidCheckbox;
	CButton	m_OKButton;
	CButton	m_ApplyButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBehaviorPage)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBehaviorPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplybutton();
	afx_msg void OnCancelbutton();
	afx_msg void OnOkbutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// claspage.h : header file
//
// CClassificationPage dialog
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


class OIFIILIB_API CClassificationPage : public COWPropertyPage
{
	DECLARE_DYNCREATE(CClassificationPage)

// Construction
public:
	class CClassificationSettings* m_pClassificationSettings;
	virtual BOOL UpdateSettingsFromDocToSheet();
	virtual BOOL UpdateSettingsFromSheetToDoc();

	CClassificationPage();
	~CClassificationPage();

// Dialog Data
	//{{AFX_DATA(CClassificationPage)
	enum { IDD = IDD_CLASSIFICATIONSETTINGS };
	CButton	m_CTSupervisedCheckbox;
	CButton	m_CTNonsupervisedCheckbox;
	CButton	m_OKButton;
	CButton	m_ApplyButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CClassificationPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CClassificationPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnOkbutton();
	afx_msg void OnCancelbutton();
	afx_msg void OnApplybutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// extrpage.h : header file
//
// CExtractionPage dialog
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//


class OIFIILIB_API CExtractionPage : public COWPropertyPage
{
	DECLARE_DYNCREATE(CExtractionPage)

// Construction
public:

	class CExtractionSettings* m_pExtractionSettings;
	virtual BOOL UpdateSettingsFromDocToSheet();
	virtual BOOL UpdateSettingsFromSheetToDoc();

	CExtractionPage();
	~CExtractionPage();

// Dialog Data
	//{{AFX_DATA(CExtractionPage)
	enum { IDD = IDD_EXTRACTIONSETTINGS };
	CButton	m_OKButton;
	CButton	m_EZTrainingButton;
	CButton	m_TCDDetectCheck;
	CButton	m_TCCSpectralVariabilityCheck;
	CButton	m_TCCHeightCheck;
	CButton	m_TCCDensityCheck;
	CButton	m_TCCAverageIntensityCheck;
	CButton	m_TCCAreaCheck;
	CEdit	m_TCDThresholdEdit;
	CEdit	m_TCDScaleEdit;
	CButton	m_EZRoadCheck;
	CButton	m_EZManMadeCheck;
	CButton	m_EZForestationCheck;
	CButton	m_EZDeforestationCheck;
	CButton	m_CancelButton;
	CButton	m_ApplyButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CExtractionPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CExtractionPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplybutton();
	afx_msg void OnCancelbutton();
	afx_msg void OnOkbutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// fuspage.h : header file
//
// CFusionPage dialog
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//


class OIFIILIB_API CFusionPage : public COWPropertyPage
{
	DECLARE_DYNCREATE(CFusionPage)

// Construction
public:
	class CFusionSettings* m_pFusionSettings;
	BOOL UpdateSettingsFromDocToSheet();
	BOOL UpdateSettingsFromSheetToDoc();

	CFusionPage();
	~CFusionPage();

// Dialog Data
	//{{AFX_DATA(CFusionPage)
	enum { IDD = IDD_FUSIONSETTINGS };
	CButton	m_STMixedHSOHRGCheckbox;
	CButton	m_STHSOCheckbox;
	CButton	m_STHRGCheckbox;
	CEdit	m_StatMinEdit;
	CEdit	m_StatMaxEdit;
	CComboBox	m_USCWPreClassificationChannelCombobox;
	CComboBox	m_FusionChannelCombobox;
	CButton	m_USCWPreClassificationCheckbox;
	CEdit	m_USCWExclusionThresholdEdit;
	CButton	m_USCWExclusionThresholdCheckbox;
	CButton	m_OKButton;
	CButton	m_CancelButton;
	CButton	m_ApplyButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFusionPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFusionPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplybutton();
	afx_msg void OnCancelbutton();
	afx_msg void OnOkbutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};





/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// projpage.cpp : implementation file
//
// CProjectionPage dialog
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


class OIFIILIB_API CProjectionPage : public COWPropertyPage
{
	DECLARE_DYNCREATE(CProjectionPage)

// Construction
public:
	class CProjectionSettings* m_pProjectionSettings;
	virtual BOOL UpdateSettingsFromDocToSheet();
	virtual BOOL UpdateSettingsFromSheetToDoc();

	CProjectionPage();
	~CProjectionPage();

// Dialog Data
	//{{AFX_DATA(CProjectionPage)
	enum { IDD = IDD_PROJECTIONSETTINGS };
	CEdit	m_VVSTopEdit;
	CEdit	m_VVSRightEdit;
	CButton	m_VVSPerspectiveCheckbox;
	CButton	m_VVSOrthoCheckbox;
	CEdit	m_VVSNearEdit;
	CEdit	m_VVSLeftEdit;
	CEdit	m_VVSFarEdit;
	CEdit	m_VVSBottomEdit;
	CEdit	m_LAUpZEdit;
	CEdit	m_LAUpYEdit;
	CEdit	m_LAUpXEdit;
	CButton	m_LAUseItCheckbox;
	CButton	m_LAIgnoreCheckbox;
	CEdit	m_LAEyeZEdit;
	CEdit	m_LAEyeYEdit;
	CEdit	m_LAEyeXEdit;
	CEdit	m_LACenterZEdit;
	CEdit	m_LACenterYEdit;
	CEdit	m_LACenterXEdit;
	CButton	m_OKButton;
	CButton	m_ApplyButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CProjectionPage)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CProjectionPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplybutton();
	afx_msg void OnCancelbutton();
	afx_msg void OnOkbutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// tracpage.h : implementation file
//
// CTracePage dialog
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


class OIFIILIB_API CTracePage : public COWPropertyPage
{
	DECLARE_DYNCREATE(CTracePage)

// Construction
public:
	class CTraceSettings* m_pTraceSettings;
	virtual BOOL UpdateSettingsFromDocToSheet();
	virtual BOOL UpdateSettingsFromSheetToDoc();

	CTracePage();
	~CTracePage();

// Dialog Data
	//{{AFX_DATA(CTracePage)
	enum { IDD = IDD_TRACESETTINGS };
	CComboBox	m_TLAStat4Combo;
	CComboBox	m_TLAStat3Combo;
	CComboBox	m_TLAStat1Combo;
	CButton	m_TLAStat4Checkbox;
	CButton	m_TLAStat3Checkbox;
	CButton	m_TLAStat2Checkbox;
	CButton	m_TLAStat1Checkbox;
	CButton	m_TPVoronoisetCheckbox;
	CButton	m_TPTrianglesetCheckbox;
	CButton	m_TPSegmentsetCheckbox;
	CButton	m_TPPointsetCheckbox;
	CButton	m_TPPixelsetCheckbox;
	CButton	m_OKButton;
	CButton	m_ApplyButton;
	CString	m_TLAStat2Combo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTracePage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTracePage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplybutton();
	afx_msg void OnCancelbutton();
	afx_msg void OnOkbutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// tranpage.h : implementation file
//
// CTransformationPage dialog
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


class OIFIILIB_API CTransformationPage : public COWPropertyPage
{
	DECLARE_DYNCREATE(CTransformationPage)

// Construction
public:
	class CTransformationSettings* m_pTransformationSettings;
	virtual BOOL UpdateSettingsFromDocToSheet();
	virtual BOOL UpdateSettingsFromSheetToDoc();

	CTransformationPage();
	~CTransformationPage();

// Dialog Data
	//{{AFX_DATA(CTransformationPage)
	enum { IDD = IDD_TRANSFORMATIONSETTINGS };
	CComboBox	m_DefaultsComboBox;
	CEdit	m_VTYEdit;
	CEdit	m_VTXEdit;
	CEdit	m_VTWidthEdit;
	CEdit	m_VTHeightEdit;
	CButton	m_MTUseItCheckbox;
	CEdit	m_MTTranslateZEdit;
	CEdit	m_MTTranslateYEdit;
	CEdit	m_MTTranslateXEdit;
	CEdit	m_MTScaleZEdit;
	CEdit	m_MTScaleYEdit;
	CEdit	m_MTScaleXEdit;
	CEdit	m_MTRotateZEdit;
	CEdit	m_MTRotateYEdit;
	CEdit	m_MTRotateXEdit;
	CButton	m_MTRelativeCheckbox;
	CButton	m_MTIgnoreCheckbox;
	CButton	m_MTAbsoluteCheckbox;
	CButton	m_CancelButton;
	CButton	m_OKButton;
	CButton	m_ApplyButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTransformationPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetDefaultComboboxToCUSTOM();

	// Generated message map functions
	//{{AFX_MSG(CTransformationPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplybutton();
	afx_msg void OnCancelbutton();
	afx_msg void OnOkbutton();
	afx_msg void OnSelchangeDefaultscombo();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusMtrotatexedit();
	afx_msg void OnSetfocusMtrotatexedit();
	afx_msg void OnSetfocusMtrotateyedit();
	afx_msg void OnSetfocusMtrotatezedit();
	afx_msg void OnSetfocusMtscalexedit();
	afx_msg void OnSetfocusMtscaleyedit();
	afx_msg void OnSetfocusMtscalezedit();
	afx_msg void OnSetfocusMttranslatexedit();
	afx_msg void OnSetfocusMttranslateyedit();
	afx_msg void OnSetfocusMttranslatezedit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};




/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// viewpage.h : implementation file
//
// CViewDataPage dialog
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


class OIFIILIB_API CViewDataPage : public COWPropertyPage
{
	DECLARE_DYNCREATE(CViewDataPage)

// Construction
public:
	CViewDataPage();
	~CViewDataPage();

	class CViewDataSettings* m_pViewDataSettings;
	virtual BOOL UpdateSettingsFromDocToSheet();
	virtual BOOL UpdateSettingsFromSheetToDoc();


// Dialog Data
	//{{AFX_DATA(CViewDataPage)
	enum { IDD = IDD_VIEWDATASETTINGS };
	CButton	m_ClasssetLabelsButton;
	CButton	m_OKButton;
	CButton	m_ApplyButton;
	CButton	m_TrianglesetCentersButton;
	CButton	m_VoronoisetButton;
	CEdit	m_VoronoisetEdit;
	CEdit	m_ViewChannelEdit;
	CComboBox	m_ViewChannelComboBox;
	CButton	m_TrianglesetButton;
	CButton	m_TrianglesetLabelsButton;
	CEdit	m_TrianglesetEdit;
	CButton	m_SegmentsetButton;
	CButton	m_SegmentsetLabelsButton;
	CEdit	m_SegmentsetEdit;
	CButton	m_PointsetLabelsButton;
	CButton	m_PointsetButton;
	CEdit	m_PointsetEdit;
	CEdit	m_PixelsetEdit;
	CButton	m_PixelsetButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CViewDataPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CViewDataPage)
	afx_msg void OnCancelbutton();
	afx_msg void OnOkbutton();
	afx_msg void OnApplybutton();
	afx_msg void OnChangePixelsetedit();
	afx_msg void OnPixelsetstatic();
	afx_msg void OnChangePointsetedit();
	afx_msg void OnPointsetstatic();
	afx_msg void OnChangeSegmentsetedit();
	afx_msg void OnSegmentsetstatic();
	afx_msg void OnChangeTrianglesetedit();
	afx_msg void OnTrianglesetstatic();
	afx_msg void OnSelchangeViewchannelcombo();
	afx_msg void OnChangeViewchanneledit();
	afx_msg void OnChangeVoronoisetedit();
	afx_msg void OnVoronoisetstatic();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTrianglesetcentersstatic();
	afx_msg void OnClasssetlabels();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};







/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// profpage.h : implementation file
//
// CProfilePage dialog
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


class OIFIILIB_API CProfilePage : public COWPropertyPage
{
	DECLARE_DYNCREATE(CProfilePage)

// Construction
public:

	class CProfileSettings* m_pProfileSettings;
	virtual BOOL UpdateSettingsFromDocToSheet();
	virtual BOOL UpdateSettingsFromSheetToDoc();
	virtual BOOL UpdateGraph() { return UpdateProfile(); }
	BOOL UpdateProfile();
	BOOL FillComboBoxWithPointCollectionNames(CComboBox* pComboBox);

	CProfilePage();
	~CProfilePage();
private:
	class CProfileGraph* m_pProfileGraph;
	int m_idROI;
	int m_idVertexAtMouseLocation;
	int m_idSegmentAtMouseLocation;
	BOOL bBusyUpdating;
public:
// Dialog Data
	//{{AFX_DATA(CProfilePage)
	enum { IDD = IDD_PROFILESETTINGS };
	CButton	m_ROICheck;
	CComboBox	m_PointCollectionCombo;
	CButton	m_AutoScaleCheck;
	CButton	m_FitScaleCheck;
	CButton	m_OKButton;
	CButton	m_ProfileFrameStatic;
	CEdit	m_NeighborhoodEdit;
	CComboBox	m_StatChannelComboBox;
	CButton	m_LocalNeighborhoodCheckBox;
	CButton	m_ImagePixelsCheckBox;
	CButton	m_ImageFeaturesCheckBox;
	CButton	m_GlobalCheckBox;
	CEdit	m_GivenSegmentEdit;
	CButton	m_GivenSegmentCheckBox;
	CButton	m_ComboStatic;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CProfilePage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
 	LONG OnMouseMovedUpdateLocalGraph(struct tagCONTEXTINFO*);
protected:

	BOOL FillComboBoxWithFusionInfoChannelNames();
	void Apply();
	void ScaleFix();

	// Generated message map functions
	//{{AFX_MSG(CProfilePage)
	afx_msg void OnCancelbutton();
	afx_msg void OnOkbutton();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangeProfilechannelcombo();
	afx_msg void OnGivensegmentcheck();
	afx_msg void OnGlobalcheck();
	afx_msg void OnImagefeaturescheck();
	afx_msg void OnImagepixelscheck();
	afx_msg void OnLocalneighborhoodcheck();
	afx_msg void OnSetfocusNeighborhoodedit();
	afx_msg void OnChangeNeighborhoodedit();
	afx_msg void OnChangeGivensegmentedit();
	afx_msg void OnSetfocusGivensegmentedit();
	afx_msg void OnScaleAuto();
	afx_msg void OnScaleFit();
	afx_msg void OnLocalROIcheck();
	afx_msg void OnSelchangeLocalpointcollectioncombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};






/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// histopage.h : header file
//
// CHistoPage dialog
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


class OIFIILIB_API CHistoPage : public COWPropertyPage
{
	DECLARE_DYNCREATE(CHistoPage)

// Construction
public:
	class CHistoSettings* m_pHistoSettings;
	virtual BOOL UpdateSettingsFromDocToSheet();
	virtual BOOL UpdateSettingsFromSheetToDoc();
	virtual BOOL UpdateGraph() { return UpdateHisto(); }
	BOOL UpdateHisto();
	BOOL FillComboBoxWithPointCollectionNames(CComboBox* pComboBox);

	CHistoPage();
	~CHistoPage();
private:
	class CHistogram* m_pHistogram;
	int m_idROI; //region of interest, that's the user moveable selection rectangle
	int m_idVertexAtMouseLocation;
	int m_idSegmentAtMouseLocation;

	BOOL m_bBusyUpdating;
	void Apply();
	void ScaleFix();

public:

// Dialog Data
	//{{AFX_DATA(CHistoPage)
	enum { IDD = IDD_HISTOSETTINGS };
	CButton	m_ROICheck;
	CComboBox	m_PointCollectionCombo;
	CButton	m_AutoScaleCheck;
	CButton	m_FitScaleCheck;
	CButton	m_OKButton;
	CEdit	m_GivenSegmentEdit;
	CButton	m_ImageFeaturesCheckBox;
	CButton	m_ImagePixelsCheckBox;
	CButton	m_LocalNeighborhoodCheckBox;
	CButton	m_GlobalCheckBox;
	CButton	m_GivenSegmentCheckBox;
	CButton	m_HistoFrameStaticButton;
	CEdit	m_NeighborhoodEdit;
	CComboBox	m_StatChannelComboBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CHistoPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
 	LONG OnMouseMovedUpdateLocalGraph(struct tagCONTEXTINFO*);
	// Generated message map functions
	//{{AFX_MSG(CHistoPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCancelbutton();
	afx_msg void OnOkbutton();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLocalstatchannelcombo();
	afx_msg void OnGivensegmentcheck();
	afx_msg void OnSetfocusGivensegmentedit();
	afx_msg void OnChangeGivensegmentedit();
	afx_msg void OnGlobalcheck();
	afx_msg void OnLocalneighborhoodcheck();
	afx_msg void OnChangeNeighborhoodedit();
	afx_msg void OnSetfocusNeighborhoodedit();
	afx_msg void OnImagepixelscheck();
	afx_msg void OnImageFeaturescheck();
	afx_msg void OnScaleAuto();
	afx_msg void OnScaleFit();
	afx_msg void OnLocalROIcheck();
	afx_msg void OnSelchangeLocalpointcollectioncombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};



/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// scatterpage.h : implementation file
//
// CScatterPage dialog
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


class OIFIILIB_API CScatterPage : public COWPropertyPage
{
	DECLARE_DYNCREATE(CScatterPage)

// Construction
public:
	class CScatterSettings* m_pScatterSettings;
	virtual BOOL UpdateSettingsFromDocToSheet();
	virtual BOOL UpdateSettingsFromSheetToDoc();
	virtual BOOL UpdateGraph() { return UpdateScatter(); }
	BOOL UpdateScatter();
	BOOL FillComboBoxWithPointCollectionNames(CComboBox* pComboBox);

	CScatterPage();
	~CScatterPage();
private:
	class CScatterGraph* m_pScatterGraph;
	int m_idROI;
	int m_idVertexAtMouseLocation;
	int m_idSegmentAtMouseLocation;
	BOOL m_bBusyUpdating;
	void Apply(); 
	void ScaleFix();

public:

// Dialog Data
	//{{AFX_DATA(CScatterPage)
	enum { IDD = IDD_SCATTERSETTINGS };
	CButton	m_ROICheck;
	CComboBox	m_PointCollectionCombo;
	CButton	m_ImageFeaturesCheckBox;
	CButton	m_ImagePixelsCheckBox;
	CButton	m_FitScaleCheck;
	CButton	m_AutoScaleCheck;
	CComboBox	m_StatChannelCombo2;
	CComboBox	m_StatChannelCombo1;
	CEdit	m_NeighborhoodEdit;
	CButton	m_LocalNeighborhoodCheckBox;
	CButton	m_GlobalCheckBox;
	CEdit	m_GivenSegmentEdit;
	CButton	m_GivenSegmentCheckBox;
	CButton	m_HistoFrameStaticButton;
	CButton	m_OKButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CScatterPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
 	LONG OnMouseMovedUpdateLocalGraph(struct tagCONTEXTINFO*);
protected:
	// Generated message map functions
	//{{AFX_MSG(CScatterPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCancelbutton();
	afx_msg void OnOkbutton();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeScatterchannelcombo1();
	afx_msg void OnSelchangeScatterchannelcombo2();
	afx_msg void OnGivensegmentcheck();
	afx_msg void OnChangeGivensegmentedit();
	afx_msg void OnSetfocusGivensegmentedit();
	afx_msg void OnGlobalcheck();
	afx_msg void OnLocalneighborhoodcheck();
	afx_msg void OnChangeNeighborhoodedit();
	afx_msg void OnSetfocusNeighborhoodedit();
	afx_msg void OnScaleAuto();
	afx_msg void OnScaleFit();
	afx_msg void OnImagepixelscheck();
	afx_msg void OnImageFeaturescheck();
	afx_msg void OnLocalROIcheck();
	afx_msg void OnSelchangeLocalpointcollectioncombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


#endif// _OIFII_PROPPAGE_H

