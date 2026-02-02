@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by OIII.HPJ. >hlp\OIII.hm
echo. >>hlp\OIII.hm
echo // Commands (ID_* and IDM_*) >>hlp\OIII.hm
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>hlp\OIII.hm
echo. >>hlp\OIII.hm
echo // Prompts (IDP_*) >>hlp\OIII.hm
makehm IDP_,HIDP_,0x30000 resource.h >>hlp\OIII.hm
echo. >>hlp\OIII.hm
echo // Resources (IDR_*) >>hlp\OIII.hm
makehm IDR_,HIDR_,0x20000 resource.h >>hlp\OIII.hm
echo. >>hlp\OIII.hm
echo // Dialogs (IDD_*) >>hlp\OIII.hm
makehm IDD_,HIDD_,0x20000 resource.h >>hlp\OIII.hm
echo. >>hlp\OIII.hm
echo // Frame Controls (IDW_*) >>hlp\OIII.hm
makehm IDW_,HIDW_,0x50000 resource.h >>hlp\OIII.hm
REM -- Make help for Project OIII

echo Building Win32 Help files
call hc31 OIII.hpj
if exist windebug copy OIII.hlp windebug
if exist winrel copy OIII.hlp winrel
EndLocal
