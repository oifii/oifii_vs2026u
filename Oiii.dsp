# Microsoft Developer Studio Project File - Name="OIII" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=OIII - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Oiii.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Oiii.mak" CFG="OIII - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OIII - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "OIII - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OIII - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\WinDebug"
# PROP Intermediate_Dir ".\WinDebug"
# PROP Ignore_Export_Lib 0
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "OWGRAPH_INTERNAL" /D "OIIIAPP" /D "_AFXDLL" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ow_iff.lib ow_ipl.lib opengl32.lib glu32.lib /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "OIII - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\WinRel"
# PROP Intermediate_Dir ".\WinRel"
# PROP Ignore_Export_Lib 0
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "OWGRAPH_INTERNAL" /D "OIIIAPP" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ow_iff.lib ow_ipl.lib opengl32.lib glu32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "OIII - Win32 Debug"
# Name "OIII - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\c_debug.cpp
# End Source File
# Begin Source File

SOURCE=.\c_fileio.cpp
# End Source File
# Begin Source File

SOURCE=.\c_pointset.cpp
# End Source File
# Begin Source File

SOURCE=.\c_segmentset.cpp
# End Source File
# Begin Source File

SOURCE=.\c_view.cpp
# End Source File
# Begin Source File

SOURCE=.\c_xyzi.cpp
# End Source File
# Begin Source File

SOURCE=.\gl_datamdi.cpp
# End Source File
# Begin Source File

SOURCE=.\gl_dataview.cpp
# End Source File
# Begin Source File

SOURCE=.\gl_graphdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\gl_graphmdi.cpp
# End Source File
# Begin Source File

SOURCE=.\gl_graphset.cpp
# End Source File
# Begin Source File

SOURCE=.\gl_graphvw.cpp
# End Source File
# Begin Source File

SOURCE=.\gotodlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LOGDOC.CPP
# End Source File
# Begin Source File

SOURCE=.\LOGMDI.CPP
# End Source File
# Begin Source File

SOURCE=.\LOGVIEW.CPP
# End Source File
# Begin Source File

SOURCE=.\mainfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\oifii_proppage.cpp
# End Source File
# Begin Source File

SOURCE=.\oifii_propsettings.cpp
# End Source File
# Begin Source File

SOURCE=.\oifii_propsheet.cpp
# End Source File
# Begin Source File

SOURCE=.\OIII.cpp
# End Source File
# Begin Source File

SOURCE=.\OIII.DEF
# End Source File
# Begin Source File

SOURCE=.\OIII.rc
# End Source File
# Begin Source File

SOURCE=.\OW2doc.cpp
# End Source File
# Begin Source File

SOURCE=.\ow2mdi.cpp
# End Source File
# Begin Source File

SOURCE=.\OW2view.cpp
# End Source File
# Begin Source File

SOURCE=.\OW3VIEW.CPP
# End Source File
# Begin Source File

SOURCE=.\owclassset.cpp
# End Source File
# Begin Source File

SOURCE=.\owdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\OWedgeset.cpp
# End Source File
# Begin Source File

SOURCE=.\owgl_histo.cpp
# End Source File
# Begin Source File

SOURCE=.\owgl_histovw.cpp
# End Source File
# Begin Source File

SOURCE=.\owgl_profile.cpp
# End Source File
# Begin Source File

SOURCE=.\owgl_profilevw.cpp
# End Source File
# Begin Source File

SOURCE=.\owgl_scatter.cpp
# End Source File
# Begin Source File

SOURCE=.\owgl_scattervw.cpp
# End Source File
# Begin Source File

SOURCE=.\OWmultidoc.cpp
# End Source File
# Begin Source File

SOURCE=.\OWpixelset.cpp
# End Source File
# Begin Source File

SOURCE=.\owpointset.cpp
# End Source File
# Begin Source File

SOURCE=.\owsegmentset.cpp
# End Source File
# Begin Source File

SOURCE=.\OWview.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\c_debug.h
# End Source File
# Begin Source File

SOURCE=.\c_fileio.h
# End Source File
# Begin Source File

SOURCE=.\c_pointset.h
# End Source File
# Begin Source File

SOURCE=.\c_segmentset.h
# End Source File
# Begin Source File

SOURCE=.\c_xyzi.h
# End Source File
# Begin Source File

SOURCE=.\gl_owgraph.h
# End Source File
# Begin Source File

SOURCE=.\gotodlg.h
# End Source File
# Begin Source File

SOURCE=.\LOGDOC.H
# End Source File
# Begin Source File

SOURCE=.\LOGMDI.H
# End Source File
# Begin Source File

SOURCE=.\LOGVIEW.H
# End Source File
# Begin Source File

SOURCE=.\mainfrm.h
# End Source File
# Begin Source File

SOURCE=.\oifii_proppage.h
# End Source File
# Begin Source File

SOURCE=.\oifii_propsettings.h
# End Source File
# Begin Source File

SOURCE=.\oifii_propsheet.h
# End Source File
# Begin Source File

SOURCE=.\oiii.h
# End Source File
# Begin Source File

SOURCE=.\ow2doc.h
# End Source File
# Begin Source File

SOURCE=.\ow2mdi.h
# End Source File
# Begin Source File

SOURCE=.\ow2view.h
# End Source File
# Begin Source File

SOURCE=.\ow3view.h
# End Source File
# Begin Source File

SOURCE=.\ow_iff\ow_iff.h
# End Source File
# Begin Source File

SOURCE=.\ow_ipl.h
# End Source File
# Begin Source File

SOURCE=.\owclassset.h
# End Source File
# Begin Source File

SOURCE=.\owdoc.h
# End Source File
# Begin Source File

SOURCE=.\OWedgeset.h
# End Source File
# Begin Source File

SOURCE=.\owgl_histo.h
# End Source File
# Begin Source File

SOURCE=.\owgl_histovw.h
# End Source File
# Begin Source File

SOURCE=.\owgl_profile.h
# End Source File
# Begin Source File

SOURCE=.\owgl_profilevw.h
# End Source File
# Begin Source File

SOURCE=.\owgl_scatter.h
# End Source File
# Begin Source File

SOURCE=.\owgl_scattervw.h
# End Source File
# Begin Source File

SOURCE=.\OWmultidoc.h
# End Source File
# Begin Source File

SOURCE=.\OWpixelset.h
# End Source File
# Begin Source File

SOURCE=.\owpointset.h
# End Source File
# Begin Source File

SOURCE=.\owsegmentset.h
# End Source File
# Begin Source File

SOURCE=.\owview.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\Res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\Res\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\res\idr_comm.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\RES\IDR_ZOOM.ICO
# End Source File
# Begin Source File

SOURCE=.\res\OIII.ico
# End Source File
# Begin Source File

SOURCE=.\res\OIII.rc2
# End Source File
# Begin Source File

SOURCE=.\res\OW2doc.ico
# End Source File
# Begin Source File

SOURCE=.\Res\roipolyl.cur
# End Source File
# Begin Source File

SOURCE=.\Res\roirect1.cur
# End Source File
# Begin Source File

SOURCE=.\Res\roirectd.cur
# End Source File
# Begin Source File

SOURCE=.\res\toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\ZOOMDRAG.CUR
# End Source File
# Begin Source File

SOURCE=.\RES\ZOOMIN.CUR
# End Source File
# Begin Source File

SOURCE=.\RES\ZOOMOUT.CUR
# End Source File
# Begin Source File

SOURCE=.\RES\ZOOMRECT.CUR
# End Source File
# End Group
# Begin Source File

SOURCE=.\ow_iff\Debug\ow_iff.dll
# End Source File
# Begin Source File

SOURCE=.\ow_ipl\Debug\ow_ipl.dll
# End Source File
# Begin Source File

SOURCE=.\ow_ipl\Debug\ow_ipl.lib
# End Source File
# Begin Source File

SOURCE=.\ow_iff\Debug\ow_iff.lib
# End Source File
# End Target
# End Project
