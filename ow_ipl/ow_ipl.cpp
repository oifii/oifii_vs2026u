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

// ow_ipl.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "ow_ipl.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}



/////////////////////////////////////////
//ipl image allocate, deallocate and copy
/////////////////////////////////////////
OW_IPL_API IplImage* OW_IPL_iplCreateImageHeader(	int nChannels,  
													int alphaChannel, 
													int  depth,
													char* pCharColorModel, 
													char* pCharChannelSeq,   
													int dataOrder,
													int origin,     
													int align,
													int width,      
													int height, 
													IplROI* pIplROI, 
													IplImage* pIplImageMaskROI,
													void* imageId,    
													IplTileInfo* pIplTileInfo)
{
#if defined _M_IX86
	return iplCreateImageHeader(nChannels,  
								alphaChannel, 
								depth,
								pCharColorModel, 
								pCharChannelSeq,   
								dataOrder,
								origin,     
								align,
								width,      
								height, 
								pIplROI, 
								pIplImageMaskROI,
								imageId,    
								pIplTileInfo);
#elif defined _M_X64
	return NULL;
#endif
}

OW_IPL_API void OW_IPL_iplAllocateImage(IplImage* pIplImage, int doFill, int fillValue)
{
#if defined _M_IX86
	iplAllocateImage(pIplImage, doFill, fillValue);
	return;
#elif defined _M_X64
	return;
#endif
}

OW_IPL_API IplImage* OW_IPL_iplCloneImage(const IplImage* pIplImage)
{
#if defined _M_IX86
	return iplCloneImage(pIplImage);
#elif defined _M_X64
	return NULL;
#endif
}

OW_IPL_API void OW_IPL_iplDeallocateHeader(IplImage* pIplImage)
{
#if defined _M_IX86
	iplDeallocateHeader(pIplImage);
	return;
#elif defined _M_X64
	return;
#endif
}

OW_IPL_API void OW_IPL_iplDeallocateImage(IplImage* pIplImage)
{
#if defined _M_IX86
	iplDeallocateImage(pIplImage);
	return;
#elif defined _M_X64
	return;
#endif
}

OW_IPL_API void OW_IPL_iplDeallocate(IplImage* pIplImage, int flag)
{
#if defined _M_IX86
	iplDeallocate(pIplImage, flag);
	return;
#elif defined _M_X64
	return;
#endif
}

/////////////////////////
//convert to and from dib
/////////////////////////
OW_IPL_API void OW_IPL_iplConvertFromDIB(BITMAPINFOHEADER* pBIH, IplImage* pIplImage)
{
#if defined _M_IX86
	iplConvertFromDIB(pBIH, pIplImage);
	return;
#elif defined _M_X64
	return;
#endif
}


OW_IPL_API void OW_IPL_iplConvertToDIB(	IplImage* pIplImage, 
										BITMAPINFOHEADER* pBIH, 
										int dither, 
										int paletteConversion)
{
#if defined _M_IX86
	iplConvertToDIB(pIplImage, pBIH, dither, paletteConversion);
	return;
#elif defined _M_X64
	return;
#endif
}


OW_IPL_API void OW_IPL_iplConvert(IplImage* pIplImageSource, IplImage* pIplImageDest)
{
#if defined _M_IX86
	iplConvert(pIplImageSource, pIplImageDest);
	return;
#elif defined _M_X64
	return;
#endif
}

////////////////
//filters
////////////////
OW_IPL_API IPLStatus OW_IPL_iplFixedFilter(IplImage* srcImage, 
										   IplImage* dstImage,
										   IplFilter filter)
{
#if defined _M_IX86
	return iplFixedFilter(srcImage, dstImage, filter);
#elif defined _M_X64
	return 0;
#endif
}
