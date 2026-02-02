# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=OIII - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to OIII - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "OIII - Win32 Debug" && "$(CFG)" != "OIII - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "OIII.mak" CFG="OIII - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OIII - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "OIII - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "OIII - Win32 Release"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "OIII - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : "$(OUTDIR)\OIII.exe" "$(OUTDIR)\OIII.tlb" "$(OUTDIR)\OIII.bsc"

CLEAN : 
	-@erase "$(INTDIR)\behapage.obj"
	-@erase "$(INTDIR)\behapage.sbr"
	-@erase "$(INTDIR)\behasett.obj"
	-@erase "$(INTDIR)\behasett.sbr"
	-@erase "$(INTDIR)\claspage.obj"
	-@erase "$(INTDIR)\claspage.sbr"
	-@erase "$(INTDIR)\classett.obj"
	-@erase "$(INTDIR)\classett.sbr"
	-@erase "$(INTDIR)\DATAMDI.OBJ"
	-@erase "$(INTDIR)\DATAMDI.SBR"
	-@erase "$(INTDIR)\DATAVIEW.OBJ"
	-@erase "$(INTDIR)\DATAVIEW.SBR"
	-@erase "$(INTDIR)\DEBUG.OBJ"
	-@erase "$(INTDIR)\DEBUG.SBR"
	-@erase "$(INTDIR)\extrpage.obj"
	-@erase "$(INTDIR)\extrpage.sbr"
	-@erase "$(INTDIR)\extrsett.obj"
	-@erase "$(INTDIR)\extrsett.sbr"
	-@erase "$(INTDIR)\FILEIO.OBJ"
	-@erase "$(INTDIR)\FILEIO.SBR"
	-@erase "$(INTDIR)\FUSION.OBJ"
	-@erase "$(INTDIR)\FUSION.SBR"
	-@erase "$(INTDIR)\fuspage.obj"
	-@erase "$(INTDIR)\fuspage.sbr"
	-@erase "$(INTDIR)\fussett.obj"
	-@erase "$(INTDIR)\fussett.sbr"
	-@erase "$(INTDIR)\globpage.obj"
	-@erase "$(INTDIR)\globpage.sbr"
	-@erase "$(INTDIR)\globsett.obj"
	-@erase "$(INTDIR)\globsett.sbr"
	-@erase "$(INTDIR)\gotodlg.obj"
	-@erase "$(INTDIR)\gotodlg.sbr"
	-@erase "$(INTDIR)\GRAPHDOC.OBJ"
	-@erase "$(INTDIR)\GRAPHDOC.SBR"
	-@erase "$(INTDIR)\graphmdi.obj"
	-@erase "$(INTDIR)\graphmdi.sbr"
	-@erase "$(INTDIR)\GRAPHSET.OBJ"
	-@erase "$(INTDIR)\GRAPHSET.SBR"
	-@erase "$(INTDIR)\Graphvw.obj"
	-@erase "$(INTDIR)\Graphvw.sbr"
	-@erase "$(INTDIR)\HISTO.OBJ"
	-@erase "$(INTDIR)\HISTO.SBR"
	-@erase "$(INTDIR)\HISTOVW.OBJ"
	-@erase "$(INTDIR)\HISTOVW.SBR"
	-@erase "$(INTDIR)\locpage.obj"
	-@erase "$(INTDIR)\locpage.sbr"
	-@erase "$(INTDIR)\locsett.obj"
	-@erase "$(INTDIR)\locsett.sbr"
	-@erase "$(INTDIR)\LOGDOC.OBJ"
	-@erase "$(INTDIR)\LOGDOC.SBR"
	-@erase "$(INTDIR)\LOGMDI.OBJ"
	-@erase "$(INTDIR)\LOGMDI.SBR"
	-@erase "$(INTDIR)\LOGVIEW.OBJ"
	-@erase "$(INTDIR)\LOGVIEW.SBR"
	-@erase "$(INTDIR)\mainfrm.obj"
	-@erase "$(INTDIR)\mainfrm.sbr"
	-@erase "$(INTDIR)\NEIGHBOR.OBJ"
	-@erase "$(INTDIR)\NEIGHBOR.SBR"
	-@erase "$(INTDIR)\OIII.obj"
	-@erase "$(INTDIR)\OIII.pch"
	-@erase "$(INTDIR)\OIII.res"
	-@erase "$(INTDIR)\OIII.sbr"
	-@erase "$(INTDIR)\OIII.tlb"
	-@erase "$(INTDIR)\OURDOC.OBJ"
	-@erase "$(INTDIR)\OURDOC.SBR"
	-@erase "$(INTDIR)\OW2doc.obj"
	-@erase "$(INTDIR)\OW2doc.sbr"
	-@erase "$(INTDIR)\ow2mdi.obj"
	-@erase "$(INTDIR)\ow2mdi.sbr"
	-@erase "$(INTDIR)\OW2view.obj"
	-@erase "$(INTDIR)\OW2view.sbr"
	-@erase "$(INTDIR)\OW3VIEW.OBJ"
	-@erase "$(INTDIR)\OW3VIEW.SBR"
	-@erase "$(INTDIR)\owpropag.obj"
	-@erase "$(INTDIR)\owpropag.sbr"
	-@erase "$(INTDIR)\OWview.obj"
	-@erase "$(INTDIR)\OWview.sbr"
	-@erase "$(INTDIR)\pixelset.obj"
	-@erase "$(INTDIR)\pixelset.sbr"
	-@erase "$(INTDIR)\POINTSET.OBJ"
	-@erase "$(INTDIR)\POINTSET.SBR"
	-@erase "$(INTDIR)\profpage.obj"
	-@erase "$(INTDIR)\profpage.sbr"
	-@erase "$(INTDIR)\profsett.obj"
	-@erase "$(INTDIR)\profsett.sbr"
	-@erase "$(INTDIR)\projpage.obj"
	-@erase "$(INTDIR)\projpage.sbr"
	-@erase "$(INTDIR)\projsett.obj"
	-@erase "$(INTDIR)\projsett.sbr"
	-@erase "$(INTDIR)\SEGMENT.OBJ"
	-@erase "$(INTDIR)\SEGMENT.SBR"
	-@erase "$(INTDIR)\SEGSET.OBJ"
	-@erase "$(INTDIR)\SEGSET.SBR"
	-@erase "$(INTDIR)\setsheet.obj"
	-@erase "$(INTDIR)\setsheet.sbr"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\stdafx.sbr"
	-@erase "$(INTDIR)\tracpage.obj"
	-@erase "$(INTDIR)\tracpage.sbr"
	-@erase "$(INTDIR)\tracsett.obj"
	-@erase "$(INTDIR)\tracsett.sbr"
	-@erase "$(INTDIR)\tranpage.obj"
	-@erase "$(INTDIR)\tranpage.sbr"
	-@erase "$(INTDIR)\transett.obj"
	-@erase "$(INTDIR)\transett.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\viewpage.obj"
	-@erase "$(INTDIR)\viewpage.sbr"
	-@erase "$(INTDIR)\viewsett.obj"
	-@erase "$(INTDIR)\viewsett.sbr"
	-@erase "$(INTDIR)\XYZI.OBJ"
	-@erase "$(INTDIR)\XYZI.SBR"
	-@erase "$(OUTDIR)\OIII.bsc"
	-@erase "$(OUTDIR)\OIII.exe"
	-@erase "$(OUTDIR)\OIII.ilk"
	-@erase "$(OUTDIR)\OIII.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "OWGRAPH_INTERNAL" /D "OIIIAPP" /Fr /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /D "OWGRAPH_INTERNAL" /D "OIIIAPP" /Fr"$(INTDIR)/"\
 /Fp"$(INTDIR)/OIII.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\WinDebug/
CPP_SBRS=.\WinDebug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/OIII.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/OIII.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\behapage.sbr" \
	"$(INTDIR)\behasett.sbr" \
	"$(INTDIR)\claspage.sbr" \
	"$(INTDIR)\classett.sbr" \
	"$(INTDIR)\DATAMDI.SBR" \
	"$(INTDIR)\DATAVIEW.SBR" \
	"$(INTDIR)\DEBUG.SBR" \
	"$(INTDIR)\extrpage.sbr" \
	"$(INTDIR)\extrsett.sbr" \
	"$(INTDIR)\FILEIO.SBR" \
	"$(INTDIR)\FUSION.SBR" \
	"$(INTDIR)\fuspage.sbr" \
	"$(INTDIR)\fussett.sbr" \
	"$(INTDIR)\globpage.sbr" \
	"$(INTDIR)\globsett.sbr" \
	"$(INTDIR)\gotodlg.sbr" \
	"$(INTDIR)\GRAPHDOC.SBR" \
	"$(INTDIR)\graphmdi.sbr" \
	"$(INTDIR)\GRAPHSET.SBR" \
	"$(INTDIR)\Graphvw.sbr" \
	"$(INTDIR)\HISTO.SBR" \
	"$(INTDIR)\HISTOVW.SBR" \
	"$(INTDIR)\locpage.sbr" \
	"$(INTDIR)\locsett.sbr" \
	"$(INTDIR)\LOGDOC.SBR" \
	"$(INTDIR)\LOGMDI.SBR" \
	"$(INTDIR)\LOGVIEW.SBR" \
	"$(INTDIR)\mainfrm.sbr" \
	"$(INTDIR)\NEIGHBOR.SBR" \
	"$(INTDIR)\OIII.sbr" \
	"$(INTDIR)\OURDOC.SBR" \
	"$(INTDIR)\OW2doc.sbr" \
	"$(INTDIR)\ow2mdi.sbr" \
	"$(INTDIR)\OW2view.sbr" \
	"$(INTDIR)\OW3VIEW.SBR" \
	"$(INTDIR)\owpropag.sbr" \
	"$(INTDIR)\OWview.sbr" \
	"$(INTDIR)\pixelset.sbr" \
	"$(INTDIR)\POINTSET.SBR" \
	"$(INTDIR)\profpage.sbr" \
	"$(INTDIR)\profsett.sbr" \
	"$(INTDIR)\projpage.sbr" \
	"$(INTDIR)\projsett.sbr" \
	"$(INTDIR)\SEGMENT.SBR" \
	"$(INTDIR)\SEGSET.SBR" \
	"$(INTDIR)\setsheet.sbr" \
	"$(INTDIR)\stdafx.sbr" \
	"$(INTDIR)\tracpage.sbr" \
	"$(INTDIR)\tracsett.sbr" \
	"$(INTDIR)\tranpage.sbr" \
	"$(INTDIR)\transett.sbr" \
	"$(INTDIR)\viewpage.sbr" \
	"$(INTDIR)\viewsett.sbr" \
	"$(INTDIR)\XYZI.SBR"

"$(OUTDIR)\OIII.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 accuimr5.lib opengl32.lib glu32.lib /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=accuimr5.lib opengl32.lib glu32.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/OIII.pdb" /debug /machine:I386\
 /def:".\OIII.DEF" /out:"$(OUTDIR)/OIII.exe" 
DEF_FILE= \
	".\OIII.DEF"
LINK32_OBJS= \
	"$(INTDIR)\behapage.obj" \
	"$(INTDIR)\behasett.obj" \
	"$(INTDIR)\claspage.obj" \
	"$(INTDIR)\classett.obj" \
	"$(INTDIR)\DATAMDI.OBJ" \
	"$(INTDIR)\DATAVIEW.OBJ" \
	"$(INTDIR)\DEBUG.OBJ" \
	"$(INTDIR)\extrpage.obj" \
	"$(INTDIR)\extrsett.obj" \
	"$(INTDIR)\FILEIO.OBJ" \
	"$(INTDIR)\FUSION.OBJ" \
	"$(INTDIR)\fuspage.obj" \
	"$(INTDIR)\fussett.obj" \
	"$(INTDIR)\globpage.obj" \
	"$(INTDIR)\globsett.obj" \
	"$(INTDIR)\gotodlg.obj" \
	"$(INTDIR)\GRAPHDOC.OBJ" \
	"$(INTDIR)\graphmdi.obj" \
	"$(INTDIR)\GRAPHSET.OBJ" \
	"$(INTDIR)\Graphvw.obj" \
	"$(INTDIR)\HISTO.OBJ" \
	"$(INTDIR)\HISTOVW.OBJ" \
	"$(INTDIR)\locpage.obj" \
	"$(INTDIR)\locsett.obj" \
	"$(INTDIR)\LOGDOC.OBJ" \
	"$(INTDIR)\LOGMDI.OBJ" \
	"$(INTDIR)\LOGVIEW.OBJ" \
	"$(INTDIR)\mainfrm.obj" \
	"$(INTDIR)\NEIGHBOR.OBJ" \
	"$(INTDIR)\OIII.obj" \
	"$(INTDIR)\OIII.res" \
	"$(INTDIR)\OURDOC.OBJ" \
	"$(INTDIR)\OW2doc.obj" \
	"$(INTDIR)\ow2mdi.obj" \
	"$(INTDIR)\OW2view.obj" \
	"$(INTDIR)\OW3VIEW.OBJ" \
	"$(INTDIR)\owpropag.obj" \
	"$(INTDIR)\OWview.obj" \
	"$(INTDIR)\pixelset.obj" \
	"$(INTDIR)\POINTSET.OBJ" \
	"$(INTDIR)\profpage.obj" \
	"$(INTDIR)\profsett.obj" \
	"$(INTDIR)\projpage.obj" \
	"$(INTDIR)\projsett.obj" \
	"$(INTDIR)\SEGMENT.OBJ" \
	"$(INTDIR)\SEGSET.OBJ" \
	"$(INTDIR)\setsheet.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\tracpage.obj" \
	"$(INTDIR)\tracsett.obj" \
	"$(INTDIR)\tranpage.obj" \
	"$(INTDIR)\transett.obj" \
	"$(INTDIR)\viewpage.obj" \
	"$(INTDIR)\viewsett.obj" \
	"$(INTDIR)\XYZI.OBJ"

"$(OUTDIR)\OIII.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "OIII - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : "$(OUTDIR)\OIII.exe" "$(OUTDIR)\OIII.tlb"

CLEAN : 
	-@erase "$(INTDIR)\behapage.obj"
	-@erase "$(INTDIR)\behasett.obj"
	-@erase "$(INTDIR)\claspage.obj"
	-@erase "$(INTDIR)\classett.obj"
	-@erase "$(INTDIR)\DATAMDI.OBJ"
	-@erase "$(INTDIR)\DATAVIEW.OBJ"
	-@erase "$(INTDIR)\DEBUG.OBJ"
	-@erase "$(INTDIR)\extrpage.obj"
	-@erase "$(INTDIR)\extrsett.obj"
	-@erase "$(INTDIR)\FILEIO.OBJ"
	-@erase "$(INTDIR)\FUSION.OBJ"
	-@erase "$(INTDIR)\fuspage.obj"
	-@erase "$(INTDIR)\fussett.obj"
	-@erase "$(INTDIR)\globpage.obj"
	-@erase "$(INTDIR)\globsett.obj"
	-@erase "$(INTDIR)\gotodlg.obj"
	-@erase "$(INTDIR)\GRAPHDOC.OBJ"
	-@erase "$(INTDIR)\graphmdi.obj"
	-@erase "$(INTDIR)\GRAPHSET.OBJ"
	-@erase "$(INTDIR)\Graphvw.obj"
	-@erase "$(INTDIR)\HISTO.OBJ"
	-@erase "$(INTDIR)\HISTOVW.OBJ"
	-@erase "$(INTDIR)\locpage.obj"
	-@erase "$(INTDIR)\locsett.obj"
	-@erase "$(INTDIR)\LOGDOC.OBJ"
	-@erase "$(INTDIR)\LOGMDI.OBJ"
	-@erase "$(INTDIR)\LOGVIEW.OBJ"
	-@erase "$(INTDIR)\mainfrm.obj"
	-@erase "$(INTDIR)\NEIGHBOR.OBJ"
	-@erase "$(INTDIR)\OIII.obj"
	-@erase "$(INTDIR)\OIII.pch"
	-@erase "$(INTDIR)\OIII.res"
	-@erase "$(INTDIR)\OIII.tlb"
	-@erase "$(INTDIR)\OURDOC.OBJ"
	-@erase "$(INTDIR)\OW2doc.obj"
	-@erase "$(INTDIR)\ow2mdi.obj"
	-@erase "$(INTDIR)\OW2view.obj"
	-@erase "$(INTDIR)\OW3VIEW.OBJ"
	-@erase "$(INTDIR)\owpropag.obj"
	-@erase "$(INTDIR)\OWview.obj"
	-@erase "$(INTDIR)\pixelset.obj"
	-@erase "$(INTDIR)\POINTSET.OBJ"
	-@erase "$(INTDIR)\profpage.obj"
	-@erase "$(INTDIR)\profsett.obj"
	-@erase "$(INTDIR)\projpage.obj"
	-@erase "$(INTDIR)\projsett.obj"
	-@erase "$(INTDIR)\SEGMENT.OBJ"
	-@erase "$(INTDIR)\SEGSET.OBJ"
	-@erase "$(INTDIR)\setsheet.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\tracpage.obj"
	-@erase "$(INTDIR)\tracsett.obj"
	-@erase "$(INTDIR)\tranpage.obj"
	-@erase "$(INTDIR)\transett.obj"
	-@erase "$(INTDIR)\viewpage.obj"
	-@erase "$(INTDIR)\viewsett.obj"
	-@erase "$(INTDIR)\XYZI.OBJ"
	-@erase "$(OUTDIR)\OIII.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "OWGRAPH_INTERNAL" /D "OIIIAPP" /Yu"stdafx.h" /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /D "OWGRAPH_INTERNAL" /D "OIIIAPP" /Fp"$(INTDIR)/OIII.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\WinRel/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/OIII.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/OIII.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 accuimr5.lib opengl32.lib glu32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=accuimr5.lib opengl32.lib glu32.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/OIII.pdb" /machine:I386 /def:".\OIII.DEF"\
 /out:"$(OUTDIR)/OIII.exe" 
DEF_FILE= \
	".\OIII.DEF"
LINK32_OBJS= \
	"$(INTDIR)\behapage.obj" \
	"$(INTDIR)\behasett.obj" \
	"$(INTDIR)\claspage.obj" \
	"$(INTDIR)\classett.obj" \
	"$(INTDIR)\DATAMDI.OBJ" \
	"$(INTDIR)\DATAVIEW.OBJ" \
	"$(INTDIR)\DEBUG.OBJ" \
	"$(INTDIR)\extrpage.obj" \
	"$(INTDIR)\extrsett.obj" \
	"$(INTDIR)\FILEIO.OBJ" \
	"$(INTDIR)\FUSION.OBJ" \
	"$(INTDIR)\fuspage.obj" \
	"$(INTDIR)\fussett.obj" \
	"$(INTDIR)\globpage.obj" \
	"$(INTDIR)\globsett.obj" \
	"$(INTDIR)\gotodlg.obj" \
	"$(INTDIR)\GRAPHDOC.OBJ" \
	"$(INTDIR)\graphmdi.obj" \
	"$(INTDIR)\GRAPHSET.OBJ" \
	"$(INTDIR)\Graphvw.obj" \
	"$(INTDIR)\HISTO.OBJ" \
	"$(INTDIR)\HISTOVW.OBJ" \
	"$(INTDIR)\locpage.obj" \
	"$(INTDIR)\locsett.obj" \
	"$(INTDIR)\LOGDOC.OBJ" \
	"$(INTDIR)\LOGMDI.OBJ" \
	"$(INTDIR)\LOGVIEW.OBJ" \
	"$(INTDIR)\mainfrm.obj" \
	"$(INTDIR)\NEIGHBOR.OBJ" \
	"$(INTDIR)\OIII.obj" \
	"$(INTDIR)\OIII.res" \
	"$(INTDIR)\OURDOC.OBJ" \
	"$(INTDIR)\OW2doc.obj" \
	"$(INTDIR)\ow2mdi.obj" \
	"$(INTDIR)\OW2view.obj" \
	"$(INTDIR)\OW3VIEW.OBJ" \
	"$(INTDIR)\owpropag.obj" \
	"$(INTDIR)\OWview.obj" \
	"$(INTDIR)\pixelset.obj" \
	"$(INTDIR)\POINTSET.OBJ" \
	"$(INTDIR)\profpage.obj" \
	"$(INTDIR)\profsett.obj" \
	"$(INTDIR)\projpage.obj" \
	"$(INTDIR)\projsett.obj" \
	"$(INTDIR)\SEGMENT.OBJ" \
	"$(INTDIR)\SEGSET.OBJ" \
	"$(INTDIR)\setsheet.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\tracpage.obj" \
	"$(INTDIR)\tracsett.obj" \
	"$(INTDIR)\tranpage.obj" \
	"$(INTDIR)\transett.obj" \
	"$(INTDIR)\viewpage.obj" \
	"$(INTDIR)\viewsett.obj" \
	"$(INTDIR)\XYZI.OBJ"

"$(OUTDIR)\OIII.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "OIII - Win32 Debug"
# Name "OIII - Win32 Release"

!IF  "$(CFG)" == "OIII - Win32 Debug"

!ELSEIF  "$(CFG)" == "OIII - Win32 Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\stdafx.cpp
DEP_CPP_STDAF=\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /D "OWGRAPH_INTERNAL" /D "OIIIAPP" /Fr"$(INTDIR)/"\
 /Fp"$(INTDIR)/OIII.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\stdafx.sbr" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\OIII.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "OIII - Win32 Release"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /D "OWGRAPH_INTERNAL" /D "OIIIAPP" /Fp"$(INTDIR)/OIII.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\OIII.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OIII.cpp
DEP_CPP_OIII_=\
	".\behasett.h"\
	".\classett.h"\
	".\edgeset.h"\
	".\extrsett.h"\
	".\fussett.h"\
	".\globpage.h"\
	".\globsett.h"\
	".\gotodlg.h"\
	".\locpage.h"\
	".\locsett.h"\
	".\LOGDOC.H"\
	".\LOGMDI.H"\
	".\LOGVIEW.H"\
	".\mainfrm.h"\
	".\ow3view.h"\
	".\owgraph.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\ow2mdi.h"\
	".\OWGraph\..\ow2view.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\segset.h"\
	".\OWGraph\..\stdafx.h"\
	".\owpropag.h"\
	".\pixelset.h"\
	".\profpage.h"\
	".\profsett.h"\
	".\projsett.h"\
	".\setsheet.h"\
	".\tracsett.h"\
	".\transett.h"\
	".\viewsett.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\OIII.obj" : $(SOURCE) $(DEP_CPP_OIII_) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\OIII.sbr" : $(SOURCE) $(DEP_CPP_OIII_) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\OIII.obj" : $(SOURCE) $(DEP_CPP_OIII_) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mainfrm.cpp
DEP_CPP_MAINF=\
	".\LOGDOC.H"\
	".\mainfrm.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\stdafx.h"\
	".\XYZI.H"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\mainfrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\mainfrm.sbr" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\mainfrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OW2doc.cpp
DEP_CPP_OW2DO=\
	".\behasett.h"\
	".\classett.h"\
	".\DEBUG.H"\
	".\edgeset.h"\
	".\extrsett.h"\
	".\FILEIO.H"\
	".\fussett.h"\
	".\globsett.h"\
	".\gotodlg.h"\
	".\IMG_ERR.H"\
	".\IMGLIB.H"\
	".\locsett.h"\
	".\LOGDOC.H"\
	".\ow3view.h"\
	".\owgraph.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\ow2mdi.h"\
	".\OWGraph\..\ow2view.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\segset.h"\
	".\OWGraph\..\stdafx.h"\
	".\pixelset.h"\
	".\profsett.h"\
	".\projsett.h"\
	".\setsheet.h"\
	".\tracsett.h"\
	".\transett.h"\
	".\viewsett.h"\
	".\XYZI.H"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\OW2doc.obj" : $(SOURCE) $(DEP_CPP_OW2DO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\OW2doc.sbr" : $(SOURCE) $(DEP_CPP_OW2DO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\OW2doc.obj" : $(SOURCE) $(DEP_CPP_OW2DO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OW2view.cpp
DEP_CPP_OW2VI=\
	".\behasett.h"\
	".\edgeset.h"\
	".\gotodlg.h"\
	".\IMG_ERR.H"\
	".\IMGLIB.H"\
	".\LOGDOC.H"\
	".\mainfrm.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\ow2mdi.h"\
	".\OWGraph\..\ow2view.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\segment.h"\
	".\OWGraph\..\segset.h"\
	".\OWGraph\..\stdafx.h"\
	".\pixelset.h"\
	".\projsett.h"\
	".\setsheet.h"\
	".\transett.h"\
	".\viewsett.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\OW2view.obj" : $(SOURCE) $(DEP_CPP_OW2VI) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\OW2view.sbr" : $(SOURCE) $(DEP_CPP_OW2VI) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\OW2view.obj" : $(SOURCE) $(DEP_CPP_OW2VI) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OIII.rc
DEP_RSC_OIII_R=\
	".\RES\IDR_ZOOM.ICO"\
	".\res\OIII.ico"\
	".\res\OIII.rc2"\
	".\res\OW2doc.ico"\
	".\res\toolbar.bmp"\
	".\RES\ZOOMDRAG.CUR"\
	".\RES\ZOOMIN.CUR"\
	".\RES\ZOOMOUT.CUR"\
	".\RES\ZOOMRECT.CUR"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\OIII.res" : $(SOURCE) $(DEP_RSC_OIII_R) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/OIII.res" /i "WinDebug" /d "_DEBUG" /d\
 "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\OIII.res" : $(SOURCE) $(DEP_RSC_OIII_R) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/OIII.res" /i "WinRel" /d "NDEBUG" /d "_AFXDLL"\
 $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\readme.txt

!IF  "$(CFG)" == "OIII - Win32 Debug"

!ELSEIF  "$(CFG)" == "OIII - Win32 Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OIII.odl

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(OUTDIR)\OIII.tlb" : $(SOURCE) "$(OUTDIR)"
   $(MTL) /nologo /D "_DEBUG" /tlb "$(OUTDIR)/OIII.tlb" /win32 $(SOURCE)


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(OUTDIR)\OIII.tlb" : $(SOURCE) "$(OUTDIR)"
   $(MTL) /nologo /D "NDEBUG" /tlb "$(OUTDIR)/OIII.tlb" /win32 $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\POINTSET.CPP
DEP_CPP_POINT=\
	".\LOGDOC.H"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\POINTSET.OBJ" : $(SOURCE) $(DEP_CPP_POINT) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\POINTSET.SBR" : $(SOURCE) $(DEP_CPP_POINT) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\POINTSET.OBJ" : $(SOURCE) $(DEP_CPP_POINT) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\XYZI.CPP
DEP_CPP_XYZI_=\
	".\OWGraph\..\stdafx.h"\
	".\XYZI.H"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\XYZI.OBJ" : $(SOURCE) $(DEP_CPP_XYZI_) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\XYZI.SBR" : $(SOURCE) $(DEP_CPP_XYZI_) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\XYZI.OBJ" : $(SOURCE) $(DEP_CPP_XYZI_) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LOGDOC.CPP
DEP_CPP_LOGDO=\
	".\LOGDOC.H"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\LOGDOC.OBJ" : $(SOURCE) $(DEP_CPP_LOGDO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\LOGDOC.SBR" : $(SOURCE) $(DEP_CPP_LOGDO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\LOGDOC.OBJ" : $(SOURCE) $(DEP_CPP_LOGDO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LOGVIEW.CPP
DEP_CPP_LOGVI=\
	".\LOGDOC.H"\
	".\LOGMDI.H"\
	".\LOGVIEW.H"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\LOGVIEW.OBJ" : $(SOURCE) $(DEP_CPP_LOGVI) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\LOGVIEW.SBR" : $(SOURCE) $(DEP_CPP_LOGVI) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\LOGVIEW.OBJ" : $(SOURCE) $(DEP_CPP_LOGVI) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LOGMDI.CPP
DEP_CPP_LOGMD=\
	".\LOGMDI.H"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\LOGMDI.OBJ" : $(SOURCE) $(DEP_CPP_LOGMD) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\LOGMDI.SBR" : $(SOURCE) $(DEP_CPP_LOGMD) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\LOGMDI.OBJ" : $(SOURCE) $(DEP_CPP_LOGMD) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OURDOC.CPP
DEP_CPP_OURDO=\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\OURDOC.OBJ" : $(SOURCE) $(DEP_CPP_OURDO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\OURDOC.SBR" : $(SOURCE) $(DEP_CPP_OURDO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\OURDOC.OBJ" : $(SOURCE) $(DEP_CPP_OURDO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ow2mdi.cpp
DEP_CPP_OW2MD=\
	".\gotodlg.h"\
	".\mainfrm.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\ow2mdi.h"\
	".\OWGraph\..\ow2view.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\stdafx.h"\
	".\pixelset.h"\
	".\setsheet.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\ow2mdi.obj" : $(SOURCE) $(DEP_CPP_OW2MD) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\ow2mdi.sbr" : $(SOURCE) $(DEP_CPP_OW2MD) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\ow2mdi.obj" : $(SOURCE) $(DEP_CPP_OW2MD) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SEGMENT.CPP
DEP_CPP_SEGME=\
	".\LOGDOC.H"\
	".\NEIGHBOR.H"\
	".\OWGraph\..\fusion.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\segment.h"\
	".\OWGraph\..\segset.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\SEGMENT.OBJ" : $(SOURCE) $(DEP_CPP_SEGME) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\SEGMENT.SBR" : $(SOURCE) $(DEP_CPP_SEGME) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\SEGMENT.OBJ" : $(SOURCE) $(DEP_CPP_SEGME) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FUSION.CPP
DEP_CPP_FUSIO=\
	".\NEIGHBOR.H"\
	".\OWGraph\..\fusion.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\segment.h"\
	".\OWGraph\..\segset.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\FUSION.OBJ" : $(SOURCE) $(DEP_CPP_FUSIO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\FUSION.SBR" : $(SOURCE) $(DEP_CPP_FUSIO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\FUSION.OBJ" : $(SOURCE) $(DEP_CPP_FUSIO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\NEIGHBOR.CPP
DEP_CPP_NEIGH=\
	".\NEIGHBOR.H"\
	".\OWGraph\..\fusion.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\segment.h"\
	".\OWGraph\..\segset.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\NEIGHBOR.OBJ" : $(SOURCE) $(DEP_CPP_NEIGH) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\NEIGHBOR.SBR" : $(SOURCE) $(DEP_CPP_NEIGH) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\NEIGHBOR.OBJ" : $(SOURCE) $(DEP_CPP_NEIGH) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SEGSET.CPP
DEP_CPP_SEGSE=\
	".\NEIGHBOR.H"\
	".\OWGraph\..\fusion.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\segment.h"\
	".\OWGraph\..\segset.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\SEGSET.OBJ" : $(SOURCE) $(DEP_CPP_SEGSE) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\SEGSET.SBR" : $(SOURCE) $(DEP_CPP_SEGSE) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\SEGSET.OBJ" : $(SOURCE) $(DEP_CPP_SEGSE) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FILEIO.CPP
DEP_CPP_FILEI=\
	".\NEIGHBOR.H"\
	".\OWGraph\..\fusion.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\segment.h"\
	".\OWGraph\..\segset.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\FILEIO.OBJ" : $(SOURCE) $(DEP_CPP_FILEI) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\FILEIO.SBR" : $(SOURCE) $(DEP_CPP_FILEI) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\FILEIO.OBJ" : $(SOURCE) $(DEP_CPP_FILEI) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DEBUG.CPP
DEP_CPP_DEBUG=\
	".\DEBUG.H"\
	".\NEIGHBOR.H"\
	".\OWGraph\..\fusion.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\segment.h"\
	".\OWGraph\..\segset.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\DEBUG.OBJ" : $(SOURCE) $(DEP_CPP_DEBUG) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\DEBUG.SBR" : $(SOURCE) $(DEP_CPP_DEBUG) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\DEBUG.OBJ" : $(SOURCE) $(DEP_CPP_DEBUG) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\gotodlg.cpp
DEP_CPP_GOTOD=\
	".\gotodlg.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\ow2mdi.h"\
	".\OWGraph\..\ow2view.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\segset.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\gotodlg.obj" : $(SOURCE) $(DEP_CPP_GOTOD) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\gotodlg.sbr" : $(SOURCE) $(DEP_CPP_GOTOD) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\gotodlg.obj" : $(SOURCE) $(DEP_CPP_GOTOD) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OIII.DEF

!IF  "$(CFG)" == "OIII - Win32 Debug"

!ELSEIF  "$(CFG)" == "OIII - Win32 Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\pixelset.cpp
DEP_CPP_PIXEL=\
	".\IMG_ERR.H"\
	".\IMGLIB.H"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\stdafx.h"\
	".\pixelset.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\pixelset.obj" : $(SOURCE) $(DEP_CPP_PIXEL) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\pixelset.sbr" : $(SOURCE) $(DEP_CPP_PIXEL) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\pixelset.obj" : $(SOURCE) $(DEP_CPP_PIXEL) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OW3VIEW.CPP
DEP_CPP_OW3VI=\
	".\edgeset.h"\
	".\ow3view.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\segment.h"\
	".\OWGraph\..\segset.h"\
	".\OWGraph\..\stdafx.h"\
	".\pixelset.h"\
	".\projsett.h"\
	".\setsheet.h"\
	".\transett.h"\
	".\viewsett.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\OW3VIEW.OBJ" : $(SOURCE) $(DEP_CPP_OW3VI) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\OW3VIEW.SBR" : $(SOURCE) $(DEP_CPP_OW3VI) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\OW3VIEW.OBJ" : $(SOURCE) $(DEP_CPP_OW3VI) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OWview.cpp
DEP_CPP_OWVIE=\
	".\behasett.h"\
	".\classett.h"\
	".\edgeset.h"\
	".\extrsett.h"\
	".\fussett.h"\
	".\globsett.h"\
	".\gotodlg.h"\
	".\IMG_ERR.H"\
	".\IMGLIB.H"\
	".\locsett.h"\
	".\LOGDOC.H"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\ow2mdi.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\segment.h"\
	".\OWGraph\..\segset.h"\
	".\OWGraph\..\stdafx.h"\
	".\pixelset.h"\
	".\projsett.h"\
	".\setsheet.h"\
	".\tracsett.h"\
	".\transett.h"\
	".\viewsett.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\OWview.obj" : $(SOURCE) $(DEP_CPP_OWVIE) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\OWview.sbr" : $(SOURCE) $(DEP_CPP_OWVIE) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\OWview.obj" : $(SOURCE) $(DEP_CPP_OWVIE) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\setsheet.cpp
DEP_CPP_SETSH=\
	".\behapage.h"\
	".\behasett.h"\
	".\claspage.h"\
	".\classett.h"\
	".\extrpage.h"\
	".\extrsett.h"\
	".\fuspage.h"\
	".\fussett.h"\
	".\globpage.h"\
	".\globsett.h"\
	".\locpage.h"\
	".\locsett.h"\
	".\ow3view.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\ow2mdi.h"\
	".\OWGraph\..\ow2view.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\stdafx.h"\
	".\owpropag.h"\
	".\profpage.h"\
	".\profsett.h"\
	".\projpage.h"\
	".\projsett.h"\
	".\setsheet.h"\
	".\tracpage.h"\
	".\tracsett.h"\
	".\tranpage.h"\
	".\transett.h"\
	".\viewpage.h"\
	".\viewsett.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\setsheet.obj" : $(SOURCE) $(DEP_CPP_SETSH) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\setsheet.sbr" : $(SOURCE) $(DEP_CPP_SETSH) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\setsheet.obj" : $(SOURCE) $(DEP_CPP_SETSH) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\viewpage.cpp
DEP_CPP_VIEWP=\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\stdafx.h"\
	".\owpropag.h"\
	".\setsheet.h"\
	".\viewpage.h"\
	".\viewsett.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\viewpage.obj" : $(SOURCE) $(DEP_CPP_VIEWP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\viewpage.sbr" : $(SOURCE) $(DEP_CPP_VIEWP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\viewpage.obj" : $(SOURCE) $(DEP_CPP_VIEWP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\extrpage.cpp
DEP_CPP_EXTRP=\
	".\extrpage.h"\
	".\extrsett.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\stdafx.h"\
	".\owpropag.h"\
	".\setsheet.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\extrpage.obj" : $(SOURCE) $(DEP_CPP_EXTRP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\extrpage.sbr" : $(SOURCE) $(DEP_CPP_EXTRP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\extrpage.obj" : $(SOURCE) $(DEP_CPP_EXTRP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\fuspage.cpp
DEP_CPP_FUSPA=\
	".\fuspage.h"\
	".\fussett.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\stdafx.h"\
	".\owpropag.h"\
	".\setsheet.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\fuspage.obj" : $(SOURCE) $(DEP_CPP_FUSPA) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\fuspage.sbr" : $(SOURCE) $(DEP_CPP_FUSPA) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\fuspage.obj" : $(SOURCE) $(DEP_CPP_FUSPA) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\globpage.cpp
DEP_CPP_GLOBP=\
	".\globpage.h"\
	".\globsett.h"\
	".\HISTO.H"\
	".\HISTOVW.H"\
	".\owgraph.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\ow2mdi.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\stdafx.h"\
	".\owpropag.h"\
	".\setsheet.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\globpage.obj" : $(SOURCE) $(DEP_CPP_GLOBP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\globpage.sbr" : $(SOURCE) $(DEP_CPP_GLOBP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\globpage.obj" : $(SOURCE) $(DEP_CPP_GLOBP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\locpage.cpp
DEP_CPP_LOCPA=\
	".\HISTO.H"\
	".\HISTOVW.H"\
	".\locpage.h"\
	".\locsett.h"\
	".\owgraph.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\ow2mdi.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\stdafx.h"\
	".\owpropag.h"\
	".\setsheet.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\locpage.obj" : $(SOURCE) $(DEP_CPP_LOCPA) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\locpage.sbr" : $(SOURCE) $(DEP_CPP_LOCPA) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\locpage.obj" : $(SOURCE) $(DEP_CPP_LOCPA) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\projpage.cpp
DEP_CPP_PROJP=\
	".\ow3view.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\stdafx.h"\
	".\owpropag.h"\
	".\projpage.h"\
	".\projsett.h"\
	".\setsheet.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\projpage.obj" : $(SOURCE) $(DEP_CPP_PROJP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\projpage.sbr" : $(SOURCE) $(DEP_CPP_PROJP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\projpage.obj" : $(SOURCE) $(DEP_CPP_PROJP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tranpage.cpp
DEP_CPP_TRANP=\
	".\ow3view.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\stdafx.h"\
	".\owpropag.h"\
	".\setsheet.h"\
	".\tranpage.h"\
	".\transett.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\tranpage.obj" : $(SOURCE) $(DEP_CPP_TRANP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\tranpage.sbr" : $(SOURCE) $(DEP_CPP_TRANP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\tranpage.obj" : $(SOURCE) $(DEP_CPP_TRANP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\claspage.cpp
DEP_CPP_CLASP=\
	".\claspage.h"\
	".\classett.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\stdafx.h"\
	".\owpropag.h"\
	".\setsheet.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\claspage.obj" : $(SOURCE) $(DEP_CPP_CLASP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\claspage.sbr" : $(SOURCE) $(DEP_CPP_CLASP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\claspage.obj" : $(SOURCE) $(DEP_CPP_CLASP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tracpage.cpp
DEP_CPP_TRACP=\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\stdafx.h"\
	".\owpropag.h"\
	".\setsheet.h"\
	".\tracpage.h"\
	".\tracsett.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\tracpage.obj" : $(SOURCE) $(DEP_CPP_TRACP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\tracpage.sbr" : $(SOURCE) $(DEP_CPP_TRACP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\tracpage.obj" : $(SOURCE) $(DEP_CPP_TRACP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\behapage.cpp
DEP_CPP_BEHAP=\
	".\behapage.h"\
	".\behasett.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\stdafx.h"\
	".\owpropag.h"\
	".\setsheet.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\behapage.obj" : $(SOURCE) $(DEP_CPP_BEHAP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\behapage.sbr" : $(SOURCE) $(DEP_CPP_BEHAP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\behapage.obj" : $(SOURCE) $(DEP_CPP_BEHAP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\behasett.cpp
DEP_CPP_BEHAS=\
	".\behasett.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\behasett.obj" : $(SOURCE) $(DEP_CPP_BEHAS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\behasett.sbr" : $(SOURCE) $(DEP_CPP_BEHAS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\behasett.obj" : $(SOURCE) $(DEP_CPP_BEHAS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\viewsett.cpp
DEP_CPP_VIEWS=\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\stdafx.h"\
	".\viewsett.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\viewsett.obj" : $(SOURCE) $(DEP_CPP_VIEWS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\viewsett.sbr" : $(SOURCE) $(DEP_CPP_VIEWS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\viewsett.obj" : $(SOURCE) $(DEP_CPP_VIEWS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\transett.cpp
DEP_CPP_TRANS=\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\stdafx.h"\
	".\transett.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\transett.obj" : $(SOURCE) $(DEP_CPP_TRANS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\transett.sbr" : $(SOURCE) $(DEP_CPP_TRANS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\transett.obj" : $(SOURCE) $(DEP_CPP_TRANS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tracsett.cpp
DEP_CPP_TRACS=\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\stdafx.h"\
	".\tracsett.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\tracsett.obj" : $(SOURCE) $(DEP_CPP_TRACS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\tracsett.sbr" : $(SOURCE) $(DEP_CPP_TRACS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\tracsett.obj" : $(SOURCE) $(DEP_CPP_TRACS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\projsett.cpp
DEP_CPP_PROJS=\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\stdafx.h"\
	".\projsett.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\projsett.obj" : $(SOURCE) $(DEP_CPP_PROJS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\projsett.sbr" : $(SOURCE) $(DEP_CPP_PROJS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\projsett.obj" : $(SOURCE) $(DEP_CPP_PROJS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\locsett.cpp
DEP_CPP_LOCSE=\
	".\locsett.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\locsett.obj" : $(SOURCE) $(DEP_CPP_LOCSE) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\locsett.sbr" : $(SOURCE) $(DEP_CPP_LOCSE) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\locsett.obj" : $(SOURCE) $(DEP_CPP_LOCSE) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\globsett.cpp
DEP_CPP_GLOBS=\
	".\globsett.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\globsett.obj" : $(SOURCE) $(DEP_CPP_GLOBS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\globsett.sbr" : $(SOURCE) $(DEP_CPP_GLOBS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\globsett.obj" : $(SOURCE) $(DEP_CPP_GLOBS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\fussett.cpp
DEP_CPP_FUSSE=\
	".\fussett.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\fussett.obj" : $(SOURCE) $(DEP_CPP_FUSSE) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\fussett.sbr" : $(SOURCE) $(DEP_CPP_FUSSE) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\fussett.obj" : $(SOURCE) $(DEP_CPP_FUSSE) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\extrsett.cpp
DEP_CPP_EXTRS=\
	".\extrsett.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\extrsett.obj" : $(SOURCE) $(DEP_CPP_EXTRS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\extrsett.sbr" : $(SOURCE) $(DEP_CPP_EXTRS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\extrsett.obj" : $(SOURCE) $(DEP_CPP_EXTRS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\classett.cpp
DEP_CPP_CLASS=\
	".\classett.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\classett.obj" : $(SOURCE) $(DEP_CPP_CLASS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\classett.sbr" : $(SOURCE) $(DEP_CPP_CLASS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\classett.obj" : $(SOURCE) $(DEP_CPP_CLASS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\owpropag.cpp
DEP_CPP_OWPRO=\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\stdafx.h"\
	".\owpropag.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\owpropag.obj" : $(SOURCE) $(DEP_CPP_OWPRO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\owpropag.sbr" : $(SOURCE) $(DEP_CPP_OWPRO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\owpropag.obj" : $(SOURCE) $(DEP_CPP_OWPRO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\profpage.cpp
DEP_CPP_PROFP=\
	".\HISTO.H"\
	".\HISTOVW.H"\
	".\owgraph.h"\
	".\OWGraph\..\fusion.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\ow2mdi.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\stdafx.h"\
	".\owpropag.h"\
	".\profpage.h"\
	".\profsett.h"\
	".\setsheet.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\profpage.obj" : $(SOURCE) $(DEP_CPP_PROFP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\profpage.sbr" : $(SOURCE) $(DEP_CPP_PROFP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\profpage.obj" : $(SOURCE) $(DEP_CPP_PROFP) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\profsett.cpp
DEP_CPP_PROFS=\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\stdafx.h"\
	".\profsett.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\profsett.obj" : $(SOURCE) $(DEP_CPP_PROFS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\profsett.sbr" : $(SOURCE) $(DEP_CPP_PROFS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\profsett.obj" : $(SOURCE) $(DEP_CPP_PROFS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\HISTO.CPP
DEP_CPP_HISTO=\
	".\HISTO.H"\
	".\HISTOVW.H"\
	".\owgraph.h"\
	".\OWGraph\..\fusion.h"\
	".\OWGraph\..\ourdoc.h"\
	".\OWGraph\..\ow2doc.h"\
	".\OWGraph\..\ow2mdi.h"\
	".\OWGraph\..\ow2view.h"\
	".\OWGraph\..\owview.h"\
	".\OWGraph\..\pointset.h"\
	".\OWGraph\..\segment.h"\
	".\OWGraph\..\segset.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\HISTO.OBJ" : $(SOURCE) $(DEP_CPP_HISTO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\HISTO.SBR" : $(SOURCE) $(DEP_CPP_HISTO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\HISTO.OBJ" : $(SOURCE) $(DEP_CPP_HISTO) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\HISTOVW.CPP
DEP_CPP_HISTOV=\
	".\HISTOVW.H"\
	".\owgraph.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\HISTOVW.OBJ" : $(SOURCE) $(DEP_CPP_HISTOV) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\HISTOVW.SBR" : $(SOURCE) $(DEP_CPP_HISTOV) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\HISTOVW.OBJ" : $(SOURCE) $(DEP_CPP_HISTOV) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Graphvw.cpp
DEP_CPP_GRAPH=\
	".\owgraph.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\Graphvw.obj" : $(SOURCE) $(DEP_CPP_GRAPH) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\Graphvw.sbr" : $(SOURCE) $(DEP_CPP_GRAPH) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\Graphvw.obj" : $(SOURCE) $(DEP_CPP_GRAPH) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\graphmdi.cpp
DEP_CPP_GRAPHM=\
	".\owgraph.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\graphmdi.obj" : $(SOURCE) $(DEP_CPP_GRAPHM) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\graphmdi.sbr" : $(SOURCE) $(DEP_CPP_GRAPHM) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\graphmdi.obj" : $(SOURCE) $(DEP_CPP_GRAPHM) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\GRAPHSET.CPP
DEP_CPP_GRAPHS=\
	".\owgraph.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\GRAPHSET.OBJ" : $(SOURCE) $(DEP_CPP_GRAPHS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\GRAPHSET.SBR" : $(SOURCE) $(DEP_CPP_GRAPHS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\GRAPHSET.OBJ" : $(SOURCE) $(DEP_CPP_GRAPHS) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\GRAPHDOC.CPP
DEP_CPP_GRAPHD=\
	".\owgraph.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\GRAPHDOC.OBJ" : $(SOURCE) $(DEP_CPP_GRAPHD) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\GRAPHDOC.SBR" : $(SOURCE) $(DEP_CPP_GRAPHD) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\GRAPHDOC.OBJ" : $(SOURCE) $(DEP_CPP_GRAPHD) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DATAVIEW.CPP
DEP_CPP_DATAV=\
	".\owgraph.h"\
	".\OWGraph\..\oiii.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\DATAVIEW.OBJ" : $(SOURCE) $(DEP_CPP_DATAV) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\DATAVIEW.SBR" : $(SOURCE) $(DEP_CPP_DATAV) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\DATAVIEW.OBJ" : $(SOURCE) $(DEP_CPP_DATAV) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DATAMDI.CPP
DEP_CPP_DATAM=\
	".\owgraph.h"\
	".\OWGraph\..\stdafx.h"\
	

!IF  "$(CFG)" == "OIII - Win32 Debug"


"$(INTDIR)\DATAMDI.OBJ" : $(SOURCE) $(DEP_CPP_DATAM) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"

"$(INTDIR)\DATAMDI.SBR" : $(SOURCE) $(DEP_CPP_DATAM) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ELSEIF  "$(CFG)" == "OIII - Win32 Release"


"$(INTDIR)\DATAMDI.OBJ" : $(SOURCE) $(DEP_CPP_DATAM) "$(INTDIR)"\
 "$(INTDIR)\OIII.pch"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
