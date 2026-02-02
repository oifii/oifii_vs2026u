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
// logdoc.h : definition for the class CLogDocument
//
// Stephane Poirier, jan 1996 
///////////////////////////////////////////////////


#ifndef _LOGDOC_H
#define _LOGDOC_H

   
   
//////////////////////
// Log Document Macros
//////////////////////

#define LOG_MAXBUFFERLENGTH      64000


#include "owdoc.h" 



//const char pszDefaultLogDirName[] = {"c:\\"};
const char pszDefaultLogDirName[] = {".\\"};
const char pszDefaultLogFileName[] = {"log.txt"};




class OIFIILIB_API CLogDocument : public COWDocument
{
	DECLARE_DYNCREATE(CLogDocument)
// Attributes
public:    
	CString m_BufferString;

private: 
	BOOL m_bUpdateWhenModified; //if TRUE, views are updated everytime AddText() is used. 
	BOOL m_bBusyUpdating; //set to TRUE when adding text
	BOOL m_bUpdateFileWhenModified; //if TRUE, default log.txt file is updated everytime AddText() is used
	   
// Operations
// Implementation
public:
	CLogDocument();  
	~CLogDocument(); 
	void AddTextW(LPCTSTR lpszString);
	void AddText(LPCSTR lpszString);
	void ClearAllText();
	void ClearLogFile();
	//log buffer, both log edit view and log file
	BOOL m_bEnableLog;
	void EnableLog();
	void DisableLog();
	//log edit view
	void EnableViewUpdating();
	void DisableViewUpdating();
	//log file
	void EnableFileUpdating();
	void DisableFileUpdating();
	
private:
	void RemoveText(int nCharToRemove);   
	
protected:                                                                   
	virtual	BOOL OnNewDocument();
    virtual BOOL OnSaveDocument(const char* pszPathName);
public:    
	virtual BOOL SaveModified(); 
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);

	// Generated message map functions
protected:
	//{{AFX_MSG(CLogDocument)
	afx_msg void OnFileSave(); 
	afx_msg void OnFileSaveAs();
	afx_msg void OnEditClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //_LOGDOC_H
