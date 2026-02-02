rem for debugging 64 bit version, because it is copied last

rem 32 bit versions
rem release
copy .\oifiilib\release\oifiilib.dll .\winrel\oifiilib.dll
copy .\ow_iff\release\ow_iff.dll .\winrel\ow_iff.dll
copy .\ow_ipl\release\ow_ipl.dll .\winrel\ow_ipl.dll
rem debug
copy .\oifiilib\debug\oifiilib.dll .\windebug\oifiilib.dll
copy .\oifiilib\debug\oifiilib.dll .\oifiilib.dll
copy .\ow_iff\debug\ow_iff.dll .\windebug\ow_iff.dll
copy .\ow_iff\debug\ow_iff.dll .\ow_iff.dll
copy .\ow_ipl\debug\ow_ipl.dll .\windebug\ow_ipl.dll
copy .\ow_ipl\debug\ow_ipl.dll .\ow_ipl.dll

rem 64 bit versions
rem release
copy .\oifiilib\x64\release\oifiilib.dll .\x64\release\oifiilib.dll
copy .\ow_iff\x64\release\ow_iff.dll .\x64\release\ow_iff.dll
copy .\ow_ipl\x64\release\ow_ipl.dll .\x64\release\ow_ipl.dll
rem debug
copy .\oifiilib\x64\debug\oifiilib.dll .\x64\debug\oifiilib.dll
copy .\oifiilib\x64\debug\oifiilib.dll .\oifiilib.dll
copy .\ow_iff\x64\debug\ow_iff.dll .\x64\debug\ow_iff.dll
copy .\ow_iff\x64\debug\ow_iff.dll .\ow_iff.dll
copy .\ow_ipl\x64\debug\ow_ipl.dll .\x64\debug\ow_ipl.dll
copy .\ow_ipl\x64\debug\ow_ipl.dll .\ow_ipl.dll
