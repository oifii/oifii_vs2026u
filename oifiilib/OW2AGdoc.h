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

///////////////////////////////////////////////////////////////////////////////
// OW2AGdoc.h : interface of the COW2AGDoc class
//
// OW2 Audio GUI document is represented as a grid with the OW2 Audio GUI view.
// The number of row correspond to the number of wav file droped onto the view.
// The number of col correspond to the temporal resolution of the view, i.e. 8.
//
// The OW2 Audio GUI library present an intuitive voronoi-based resizable GUI.
//
// The OW2AGView class provides continuous control functionality through voronoi 
// regions being floodfillable (from 0% to 100%) by user.
//
// Concept, design and development by Stephane Poirier, OIFII.ORG 2012.
//
// stephane.poirier@oifii.org
///////////////////////////////////////////////////////////////////////////////

#ifndef _OW2AGDOC_H
#define _OW2AGDOC_H



// the following added with POINTSET_EXTRA_NUMBEROFSTAT should give the actual offset in the statistics 
#define OW2AGDOC_XYZI_INTENSITY		0
#define OW2AGDOC_XYZI_DIMX			1
#define OW2AGDOC_XYZI_DIMY			2
#define OW2AGDOC_XYZI_LEVEL			3

#define OW2AGDOC_CONTROL_TOTALNUMBEROFCONTROL		6
#define OW2AGDOC_CONTROL_IDPOINTOFFSET_VOLUME				0
#define OW2AGDOC_CONTROL_IDPOINTOFFSET_TEMPO				1
#define OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGNUMERATOR		2
#define OW2AGDOC_CONTROL_IDPOINTOFFSET_TIMESIGDENOMINATOR	3
#define OW2AGDOC_CONTROL_IDPOINTOFFSET_MEASURESINDISPLAY	4
#define OW2AGDOC_CONTROL_IDPOINTOFFSET_NUMBEROFSTEPSPERBEAT	5

#define OW2AGDOC_CONTROL_VOLUME_MIN					0.0
#define OW2AGDOC_CONTROL_VOLUME_MAX					100.0
#define OW2AGDOC_CONTROL_VOLUME_DEFAULT				100.0

#define OW2AGDOC_CONTROL_TEMPO_MIN					10.0
#define OW2AGDOC_CONTROL_TEMPO_MAX					1000.0
#define OW2AGDOC_CONTROL_TEMPO_DEFAULT				120.0

#define OW2AGDOC_CONTROL_TIMESIGNUMERATOR_MIN			1
#define OW2AGDOC_CONTROL_TIMESIGNUMERATOR_MAX			8
#define OW2AGDOC_CONTROL_TIMESIGNUMERATOR_DEFAULT		4

#define OW2AGDOC_CONTROL_TIMESIGDENOMINATOR_MIN			1
#define OW2AGDOC_CONTROL_TIMESIGDENOMINATOR_MAX			6
#define OW2AGDOC_CONTROL_TIMESIGDENOMINATOR_DEFAULT		2

#define OW2AGDOC_CONTROL_MEASURESINDISPLAY_MIN			1
#define OW2AGDOC_CONTROL_MEASURESINDISPLAY_MAX			8
#define OW2AGDOC_CONTROL_MEASURESINDISPLAY_DEFAULT		4

#define OW2AGDOC_CONTROL_NUMBEROFSTEPSPERBEAT_MIN			1
#define OW2AGDOC_CONTROL_NUMBEROFSTEPSPERBEAT_MAX			8
#define OW2AGDOC_CONTROL_NUMBEROFSTEPSPERBEAT_DEFAULT		1

#define OW2AGDOC_CONTROL_RECTANGULARAREASIZE_INPERCENT		20


class OIFIILIB_API COW2AGDoc : public COW2Doc
{
public: // create from serialization only
	COW2AGDoc();
	DECLARE_DYNCREATE(COW2AGDoc)

	virtual ~COW2AGDoc();
// Attributes
public:
	class CPtrList* m_pWavsetList;
	int m_idpointstart;
	int m_idpointstartrow;
private:
	int m_nCol;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COW2AGDoc)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetNumberOfRow();
	int GetNumberOfCol();
	void SetNumberOfCol(int nCol);
	BOOL IsSupportedPixelset(const char* filename);
	BOOL ReplacePixelset(const char* filename);
	BOOL IsSupportedWavset(const char* filename);
	BOOL AddWavset(const char* filename);
	class WavSet* GetWavsetFromFilename(const char* filename);
	BOOL AddRow(const char* filename);
	BOOL AddFrameAndGlobalControls(int iflagTINUpdate=POINTSET_TIN_NOUPDATE);
	BOOL AddRow(int nRowBefore, int iflagTINUpdate=POINTSET_TIN_NOUPDATE);
	BOOL RemoveAllLastPointsFrom(int idpointstartrow, int iflagTINUpdate=POINTSET_TIN_NOUPDATE);
	BOOL UpdateRows(int iflagTINUpdate=POINTSET_TIN_UPDATE);
	BOOL UpdateTIN();
	double GetControlUserValue(int idpoint);
	double ComputeControlUserValue(int idpoint, double fRatio);
	double ComputeControlRatio(int idpoint, double dfUserValue);
	void UpdateNumberOfColFromControls();
// Generated message map functions
public:
	//{{AFX_MSG(COW2AGDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(COW2AGDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
};

#endif //_OW2AGDOC_H