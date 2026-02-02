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

// gotodlg.cpp : implementation file
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

#include "OWdoc.h"	//doc
#include "OW2doc.h"
#include "OW2mdi.h" //frame
#include "OWview.h" //view
#include "OW2view.h"

#include "oifii_gotodlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGotoPartitionDlg dialog

//since there is always only one instance and since we had strange memory access fault
/*
int m_iMinNumSeg;
int m_iMaxNumSeg;
COW2View* m_pOW2View;
COW2Doc* m_pOW2Doc;
*/

CGotoPartitionDlg::CGotoPartitionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGotoPartitionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGotoPartitionDlg)
	m_FinalNumSegment = _T("");
	m_InitialNumSegment = _T("");
	m_DesiredNumSegment = _T("");
	m_EndingNumSegment = _T("");
	m_StartingNumSegment = _T("");
	//}}AFX_DATA_INIT

	m_pOW2View = NULL;
	m_pOW2Doc = NULL;
	m_iMinNumSeg = 0;
	m_iMaxNumSeg = 0;

	//poirier dec 98, added for classification
	m_bIsForClassification = FALSE;
}


void CGotoPartitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGotoPartitionDlg)
	DDX_Control(pDX, IDSTOP, m_StopButton);
	DDX_Control(pDX, IDPLAY, m_PlayButton);
	DDX_Control(pDX, IDGOTO, m_GotoButton);
	DDX_Control(pDX, IDGOTONEXT, m_GotoNextButton);
	DDX_Control(pDX, ID_TOSTART_EDIT, m_ToStartEdit);
	DDX_Control(pDX, ID_TOEND_EDIT, m_ToEndEdit);
	DDX_Control(pDX, ID_TODISPLAY_EDIT, m_ToDisplayEdit);
	DDX_Text(pDX, ID_FINAL_EDIT, m_FinalNumSegment);
	DDX_Text(pDX, ID_INITIAL_EDIT, m_InitialNumSegment);
	DDX_Text(pDX, ID_TODISPLAY_EDIT, m_DesiredNumSegment);
	DDX_Text(pDX, ID_TOEND_EDIT, m_EndingNumSegment);
	DDX_Text(pDX, ID_TOSTART_EDIT, m_StartingNumSegment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGotoPartitionDlg, CDialog)
	//{{AFX_MSG_MAP(CGotoPartitionDlg)
	ON_EN_KILLFOCUS(ID_TODISPLAY_EDIT, OnKillfocusTodisplayEdit)
	ON_BN_CLICKED(IDCLOSE, OnClose)
	ON_BN_CLICKED(IDGOTO, OnGoto)
	ON_BN_CLICKED(IDPLAY, OnPlay)
	ON_BN_CLICKED(IDSTOP, OnStop)
	ON_BN_CLICKED(IDGOTONEXT, OnGotoNext)
	ON_MESSAGE(OWM_GOTODLG_UPDATEINFO, OnUpdateDlgInfo) //offworld defined message
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGotoPartitionDlg message handlers

void CGotoPartitionDlg::OnKillfocusTodisplayEdit() 
{
	//validate
	/*
	int iMin = atoi(m_FinalNumSegment);
	int iMax = atoi(m_InitialNumSegment);
	int iDesired = atoi(m_ToDisplayEdit.GetWindowText());
	if(iDesired<iMin || iDesired>iMax)
	{
		//invalid user entry
		//m_ToDisplayEdit.SetWindowText("");
		m_ToDisplayEdit.SetFocus();
	}
	*/
}


void CGotoPartitionDlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
	//DestroyWindow(); //dlg will be destroyed by the app
}

void CGotoPartitionDlg::OnGoto() 
{
	UpdateData(TRUE);
	int inumberofsegforfinalpartition = _wtoi(m_DesiredNumSegment);
	if(inumberofsegforfinalpartition<m_iMinNumSeg) inumberofsegforfinalpartition = m_iMinNumSeg;
	if(inumberofsegforfinalpartition>m_iMaxNumSeg) inumberofsegforfinalpartition = m_iMaxNumSeg;
	if(m_bIsForClassification==FALSE)
	{
		m_pOW2View->m_iNumberOfSegmentForFinalPartition = inumberofsegforfinalpartition;
	}
	else
	{
		//poirier dec 98, added for classification
		m_pOW2View->m_iNumberOfSegmentForFinalClassification = inumberofsegforfinalpartition;
	}
	m_pOW2View->Invalidate();
	
}

void CGotoPartitionDlg::OnGotoNext() 
{
	int iOldNumSeg = _wtoi(m_DesiredNumSegment);
	int iNewNumSeg = iOldNumSeg-1;
	if(iNewNumSeg<m_iMinNumSeg)
	{
		//cannot go futher, do nothing and return
		return;
	}
	else
	{
		//update edit box and data member
		m_DesiredNumSegment.Format(L"%d", iNewNumSeg);
		if(m_bIsForClassification==FALSE) m_pOW2View->m_iNumberOfSegmentForFinalPartition = _wtoi(m_DesiredNumSegment);
			else m_pOW2View->m_iNumberOfSegmentForFinalClassification = _wtoi(m_DesiredNumSegment);
	}
	UpdateData(FALSE);
	CDC* pDC = m_pOW2View->GetDC();
	m_pOW2View->UpdatePartition(iOldNumSeg, iNewNumSeg);
	return;
}


void CGotoPartitionDlg::OnPlay() 
{
	// TODO: Add your control notification handler code here
}

void CGotoPartitionDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	
}

//spi, nov 2018, begin
//LONG CGotoPartitionDlg::OnUpdateDlgInfo(WPARAM wParam, LPARAM lParam)
LRESULT CGotoPartitionDlg::OnUpdateDlgInfo(WPARAM wParam, LPARAM lParam)
//spi, nov 2018, end
{
	//1) capture active mdi child view and doc
	CMDIFrameWnd* pMDIParentFrameWnd = (CMDIFrameWnd*) AfxGetApp()->m_pMainWnd;
	CFrameWnd* pFrameWnd = pMDIParentFrameWnd->GetActiveFrame();
	//1.1) is the active child of type COW2MDI ?
	BOOL bIsOW2MDI = pFrameWnd->IsKindOf(RUNTIME_CLASS(COW2MDI));
	BOOL bArePartitionsComputed = FALSE;
	SEGMENTSET* pSegmentset = NULL;

	if(bIsOW2MDI==TRUE)
	{
		CView* pView = pFrameWnd->GetActiveView();
		COW2View* pOW2View = (COW2View*) pView;
		m_pOW2View = pOW2View;
		if(m_pOW2View==NULL)
		{
			bArePartitionsComputed = FALSE;
		}
		else
		{ 
			m_pOW2Doc = (COW2Doc*) m_pOW2View->GetDocument();	
			if(m_pOW2Doc==NULL)
			{
				bArePartitionsComputed = FALSE;
			}
			else
			{
				if(m_bIsForClassification==FALSE) pSegmentset = m_pOW2Doc->m_pSegmentset;
					else pSegmentset = m_pOW2Doc->m_pClassset->GetSegmentsetPointer();
				//for segmentation
				if(pSegmentset==NULL)
				{
					bArePartitionsComputed = FALSE;
				}
				else
				{
					bArePartitionsComputed = TRUE;
				}
			}
		}
	}

	//1.1) if not COW2MDI child or partitions not computed
	if(bIsOW2MDI==FALSE || bArePartitionsComputed==FALSE)
	{
		//empty fields
		m_InitialNumSegment	= "";
		m_FinalNumSegment = "";
		m_StartingNumSegment = "";
		m_DesiredNumSegment	= "";
		m_EndingNumSegment = "";
		//disable edit boxes and buttons
		m_ToStartEdit.EnableWindow(FALSE);
		m_ToEndEdit.EnableWindow(FALSE);
		m_PlayButton.EnableWindow(FALSE);
		m_StopButton.EnableWindow(FALSE);
		m_GotoButton.EnableWindow(FALSE);
	   	//
		//update dlg data
		UpdateData(FALSE);

		return TRUE;
	}
	//view and doc should be valid, partition should be computed
	else if(bIsOW2MDI==TRUE || bArePartitionsComputed==TRUE)
	{
		//set number of computed segment according to document
		char pBuf[50];
		m_iMaxNumSeg = pSegmentset->iInitialNumberOfSegment;
		wsprintfA(pBuf, "%i", m_iMaxNumSeg);
		m_InitialNumSegment = pBuf;
		m_iMinNumSeg = 2 * m_iMaxNumSeg - pSegmentset->iFinalNumberOfSegment;
		if(m_iMinNumSeg<1)
		{
			ASSERT(FALSE);
			m_iMinNumSeg = 1; //keep going safely, but dev-time error
		}
		wsprintfA(pBuf, "%i", m_iMinNumSeg);
		m_FinalNumSegment = pBuf;

		//set default starting, desired and ending number of segment 
	 	m_StartingNumSegment = m_InitialNumSegment;
		int iActualNumSegment;
		if(m_bIsForClassification==FALSE) iActualNumSegment= m_pOW2View->m_iNumberOfSegmentForFinalPartition;
			else iActualNumSegment= m_pOW2View->m_iNumberOfSegmentForFinalClassification;
		if(iActualNumSegment>m_iMaxNumSeg || iActualNumSegment<m_iMinNumSeg)
		{
			m_DesiredNumSegment = m_InitialNumSegment;	
		}
		else
		{
			wsprintfA(pBuf, "%i", iActualNumSegment);
			m_DesiredNumSegment = pBuf;	
		}
		ASSERT((m_iMinNumSeg+10)<m_iMaxNumSeg);
		wsprintfA(pBuf, "%i", m_iMinNumSeg+10);
		m_EndingNumSegment = pBuf;
		
		//update dlg data
		UpdateData(FALSE);

#ifdef _DEBUG
		//enable edit boxes and buttons
		m_ToStartEdit.EnableWindow(TRUE);
		m_ToEndEdit.EnableWindow(TRUE);
		m_PlayButton.EnableWindow(TRUE);
		m_StopButton.EnableWindow(TRUE);
		m_GotoButton.EnableWindow(TRUE);
#else
	//disable features not fully ready for release version
		m_ToStartEdit.EnableWindow(FALSE);
		m_ToEndEdit.EnableWindow(TRUE);
		m_PlayButton.EnableWindow(FALSE);
		m_StopButton.EnableWindow(FALSE);
		m_GotoButton.EnableWindow(TRUE);

#endif //_DEBUG
	}
	else
	{
		ASSERT(FALSE); //development-time error
	}
	return TRUE;
}


BOOL CGotoPartitionDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
    //1) compute initial dlg size and position
    CMDIFrameWnd* pMDIParentFrameWnd = (CMDIFrameWnd*) AfxGetApp()->m_pMainWnd;
    CRect myRect;
    pMDIParentFrameWnd->GetClientRect(&myRect);
    cs.x = myRect.left;
    if( cs.x < 0 ) cs.x = 0;
    // get the height of the title bar
	int ntitlebarheight = GetSystemMetrics( SM_CYCAPTION ) - GetSystemMetrics( SM_CYBORDER ) + 5;
    cs.y = myRect.bottom - cs.cy - 2*ntitlebarheight;
    if( cs.y < 0 ) cs.y = 0;

	
	return CDialog::PreCreateWindow(cs);
}

int CGotoPartitionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    //1) compute initial dlg size and position
    CMDIFrameWnd* pMDIParentFrameWnd = (CMDIFrameWnd*) AfxGetApp()->m_pMainWnd;
    CRect myRect;
    pMDIParentFrameWnd->GetClientRect(&myRect);
    lpCreateStruct->x = myRect.left;
    if( lpCreateStruct->x < 0 ) lpCreateStruct->x = 0;
    // get the height of the title bar
	int ntitlebarheight = GetSystemMetrics( SM_CYCAPTION ) - GetSystemMetrics( SM_CYBORDER ) + 5;
    lpCreateStruct->y = myRect.bottom - lpCreateStruct->cy - 2*ntitlebarheight;
    if( lpCreateStruct->y < 0 ) lpCreateStruct->y = 0;


	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

BOOL CGotoPartitionDlg::OnInitDialog() 
{
	
	CDialog::OnInitDialog(); //calls UpdateData(TRUE)
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
