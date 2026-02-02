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

//this class is dependent of MFC, app. global and the definition of itself         
#include "stdafx.h"
#include "oifiilib.h" //spi 2014

#include "gl_owgraph.h"
//#include "graphdoc.h"
//#include "graphset.h"
//#include "private.h" 

#include "limits.h"
#include "float.h"

                            
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



////////////////////////////////
// Macros private to this module
////////////////////////////////

#define MAX_DUMMY_STRING_SIZE                   256   //when using dummy string buffer
#define MAX_GRAPHSET_SIZE 						10000 //maximum number of values in one graphset, when pasting data to document
#define TABBEDTEXTBUFFER_INITIAL_BLOCKSIZE 		50000 //used by function GenerateTabbedTextBuffer()
#define TABBEDTEXTBUFFER_ADDITIONAL_BLOCKSIZE 	50000 //used by function GenerateTabbedTextBuffer()







/////////////////////////////////////////////////////////////////////////////
// COWGraphDoc

IMPLEMENT_SERIAL(COWGraphDoc, CDocument, 0 /* schema number*/ )

BEGIN_MESSAGE_MAP(COWGraphDoc, CDocument)
	//{{AFX_MSG_MAP(COWGraphDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here. 
		ON_COMMAND(ID_FILE_SAVE, OnFileSave)
		
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
               
// Using this constructor will result in a empty GraphView, then user would be
// able to construct its graph data at run-time                
COWGraphDoc::COWGraphDoc()
{    
    m_numberofvisiblesets = 0;  
    
    //depending upon autorescaling mode, the following variables are maintained or not
	m_autorescaling = TRUE;
    m_maxnumberofpointsperset = 0; 	//maximum x value
    m_oldymax = 0; 					//maximum y value
    
    //label when copying data to clipboard
    m_addlabels = TRUE;  
	    
    //default title and axis labels
    CDocument::SetTitle(L"Graph");
    m_XAxisLabelString = "X";
    m_YAxisLabelString = "Y";
    
	m_bMapIndexToFloatDISPLAYFLOAT = FALSE;
	m_dfMapIndexToFloatMINFLOAT = 0.0;
	m_dfMapIndexToFloatMAXFLOAT = 255.0;
	m_iMapIndexToFloatNUMBEROFLEVEL = 256;

	m_lgraphtype = GRAPH_LINE; //GRAPH_VBAR  //GRAPH_LINE	//GRAPH_SCATTER
	m_lviewgrid = GRID_HORZ | GRID_VERT; //GRID_HORZ with GRID_VERT or 0 for none

    //pointer to attached graphic object (ie: OnLineProfile case)
    m_pImageDoc = NULL;

	m_crDefaultBackgroundColor = RGB(255,255,255);
	m_crDefaultTextColor = RGB(0,0,0);

	//used by isera group
	m_iPenWidthForFirstChannelBarTop = 2;

	//standard multi-color table
	m_bUseDefaultMultiColorTable = TRUE;  
	m_bUseDefaultGrayColorTable = FALSE;
	ASSERT(m_bUseDefaultMultiColorTable!=m_bUseDefaultGrayColorTable);

	//will be used with fix scale mode COWGraphDoc::SetAutoScaleFlag(FALSE),
	//in which case COWGraphDoc::FixMaxNumberOfPointPerSet() should be called
	//to fix this variable
	m_PreFixed_maxnumberofpointsperset = -1;
	//will be used with fix scale mode COWGraphDoc::SetAutoScaleFlag(FALSE),
	//in which case COWGraphDoc::FixSCATTERMinMaxXYValue() should be called
	//to fix this variable
	m_PreFixed_SCATTERdfMinValueX = FLT_MAX;
	m_PreFixed_SCATTERdfMaxValueX = FLT_MIN;
	m_PreFixed_SCATTERdfMinValueY = FLT_MAX;
	m_PreFixed_SCATTERdfMaxValueY = FLT_MIN;
}  

void COWGraphDoc::FixMaxNumberOfPointPerSet(int nMaxNumPoint)
{
	m_PreFixed_maxnumberofpointsperset = nMaxNumPoint;
}

void COWGraphDoc::FixPROFILEMinMaxXYValue(double fMinY,
										  double fMaxY)
{
	m_PreFixed_PROFILEdfMinValueY = fMinY;
	m_PreFixed_PROFILEdfMaxValueY = fMaxY;

	//used in vbar and line graph type
	//fMinY ignored in current implementation
	m_oldymax = (int)(fMaxY+0.5);
}

void COWGraphDoc::FixHISTOMinMaxXYValue(double fMinY,
										double fMaxY)
{
	m_PreFixed_HISTOdfMinValueY = fMinY;
	m_PreFixed_HISTOdfMaxValueY = fMaxY;

	//used in vbar and line graph type
	//fMinY ignored in current implementation
	m_oldymax = (int)(fMaxY+0.5);
}

void COWGraphDoc::FixSCATTERMinMaxXYValue(double fMinX, 
										  double fMaxX, 
										  double fMinY,
										  double fMaxY)
{
	//used in scatter graph type
	m_PreFixed_SCATTERdfMinValueX = fMinX;
	m_PreFixed_SCATTERdfMaxValueX = fMaxX;
	m_PreFixed_SCATTERdfMinValueY = fMinY;
	m_PreFixed_SCATTERdfMaxValueY = fMaxY;
}

COWGraphDoc::~COWGraphDoc()
{  
	int indexend = m_GraphSetList.GetSize();                 
	if(indexend>0)
	{
		for(int index=0; index<indexend; index++)
		{
			delete m_GraphSetList.GetAt(index);
		}
	}
	
	/* since integration into oiii, steph, put it back if needed
	//when line profile graph is attached to an image
	if (m_pImageDoc)
	{
		// get the first view in the list of views:
		POSITION pos = m_pImageDoc->GetFirstViewPosition();
		CView* pFirstView = GetNextView( pos );
		if (pFirstView != NULL)
			pFirstView->SendMessage(OWM_DETACH_GRAPH_DOC, 0, (LPARAM)this);
    }
	*/
}

void COWGraphDoc::MapIndexToFloatValuesOnDisplay(BOOL bEnable)
{
	m_bMapIndexToFloatDISPLAYFLOAT = bEnable;
	if(bEnable==TRUE)
	{
		//default
		m_dfMapIndexToFloatMINFLOAT = 0.0;
		m_dfMapIndexToFloatMAXFLOAT = 256.0;
		m_iMapIndexToFloatNUMBEROFLEVEL = 256;
	}
}

BOOL COWGraphDoc::MapIndexToFloatValuesOnDisplay(double dfMin, double dfMax, int iNumberOfLevel)
{
	m_bMapIndexToFloatDISPLAYFLOAT = TRUE;
	m_dfMapIndexToFloatMINFLOAT = dfMin;
	m_dfMapIndexToFloatMAXFLOAT = dfMax;
	m_iMapIndexToFloatNUMBEROFLEVEL = iNumberOfLevel;
	return TRUE;
}

//this function is called by image document OnLineProfile() member function
void COWGraphDoc::AttachImageDoc(CDocument* pImageDoc)
{      
	m_pImageDoc = pImageDoc;
	return;
}

//this function is called by Grey or ColImage::OnLineProfile()
void COWGraphDoc::DetachImageDoc()
{   
	m_pImageDoc = NULL;   
	return;
}



   
//remember: OnNewDocument is called after constructors of CDoc, CMDIChildWnd and CView 
//			are called.  Therefore at this stage the COWGraphView is 
//			already created and initialized.
BOOL COWGraphDoc::OnNewDocument()
{                                                 
	if (!CDocument::OnNewDocument()) return FALSE;
	return TRUE;
}

void COWGraphDoc::SetGraphType(int iGraphType)
{
	m_lgraphtype = iGraphType;
}

//you can combine GRID_HORZ with GRID_VERT or 0 for none
void COWGraphDoc::SetViewGrid(int iFlag)
{
	m_lviewgrid = iFlag;
}

      
void COWGraphDoc::AddGraphSet(int far* lp_inputlongarray, 
							int ldatasize,
							LPCSTR psz_channelname, 
							int lchannelid, 
							BOOL bVisible)
{                 
	COWGraphSet* p_currentGraphSet = (COWGraphSet*) new COWGraphSet(lp_inputlongarray, 
															  ldatasize, 
															  psz_channelname,
															  lchannelid);    
	//if newly added graphset is the largest 
	if( (p_currentGraphSet->GetDataSize() > m_maxnumberofpointsperset)
	     && (m_autorescaling==TRUE) )
	{  
		m_maxnumberofpointsperset = (int) p_currentGraphSet->GetDataSize();
	}	
	else if(m_autorescaling==FALSE && m_PreFixed_maxnumberofpointsperset!=-1)
	{
		ASSERT(m_PreFixed_maxnumberofpointsperset!=-1); 
		//note: dev. time error, prior to adding graphset with fix scale mode, you should always 
		//      call the function COWGraphDoc::FixMaxNumberOfPointPerSet()
		m_maxnumberofpointsperset = m_PreFixed_maxnumberofpointsperset;
	}

	//Add the new graphset                                                          
	m_GraphSetList.Add( p_currentGraphSet );
	if(bVisible==FALSE) p_currentGraphSet->SetVisibility(FALSE);
	
	//update graph list info
	if(p_currentGraphSet->IsVisible()) m_numberofvisiblesets++;
	
	//SetModifiedFlag(TRUE);
	return;
}
      
          
          
void COWGraphDoc::AddGraphSet(	double far* lp_inputfloatarray, 
								int ldatasize,
								LPCSTR psz_channelname, 
								int lchannelid,
								BOOL bVisible)
{   
	COWGraphSet* p_currentGraphSet = (COWGraphSet*) new COWGraphSet(lp_inputfloatarray, 
															  ldatasize,
															  psz_channelname,
															  lchannelid);    
	//if newly added graphset is the largest 
	if( (p_currentGraphSet->GetDataSize() > m_maxnumberofpointsperset)
	     && (m_autorescaling==TRUE) )
	
	{  
		m_maxnumberofpointsperset = (int) p_currentGraphSet->GetDataSize();
	}	
	else if(m_autorescaling==FALSE && m_PreFixed_maxnumberofpointsperset!=-1)
	{
		ASSERT(m_PreFixed_maxnumberofpointsperset!=-1); 
		//note: dev. time error, prior to adding graphset with fix scale mode, you should always 
		//      call the function COWGraphDoc::FixMaxNumberOfPointPerSet()
		m_maxnumberofpointsperset = m_PreFixed_maxnumberofpointsperset;
	}
    
    //Add the new set
	m_GraphSetList.Add(p_currentGraphSet);
	if(bVisible==FALSE) p_currentGraphSet->SetVisibility(FALSE);

	//update graph list info
	if(p_currentGraphSet->IsVisible()) m_numberofvisiblesets++;
	
	//SetModifiedFlag(TRUE);
	return;
}
   
	//it copies the input double float data (that are interlaced) 
	//as a contiguous float vector in the created graphset. 
	//warning, this function also allocates data, like the other version 
	//of AddGraphSet(), but unlike the CGraphSet constructor that 
	//initializes with double that does not allocate any additional memory. 
void COWGraphDoc::AddGraphSet(	double* lp_dfBasePointer,
								int lOffsetBetweenTwoConsecutiveValues,
								int lchannelsize, 
								LPCSTR psz_channelname/*=NULL*/, 
								int lchannelid/*=0*/,
								BOOL bReallocateAsFloatArray/*=FALSE*/)
{
	COWGraphSet* p_currentGraphSet = (COWGraphSet*) new COWGraphSet(lp_dfBasePointer, 
																	lOffsetBetweenTwoConsecutiveValues,
																	lchannelsize,
																	psz_channelname,
																	lchannelid,
																	bReallocateAsFloatArray);    
	//if newly added graphset is the largest 
	if( (p_currentGraphSet->GetDataSize() > m_maxnumberofpointsperset)
	     && (m_autorescaling==TRUE) )
	
	{  
		m_maxnumberofpointsperset = (int) p_currentGraphSet->GetDataSize();
	}	
	else if(m_autorescaling==FALSE && m_PreFixed_maxnumberofpointsperset!=-1)
	{
		ASSERT(m_PreFixed_maxnumberofpointsperset!=-1); 
		//note: dev. time error, prior to adding graphset with fix scale mode, you should always 
		//      call the function COWGraphDoc::FixMaxNumberOfPointPerSet()
		m_maxnumberofpointsperset = m_PreFixed_maxnumberofpointsperset;
	}
    
    //Add the new set
	m_GraphSetList.Add( p_currentGraphSet );
	
	//update graph list info
	if(p_currentGraphSet->IsVisible()) m_numberofvisiblesets++;
	
	//SetModifiedFlag(TRUE);
	return;
}
   
//Add value is only used with float, for blob statistics display
int COWGraphDoc::AddValueToGraphSet(double fvalue, int lchannelid)
{      
	int nindex = SearchForGraphSetIndex(lchannelid);
	if(nindex >= 0) ((COWGraphSet*)m_GraphSetList.GetAt(nindex))->AddValue(fvalue);
	//SetModifiedFlag(TRUE);
	return nindex; //this function will return positive number on succes 
}



int COWGraphDoc::RemoveGraphSet(int lchannelid)
{
	int nindex = SearchForGraphSetIndex(lchannelid);
	if(nindex >= 0) m_GraphSetList.RemoveAt(nindex, 1);
	//SetModifiedFlag(TRUE);
	return nindex; //this function will return positive number on succes	
}


void COWGraphDoc::RemoveAllGraphSet()
{   
	//get CObArray size
	int indexend = m_GraphSetList.GetSize();
	
	//delete each object in the list
	for(int index=0; index<indexend; index++)
	{
		delete m_GraphSetList.GetAt(index);
	}
	
	//erase all the CObArray
    m_GraphSetList.RemoveAll(); 
    
    //reiniitalize graph document member variables
    m_numberofvisiblesets = 0;  
    m_maxnumberofpointsperset = 0;   
    
    //force the graph view to be redrawn
    UpdateAllViews(NULL);
    return;
}
 
 
void COWGraphDoc::SetAutoRescaling(BOOL bFlag)
{          
	m_autorescaling = bFlag;
	return;	
} 
 
int COWGraphDoc::SearchForGraphSetIndex(int requestedchannelid)
{      
	//it performs a search throught the whole array
	int index=0;
	int indexend = m_GraphSetList.GetSize();
    while(index<indexend)
    {       
    	if( requestedchannelid == ((COWGraphSet*)m_GraphSetList.GetAt(index))->GetChannelID())                      
    	{
    		return index;
    	}
    	index++; 
    }
    return -1; //if not found
}


void COWGraphDoc::ModifyVisibilityOfGraphSet(int index)
{   
	//0) validate entry parameter 
	int indexend = m_GraphSetList.GetSize();
	if(index<0 || index>=indexend) return;
	      
	//1) get the graphset at the specified physical index. 
	COWGraphSet* pCurrentGraphSet = (COWGraphSet*)m_GraphSetList.GetAt(index); 
	
	//2.1) change its visibility, if visible, disable it.  if not visible, enable it.
	//2.2) update graphdoc member variable, m_numberofvisiblesets-- or m_numberofvisiblesets++.
	if(pCurrentGraphSet->IsVisible())
	{
		pCurrentGraphSet->SetVisibility(FALSE);
		m_numberofvisiblesets--;	
	}
	else
	{
		pCurrentGraphSet->SetVisibility(TRUE); 
		m_numberofvisiblesets++;
	}
	return;
}

int COWGraphDoc::GetNumberOfSets()
{
	return m_GraphSetList.GetSize();
}

void COWGraphDoc::GetVisibilityOfAllGraphSets(BOOL* pBOOL, int* nBOOL)
{
	ASSERT(pBOOL!=NULL && nBOOL!=NULL);

	int index=0;
	int indexend = m_GraphSetList.GetSize();
    while(index<indexend)
    {       
    	pBOOL[index] = ((COWGraphSet*)m_GraphSetList.GetAt(index))->IsVisible();                     
    	index++; 
    }

	*nBOOL = index; //number of graphset
	ASSERT(*nBOOL==GetNumberOfSets());
	return;
}

void COWGraphDoc::SetVisibilityOfAllGraphSets(BOOL* pBOOL, int nBOOL)
{
	ASSERT(pBOOL!=NULL);

	BOOL bSkip = FALSE;
	if(nBOOL<GetNumberOfSets())
	{
		bSkip = TRUE;
	}

	int index=0;
	int indexend = m_GraphSetList.GetSize();
    while(index<indexend)
    {       
		if(bSkip)
		{
			//reset
			((COWGraphSet*)m_GraphSetList.GetAt(index))->SetVisibility(TRUE);
		}
		else
		{
    		((COWGraphSet*)m_GraphSetList.GetAt(index))->SetVisibility(pBOOL[index]);                     
		}
    	index++; 
    }
	return;
}
          
double COWGraphDoc::GetYMin()
{      
	//it compares ymins of the whole array
	int indexend = m_GraphSetList.GetSize(); 
	double ymin = FLT_MAX;
	double thismin = 0.0;
    for(int index=0; index<indexend; index++)
    {       
    	thismin = ((COWGraphSet*)m_GraphSetList.GetAt(index))->GetYMin();
    	if(ymin > thismin)                      
    	{    
    		ymin = thismin; 
    	}
    }
    return ymin; 
}

       
double COWGraphDoc::GetYMin(int lchannelid)
{      
	double ymin = FLT_MAX; 
	
	int index = SearchForGraphSetIndex(lchannelid);
	if(index >= 0)
	{
    	ymin = ((COWGraphSet*)m_GraphSetList.GetAt(index))->GetYMin();
	}
    return ymin; 
}
   

double COWGraphDoc::GetYMax()
{      
	//it compares ymax of the whole array
	int indexend = m_GraphSetList.GetSize(); 
	double ymax = FLT_MIN;
	double thismax = 0.0f;
    for(int index=0; index<indexend; index++)
    {       
    	thismax = ((COWGraphSet*)m_GraphSetList.GetAt(index))->GetYMax();
    	if(ymax < thismax)                      
    	{    
    		ymax = thismax; 
    	}
    }
    return ymax; 
}
 

double COWGraphDoc::GetYMax(int lchannelid)
{      
	double ymax = FLT_MIN; 
	
	int index = SearchForGraphSetIndex(lchannelid);
	if(index >= 0) 
	{
    	ymax = ((COWGraphSet*)m_GraphSetList.GetAt(index))->GetYMax();
	}
    return ymax; 
}
   

int COWGraphDoc::GetXMax()
{   
	//it compares xmax of the whole array
	int indexend = m_GraphSetList.GetSize(); 
	int xmax = SHRT_MIN;
	int thismax = 0;
    for(int index=0; index<indexend; index++)
    {       
    	thismax = (int) (((COWGraphSet*)m_GraphSetList.GetAt(index))->GetDataSize());
    	if(xmax < thismax)                      
    	{    
    		xmax = thismax; 
    	}
    }
	return xmax;
} 
   
//
// SetTitle
//
// This function can be used by the image class to set the name of the graph.
void COWGraphDoc::SetTitle(LPCSTR lpszString)
{   
	//modify the WindowTitle, member variable of the DOC.
	CString myString(lpszString);
	CDocument::SetTitle(myString);
	return;
}


//
// This function can be used by the image class to set the x-axis label of the graph.
//
void COWGraphDoc::SetXAxisLabel(LPCSTR lpszString)
{   
	m_XAxisLabelString = lpszString;       
	return;
}

//
// This function can be used by the image class to set the y-axis label of the graph.
//
void COWGraphDoc::SetYAxisLabel(LPCSTR lpszString)
{
	m_YAxisLabelString = lpszString;       
	return;
}

//
//
//
void COWGraphDoc::SetPenWidthForFirstChannelBarTopRendering(int iPenWidth)
{
	m_iPenWidthForFirstChannelBarTop = iPenWidth;
	return;
}



//
//poirier, this is the function that should be called from the application
//         menu when copying datato clipboard.  it is actualy called 
//         indirectly throught CopyGraphSetsToClipboardVerticaly().
//
//
//this is a third version of the function that copies data to clipboard.
//
//
//improvements over CopyVisibleGraphSetsToClipboard:
//
//	- no more restriction on tabbedtextbuffer size, using hpv_string (realloc if required). 
//    (there was a 64k limitation because we were using the lstrcat() winapi function
//     or CString object.  now, we use hmemcpy() to concatenate and GlobalReAlloc() if extra
//     space required).
//
//	- it copies each channel as a column to the clipboard (we add limitation in the
//    previous version because each channel was exported to clipboard as a line, limitation
//    occured when pasting into Excel because spreadsheet linesize cannot exceed 255).
//     
//
//  Note: an easy way of passing 64k text buffer to CEdit control is to use CWnd::SetWindowText().
//        That is the way we have chosen to pass text to CEditView.  There is no size limitation
//        when generating the text buffer, but CEditView and CEdit control are limited to 64k text
//        buffer, to overpass this limitation we would have to manage some kind of paging or wait
//        for Microsoft future MFC release.
//
HGLOBAL COWGraphDoc::GenerateTabbedTextBuffer()     
{
	//copy all visible graph sets data into a tabbedtext string,
	//that is using tab-txt format (values separated by tab, \t, 
	//and set's index separated using return, \r\n.)    
	//
	//
	//	produces following output (examples):
	//
	//     ch1:val1 \t ch2:val1 \t ch3:val1 \t ch4:val1 \r\n
	//     ch1:val2 \t ch2:val2 \t ch3:val2 \t ch4:val2 \r\n
	//     ch1:val3 \t ch2:val3 \t ch3:val3 \t ch4:val3 \r\n
	//     \0     
	//
	//
	//HGLOBAL hString:
	//
	//this function can be used when generating tabbedtext for clipboard 
	//as well as when generating tabbedtext for CEdit control.
	//
	//
	
	////////////////////////////////////////
	//first, initialize hpv_tabbedtextbuffer
	////////////////////////////////////////  
	LPSTR text = new char[MAX_DUMMY_STRING_SIZE];  
	WCHAR* wctext = new WCHAR[MAX_DUMMY_STRING_SIZE];

	char* hpv_tabbedtextbuffer = NULL;
	int blocksize = TABBEDTEXTBUFFER_INITIAL_BLOCKSIZE;
	int totalallocsize = TABBEDTEXTBUFFER_ADDITIONAL_BLOCKSIZE; 
	int indexbuffer = 0; 
	
	HGLOBAL hString = GlobalAlloc(LHND, (UINT)totalallocsize);
	VERIFY(hString);	
	hpv_tabbedtextbuffer = (char*) GlobalLock(hString);	  
	VERIFY(hpv_tabbedtextbuffer); 
	
    

	/////////////////////////////////////////////////
	//second, fill the hpv_tabbedtextbuffer with data
	/////////////////////////////////////////////////  

    indexbuffer = 0; //should always indicates the end index of the tabbedtextbuffer 
    int i = 0; //index of the channel data array
    int textlength = 0;
    
	int numberofsets = m_GraphSetList.GetSize();
	COWGraphSet* p_currentGraphSet = NULL;
	
	//if no data, unlock the handle and exit this function
	if( numberofsets == 0) 
	{
		//add the null character
		hpv_tabbedtextbuffer[indexbuffer] = '\0'; 
		indexbuffer++; 
		VERIFY(!GlobalUnlock(hString)); 
		if(text!=NULL) delete[] text;
	    return hString;
	}	
	 
	 
	//print X and Y axis label on the first line   
	CStringA myString1(m_XAxisLabelString);
	const char* psz_XAxisLabel = (const char*)myString1;
	CStringA myString2(m_YAxisLabelString);
	const char* psz_YAxisLabel = (const char*) myString2;
	ASSERT(psz_XAxisLabel);
	ASSERT(psz_YAxisLabel);
	wsprintf(wctext, L" %s\t%s\r\n",psz_XAxisLabel, psz_YAxisLabel);
    VERIFY( textlength = strlen(text) ); 
	memcpy(&(hpv_tabbedtextbuffer[indexbuffer]),(char*)text, textlength );
	indexbuffer += textlength;  
		
	//concatenate a tab to the tabbedtextbuffer (leaves first field of second line empty)
	hpv_tabbedtextbuffer[indexbuffer] = '\t';  //tab, ascii 9
	indexbuffer ++;     
		
	for(int index=0; index<numberofsets; index++)
	{       
		p_currentGraphSet = (COWGraphSet*) m_GraphSetList.GetAt(index);
		if(p_currentGraphSet->IsVisible())
		{   
			lstrcpyA( text, p_currentGraphSet->GetGraphSetName() );				
			if( text[0] == '\0' ) wsprintfA(text, "set %d\t",index);
			  else lstrcatA( text, "\t"); 
		    VERIFY( textlength = strlen(text) ); 
			memcpy(&(hpv_tabbedtextbuffer[indexbuffer]),(char*)text, textlength );
			indexbuffer += textlength;  
		}
	}
	//overwrite on top of the last tab
	indexbuffer--;
	//concatenate a carriage return and linefeed to the tabbedtextbuffer
	hpv_tabbedtextbuffer[indexbuffer] = '\r';  //carriage return, ascii 13
	indexbuffer ++;     
	hpv_tabbedtextbuffer[indexbuffer] = '\n';  //linefeed, ascii 10
	indexbuffer ++;     
		
	
	for(i=0; i<m_maxnumberofpointsperset; i++)
	{   
		//print the X axis values 
		wsprintfA(text, " %d\t",i);
	    VERIFY( textlength = strlen(text) ); 
		memcpy(&(hpv_tabbedtextbuffer[indexbuffer]),(char*)text, textlength );
		indexbuffer += textlength;  
		
		
		for(int index=0; index<numberofsets; index++)
		{                           
			//for each visible graphsets
			p_currentGraphSet = (COWGraphSet*) m_GraphSetList.GetAt(index);
			if(p_currentGraphSet->IsVisible())
			{    
				//convert the value into a text string
				lstrcpy(wctext, p_currentGraphSet->GetValueAsString((int)i));
				//concatenate this text string to the tabbedtextbuffer
				textlength = lstrlen(wctext);
				if( (indexbuffer+textlength+10) > totalallocsize) 
				{   
					//reallocate if extra space required
					VERIFY(hString!=NULL);
					HGLOBAL hNewAlloc;
					VERIFY(!GlobalUnlock(hString)); 
					totalallocsize += blocksize; 
					hNewAlloc = GlobalReAlloc(hString, totalallocsize, GMEM_MOVEABLE);
					if(hNewAlloc==NULL) 
					{
						VERIFY(!GlobalFree(hString));
						if(text!=NULL) delete[] text;
						return NULL; //error
					}	
					hString = hNewAlloc;  
					VERIFY(hpv_tabbedtextbuffer = (char*) GlobalLock(hString));  
				}
				//if text string not empty, add it to the tabbedtext buffer				
				if(textlength>0)
				{
					CStringA myString(wctext);
					lstrcpyA(text, (const char*)myString);
					memcpy(&(hpv_tabbedtextbuffer[indexbuffer]),(char*)text, textlength );
					indexbuffer += textlength;  
				}	
				//concatenate a tab to the tabbedtextbuffer
				hpv_tabbedtextbuffer[indexbuffer] = '\t';  //tab, ascii 9
				indexbuffer ++;     
			}
		} 
		//concatenate a carriage return and linefeed to the tabbedtextbuffer
		indexbuffer--; //we want to overwrite on top of the last tab caracter 
		
		hpv_tabbedtextbuffer[indexbuffer] = '\r';  //replace by a carriage return , ascii 13
		indexbuffer++; 
		
		hpv_tabbedtextbuffer[indexbuffer] = '\n';  //replace by a linefeed, ascii 10
		indexbuffer++; 
	}
	                  
	//add the null character
	hpv_tabbedtextbuffer[indexbuffer] = '\0'; 
	indexbuffer++; 
	
	VERIFY(!GlobalUnlock(hString)); 
    if(text!=NULL) delete[] text;
    return hString;
}  

          
          
BOOL COWGraphDoc::CopyGraphSetsToClipboardVerticaly()     
{
	//copy all visible graph sets data into a tabbedtext string,
	//that is using tab-txt format (values separated by tab, \t, 
	//and set's index separated using return, \r\n.)    
	//
	//
	//	produces following output (examples):
	//
	//     ch1:val1 \t ch2:val1 \t ch3:val1 \t ch4:val1 \r\n
	//     ch1:val2 \t ch2:val2 \t ch3:val2 \t ch4:val2 \r\n
	//     ch1:val3 \t ch2:val3 \t ch3:val3 \t ch4:val3 \r\n
	//     \0
	
	HGLOBAL hTabbedTextBuffer = GenerateTabbedTextBuffer();       
	VERIFY(hTabbedTextBuffer!=NULL);
	
	//
	//Open, Erase, Write to and Close the Clipboard 
	HWND hMainWindow = NULL;
	VERIFY(hMainWindow = AfxGetApp()->m_pMainWnd->GetSafeHwnd());
	if( ! ::OpenClipboard(hMainWindow) )
	{
		//ERR_HANDLER_SHOWWARNING(IDS_ERR_OPENING_CLIPBOARD);
		AfxMessageBox(L"Error Opening Clipboard");
		GlobalFree(hTabbedTextBuffer);
		return FALSE;
	}
	VERIFY(EmptyClipboard());
	VERIFY(SetClipboardData(CF_TEXT, hTabbedTextBuffer));
	VERIFY(CloseClipboard());
	
	//
	//Do not free the hTabbedTextBuffer, it now belongs to the clipboard
	
	return TRUE;                                                
}



	
	
//As of today, jan 1995, this function supports data pasted from
//clipboard only if previously copied from Excel.  It also assumes each
//row of data is one channel (which is contradictory with the comments
//below, but this function was on its way to be modified).
//
// This function is provided only to ease the testing of graph module.
// This function will be temporarely available from the EDIT_PASTE menu item
// of the GraphData EditView menu bar.
//
BOOL COWGraphDoc::PasteGraphSetsFromClipboard()     
{     
	//
	//assuming the clipboard contains CF_TEXT characters text array,
	//this function gets and reads values out of this string. 
	//
	//note on tab-txt format:  values are separated by tab, \t, 
	//and sets separated using return, \r\n.    
    //
    //note for developper: be carefull when modifying this code, it has to work
    //for oiii copy paste as well as Excel and maybe others in the future.
    //
    //there is two most common cases: 
    //
    //	1) Tabbed text buffer generated by oiii on copy data to clipboard,
    //     the tabbed text buffer might contain text identifiers
    //	   for both channels (columns) and values (rows).  
    //
	//	   takes following input (examples, case 1):
	//     
	//     \t        channel1 \t channel2 \t channel3 \t channel4 \r\n  
	//     value1 \t ch1:val1 \t ch2:val1 \t ch3:val1 \t ch4:val1 \r\n
	//     value2 \t ch1:val2 \t ch2:val2 \t ch3:val2 \t ch4:val2 \r\n
	//     value3 \t ch1:val3 \t ch2:val3 \t ch3:val3 \t ch4:val3 \r\n
	//     \0     
    //     		where ch1:val1 is a numeric value, first value of the first channel.
    //
    //
    //	2) Tabbed text buffer generated by Excel on copy selected data to clipboard,
    //	   the tabbed text buffer should not contain any characters, only the values.
	//
	//	   takes following input (examples, case 2):
	//
	//     ch1:val1 \t ch2:val1 \t ch3:val1 \t ch4:val1 \r\n
	//     ch1:val2 \t ch2:val2 \t ch3:val2 \t ch4:val2 \r\n
	//     ch1:val3 \t ch2:val3 \t ch3:val3 \t ch4:val3 \r\n
	//     \0     
	//			where ch1:val1 is a numeric value, first value of the first channel.
	//
	//
	
	
	       
	// 1) check if the clipboard data is a CF_TEXT character array.
	//    
	HGLOBAL hString = NULL;
	HWND hMainWindow = NULL;
	VERIFY(hMainWindow = AfxGetApp()->m_pMainWnd->GetSafeHwnd());
	if(! ::OpenClipboard(hMainWindow) )
	{   
		//error opening clipboard          
		//ERR_HANDLER_SHOWWARNING(IDS_ERR_OPENING_CLIPBOARD);
		return FALSE;
	}
	hString = GetClipboardData(CF_TEXT);
	if(hString==NULL) 
	{   
		//error getting data from clipboard
		AfxMessageBox(L"Error Opening Clipboard");
		//ERR_HANDLER_SHOWWARNING(IDS_ERR_OPENING_CLIPBOARD);
		VERIFY(CloseClipboard());	
		return FALSE;
	}	


	// 2) scan the whole clipboard character array, doing so,
	//           -check if contains at least one point, ".",
	//            if yes then float values otherwise long.
	//
	int lsize = (int) GlobalSize(hString);
	if(lsize<SHRT_MAX && lsize>1) 
	{    
		//size smaller than 32767 characters, we can use a CString object to
		//manipulate the buffer.
		
		char* hpv_hString = NULL;
		VERIFY(hpv_hString = (char*) GlobalLock(hString));
		//
		//transfer buffer from handle to CString
		char* p_hString = (char*) hpv_hString; 
		CString string(p_hString);
        int ilength = string.GetLength(); //ilength count excludes the null char.
         
		//
		//must validate data, to prevent crash or endless loop		
		//
		
        //is the last character a carriage return?
        if(string[ilength-1] != 13)
        {                                                   
        	if(string[ilength-1]==10 && string[ilength-2]==13)
        	{
        		//microsoft excel puts a line-feed and a carriage at the end, "\r\n"
        		ilength = ilength - 1;
        	}
        	else
        	{
        		//add the ascii 13 caracter to the end of the string
        		string += "\r\n";   
        		ilength = string.GetLength();
        	}
        }        
        
        		
		//is there real numbers, or only long integers?
		BOOL bContainsFloat = FALSE;
		if(string.Find('.')>=0) bContainsFloat = TRUE; 
		
		//are the text labels present or not?  
		BOOL bContainsLabels = FALSE;		
		if(string[0]=='\t' && string[1]=='C' && string[2]=='h') bContainsLabels = TRUE;
		

        //right now, we cannot copy and paste the graphdata from the application
        if(bContainsLabels) return FALSE;
        
		 
		 
		// 3) now, we can create buffer arrays to receive sequencialy the values.
		//   
		// 4) for each buffer array, initialize a GraphSet and add it to the 
		//    existing m_GraphSetList.
		//  
		int* lp_longarray = NULL;
		double* lp_floatarray = NULL;
		int lvalue = 0;
		float fvalue = 0.0f;
		
		int i = 0; //logical pointer to char buffer
		int ivalue = 0; //logical pointer to a specific value in the set
		int iset = 0; //logical pointer to a specific set 
		int inumbervaluesperset = 0; //total number of values per set
		int inumberofsets = 0; //total number of sets
		CString stringTmp("");
		
		
		
		if(!bContainsFloat)
		{
			//all values are long integers 
			lp_longarray = new int[MAX_GRAPHSET_SIZE]; //to store temporarely the graphset data
			ASSERT(lp_longarray);
			lvalue = 0; //to store temporarely a value
		}
		else
		{
			//some values are float, therefore all values considered as float 
			lp_floatarray = new double[MAX_GRAPHSET_SIZE]; //to store temporarely the graph set data
			ASSERT(lp_floatarray);
			fvalue = 0.0f; //to store temporarely a value
		}
		
		
		while (i<ilength)
		{   
		    stringTmp = " ";    
			while(string[i]!=9 && string[i]!=13)
			{   
				//while caracter not tab or carriage, fill the temporary string    
				stringTmp += string[i];
				i++;
			}
			//after ith caracter is a tab or carriage
			if(stringTmp[0]!=0)
			{   
				//if temporary string not empty, put it in the data array
				if(!bContainsFloat)
				{
					lvalue = _wtol(stringTmp);
					lp_longarray[ivalue] = lvalue;
				}      
				else
				{
					fvalue = (float) _wtof(stringTmp);
					lp_floatarray[ivalue] = fvalue;
				}
				//set logical pointer to next value
				ivalue++;
			}
			else if(stringTmp[0]==0 && string[i]==9)
			{
				//if clipboard contains two tabs in a row, the string is empty
				//we can load it as a zero value (or we could alert user!)
				if(!bContainsFloat) lp_longarray[ivalue] = 0;
				  else lp_floatarray[ivalue] = 0.0f;
				//set logical pointer to next value
				ivalue++;
			}
			if(string[i]==9)
			{        
				//set logical pointer to next char
				i++; 
			}
			else if(string[i]==13)
			{   
				//if end of set, create the COWGraphSet object and add it to
				//the COWGraphDoc::m_GraphSetList.
				inumbervaluesperset = ivalue;
					
				if(!bContainsFloat) AddGraphSet(lp_longarray, inumbervaluesperset);
				  else AddGraphSet(lp_floatarray, inumbervaluesperset);
										
				//set logical pointer to next char and next set.
				i++;
				if(i<ilength)
				{   
					//since Excel seems to add a linefeed character after each line
					if(string[i]=='\n') i++; //SPoirier dec 6th 1993
				}					
				iset++;                       
				//reset logical pointer (or array index) to next value
				ivalue = 0;
			}	
		} //end of while(not end of string buffer) 
		inumberofsets = iset;      
			
		//delete the long or float array pointer
		if(lp_longarray) delete[](lp_longarray);
	    if(lp_floatarray) delete[](lp_floatarray);		
		
	}//end of if(size < SHRT_MAX)
	else
	{
		//for now, if clipboard bigger than SHRT_MAX (32767) bytes, do nothing (do it later, on request)
		AfxMessageBox(L"Error Data is Too Large for Clipboard");
		//ERR_HANDLER_SHOWWARNING( IDS_ERR_CLIPBOARD_TOOBIG );
	}//end of if(size > SHRT_MAX) 
	
	GlobalUnlock(hString);
	CloseClipboard();

	//SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
	return TRUE;                                                
}

		
void COWGraphDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


void COWGraphDoc::OnFileSave()
{   
	//we do not save for now, but we must override to prevent default save as raw to occur
	return;
}

void COWGraphDoc::SetDefaultGraphBackgroundAndTextColor(COLORREF crBackGroundColor/* = RGB(255,255,255)*/,
														COLORREF crTextColor/* = RGB(0,0,0)*/)
{
	m_crDefaultBackgroundColor = crBackGroundColor;
	m_crDefaultTextColor = crTextColor;
	//TRACE("graphdoc's bkcolor is 0x%x, graphdoc's txt color is 0x%x\n", m_crDefaultBackgroundColor, m_crDefaultTextColor );
	return;
}

int COWGraphDoc::GetNumberOfVisibleSets()
{
	return m_numberofvisiblesets;
}

void COWGraphDoc::UseDefaultGraphGrayColorTable()
{
	m_bUseDefaultMultiColorTable = FALSE;
	m_bUseDefaultGrayColorTable = TRUE;
	return;
}

void COWGraphDoc::UseDefaultGraphMultiColorTable()
{
	m_bUseDefaultGrayColorTable = FALSE;
	m_bUseDefaultMultiColorTable = TRUE;
	return;
}
