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

///////////////////////////////////////////////////
// ourdoc.cpp : behavior for the class COWDocument
//
// Stephane Poirier
///////////////////////////////////////////////////
 
         
#include "stdafx.h"
#include "oifiilib.h" //spi 2014
#include "direct.h" 	//SPoirier, because of _getcwd() in GetCurrentDirectory()  
#include "owdoc.h"

//#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



////////////////////////////////
// Macros private to this module
////////////////////////////////
#define OURDOC_MAXFILENAMELENGTH	1024



///////////////
// COWDocument
///////////////
IMPLEMENT_DYNCREATE(COWDocument, CDocument)

BEGIN_MESSAGE_MAP(COWDocument, CDocument)
	//{{AFX_MSG_MAP(COWDocument)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
 
               
COWDocument::COWDocument()
{     
	m_parentHWND=NULL; //spi 2014
}  

COWDocument::COWDocument(HWND hwnd) //spi 2014, new constructor
{
	m_parentHWND=hwnd;
}

COWDocument::~COWDocument()
{  
}


CString COWDocument::GetPathNameOnly()
{   
	CString stringPathNameOnly("");  
	CString stringPathAndFileName(GetPathName());
	int nIndex = stringPathAndFileName.ReverseFind('\\');
	if(nIndex>=0)
	{
		stringPathNameOnly = stringPathAndFileName.Left(nIndex); 
	}	
	else if(stringPathAndFileName == "")
	{   
		stringPathNameOnly = ""; 
	}
	else 
	{   
		//there is no back slash in the full path and file name.
		ASSERT(FALSE); //development-time error, if pathname not null, it should contains at least a backslash
		//return safely, assumes stringPathAndFileName contains only the pathname 
		stringPathNameOnly = stringPathAndFileName; 	
	}
	return stringPathNameOnly;
}


CString COWDocument::GetFileNameOnly()
{   
	CString stringFileNameOnly(""); 
	CString stringPathAndFileName(GetPathName());
	int nIndex = stringPathAndFileName.ReverseFind('\\');
	if(nIndex>=0)
	{
		stringFileNameOnly = stringPathAndFileName.Left(nIndex); 
		int nCount = stringPathAndFileName.GetLength();
		nCount -= (nIndex+1);
		stringFileNameOnly = stringPathAndFileName.Right(nCount);  
	}	
	else if(stringPathAndFileName == "")
	{   
		stringFileNameOnly = ""; 
	}
	else
	{   
		//there is no back slash in the full path and file name.
		ASSERT(FALSE); //development-time error, if pathname not null, it should contains at least a backslash
		//return safely, assumes stringPathAndFileName contains only the filename 
		stringFileNameOnly = stringPathAndFileName; 	
	}
	return stringFileNameOnly;
}


CString COWDocument::GetCurrentDirectory()
{
	CString stringPathName(""); 
	char buffer[OURDOC_MAXFILENAMELENGTH]; 
	if(_getcwd(buffer, OURDOC_MAXFILENAMELENGTH) == NULL)
	{   
		//if error, set stringPathName safely 
		stringPathName = "c:\\"; 
	} 
	else
	{   
		//if ok
		stringPathName = buffer;
   		int nLength = stringPathName.GetLength();
   		if(stringPathName[nLength-1]!='\\') stringPathName += "\\"; 
	}
	return stringPathName; 
}

