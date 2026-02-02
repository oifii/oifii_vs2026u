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

///////////////////////////////////////////////////////////////
// ow_iff.cpp : Defines the entry point for the DLL application.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ow_iff.h"

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


// This is an example of an exported variable
OW_IFF_API int nOW_iff=0;

// This is an example of an exported function.
OW_IFF_API int fnOW_iff(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see ow_iff.h for the class definition
OW_IFF_API COW_iff::COW_iff()
{ 
	return; 
}


OW_IFF_API int ACCUAPI OW_IFF_IMGLOW_get_filetype(char far *filename)
{
#if defined _M_IX86
	return IMGLOW_get_filetype(filename);
#elif defined _M_X64
	return 0;
#endif
}

OW_IFF_API int ACCUAPI OW_IFF_IMGLOW_get_ascii_attributes(LPASCIITEXTATTR lpattributes)
{
#if defined _M_IX86
	return IMGLOW_get_ascii_attributes(lpattributes);
#elif defined _M_X64
	return 0;
#endif
}

OW_IFF_API int ACCUAPI OW_IFF_IMGLOW_set_ascii_attributes(LPASCIITEXTATTR lpattributes)
{
#if defined _M_IX86
	return IMGLOW_set_ascii_attributes(lpattributes);
#elif defined _M_X64
	return 0;
#endif
}

OW_IFF_API int ACCUAPI OW_IFF_IMGLOW_get_pages(char FAR *filename)
{
#if defined _M_IX86
	return IMGLOW_get_pages(filename);
#elif defined _M_X64
	return 0;
#endif
}


OW_IFF_API HBITMAP ACCUAPI OW_IFF_IMG_dib_to_ddb(	int	imghandle,
													int	width,
													int	height)
{
#if defined _M_IX86
	return IMG_dib_to_ddb(imghandle, width, height);
#elif defined _M_X64
	return NULL;
#endif
}

OW_IFF_API int ACCUAPI OW_IFF_IMG_display_ddb(	HBITMAP	hbitmap,
												HDC		hdc,
												int		xpos,
												int		ypos)
{
#if defined _M_IX86
	return IMG_display_ddb(	hbitmap, hdc, xpos, ypos);
#elif defined _M_X64
	return 0;
#endif
}

OW_IFF_API int ACCUAPI OW_IFF_IMG_decompress_bitmap(char FAR* filename)
{
#if defined _M_IX86
	return IMG_decompress_bitmap(filename);
#elif defined _M_X64
	return 0;
#endif
}

OW_IFF_API int ACCUAPI OW_IFF_IMG_get_croprect(	int	imghandle,
												int FAR *xpos,
												int FAR *ypos,
												int FAR *width,
												int FAR *height)
{
#if defined _M_IX86
	return IMG_get_croprect(imghandle, xpos, ypos, width, height);
#elif defined _M_X64
	return 0;
#endif
}

OW_IFF_API int ACCUAPI OW_IFF_IMG_set_croprect(	int	imghandle,
												int	xpos,
												int	ypos,
												int	width,
												int	height)
{
#if defined _M_IX86
	return IMG_set_croprect(imghandle, xpos, ypos, width, height);
#elif defined _M_X64
	return 0;
#endif
}

OW_IFF_API int ACCUAPI OW_IFF_IMG_import_ascii(char FAR *filename, 
											   int page)
{
#if defined _M_IX86
	return IMG_import_ascii(filename, page);
#elif defined _M_X64
	return 0;
#endif
}

OW_IFF_API HGLOBAL ACCUAPI OW_IFF_IMG_bitmap_info(	int imghandle,
													int FAR *width,
													int FAR *height,
													int FAR *bits_per_pixel)
{
#if defined _M_IX86
	return IMG_bitmap_info(imghandle, width, height, bits_per_pixel);
#elif defined _M_X64
	return NULL;
#endif
}

OW_IFF_API int ACCUAPI OW_IFF_IMGLOW_get_bitmap_header(	int imghandle, 
														LPBITMAPINFOHEADER pBIH)
{
#if defined _M_IX86
	return IMGLOW_get_bitmap_header(imghandle, pBIH);
#elif defined _M_X64
	return 0;
#endif
}

OW_IFF_API int ACCUAPI OW_IFF_IMG_create_handle(LPBITMAPINFOHEADER	lpbih)
{
#if defined _M_IX86
	return IMG_create_handle(lpbih);
#elif defined _M_X64
	return 0;
#endif
}


OW_IFF_API int ACCUAPI OW_IFF_IMG_delete_bitmap(int imghandle)
{
#if defined _M_IX86
	return IMG_delete_bitmap(imghandle);
#elif defined _M_X64
	return 0;
#endif
}

OW_IFF_API int ACCUAPI OW_IFF_IMG_promote_8(int imghandle)
{
#if defined _M_IX86
	return IMG_promote_8(imghandle);
#elif defined _M_X64
	return 0;
#endif
}

OW_IFF_API int ACCUAPI OW_IFF_IMG_promote_24(int imghandle)
{
#if defined _M_IX86
	return IMG_promote_24(imghandle);
#elif defined _M_X64
	return 0;
#endif
}

OW_IFF_API int ACCUAPI OW_IFF_IMG_save_bitmap(	int	imghandle,
												char FAR *filename,
												int	type)
{
#if defined _M_IX86
	return IMG_save_bitmap(	imghandle, filename, type);
#elif defined _M_X64
	return 0;
#endif
}
