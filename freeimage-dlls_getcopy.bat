goto LABEL1

rem 2021nov29 freeimage_vs2017 does not compile in debug
rem
rem 32bit
rem 32bit
rem 32bit
rem debug dlls
rem copy ..\lib-src\freeimage_vs2017\Dist\x32\FreeImage.dll FreeImage.dll
rem copy ..\lib-src\freeimage_vs2017\Wrapper\FreeImagePlus\dist\x32\FreeImagePlus.dll FreeImagePlus.dll
rem copy ..\lib-src\freeimage_vs2017\Dist\x32\FreeImage.dll windebug\FreeImage.dll
rem copy ..\lib-src\freeimage_vs2017\Wrapper\FreeImagePlus\dist\x32\FreeImagePlus.dll windebug\FreeImagePlus.dll
rem release dlls
rem copy ..\lib-src\freeimage_vs2017\Dist\x32\FreeImage.dll winrel\FreeImage.dll
rem copy ..\lib-src\freeimage_vs2017\Wrapper\FreeImagePlus\dist\x32\FreeImagePlus.dll winrel\FreeImagePlus.dll
rem
rem
rem 64bit
rem 64bit
rem 64bit
rem debug dlls
rem copy ..\lib-src\freeimage_vs2017(x64)\Dist\x64\FreeImage.dll x64\debug\FreeImage.dll
rem copy ..\lib-src\freeimage_vs2017(x64)\Wrapper\FreeImagePlus\dist\x64\FreeImagePlus.dll x64\debug\FreeImagePlus.dll
rem copy ..\lib-src\freeimage_vs2017(x64)\Dist\x64\FreeImage.dll x64\debug\FreeImage.dll
rem copy ..\lib-src\freeimage_vs2017(x64)\Wrapper\FreeImagePlus\dist\x64\FreeImagePlus.dll x64\debug\FreeImagePlus.dll
rem release dlls
rem copy ..\lib-src\freeimage_vs2017(x64)\Dist\x64\FreeImage.dll x64\release\FreeImage.dll
rem copy ..\lib-src\freeimage_vs2017(x64)\Wrapper\FreeImagePlus\dist\x64\FreeImagePlus.dll x64\release\FreeImagePlus.dll
rem exit


:LABEL1
rem 2021nov30 prebuilt freeimage3180_vs2017 instead
rem 32bit
rem 32bit
rem 32bit
rem debug dlls
copy ..\freeimage3180_vs2017\Dist\x32\FreeImage.dll FreeImage.dll
copy ..\freeimage3180_vs2017\Wrapper\FreeImagePlus\dist\x32\FreeImagePlusd.dll FreeImagePlusd.dll
copy ..\freeimage3180_vs2017\Dist\x32\FreeImage.dll windebug\FreeImage.dll
copy ..\freeimage3180_vs2017\Wrapper\FreeImagePlus\dist\x32\FreeImagePlusd.dll windebug\FreeImagePlusd.dll
rem release dlls
copy ..\freeimage3180_vs2017\Dist\x32\FreeImage.dll winrel\FreeImage.dll
copy ..\freeimage3180_vs2017\Wrapper\FreeImagePlus\dist\x32\FreeImagePlus.dll winrel\FreeImagePlus.dll
rem 64bit
rem 64bit
rem 64bit
rem debug dlls
copy ..\freeimage3180_vs2017\Dist\x64\FreeImage.dll FreeImage.dll
copy ..\freeimage3180_vs2017\Wrapper\FreeImagePlus\dist\x64\FreeImagePlusd.dll FreeImagePlusd.dll
copy ..\freeimage3180_vs2017\Dist\x64\FreeImage.dll x64\debug\FreeImage.dll
copy ..\freeimage3180_vs2017\Wrapper\FreeImagePlus\dist\x64\FreeImagePlusd.dll x64\debug\FreeImagePlusd.dll
rem release dlls
copy ..\freeimage3180_vs2017\Dist\x64\FreeImage.dll x64\release\FreeImage.dll
copy ..\freeimage3180_vs2017\Wrapper\FreeImagePlus\dist\x64\FreeImagePlus.dll x64\release\FreeImagePlus.dll
exit
