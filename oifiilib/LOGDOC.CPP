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
// logdoc.cpp : behavior for the class CLogDocument
//
// Stephane Poirier, jan 1996 
///////////////////////////////////////////////////
 
         
#include "stdafx.h" 
#include "oifiilib.h" //spi 2014
#include "io.h" 		//SPoirier, for access (see OnFileSave)
#include "owdoc.h" 
#include "logdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



////////////////////////////////
// Macros private to this module
////////////////////////////////

#define LOGDOC_MAXFILENAMELENGTH	512






/////////////////////
// CLogDocument
/////////////////////
IMPLEMENT_DYNCREATE(CLogDocument, COWDocument)

BEGIN_MESSAGE_MAP(CLogDocument, COWDocument)
	//{{AFX_MSG_MAP(CLogDocument)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)	
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
 
               
CLogDocument::CLogDocument()
{     
	m_bEnableLog = TRUE;

	m_bUpdateWhenModified = TRUE;
	m_bBusyUpdating = FALSE;
	m_bUpdateFileWhenModified = FALSE;

	//erase default log file
	ClearLogFile();
}  


CLogDocument::~CLogDocument()
{  
}

BOOL CLogDocument::OnNewDocument()
{                                                 
	if (!CDocument::OnNewDocument()) return FALSE; 
	return TRUE;
}

         
void CLogDocument::OnFileSave()
{   
	OnFileSaveAs();
	/*
	if (_access(m_strPathName, 6) != 0)
	{
		// <<ERROR>>, we do not have write access or the file does not exist 
		CLogDocument::OnFileSaveAs();
	}           
	else
	{
		OnSaveDocument(m_strPathName);
		SetModifiedFlag(FALSE); // back to unmodified
	}
	return;
	*/
}

void CLogDocument::OnFileSaveAs()
{    
    //1) get actual document path and file name
	CString stringPathName("");
	CString stringFileName("");
	if(GetPathName() == "")
	{
		//if new unsaved file, get current directory
		stringPathName = COWDocument::GetCurrentDirectory();
		//get current and document's title
	   	stringFileName = "log.txt";
	}
	else
	{   
		stringPathName = COWDocument::GetPathNameOnly();
		stringFileName = COWDocument::GetFileNameOnly(); 
	} 
	
	//2) prepare the open filename structure 
	OPENFILENAME ofn;           
	//char* szFilter[]= { "All Files (*.*)", "*.*", "" };
	WCHAR* pszFilter[]= {L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0"};

	memset(&ofn, 0, sizeof(OPENFILENAME));		//set structure to zero
	WCHAR PathName[LOGDOC_MAXFILENAMELENGTH];
	WCHAR FileName[LOGDOC_MAXFILENAMELENGTH];			
	WCHAR FileTitle[LOGDOC_MAXFILENAMELENGTH];
	wcscpy(PathName, stringPathName);
	wcscpy(FileName, stringFileName);
	FileTitle[0]='\0';
   	ofn.lStructSize       = sizeof (OPENFILENAME);
   	ofn.hwndOwner         = AfxGetApp()->m_pMainWnd->GetSafeHwnd();
   	ofn.lpstrFilter       = pszFilter[0];
   	ofn.lpstrFile         = FileName;
   	ofn.nMaxFile          = LOGDOC_MAXFILENAMELENGTH; //sizeof (FileName);
   	ofn.lpstrFileTitle    = FileTitle;
   	ofn.nMaxFileTitle     = LOGDOC_MAXFILENAMELENGTH; //sizeof (FileTitle);
   	ofn.lpstrInitialDir   = PathName; 
   	ofn.lpstrTitle		  = L"File Save As";
   	ofn.Flags             = OFN_HIDEREADONLY | OFN_SHOWHELP | OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT| OFN_PATHMUSTEXIST;

    //3) use the open filename dialog 
	DWORD dwError;
   	if(::GetSaveFileName(&ofn))
   	{   
   		//4) if successfull, save document and set title
   		CString stringNewName(ofn.lpstrFile);
		CStringA myString(stringNewName);
		OnSaveDocument(myString);
   		m_strPathName = stringNewName; 
		SetModifiedFlag(FALSE); // back to unmodified
   	}
   	else
   	{   
   		//development-time error, dwError could have various values (see Help On CommDlgExtendedError() ) 
   		dwError = ::CommDlgExtendedError();   
   		if(dwError!=0) ASSERT(FALSE); //if CANCEL button was pressed, there is no error 
   	} 
   	return; 
}

BOOL CLogDocument::CanCloseFrame(CFrameWnd* pFrame)
{
	//COWDocument::CanCloseFrame(pFrame);
	return TRUE;
}



BOOL CLogDocument::OnSaveDocument(const char* pszPathName)
{
	//1) Open a CFile for writing
	CString myString(pszPathName);
	CFile myFile;
	if (!myFile.Open(myString, CFile::modeCreate | CFile::modeWrite)) return FALSE;
	
	//2) Write the text buffer 
	int nCount = m_BufferString.GetLength();  
	CStringA myStringA(m_BufferString);
	myFile.Write((const char*)myStringA, nCount);
	
	//3) Close file
	myFile.Close();
	
	SetModifiedFlag(FALSE); 
	return TRUE;
}

BOOL CLogDocument::SaveModified()
{
	//if (!IsModified()) return TRUE; // ok to continue
	return TRUE;

	CString name = m_strPathName;
	if (name.IsEmpty()) name = GetTitle();

	CString prompt;
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, name);
	switch (AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE))
	{
	case IDCANCEL:
		return FALSE;       // don't continue

	case IDYES:
		// If so, either Save or Update, as appropriate 
		if(m_strPathName.IsEmpty())
		{
			OnFileSaveAs();
			return TRUE;
		}
		if (!DoSave(m_strPathName))
			return FALSE;       // don't continue
		break;

	case IDNO:
		// If not saving changes, revert the document
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;    // keep going
}

void CLogDocument::AddTextW(LPCTSTR lpszString)
{
	CStringA myString(lpszString);
	AddText(myString);
}

void CLogDocument::AddText(LPCSTR lpszString)
{   
	if(m_bEnableLog==FALSE) return;

	if(m_bBusyUpdating) 
	{
		//ASSERT(FALSE);
		return;
	}
	m_bBusyUpdating = TRUE;

	//1) check if enough room to add lpszString 
	CString myString(lpszString); 
	int nStringLength = myString.GetLength();
	int nBufferLength = m_BufferString.GetLength();
	if( ((long)nBufferLength + (long)nStringLength) > (long)LOG_MAXBUFFERLENGTH )
	{   
		//poirier, sept 2001, begin
		/*
		//not enough room, remove minimal amount of characters
		int nCharToRemove = (int)((long)nBufferLength + (long)nStringLength - (long)LOG_MAXBUFFERLENGTH);
		RemoveText(nCharToRemove);
		*/
		ClearAllText();
		//poirier, sept 2001, end
	}
	
	//2) add lpszString
	m_BufferString += myString; 
	
	//3) Update or Not, depending upon the m_bUpdateWhenModified flag
	//2021fev08, spi, begin
	//leave as is
	//2021fev08, spi, end
	if(m_bUpdateWhenModified) UpdateAllViews(NULL);
	
	if(!IsModified()) SetModifiedFlag(TRUE);

	//4) update log.txt file
	if(m_bUpdateFileWhenModified)
	{
		char pszPathName[1024];
		strcpy(pszPathName, pszDefaultLogDirName);
		strcat(pszPathName, pszDefaultLogFileName);

		FILE* pFILE = NULL;
		if( (pFILE = fopen(pszPathName, "a")) == NULL )
		{
			myString.Format(L"     Error, cannot open %s file (directory path may not exist) )\r\n",pszPathName);
			m_BufferString += myString; 
			return;
		}
		//VERIFY(fwrite(lpszString, nStringLength*sizeof(char), 1, pFILE)==1);
		if(pFILE)
		{
			fwrite(lpszString, nStringLength * sizeof(char), 1, pFILE);
			fclose(pFILE);
		}

	}

	m_bBusyUpdating = FALSE;
	return;
}
     
     
void CLogDocument::RemoveText(int nCharToRemove)
{   
	ASSERT(m_BufferString.GetLength() >= nCharToRemove); //development-time error, attempt to remove too many characters 
	m_BufferString = m_BufferString.Mid(nCharToRemove);  
	return;
} 


void CLogDocument::ClearAllText()
{   
	//clear log document
	m_BufferString = "";
	UpdateAllViews(NULL);

	//clear log file
	ClearLogFile();
	return;
}


void CLogDocument::ClearLogFile()
{
	//erase default log file
	char pszPathName[1024];
	strcpy(pszPathName, pszDefaultLogDirName);
	strcat(pszPathName, pszDefaultLogFileName);

	FILE* pFILE = NULL;
	if( (pFILE = fopen(pszPathName, "w")) == NULL )
	{
		CString myString;
		myString.Format(L"     Error, cannot open %s file (directory path may not exist) )\r\n",pszPathName);
		m_BufferString += myString; 
		return;
	}
	if(pFILE)
	{
		fclose(pFILE);
	}
}


void CLogDocument::EnableLog()
{
	m_bEnableLog = TRUE;
}
void CLogDocument::DisableLog()
{
	m_bEnableLog = FALSE;
	return;
}

void CLogDocument::EnableViewUpdating()
{   
	m_bUpdateWhenModified = TRUE;
	UpdateAllViews(NULL);
	return;
} 
void CLogDocument::DisableViewUpdating()
{   
	m_bUpdateWhenModified = FALSE;
	return;
}

void CLogDocument::EnableFileUpdating()
{
	m_bUpdateFileWhenModified = TRUE;
}
void CLogDocument::DisableFileUpdating()
{
	m_bUpdateFileWhenModified = FALSE;
}

void CLogDocument::OnEditClear() 
{
	ClearAllText();
	return;
}
