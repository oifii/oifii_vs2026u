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

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E404F2F4_A913_11D5_A430_0050BAAE704B__INCLUDED_)
#define AFX_STDAFX_H__E404F2F4_A913_11D5_A430_0050BAAE704B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Interaction between your application and the browser: To access the content of your browser directly, you have to import the IE Typelibrary like this in your stdafx.h:
#pragma warning(disable:4192)
#pragma warning(disable:4049)
//#import "E:\WINDOWS\system32\mshtml.tlb"
//#import "d:\WINDOWS\system32\mshtml.tlb"
#import "c:\WINDOWS\system32\mshtml.tlb"

// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E404F2F4_A913_11D5_A430_0050BAAE704B__INCLUDED_)
