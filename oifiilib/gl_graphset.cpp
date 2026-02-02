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

#include "stdafx.h"  
#include "oifiilib.h" //spi 2014

#include "gl_owgraph.h"
#include "limits.h"
#include "float.h"
 
//#include "graphset.h"
//#include "private.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
 
 
//////////////////////////////////////////
// COWGraphSet constructors and destructor
//////////////////////////////////////////
COWGraphSet::COWGraphSet()
{
	m_lp_longarray = NULL;
	m_lp_floatarray = NULL;
	m_lp_doublearray = NULL;
	m_ldatatype = 0;
	m_ldatasize = 0;  
	m_lchannelid = 0;
	m_lgraphsetcolor = GRAPH_DEFAULT_COLOR;
	m_bUseColorTable = TRUE;
	lstrcpyA(m_psz_graphsetname, "");
	m_bvisible = TRUE;
	
	m_loffsetbetweentwoconsecutivevalues = 1;
}	

COWGraphSet::COWGraphSet(	int far* lp_inputlongarray, 
							int datasize,  
							LPCSTR psz_channelname,
							int channelid,
							COLORREF crRGBColor)
{           
    m_lp_longarray = new int[datasize];
	m_lp_floatarray = NULL;
    m_ldatatype = LONGTYPE;
    m_ldatasize = datasize; 
	m_lchannelid = channelid;
	m_lgraphsetcolor = crRGBColor;
	if(crRGBColor!=GRAPH_DEFAULT_COLOR)
	{
		m_bUseColorTable = FALSE;
	}
	if(psz_channelname)	lstrcpyA(m_psz_graphsetname, psz_channelname);
	  else lstrcpyA(m_psz_graphsetname, "");
	    
	//call member initialization function
	COWGraphSet::Initialize();
	  
	m_bvisible = TRUE;	
    //take a copy of the input pointer to data 
    memcpy(m_lp_longarray, lp_inputlongarray, (size_t) datasize*sizeof(int)); 
}

COWGraphSet::COWGraphSet(	double far* lp_inputfloatarray, 
							int datasize,
							LPCSTR psz_channelname, 
							int channelid,
							COLORREF crRGBColor)
{                        
	m_lp_longarray = NULL;
    m_lp_floatarray = new double[datasize];
    m_ldatatype = FLOATTYPE;
    m_ldatasize = datasize; 
	m_lchannelid = channelid;
	m_lgraphsetcolor = crRGBColor;
	if(crRGBColor!=GRAPH_DEFAULT_COLOR)
	{
		m_bUseColorTable = FALSE;
	}
	if(psz_channelname)	lstrcpyA(m_psz_graphsetname, psz_channelname);
	  else lstrcpyA(m_psz_graphsetname, ""); 
	   
	//call member initialization function
	COWGraphSet::Initialize();  
	
	m_bvisible = TRUE;	
    //take a copy of the input pointer to data 
    memcpy(m_lp_floatarray, lp_inputfloatarray, (size_t) datasize*sizeof(double));
}

COWGraphSet::COWGraphSet(	double far* lp_dfBasePointer,
							int lOffsetBetweenTwoConsecutiveValues,
							int lNumberOfValues, 
							LPCSTR psz_channelname/*=NULL*/, 
							int lChannelId/*=0*/,
							BOOL bReallocateAsFloatArray/*=FALSE*/,
							COLORREF crRGBColor)
{
	m_lp_longarray = NULL;
    m_lp_floatarray = NULL;
	if(bReallocateAsFloatArray==FALSE)
	{
		//no data allocation
		m_ldatatype = DOUBLETYPE;
		m_lp_doublearray = lp_dfBasePointer;
		m_loffsetbetweentwoconsecutivevalues = lOffsetBetweenTwoConsecutiveValues;	  

	}
	else
	{
		//reallocate as a float array
		m_ldatatype = FLOATTYPE;
		m_lp_floatarray = new double[lNumberOfValues];
		//copy data from double interlaced array into double array
		for(int i=0; i<lNumberOfValues; i++)
		{
			m_lp_floatarray[i] = (double) lp_dfBasePointer[i*lOffsetBetweenTwoConsecutiveValues];
		}
	}
    m_ldatasize = lNumberOfValues; 
	m_lchannelid = lChannelId;
	m_lgraphsetcolor = crRGBColor;
	if(crRGBColor!=GRAPH_DEFAULT_COLOR)
	{
		m_bUseColorTable = FALSE;
	}
	if(psz_channelname)	lstrcpyA(m_psz_graphsetname, psz_channelname);
	  else lstrcpyA(m_psz_graphsetname, ""); 

	//call member initialization function
	COWGraphSet::Initialize();  
	
	m_bvisible = TRUE;	
}


void COWGraphSet::Initialize()
{   
	// initialize the color based on the graphset name, the color index to the one defined in
	// graphvw.cpp module. 
	if(lstrcmpiA(m_psz_graphsetname, "grey")==0 || lstrcmpiA(m_psz_graphsetname, "gris")==0)
	{
		if(m_bUseColorTable) m_lgraphsetcolor = 0;
	}
	if(lstrcmpiA(m_psz_graphsetname, "red")==0 || lstrcmpiA(m_psz_graphsetname, "rouge")==0
	   || lstrcmpiA(m_psz_graphsetname, "r")==0)
	{
		if(m_bUseColorTable) m_lgraphsetcolor = 1;
	}  
	else if(lstrcmpiA(m_psz_graphsetname, "green")==0 || lstrcmpiA(m_psz_graphsetname, "vert")==0
	        || lstrcmpiA(m_psz_graphsetname, "g")==0 || lstrcmpiA(m_psz_graphsetname, "v")==0 )
	{
		if(m_bUseColorTable) m_lgraphsetcolor = 2;
	}  
	else if(lstrcmpiA(m_psz_graphsetname, "blue")==0 || lstrcmpiA(m_psz_graphsetname, "bleu")==0
	        || lstrcmpiA(m_psz_graphsetname, "b")==0)
	{
		if(m_bUseColorTable) m_lgraphsetcolor = 3;
	}
	return;  
}


COWGraphSet::~COWGraphSet()
{	
	if(m_lp_longarray!=NULL) delete[](m_lp_longarray);
	if(m_lp_floatarray!=NULL) delete[](m_lp_floatarray);
}         
     
 
 
 

///////////////////////////////////////
//To read, write and edit array of data
///////////////////////////////////////     
int COWGraphSet::GetValueAsLong(int index)
{
	if(index>=m_ldatasize)
	{
		//since all graphdoc's graphsets may not have identical length
		return 0L;
	}
	if(m_ldatatype == LONGTYPE) return m_lp_longarray[index];
	else if(m_ldatatype == FLOATTYPE) return (int)m_lp_floatarray[index]; 
	
	//on error
    return 0L;
} 

double COWGraphSet::GetValueAsFloat(int index)
{	
	if(index>=m_ldatasize)
	{
		//since all graphdoc's graphsets may not have identical length
		return 0.0f;
	}
	if(m_ldatatype == LONGTYPE) return (double) m_lp_longarray[index];
	else if(m_ldatatype == FLOATTYPE) return m_lp_floatarray[index]; 
	else if(m_ldatatype == DOUBLETYPE)
	{
		return (double) GetValueAsDouble(index);
	}
	//on error
	ASSERT(FALSE);
    return 0.0f;
} 

double COWGraphSet::GetValueAsDouble(int index)
{
	if(index>=m_ldatasize)
	{
		//since all graphdoc's graphsets may not have identical length
		return 0L;
	}
	if(m_ldatatype == LONGTYPE) return (double)m_lp_longarray[index];
	else if(m_ldatatype == FLOATTYPE) return (double)m_lp_floatarray[index]; 
	else if(m_ldatatype == DOUBLETYPE) 
	{
		return m_lp_doublearray[index*m_loffsetbetweentwoconsecutivevalues]; 
	}
	//on error
    return 0L;
}

//this function can be used to display the graph data values
//in the child frame's status bar      
CString COWGraphSet::GetValueAsString(int index)
{    
	CString myString = "";
	
	if(index>=m_ldatasize)
	{
		//if outside boundary, return an empty string
		return myString;
	}
	if(m_ldatatype == LONGTYPE) 
	{
		myString.Format(L"%ld", m_lp_longarray[index]);
		return myString;
	}	
	else if(m_ldatatype == FLOATTYPE) 
	{
		myString.Format(L"%g",m_lp_floatarray[index]);
		return myString; 
	}	
	else if(m_ldatatype == DOUBLETYPE) 
	{
		myString.Format(L"%g",GetValueAsDouble(index));
		return myString; 
	}		
	//on error
	ASSERT(FALSE);
    return myString;
} 
       
       
void COWGraphSet::SetValue(int value, int index)
{	
	m_lp_longarray[index] = value;
} 

void COWGraphSet::SetValue(double value, int index)
{	
	m_lp_floatarray[index] = value;
}

BOOL COWGraphSet::SetAllValues(int far* lp_inputlongarray, int datasize, int channelid)
{
    m_lp_longarray = new int[datasize];
	m_lp_floatarray = NULL;
    m_ldatatype = LONGTYPE;
    m_ldatasize = datasize; 
	m_lchannelid = channelid;
	m_bvisible = TRUE;	
    //take a copy of the input pointer to data 
    memcpy(m_lp_longarray, lp_inputlongarray, (size_t) datasize*sizeof(int));
    return TRUE;
}

BOOL COWGraphSet::SetAllValues(double far* lp_inputfloatarray, int datasize, int channelid)
{
    m_lp_longarray = NULL;
	m_lp_floatarray = new double[datasize];
    m_ldatatype = FLOATTYPE;
    m_ldatasize = datasize; 
	m_lchannelid = channelid;
	m_bvisible = TRUE;	
    //take a copy of the input pointer to data 
    memcpy(m_lp_floatarray, lp_inputfloatarray, (size_t) datasize*sizeof(int));
    return TRUE;
}
         
BOOL COWGraphSet::AddValue(double fvalue)
{     
	//1) if extra space required, allocate extra space
	//2) add the new float value at the end of the array
	fvalue = 0.0f;
	return TRUE;
}
     
BOOL COWGraphSet::AddValue(int lvalue)
{     
	//1) if extra space required, allocate extra space
	//2) add the new float value at the end of the array
	lvalue = 0L;
	return TRUE;
}
  
         
BOOL COWGraphSet::AddExtraValues(int far* /*lp_extravalues*/, int /*nvalues*/ )
{   
	//ASSERT(lp_extravalues);
	//if(nvalues);            
	return TRUE;	
}
BOOL COWGraphSet::AddExtraValues(double far* /*lp_extravalues*/, int /*nvalues*/)
{   
	//ASSERT(lp_extravalues);
	//if(nvalues);            
	return TRUE;	
}


// This function returns graphset's name
LPCSTR COWGraphSet::GetGraphSetName()
{                 
	return (LPCSTR) m_psz_graphsetname;
}                 

int COWGraphSet::GetDataType()
{
	return m_ldatatype; 
}

void COWGraphSet::SetDataType(int ldatatype)
{
	m_ldatatype = ldatatype; 
}



////////////
//Visibility
////////////
BOOL COWGraphSet::IsVisible()
{
	return m_bvisible;
}
void COWGraphSet::SetVisibility(BOOL bstate)
{   
	m_bvisible = bstate;
	return;
}
 
 
 
/////////////////////////////////////////
//To compute statistics from the data set 
/////////////////////////////////////////
double COWGraphSet::GetYMin()
{   
	//it scans the whole array
	int indexend = m_ldatasize; 
	double dfymin = FLT_MAX;
	double dfthismin = 0.0;
	double fymin = FLT_MAX;
	double fthismin = 0.0f;  
	int lymin = SHRT_MAX;
	int lthismin = 0;  
	if(m_ldatatype == LONGTYPE)
	{
	    for(int index=0; index<indexend; index++)
	    {       
	    	lthismin = m_lp_longarray[index];
	    	if(lthismin < lymin)                      
	    	{    
	    		lymin = lthismin; 
	    	}
	    }
	}
	else if(m_ldatatype == FLOATTYPE)
	{
	    for(int index=0; index<indexend; index++)
	    {       
	    	fthismin = m_lp_floatarray[index];
	    	if(fthismin < fymin)                      
	    	{    
	    		fymin = fthismin; 
	    	}
	    }
	}                         
	else if(m_ldatatype == DOUBLETYPE)
	{
	    for(int index=0; index<indexend; index++)
	    {       
	    	dfthismin = m_lp_doublearray[index*m_loffsetbetweentwoconsecutivevalues];
	    	if(dfthismin < dfymin)                      
	    	{    
	    		dfymin = dfthismin; 
	    	}
	    }
	}                         
	
	if(m_ldatatype == LONGTYPE) 
	{
		return (double)lymin;
	}	
	else if(m_ldatatype == FLOATTYPE) 
	{
		return fymin; 
	}
	else if(m_ldatatype == DOUBLETYPE)
	{
		return (double)dfymin;
	}
	else return FLT_MAX; //error
		
}       


double COWGraphSet::GetYMax()
{              
	//it scans the whole array
	int indexend = m_ldatasize; 
	double dfymax = FLT_MIN;
	double dfthismax = 0.0;
	double fymax = FLT_MIN;
	double fthismax = 0.0f;  
	int lymax = SHRT_MIN;
	int lthismax = 0;  
	if(m_ldatatype == LONGTYPE)
	{
	    for(int index=0; index<indexend; index++)
	    {       
	    	lthismax = m_lp_longarray[index];
	    	if(lymax < lthismax)                      
	    	{    
	    		lymax = lthismax; 
	    	}
	    }
	}
	else if(m_ldatatype == FLOATTYPE)
	{
	    for(int index=0; index<indexend; index++)
	    {       
	    	fthismax = m_lp_floatarray[index];
	    	if(fymax < fthismax)                      
	    	{    
	    		fymax = fthismax; 
	    	}
	    }
	}                         
	else if(m_ldatatype == DOUBLETYPE)
	{
	    for(int index=0; index<indexend; index++)
	    {       
	    	dfthismax = m_lp_doublearray[index*m_loffsetbetweentwoconsecutivevalues];
	    	if(dfymax < dfthismax)                      
	    	{    
	    		dfymax = dfthismax; 
	    	}
	    }
	}                         
	
	if(m_ldatatype == LONGTYPE) 
	{
		return (double)lymax;
	}	
	else if(m_ldatatype == FLOATTYPE) 
	{
		return fymax; 
	}
	else if(m_ldatatype == DOUBLETYPE)
	{
		return (double)dfymax;
	}
	else return FLT_MIN; //error
}                


double COWGraphSet::GetAverage()
{   
	ASSERT(FALSE);
	return 0.0f;
}
double COWGraphSet::GetStddev()
{               
	ASSERT(FALSE);
	return 0.0f;
}

int COWGraphSet::GetDataSize()
{
	return m_ldatasize;              
}

int COWGraphSet::GetChannelID()
{
	return m_lchannelid;              
}

int COWGraphSet::GetColor()
{
	return m_lgraphsetcolor;              
}
