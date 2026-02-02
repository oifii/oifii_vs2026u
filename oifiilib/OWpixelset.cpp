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
#include "stdafx.h"
#include "oifiilib.h" //spi 2014


#include "ow_iff.h"
/*
//accusoft image libraries
#include "imglib.h"
#include "img_err.h"
#include "accuipl.h"
#include "ipl_errs.h"
*/

//2020july16, spi, begin
#include "utility.h"
//2020july16, spi, end

#include "OWpixelset.h" //offworld image library

#include "c_pointset.h" //offworld vertor library

#include "oifii_app.h"
#include "logdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


COWPixelset::COWPixelset()
{
	DefaultInitialization();
}

void COWPixelset::DefaultInitialization()
{
	//OFFWORLD BII - BAND INTERLEAF IMAGE - IMAGE PROCESSING BUFFER
	m_pData = NULL;

	//OFFWORLD IPL - INTEL IPL - IMAGE PROCESSING BUFFER
	m_pIplImage = NULL;

	//OFFWORLD - WINDOWS BITMAP SUPPORT - IMAGE DISPLAY BUFFER
	m_pDIB = NULL;

	//OFFWORLD IFF - ACCUSOFT IFF LIBRARY - IMAGE DISPLAY BUFFER
	m_imghandle = -1;
	m_ipl_image = -1; //ACCUSOFT IPL LIBRARY - IMAGE PROCESSING BUFFER

	//default palettes
	InitializeDefaultPalettes();

	//2020july16, spi, begin
	m_hsvdatasize_inbyte = 0;
	//2020july16, spi, end

	m_datasize_inbyte = 0;

	m_pixelsize_inbyte = 0;
	m_linesize_inbyte = 0;
	m_channelsize_inbyte = 0;

	m_numberofpixel_perline = 0;
	m_numberofline_perchannel = 0;
	m_numberofchannel = 0;

	//m_pHistogram, statically allocated

	//by default, the croprect is the whole image surface (it could be userdefined to a sub rectangle)
	m_croprect.left = 0;
	m_croprect.top= 0;
	m_croprect.right = m_numberofpixel_perline;
	m_croprect.bottom = m_numberofline_perchannel;

	m_iTCDThreshold = 80; //level
	//2020july23, spi, begin
	//m_iTCDScale = 3; //pixel
	m_iTCDScale = 3; // = 27; //pixel, let's default to a bigger convolution window size since we nowadays work with much larger images
	//2020july23, spi, end

	for(int i=0; i<PIXELSET_CHANNEL_MAXNUMBER; i++) 
	{
		m_pIsGlobalHistoChannelDirty[i] = TRUE;
		for(int j=0; j<PIXELSET_HISTO_MAXLEVEL; j++) 
		{
			m_pGlobalHistogram[i][j] = 0;
		}
	}
	return;
}


COWPixelset::COWPixelset(COWPixelset* pPixelset, 
						 CRect* pRect/*=NULL*/, 
						 int nchannel /*=-1*/, 
						 int iflag/*=PIXELSET_USE_APP_TEMPINFO*/,
						 int ivalue /*=-1*/)
{
	DefaultInitialization();
	if(pPixelset==NULL && pRect==NULL && iflag==PIXELSET_USE_APP_TEMPINFO) 
	{
		//no buffer allocated
		return;
	}
	if(pPixelset==NULL && pRect==NULL && iflag==PIXELSET_IGNORE_APP_TEMPINFO) 
	{
		//no buffer allocated
		ASSERT(FALSE);
		return;
	}

	if(pPixelset==NULL)
	{
		////////////////////////////////////////////
		//AllocBII from pRect and nchannel parameter
		////////////////////////////////////////////
		if(nchannel==-1) nchannel = 1;
		m_numberofchannel = nchannel;
		m_numberofpixel_perline = pRect->Width();
		m_numberofline_perchannel  = pRect->Height();
		AllocBII(	nchannel*8, 
					m_numberofpixel_perline,
					m_numberofline_perchannel,
					ivalue);
		return;
	}
	else
	{
		///////////////////////////////////
		//AllocIFF from pPixelset parameter
		///////////////////////////////////

		//use non-null pRect to copy image sub rect
		m_numberofpixel_perline = pPixelset->m_numberofpixel_perline;
		m_numberofline_perchannel  = pPixelset->m_numberofline_perchannel;
		if(pRect && !pRect->IsRectEmpty())
		{
			//2012august15, poirier, begin
			//ensure rect lies fully within pixelset
			if(pRect->top<0)
			{
				pRect->top=0;
			}
			if(pRect->left<0)
			{
				pRect->left=0;
			}
			if(pRect->bottom>pPixelset->m_numberofline_perchannel)
			{
				pRect->bottom=pPixelset->m_numberofline_perchannel;
			}
			if(pRect->bottom>pPixelset->m_numberofpixel_perline)
			{
				pRect->bottom=pPixelset->m_numberofpixel_perline;
			}
			//ensure rect's width is a multiple of 4, otherwize reduce it to closest multiple of 4
			if(pRect->Width()%4!=0)
			{
				pRect->right = pRect->right - pRect->Width()%4;
			}	
			//2012august15, poirier, end
			m_numberofpixel_perline = pRect->Width();
			m_numberofline_perchannel  = pRect->Height();
		}
		//use non-negative nchannel
		if(nchannel==-1)
		{
			if(iflag==PIXELSET_USE_APP_TEMPINFO
				&& pPixelset->m_pData!=NULL 
				&& ((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_bRawData==TRUE)
			{
				m_numberofchannel = pPixelset->m_numberofchannel;
			}
			else
			{
				//2012july22, poirier, begin
				/*
				int width, height, bits_per_pixel;
				OW_IFF_IMG_bitmap_info(pPixelset->m_imghandle, &width, &height, &bits_per_pixel);
				m_numberofchannel = bits_per_pixel/8;
				*/
				m_numberofchannel = pPixelset->m_numberofchannel;
				//2012july22, poirier, begin
			}
		}
		else
		{
			m_numberofchannel = nchannel;
		}

		//AllocIFF(BITMAPINFOHEADER* pDIB, 
		m_pixelsize_inbyte = 1;
		m_linesize_inbyte = m_pixelsize_inbyte * m_numberofpixel_perline;
		m_channelsize_inbyte = m_linesize_inbyte * m_numberofline_perchannel;

		m_datasize_inbyte = m_channelsize_inbyte * m_numberofchannel;
		if(m_channelsize_inbyte)
		{
			//define monochrome header
			//BITMAPINFOHEADER bih;
			int extranumberofpixel_perline = 0;
			if(m_numberofpixel_perline%4!=0)
			{
				extranumberofpixel_perline = 4 - m_numberofpixel_perline%4;
			}	
			int imagesize = (m_numberofpixel_perline + extranumberofpixel_perline) * m_numberofline_perchannel * m_pixelsize_inbyte * m_numberofchannel;
			//int dibsize = dibsize = sizeof(BITMAPINFOHEADER) + imagesize;
			int dibsize = sizeof(BITMAPINFOHEADER) + imagesize;
			int palettesize = 256*4;
			if(m_numberofchannel==1)
			{
				//allocate a palette for monochrome bitmap
				dibsize = sizeof(BITMAPINFOHEADER) + imagesize + palettesize;
			}
			HANDLE hBIH = GlobalAlloc(GMEM_MOVEABLE, dibsize*sizeof(char));
			BITMAPINFOHEADER* pBIH = (BITMAPINFOHEADER*) GlobalLock(hBIH);
			pBIH->biSize = sizeof(BITMAPINFOHEADER);
			pBIH->biWidth = m_numberofpixel_perline;
			pBIH->biHeight = m_numberofline_perchannel;
			pBIH->biPlanes = 1; 
			pBIH->biBitCount = (WORD)(m_pixelsize_inbyte * 8 * m_numberofchannel); 
			pBIH->biCompression = BI_RGB;
			pBIH->biSizeImage = imagesize;
			pBIH->biXPelsPerMeter = 300;
			pBIH->biYPelsPerMeter = 300;
			pBIH->biClrUsed = 0;
			pBIH->biClrImportant = 0;

			//2020july03, spi, begin
			//char* pDIBData = (char*) ((int)pBIH + (int)pBIH->biSize);
			char* pDIBData = (char*)((char*)pBIH + pBIH->biSize);
			//2020july03, spi, end
			if(m_numberofchannel==1)
			{
				//copy palette
				for(int i=0; i<256; i++)
				{
					((COLORREF*)pDIBData)[i] = RGB(i,i,i); //gray palette
				}
				//2020july03, spi, begin
				//pDIBData = (char*) ((int)pDIBData + palettesize);
				pDIBData = (char*)((char*)pDIBData + palettesize);
				//2020july03, spi, end
			}
			if(pPixelset==NULL)
			{
				//no seed image data, clear buffer
				memset(pDIBData, 0, imagesize);

			}

			//2012august15, poirier, begin
			/* 
			m_imghandle = OW_IFF_IMG_create_handle(pBIH);
			if(pPixelset!=NULL)
			{
				if(iflag==PIXELSET_USE_APP_TEMPINFO
						&& pPixelset->m_pData!=NULL 
						&& ((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_bRawData==TRUE)
				{
					//m_imghandle <<== pPixelset->m_pData
					CRect DataRect(((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_rectRawData);
					UpdateIFFFromBII(PIXELSET_ALL, pPixelset->m_pData, &DataRect, pRect);
				}
				else if(iflag==PIXELSET_USE_APP_TEMPINFO 
						&& pPixelset->m_imghandle!=-1 
						&& ((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_bRawData==FALSE)
				{
					//m_imghandle <<=== pPixelset->m_imghandle
					//RefreshAccusoftImgHandleFromImgHandle(pPixelset->m_imghandle, pRect);
					UpdateIFFFrom(pPixelset->m_imghandle, pRect);
					
				}
				else
				{
					ASSERT(iflag==PIXELSET_IGNORE_APP_TEMPINFO);
					ASSERT(pPixelset->m_imghandle!=-1);
					//RefreshAccusoftImgHandleFromImgHandle(pPixelset->m_imghandle, pRect);
					UpdateIFFFrom(pPixelset->m_imghandle, pRect);
				}
			}
			*/
			/* */
			//AllocBII(pBIH);
			AllocDIB(pBIH);
			if(pPixelset!=NULL)
			{
				if(iflag==PIXELSET_USE_APP_TEMPINFO
						&& pPixelset->m_pData!=NULL 
						&& ((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_bRawData==TRUE)
				{
					CRect DataRect(((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_rectRawData);
					//UpdateBIIFromBII(PIXELSET_ALL, pPixelset->m_pData, &DataRect, pRect);
					UpdateDIBFromBII(PIXELSET_ALL, pPixelset->m_pData, &DataRect, pRect);
				}
				else if(iflag==PIXELSET_USE_APP_TEMPINFO 
						&& pPixelset->m_pDIB!=NULL 
						&& ((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_bRawData==FALSE)
				{
					//UpdateBIIFrom(pPixelset, pRect);	
					UpdateDIBFrom(pPixelset->m_pDIB, pRect);	
				}
				else
				{
					ASSERT(iflag==PIXELSET_IGNORE_APP_TEMPINFO);
					ASSERT(pPixelset->m_pDIB);
					//UpdateBIIFrom(pPixelset, pRect);
					UpdateDIBFrom(pPixelset->m_pDIB, pRect);
				}
			}
			/* */
			//2012august15, poirier, end
			GlobalFree(hBIH);
		}
	}

}

COWPixelset::~COWPixelset()
{
	//OFFWORLD - IFF - ACCUSOFT IMAGE BUFFER
	if(m_imghandle>-1) 
	{
		FreeIFF();
	}
	//OFFWORLD - DIB - WINDOWS BITMAP SUPPORT
	if(m_pDIB) 
	{
		FreeDIB();
	}

	//OFFWORLD - IPL - INTEL IMAGE BUFFER
	if(m_pIplImage) 
	{
		FreeIPL();
	}

	//OFFWORLD - BII - BAND INTERLEAF IMAGE BUFFER
	if(m_pData)
	{
		FreeBII();
	}
}

char* COWPixelset::GetBIIPointer()
{
	return m_pData;
}

IplImage* COWPixelset::GetIPLPointer()
{
	return m_pIplImage;
}

BITMAPINFOHEADER* COWPixelset::GetDIBPointer()
{
	return m_pDIB;
}

int COWPixelset::GetIFFHandle()
{
	return m_imghandle;
}

//set buffer
BOOL COWPixelset::SetBIIPointer(char* pData)
{
	if(pData==NULL || m_pData!=NULL)
	{
		//do not allow overwrite buffer
		//must deallocate buffer prior to setting new one
		ASSERT(FALSE);
		return FALSE;
	}
	m_pData = pData;
	return TRUE;
}

BOOL COWPixelset::SetIPLPointer(IplImage* pIplImage)
{
	if(pIplImage==NULL || m_pIplImage!=NULL)
	{
		//do not allow overwrite buffer
		//must deallocate buffer prior to setting new one
		ASSERT(FALSE);
		return FALSE;
	}
	m_pIplImage = pIplImage;
	return TRUE;
}

BOOL COWPixelset::SetDIBPointer(BITMAPINFOHEADER* pDIB)
{
	if(pDIB==NULL || m_pDIB!=NULL)
	{
		//do not allow overwrite buffer
		//must deallocate buffer prior to setting new one
		ASSERT(FALSE);
		return FALSE;
	}

	//pDIB should be allocated (pBIH + palette + data)
	if(!IsDIBValid(pDIB)) return FALSE;

	m_pDIB = pDIB;
	return TRUE;
}

BOOL COWPixelset::SetIFFHandle(int imghandle)
{
	if(imghandle==-1 || m_imghandle!=-1)
	{
		//do not allow overwrite buffer
		//must deallocate buffer prior to setting new one
		ASSERT(FALSE);
		return FALSE;
	}
	m_imghandle = imghandle;
	return TRUE;
}

BOOL COWPixelset::IsDIBValid(BITMAPINFOHEADER* pDIB)
{
	ASSERT(FALSE);
	return TRUE;
}

BOOL COWPixelset::IsBIIPresent()
{
	if(m_pData==NULL) return FALSE;
	return TRUE;
}

BOOL COWPixelset::IsIPLPresent()
{
	if(m_pIplImage==NULL) return FALSE;
	return TRUE;
}

BOOL COWPixelset::IsDIBPresent()
{
	if(m_pDIB==NULL) return FALSE;
	return TRUE;
}

BOOL COWPixelset::IsIFFPresent()
{
	ASSERT(m_ipl_image==-1);
	if(m_imghandle==-1) return FALSE;
	return TRUE;
}


BOOL COWPixelset::SetTCDThreshold(int iNewThres_inlevel)
{
	m_iTCDThreshold = iNewThres_inlevel;
	return TRUE;
}

int COWPixelset::GetTCDThreshold()
{
	return m_iTCDThreshold;
}

BOOL COWPixelset::SetTCDScale(int iNewScale_inpixel)
{
	m_iTCDScale = iNewScale_inpixel;
	return TRUE;
}

int COWPixelset::GetTCDScale()
{
	return m_iTCDScale;
}

BOOL COWPixelset::IsRGB()
{
	BOOL b = FALSE;
	if(IsRedPresent() && IsGreenPresent() && IsBluePresent())
	{
		b = TRUE;
	}
	return b;
}

BOOL COWPixelset::IsColor()
{
	if(OIII_USE_OWIFFACCUSOFT==1)
	{
		if(m_imghandle>-1)
		{
			int width, height, bits_per_pixel;
			HGLOBAL hGlobal = OW_IFF_IMG_bitmap_info(m_imghandle, &width, &height, &bits_per_pixel);
			if(bits_per_pixel==24) return TRUE;
		}
	}
	if(OIII_USE_FREEIMAGE==1)
	{
		if(IsDIBPresent())
		{
			if(m_pDIB->biBitCount==24 || m_pDIB->biBitCount==32) return TRUE;
		}
	}
	return FALSE;
}

BOOL COWPixelset::IsGray()
{
	if(OIII_USE_OWIFFACCUSOFT==1)
	{
		if(m_imghandle>-1)
		{
			int width, height, bits_per_pixel;
			HGLOBAL hGlobal = OW_IFF_IMG_bitmap_info(m_imghandle, &width, &height, &bits_per_pixel);
			if(bits_per_pixel==8) return TRUE;
		}
	}
	if(OIII_USE_FREEIMAGE==1)
	{
		if(IsDIBPresent())
		{
			if(m_pDIB->biBitCount==8 ) return TRUE;
		}
	}
	if(m_pData!=NULL) return TRUE;
	return FALSE;
}

BOOL COWPixelset::IsRedPresent()
{
	return IsColor();
}
BOOL COWPixelset::IsGreenPresent()
{
	return IsColor();
}
BOOL COWPixelset::IsBluePresent()
{
	return IsColor();
}
BOOL COWPixelset::IsHuePresent()
{
	return IsColor();
}
BOOL COWPixelset::IsSaturationPresent()
{
	return IsColor();
}
BOOL COWPixelset::IsIntensityPresent()
{
	if(OIII_USE_OWIFFACCUSOFT==1)
	{
		if(m_imghandle>-1)
		{
			int width, height, bits_per_pixel;
			HGLOBAL hGlobal = OW_IFF_IMG_bitmap_info(m_imghandle, &width, &height, &bits_per_pixel);
			if(bits_per_pixel>=8) return TRUE;
		}
	}
	if(OIII_USE_FREEIMAGE==1)
	{
		if(IsDIBPresent())
		{
			if(m_pDIB->biBitCount>=8 ) return TRUE;
		}
	}
	if(m_pData!=NULL) return TRUE;
	return FALSE;
}

int COWPixelset::GetPixelsetNumberOfChannel()
{
	int numberofchannel;
	int nc_bii = 0;
	int nc_ipl = 0;
	int nc_dib = 0;
	int nc_iff = 0;
	if(IsBIIPresent())
	{
		nc_bii = m_numberofchannel;
	}
	if(IsIPLPresent())
	{
		nc_ipl = m_pIplImage->nChannels;
	}
	if(IsDIBPresent())
	{
		nc_dib = m_pDIB->biBitCount/8;
		ASSERT(m_pDIB->biBitCount==8 || m_pDIB->biBitCount==24);
	}
	if(IsIFFPresent())
	{
		BITMAPINFOHEADER* pBIH = GetIFFHeaderPointer(m_imghandle);
		nc_iff = pBIH->biBitCount/8;
	}
	//check on which is the greatest
	//or apply condition so nc_bii, nc_ipl, nc_dib and nc_iff are all the same
	if(nc_bii) 
	{
		if(nc_ipl) ASSERT(nc_bii==nc_ipl);
		if(nc_dib) ASSERT(nc_bii==nc_dib);
		if(nc_iff) ASSERT(nc_bii==nc_iff);
	}
	if(nc_ipl) 
	{
		if(nc_bii) ASSERT(nc_ipl==nc_bii);
		if(nc_dib) ASSERT(nc_ipl==nc_dib);
		if(nc_iff) ASSERT(nc_ipl==nc_iff);
	}
	if(nc_dib) 
	{
		if(nc_bii) ASSERT(nc_dib==nc_bii);
		if(nc_ipl) ASSERT(nc_dib==nc_ipl);
		if(nc_iff) ASSERT(nc_dib==nc_iff);
	}
	if(nc_iff) 
	{
		if(nc_bii) ASSERT(nc_iff==nc_bii);
		if(nc_ipl) ASSERT(nc_iff==nc_ipl);
		if(nc_dib) ASSERT(nc_iff==nc_dib);
	}

	numberofchannel = nc_bii;
	return numberofchannel;
}


RECT COWPixelset::GetCropRect()
{
	RECT myRect;
	if(m_imghandle>-1)
	{
		int xpos, ypos, width, height;
		OW_IFF_IMG_get_croprect(m_imghandle, &xpos, &ypos, &width, &height);
		myRect.left = (int) xpos;
		myRect.top = (int)	ypos;
	    myRect.right = (int) (xpos + width);
		myRect.bottom = (int) (ypos + height);
	}
	else if(m_pData!=NULL)
	{
		myRect = m_croprect;				
	}
	else if(m_pDIB!=NULL)
	{
		myRect.left = (int) 0;
		myRect.top = (int)	0;
	    myRect.right = (int) m_pDIB->biWidth;
		myRect.bottom = (int) m_pDIB->biHeight;
	}
	else
	{
		//either m_imghandle or m_pData must be valid
		ASSERT(FALSE);
		myRect.left = 0;
		myRect.top = 0;
	    myRect.right = 0;
		myRect.bottom = 0;
	}
	return myRect;
}

BOOL COWPixelset::SetCropRect(RECT* pRECT)
{
	if(pRECT==NULL)
	{
		return FALSE;
	}

	if(m_imghandle>-1)
	{
		int xpos = pRECT->left;
		int ypos = pRECT->top;
		int width = pRECT->right;
		int height = pRECT->bottom;
		OW_IFF_IMG_set_croprect(m_imghandle, xpos, ypos, width, height);
	}

	m_croprect = *pRECT;				
	return TRUE;
}



//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//OFFWORLD BII - BAND INTERLEAF IMAGE LIBRARY 
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

int COWPixelset::GetWidth()
{
	return m_numberofpixel_perline;
}

int COWPixelset::GetHeight()
{
	return m_numberofline_perchannel;
}

int COWPixelset::GetMaxValue(int i_channel)
{
	int i_x, i_y, i_z, i_zmax;

	i_zmax = 0;
    for(i_y=0; i_y<m_numberofline_perchannel; i_y++)
    {
		for(i_x=0; i_x<m_numberofpixel_perline; i_x++)
		{
			i_z = GetPixelValue(i_x, i_y, i_channel);
			if(i_z>i_zmax) i_zmax = i_z;
		}
	}
	return i_zmax;			
}

int COWPixelset::GetMinValue(int i_channel)
{
	int i_x, i_y, i_z, i_zmin;

	i_zmin = 65536;
    for(i_y=0; i_y<m_numberofline_perchannel; i_y++)
    {
		for(i_x=0; i_x<m_numberofpixel_perline; i_x++)
		{
			i_z = GetPixelValue(i_x, i_y, i_channel);
			if(i_z<i_zmin) i_zmin = i_z;
		}
	}
	return i_zmin;			
}



int COWPixelset::RenderPixelsetDataAsTextBuffer(CString* pszTextBuffer, int i_channel, RECT* pROIRect)
{
	if(m_pData==NULL)
	{
		ASSERT(FALSE); //caller should have previously allocated m_pPixelset
		return FALSE;
	}
	if(pszTextBuffer==NULL)
	{
		ASSERT(FALSE); //caller should supply the allocated CString
		return FALSE;
	}

	*pszTextBuffer = "";
	char pszCharBuf[1024];
	int i_x, i_y, i_x_start, i_y_start, i_x_end, i_y_end, i_value;
	i_channel = 0;
	i_x_start = 0;
	i_x_end = GetWidth();
	i_y_start = 0;
	i_y_end = GetHeight();
	wsprintfA(pszCharBuf, "image width %ld, height %ld\r\n", i_x_end, i_y_end);
	*pszTextBuffer += pszCharBuf;
	if(pROIRect!=NULL)
	{
		if(pROIRect->left>i_x_start && pROIRect->left<i_x_end) i_x_start = pROIRect->left;	
		if(pROIRect->top>i_y_start && pROIRect->top<i_y_end) i_y_start = pROIRect->top;	
		if(pROIRect->right<i_x_end-1 && pROIRect->right>i_x_start) i_x_end = pROIRect->right + 1;	
		if(pROIRect->bottom<i_y_end && pROIRect->bottom>i_y_start) i_y_end = pROIRect->bottom + 1;	
	}
	wsprintfA(pszCharBuf, "roi x_start %ld, x_end %ld, y_start %ld, y_end %ld\r\n", i_x_start, i_x_end, i_y_start, i_y_end);
	*pszTextBuffer += pszCharBuf;
	for(i_y=i_y_start; i_y<i_y_end; i_y++)
	{
		for(i_x=i_x_start; i_x<i_x_end; i_x++)
		{
			i_value = GetPixelValue(i_x, i_y, i_channel);
			wsprintfA(pszCharBuf, "%5ld", i_value);
			*pszTextBuffer += pszCharBuf;
		}
		*pszTextBuffer += "\r\n";
	}

	return TRUE;
}

int COWPixelset::CopyPixelsetTo(COWPixelset* pOutputPixelset)
{
	if(pOutputPixelset==FALSE) return FALSE;
	
	if(m_datasize_inbyte>0)
	{
		//if output pixelset not allocated
		if(pOutputPixelset->m_datasize_inbyte = 0)
		{
			pOutputPixelset->m_datasize_inbyte = m_datasize_inbyte;
			pOutputPixelset->m_pData = (char*) malloc(m_datasize_inbyte * sizeof(char));
			if(pOutputPixelset->m_pData == NULL) return FALSE;
			memcpy(pOutputPixelset->m_pData, m_pData, m_datasize_inbyte);
		}
		else
		{
			ASSERT(FALSE); //output buffer is already allocated
			return FALSE;
		}

	}

	pOutputPixelset->m_pixelsize_inbyte = m_pixelsize_inbyte;
	pOutputPixelset->m_linesize_inbyte = m_pixelsize_inbyte;
	pOutputPixelset->m_channelsize_inbyte = m_pixelsize_inbyte;

	pOutputPixelset->m_numberofpixel_perline = m_pixelsize_inbyte;
	pOutputPixelset->m_numberofline_perchannel = m_pixelsize_inbyte;
	pOutputPixelset->m_numberofchannel = m_pixelsize_inbyte;

	return TRUE;
}



int COWPixelset::GetPixelValue(int i_x, int i_y,	int i_channel)
{
	int i_pixel_value = 0;

	if(m_imghandle>-1 && m_pData==NULL)
	{
		ASSERT(FALSE); //call AllocImgBufferFromAccusoftImgHandle() before any pixelset image data access/process functions
		return 0;
	}

	//When filtering, this function is used on inputImage
	//
	int i_offset = 0;

	//for band interleaf format
	i_offset = i_x*m_pixelsize_inbyte + i_y*m_linesize_inbyte + i_channel*m_channelsize_inbyte;

	/*
	//for pixel interleaf format
	i_offset = (i_x*m_pixelsize_inbyte*m_numberofchannel + i_channel*m_pixelsize_inbyte) +  
				i_y*m_linesize_inbyte ;
	*/

	/*
	if(p_header->IPACK==2)
	{
		//We want to get two bytes at a time
		i_pixel_value = (int) ((short __huge*) p_data[i_offset]);
	}
	else
	{
		//We want to get one byte at a time
		i_pixel_value = (int) ((unsigned char __huge*) p_data[i_offset]);
	}
	*/
	if(m_pixelsize_inbyte==2)
	{
		//We want to get two bytes at a time
		//2020july03, spi, begin
		//i_pixel_value = (* ((unsigned short*)((int)m_pData+i_offset)) );
		i_pixel_value = (*((unsigned short*)((unsigned short*)m_pData + i_offset)));
		//2020july03, spi, end
	}
	else // assume pixelsize_inbyte==1
	{
		//We want to get one byte at a time
		//i_pixel_value = (int) (*( (char*) ((int)m_pData+i_offset)) );
		i_pixel_value = (unsigned char) m_pData[i_offset];
	}

    return i_pixel_value;
}


int COWPixelset::SetPixelToValue(int i_x, int i_y, int i_channel, int i_val)
{
    //When filtering, this function is used on outputImage
    int i_offset = 0;
    i_offset = i_x*m_pixelsize_inbyte + i_y*m_linesize_inbyte + i_channel*m_channelsize_inbyte;
    /*
    if(p_header->IPACK==2)
    {
		//We want to set two bytes at a time
		p_data[i_offset] = (short) i_val;
    }
    else
    {
		//We want to set one byte at a time
		p_data[i_offset] = (unsigned char) i_val;
    }
    */
	if(m_pixelsize_inbyte==2)
	{
		//We want to set two bytes at a time
		//2020july03, spi, begin
		//(*( (short*) ((int)m_pData+i_offset)) ) = (short)i_val;
		(*((short*)((short*)m_pData + i_offset))) = (short)i_val;
		//2020july03, spi, end
	}
	else // assume pixelsize_inbyte==1
	{
		//We want to set one byte at a time
		//2020july03, spi, begin
		//(*( (char*) ((int)m_pData+i_offset)) ) = (char)i_val;
		(*((char*)((char*)m_pData + i_offset))) = (char)i_val;
		//2020july03, spi, end
	}

    return TRUE;
}

void COWPixelset::SetGlobalHistoChannelDirty(int i_channel, BOOL bIsDirty) //default i_channel=PIXELSET_ALL
{
	//update histo dirty flag
	if(i_channel==PIXELSET_ALL)
	{
		for(int i_c = 0; i_c<m_numberofchannel; i_c++)
		{
			m_pIsGlobalHistoChannelDirty[i_c] = bIsDirty; //TRUE initially
		}
	}
	else
	{
		m_pIsGlobalHistoChannelDirty[i_channel] = bIsDirty; //TRUE initially
	}
}

void COWPixelset::ClearGlobalHistoChannel(int i_channel) //default i_channel=PIXELSET_ALL
{
	if(i_channel==PIXELSET_ALL)
	{
		for(int i=0; i<PIXELSET_CHANNEL_MAXNUMBER; i++) 
		{
			m_pIsGlobalHistoChannelDirty[i] = TRUE;
			for(int j=0; j<PIXELSET_HISTO_MAXLEVEL; j++) 
			{
				m_pGlobalHistogram[i][j] = 0;
			}
		}
	}
	else
	{
		ASSERT(i_channel>-1);

		m_pIsGlobalHistoChannelDirty[i_channel] = TRUE;
		for(int j=0; j<PIXELSET_HISTO_MAXLEVEL; j++) 
		{
			m_pGlobalHistogram[i_channel][j] = 0;
		}
	}
	return;
}

void COWPixelset::ClearLocalHistoChannel(int i_channel) //default i_channel=PIXELSET_ALL
{
	if(i_channel==PIXELSET_ALL)
	{
		for(int i=0; i<PIXELSET_CHANNEL_MAXNUMBER; i++) 
		{
			for(int j=0; j<PIXELSET_HISTO_MAXLEVEL; j++) 
			{
				m_pLocalHistogram[i][j] = 0;
			}
		}
	}
	else
	{
		ASSERT(i_channel>-1);

		for(int j=0; j<PIXELSET_HISTO_MAXLEVEL; j++) 
		{
			m_pLocalHistogram[i_channel][j] = 0;
		}
	}
	return;
}

//compute histogram for specified channel into pUserData vector (provided list of 
//pixelset POINT coordinate)
int COWPixelset::GetHistogramChannel(int i_channel, int* pUserData , POINT* pPOINT, int npoints, int nlevels)
{
	ASSERT(m_pixelsize_inbyte==1);
	if(pUserData==NULL || pPOINT==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	int* pHistogram = pUserData;

	//clear histo
	for(int i=0; i<nlevels; i++)
	{
		pHistogram[i] = 0;
	}
	//compute histo
	if(nlevels==256)
	{
		for(int i=0; i<npoints; i++)
		{
			int i_z = GetPixelValue(pPOINT[i].x, pPOINT[i].y, i_channel);
			pHistogram[i_z] ++;
		}
	}
	else if(nlevels<256)
	{
		for(int i=0; i<npoints; i++)
		{
			int i_z = GetPixelValue(pPOINT[i].x, pPOINT[i].y, i_channel);
			int index = (int)((i_z /256.0) * nlevels);
			pHistogram[index] ++;
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	return TRUE;
}

int COWPixelset::GetHistogramChannel(int i_channel, int* pUserData , CRect* pRect)
{
	//case1: computes and stores histogram vectors when pRect==NULL (first time)
	//case2: returns the stored histogram vectors when pRect==NULL (following time, when m_pIsHistoChannelDirty[i_channel]==FALSE)
	//case3: recomputes and stores histogram vectors when pRect==NULL (following time, when m_pIsHistoChannelDirty[i_channel]==TRUE)
	//case4: always build histogram vectors when pRect!=NULL (never store local histogram vectors, faster to compute) 

	//validate
	int mynumberofchannel;
	if(m_imghandle>-1)
	{
		int nWidth, nHeight, nBitsPerPixel;
		OW_IFF_IMG_bitmap_info(m_imghandle, &nWidth, &nHeight, &nBitsPerPixel);
		mynumberofchannel = nBitsPerPixel/8;
	}
	else
	{
		mynumberofchannel = m_numberofchannel;
	}
    if( pUserData==NULL || i_channel<PIXELSET_ALL || i_channel>PIXELSET_CHANNEL_MAXNUMBER-1 || (i_channel>(mynumberofchannel-1)) ) 
    {		
    	return FALSE;
    }

	if(pRect==NULL)
	{
		//request for global histogram vectors
		//case1, case2 and case3
		//read or recompute histo
		int i_c;
		if(i_channel!=PIXELSET_ALL)
		{
			///////////////////
			//case all channels
			///////////////////

			//check if histo vector channels dirty
			if(m_pIsGlobalHistoChannelDirty[i_channel]==TRUE)
			{
				//global histo dirty, rebuild global histo
				BuildHistogramChannel(i_channel, NULL);
			}
			//copy global histo vector back to caller
			memcpy(pUserData, &(m_pGlobalHistogram[i_channel][0]), sizeof(int)*PIXELSET_HISTO_MAXLEVEL);
		}
		else
		{
			/////////////////////
			//case unique channel
			/////////////////////

			for(i_c = 0; i_c<m_numberofchannel; i_c++)
			{
				//check if global histo vector channels dirty
				if(m_pIsGlobalHistoChannelDirty[i_c]==TRUE)
				{
					//global histo dirty, rebuild global histo
					BuildHistogramChannel(i_c, NULL);
				}
			}
			//copy global histo vector back to caller
			memcpy(pUserData, &(m_pGlobalHistogram[0][0]), sizeof(int)*PIXELSET_HISTO_MAXLEVEL*m_numberofchannel);
		}
	}
	else
	{
		ASSERT(pRect!=NULL);
		//request for local histogram vectors
		//case4
		int i_c;
		if(i_channel!=PIXELSET_ALL)
		{
			//compute local histogram vector for one channel
			BuildHistogramChannel(i_channel, pRect);
			//copy local histo vector back to caller
			memcpy(pUserData, &(m_pLocalHistogram[i_channel][0]), sizeof(int)*PIXELSET_HISTO_MAXLEVEL);
		}
		else
		{
			for(i_c = 0; i_c<m_numberofchannel; i_c++)
			{
				//compute the local histogram vector for all channels
				BuildHistogramChannel(i_c, pRect);
			}
			//copy local histo vector back to caller
			memcpy(pUserData, &(m_pLocalHistogram[i_channel][0]), sizeof(int)*PIXELSET_HISTO_MAXLEVEL*m_numberofchannel);
		}
	}
	return TRUE; 
}

int COWPixelset::BuildHistogramChannel(int i_channel, CRect* pRect)
{
	//validate
	int mynumberofchannel;
	if(m_imghandle>-1)
	{
		int nWidth, nHeight, nBitsPerPixel;
		OW_IFF_IMG_bitmap_info(m_imghandle, &nWidth, &nHeight, &nBitsPerPixel);
		mynumberofchannel = nBitsPerPixel/8;
	}
	else
	{
		mynumberofchannel = m_numberofchannel;
	}
    if( i_channel<PIXELSET_ALL || i_channel>PIXELSET_CHANNEL_MAXNUMBER-1 || (i_channel>(mynumberofchannel-1)) )
    {
		return FALSE;
    }

	if(m_imghandle>-1 && m_pData==NULL)
	{
		AllocBII(m_imghandle);
		//AllocImgBufferFromAccusoftImgHandle2();
		//AllocImgBufferFromAccusoftImgHandle();
	}


	int i_y_start = 0;
	int i_y_end = m_numberofline_perchannel;
	int i_x_start = 0;
	int i_x_end = m_numberofpixel_perline;

	//poirier, sept 2001, begin
	if(pRect!=NULL)
	{
		//////////////////
		//case local histo
		//////////////////
		ClearLocalHistoChannel(i_channel);
		if(pRect->IsRectEmpty())
		{
			//local but selection outside pixelset rect

			//when user selection rect (in world coor) is outside pixelset rect (in world coor)
			//the pRect passed here should be set to empty, so there is no graph to compute
			return FALSE;
		}
		else
		{
			//local and selection inside pixelset rect

			//initialize with selection rectangle
			CRect selrect(pRect);
			/*
			CRect selrect(((COIIIApp*)AfxGetApp())->m_DocCreate_TempInfo_worldrect);
			*/
			//poirier, sept 2001, end
			if(selrect.top>=0 && selrect.top<m_numberofline_perchannel) 
			{
				i_y_start = selrect.top;
			}
			if(selrect.bottom<=m_numberofline_perchannel && selrect.bottom>0) 
			{
				i_y_end = selrect.bottom;
			}
			if(selrect.left>=0 && selrect.left<m_numberofpixel_perline) 
			{
				i_x_start = selrect.left;
			}
			if(selrect.right<=m_numberofpixel_perline && selrect.right>0) 
			{
				i_x_end = selrect.right;
			}
		}
	}
	//poirier, sept 2001, end

	int i_x, i_y, i_z;
	int i_c;
	for(i_y=i_y_start; i_y<i_y_end; i_y++) //poirier, sept 2001, modified, i_y_start was 0, i_y_end was m_numberofline_perchannel
	{
		for(i_x=i_x_start; i_x<i_x_end; i_x++) //poirier, sept 2001, modified, i_x_start was 0, i_x_end was m_numberofpixel_perline
		{
			if(i_channel!=PIXELSET_ALL)
			{
				i_z = GetPixelValue(i_x, i_y, i_channel);
				#ifdef _DEBUG
					if(i_z<0 || i_z>=PIXELSET_HISTO_MAXLEVEL) 
					{
						ASSERT(FALSE);
						return FALSE;
					}
				#endif //_DEBUG
				//poirier, sept 2001, begin
				if(pRect==NULL) 
				{
					m_pGlobalHistogram[i_channel][i_z] ++;
				}
				else
				{
					m_pLocalHistogram[i_channel][i_z] ++;
				}
				//poirier, sept 2001, end
			}
			else
			{
				for(i_c = 0; i_c<m_numberofchannel; i_c++)
				{
					i_z = GetPixelValue(i_x, i_y, i_channel);
					#ifdef _DEBUG
						if(i_z<0 || i_z>PIXELSET_HISTO_MAXLEVEL) 
						{
							ASSERT(FALSE);
							return FALSE;
						}
					#endif //_DEBUG
					//poirier, sept 2001, begin
					if(pRect==NULL) 
					{
						m_pGlobalHistogram[i_channel][i_z] ++;
					}
					else
					{
						m_pLocalHistogram[i_channel][i_z] ++;
					}
					//poirier, sept 2001, end
				}
			}
		}
	}

	if(pRect==NULL)
	{
		//update histo dirty flag to NOT dirty
		SetGlobalHistoChannelDirty(i_channel, FALSE);
	}
    return TRUE;
}



//int COWPixelset::ExtractLocalMax(CPointset* pPointset)
int COWPixelset::ExtractLocalMax(COWPointset* pOWPointset)
{
	//validate parameters
	if(pOWPointset==NULL) return FALSE;

    //Get a new filter, a Finite Spatial/Spectral Responce filter
    //(the filter is created with it's default parameters)
    LocalMaxParam* p_FilterParam = new(LocalMaxParam);
	if(p_FilterParam==NULL) return FALSE;

    //Set the parameters of the chosen filter
    p_FilterParam->selection_x_start=0;
    p_FilterParam->selection_y_start=0;
    p_FilterParam->selection_x_end=0; //default, will be set to maximum
    p_FilterParam->selection_y_end=0; //default, will be set to maximum
    p_FilterParam->selection_channel_start=0;
    p_FilterParam->selection_channel_end=0;
    p_FilterParam->window_spatial_width= m_iTCDScale; //was 3, TCDScale in pixel
    p_FilterParam->window_spectral_width=1;

    //Apply local maximum finder on the pixelset data
    if ( DoFSSMovingWindow(pOWPointset, p_FilterParam) != 0)
    {
		if(p_FilterParam != NULL) delete p_FilterParam;
		return FALSE;
    }

	if(p_FilterParam != NULL) delete p_FilterParam;
    return TRUE;
}



int COWPixelset::ValidateFilterParameters(LocalMaxParam* p_FilterParam)
{
    //Filter Parameter Validation
    if( p_FilterParam->window_spatial_width<1  ||
	p_FilterParam->window_spatial_width>51 ||
	p_FilterParam->window_spatial_width>m_numberofpixel_perline ||
	p_FilterParam->window_spatial_width>m_numberofline_perchannel )
    {
		//if spatial window width out of range, return ERROR
		return FALSE;
    }

    int  i_result;
    double f_result1, f_result2;
    f_result1 = ((double) p_FilterParam->window_spatial_width)/2.0f;
    i_result =	( (int) p_FilterParam->window_spatial_width)/2;
    f_result2 = (double) i_result;
    if( f_result1 == f_result2 )
    {
		//if spatial window width is even, set it odd
		p_FilterParam->window_spatial_width =+ 1;
    }
    if( p_FilterParam->selection_x_start<0 ||
		p_FilterParam->selection_x_start>=m_numberofpixel_perline )
    {
		//if x selection start not valid, set it to min value
		p_FilterParam->selection_x_start = 0;
    }
    if( p_FilterParam->selection_x_end<0 ||
		p_FilterParam->selection_x_end>=m_numberofpixel_perline ||
		p_FilterParam->selection_x_end<
	   (p_FilterParam->selection_x_start
	   +p_FilterParam->window_spatial_width-1) )
    {
		//if x selection end not specified, set it to max value
		p_FilterParam->selection_x_end = m_numberofpixel_perline-1;
    }
    if( p_FilterParam->selection_y_start<0 ||
		p_FilterParam->selection_y_start>=m_numberofline_perchannel )
    {
		//if y selection start not valid, set it to min value
		p_FilterParam->selection_y_start = 0;
    }
    if( p_FilterParam->selection_y_end<0 ||
		p_FilterParam->selection_y_end>=m_numberofline_perchannel ||
		p_FilterParam->selection_y_end<
	   (p_FilterParam->selection_y_start
	   +p_FilterParam->window_spatial_width-1) )
    {
		//if y selection end not specified, set it to max value
		p_FilterParam->selection_y_end = m_numberofline_perchannel-1;
    }

    if( p_FilterParam->window_spectral_width<1	||
		p_FilterParam->window_spectral_width>PIXELSET_KERNEL_MAXDIMENSION_Z ||
		p_FilterParam->window_spectral_width>m_numberofchannel )
    {
		//if spectral window width out of range, return ERROR
		return FALSE;
    }
    f_result1 = ((double) p_FilterParam->window_spectral_width)/2.0f;
    i_result =	( (int) p_FilterParam->window_spectral_width)/2;
    f_result2 = (double) i_result;
    if( f_result1 == f_result2 )
    {
		//if spectral window width is even, set it odd
		p_FilterParam->window_spectral_width =+ 1;
    }
    if( p_FilterParam->selection_channel_start<0 ||
		p_FilterParam->selection_channel_start>=m_numberofchannel )
    {
		//if channel selection start not valid, set it to min value
		p_FilterParam->selection_channel_start = 0;
    }
    if( p_FilterParam->selection_channel_end<0 ||
	p_FilterParam->selection_channel_end>=m_numberofchannel ||
	p_FilterParam->selection_channel_end<
	   (p_FilterParam->selection_channel_start
	   +p_FilterParam->window_spectral_width-1) )
    {
		//if channel selection end not specified, set it to max value
		p_FilterParam->selection_channel_end = m_numberofchannel-1;
    }
    return TRUE;
}




//int COWPixelset::DoFSSMovingWindow(CPointset* pPointset, LocalMaxParam* p_FilterParam)
int COWPixelset::DoFSSMovingWindow(COWPointset* pOWPointset, LocalMaxParam* p_FilterParam)
{
    //Finite Spatial-Spectral Moving Window
    //
    //In other words, this filter is displacing a finite size window
    //over a selected portion of the raster image.
    //
    //This function will expect parameters to be pre-defined in the
    //LocalMaxParam structure and will validate the laters right
    //at beginning.  If any error occurs during validation, function will
    //exit with proper error message.

    if( pOWPointset==NULL || p_FilterParam==NULL )
    {
		return FALSE;
    }
    if(ValidateFilterParameters(p_FilterParam)==FALSE)
    {
		return FALSE;
    }


    int i_spatial_whwidth  =((int) (p_FilterParam->window_spatial_width/2));
    int i_spectral_whwidth =((int) (p_FilterParam->window_spectral_width/2));
    int i_spatial_wwidthsquared =(int)(p_FilterParam->window_spatial_width
				     *p_FilterParam->window_spatial_width);

    int i_start_x = p_FilterParam->selection_x_start+i_spatial_whwidth;
    int i_start_y = p_FilterParam->selection_y_start+i_spatial_whwidth;
    int i_start_channel = p_FilterParam->selection_channel_start+i_spectral_whwidth;
    int i_end_x = p_FilterParam->selection_x_end-i_spatial_whwidth;
    int i_end_y = p_FilterParam->selection_y_end-i_spatial_whwidth;
    int i_end_channel = p_FilterParam->selection_channel_end-i_spectral_whwidth;

    int i_x, i_y, i_channel, i_z, prev_i_z;
    int i_wx,i_wy,i_wchannel;
    int i_spatial_center = 0;
    int i_spatial_max = 0;
    int i_spatial_new = 0;
    int i_new_value = 0;

    int i_intensity = 0;

    double f_spatial_sum = 0.0;
    double f_spectral_sum = 0.0;
    double f_spectral_nx_sum = 0.0;
    double f_spectral_ny_sum = 0.0;
    double f_average_spatial_sum = 0.0;
    double f_average_spectral_sum = 0.0;
    double f_average_spectral_nx_sum = 0.0;
    double f_average_spectral_ny_sum = 0.0;

    //
    //these values have been set by inspection
    //of histogram for the whole image (using
    //the debugger on COWPixelset->m_pHistogram).
    //
    int i_background_noise[PIXELSET_CHANNEL_MAXNUMBER];
    for(int ic=0; ic<PIXELSET_CHANNEL_MAXNUMBER; ic++)
    {
		i_background_noise[ic] = m_iTCDThreshold; //was 80, TCDThreshold in level value
		if(i_background_noise[ic]==-1) //auto background noise detection flag
		{
			i_background_noise[ic] = 80;
		}
    }
    int i_max_intensity = 255;
	//poirier dec 98, removed, was not used
    //int i_mean_intensity = 101;

    int i_localmaxcounter = 0;

    int i_nx = 0; //counter for number of pixel in x direction around the local maximum, include center pixel
    int i_ny = 0; //counter for number of pixel in y direction around the local maximum, includes center pixel
    int i_nz = 0; //counter for number of band where window center is a local maximum
	//poirier, march 27 1997
	int i_nxp = 0; //counter for number of pixel in positive x direction around the local maximum, exclude center pixel
	int i_nxn = 0;	//counter for number of pixel in negative x direction around the local maximum, exclude center pixel
	int i_nyp = 0; //counter for number of pixel in positive y direction around the local maximum, exclude center pixel
	int i_nyn = 0;	//counter for number of pixel in negative y direction around the local maximum, exclude center pixel
    /*
    //to visualise the object found
    CDC* p_thisDC = p_OutputFile->m_parent->GetDC();
	*/

	//2012july25, poirier, begin
	if(pOWPointset->GetPointerToStatistics()==NULL)
	{
		ASSERT(FALSE);	
		return FALSE;
	}
	if(pOWPointset->GetNumberOfStatPerPoint()!=POINTSET_EXTRA_NUMBEROFSTAT+4)
	{
		ASSERT(FALSE);	
		return FALSE;
	}
	/*
	if(pPointset->GetPointerToStatistics()!=NULL)
	{
		ASSERT(FALSE);	
		return FALSE;
	}
	else
	{
		ASSERT(pPointset->GetMaxNumberOfElements()>0);
		
		//allocate pfStastistics
		if(pPointset->NewPointsetStatistics(pPointset->GetMaxNumberOfElements(), 4) ==FALSE)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		//pPointset->nStatPerPoint = 4;
		//pPointset->nSizeStatInByte = pPointset->nStatPerPoint * pPointset->maxnumberofelements * sizeof(double);
		//pPointset->pfStatistics = (double*) malloc(pPointset->nSizeStatInByte );
		//if(pPointset->pfStatistics == NULL)
		//{
		//	ASSERT(FALSE);
		//	return FALSE;
		//}
	}
	*/
	//2012july25, poirier, end

	//for debugging
	BOOL bNotATrueLocalMax = FALSE;
	//FILE* pFile = NULL;
	//pFile = fopen("db_tree.txt", "w");

    /////////////////////////
    //For the whole selection
    /////////////////////////
    for(i_channel=i_start_channel; i_channel<=i_end_channel; i_channel++)
    {
		for(i_y=i_start_y; i_y<=i_end_y; i_y++)
		{
			for(i_x=i_start_x; i_x<=i_end_x; i_x++)
			{
				/*
				//if(m_parentHWND==NULL) //spi 2014
				//{
					BOOL bEsc = ((COIIIApp*)AfxGetApp())->PeekKeyState(VK_ESCAPE);
					if(bEsc) 
					{
						CString myString;
						myString.Format("\r\nDoFSSMovingWindow() aborted by user after collecting %d image features\r\n", i_localmaxcounter);
						((COIIIApp*)AfxGetApp())->GetLogDocument()->AddText(myString);
						return TRUE;
					}
				//}
				*/
				/////////////////////////////
				//For the whole kernel window
				/////////////////////////////
				i_nz = 0;
				f_spectral_sum = 0;
				f_spectral_nx_sum = 0;
				f_spectral_ny_sum = 0;
				for(i_wchannel=(i_channel-i_spectral_whwidth); i_wchannel<=(i_channel+i_spectral_whwidth); i_wchannel++)
				{
					f_spatial_sum = 0;
					i_spatial_max = 0;
					for(i_wy=i_y-i_spatial_whwidth; i_wy<=(i_y+i_spatial_whwidth); i_wy++)
					{
	    				for(i_wx=i_x-i_spatial_whwidth; i_wx<=(i_x+i_spatial_whwidth); i_wx++)
	      				{
							//
							//do not check on window center
							if( !(i_wy==i_y && i_wx==i_x) )
							{
			   					//
			   					//store maximum value found in the window
			   					i_spatial_new = GetPixelValue(i_wx, i_wy, i_wchannel);
			   					if(i_spatial_new>i_spatial_max)
			   					{
			       					i_spatial_max = i_spatial_new;
			   					}
							}
	      				}//end of i_wx loop
					}//end of i_wy loop
					i_spatial_center = GetPixelValue(i_x, i_y, i_wchannel);
					//poirier, march 28 1997, to avoid detecting the same tree crown many times
					bNotATrueLocalMax = FALSE;
					//this is the localmax condition, it also provides a unique reference for crown center
					//if(   (i_spatial_center > i_spatial_max)

					//poirier, march 28 1997, to detect more tree crowns
					//but since we not only let the kernel's local maximum to be a crown canditate, 
					//but all the kernel's higher pixel value to be crown candidates we need
					//a strategy to avoid detecting many times the same crown.  the fisrt suggestion
					//is to throw the crown candidate await when the under evaluation crown center 
					//(i_x, i_y) is not found to be at the very center of the estimated crown
					//surface (obtained searching in x+, x-, y+ and y- direction around the candidate)
					//2020august08, spi, begin
					//going back to forcing the center to be a kernel window maximum
					//this way, much less local max should be found
					//which is more suitable for very large image
					//if(  (i_spatial_center >= i_spatial_max)
					if ((i_spatial_center > i_spatial_max)
					//2020august08, spi, end
					   && (i_spatial_center > i_background_noise[i_wchannel]) )
					{
						//////////////////////////////////////////////////////
						//if local maximum found, compute different statistics
						//////////////////////////////////////////////////////
						//
						//increase the counter of the number of band involve with
						//this local maximum
						i_nz ++;
						//
						//scan the window in the x direction, from center to right
						//i_nx = 0;
						i_nx = 1;
						i_nxp = 0;
						i_z = GetPixelValue(i_x, i_y, i_wchannel); //center pixel
						f_spatial_sum = (double) i_z; //center pixel
						i_wx = i_x;	//center pixel
						i_wy = i_y; //center pixel
						do
						{
							prev_i_z = i_z;
							i_wx++;
							i_nx++;
							i_nxp++;
							i_z = GetPixelValue(i_wx, i_wy, i_wchannel);
							f_spatial_sum += (double) i_z;
						} while ((i_z >= (i_background_noise[i_wchannel] + i_spatial_center) / 2.0)
							&& i_z <= prev_i_z
							&& i_wx < p_FilterParam->selection_x_end //); //limited by image size
						//	  && (i_wx<(i_x+i_spatial_whwidth)) );	    //limited by window size
						//2020july13, spi, begin
							  && (i_wx < (i_x + i_spatial_whwidth)) );	//limited by window size
						//2020july13, spi, end
						if( i_z < (i_background_noise[i_wchannel]+i_spatial_center)/2.0 )
						{
							f_spatial_sum -= (double) i_z; //undo last addition
							//poirier, march 27 1997
							i_nx--;
							i_nxp--;
						}
						//
						//scan the window in the other x direction, from center to left
						//poirier, march 27 1997
						//i_nx --;
						i_nxn = 0;
						i_wx = i_x; //center pixel
						i_z = GetPixelValue(i_x, i_y, i_wchannel); //center pixel
						do
						{
							prev_i_z = i_z;
							i_wx--;
							i_nx++;
							//poirier, march 27 1997
							i_nxn++;
							i_z = GetPixelValue(i_wx, i_wy, i_wchannel);
							f_spatial_sum += (double) i_z;
						} while ( (i_z >= (i_background_noise[i_wchannel]+i_spatial_center)/2.0 )
							  && i_z <= prev_i_z
							  && i_wx>p_FilterParam->selection_x_start //); //limited by image size
						//	  && (i_wx>(i_x-i_spatial_whwidth)) );	      //limited by window size
						//2020july13, spi, begin
								&& (i_wx > (i_x - i_spatial_whwidth)) );	//limited by window size
						//2020july13, spi, end
						if( i_z < (i_background_noise[i_wchannel]+i_spatial_center)/2.0 )
						{
							f_spatial_sum -= (double) i_z; //undo last addition
							//poirier, march 27 1997
							i_nx--;
							i_nxn--;

						}
						//
						//scan the window in the y direction, from center to bottom
						//poirier, march 27 1997
						//i_ny = 0;
						i_ny = 1;
						i_nyp = 0;
						i_wx = i_x;
						i_wy = i_y;
						i_z = GetPixelValue(i_x, i_y, i_wchannel);
						do
						{
							prev_i_z = i_z;
							i_wy++;
							i_ny++;
							//poirier, march 27 1997
							i_nyp++;
							i_z = GetPixelValue(i_wx, i_wy, i_wchannel);
							f_spatial_sum += (double) i_z;
						} while ( (i_z >= (i_background_noise[i_wchannel]+i_spatial_center)/2.0 )
							  && i_z <= prev_i_z
							  && i_wy<p_FilterParam->selection_y_end //); //limited by image size
						//	  && (i_wy<(i_y+i_spatial_whwidth)) );	    //limited by window size
						//2020july13, spi, begin
								&& (i_wy < (i_y + i_spatial_whwidth)) );	//limited by window size
						//2020july13, spi, end
						if( i_z < (i_background_noise[i_wchannel]+i_spatial_center)/2.0 )
						{
							f_spatial_sum -= (double) i_z; //undo last addition
							//poirier, march 27 1997
							i_ny--;
							i_nyp--;
						}
						//
						//scan the window in the other y direction, from center to top
						i_wy = i_y;
						//poirier, march 27 1997
						//i_ny --;
						i_nyn = 0;
						i_z = GetPixelValue(i_x, i_y, i_wchannel);
						do
						{
							prev_i_z = i_z;
							i_wy--;
							i_ny++;
							//poirier, march 27 1997
							i_nyn++;
							i_z = GetPixelValue(i_wx, i_wy, i_wchannel);
							f_spatial_sum += (double) i_z;
						} while ( (i_z >= (i_background_noise[i_wchannel]+i_spatial_center)/2.0 )
							  && i_z <= prev_i_z
							  && i_wy>p_FilterParam->selection_x_start //); //limited by image size
						//	  && (i_wy>(i_y-i_spatial_whwidth)) );	      //limited by window size
						//2020july13, spi, begin
								&& (i_wy > (i_y - i_spatial_whwidth)) );	//limited by window size
						//2020july13, spi, end
						if( i_z < (i_background_noise[i_wchannel]+i_spatial_center)/2.0 )
						{
							f_spatial_sum -= (double) i_z; //undo last addition
							//poirier, march 27 1997
							i_ny--;
							i_nyn--;
						}
						//poirier, march 27 1997
						if(i_ny!=(i_nyp+i_nyn+1)
							|| i_nx!=(i_nxp+i_nxn+1))
						{
							ASSERT(FALSE);
						}
						//accept or reject this candidate, based on condition that
						//candidate center pixel that is not a true localmax should 
						//be lie exactly in the center of the estimated crown surface
						if(i_spatial_center == i_spatial_max)
						{
							bNotATrueLocalMax = TRUE;
							//check in x direction
							float f = (float)i_nx/2.0f;
							int iInt = (int) f;
							int iFrac = (int)(f-(float)iInt+0.5f);
							if(iFrac==0 ) //f is even 
							{
								if(abs(i_nxp-i_nxn)==1) //if offset is 1 (the smallest in this case)
								{
									//could be two consecutive possible candidates
									//take the first
									if(i_nxp>i_nxn)
									{
										//accept
									}
									else
									{
										//reject
										i_nz = 0; //will force a reject
									}
								}
								else
								{
									//reject
									i_nz = 0; //will force a reject
								}
							}
							else //f is odd
							{
								if((i_nxp-i_nxn)==0) //if no offset (the smallest in this case)
								{
									//one possible candidate, accept
								}
								else
								{
									//reject
									i_nz = 0; //will force a reject
								}
							}
							//now, check in y direction
 							f = (float)i_ny/2.0f;
							iInt = (int) f;
							iFrac = (int)(f-(float)iInt+0.5f);
							if(iFrac==0 ) //f is even 
							{
								if(abs(i_nyp-i_nyn)==1) //if offset is 1 (the smallest in this case)
								{
									//could be two consecutive possible candidates
									//take the first
									if(i_nyp>i_nyn)
									{
										//accept
									}
									else
									{
										//reject
										i_nz = 0; //will force a reject
									}
								}
								else
								{
									//reject
									i_nz = 0; //will force a reject
								}
							}
							else //f is odd
							{
								if((i_nyp-i_nyn)==0) //if no offset (the smallest in this case)
								{
									//accept
								}
								else
								{
									//reject
									i_nz = 0; //will force a reject
								}
							}

						}

						//
						//for each influencing spectrum, average spatial sum
						f_average_spatial_sum = (double) ( f_spatial_sum /((double)(i_nx+i_ny-1)) ); //-1 because we dont want to count the center point twice!
						f_spectral_sum += f_average_spatial_sum;
						f_spectral_nx_sum += (double) i_nx;
						f_spectral_ny_sum += (double) i_ny;
	 				}//end of if localmax found
				}//fin de i_wchannel loop (for kernel window)
	  

				//if n_z is different than 0, it means a local max was found
				if(i_nz != 0)
				{
					i_localmaxcounter++;
					if(i_localmaxcounter>pOWPointset->GetMaxNumberOfElements())
					{
						//reallocate pPointset and pfStatistics
						int i_newnumberofelement = pOWPointset->GetMaxNumberOfElements() + POINTSET_ADDITIONAL_NUMBEROFPOINTS;

						if(pOWPointset->ReallocPointset(i_newnumberofelement)==FALSE) 
						{
							ASSERT(FALSE);
							return FALSE;
						}

						if(pOWPointset->ReallocPointsetStatistics(i_newnumberofelement)==FALSE)
						{
							ASSERT(FALSE);
							return FALSE;
						}

						/*
						//pPointset->nStatPerPoint = 4;
						pPointset->nSizeStatInByte = pPointset->nStatPerPoint * pPointset->maxnumberofelements * sizeof(double);
						pPointset->pfStatistics = (double*) realloc(pPointset->pfStatistics, pPointset->nSizeStatInByte );
						if(pPointset->pfStatistics == NULL)
						{
							ASSERT(FALSE);
							return FALSE;
						}
						*/
					}

					//poirier, march 28 1997, if i_nz is different than one, the localmax would be
					//a localmax in more than one band, this i_nz does not mean much, each band's statistic
					//must be computed and compiled individually
					if(i_nz!=1) ASSERT(FALSE);
					f_average_spectral_sum = f_spectral_sum/i_nz;
					f_average_spectral_nx_sum = f_spectral_nx_sum/i_nz;
					f_average_spectral_ny_sum = f_spectral_ny_sum/i_nz;
					//poirier dec 98, added to replace old tree intensity measurement
					double dfLocalMaxIntensity = i_spatial_center;
					
					//
					//we can now store the statistics in pointset object

					pOWPointset->SetNumberOfPoints(i_localmaxcounter);
					int i_localmaxindex = i_localmaxcounter-1;
					pOWPointset->m_pPOINTSET->px[i_localmaxindex] = (double)i_x + 0.5; //+0.5 added to x and y local max center position   		
					pOWPointset->m_pPOINTSET->py[i_localmaxindex] = (m_numberofline_perchannel-(i_y+0.5));	//invert in y, so it can be properly displayed;     		
					double* pfStatistics = pOWPointset->GetPointerToStatistics(i_localmaxindex);
					/* //poirier dec 98, replace old tree intensity measurement
					pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_INTENSITY] = f_average_spectral_sum; //intensite moyenne de l'objet
					*/
					pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_INTENSITY] = dfLocalMaxIntensity; //intensite du max local de l'objet
					pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMX] = f_average_spectral_nx_sum; //dimension x moyenne de l'objet
					pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMY] = f_average_spectral_ny_sum; //dimension y moyenne de l'objet
					pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEAREA] = f_average_spectral_nx_sum * f_average_spectral_ny_sum; //nombre pixel contenu dans l'objet


					//update pointset min max info
					pOWPointset->m_pPOINTSET->xmin=(pOWPointset->m_pPOINTSET->xmin < pOWPointset->m_pPOINTSET->px[i_localmaxindex] ) ? pOWPointset->m_pPOINTSET->xmin : pOWPointset->m_pPOINTSET->px[i_localmaxindex];
					pOWPointset->m_pPOINTSET->ymin=(pOWPointset->m_pPOINTSET->ymin < pOWPointset->m_pPOINTSET->py[i_localmaxindex] ) ? pOWPointset->m_pPOINTSET->ymin : pOWPointset->m_pPOINTSET->py[i_localmaxindex];
					pOWPointset->m_pPOINTSET->xmax=(pOWPointset->m_pPOINTSET->xmax > pOWPointset->m_pPOINTSET->px[i_localmaxindex] ) ? pOWPointset->m_pPOINTSET->xmax : pOWPointset->m_pPOINTSET->px[i_localmaxindex];
					pOWPointset->m_pPOINTSET->ymax=(pOWPointset->m_pPOINTSET->ymax > pOWPointset->m_pPOINTSET->py[i_localmaxindex] ) ? pOWPointset->m_pPOINTSET->ymax : pOWPointset->m_pPOINTSET->py[i_localmaxindex];

					//for debuggging
					/*
					if(pFile)fprintf(pFile, "%d, id= %d\t x=%f\t y=%f\t dx=%f\t dy=%f\n",
											bNotATrueLocalMax,
											i_localmaxindex,
											pPointset->px[i_localmaxindex], 
											pPointset->py[i_localmaxindex],
											pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMX],
											pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMY]);
					*/
					/*
					//we can also, draw the object found in the window
					RECT* p_objectrect = new(RECT);
					p_objectrect->left   = i_x - ( (int) f_average_spectral_nx_sum/2.0);
					p_objectrect->top	  = i_y - ( (int) f_average_spectral_ny_sum/2.0);
					p_objectrect->right  = i_x + ( (int) f_average_spectral_nx_sum/2.0);
					p_objectrect->bottom = i_y + ( (int) f_average_spectral_ny_sum/2.0);
					i_intensity = f_average_spectral_sum;
					CBrush* p_objectbrush = new CBrush(RGB(i_intensity,
									i_intensity,
									i_intensity));
					p_thisDC->FillRect(p_objectrect, p_objectbrush);
					delete p_objectrect;
					delete p_objectbrush;
					*/
				}//end of if(i_nz!=0)
			}//end of for(i_x)
		}//end of for(i_y)
 	}//end of for(i_channel)
	//if(pFile) fclose(pFile);
	return TRUE;
}

/*
int COWPixelset::DoFSSMovingWindow(POINTSET* pPointset,
				 					LocalMaxParam* p_FilterParam)
{
    //Finite Spatial-Spectral Moving Window
    //
    //In other words, this filter is displacing a finite size window
    //over a selected portion of the raster image.
    //
    //This function will expect parameters to be pre-defined in the
    //LocalMaxParam structure and will validate the laters right
    //at beginning.  If any error occurs during validation, function will
    //exit with proper error message.

    if( pPointset==NULL || p_FilterParam==NULL )
    {
		return FALSE;
    }
    if(ValidateFilterParameters(p_FilterParam)==FALSE)
    {
		return FALSE;
    }


    int i_spatial_whwidth  =((int) (p_FilterParam->window_spatial_width/2));
    int i_spectral_whwidth =((int) (p_FilterParam->window_spectral_width/2));
    int i_spatial_wwidthsquared =(int)(p_FilterParam->window_spatial_width
				     *p_FilterParam->window_spatial_width);

    int i_start_x = p_FilterParam->selection_x_start+i_spatial_whwidth;
    int i_start_y = p_FilterParam->selection_y_start+i_spatial_whwidth;
    int i_start_channel = p_FilterParam->selection_channel_start+i_spectral_whwidth;
    int i_end_x = p_FilterParam->selection_x_end-i_spatial_whwidth;
    int i_end_y = p_FilterParam->selection_y_end-i_spatial_whwidth;
    int i_end_channel = p_FilterParam->selection_channel_end-i_spectral_whwidth;

    int i_x, i_y, i_channel, i_z, prev_i_z;
    int i_wx,i_wy,i_wchannel;
    int i_spatial_center = 0;
    int i_spatial_max = 0;
    int i_spatial_new = 0;
    int i_new_value = 0;

    int i_intensity = 0;

    double f_spatial_sum = 0.0;
    double f_spectral_sum = 0.0;
    double f_spectral_nx_sum = 0.0;
    double f_spectral_ny_sum = 0.0;
    double f_average_spatial_sum = 0.0;
    double f_average_spectral_sum = 0.0;
    double f_average_spectral_nx_sum = 0.0;
    double f_average_spectral_ny_sum = 0.0;

    //
    //these values have been set by inspection
    //of histogram for the whole image (using
    //the debugger on COWPixelset->m_pHistogram).
    //
    int i_background_noise[PIXELSET_CHANNEL_MAXNUMBER];
    for(int ic=0; ic<PIXELSET_CHANNEL_MAXNUMBER; ic++)
    {
		i_background_noise[ic] = m_iTCDThreshold; //was 80, TCDThreshold in level value
    }
    int i_max_intensity = 255;
    int i_mean_intensity = 101;

    int i_localmaxcounter = 0;

    int i_nx = 0; //counter for number of pixel in x direction around the local maximum
    int i_ny = 0; //counter for number of pixel in y direction around the local maximum
    int i_nz = 0; //counter for number of band where window center is a local maximum


    ////to visualise the object found
    //CDC* p_thisDC = p_OutputFile->m_parent->GetDC();

	if(pPointset->pfStatistics!=NULL)
	{
		ASSERT(FALSE);	
		return FALSE;
	}
	else
	{
		ASSERT(pPointset->maxnumberofelements>0);
		
		//allocate pfStastistics
		if(NewPointsetStatistics(pPointset, pPointset->maxnumberofelements, 4) ==FALSE)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		//pPointset->nStatPerPoint = 4;
		//pPointset->nSizeStatInByte = pPointset->nStatPerPoint * pPointset->maxnumberofelements * sizeof(double);
		//pPointset->pfStatistics = (double*) malloc(pPointset->nSizeStatInByte );
		//if(pPointset->pfStatistics == NULL)
		//{
		//	ASSERT(FALSE);
		//	return FALSE;
		//}
	}

	
    /////////////////////////
    //For the whole selection
    /////////////////////////
    for(i_channel=i_start_channel; i_channel<=i_end_channel; i_channel++)
    {
		for(i_y=i_start_y; i_y<=i_end_y; i_y++)
		{
			for(i_x=i_start_x; i_x<=i_end_x; i_x++)
			{
				/////////////////////////////
				//For the whole kernel window
				/////////////////////////////
				i_nz = 0;
				f_spectral_sum = 0;
				f_spectral_nx_sum = 0;
				f_spectral_ny_sum = 0;
				for(i_wchannel=(i_channel-i_spectral_whwidth); i_wchannel<=(i_channel+i_spectral_whwidth); i_wchannel++)
				{
					f_spatial_sum = 0;
					i_spatial_max = 0;
					for(i_wy=i_y-i_spatial_whwidth; i_wy<=(i_y+i_spatial_whwidth); i_wy++)
					{
	    				for(i_wx=i_x-i_spatial_whwidth; i_wx<=(i_x+i_spatial_whwidth); i_wx++)
	      				{
							//
							//do not check on window center
							if( !(i_wy==i_y && i_wx==i_x) )
							{
			   					//
			   					//store maximum value found in the window
			   					i_spatial_new = GetPixelValue(i_wx, i_wy, i_wchannel);
			   					if(i_spatial_new>i_spatial_max)
			   					{
			       					i_spatial_max = i_spatial_new;
			   					}
							}
	      				}//end of i_wx loop
					}//end of i_wy loop
					i_spatial_center = GetPixelValue(i_x, i_y, i_wchannel);
					//poirier, march 28 1997, to avoid detecting the same tree crown many times
					//this is the localmax condition, it also provides a unique reference for crown center
					if(   (i_spatial_center > i_spatial_max)

					//poirier, march 28 1997, to detect more tree crowns
					//but since we not only let the kernel's local maximum to be a crown canditate, 
					//but all the kernel's higher pixel value to be crown candidates we need
					//a strategy to avoid detecting many times the same crown.  the fisrt suggestion
					//is to throw the crown candidate await when the under evaluation crown center 
					//(i_x, i_y) is not found to be at the very center of the estimated crown
					//surface (obtained searching in x+, x-, y+ and y- direction around the candidate)
					//if(   (i_spatial_center >= i_spatial_max)
					   && (i_spatial_center > i_background_noise[i_wchannel]) )
					{
						//////////////////////////////////////////////////////
						//if local maximum found, compute different statistics
						//////////////////////////////////////////////////////
						//
						//increase the counter of the number of band involve with
						//this local maximum
						i_nz ++;
						//
						//scan the window in the x direction, from center to right
						i_nx = 0;
						i_z = GetPixelValue(i_x, i_y, i_wchannel);
						f_spatial_sum = (double) i_z;
						i_wx = i_x;
						i_wy = i_y;
						do
						{
							prev_i_z = i_z;
							i_wx++;
							i_nx++;
							i_z = GetPixelValue(i_wx, i_wy, i_wchannel);
							f_spatial_sum += (double) i_z;
						} while ( (i_z >= (i_background_noise[i_wchannel]+i_spatial_center)/2.0 )
							  && i_z <= prev_i_z
							  && i_wx<p_FilterParam->selection_x_end ); //limited by image size
						//	  && (i_wx<(i_x+i_spatial_whwidth)) );	    //limited by window size
						//
						if( i_z < (i_background_noise[i_wchannel]+i_spatial_center)/2.0 )
						{
							f_spatial_sum -= (double) i_z; //undo last addition
						}
						//
						//scan the window in the other x direction, from center to left
						i_nx --;
						i_wx = i_x;
						i_z = GetPixelValue(i_x, i_y, i_wchannel);
						do
						{
							prev_i_z = i_z;
							i_wx--;
							i_nx++;
							i_z = GetPixelValue(i_wx, i_wy, i_wchannel);
							f_spatial_sum += (double) i_z;
						} while ( (i_z >= (i_background_noise[i_wchannel]+i_spatial_center)/2.0 )
							  && i_z <= prev_i_z
							  && i_wx>p_FilterParam->selection_x_start ); //limited by image size
						//	  && (i_wx>(i_x-i_spatial_whwidth)) );	      //limited by window size
						//
						if( i_z < (i_background_noise[i_wchannel]+i_spatial_center)/2.0 )
						{
							f_spatial_sum -= (double) i_z; //undo last addition
						}
						//
						//scan the window in the y direction, from center to bottom
						i_ny = 0;
						i_wx = i_x;
						i_wy = i_y;
						i_z = GetPixelValue(i_x, i_y, i_wchannel);
						do
						{
							prev_i_z = i_z;
							i_wy++;
							i_ny++;
							i_z = GetPixelValue(i_wx, i_wy, i_wchannel);
							f_spatial_sum += (double) i_z;
						} while ( (i_z >= (i_background_noise[i_wchannel]+i_spatial_center)/2.0 )
							  && i_z <= prev_i_z
							  && i_wy<p_FilterParam->selection_y_end ); //limited by image size
						//	  && (i_wy<(i_y+i_spatial_whwidth)) );	    //limited by window size
						//
						if( i_z < (i_background_noise[i_wchannel]+i_spatial_center)/2.0 )
						{
							f_spatial_sum -= (double) i_z; //undo last addition
						}
						//
						//scan the window in the other y direction, from center to top
						i_wy = i_y;
						i_ny --;
						i_z = GetPixelValue(i_x, i_y, i_wchannel);
						do
						{
							prev_i_z = i_z;
							i_wy--;
							i_ny++;
							i_z = GetPixelValue(i_wx, i_wy, i_wchannel);
							f_spatial_sum += (double) i_z;
						} while ( (i_z >= (i_background_noise[i_wchannel]+i_spatial_center)/2.0 )
							  && i_z <= prev_i_z
							  && i_wy>p_FilterParam->selection_x_start ); //limited by image size
						//	  && (i_wy>(i_y-i_spatial_whwidth)) );	      //limited by window size
						//
						if( i_z < (i_background_noise[i_wchannel]+i_spatial_center)/2.0 )
						{
							f_spatial_sum -= (double) i_z; //undo last addition
						}
						//
						//for each influencing spectrum, average spatial sum
						f_average_spatial_sum = (double) ( f_spatial_sum /((double)(i_nx+i_ny-1)) ); //-1 because we dont want to count the center point twice!
						f_spectral_sum += f_average_spatial_sum;
						f_spectral_nx_sum += (double) i_nx;
						f_spectral_ny_sum += (double) i_ny;
	 				}//fin de localmax found
				}//fin de i_wchannel loop (for kernel window)
	  

				//if n_z is different than 0, it means a local max was found
				if(i_nz != 0)
				{
					i_localmaxcounter++;
					if(i_localmaxcounter>pPointset->maxnumberofelements)
					{
						//reallocate pPointset and pfStatistics
						int i_newnumberofelement = pPointset->maxnumberofelements + POINTSET_ADDITIONAL_NUMBEROFPOINTS;

						if(ReallocPointset(pPointset, i_newnumberofelement)==FALSE) 
						{
							ASSERT(FALSE);
							return FALSE;
						}

						if(ReallocPointsetStatistics(pPointset, i_newnumberofelement)==FALSE)
						{
							ASSERT(FALSE);
							return FALSE;
						}

						//pPointset->nStatPerPoint = 4;
						//pPointset->nSizeStatInByte = pPointset->nStatPerPoint * pPointset->maxnumberofelements * sizeof(double);
						//pPointset->pfStatistics = (double*) realloc(pPointset->pfStatistics, pPointset->nSizeStatInByte );
						//if(pPointset->pfStatistics == NULL)
						//{
						//	ASSERT(FALSE);
						//	return FALSE;
						//}
					}

					//poirier, march 28 1997, if i_nz is different than one, the localmax would be
					//a localmax in more than one band, this i_nz does not mean much, each band's statistic
					//must be computed and compiled individually
					if(i_nz!=1) ASSERT(FALSE);
					f_average_spectral_sum = f_spectral_sum/i_nz;
					f_average_spectral_nx_sum = f_spectral_nx_sum/i_nz;
					f_average_spectral_ny_sum = f_spectral_ny_sum/i_nz;

					//
					//we can now store the statistics in pointset object

					pPointset->npts = i_localmaxcounter;
					int i_localmaxindex = i_localmaxcounter-1;
					pPointset->px[i_localmaxindex] = (double)i_x;     		
					pPointset->py[i_localmaxindex] = (m_numberofline_perchannel-i_y);	//invert in y, so it can be properly displayed;     		
					double* pfStatistics = GetPointsetPointerToStatistics(pPointset, i_localmaxindex);
					pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_INTENSITY] = f_average_spectral_sum; //intensite moyenne de l'objet
					pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMX] = f_average_spectral_nx_sum; //dimension x moyenne de l'objet
					pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEDIMY] = f_average_spectral_ny_sum; //dimension y moyenne de l'objet
					pfStatistics[POINTSET_EXTRA_NUMBEROFSTAT+POINTSET_XYZI_TREEAREA] = f_average_spectral_nx_sum * f_average_spectral_ny_sum; //nombre pixel contenu dans l'objet


					//update pointset min max info
					pPointset->xmin=(pPointset->xmin < pPointset->px[i_localmaxindex] ) ? pPointset->xmin : pPointset->px[i_localmaxindex];
					pPointset->ymin=(pPointset->ymin < pPointset->py[i_localmaxindex] ) ? pPointset->ymin : pPointset->py[i_localmaxindex];
					pPointset->xmax=(pPointset->xmax > pPointset->px[i_localmaxindex] ) ? pPointset->xmax : pPointset->px[i_localmaxindex];
					pPointset->ymax=(pPointset->ymax > pPointset->py[i_localmaxindex] ) ? pPointset->ymax : pPointset->py[i_localmaxindex];


					
					//we can also, draw the object found in the window
					//RECT* p_objectrect = new(RECT);
					//p_objectrect->left   = i_x - ( (int) f_average_spectral_nx_sum/2.0);
					//p_objectrect->top	  = i_y - ( (int) f_average_spectral_ny_sum/2.0);
					//p_objectrect->right  = i_x + ( (int) f_average_spectral_nx_sum/2.0);
					//p_objectrect->bottom = i_y + ( (int) f_average_spectral_ny_sum/2.0);
					//i_intensity = f_average_spectral_sum;
					//CBrush* p_objectbrush = new CBrush(RGB(i_intensity,
					//				i_intensity,
					//				i_intensity));
					//p_thisDC->FillRect(p_objectrect, p_objectbrush);
					//delete p_objectrect;
					//delete p_objectbrush;
					
				}//end of if(i_nz!=0)
			}//end of for(i_x)
		}//end of for(i_y)
 	}//end of for(i_channel)

	return TRUE;
}
*/


int COWPixelset::ConvolutionFilter(COWPixelset* pOutputPixelset)
{
	if(pOutputPixelset==NULL) return FALSE;

    //Get a new filter, a Finite Spatial/Spectral Responce filter
    //(the filter is created with it's default parameters)
    FSSRFilterParam* p_FilterParam = new(FSSRFilterParam);


    //Set the parameters of the chosen filter
    p_FilterParam->selection_x_start=0;
    p_FilterParam->selection_y_start=0;
    p_FilterParam->selection_x_end=0; //default, will be set to maximum
    p_FilterParam->selection_y_end=0; //default, will be set to maximum
    p_FilterParam->selection_channel_start=0;
    p_FilterParam->selection_channel_end=0;
    p_FilterParam->window_spatial_width=3;
    p_FilterParam->window_spectral_width=1;

    p_FilterParam->i_coef[0][0][0]=1;
    p_FilterParam->i_coef[1][0][0]=1;
    p_FilterParam->i_coef[2][0][0]=1;
    p_FilterParam->i_coef[0][1][0]=1;
    p_FilterParam->i_coef[1][1][0]=1;
    p_FilterParam->i_coef[2][1][0]=1;
    p_FilterParam->i_coef[0][2][0]=1;
    p_FilterParam->i_coef[1][2][0]=1;
    p_FilterParam->i_coef[2][2][0]=1;

    p_FilterParam->i_coef[0][0][1]=1;
    p_FilterParam->i_coef[1][0][1]=1;
    p_FilterParam->i_coef[2][0][1]=1;
    p_FilterParam->i_coef[0][1][1]=1;
    p_FilterParam->i_coef[1][1][1]=1;
    p_FilterParam->i_coef[2][1][1]=1;
    p_FilterParam->i_coef[0][2][1]=1;
    p_FilterParam->i_coef[1][2][1]=1;
    p_FilterParam->i_coef[2][2][1]=1;

    p_FilterParam->i_coef[0][0][2]=1;
    p_FilterParam->i_coef[1][0][2]=1;
    p_FilterParam->i_coef[2][0][2]=1;
    p_FilterParam->i_coef[0][1][2]=1;
    p_FilterParam->i_coef[1][1][2]=1;
    p_FilterParam->i_coef[2][1][2]=1;
    p_FilterParam->i_coef[0][2][2]=1;
    p_FilterParam->i_coef[1][2][2]=1;
    p_FilterParam->i_coef[2][2][2]=1;

    //Create new Output Image COWPixelset
    //COWPixelset* pOutputPixelset= new COWPixelset();
    //CFileException* p_exception;
    //char pFilename[15];
    //strcpy(pFilename, "imgout.lan");
    //p_OutputImage->Open(pFilename, CFile::modeCreate |
    //				   CFile::modeWrite  |
    //				   CFile::typeBinary, p_exception);

    //Copy Image to
	CopyPixelsetTo(pOutputPixelset);

    //Perform filtering of your Image File
    if ( DoFSSRFiltering(pOutputPixelset, p_FilterParam) != 0)
    {
		if(p_FilterParam!=NULL) delete(p_FilterParam);
		//Close outputfile
		//p_OutputImage->Close();
		//Return a NULL pointer if error while filtering
		return FALSE;
    }

    if(p_FilterParam!=NULL) delete(p_FilterParam);
    return TRUE;
}


int COWPixelset::ValidateFilterParameters(FSSRFilterParam* p_FilterParam)
{
    //Filter Parameter Validation
    if( p_FilterParam->window_spatial_width<1  ||
	p_FilterParam->window_spatial_width>PIXELSET_KERNEL_MAXDIMENSION_X ||
	p_FilterParam->window_spatial_width>m_numberofpixel_perline ||
	p_FilterParam->window_spatial_width>m_numberofline_perchannel )
    {
	//if spatial window width out of range, return ERROR
	return PIXELSET_FILTER_SPATIAL_WINDOW_WIDTH_ERROR;
    }
    int  i_result;
    double f_result1, f_result2;
    f_result1 = ((double) p_FilterParam->window_spatial_width)/2.0f;
    i_result =	( (int) p_FilterParam->window_spatial_width)/2;
    f_result2 = (double) i_result;
    if( f_result1 == f_result2 )
    {
	//if spatial window width is even, set it odd
	p_FilterParam->window_spatial_width=+1;
    }
    if( p_FilterParam->selection_x_start<0 ||
	p_FilterParam->selection_x_start>=m_numberofpixel_perline )
    {
	//if x selection start not valid, set it to min value
	p_FilterParam->selection_x_start=0;
    }
    if( p_FilterParam->selection_x_end<0 ||
	p_FilterParam->selection_x_end>=m_numberofpixel_perline ||
	p_FilterParam->selection_x_end<
	   (p_FilterParam->selection_x_start
	   +p_FilterParam->window_spatial_width-1) )
    {
	//if x selection end not specified, set it to max value
	p_FilterParam->selection_x_end=m_numberofpixel_perline-1;
    }
    if( p_FilterParam->selection_y_start<0 ||
	p_FilterParam->selection_y_start>=m_numberofline_perchannel )
    {
	//if y selection start not valid, set it to min value
	p_FilterParam->selection_y_start=0;
    }
    if( p_FilterParam->selection_y_end<0 ||
	p_FilterParam->selection_y_end>=m_numberofline_perchannel ||
	p_FilterParam->selection_y_end<
	   (p_FilterParam->selection_y_start
	   +p_FilterParam->window_spatial_width-1) )
    {
	//if y selection end not specified, set it to max value
	p_FilterParam->selection_y_end=m_numberofline_perchannel-1;
    }

    if( p_FilterParam->window_spectral_width<1	||
	p_FilterParam->window_spectral_width>5 ||
	p_FilterParam->window_spectral_width>m_numberofchannel )
    {
	//if spectral window width out of range, return ERROR
	return PIXELSET_FILTER_SPECTRAL_WINDOW_WIDTH_ERROR;
    }
    f_result1 = ((double) p_FilterParam->window_spectral_width)/2.0f;
    i_result =	( (int) p_FilterParam->window_spectral_width)/2;
    f_result2 = (double) i_result;
    if( f_result1 == f_result2 )
    {
	//if spectral window width is even, set it odd
	p_FilterParam->window_spectral_width=+1;
    }
    if( p_FilterParam->selection_channel_start<0 ||
	p_FilterParam->selection_channel_start>=m_numberofchannel )
    {
	//if channel selection start not valid, set it to min value
	p_FilterParam->selection_channel_start=0;
    }
    if( p_FilterParam->selection_channel_end<0 ||
	p_FilterParam->selection_channel_end>=m_numberofchannel ||
	p_FilterParam->selection_channel_end<
	   (p_FilterParam->selection_channel_start
	   +p_FilterParam->window_spectral_width-1) )
    {
		//if channel selection end not specified, set it to max value
		p_FilterParam->selection_channel_end=m_numberofchannel-1;
    }

    //Now check finite responce matrix coefficients
    //It only checks the first coef[0][0][0] in case no coef would have
    //been set, the coef can be any float number.
    if(( p_FilterParam->i_coef[0][0][0]<-10000 ||
	     p_FilterParam->i_coef[0][0][0]>10000)
     &&( p_FilterParam->i_coef[p_FilterParam->window_spatial_width]
				  [p_FilterParam->window_spatial_width]
				  [p_FilterParam->window_spectral_width]<-10000 ||
	     p_FilterParam->i_coef[p_FilterParam->window_spatial_width]
				  [p_FilterParam->window_spatial_width]
				  [p_FilterParam->window_spectral_width]>10000) )
    {
	    return PIXELSET_FILTER_POSSIBLE_COEF_ERROR;
    }
    return TRUE;
}


int COWPixelset::DoFSSRFiltering(COWPixelset* p_outputImage,
			       FSSRFilterParam* p_FilterParam)
{
    //Finite Spatial-Spectral Responce Filtering
    //
    //In other words, this filter convolves a finite-size rectangular
    //window over a selected portion of the raster image.
    //
    //This function will expect parameters to be pre-defined in the
    //CFSSRFilter object and will validate the laters right
    //at beginning.  If any error occurs during validation, function will
    //exit with proper error message.

    if( p_outputImage==NULL || p_FilterParam==NULL )
    {
	return PIXELSET_FILTER_NULL_POINTER_ERROR;
    }
    if(ValidateFilterParameters(p_FilterParam))
    {
	return PIXELSET_FILTER_PARAM_VALIDATION_ERROR;
    }


    int i_spatial_whwidth  =((int) (p_FilterParam->window_spatial_width/2));
    int i_spectral_whwidth =((int) (p_FilterParam->window_spectral_width/2));
    int i_spatial_wwidthsquared =(int)(p_FilterParam->window_spatial_width
				     *p_FilterParam->window_spatial_width);

    int i_start_x = p_FilterParam->selection_x_start+i_spatial_whwidth;
    int i_start_y = p_FilterParam->selection_y_start+i_spatial_whwidth;
    int i_start_channel = p_FilterParam->selection_channel_start+i_spectral_whwidth;
    int i_end_x = p_FilterParam->selection_x_end-i_spatial_whwidth;
    int i_end_y = p_FilterParam->selection_y_end-i_spatial_whwidth;
    int i_end_channel = p_FilterParam->selection_channel_end-i_spectral_whwidth;

    int i_x, i_y, i_channel;
    int i_wx,i_wy,i_wchannel;
    int i_spatial_sum = 0;
    int i_normalized_spatial_value = 0;
    int i_normalized_spatial_sum = 0;
    int i_new_value = 0;
    /////////////////////////
    //For the whole selection
    /////////////////////////
    for(i_channel=i_start_channel; i_channel<=i_end_channel; i_channel++)
    {
      for(i_y=i_start_y; i_y<=i_end_y; i_y++)
      {
	for(i_x=i_start_x; i_x<=i_end_x; i_x++)
	{
	  //////////////////////
	  //For the whole window
	  //////////////////////
	  i_normalized_spatial_sum = 0;
	  for(i_wchannel=(i_channel-i_spectral_whwidth);
	      i_wchannel<=(i_channel+i_spectral_whwidth); i_wchannel++)
	  {
	    i_spatial_sum = 0;
	    i_normalized_spatial_value = 0;
	    for(i_wy=i_y-i_spatial_whwidth;
		i_wy<=(i_y+i_spatial_whwidth); i_wy++)
	    {
	      for(i_wx=i_x-i_spatial_whwidth;
		  i_wx<=(i_x+i_spatial_whwidth); i_wx++)
	      {
		//sum spatial weight times pixel value
		i_spatial_sum =+ GetPixelValue(i_wx, i_wy, i_wchannel)*
		   p_FilterParam->i_coef[i_wx-i_x+i_spatial_whwidth]
					[i_wy-i_y+i_spatial_whwidth]
					[i_wchannel-i_channel+i_spectral_whwidth];
	      }
	    }
	    //for each influencing spectrum, normalize spatial sum
	    i_normalized_spatial_value=(int)(i_spatial_sum/
					    i_spatial_wwidthsquared);
	    //sum normalized spatial value of each influencing spectrum
	    i_normalized_spatial_sum =+ i_normalized_spatial_value;
	  }
	  //i_new_value = (int) (i_normalized_spatial_sum/
	  //			p_FilterParam->window_spectral_width);
	  i_new_value = 128;
	  p_outputImage->SetPixelToValue(i_x, i_y, i_channel, i_new_value);
	}
      }
    }
    return TRUE;
}



int COWPixelset::Do2DimensionFFT(COWPixelset* p_outputImage,
			       				  TwoDFFTParam* p_FilterParam)
{
    return TRUE;
}


//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//OFFWORLD BII - BAND INTERLEAF IMAGE LIBRARY 
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////


int COWPixelset::AllocBII(int depth, int width, int height, int ivalue) //ivalue default to -1 for not initialized buffer
{
	if(m_pData!=NULL)
	{
		//when there is already an existing offworld image buffer allocated
		ASSERT(FALSE); 
		return FALSE;
	}

	if(m_pDIB!=NULL) 
	{
		//when there is already an existing windows dib image buffer allocated
		ASSERT(FALSE);
		//return FALSE;
	}
	if(m_imghandle!=-1) 
	{
		//when there is already an existing accusoft iff image buffer allocated
		ASSERT(FALSE);
		//return FALSE;
	}
	if(m_pIplImage!=NULL)
	{
		//when there is already an existing intel ipl image buffer allocated
		ASSERT(FALSE); 
		//return FALSE;
	}

	if(depth!=8 && depth!=24)
	{
		//offworld image buffer creation supports only 8bit and 24bit per pixel
		ASSERT(FALSE);
		return FALSE;
	}
	int bits_per_pixel = depth;
	
	//HANDLE hDIB = OW_IFF_IMG_bitmap_info(m_imghandle, &width, &height, &bits_per_pixel);
	//BITMAPINFOHEADER* pBIH = NULL;
	m_numberofpixel_perline = width;
	m_numberofline_perchannel = height;

	if(bits_per_pixel==8)
	{
		m_numberofchannel = 1;
		m_pixelsize_inbyte = 1;
	}
	else if(bits_per_pixel==24)
	{
		m_numberofchannel = 3;
		m_pixelsize_inbyte = 1;
	}
	else
	{
		//offworld image buffer creation supports only 8bit and 24bit per pixel
		ASSERT(FALSE);
		return FALSE;
	}
	m_linesize_inbyte = m_numberofpixel_perline * m_pixelsize_inbyte;
	m_channelsize_inbyte = m_numberofline_perchannel * m_linesize_inbyte;
	m_datasize_inbyte =  m_numberofchannel * m_channelsize_inbyte;
	m_pData = (char*) malloc(m_datasize_inbyte);

	if(m_pData==NULL) 
	{
		//out of memory
		ASSERT(FALSE);
		return FALSE;
	}

	if(ivalue>=0 && ivalue<256)
	{
		//initialize image raster data with ivalue
		/*
		for(int i=0; i<m_datasize_inbyte; i++)
		{
			memset(&(m_pData[i]),ivalue, m_datasize_inbyte);
		}
		*/
		memset(&(m_pData[0]),ivalue, m_datasize_inbyte);
	}
	else
	{
		//-1 is ok for no buffer initialization
		//ASSERT(ivalue==-1);
	}

	//update data member
	m_croprect.left = 0;
	m_croprect.top= 0;
	m_croprect.right = m_numberofpixel_perline;
	m_croprect.bottom = m_numberofline_perchannel;

	return TRUE;
}

int COWPixelset::AllocBII(BITMAPINFOHEADER* pDIB, 
						  CRect* pSelectionRect, 
						  int ivalue)
{
	if(pDIB==NULL && m_pDIB==NULL) 
	{
		//no dib supplied, no dib in current pixelset
		ASSERT(FALSE);
		return FALSE;
	}
	if(pDIB==NULL) pDIB = m_pDIB;

	if(pSelectionRect!=NULL)
	{
		//not supported for now
		ASSERT(FALSE);
	}

	
	if(m_imghandle!=-1) 
	{
		//when there is already an existing accusoft iff image buffer allocated
		//ASSERT(FALSE);
		//allow to be called for test
		//return FALSE;
	}
	
	if(m_pIplImage!=NULL)
	{
		//when there is already an existing intel ipl image buffer allocated
		//ASSERT(FALSE); 
		//return FALSE;
	}

	if(m_pData!=NULL)
	{
		//when there is already an existing offworld image buffer allocated
		ASSERT(FALSE); 
		//buffer must be deallocated prior to call this function
		return FALSE;
	}

	BITMAPINFOHEADER* pBIH = pDIB;

	/* //can't assume dib palette or data is allocated
	int nColorUsedInHeaderPalette = 0;
	if(pBIH->biBitCount==0) 
	{
		nColorUsedInHeaderPalette = 0;
	}
	else if(pBIH->biBitCount==1) 
	{
		nColorUsedInHeaderPalette = 2;
	}
	else if(pBIH->biBitCount==4) 
	{
		nColorUsedInHeaderPalette = 16;
	}
	else if(pBIH->biBitCount==8) 
	{
		nColorUsedInHeaderPalette = 256;
	}
	RGBQUAD* pColorTable = (RGBQUAD*)((int)pBIH + (WORD)(pBIH->biSize));
	char* pDIBData = (char*) ((int)pBIH + (int)pBIH->biSize + nColorUsedInHeaderPalette*4);

	//assume 24 or 32 bit, no palette required
	BOOL bIsGrayScale = FALSE;
	BOOL bNeedLookUpTable = FALSE; //lookup table is the color palette 
	*/

	/*
	//set depth of 1 or 3 channel depending upon pDIB compression
	int nBitPerPixel = pBIH->biBitCount;
	if(nBitPerPixel!=24 && nBitPerPixel!=32)
	{
		bNeedLookUpTable = TRUE;

		//palette image, get color from pBIH color table
		if(nBitPerPixel==1)
		{
			//monochrome, get color from pBIH color table
		}
		else if(nBitPerPixel==8)
		{
			//assume gray scale with no palette required, but check color table entries
			bIsGrayScale = TRUE;
			bNeedLookUpTable = FALSE;
			//check if gray scale or 256 arbitrary colors
			for(int i=0; i<256; i++)
			{
				if(pColorTable[i].rgbBlue!=pColorTable[i].rgbGreen)
				{
					bIsGrayScale = FALSE;
					break;
				}
				else if(pColorTable[i].rgbBlue!=pColorTable[i].rgbRed)
				{
					bIsGrayScale = FALSE;
				}
				else if(pColorTable[i].rgbBlue!=i)
				{
					//it is a gray scale, but will need to refer to color table
					bNeedLookUpTable = TRUE;
				}
			}
		}
	}
	*/

	//set widht and height
	m_numberofpixel_perline = pBIH->biWidth;
	m_numberofline_perchannel = pBIH->biHeight;

	//set depth of 1 or 3 channel depending upon pDIB compression
	int nBitPerPixel = pBIH->biBitCount;
	if( nBitPerPixel==1 || nBitPerPixel==8 )
	{
		//one band gray buffer
		m_numberofchannel = 1;
		m_pixelsize_inbyte = 1;
		m_linesize_inbyte = m_numberofpixel_perline * m_pixelsize_inbyte;
		m_channelsize_inbyte = m_numberofline_perchannel * m_linesize_inbyte;
		m_datasize_inbyte =  m_numberofchannel * m_channelsize_inbyte;
		if(nBitPerPixel==1)
		{
			//1 bit per pixel, will have difficulty reading input pDIBData 
			//(try it, read one input byte, that is 8 pixels, 
			//filter the 8bits of that byte using successive logical & mask 
			//like 00000001, 00000010, 000000100, 00001000, 
			// 00010000, 00100000, 01000000, 10000000
			ASSERT(FALSE);
			return FALSE;
		}
	}
	else if(nBitPerPixel==2 || nBitPerPixel==4 || nBitPerPixel==16
				|| nBitPerPixel==24 || nBitPerPixel==32)
	{
		//3 bands color buffer
		m_numberofchannel = 3;
		m_pixelsize_inbyte = 1;
		m_linesize_inbyte = m_numberofpixel_perline * m_pixelsize_inbyte;
		m_channelsize_inbyte = m_numberofline_perchannel * m_linesize_inbyte;
		m_datasize_inbyte =  m_numberofchannel * m_channelsize_inbyte;

	}
	else
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//alloc image buffer
	m_pData = (char*) malloc(m_datasize_inbyte);
	if(m_pData==NULL) 
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//copy image buffer
	/* to copy, use UpdateBIIFrom()
	*/

	//update data member
	m_croprect.left = 0;
	m_croprect.top= 0;
	m_croprect.right = m_numberofpixel_perline;
	m_croprect.bottom = m_numberofline_perchannel;
	return TRUE;
}

//2020july16, spi, begin
BOOL COWPixelset::IsHSV()
{
	if (m_hsvdatasize_inbyte != 0 && m_hsvdatasize_inbyte == m_datasize_inbyte)
	{
		return TRUE;
	}
	return FALSE;
}

int COWPixelset::ReallocBIIforHSVandUpdateBIIComputingHSV()
{
	if (m_numberofchannel != 3)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//alloc image buffer
	m_hsvdatasize_inbyte = m_datasize_inbyte;
	m_pData = (char*)realloc(m_pData, m_hsvdatasize_inbyte+m_datasize_inbyte);
	if (m_pData == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//compute newly allocated HSV channels from actual RGB channels
	int idchannel_red = 0;
	int idchannel_green = 1;
	int idchannel_blue = 2;
	int idchannel_h = 3;
	int idchannel_s = 4;
	int idchannel_v = 5;
	RgbColor myRgbColor;
	HsvColor myHsvColor;
	for (int i = 0; i < m_numberofline_perchannel; i++)
	{
		int i_offset = i * m_linesize_inbyte; //linestart for channel 0
		for (int j = 0; j < m_numberofpixel_perline; j++)
		{
			//color, read the 3 RGB channels
			//red channel
			myRgbColor.r = m_pData[i_offset + j];
			//green channel
			myRgbColor.g = m_pData[i_offset + idchannel_green * m_channelsize_inbyte + j];
			//blue channel
			myRgbColor.b = m_pData[i_offset + idchannel_blue * m_channelsize_inbyte + j];
			//color, convert color space
			myHsvColor = RgbToHsv(myRgbColor);
			//color, write to the 3 HSV channels
			//h channel
			m_pData[i_offset + idchannel_h * m_channelsize_inbyte + j] = myHsvColor.h;
			//s channel
			m_pData[i_offset + idchannel_s * m_channelsize_inbyte + j] = myHsvColor.s;
			//v channel
			m_pData[i_offset + idchannel_v * m_channelsize_inbyte + j] = myHsvColor.v;
		}
	}
	return TRUE;
}
//2020july16, spi, end

int COWPixelset::AllocBII(int imghandle, 
						  CRect* pSelectionRect, //pSelectionRect=NULL
						  int ivalue) //ivalue=-1
{
	if(imghandle==-1) 
	{
		//that is the buffer we want to used in here
		//when there is no already an existing accusoft iff image buffer allocated
		ASSERT(FALSE);
		return FALSE;
	}

	if(pSelectionRect!=NULL)
	{
		//this function is not yet supporting selection rect
		ASSERT(FALSE);
		//return FALSE;
	}

	if(m_pDIB!=NULL) 
	{
		//when there is already an existing windows dib image buffer allocated
		//2012july22, poirier, begin
		//ASSERT(FALSE);
		//2012july22, poirier, end
		//return FALSE;
	}
	if(m_pIplImage!=NULL)
	{
		//when there is already an existing intel ipl image buffer allocated
		//ASSERT(FALSE); 
		//return FALSE;
	}

	if(m_pData!=NULL)
	{ 
		//when there is already an existing offworld image buffer allocated
		ASSERT(FALSE); 
		//we cannot overright the image BII buffer, must call FreeBII() before new alloc
		return FALSE;
	}

	//1) to get a BITMAPINFOHEADER*, call accusoft iff library
	int width, height, bits_per_pixel;
	HANDLE hDIB = OW_IFF_IMG_bitmap_info(m_imghandle, &width, &height, &bits_per_pixel);

	//2) this block, to be eventually performed without accusoft 
	//   within COWPixelset::AllocImgBufferFromDIB
	if(bits_per_pixel==1)
	{
		//monochrome image, convert into 1 channel (8bit per pixel)
		OW_IFF_IMG_promote_8(m_imghandle);
		hDIB = OW_IFF_IMG_bitmap_info(m_imghandle, &width, &height, &bits_per_pixel);
	}
	if(bits_per_pixel==2 || bits_per_pixel==4 || bits_per_pixel==16)
	{
		//compressed colors image, convert into 3 channels (8bit per pixel)
		OW_IFF_IMG_promote_24(m_imghandle);
		hDIB = OW_IFF_IMG_bitmap_info(m_imghandle, &width, &height, &bits_per_pixel);
	}

	//3) get bitmapinfoheader calling windows global lock
	BITMAPINFOHEADER* pBIH = (BITMAPINFOHEADER*) GlobalLock(hDIB);
	//4) call offworld pixelset library - windows dib support
	int iresult = AllocBII(pBIH);
	GlobalUnlock(hDIB);
	return iresult;
}

int COWPixelset::AllocBII(IplImage* pIplImage, 
						  CRect* pSelectionRect,//=NULL, 
						  int ivalue)//=-1)
{
	if(pIplImage==NULL && m_pIplImage==NULL) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	if(pIplImage==NULL) 
	{
		//use existing ipl image 
		pIplImage = m_pIplImage;
	}
	
	if(pSelectionRect!=NULL)
	{
		//never tested
		ASSERT(FALSE);
		return FALSE;
	}
	//temp code
	/*
	if(m_pIplImage==NULL)
	{
		int depth = 8;
		int width = 400;
		int height = 400;
		//iplCreateImageJaehne creates a test image from Jaehne's book on image processing
		m_pIplImage = iplCreateImageJaehne(depth, width, height);
	}
	*/

	/*
	COWIPLImageset myIPLImageset;
	m_pIplImage = myIPLImageset.AllocIPLImage();

	BITMAPINFOHEADER myBIH;
	int dither = 0; 
	int paletteConversion = 0;
	OW_IPL_iplConvertToDIB(m_pIplImage, &myBIH, dither, paletteConversion);
	*/

	/*
	BITMAPINFOHEADER* pBIH;
	if(m_pDIB!=NULL)
	{
		pBIH = m_pDIB;
	}
	else if(m_imghandle!=-1)
	{
		pBIH = GetIFFHeaderPointer();
	}
	AllocIPL(pBIH);
	*/

	BITMAPINFOHEADER myBIH;
	int iresult = CreateBIHFrom(pIplImage, &myBIH, pSelectionRect);
	if(iresult==FALSE)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	return AllocBII(&myBIH);
}

int COWPixelset::AllocBII(class COWPixelset* pPixelset, 
						  CRect* pSelectionRect,//=NULL, 
						  int ivalue)//=-1)
{
	if(m_pData!=NULL)
	{
		//this is the destination buffer that must be allocated
		//this pixelset's BII buffer should not be allocated
		ASSERT(FALSE);
		return FALSE;
	}

	//pick the buffer from source pixelset
	BITMAPINFOHEADER myBIH;
	int iresult = TRUE;
	if(pPixelset->m_pData!=NULL)
	{
		//BII has the first priority to be picked
		iresult = CreateBIHFromBII(&myBIH, pSelectionRect);
	}
	else if(pPixelset->m_pIplImage!=NULL)
	{
		//IPL has the second priority to be picked
		iresult = CreateBIHFrom(pPixelset->m_pIplImage, &myBIH, pSelectionRect);
	}
	else if(pPixelset->m_pDIB!=NULL)
	{
		//DIB has the third priority to be picked
		myBIH = *(pPixelset->m_pDIB);
	}
	else if(pPixelset->m_imghandle!=-1)
	{
		//IFF has the fourth priority to be picked
		BITMAPINFOHEADER* pBIH = GetIFFHeaderPointer();
		//copy into myBIH
		/*
		myBIH = *pBIH;
		*/
		memcpy(&myBIH, pBIH, sizeof(BITMAPINFOHEADER));
	}
	else
	{
		//no buffer at all ???
		ASSERT(FALSE);
		return FALSE;
	}
	if(iresult==FALSE)
	{
		//out of memory
		return FALSE;
	}
	return AllocBII(&myBIH);
}


int COWPixelset::FreeBII()
{
	//no BII buffer to free
	if(m_pData==NULL) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//delete[] m_pData;
	free(m_pData);
	m_pData = NULL;
	return TRUE;
}


//can be used to clear image buffer
int COWPixelset::FillBII(int ivalue)
{
	if(m_pData==NULL) 
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//faster way (restriction: ivalue must be 8bit)
	memset(m_pData, (char)ivalue, m_datasize_inbyte);
	/*
	for(int i=0; i<m_numberofchannel; i++)
	{
		for(int i_offset=0; i_offset<m_channelsize_inbyte; i_offset++)
		{
			m_pData[i_offset] = byteColor;	
		}
	}
	*/
	return TRUE;
}

int COWPixelset::UpdateBIIFrom(BITMAPINFOHEADER* pDIB, 
							   CRect* pSelectionRect)//=NULL)
{
	if(m_pData==NULL)
	{
		ASSERT(FALSE); 
		//buffer must be allocated prior to call this function
		return FALSE;
	}

	if(pDIB==NULL && m_pDIB==NULL) 
	{
		//no dib supplied, no dib in current pixelset
		ASSERT(FALSE);
		return FALSE;
	}
	if(pDIB==NULL) pDIB = m_pDIB;

	if(pSelectionRect!=NULL)
	{
		//not supported for now
		ASSERT(FALSE);
	}

	
	if(m_imghandle!=-1) 
	{
		//when there is already an existing accusoft iff image buffer allocated
		//ASSERT(FALSE);
		//allow to be called for test
		//return FALSE;
	}
	
	if(m_pIplImage!=NULL)
	{
		//when there is already an existing intel ipl image buffer allocated
		//ASSERT(FALSE); 
		//return FALSE;
	}


	BITMAPINFOHEADER* pBIH = pDIB;
	int nColorUsedInHeaderPalette = 0;
	if(pBIH->biBitCount==0) 
	{
		nColorUsedInHeaderPalette = 0;
	}
	else if(pBIH->biBitCount==1) 
	{
		nColorUsedInHeaderPalette = 2;
	}
	else if(pBIH->biBitCount==4) 
	{
		nColorUsedInHeaderPalette = 16;
	}
	else if(pBIH->biBitCount==8) 
	{
		nColorUsedInHeaderPalette = 256;
	}
	//2020june20, spi, begin
	//RGBQUAD* pColorTable = (RGBQUAD*)((int)pBIH + (WORD)(pBIH->biSize));
	//char* pDIBData = (char*) ((int)pBIH + (int)pBIH->biSize + nColorUsedInHeaderPalette*4);
	RGBQUAD* pColorTable = (RGBQUAD*)( ((char*)pBIH) + (int)(pBIH->biSize) );
	char* pDIBData = (char*)( ((char*)pBIH) + (int)pBIH->biSize + nColorUsedInHeaderPalette * 4 );
	//2020june20, spi, end

	//assume 24 or 32 bit, no palette required
	BOOL bIsGrayScale = FALSE;
	BOOL bNeedLookUpTable = FALSE; //lookup table is the color palette 

	//set widht and height
	m_numberofpixel_perline = pBIH->biWidth;
	m_numberofline_perchannel = pBIH->biHeight;

	//set depth of 1 or 3 channel depending upon pDIB compression
	int nBitPerPixel = pBIH->biBitCount;
	if(nBitPerPixel!=24 && nBitPerPixel!=32)
	{
		bNeedLookUpTable = TRUE;

		//palette image, get color from pBIH color table
		if(nBitPerPixel==1)
		{
			//monochrome, get color from pBIH color table
		}
		else if(nBitPerPixel==8)
		{
			//assume gray scale with no palette required, but check color table entries
			bIsGrayScale = TRUE;
			bNeedLookUpTable = FALSE;
			//check if gray scale or 256 arbitrary colors
			for(int i=0; i<256; i++)
			{
				if(pColorTable[i].rgbBlue!=pColorTable[i].rgbGreen)
				{
					bIsGrayScale = FALSE;
					break;
				}
				else if(pColorTable[i].rgbBlue!=pColorTable[i].rgbRed)
				{
					bIsGrayScale = FALSE;
					break;
				}
				else if(pColorTable[i].rgbBlue!=i)
				{
					//it is a gray scale, but will need to refer to color table
					bNeedLookUpTable = TRUE;
					break;
				}
			}
		}
	}

	if(bNeedLookUpTable==TRUE)
	{
		ASSERT(FALSE);
	}

	if( nBitPerPixel==1 || (nBitPerPixel==8 && bIsGrayScale) )
	{
		//one band gray buffer
		m_numberofchannel = 1;
		m_pixelsize_inbyte = 1;
		m_linesize_inbyte = m_numberofpixel_perline * m_pixelsize_inbyte;
		m_channelsize_inbyte = m_numberofline_perchannel * m_linesize_inbyte;
		m_datasize_inbyte =  m_numberofchannel * m_channelsize_inbyte;
		if(nBitPerPixel==1)
		{
			//1 bit per pixel, will have difficulty reading input pDIBData 
			//(try it, read one input byte, that is 8 pixels, 
			//filter the 8bits of that byte using successive logical & mask 
			//like 00000001, 00000010, 000000100, 00001000, 
			// 00010000, 00100000, 01000000, 10000000
			ASSERT(FALSE);
			return FALSE;
		}
	}
	else if(nBitPerPixel==2 || nBitPerPixel==4 || nBitPerPixel==16
				|| nBitPerPixel==24 || nBitPerPixel==32)
	{
		//3 bands color buffer
		m_numberofchannel = 3;
		m_pixelsize_inbyte = 1;
		m_linesize_inbyte = m_numberofpixel_perline * m_pixelsize_inbyte;
		m_channelsize_inbyte = m_numberofline_perchannel * m_linesize_inbyte;
		m_datasize_inbyte =  m_numberofchannel * m_channelsize_inbyte;

	}
	else
	{
		ASSERT(FALSE);
		return FALSE;
	}

	/*
	//alloc image buffer
	m_pData = (char*) malloc(m_datasize_inbyte);
	if(m_pData==NULL) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	*/

	////////////////////////////////
	//copy pixel data
	//case: no lookup table required
	////////////////////////////////
	if( (nBitPerPixel==8 && bIsGrayScale==TRUE && bNeedLookUpTable==FALSE) 
		|| nBitPerPixel==24 || nBitPerPixel==32)
	{

		if(nBitPerPixel==8)
		{
			//copy image raster data (DIB, upside down, 1 byte per pixel)
			int extranumberofpixel_perline = 0;
			if(m_numberofpixel_perline%4!=0)
			{
				extranumberofpixel_perline = 4-m_numberofpixel_perline%4;
			}
			double fdib_pixelsize_inbyte = nBitPerPixel/8.0;
			int dib_pixelsize_inbyte = (int)(nBitPerPixel/8.0);
			int dib_linesize_inbyte = (int)( (pBIH->biWidth + extranumberofpixel_perline) * dib_pixelsize_inbyte);
			int dib_numberofline_perchannel = pBIH->biHeight;
			for(int i=0; i<m_numberofline_perchannel; i++)
			{
				int i_offset = i*m_linesize_inbyte; //linestart
				int i_offsetdib = (m_numberofline_perchannel-1-i)*dib_linesize_inbyte; //corresponding linestart in accusoft DIB buffer
				memcpy(&(m_pData[i_offset]),&(pDIBData[i_offsetdib]), m_linesize_inbyte);
			}
		}
		else if(nBitPerPixel==24 || nBitPerPixel==32)
		{
			//copy raster image data (DIB, upside down, 8bit/pixel, BGR ordered)
			int extranumberofpixel_perline = 0;
			if(m_numberofpixel_perline%4!=0)
			{
				extranumberofpixel_perline = 4-m_numberofpixel_perline%4;
			}
			double fdib_pixelsize_inbyte = nBitPerPixel/8.0;
			int dib_pixelsize_inbyte = (int)(nBitPerPixel/8.0);
			ASSERT(nBitPerPixel==24 || nBitPerPixel==32);

			int dib_linesize_inbyte = (int)( (pBIH->biWidth + extranumberofpixel_perline) * dib_pixelsize_inbyte);
			int dib_numberofline_perchannel = pBIH->biHeight;
			char* pCharBuffer = (char*) malloc(dib_linesize_inbyte);//accu_numberofchannel*m_linesize_inbyte);		
			for(int i=0; i<m_numberofline_perchannel; i++)
			{
				int i_offset = i*m_linesize_inbyte; //linestart for channel 0
				int i_offsetdib = (m_numberofline_perchannel-1-i)*dib_linesize_inbyte; //corresponding linestart in accusoft DIB buffer
				memcpy(pCharBuffer,&(pDIBData[i_offsetdib]), dib_linesize_inbyte);
				for(int j=0; j<m_numberofpixel_perline; j++)
				{
					//color, write to the 3 channel
					//blue channel
					m_pData[i_offset+2*m_channelsize_inbyte+j] = pCharBuffer[j*dib_pixelsize_inbyte]; 
					//green channel
					m_pData[i_offset+m_channelsize_inbyte+j] = pCharBuffer[j*dib_pixelsize_inbyte+1];
					//red channel
					m_pData[i_offset+j] = pCharBuffer[j*dib_pixelsize_inbyte+2]; 
				}
			}
			//delete[] pCharBuffer;
			free(pCharBuffer);
		}
		else
		{
			//buffer allocated but no pixel data copied
			ASSERT(FALSE);
			return FALSE;
		}


	}
	////////////////////////////////
	//copy pixel data
	//case: lookup table is required
	////////////////////////////////
	else
	{
		ASSERT(bNeedLookUpTable==TRUE);
		//from pDIB's lookup table

		//easy to implement, no time today!
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}

int COWPixelset::UpdateBIIFrom(int imghandle, 
							   CRect* pSelectionRect)//=NULL)
{
	//1) to get a BITMAPINFOHEADER*, call accusoft iff library
	int width, height, bits_per_pixel;
	HANDLE hDIB = OW_IFF_IMG_bitmap_info(m_imghandle, &width, &height, &bits_per_pixel);
	BITMAPINFOHEADER* pDIB = (BITMAPINFOHEADER*) GlobalLock(hDIB);

	UpdateBIIFrom(pDIB, pSelectionRect);
	GlobalUnlock(hDIB);
	return TRUE;
}

int COWPixelset::UpdateBIIFrom(IplImage* pIplImage, 
							   CRect* pSelectionRect)//=NULL)
{
	//non-optimized implementation
	int iresult = FALSE;
	
	//alloc a temporary dib from ipl image
	BITMAPINFOHEADER* pDIB = CreateDIBFrom(pIplImage);
	if(pDIB==NULL) return FALSE;
	//copy DIB to BII
	iresult = UpdateBIIFrom(pDIB, pSelectionRect);
	//free DIB
	free((char*)pDIB);
	
	/* //for test
	FillBII(128);
	*/
	return iresult;
}

int COWPixelset::UpdateBIIFrom(class COWPixelset* pPixelset, 
							   CRect* pSelectionRect)//=NULL)
{
	int iresult = FALSE;
	//2012july22, poirier, begin
	/*
	if(m_pData!=NULL)
	{
		//this is the destination buffer that must be allocated
		//this pixelset's BII buffer should not be allocated
		ASSERT(FALSE);
		return FALSE;
	}
	*/
	//2012july22, poirier, end
	if(pPixelset==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//pick the buffer from source pixelset
	if(pPixelset->m_pData!=NULL)
	{
		//BII has the first priority to be picked
		//2012july22, poirier, begin
		//iresult = UpdateBIIFromBII(PIXELSET_ALL, pPixelset->m_pData, NULL, pSelectionRect);
		iresult = UpdateBIIFromBII(PIXELSET_ALL, pPixelset->m_pData, pSelectionRect, NULL);
		//2012july22, poirier, end
	}
	else if(pPixelset->m_pIplImage!=NULL)
	{
		//IPL has the second priority to be picked
		iresult = UpdateBIIFrom(pPixelset->m_pIplImage, pSelectionRect);
	}
	else if(pPixelset->m_pDIB!=NULL)
	{
		//DIB has the third priority to be picked
		iresult = UpdateBIIFrom(pPixelset->m_pDIB, pSelectionRect);
	}
	else if(pPixelset->m_imghandle!=-1)
	{
		iresult = UpdateBIIFrom(pPixelset->m_imghandle, pSelectionRect);
	}
	else
	{
		//no buffer at all ???
		ASSERT(FALSE);
		return FALSE;
	}
	return iresult;
}

int COWPixelset::UpdateBIIFromBII(int ichannel, //=-1 for all channels, 
								  char* pSrcData, //=NULL, 
								  CRect* pSrcDataRect, //=NULL, 
								  CRect* pSelectionRect)//=NULL)
{
	if(m_pData==NULL)
	{
		//BII buffer not allocated!
		ASSERT(FALSE);
		return FALSE;
	}
	if(pSrcDataRect->Width()!=pSelectionRect->Width())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(pSrcDataRect->Height()!=pSelectionRect->Height())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(pSrcDataRect==NULL && pSelectionRect==NULL)
	{
		memcpy(m_pData, pSrcData, m_datasize_inbyte);
	}
	else
	{
		ASSERT(FALSE);
	}
	return TRUE;
}

int COWPixelset::CreateBIHFromBII(BITMAPINFO* pBI, //destination
								  CRect* pSelectionRect)//=NULL)
{	
	
	//attach one of the default palette
	if(m_pixelsize_inbyte==1)
	{
		//pBI->bmiColors = m_pPalette_256GrayTones;
		ASSERT(FALSE);

	}
	//create BIH
	return CreateBIHFromBII(&(pBI->bmiHeader), pSelectionRect);
}

int COWPixelset::CreateBIHFromBII(BITMAPINFOHEADER* pBIH, //destination
								  CRect* pSelectionRect)//=NULL)
{
	if(pBIH==NULL)
	{
		//destination BIH, BITMAPINFOHEADER should be statically declared by caller
		ASSERT(FALSE);
		return FALSE;
	}
	if(pSelectionRect!=NULL)
	{
		//not supported yet
		ASSERT(FALSE);
	}

	//char* m_pData;	//pixelset image buffer
	//int m_datasize_inbyte;
	//int m_pixelsize_inbyte;
	//int m_linesize_inbyte;
	//int m_channelsize_inbyte;
	//int m_numberofpixel_perline;	//column
	//int m_numberofline_perchannel;	//row
	//int m_numberofchannel;			//band

	//set bitmap info header header
	pBIH->biSize = sizeof(BITMAPINFOHEADER); //for various BITMAPINFOHEADER version support
	//pBIH->biWidth = width; 
	//pBIH->biHeight = height; 
	pBIH->biWidth = m_numberofpixel_perline; 
	pBIH->biHeight = m_numberofline_perchannel; 

	pBIH->biPlanes = 1; 
	//pBIH->biBitCount = depth; 
	pBIH->biBitCount = m_pixelsize_inbyte * 8 * m_numberofchannel; 

	pBIH->biCompression = BI_RGB; 
	pBIH->biSizeImage = 0; //can be set to 0 when BI_RGB


	pBIH->biXPelsPerMeter = 0; 
	pBIH->biYPelsPerMeter = 0; 
	pBIH->biClrUsed = 0; 
	/*
	//2012july21, begin
	if(pBIH->biBitCount==8)
	{
		pBIH->biClrUsed = 256; 
	}
	//2012july21, end
	*/
	pBIH->biClrImportant = 0; 

	if(pBIH->biBitCount==8)
	{
		//256 color palette will have to be created (not in here) 
		//but later when that BIH is allocated with the data
		//ASSERT(FALSE);
		/*
		pPalette = CreateDIBPalette(BITMAPINFOHEADER* pBIH, RGBQUAD* pRGBQUAD, int flag=0);
		*/
		//2012july21, begin
		//pPalette = CreateDIBPalette(BITMAPINFOHEADER* pBIH, RGBQUAD* pRGBQUAD, int flag=0);
		//2012july21, end
	}
	return TRUE;
}



/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//OFFWORLD - IPL - INTEL IMAGE PROCESSING LIBRARY
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
int COWPixelset::AllocIPL(int depth, 
						  int width, 
						  int height, 
						  int ivalue)//=-1)
{
	if(m_pIplImage!=NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_pIplImage = CreateIPLFrom(depth, width, height, ivalue);
	if(m_pIplImage==NULL) return FALSE;

	return TRUE;
}

int COWPixelset::AllocIPL(BITMAPINFOHEADER* pBIH,
						  CRect* pSelectionRect, //=NULL, 
						  int ivalue) //=-1)
{
	if(pBIH==NULL && m_pDIB==NULL) 
	{
		//no dib supplied, no dib already in pixelset
		return FALSE;
	}
	if(pBIH==NULL) 
	{
		//use existing pixelset dib
		pBIH = m_pDIB;
	}

	m_pIplImage = CreateIPLFrom(pBIH, pSelectionRect, ivalue);
	if(m_pIplImage==NULL) return FALSE;

	return TRUE;
}


int COWPixelset::AllocIPL(int imghandle, 
						  CRect* pSelectionRect,//=NULL, 
						  int ivalue)//=-1)
{
	if(m_imghandle==-1)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	BITMAPINFOHEADER* pBIH = GetIFFHeaderPointer(imghandle);
	if(pBIH==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	return AllocIPL(pBIH, pSelectionRect, ivalue);
}

int COWPixelset::AllocIPL(IplImage* pIplImage, 
						  CRect* pSelectionRect,//=NULL, 
						  int ivalue)//=-1)
{
	if(m_pIplImage!=NULL)
	{
		//buffer should not be allocated
		ASSERT(FALSE);
		return FALSE;
	}
	if(pIplImage==NULL)
	{
		//no supplied image
		ASSERT(FALSE);
		return FALSE;
	}

	/*
	BITMAPINFOHEADER myBIH;
	int iresult = CreateBIHFrom(pIplImage, &myBIH, pSelectionRect);
	if(iresult==FALSE)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	return AllocIPL(&myBIH, pSelectionRect, ivalue);
	*/
	m_pIplImage = CreateIPLFrom(pIplImage, pSelectionRect, ivalue);
	if(m_pIplImage==NULL) return FALSE;

	return TRUE;
}

int COWPixelset::AllocIPL(class COWPixelset* pPixelset, 
						  CRect* pSelectionRect,//=NULL, 
						  int ivalue)//=-1)
{
	if(m_pIplImage!=NULL)
	{
		//this is the destination buffer that must be allocated
		//this pixelset's IPL buffer should not be currently allocated
		ASSERT(FALSE);
		return FALSE;
	}

	//pick the buffer from source pixelset
	BITMAPINFOHEADER myBIH;
	int iresult = TRUE;
	if(pPixelset->m_pData!=NULL)
	{
		//BII has the first priority to be picked
		iresult = CreateBIHFromBII(&myBIH, pSelectionRect);
	}
	else if(pPixelset->m_pIplImage!=NULL)
	{
		//IPL has the second priority to be picked
		iresult = CreateBIHFrom(pPixelset->m_pIplImage, &myBIH, pSelectionRect);
	}
	else if(pPixelset->m_pDIB!=NULL)
	{
		//DIB has the third priority to be picked
		myBIH = *(pPixelset->m_pDIB);
	}
	else if(pPixelset->m_imghandle!=-1)
	{
		//IFF has the fourth priority to be picked
		BITMAPINFOHEADER* pBIH = GetIFFHeaderPointer();
		//copy into myBIH
		/*
		myBIH = *pBIH;
		*/
		memcpy(&myBIH, pBIH, sizeof(BITMAPINFOHEADER));
	}
	else
	{
		//no buffer at all ???
		ASSERT(FALSE);
		return FALSE;
	}

	if(iresult==FALSE)
	{
		//out of memory
		return FALSE;
	}
	return AllocIPL(&myBIH, pSelectionRect, ivalue);
}

IplImage* COWPixelset::CreateIPLFrom(int depth, int width, int height, int ivalue)
{
	BITMAPINFOHEADER* pDIB = CreateDIBFrom(depth, width, height, ivalue);
	if(pDIB==NULL) return FALSE;
	IplImage* pIplImage = CreateIPLFrom(pDIB);
	free(pDIB);
	return pIplImage;
}

IplImage* COWPixelset::CreateIPLFrom(IplImage* pIplImage, CRect* pSelectionRect, int ivalue)
{
	if(pIplImage==NULL)
	{
		//no buffer supplied
		ASSERT(FALSE);
		return FALSE;
	}
	IplImage* src_pIplImage = pIplImage;
	//alloc header
	IplImage* dest_pIplImage = (IplImage*) malloc(sizeof(IplImage));
	//copy header
	memcpy(dest_pIplImage, src_pIplImage, sizeof(IplImage));
	dest_pIplImage->imageData = NULL;
	dest_pIplImage->imageDataOrigin = NULL;
	
	//alloc data
	BOOL bInit = FALSE;
	if(ivalue!=-1) TRUE; 
	OW_IPL_iplAllocateImage(dest_pIplImage, bInit, ivalue);

	return dest_pIplImage;
}

IplImage* COWPixelset::CreateIPLFrom(BITMAPINFOHEADER* pDIB, CRect* pSelectionRect, int ivalue)
{
	if(pDIB==NULL) 
	{
		//no dib supplied
		ASSERT(FALSE);
		return FALSE;
	}

	IplImage* pIplImage = CreateIPLHeader(pDIB);
	if(pIplImage)
	{
		////////////////////////////
		//3) allocate ipl image data
		////////////////////////////
		BOOL bInit = FALSE;
		if(ivalue!=-1) TRUE; 
		OW_IPL_iplAllocateImage(pIplImage,bInit, ivalue);

		//////////////////////////////
		//4) set ipl image data (copy)
		//////////////////////////////
		//UpdateIPLFrom(pBIH)
	}
	return pIplImage;
}

IplImage* COWPixelset::CreateIPLFrom(int imghandle, CRect* pSelectionRect, int ivalue)
{
	if(imghandle==-1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	BITMAPINFOHEADER* pBIH = GetIFFHeaderPointer(imghandle);
	if(pBIH==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	IplImage* pIplImage = CreateIPLFrom(pBIH, pSelectionRect, ivalue);
	return pIplImage;
}


IplImage* COWPixelset::CreateIPLHeader(BITMAPINFOHEADER* pBIH)
{
	if(pBIH==NULL) 
	{
		ASSERT(FALSE);
		return NULL;
	}

	/*
	BOOL bClone = TRUE;
	m_pIplImage = iplTranslateDIB(pDIB, &bClone);
	*/
	/*
	OW_IPL_iplConvertFromDIB(pDIB, m_pIplImage);
     */
	
	/*
    pBIH->biSize;
    pBIH->biWidth;
    pBIH->biHeight;
    pBIH->biPlanes;
    pBIH->biBitCount;
    pBIH->biCompression;
    pBIH->biSizeImage;
    pBIH->biXPelsPerMeter;
    pBIH->biYPelsPerMeter;
    pBIH->biClrUsed;
    pBIH->biClrImportant;
	*/

	/////////////////////////////////////////////////
	//1) initialize an iplimage header from dib's bih	
	/////////////////////////////////////////////////
	if(pBIH->biBitCount!=8 && pBIH->biBitCount!=24)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//channel
	int nChannels = pBIH->biBitCount/8;
	int alphaChannel = 0;
	//depth: IPL_DEPTH_1U, IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16U, IPL_DEPTH_16S, IPL_DEPTH_32S, IPL_DEPTH_32F
	int depth = IPL_DEPTH_8U;
	//color model
	char colorModel[5];
	if(pBIH->biBitCount==8) 
	{
		strcpy_s(colorModel, 5,"GRAY");
	}
	else if(pBIH->biBitCount==24) 
	{
		strcpy_s(colorModel, 5,"RGB");
	}
	else 
	{
		ASSERT(FALSE);
	}
	//color channel sequence
	char channelSeq[4];
	if(pBIH->biBitCount==8) 
	{
		strcpy_s(channelSeq, 4,"BGR");
	}
	else if(pBIH->biBitCount==24) 
	{
		strcpy_s(channelSeq, 4,"BGR");
	}
	else 
	{
		ASSERT(FALSE);
	}
	//data order: IPL_DATA_ORDER_PIXEL or IPL_DATA_ORDER_PLANE
	int dataOrder = IPL_DATA_ORDER_PIXEL;
	//origin of the image: IPL_ORIGIN_TL or IPL_ORIGIN_BL
	int origin = IPL_ORIGIN_BL;
	//alignment of image data:	IPL_ALIGN_4BYTES (IPL_ALIGN_DWORD) or 
	//							IPL_ALIGN_8BYTES (IPL_ALIGN_QWORD) or 
	//							IPL_ALIGN_16BYTES IPL_ALIGN_32BYTES.
	int align = IPL_ALIGN_4BYTES;
	//image width
    int width = pBIH->biWidth;
	//image height
	int height = pBIH->biHeight;
	//roi
	IplROI* pIplROI = NULL;
	//roi mask
	IplImage* pIplImageMaskROI = NULL;
	//id
	void* imageId = NULL;   
	//info
	IplTileInfo* pIplTileInfo = NULL;

	//////////////////////////////
	//2) allocate ipl image header
	//////////////////////////////
	IplImage* pIplImage = OW_IPL_iplCreateImageHeader(	nChannels,  
														alphaChannel, 
														depth,
														colorModel, 
														channelSeq,
														dataOrder,
														origin, 
														align,
														width, 
														height, 
														pIplROI, 
														pIplImageMaskROI,
														imageId, 
														pIplTileInfo);

	return pIplImage;
}


int COWPixelset::FreeIPL()
{
	if(m_pIplImage==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	OW_IPL_iplDeallocate(m_pIplImage, IPL_IMAGE_ALL);
	m_pIplImage = NULL;
	return TRUE;
}


int COWPixelset::FillIPL(int ivalue)//=0)
{
	if(m_pIplImage==NULL) 
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//todo: call an IPL function in here
	ASSERT(FALSE);
	/* //if IPL buffer is allocated as a band interleaf
	//faster way (restriction: ivalue must be 8bit)
	memset(m_pIplImageData, (char)ivalue, m_datasize_inbyte);
	*/
	return TRUE;
}

//pDIB data should be allocated before calling this function
int COWPixelset::UpdateIPLFrom(BITMAPINFOHEADER* pDIB, 
							   CRect* pSelectionRect)//=NULL)
{
	if(m_pIplImage==NULL)
	{
		//no allocated ipl buffer!
		ASSERT(FALSE);
		return FALSE;
	}
	if(pSelectionRect!=NULL)
	{
		//not supported yet
		ASSERT(FALSE);
		return FALSE;
	}

	RGBQUAD* pRGBQUAD = GetDIBPalettePointer(pDIB);
	int nRGBQUAD = GetDIBPaletteSize(pDIB);
	char* pDIBData = GetDIBDataPointer(pDIB);
	int nSizeData = GetDIBDataSize(pDIB);

	/*
	if(pDIB->biBitCount==8)
	{
		//let's try to copy directly

		//assert buffers same size
		ASSERT(m_pIplImage->imageSize==nSizeData);
		//assert buffers same depth
		ASSERT(m_pIplImage->nChannels==(pDIB->biBitCount/8));
		memcpy(m_pIplImage->imageData, pDIBData, nSizeData);
	}
	else
	{
	*/
		//1) create new IPL header for the supplied pDIB, attach that pDIB data to the new IPL header
		IplImage* pIplImage = CreateIPLHeader(pDIB);
		/*
		int             imageSize; //useful size in bytes          
		char           *imageData;  //pointer to aligned image      
		int             widthStep;   //size of aligned line in bytes  
		int             BorderMode[4]; //                              
		int             BorderConst[4];                                 
		char           *imageDataOrigin;   // ptr to full, nonaligned image 
		*/
		pIplImage->imageSize = nSizeData;
		pIplImage->imageData = pDIBData;
		pIplImage->imageDataOrigin = pDIBData;

		//2) iplConvert(IplImage* pIplImageSource, IplImage* pIplImageDest)
		IplImage* pIplImageSource = pIplImage;
		IplImage* pIplImageDest = m_pIplImage;
		OW_IPL_iplConvert(pIplImageSource, pIplImageDest);
	/*
	}
	*/
	return TRUE;
}

int COWPixelset::UpdateIPLFrom(int imghandle, 
							   CRect* pSelectionRect)//=NULL)
{
	BITMAPINFOHEADER* pDIB = GetIFFHeaderPointer(imghandle);
	UpdateIPLFrom(pDIB, pSelectionRect);
	return TRUE;
}

int COWPixelset::UpdateIPLFrom(IplImage* pIplImage, 
							   CRect* pSelectionRect)//=NULL)
{
	IplImage* pIplImageSource = pIplImage;
	IplImage* pIplImageDest = m_pIplImage;
	OW_IPL_iplConvert(pIplImageSource, pIplImageDest);
	return TRUE;
}

int COWPixelset::UpdateIPLFrom(class COWPixelset* pPixelset, 
							   CRect* pSelectionRect)//=NULL)
{
	int iresult = FALSE;
	if(m_pIplImage!=NULL)
	{
		//this is the destination buffer that must be allocated
		//this pixelset's BII buffer should not be allocated
		ASSERT(FALSE);
		return FALSE;
	}
	if(pPixelset==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//pick the buffer from source pixelset
	if(pPixelset->m_pData!=NULL)
	{
		//BII has the first priority to be picked
		iresult = UpdateIPLFromBII(PIXELSET_ALL, pPixelset->m_pData, NULL, pSelectionRect);
	}
	else if(pPixelset->m_pIplImage!=NULL)
	{
		//IPL has the second priority to be picked
		iresult = UpdateIPLFrom(pPixelset->m_pIplImage, pSelectionRect);
	}
	else if(pPixelset->m_pDIB!=NULL)
	{
		//DIB has the third priority to be picked
		iresult = UpdateIPLFrom(pPixelset->m_pDIB, pSelectionRect);
	}
	else if(pPixelset->m_imghandle!=-1)
	{
		iresult = UpdateIPLFrom(pPixelset->m_imghandle, pSelectionRect);
	}
	else
	{
		//no buffer at all ???
		ASSERT(FALSE);
		return FALSE;
	}
	return iresult;
}

int COWPixelset::UpdateIPLFromBII(int ichannel, //=-1 for all channels, 
								  char* pSrcData, //=NULL, 
								  CRect* pSrcDataRect, //=NULL, 
								  CRect* pSelectionRect) //=NULL)
{
	if(m_pIplImage==NULL)
	{
		//IPL buffer must be allocated first!
		ASSERT(FALSE);
		return FALSE;
	}
	if(pSrcData==NULL) 
	{
		pSrcData = m_pData;
		if(m_pData==NULL) 
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}

	//based on the rect dimension of this allocated pixelset image handle
	CRect myRect(0, 0, m_numberofpixel_perline, m_numberofline_perchannel);
	if(pSelectionRect!=NULL && pSrcDataRect==NULL)
	{
		//selection rect cannot be specified if pSrcDataRect is not
		ASSERT(FALSE);
		return FALSE;
	}
	if(pSrcDataRect==NULL)
	{
		//set data source rect to the whole image
		pSrcDataRect = &myRect;
	}
	if(pSelectionRect==NULL)
	{
		//set selection to the whole image
		pSelectionRect = &myRect;
	}
	CPoint pt_offset = pSelectionRect->TopLeft();
	ASSERT(pt_offset.x%4==0);


	int width = m_pIplImage->width;
	int height = m_pIplImage->height;
	int bits_per_pixel = m_pIplImage->nChannels * m_pIplImage->depth;

	if(bits_per_pixel!=8 && bits_per_pixel!=24 && bits_per_pixel!=32)
	{
		//not supported for now
		ASSERT(FALSE);
		return FALSE;
	}
	if( m_numberofpixel_perline != (int)width
		|| m_numberofline_perchannel != (int)height
	//	|| m_pixelsize_inbyte != (int)bits_per_pixel/8)
		|| (m_pixelsize_inbyte * m_numberofchannel) != (int)bits_per_pixel/8)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if( m_numberofpixel_perline != (int)pSelectionRect->Width()
		|| m_numberofline_perchannel != (int)pSelectionRect->Height())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//get a pointer to image data
	/*
	BITMAPINFOHEADER* pBIH = m_pDIB;
	if(pBIH==NULL) 
	{
		ASSERT(FALSE);
		return FALSE; 
	}
	//pDIBData when no palette, for 24 and 32 bits images
	char* pDIBData = (char*) ((int)pBIH + (int)pBIH->biSize);
	*/
	char* pDIBData = m_pIplImage->imageData;
	if(m_pIplImage->nChannels>1)
	{
		//assert BGR organized
		if( !(strstr(m_pIplImage->colorModel, "RGB") && strstr(m_pIplImage->channelSeq, "BGR")) )
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}

	////////////////////////////
	//for 8 bits grayscale image
	////////////////////////////
	if(m_numberofchannel==1 && bits_per_pixel==8)
	{	
		//generate a 256 gray scale color palette
		for(int i=0; i<256; i++)
		{
			((COLORREF*)pDIBData)[i] = RGB(i,i,i); //gray palette
		}
		//pDIBData when there is a 256 colors palette
		//2020july03, spi, begin
		//pDIBData = (char*) ((int)pDIBData + (int) 256*4);
		pDIBData = (char*)((char*)pDIBData + (int)(256 * 4));
		//2020july03, spi, end
	}

	////////////////////////////
	//for 8, 24 or 32 bits image
	////////////////////////////
	int extra_numberofpixel_perline = 0;
	int accu_linesize_inbyte = m_linesize_inbyte * m_numberofchannel;
	if(PIXELSET_FORCE_4BYTES_ALIGNMENT)
	{
		if(m_numberofpixel_perline%4!=0)
		{
			extra_numberofpixel_perline = 4 - m_numberofpixel_perline%4;
			accu_linesize_inbyte = (m_numberofpixel_perline + extra_numberofpixel_perline) * m_pixelsize_inbyte * m_numberofchannel;
		}
	}
	//CPoint pt_offset(pSelectionRect->TopLeft());
	CPoint pt_final(pSelectionRect->BottomRight());
	if(pSrcDataRect->PtInRect(pt_offset)!=TRUE)
	{
		ASSERT(FALSE); //point outside image, possible?
		pt_offset = pSrcDataRect->TopLeft();
	}
	CPoint pt_inner(pt_final.x-1, pt_final.y-1);
	if(pSrcDataRect->PtInRect(pt_inner)!=TRUE)
	{
		ASSERT(FALSE); //point outside image, possible?
		pt_final = pSrcDataRect->BottomRight();
	}

	////////////////////////////
	//for 8 bits grayscale image
	////////////////////////////
	if(m_numberofchannel==1 && bits_per_pixel==8)
	{
		for(int i=0; i<pSelectionRect->Height(); i++)
		{
			int i_offset = (i+pt_offset.y)*pSrcDataRect->Width() + pt_offset.x; 
			int i_offsetaccusoft = (m_numberofline_perchannel-1-i)*accu_linesize_inbyte; //corresponding linestart in accusoft DIB buffer
			memcpy(&(pDIBData[i_offsetaccusoft]), &(pSrcData[i_offset]), m_linesize_inbyte);
		}
	}
	/////////////////////
	//24 or 32 bits image
	/////////////////////
	else if(m_numberofchannel==3 && (bits_per_pixel==24 || bits_per_pixel==32))
	{
		int accu_pixelsize_inbyte = m_numberofchannel;
		char* pCharBuffer = (char*) malloc(accu_linesize_inbyte);//accu_numberofchannel*m_linesize_inbyte);		
		for(int i=0; i<pSelectionRect->Height(); i++)
		{
			int i_offset = (i+pt_offset.y)*pSrcDataRect->Width() + pt_offset.x; //linestart for channel 0
			int i_offsetaccusoft = (m_numberofline_perchannel-1-i)*accu_linesize_inbyte; //corresponding linestart in accusoft DIB buffer
			for(int j=0; j<m_numberofpixel_perline; j++)
			{
				//color, write to the 3 channel
				//blue channel
				pCharBuffer[j*accu_pixelsize_inbyte] = m_pData[i_offset+2*m_channelsize_inbyte+j]; 
				//green channel
				pCharBuffer[j*accu_pixelsize_inbyte+1] = m_pData[i_offset+m_channelsize_inbyte+j];
				//red channel
				pCharBuffer[j*accu_pixelsize_inbyte+2] = m_pData[i_offset+j]; 
			}
			memcpy(&(pDIBData[i_offsetaccusoft]), pCharBuffer, accu_linesize_inbyte);
		}
		free(pCharBuffer);
	}
	return TRUE;
}





/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//OFFWORLD - WINDOWS BITMAP SUPPORT (pixel interleaf)
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
int COWPixelset::AllocDIB(int depth, int width, int height, int ivalue)
{
	if(m_pDIB!=NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	m_pDIB = CreateDIBFrom(depth, width, height, ivalue);
	if(m_pDIB==NULL) return FALSE;

	return TRUE;
}

int COWPixelset::AllocDIB(IplImage* pIplImage, 
						  CRect* pSelectionRect,//=NULL, 
						  int ivalue)//=-1)
{
	if(m_pDIB!=NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	BITMAPINFOHEADER* pDIB = CreateDIBFrom(pIplImage, pSelectionRect);
	if(pDIB==NULL) return FALSE;

	m_pDIB = pDIB;
	return TRUE;
}

int COWPixelset::AllocDIB(BITMAPINFOHEADER* pDIB, 
						  CRect* pSelectionRect, //=NULL, 
						  int ivalue) //=-1)
{
	if(m_pDIB!=NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(pDIB==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	m_pDIB = CreateDIBFrom(pDIB, pSelectionRect=NULL, ivalue);
	if(m_pDIB==NULL) return FALSE;

	return TRUE;
}


int COWPixelset::AllocDIB(int imghandle, 
						  CRect* pSelectionRect, //=NULL, 
						  int ivalue) //=-1)
{
	if(m_pDIB!=NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(imghandle==-1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	m_pDIB = CreateDIBFrom(imghandle, pSelectionRect, ivalue);
	if(m_pDIB==NULL) return FALSE;

	return TRUE;
}


int COWPixelset::AllocDIB(class COWPixelset* pPixelset, 
						  CRect* pSelectionRect,//=NULL, 
						  int ivalue)//=-1)
{
	if(m_pDIB!=NULL)
	{
		//this is the destination buffer that must be allocated
		//this pixelset's DIB buffer should not be allocated
		ASSERT(FALSE);
		return FALSE;
	}

	//pick the buffer from source pixelset
	BITMAPINFOHEADER myBIH;
	int iresult = TRUE;
	if(pPixelset->m_pData!=NULL)
	{
		//BII has the first priority to be picked
		iresult = CreateBIHFromBII(&myBIH, pSelectionRect);
	}
	else if(pPixelset->m_pIplImage!=NULL)
	{
		//IPL has the second priority to be picked
		iresult = CreateBIHFrom(pPixelset->m_pIplImage, &myBIH, pSelectionRect);
	}
	else if(pPixelset->m_pDIB!=NULL)
	{
		//DIB has the third priority to be picked
		myBIH = *(pPixelset->m_pDIB);
	}
	else if(pPixelset->m_imghandle!=-1)
	{
		//IFF has the fourth priority to be picked
		BITMAPINFOHEADER* pBIH = GetIFFHeaderPointer();
		//copy into myBIH
		myBIH = *pBIH;
	}
	else
	{
		//no buffer at all ???
		ASSERT(FALSE);
		return FALSE;
	}
	if(iresult==FALSE)
	{
		//out of memory
		return FALSE;
	}
	return AllocDIB(&myBIH);
}

BITMAPINFOHEADER* COWPixelset::CreateDIBFrom(int depth, int width, int height, int ivalue)
{
	BITMAPINFOHEADER* pDIB = NULL;

	//size palette
	int inum_palette_color_entries = 0;
	if(depth>8) 
	{
		//for 16, 24 and 32 bit images, no palette
		inum_palette_color_entries = 0;
	}
	else if(depth==1)
	{
		inum_palette_color_entries = 2;
	}
	else if(depth==2)
	{
		inum_palette_color_entries = 4;
	}
	else if(depth==4)
	{
		inum_palette_color_entries = 16;
	}
	else if(depth==8)
	{
		inum_palette_color_entries = 256;
	}
	else
	{
		//unsupported case
		ASSERT(FALSE);
		return FALSE;
	}
	//alignment 4 byte
	int extra = 0;
	if(width%4)
	{
		extra = 4-(width%4);
	}
	int nDIBDataSize = (int) (((depth/8.0) * (width+extra)) * height);

	//must always alloc/dealloc the bitmapinfoheader contiguous with the palette
	int nbytes = sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD)*inum_palette_color_entries) + nDIBDataSize; 
	pDIB = (BITMAPINFOHEADER*)malloc(nbytes); 
	
	//initialize DIB header and color table/palette
	if(pDIB)
	{
		char* pBIH = (char*)pDIB;
		char* pColorTable = &(((char*)pDIB)[sizeof(BITMAPINFOHEADER)]);

		//set bitmap info header header
		BITMAPINFOHEADER myBIH;
		myBIH.biSize = sizeof(BITMAPINFOHEADER); //for various BITMAPINFOHEADER version support
		myBIH.biWidth = width; 
		myBIH.biHeight = height; 
		myBIH.biPlanes = 1; 
		myBIH.biBitCount = depth; 
		myBIH.biCompression = BI_RGB; 
		myBIH.biSizeImage = 0; //can be set to 0 when BI_RGB
		myBIH.biXPelsPerMeter = 0; 
		myBIH.biYPelsPerMeter = 0; 
		myBIH.biClrUsed = 0; 
		myBIH.biClrImportant = 0; 
		//copy myBIH into allocated pBIH (so it can persist)
		memcpy(pDIB, &myBIH, sizeof(BITMAPINFOHEADER));

		//set bitmap info palette
		if(inum_palette_color_entries)
		{
			memset(pColorTable, 0, sizeof(RGBQUAD)*inum_palette_color_entries);
		}
	}
	return pDIB;
}


BITMAPINFOHEADER* COWPixelset::CreateDIBFrom(IplImage* pIplImage, 
											 CRect* pSelectionRect)
{
	if(pIplImage==NULL)
	{
		ASSERT(FALSE);
		return NULL;
	}
	if(pSelectionRect!=NULL)
	{
		//not supported yet
		ASSERT(FALSE);
	}
	BITMAPINFOHEADER myBIH;
	int iresult = CreateBIHFrom(pIplImage, &myBIH, pSelectionRect);
	if(iresult==FALSE) return NULL;

	int nPALsize = GetDIBPaletteSize(&myBIH);
	int nDATAsize = GetDIBDataSize(&myBIH);
	int nDIBsize = sizeof(BITMAPINFOHEADER) + nPALsize + nDATAsize;
	//alloc DIB
	BITMAPINFOHEADER* pBIH = (BITMAPINFOHEADER*) malloc(nDIBsize);
	if(pBIH==NULL) return FALSE;
	char* pDIBData = &((char*)pBIH)[sizeof(BITMAPINFOHEADER)+nPALsize];
	//copy BIH
	memcpy(pBIH, &myBIH, sizeof(BITMAPINFOHEADER));
	//copy palette
	RGBQUAD* pColorTable = (RGBQUAD*)&((char*)pBIH)[sizeof(BITMAPINFOHEADER)];
	if(nPALsize>0)
	{
		RGBQUAD* pDefaultColorTable = GetDefaultPalettePointer(myBIH.biBitCount);
		memcpy((char*)pColorTable, (char*)pDefaultColorTable, nPALsize);
	}
	//copy data
	OW_IPL_iplConvertToDIB(pIplImage, pBIH, IPL_DITHER_NONE, IPL_PALCONV_NONE);
	/*
	if(pIplImage->align==4 && pIplImage->nChannels==1)
	{
		memcpy(pDIBData, pIplImage->imageData, nDATAsize);
	}
	else
	{
		OW_IPL_iplConvertToDIB(pIplImage, pBIH, IPL_DITHER_NONE, IPL_PALCONV_NONE);
	}
	*/
	return pBIH;
}


BITMAPINFOHEADER* COWPixelset::CreateDIBFrom(BITMAPINFOHEADER* pDIB, CRect* pSelectionRect, int ivalue)
{
	if(pDIB==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(pSelectionRect!=NULL)
	{
		//revise implementation
		ASSERT(FALSE);
	}
	int nPALsize = GetDIBPaletteSize(pDIB);
	int nDATAsize = GetDIBDataSize(pDIB);
	int nDIBsize = sizeof(BITMAPINFOHEADER) + nPALsize + nDATAsize;
	
	//alloc DIB
	BITMAPINFOHEADER* dest_pDIB = (BITMAPINFOHEADER*) malloc(nDIBsize);
	if(dest_pDIB==NULL) return FALSE;
	//copy BIH
	memcpy(dest_pDIB, pDIB, sizeof(BITMAPINFOHEADER));
	//copy palette
	/* //if pDIB is not allocated, then palette is not allocated, what GetDIBPalettePointer(pDIB) returns is invalid
	char* src_pDIBPalette = (char*) GetDIBPalettePointer(pDIB);
	*/
	char* src_pDIBPalette = NULL;
	char* dest_pDIBPalette = NULL; 
	//poirier, may 2003, begin
	if(pDIB->biBitCount<24)
	{
		src_pDIBPalette = (char*) GetDefaultPalettePointer(pDIB->biBitCount);
		dest_pDIBPalette = (char*) GetDIBPalettePointer(dest_pDIB); 
	}
	//poirier, may 2003, end
	//spi, 2020june19 modif, begin
	//memcpy(dest_pDIBPalette, src_pDIBPalette, nPALsize);
	if (dest_pDIBPalette != NULL && src_pDIBPalette != NULL && nPALsize != 0)
	{
		memcpy(dest_pDIBPalette, src_pDIBPalette, nPALsize);
	}
	//spi, 2020june19 modif, end

	char* dest_pDIBData = GetDIBDataPointer(dest_pDIB);
	//copy data
	//no more, call UpdateDIBFrom(pDIB)
	if(ivalue!=-1)
	{
		memset(dest_pDIBData, ivalue, nDATAsize);
	}
	return dest_pDIB;
}


BITMAPINFOHEADER* COWPixelset::CreateDIBFrom(int imghandle, CRect* pSelectionRect, int ivalue)
{
	if(imghandle==-1)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//2012july22, poirier, begin
	//BITMAPINFOHEADER* src_pDIB = GetIFFHeaderPointer();
	BITMAPINFOHEADER* src_pDIB = GetIFFHeaderPointer(imghandle);
	//2012july22, poirier, end
	BITMAPINFOHEADER* dest_pDIB = CreateDIBFrom(src_pDIB, pSelectionRect, ivalue);
	return dest_pDIB;
}




int COWPixelset::FreeDIB()
{
	if(m_pDIB==NULL) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	/*
	char* pDIBData = GetDIBDataPointer();
	if(pDIBData) free(pDIBData); //free dib data
	free(m_pDIB); //free dib header (include palette)
	*/
	free(m_pDIB);
	m_pDIB = NULL;
	return TRUE;
}

int COWPixelset::FillDIB(int ivalue)//=0)
{
	if(m_pDIB==NULL) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	char* pDIBData = GetDIBDataPointer();
	int nDIBDataSize = GetDIBDataSize();

	//faster way (restriction: ivalue must be 8bit)
	memset(pDIBData, (char)ivalue, nDIBDataSize);
	return TRUE;
}


int COWPixelset::UpdateDIBFrom(	BITMAPINFOHEADER* pDIB, 
								CRect* pSelectionRect/*=NULL*/)
{
	if(m_pDIB==NULL) 
	{
		//DIB buffer must be allocated first
		ASSERT(FALSE);
		return FALSE;
	}
	if(pDIB==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//2012august15, poirier, begin
	//selection rect revision
	/*
	if(pSelectionRect!=NULL)
	{
		//revise implementation to support selection rect
		ASSERT(FALSE);
	}

	char* src_pDIBData = GetDIBDataPointer(pDIB);
	int src_nDIBDataSize = GetDIBDataSize(pDIB);

	char* dest_pDIBData = GetDIBDataPointer();
	int dest_nDIBDataSize = GetDIBDataSize();
	*/
	if(pSelectionRect==NULL)
	{
		char* src_pDIBData = GetDIBDataPointer(pDIB);
		int src_nDIBDataSize = GetDIBDataSize(pDIB);
		char* dest_pDIBData = GetDIBDataPointer();
		int dest_nDIBDataSize = GetDIBDataSize();
		if(src_nDIBDataSize!=dest_nDIBDataSize)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		//spi, 2020june19 fix, begin
		//original line
		memcpy(dest_pDIBData, src_pDIBData, src_nDIBDataSize);
		/*
		//note: replacing this memcpy causing problem with very large image
		//note: will now use the same strategy as the image subrect copy, that is copying image row by row
		//if((pDIB->biBitCount/8)!=m_pixelsize_inbyte) //can't use m_pixelsize_inbyte because BII is always gray
		if (pDIB->biBitCount != m_pDIB->biBitCount)
		{
			//further implement to support different pixel size
			ASSERT(FALSE);
			return FALSE;
		}
		if (pDIB->biBitCount != 8 && pDIB->biBitCount != 24)
		{
			//further implement to support such bitcount
			ASSERT(FALSE);
			return FALSE;
		}
		int irowstart = 0;
		int irowend = pDIB->biHeight;
		int src_nDIBDataRowSize = (pDIB->biWidth)*(pDIB->biBitCount / 8);
		int dest_nDIBDataRowSize = (m_pDIB->biWidth)*(m_pDIB->biBitCount / 8);//m_linesize_inbyte is only for BII
		if (src_nDIBDataRowSize != dest_nDIBDataRowSize)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		for (int irow = irowstart; irow < irowend; irow++)
		{
			char* src_pDIBDataRow = GetDIBDataPointer(pDIB) + (pDIB->biHeight -1 - irow)*src_nDIBDataRowSize;
			char* dest_pDIBDataRow = GetDIBDataPointer() + (m_pDIB->biHeight - 1 - irow)*dest_nDIBDataRowSize;
			memcpy(dest_pDIBDataRow, src_pDIBDataRow, src_nDIBDataRowSize);
		}
		*/
		//spi, 2020june19 fix, end
	}
	else
	{
		//at this stage, assert false if selection rect exceed image rect
		//at this stage, this case should be prevented because m_pDIB is already allocated
		if(pSelectionRect->top<0)
		{
			//pSelectionRect->top=0;
			ASSERT(FALSE);
			return FALSE;
		}
		if(pSelectionRect->left<0)
		{
			//pSelectionRect->left=0;
			ASSERT(FALSE);
			return FALSE;
		}
		if(pSelectionRect->bottom>pDIB->biHeight)
		{
			//pSelectionRect->bottom=pDIB->biHeight;
			ASSERT(FALSE);
			return FALSE;
		}
		//spi, 2020june19 fix, begin
		//if (pSelectionRect->bottom > pDIB->biWidth)
		if(pSelectionRect->right>pDIB->biWidth)
		//spi, 2020june19 fix, end
		{
			//pSelectionRect->bottom=pDIB->biWidth;
			ASSERT(FALSE);
			return FALSE;
		}
		//if((pDIB->biBitCount/8)!=m_pixelsize_inbyte) //can't use m_pixelsize_inbyte because BII is always gray
		if(pDIB->biBitCount!=m_pDIB->biBitCount)
		{
			//further implement to support different pixel size
			ASSERT(FALSE);
			return FALSE;
		}
		if(pDIB->biBitCount!=8 && pDIB->biBitCount!=24)
		{
			//further implement to support such bitcount
			ASSERT(FALSE);
			return FALSE;
		}
		int irowstart=pSelectionRect->top;
		int irowend=pSelectionRect->bottom;
		int icolstart=pSelectionRect->left;
		int icolend=pSelectionRect->right;
		int src_nDIBDataRowSize = (icolend-icolstart)*(pDIB->biBitCount/8);
		int dest_nDIBDataRowSize = (m_pDIB->biWidth)*(m_pDIB->biBitCount/8);//m_linesize_inbyte is only for BII
		if(src_nDIBDataRowSize!=dest_nDIBDataRowSize)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		for(int irow=irowstart; irow<irowend; irow++)
		{
			//char* src_pDIBDataRow = GetDIBDataPointer(pDIB)+irow*(pDIB->biWidth*pDIB->biBitCount/8)+icolstart*(pDIB->biBitCount/8);
			char* src_pDIBDataRow = GetDIBDataPointer(pDIB)+(pDIB->biHeight-irow)*(pDIB->biWidth*pDIB->biBitCount/8)+icolstart*(pDIB->biBitCount/8);
			//char* dest_pDIBDataRow = GetDIBDataPointer()+(irow-irowstart)*dest_nDIBDataRowSize;
			//char* dest_pDIBDataRow = GetDIBDataPointer()+(m_pDIB->biHeight-(irow-irowstart))*dest_nDIBDataRowSize;
			char* dest_pDIBDataRow = GetDIBDataPointer()+(m_pDIB->biHeight-1-(irow-irowstart))*dest_nDIBDataRowSize;
			memcpy(dest_pDIBDataRow, src_pDIBDataRow, src_nDIBDataRowSize);
		}
	}
	//2012august15, poirier, end
	return TRUE;
}

int COWPixelset::UpdateDIBFrom(int imghandle, 
							   CRect* pSelectionRect)//=NULL)
{
	if(m_pDIB==NULL) 
	{
		//DIB buffer must be allocated first
		ASSERT(FALSE);
		return FALSE;
	}
	if(imghandle==-1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(pSelectionRect!=NULL)
	{
		//revise implementation to support selection rect
		ASSERT(FALSE);
	}

	BITMAPINFOHEADER* pDIB = GetIFFHeaderPointer(imghandle);
	return UpdateDIBFrom(pDIB, pSelectionRect);
}

int COWPixelset::UpdateDIBFrom(IplImage* pIplImage, 
							   CRect* pSelectionRect)//=NULL)
{
	if(m_pDIB==NULL) 
	{
		//DIB buffer must be allocated first
		ASSERT(FALSE);
		return FALSE;
	}
	if(pIplImage==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(pSelectionRect!=NULL)
	{
		//revise implementation to support selection rect
		ASSERT(FALSE);
	}

	//copy data
	OW_IPL_iplConvertToDIB(pIplImage, m_pDIB, IPL_DITHER_NONE, IPL_PALCONV_NONE);
	return TRUE;
}

int COWPixelset::UpdateDIBFrom(class COWPixelset* pPixelset, 
							   CRect* pSelectionRect)//=NULL)
{
	int iresult = FALSE;
	//2012july22, poirier, begin
	/*
	if(m_pDIB!=NULL)
	{
		//this is the destination buffer that must be allocated
		//this pixelset's BII buffer should not be allocated
		ASSERT(FALSE);
		return FALSE;
	}
	*/
	//2012july22, poirier, end
	if(pPixelset==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//pick the buffer from source pixelset
	if(pPixelset->m_pData!=NULL)
	{
		//BII has the first priority to be picked
		//2012july22, poirier, begin
		//iresult = UpdateDIBFromBII(PIXELSET_ALL, pPixelset->m_pData, NULL, pSelectionRect);
		iresult = UpdateDIBFromBII(PIXELSET_ALL, pPixelset->m_pData, pSelectionRect, NULL);
		//2012july22, poirier, end
	}
	else if(pPixelset->m_pIplImage!=NULL)
	{
		//IPL has the second priority to be picked
		iresult = UpdateDIBFrom(pPixelset->m_pIplImage, pSelectionRect);
	}
	else if(pPixelset->m_pDIB!=NULL)
	{
		//DIB has the third priority to be picked
		iresult = UpdateDIBFrom(pPixelset->m_pDIB, pSelectionRect);
	}
	else if(pPixelset->m_imghandle!=-1)
	{
		//IFF has the fourth priority to be picked
		iresult = UpdateDIBFrom(pPixelset->m_imghandle, pSelectionRect);
	}
	else
	{
		//no buffer at all ???
		ASSERT(FALSE);
		return FALSE;
	}
	return iresult;
}

int COWPixelset::UpdateDIBFromBII(	int ichannel, //=-1 for all channels, 
									char* pSrcData/*=NULL*/, 
									CRect* pSrcDataRect/*=NULL*/, 
									CRect* pSelectionRect/*=NULL*/)
{
	if(m_pDIB==NULL)
	{
		//DIB must be allocated first!
		ASSERT(FALSE);
		return FALSE;
	}

	//based on the rect dimension of this allocated pixelset image handle
	CRect myRect(0, 0, m_numberofpixel_perline, m_numberofline_perchannel);
	//2012july22, poirier, begin
	/*
	if(pSelectionRect!=NULL && pSrcDataRect==NULL)
	{
		//selection rect cannot be specified if pSrcDataRect is not
		ASSERT(FALSE);
		return FALSE;
	}
	*/
	//2012july22, poirier, end
	if(pSrcDataRect==NULL)
	{
		//set data source rect to the whole image
		pSrcDataRect = &myRect;
	}
	if(pSelectionRect==NULL)
	{
		//set selection to the whole image
		pSelectionRect = &myRect;
	}
	CPoint pt_offset = pSelectionRect->TopLeft();
	ASSERT(pt_offset.x%4==0);

	if(pSrcData==NULL) 
	{
		pSrcData = m_pData;
		if(m_pData==NULL) 
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}

	int width = m_pDIB->biWidth;
	int height = m_pDIB->biHeight;
	int bits_per_pixel = m_pDIB->biBitCount;

	if(bits_per_pixel!=8 && bits_per_pixel!=24 && bits_per_pixel!=32)
	{
		//not supported for now
		ASSERT(FALSE);
		return FALSE;
	}
	if( m_numberofpixel_perline != (int)width
		|| m_numberofline_perchannel != (int)height
	//	|| m_pixelsize_inbyte != (int)bits_per_pixel/8)
		|| (m_pixelsize_inbyte * m_numberofchannel) != (int)bits_per_pixel/8)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if( m_numberofpixel_perline != (int)pSelectionRect->Width()
		|| m_numberofline_perchannel != (int)pSelectionRect->Height())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//get a pointer to image data
	BITMAPINFOHEADER* pBIH = m_pDIB;
	if(pBIH==NULL) 
	{
		ASSERT(FALSE);
		return FALSE; 
	}
	//pDIBData when no palette, for 24 and 32 bits images
	//2020june20, spi, begin
	//char* pDIBData = (char*)((int)pBIH + (int)pBIH->biSize);
	char* pDIBData = (char*) ( ((char*)pBIH) + (int)pBIH->biSize );
	//2020june20, spi, end

	////////////////////////////
	//for 8 bits grayscale image
	////////////////////////////
	if(m_numberofchannel==1 && bits_per_pixel==8)
	{	
		//generate a 256 gray scale color palette
		for(int i=0; i<256; i++)
		{
			((COLORREF*)pDIBData)[i] = RGB(i,i,i); //gray palette
		}
		//pDIBData when there is a 256 colors palette
		//2020june20, spi, begin
		//pDIBData = (char*)((int)pDIBData + (int)256 * 4);
		pDIBData = (char*) ( ((char*)pDIBData) + (int) 256*4 );
		//2020june20, spi, end
	}

	////////////////////////////
	//for 8, 24 or 32 bits image
	////////////////////////////
	int extra_numberofpixel_perline = 0;
	int accu_linesize_inbyte = m_linesize_inbyte * m_numberofchannel;
	if(PIXELSET_FORCE_4BYTES_ALIGNMENT)
	{
		if(m_numberofpixel_perline%4!=0)
		{
			extra_numberofpixel_perline = 4 - m_numberofpixel_perline%4;
			accu_linesize_inbyte = (m_numberofpixel_perline + extra_numberofpixel_perline) * m_pixelsize_inbyte * m_numberofchannel;
		}
	}
	//CPoint pt_offset(pSelectionRect->TopLeft());
	CPoint pt_final(pSelectionRect->BottomRight());
	if(pSrcDataRect->PtInRect(pt_offset)!=TRUE)
	{
		ASSERT(FALSE); //point outside image, possible?
		pt_offset = pSrcDataRect->TopLeft();
	}
	CPoint pt_inner(pt_final.x-1, pt_final.y-1);
	if(pSrcDataRect->PtInRect(pt_inner)!=TRUE)
	{
		ASSERT(FALSE); //point outside image, possible?
		pt_final = pSrcDataRect->BottomRight();
	}

	////////////////////////////
	//for 8 bits grayscale image
	////////////////////////////
	if(m_numberofchannel==1 && bits_per_pixel==8)
	{
		for(int i=0; i<pSelectionRect->Height(); i++)
		{
			int i_offset = (i+pt_offset.y)*pSrcDataRect->Width() + pt_offset.x; 
			int i_offsetaccusoft = (m_numberofline_perchannel-1-i)*accu_linesize_inbyte; //corresponding linestart in accusoft DIB buffer
			memcpy(&(pDIBData[i_offsetaccusoft]), &(pSrcData[i_offset]), m_linesize_inbyte);
		}
	}
	/////////////////////
	//24 or 32 bits image
	/////////////////////
	else if(m_numberofchannel==3 && (bits_per_pixel==24 || bits_per_pixel==32))
	{
		int accu_pixelsize_inbyte = m_numberofchannel;
		char* pCharBuffer = (char*) malloc(accu_linesize_inbyte);//accu_numberofchannel*m_linesize_inbyte);		
		for(int i=0; i<pSelectionRect->Height(); i++)
		{
			int i_offset = (i+pt_offset.y)*pSrcDataRect->Width() + pt_offset.x; //linestart for channel 0
			int i_offsetaccusoft = (m_numberofline_perchannel-1-i)*accu_linesize_inbyte; //corresponding linestart in accusoft DIB buffer
			for(int j=0; j<m_numberofpixel_perline; j++)
			{
				//color, write to the 3 channel
				//blue channel
				pCharBuffer[j*accu_pixelsize_inbyte] = m_pData[i_offset+2*m_channelsize_inbyte+j]; 
				//green channel
				pCharBuffer[j*accu_pixelsize_inbyte+1] = m_pData[i_offset+m_channelsize_inbyte+j];
				//red channel
				pCharBuffer[j*accu_pixelsize_inbyte+2] = m_pData[i_offset+j]; 
			}
			memcpy(&(pDIBData[i_offsetaccusoft]), pCharBuffer, accu_linesize_inbyte);
		}
		free(pCharBuffer);
	}
	return TRUE;
}


BITMAPINFOHEADER*  COWPixelset::GetDIBHeaderPointer()
{
	ASSERT(m_pDIB);
	BITMAPINFOHEADER* pBIH = m_pDIB;
	return pBIH;
}


int COWPixelset::InitializeDefaultPalettes()
{
	//palettesize = 2 * sizeof(RGBQUAD)
	int numberofRGBQUAD = 2;
	((COLORREF*)m_pPalette_Monochrome)[0] = RGB(0,0,0); //black
	((COLORREF*)m_pPalette_Monochrome)[1] = RGB(255,255,255); //white

	//palettesize = 2 * sizeof(RGBQUAD)
	numberofRGBQUAD = 4;
	((COLORREF*)m_pPalette_4Tones)[0] = RGB(0,0,0); //black
	((COLORREF*)m_pPalette_4Tones)[1] = RGB(255,255,255); //white
	((COLORREF*)m_pPalette_4Tones)[2] = RGB(128,128,128); //dark gray
	((COLORREF*)m_pPalette_4Tones)[3] = RGB(200,200,200); //pale gray

	//palettesize = numberofRGBQUAD * sizeof(RGBQUAD)
	//generate 16 gray scale palette
	numberofRGBQUAD = 16;
	int i_colorspace_step = 256/16; //linear scale, should eventually be a log()
	for(int i=0; i<numberofRGBQUAD; i++)
	{
		((COLORREF*)m_pPalette_16Tones)[i] = RGB(i,i,i); //gray palette
	}

	//palettesize = numberofRGBQUAD * sizeof(RGBQUAD)
	//generate 256 gray scale palette
	numberofRGBQUAD = 256;
	for(int i=0; i<numberofRGBQUAD; i++)
	{
		((COLORREF*)m_pPalette_256GrayTones)[i] = RGB(i,i,i); //gray palette
	}
	return TRUE;
}

RGBQUAD* COWPixelset::GetDefaultPalettePointer(int biBitCount)
{
	if(biBitCount==1)
	{
		return m_pPalette_Monochrome;
	}
	else if(biBitCount==2)
	{
		return m_pPalette_4Tones;
	}
	else if(biBitCount==4)
	{
		return m_pPalette_16Tones;
	}
	else if(biBitCount==8)
	{
		return m_pPalette_256GrayTones;
	}
	else
	{
		ASSERT(FALSE);
	}

	return m_pPalette_256GrayTones;
}

//PIXELSET_DIB_PALETTE_MONOCHROME
//PIXELSET_DIB_PALETTE_4GRAYTONES
//PIXELSET_DIB_PALETTE_16GRAYTONES
//PIXELSET_DIB_PALETTE_256GRAYTONES
//caller should always allocate a RGBQUAD[256] when does not check the pBIH
int COWPixelset::CreateDIBPalette(BITMAPINFOHEADER* pBIH,
								  RGBQUAD* pRGBQUAD, 
								  int iflag)//0 for PIXELSET_DIB_PALETTE_256GRAY
{
	if(pBIH==NULL && m_pDIB==NULL) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(pBIH==NULL) pBIH = m_pDIB;

	int numberofRGBQUAD = 0;

	if(pBIH->biBitCount==1)
	{
		if(iflag==PIXELSET_DIB_PALETTE_MONOCHROME)
		{
			numberofRGBQUAD = 2;
			((COLORREF*)pRGBQUAD)[0] = RGB(0,0,0); //black
			((COLORREF*)pRGBQUAD)[1] = RGB(255,255,255); //white
			//palettesize = 2 * sizeof(RGBQUAD)
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if(pBIH->biBitCount==2)
	{
		if(iflag==PIXELSET_DIB_PALETTE_4GRAYTONES)
		{
			numberofRGBQUAD = 4;
			((COLORREF*)pRGBQUAD)[0] = RGB(0,0,0); //black
			((COLORREF*)pRGBQUAD)[1] = RGB(255,255,255); //white
			((COLORREF*)pRGBQUAD)[2] = RGB(128,128,128); //dark gray
			((COLORREF*)pRGBQUAD)[3] = RGB(200,200,200); //pale gray
			//palettesize = 2 * sizeof(RGBQUAD)
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if(pBIH->biBitCount==4)
	{
		if(iflag==PIXELSET_DIB_PALETTE_16GRAYTONES)
		{
			//generate 16 gray scale palette
			numberofRGBQUAD = 16;
			int i_colorspace_step = 256/16; //linear scale, should eventually be a log()
			for(int i=0; i<numberofRGBQUAD; i++)
			{
				((COLORREF*)pRGBQUAD)[i] = RGB(i,i,i); //gray palette
			}
			//palettesize = numberofRGBQUAD * sizeof(RGBQUAD)
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if(pBIH->biBitCount==8)
	{
		if(iflag==PIXELSET_DIB_PALETTE_256GRAYTONES)
		{
			//generate 256 gray scale palette
			numberofRGBQUAD = 256;
			for(int i=0; i<numberofRGBQUAD; i++)
			{
				((COLORREF*)pRGBQUAD)[i] = RGB(i,i,i); //gray palette
			}
			//palettesize = numberofRGBQUAD * sizeof(RGBQUAD)
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	return numberofRGBQUAD; //if 0, will not need a palette when allocating pDIB from pBIH
}

RGBQUAD* COWPixelset::GetDIBPalettePointer(BITMAPINFOHEADER* pBIH)
{
	if(pBIH==NULL && m_pDIB==NULL) 
	{
		ASSERT(FALSE);
		return NULL;
	}
	if(pBIH==NULL) pBIH = m_pDIB;

	int nColorUsedInHeaderPalette = 0;

	if(pBIH->biBitCount==0) 
	{
		//strange BIH
		ASSERT(FALSE);
		return NULL;
	}
	else if(pBIH->biBitCount==16 || pBIH->biBitCount==24 || pBIH->biBitCount==32)
	{
		nColorUsedInHeaderPalette = 0;
		return NULL; //no palette
	}
	else if(pBIH->biBitCount==1) nColorUsedInHeaderPalette = 2;
	else if(pBIH->biBitCount==4) nColorUsedInHeaderPalette = 16;
	else if(pBIH->biBitCount==8) nColorUsedInHeaderPalette = 256;
	else
	{
		ASSERT(FALSE);
		return NULL;
	}
	//2020june20, spi, begin
	//RGBQUAD* pColorTable = (RGBQUAD*)((int)pBIH + (WORD)(pBIH->biSize));
	RGBQUAD* pColorTable = (RGBQUAD*)( ((char*)pBIH) + (int)(pBIH->biSize) );
	//2020june20, spi, end

	return pColorTable;
}

int COWPixelset::GetDIBPaletteSize(BITMAPINFOHEADER* pBIH)
{
	if(pBIH==NULL && m_pDIB==NULL) 
	{
		ASSERT(FALSE);
		return 0;
	}
	if(pBIH==NULL) pBIH = m_pDIB;

	int nColorUsedInHeaderPalette = 0;

	if(pBIH->biBitCount==0) 
	{
		//strange BIH
		ASSERT(FALSE);
		return 0;
	}
	else if(pBIH->biBitCount==16 || pBIH->biBitCount==24 || pBIH->biBitCount==32)
	{
		nColorUsedInHeaderPalette = 0;
		return 0; //no palette
	}
	else if(pBIH->biBitCount==1) nColorUsedInHeaderPalette = 2;
	else if(pBIH->biBitCount==4) nColorUsedInHeaderPalette = 16;
	else if(pBIH->biBitCount==8) nColorUsedInHeaderPalette = 256;
	else
	{
		ASSERT(FALSE);
		return NULL;
	}
	//RGBQUAD* pColorTable = (RGBQUAD*)((int)pBIH + (WORD)(pBIH->biSize));
	//return pColorTable;
	return nColorUsedInHeaderPalette * sizeof(RGBQUAD);
}


char* COWPixelset::GetDIBDataPointer(BITMAPINFOHEADER* pBIH)
{
	if(pBIH==NULL && m_pDIB==NULL) 
	{
		ASSERT(FALSE);
		return NULL;
	}
	if(pBIH==NULL) pBIH = m_pDIB;

	int nColorUsedInHeaderPalette = 0;
	if(pBIH->biBitCount==0) nColorUsedInHeaderPalette = 0;
	else if(pBIH->biBitCount==1) nColorUsedInHeaderPalette = 2;
	else if(pBIH->biBitCount==4) nColorUsedInHeaderPalette = 16;
	else if(pBIH->biBitCount==8) nColorUsedInHeaderPalette = 256;
	//2020june20, spi, begin
	//RGBQUAD* pColorTable = (RGBQUAD*)((int)pBIH + (WORD)(pBIH->biSize));
	RGBQUAD* pColorTable = (RGBQUAD*)( ((char*)pBIH) + (int)(pBIH->biSize) );
	//char* pDIBData = (char*) ((int)pBIH + (int)pBIH->biSize + nColorUsedInHeaderPalette*4);
	char* pDIBData = ((char*)(pBIH)) + ((int)(pBIH->biSize + nColorUsedInHeaderPalette * 4));
	//2020june20, spi, end
	return pDIBData;
}

int COWPixelset::GetDIBDataSize(BITMAPINFOHEADER* pBIH)
{
	if(pBIH==NULL && m_pDIB==NULL) 
	{
		ASSERT(FALSE);
		return NULL;
	}
	if(pBIH==NULL) pBIH = m_pDIB;

	int size = pBIH->biSizeImage; //could be 0

	int nchannel = pBIH->biBitCount/8;
	//ASSERT(pBIH->biBitCount==8 || pBIH->biBitCount==24);
	ASSERT(pBIH->biBitCount==8 || pBIH->biBitCount==24 || pBIH->biBitCount==32); //spi 2014, todo, support 32 bit images

	int height = pBIH->biHeight;
	int width = pBIH->biWidth;
	//alignment (4 bytes)
	int extranumberofpixel_perline = 0;
	if(width%4!=0)
	{
		extranumberofpixel_perline = 4-width%4;
	}
	width = width + extranumberofpixel_perline;
	return width*height*nchannel;
}

int COWPixelset::CreateBIHFrom(int depth, 
							   int width, 
							   int height, 
							   BITMAPINFOHEADER* pBIH, 
							   CRect* pSelectionRect) //=NULL)
{
	if(pBIH==NULL)
	{
		//BITMAPINFOHEADER structure should de created statically by caller
		ASSERT(FALSE);
		return FALSE;
	}
	if(pSelectionRect!=NULL)
	{
		//not supported yet
		ASSERT(FALSE);
		return FALSE;
	}

	////////////
	//DIB HEADER
	////////////

	BITMAPINFOHEADER myBIH;
	myBIH.biSize = sizeof(BITMAPINFOHEADER); //for various BITMAPINFOHEADER version support
	myBIH.biWidth = width; 
	myBIH.biHeight = height; 
	myBIH.biPlanes = 1; 
	myBIH.biBitCount = depth; 
	if(depth==8 || depth==24)
	{
		myBIH.biCompression = BI_RGB; 
	}
	else
	{
		//revise implementation
		ASSERT(FALSE);
	}
	myBIH.biSizeImage = 0; //can be set to 0 when BI_RGB
	myBIH.biXPelsPerMeter = 0; 
	myBIH.biYPelsPerMeter = 0; 
	myBIH.biClrUsed = 0; 
	myBIH.biClrImportant = 0; 

	*pBIH = myBIH;
	return TRUE;
}

int COWPixelset::CreateBIHFrom(	IplImage* pIplImage, //src
								BITMAPINFOHEADER* pBIH, //destination
								CRect* pSelectionRect)//=NULL)
{
	if(pBIH==NULL)
	{
		//BITMAPINFOHEADER structure should de created statically by caller
		ASSERT(FALSE);
		return FALSE;
	}
	if(pSelectionRect!=NULL)
	{
		//not supported yet
		ASSERT(FALSE);
		return FALSE;
	}

	////////////
	//DIB HEADER
	////////////

	BITMAPINFOHEADER myBIH;
	myBIH.biSize = sizeof(BITMAPINFOHEADER); //for various BITMAPINFOHEADER version support
	//myBIH.biWidth = width; 
	//myBIH.biHeight = height; 
	myBIH.biPlanes = 1; 
	//myBIH.biBitCount = depth; 
	//myBIH.biCompression = BI_RGB; 
	myBIH.biSizeImage = 0; //can be set to 0 when BI_RGB
	myBIH.biXPelsPerMeter = 0; 
	myBIH.biYPelsPerMeter = 0; 
	myBIH.biClrUsed = 0; 
	myBIH.biClrImportant = 0; 

	/////////////////
	//IPLIMAGE HEADER 
	/////////////////

    //pIplImage->nSize; //size of iplImage struct
    //pIplImage->ID; //version                
	//pIplImage->nChannels
    //pIplImage->alphaChannel; //alpha channel, 0 for none
    //pIplImage->depth; //pixel depth in bits  
    myBIH.biBitCount = pIplImage->nChannels * pIplImage->depth;
	if(myBIH.biBitCount!=8 && myBIH.biBitCount!=24)
	{
		//test this function
		ASSERT(FALSE);
	}
    //pIplImage->colorModel[4];
    //pIplImage->channelSeq[4];
    //pIplImage->dataOrder;
    //pIplImage->origin;
    //pIplImage->align; //4 or 8 byte align    
    //pIplImage->width;
    //pIplImage->height;
	myBIH.biWidth = pIplImage->width; 
	myBIH.biHeight = pIplImage->height; 
	myBIH.biCompression = BI_RGB; 

    //pIplImage->roi; //struct _IplROI *
    //pIplImage->maskROI; //struct _IplImage* //poiner to maskROI if any   
    //pIplImage->imageId; //void* //use of the application     
    //pIplImage->tileInfo; //struct _IplTileInfo *tileInfo // contains information on tiling 
    //pIplImage->imageSize; // useful size in bytes            
    //pIplImage->imageData; //char*  //pointer to aligned image        
    //pIplImage->widthStep; //int widthStep; // size of aligned line in bytes  
    //pIplImage->BorderMode; //int BorderMode[4]; // int*                                
    //pIplImage->BorderConst; //int BorderConst[4]; // int*                                
    //pIplImage->imageDataOrigin; //char *imageDataOrigin; //char * // ptr to full, nonaligned image   
	myBIH.biSizeImage = pIplImage->imageSize;

	/*
	//this BIH should be directly copied by calling function (while on the stack)
	return &myBIH; 
	*/
	/*
	*pBIH = myBIH;
	*/
	memcpy(pBIH, &myBIH, sizeof(BITMAPINFOHEADER));
	return TRUE;
}

int COWPixelset::AttachDataToBIH(BITMAPINFOHEADER*, 
								 char* pData, 
								 CRect* pSelectionRect) //=NULL)
{
	//to avoid reallocating too many image buffers 
	//to represent the same unique pixelset surface (BII, DIB, IFF, IPL, etc...)
	//usually a maximum of 2 should be present, a DIB or IFF for display (pixel interleaf)
	//and a BII or IPL for processing (band interleaf to optimize processing time)

	if(pSelectionRect!=NULL)
	{
		//not supported yet
		ASSERT(FALSE);
		return FALSE;
	}

	//to be implemented
	ASSERT(FALSE);
	return TRUE;
}

int COWPixelset::SaveDIB(const char* filename)
{
	BITMAPFILEHEADER	hdr;
	LPBITMAPINFOHEADER	lpbi;
 
	if (m_pDIB==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	CString myFilenameString(filename);
	CFile file;
	if (!file.Open(myFilenameString, CFile::modeWrite | CFile::modeCreate))
		return FALSE;
 
	//lpbi = (LPBITMAPINFOHEADER)hDIB;
	lpbi = m_pDIB;
 
	int nColors = 1 << lpbi->biBitCount;
 
	// Fill in the fields of the file header 
	hdr.bfType		= ((WORD) ('M' << 8) | 'B');	// is always "BM"
	//hdr.bfSize		= (DWORD)(GlobalSize (hDIB) + sizeof( hdr ));
	hdr.bfSize		= sizeof(BITMAPINFOHEADER) + GetDIBDataSize() + sizeof(hdr);
	//hdr.bfSize		= lpbi->biSize + sizeof(hdr);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	hdr.bfOffBits	= (DWORD) (sizeof( hdr ) + lpbi->biSize + nColors * sizeof(RGBQUAD));
 
	// Write the file header 
	file.Write( &hdr, sizeof(hdr) );
 
	// Write the DIB header and the bits 
	//file.Write( lpbi, (UINT)GlobalSize(hDIB) );
	file.Write(lpbi, (UINT)(sizeof(BITMAPINFOHEADER) + GetDIBDataSize()));
	//file.Write(lpbi, (UINT)(lpbi->biSize));
 
	return TRUE;
}


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//OFFWORLD - IFF - ACCUSOFT IMAGE FILE FORMAT LIBRARY
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
int COWPixelset::AllocIFF(int depth, 
						  int width, 
						  int height, 
						  int ivalue)
{
	BITMAPINFOHEADER myBIH;
	int iresult = CreateBIHFrom(depth, width, height, &myBIH, NULL);
	if(iresult==FALSE)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	return AllocIFF(&myBIH, NULL, ivalue);
}

int COWPixelset::AllocIFF(BITMAPINFOHEADER* pDIB, 
						  CRect* pSelectionRect, //=NULL, 
						  int ivalue)//=-1)
{
	BITMAPINFOHEADER* pDIB_iff = CreateDIBFrom(pDIB, pSelectionRect, ivalue);
	if(pDIB_iff==NULL) return FALSE;
	m_imghandle = OW_IFF_IMG_create_handle(pDIB_iff);
	if(pDIB_iff) free(pDIB_iff); 
	if(m_imghandle==-1) return FALSE;
	return TRUE;
}

int COWPixelset::AllocIFF(int imghandle, 
						  CRect* pSelectionRect, //=NULL, 
						  int ivalue) //=-1)
{
	if(imghandle==-1 && m_imghandle==-1) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	BITMAPINFOHEADER* pBIH = GetIFFHeaderPointer(imghandle);
	return AllocIFF(pBIH, pSelectionRect, ivalue);
}

int COWPixelset::AllocIFF(IplImage* pIplImage, 
						  CRect* pSelectionRect,//=NULL, 
						  int ivalue)//=-1)
{
	if(m_imghandle!=-1)
	{
		//buffer we are suppose to write to , should not be already allocated
		ASSERT(FALSE);
		return FALSE;
	}
	if(pIplImage==NULL && m_pIplImage==NULL) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	if(pIplImage==NULL) 
	{
		//use existing ipl image 
		pIplImage = m_pIplImage;
	}
	
	if(pSelectionRect!=NULL)
	{
		//never tested
		ASSERT(FALSE);
		return FALSE;
	}

	BITMAPINFOHEADER myBIH;
	int iresult = CreateBIHFrom(pIplImage, &myBIH, pSelectionRect);
	if(iresult==FALSE)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	return AllocIFF(&myBIH);
}

int COWPixelset::AllocIFF(class COWPixelset* pPixelset, 
						  CRect* pSelectionRect, //=NULL, 
						  int ivalue)//=-1)
{
	if(m_imghandle!=-1)
	{
		//this is the destination buffer that must be allocated
		//this pixelset's IFF buffer should not be allocated
		ASSERT(FALSE);
		return FALSE;
	}

	//pick the buffer from source pixelset
	//2012july21, poirier, begin
	//BITMAPINFOHEADER myBIH;
	//2012july21, poirier, end
	int iresult = TRUE;
	if(pPixelset->m_pData!=NULL)
	{
		//BII has the first priority to be picked
		//iresult = CreateBIHFromBII(&myBIH, pSelectionRect);
		iresult = CreateBIHFromBII(&m_tempBIH, pSelectionRect);
	}
	else if(pPixelset->m_pIplImage!=NULL)
	{
		//IPL has the second priority to be picked
		//iresult = CreateBIHFrom(pPixelset->m_pIplImage, &myBIH, pSelectionRect);
		iresult = CreateBIHFrom(pPixelset->m_pIplImage, &m_tempBIH, pSelectionRect);
	}
	else if(pPixelset->m_pDIB!=NULL)
	{
		//DIB has the third priority to be picked
		//myBIH = *(pPixelset->m_pDIB);
		m_tempBIH = *(pPixelset->m_pDIB);
	}
	else if(pPixelset->m_imghandle!=-1)
	{
		//IFF has the fourth priority to be picked
		BITMAPINFOHEADER* pBIH = GetIFFHeaderPointer();
		//copy into myBIH
		//myBIH = *pBIH;
		m_tempBIH = *pBIH;
	}
	else
	{
		//no buffer at all ???
		ASSERT(FALSE);
		return FALSE;
	}
	if(iresult==FALSE)
	{
		//out of memory
		return FALSE;
	}
	//return AllocIFF(&myBIH);
	return AllocIFF(&m_tempBIH);
}


int COWPixelset::FreeIFF()
{
	if(m_imghandle>-1)
	{
		OW_IFF_IMG_delete_bitmap(m_imghandle);
		m_imghandle = -1;
		/*
		//accusoft ipl library
		//if(m_ipl_image>-1) ip_image_free((void*)m_ipl_image);
		*/
		ASSERT(m_ipl_image==-1);
	}
	return TRUE;
}

int COWPixelset::FillIFF(int ivalue)//=0)
{
	if(m_imghandle==-1) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	BITMAPINFOHEADER* pBIH = GetIFFHeaderPointer();
	if(pBIH==NULL) return FALSE;

	char* pDIBData = GetDIBDataPointer(pBIH);
	int nDIBDataSize = GetDIBDataSize(pBIH);

	//faster way (restriction: ivalue must be 8bit)
	memset(pDIBData, (char)ivalue, nDIBDataSize);
	return TRUE;
}

BITMAPINFOHEADER* COWPixelset::GetIFFHeaderPointer(int imghandle)
{
	if(imghandle==-1 && m_imghandle==-1) 
	{
		//that is the buffer we want to used in here
		//when there is no already an existing accusoft iff image buffer allocated
		ASSERT(FALSE);
		return FALSE;
	}

	if(imghandle==-1) imghandle = m_imghandle;



	if(m_pDIB!=NULL) 
	{
		//when there is already an existing windows dib image buffer allocated
		//ASSERT(FALSE);
		//return FALSE;
	}
	if(m_pIplImage!=NULL)
	{
		//when there is already an existing intel ipl image buffer allocated
		//ASSERT(FALSE); 
		//return FALSE;
	}

	if(m_pData!=NULL)
	{
		//when there is already an existing offworld image buffer allocated
		//ASSERT(FALSE); 
		//return FALSE;
	}



	//1) to get a BITMAPINFOHEADER*, call accusoft iff library
	int width, height, bits_per_pixel;
	HANDLE hDIB = OW_IFF_IMG_bitmap_info(imghandle, &width, &height, &bits_per_pixel);

	//2) this block, to be eventually performed without accusoft 
	//   within COWPixelset::AllocImgBufferFromDIB
	if(bits_per_pixel==1)
	{
		//we should not modified the supplied accusoft imghandle, but hey!
		ASSERT(FALSE);

		//monochrome image, convert into 1 channel (8bit per pixel)
		OW_IFF_IMG_promote_8(imghandle);
		hDIB = OW_IFF_IMG_bitmap_info(imghandle, &width, &height, &bits_per_pixel);
	}
	if(bits_per_pixel==2 || bits_per_pixel==4 || bits_per_pixel==16)
	{
		//we should not modified the supplied accusoft imghandle, but hey!
		ASSERT(FALSE);

		//compressed colors image, convert into 3 channels (8bit per pixel)
		OW_IFF_IMG_promote_24(imghandle);
		hDIB = OW_IFF_IMG_bitmap_info(imghandle, &width, &height, &bits_per_pixel);
	}

	//3) get bitmapinfoheader calling windows global lock
	BITMAPINFOHEADER* pBIH = (BITMAPINFOHEADER*) GlobalLock(hDIB);
	return pBIH;
}












/*
int COWPixelset::AllocImgBufferFromAccusoftImgHandle(BOOL bForceMonochrome)
{
	//poirier, sept 2001, begin
	return AllocImgBufferFromAccusoftImgHandle2();
	//poirier, sept 2001, end

	if(m_imghandle==-1) return FALSE;
	if(m_pData!=NULL)
	{
		ASSERT(FALSE); //should not call AllocImg buffer if already allocated
		return FALSE;
	}
	int width, height, bits_per_pixel;
	HANDLE hDIB = OW_IFF_IMG_bitmap_info(m_imghandle, &width, &height, &bits_per_pixel);
	BITMAPINFOHEADER* pBIH = NULL;
	m_numberofpixel_perline = (long) width;
	m_numberofline_perchannel = (int) height;

	char* pDIBData;
	//int numberofbyte_perline, extranumberofallocatedbyte_perline;
	int accu_linesize_inbyte;
	int accu_datasize_inbyte;

	if(bits_per_pixel==1 || bits_per_pixel==8)
	{	
		//monochrome image, convert into 1 channel (8bit per pixel)
		if(bits_per_pixel==1)
		{
			OW_IFF_IMG_promote_8(m_imghandle);
			hDIB = OW_IFF_IMG_bitmap_info(m_imghandle, &width, &height, &bits_per_pixel);
			m_numberofpixel_perline = (int) width;	//should not have changed
			m_numberofline_perchannel = (int) height; //should not have changed
		}

		//get a pointer to image data
		pBIH = (BITMAPINFOHEADER*) GlobalLock(hDIB);
		int lsize = GlobalSize(hDIB);
		if(pBIH==NULL) 
		{
			ASSERT(FALSE);
			return FALSE; 
		}
		pDIBData = (char*) ((int)pBIH + (int)pBIH->biSize + 256*4) ;	
		//pDIBData = &( ((char*)pBIH)[pBIH->biSize] );
		//
		//numberofbyte_perline = pBIH->biSizeImage/(pBIH->biHeight*(pBIH->biBitCount/8));
		//extranumberofallocatedbyte_perline = numberofbyte_perline-m_numberofpixel_perline;
		int extranumberofpixel_perline = 0;
		if(m_numberofpixel_perline%4!=0)
		{
			extranumberofpixel_perline = 4-m_numberofpixel_perline%4;
		}
		m_numberofchannel = 1;
		m_pixelsize_inbyte = 1;
		m_linesize_inbyte = m_numberofpixel_perline * m_pixelsize_inbyte;
		m_channelsize_inbyte = m_numberofline_perchannel * m_linesize_inbyte;
		m_datasize_inbyte =  m_numberofchannel * m_channelsize_inbyte;
		accu_linesize_inbyte = (m_numberofpixel_perline + extranumberofpixel_perline) * m_pixelsize_inbyte;
		int accu_channelsize_inbyte = m_numberofline_perchannel * accu_linesize_inbyte;
		accu_datasize_inbyte = m_numberofchannel * accu_channelsize_inbyte;

		//allocate m_pData
		//m_pData = (char*) new char[m_datasize_inbyte];
		m_pData = (char*) malloc(m_datasize_inbyte);
		//m_pData = GlobalAlloc(GMEM_MOVEABLE, m_datasize_inbyte);

		if(m_pData==NULL) 
		{
			ASSERT(FALSE);
			return FALSE;
		}

		//copy image raster data
		for(int i=0; i<m_numberofline_perchannel; i++)
		{
			int i_offset = i*m_linesize_inbyte; //linestart
			int i_offsetaccusoft = (m_numberofline_perchannel-1-i)*accu_linesize_inbyte; //corresponding linestart in accusoft DIB buffer
			memcpy(&(m_pData[i_offset]),&(pDIBData[i_offsetaccusoft]), m_linesize_inbyte);
		}
		
				
	}
	else if(bits_per_pixel==4 || bits_per_pixel==24)
	{
		//color image, convert into 3 channels (24 bit per pixel)
		if(bits_per_pixel==4)
		{
			OW_IFF_IMG_promote_24(m_imghandle);
			hDIB = OW_IFF_IMG_bitmap_info(m_imghandle, &width, &height, &bits_per_pixel);
			m_numberofpixel_perline = (int) width;	//should not have changed
			m_numberofline_perchannel = (int) height; //should not have changed
		}

		//get a pointer to image data
		pBIH = (BITMAPINFOHEADER*) GlobalLock(hDIB);
		if(pBIH==NULL) 
		{
			ASSERT(FALSE);
			return FALSE; 
		}
		pDIBData = (char*) ((int)pBIH + (int)pBIH->biSize);
		//numberofbyte_perline = pBIH->biSizeImage/(pBIH->biHeight*(pBIH->biBitCount/8));
		//extranumberofallocatedbyte_perline = numberofbyte_perline-m_numberofpixel_perline;
		int extranumberofpixel_perline = 0;
		if(m_numberofpixel_perline%4!=0)
		{
			extranumberofpixel_perline = 4-m_numberofpixel_perline%4;
		}

		m_numberofchannel = 3; //color
		int accu_pixelsize_inbyte = m_numberofchannel;
		//forcing the image data buffer to be monochrome (while img handle is color)
		if(bForceMonochrome)
		{
			m_numberofchannel = 1; //monochrome
		}
		m_pixelsize_inbyte = 1; //perchannel		
		m_linesize_inbyte = m_numberofpixel_perline * m_pixelsize_inbyte;
		m_channelsize_inbyte = m_numberofline_perchannel * m_linesize_inbyte;
		m_datasize_inbyte =  m_numberofchannel * m_channelsize_inbyte;
		accu_linesize_inbyte = (m_numberofpixel_perline + extranumberofpixel_perline) * accu_pixelsize_inbyte;
		accu_datasize_inbyte = m_numberofline_perchannel * accu_linesize_inbyte;

		ASSERT(pBIH->biSizeImage==(DWORD)accu_datasize_inbyte 
			|| pBIH->biSizeImage==(DWORD)(accu_datasize_inbyte*accu_pixelsize_inbyte));

		//allocate m_pData
		//m_pData = (char*) new char[m_datasize_inbyte];
		m_pData = (char*) malloc(m_datasize_inbyte);
		if(m_pData==NULL) 
		{
			ASSERT(FALSE);
			return FALSE;
		}

		//copy raster image data (DIB, upside down, 8bit/pixel, BGR ordered)
		char* pCharBuffer = (char*) malloc(accu_linesize_inbyte);//accu_numberofchannel*m_linesize_inbyte);		
		for(int i=0; i<m_numberofline_perchannel; i++)
		{
			int i_offset = i*m_linesize_inbyte; //linestart for channel 0
			int i_offsetaccusoft = (m_numberofline_perchannel-1-i)*accu_linesize_inbyte; //corresponding linestart in accusoft DIB buffer
			memcpy(pCharBuffer,&(pDIBData[i_offsetaccusoft]), accu_linesize_inbyte);
			for(int j=0; j<m_numberofpixel_perline; j++)
			{
				if(bForceMonochrome)
				{
					
					//monochrome, merge the 3 channels (average)
					//m_pData[i_offset+j] = ((pCharBuffer[j*accu_pixelsize_inbyte+2] + //red
					//						pCharBuffer[j*accu_pixelsize_inbyte+1] + //green
					//					pCharBuffer[j*accu_pixelsize_inbyte])/3.0);  //blue
					//
					//
					//monochrome, keep green channel
					m_pData[i_offset+j] = (pCharBuffer[j*accu_pixelsize_inbyte+1]);  //green
					
				}
				else
				{
					//color, write to the 3 channel
					//blue channel
					m_pData[i_offset+2*m_channelsize_inbyte+j] = pCharBuffer[j*accu_pixelsize_inbyte]; 
					//green channel
					m_pData[i_offset+m_channelsize_inbyte+j] = pCharBuffer[j*accu_pixelsize_inbyte+1];
					//red channel
					m_pData[i_offset+j] = pCharBuffer[j*accu_pixelsize_inbyte+2]; 
				}
			}
		}
		//delete[] pCharBuffer;
		free(pCharBuffer);

	}
	else
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//poirier august 1999, begin
	if(bForceMonochrome)
	{
		
		//CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
		//pLogDocument->AddText("H");
		//m_ipl_image = (int) ip_image_alloc();
		//ip_image_associate(pBIH, (void*)m_ipl_image);
		//MODE mode = COLOR_IHS;
		//HDIB hI = DIB_alloc((DWORD) accu_linesize_inbyte, (DWORD) m_numberofline_perchannel, 8);
		//HDIB hH = DIB_alloc((DWORD) accu_linesize_inbyte, (DWORD) m_numberofline_perchannel, 8);
		//HDIB hS = DIB_alloc((DWORD) accu_linesize_inbyte, (DWORD) m_numberofline_perchannel, 8);
		//LPBMIH lpI = DIB_lock(hI);
		//LPBMIH lpH = DIB_lock(hH);
		//LPBMIH lpS = DIB_lock(hS);
		//IP_IMAGE i_ip_image; //ch0
		//IP_IMAGE h_ip_image; //ch1
		//IP_IMAGE s_ip_image; //ch2
		//IP_IMAGE ch3_ip_image; //ch3
		//ip_image_associate(lpI, i_ip_image);
		//ip_image_associate(lpH, h_ip_image);
		//ip_image_associate(lpS, s_ip_image);
		//
		//BOOL bResult = color_separate(	(void*)m_ipl_image, 
		//								mode, 
		//								(void*)i_ip_image, 
		//								(void*)h_ip_image, 
		//								(void*)s_ip_image, 
		//								(void*)ch3_ip_image, 
		//								NULL, NULL);
		//
		//
		//ASSERT(m_numberofchannel==1);
		//allocate space for 2 new channels (H and S)
		//void* pVoid = realloc(m_pData, m_channelsize_inbyte*2);
		//if(pVoid) 
		//{
		//	m_pData = (char*)pVoid;
		//	LPPIXEL pPixel = DIB_get_pntr_to_bitmap(h_ip_image);
		//	memcpy(&(m_pData[m_channelsize_inbyte]), pPixel, m_channelsize_inbyte);
		//	pLogDocument->AddText("S ");
		//	pPixel = DIB_get_pntr_to_bitmap(s_ip_image);
		//	memcpy(&(m_pData[m_channelsize_inbyte*2]), pPixel, m_channelsize_inbyte);
		//	m_numberofchannel = 3;
		//	m_datasize_inbyte = m_channelsize_inbyte * m_numberofchannel;
		//}
		//DIB_unlock(hI);
		//DIB_unlock(hH);
		//DIB_unlock(hS);
		//DIB_free(hI);
		//DIB_free(hH);
		//DIB_free(hS);
	}
	//poirier august 1999, end

	GlobalUnlock(hDIB);

	//update data member
	m_croprect.left = 0;
	m_croprect.top= 0;
	m_croprect.right = m_numberofpixel_perline;
	m_croprect.bottom = m_numberofline_perchannel;

	return TRUE;
}
*/




//copy, imgages must have identical size
//see new function UpdateIFFFrom(imghandle, rect);
BOOL COWPixelset::UpdateIFFFrom(int src_imghandle, 
								CRect* src_pSelectionRect/*=NULL*/)
{
	if(m_imghandle==-1 || src_imghandle==-1) 
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//get a pointer to src image DIB
	int src_width, src_height, src_bits_per_pixel;
	HGLOBAL src_hGlobal = OW_IFF_IMG_bitmap_info(src_imghandle, &src_width, &src_height, &src_bits_per_pixel);
	BITMAPINFOHEADER* src_pDIB = (BITMAPINFOHEADER*) GlobalLock(src_hGlobal);
	if(src_pDIB==NULL) 
	{
		ASSERT(FALSE);
		return FALSE; 
	}

	BOOL bResult = UpdateIFFFrom(src_pDIB, src_pSelectionRect);
	/*
	int src_width, src_height, src_bits_per_pixel;
	HGLOBAL src_hGlobal = OW_IFF_IMG_bitmap_info(src_imghandle, &src_width, &src_height, &src_bits_per_pixel);

	int width, height, bits_per_pixel;
	HGLOBAL hGlobal = OW_IFF_IMG_bitmap_info(m_imghandle, &width, &height, &bits_per_pixel);
	if(src_pSelectionRect==NULL)
	{
		if( src_width != (int)width || src_height != (int)height)
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	else
	{
		CPoint pt_offset = src_pSelectionRect->TopLeft();
		ASSERT(pt_offset.x%4==0);
		if( src_pSelectionRect->Width()!=(int)width || src_pSelectionRect->Height()!=(int)height)
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}

	//get a pointer to image data
	BITMAPINFOHEADER* pBIH = (BITMAPINFOHEADER*) GlobalLock(hGlobal);
	int sizeBIH = GlobalSize(hGlobal); 
	if(pBIH==NULL) 
	{
		ASSERT(FALSE);
		return FALSE; 
	}
	char* pDIBData = (char*) ((int)pBIH + (int)pBIH->biSize);
	if(bits_per_pixel==8)
	{
		//copy palette
		for(int i=0; i<256; i++)
		{
			((COLORREF*)pDIBData)[i] = RGB(i,i,i); //gray palette
		}
		//assume their is a palette 256*4
		pDIBData = (char*) ((int)pDIBData + (int)256*4);
	}

	//get a pointer to src image data
	BITMAPINFOHEADER* src_pBIH = (BITMAPINFOHEADER*) GlobalLock(src_hGlobal);
	int src_sizeBIH = GlobalSize(src_hGlobal); 
	if(src_pBIH==NULL) 
	{
		ASSERT(FALSE);
		return FALSE; 
	}
	char* src_pDIBData = (char*) ((int)src_pBIH + (int)src_pBIH->biSize);
	if(src_bits_per_pixel==8)
	{
		//assume there is a palette 256*4
		src_pDIBData = (char*) ((int)src_pDIBData + (int)256*4);
	}


	//copy data
	if(src_pSelectionRect==NULL && src_bits_per_pixel==8)
	{
		//monochrome, special case, copy whole image data
		memcpy(pDIBData, src_pDIBData, src_pBIH->biSizeImage);// +256*4);
	}
	else
	{
		CRect myRect(0, 0, src_width, src_height);
		if(src_pSelectionRect==NULL)
		{
			//set selection to the whole image
			src_pSelectionRect = &myRect;
		}

		//copy selection rect only
		int src_accu_numberofline_perchannel = src_height;
		int extra_numberofpixel_perline = 0;
		int accu_linesize_inbyte = width * bits_per_pixel/8; //m_linesize_inbyte;
		int accu_numberofline_perchannel = src_pSelectionRect->Height();
		if(m_numberofpixel_perline%4!=0)
		{
			extra_numberofpixel_perline = 4 - m_numberofpixel_perline%4;
			accu_linesize_inbyte = (m_numberofpixel_perline + extra_numberofpixel_perline) * bits_per_pixel/8; //m_pixelsize_inbyte;
		}

		int src_extra_numberofpixel_perline = 0;
		int src_accu_linesize_inbyte = src_width * src_bits_per_pixel/8;
		if(src_width%4!=0)
		{
			src_extra_numberofpixel_perline = 4 - src_width%4;
			src_accu_linesize_inbyte = (src_width + src_extra_numberofpixel_perline) * src_bits_per_pixel/8;
		}

		//copy image raster data, line by line 
		CPoint pt_offset(src_pSelectionRect->TopLeft());
		CPoint pt_final(src_pSelectionRect->BottomRight());
		CRect SrcRect(0, 0, src_width, src_height);
		if(SrcRect.PtInRect(pt_offset)!=TRUE)
		{
			ASSERT(FALSE); //point outside image, possible?
			pt_offset = SrcRect.TopLeft();
		}
		CPoint pt_inner(pt_final.x-1, pt_final.y-1);
		if(SrcRect.PtInRect(pt_inner)!=TRUE)
		{
			ASSERT(FALSE); //point outside image, possible?
			pt_final = SrcRect.BottomRight();
		}

		for(int i=0; i<src_pSelectionRect->Height(); i++)
		{
			int i_offsetaccusoft = (accu_numberofline_perchannel-1-i)*accu_linesize_inbyte; //(src_pSelectionRect->Height()-1-i)
			int src_i_offsetaccusoft =  pt_offset.x*src_bits_per_pixel/8 + (src_accu_numberofline_perchannel-1-i-pt_offset.y)*src_accu_linesize_inbyte;// + pt_offset.x; //*src_bits_per_pixel/8; //corresponding linestart in accusoft DIB buffer
			memcpy(&(pDIBData[i_offsetaccusoft]), &(src_pDIBData[src_i_offsetaccusoft]), accu_linesize_inbyte);
		}
	}
	*/

	GlobalUnlock(src_hGlobal);
	return TRUE;
}

int COWPixelset::UpdateIFFFrom(BITMAPINFOHEADER* src_pDIB, 
							   CRect* src_pSelectionRect)//=NULL)
{
	if(m_imghandle==-1 || src_pDIB==NULL) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//src
	int src_width = src_pDIB->biWidth;
	int src_height = src_pDIB->biHeight;
	int src_bits_per_pixel = src_pDIB->biBitCount;
	//dest
	int width, height, bits_per_pixel;
	HGLOBAL hGlobal = OW_IFF_IMG_bitmap_info(m_imghandle, &width, &height, &bits_per_pixel);


	if(src_pSelectionRect==NULL)
	{
		if( src_width!=width 
			|| src_height!=height
			|| src_bits_per_pixel!=bits_per_pixel)
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	else
	{
		CPoint pt_offset = src_pSelectionRect->TopLeft();
		ASSERT(pt_offset.x%4==0);
		if( src_pSelectionRect->Width()!=(int)width || src_pSelectionRect->Height()!=(int)height)
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}

	//get a pointer to dest image data
	BITMAPINFOHEADER* pBIH = (BITMAPINFOHEADER*) GlobalLock(hGlobal);
	int sizeBIH = GlobalSize(hGlobal); 
	if(pBIH==NULL) 
	{
		ASSERT(FALSE);
		return FALSE; 
	}
	//2020july03, spi, begin
	//char* pDIBData = (char*) ((int)pBIH + (int)pBIH->biSize);
	char* pDIBData = (char*)((char*)pBIH + pBIH->biSize);
	//2020july03, spi, end
	if(bits_per_pixel==8)
	{
		//copy palette
		for(int i=0; i<256; i++)
		{
			((COLORREF*)pDIBData)[i] = RGB(i,i,i); //gray palette
		}
		//assume their is a palette 256*4
		//2020july03, spi, begin
		//pDIBData = (char*) ((int)pDIBData + (int)256*4);
		pDIBData = (char*)((char*)pDIBData + (int)(256 * 4));
		//2020july03, spi, end
	}

	//get a pointer to src image data
	BITMAPINFOHEADER* src_pBIH = src_pDIB;
	if(src_pBIH==NULL) 
	{
		ASSERT(FALSE);
		return FALSE; 
	}
	//2020july03, spi, begin
	//char* src_pDIBData = (char*) ((int)src_pBIH + (int)src_pBIH->biSize);
	char* src_pDIBData = (char*)((char*)src_pBIH + src_pBIH->biSize);
	//2020july03, spi, end
	if(src_bits_per_pixel==8)
	{
		//assume there is a palette 256*4
		//2020july03, spi, begin
		//src_pDIBData = (char*)((int)src_pDIBData + (int)256 * 4);
		src_pDIBData = (char*) ((char*)src_pDIBData + (int)(256*4));
		//2020july03, spi, end
	}



	//copy data
	if(src_pSelectionRect==NULL && src_bits_per_pixel==8)
	{
		//monochrome, special case, copy whole image data
		memcpy(pDIBData, src_pDIBData, src_pBIH->biSizeImage);// +256*4);
	}
	else
	{
		CRect myRect(0, 0, src_width, src_height);
		if(src_pSelectionRect==NULL)
		{
			//set selection to the whole image
			src_pSelectionRect = &myRect;
		}

		//copy selection rect only
		int src_accu_numberofline_perchannel = src_height;
		int extra_numberofpixel_perline = 0;
		int accu_linesize_inbyte = width * bits_per_pixel/8; //m_linesize_inbyte;
		int accu_numberofline_perchannel = src_pSelectionRect->Height();
		if(PIXELSET_FORCE_4BYTES_ALIGNMENT)
		{
			if(m_numberofpixel_perline%4!=0)
			{
				extra_numberofpixel_perline = 4 - m_numberofpixel_perline%4;
				accu_linesize_inbyte = (m_numberofpixel_perline + extra_numberofpixel_perline) * bits_per_pixel/8; //m_pixelsize_inbyte;
			}
		}
		/*
		//poirier, begin, fix for color img handle forced monochrome buffer
		if(m_numberofchannel!=src_bits_per_pixel/8 && src_bits_per_pixel==24)
		{
			//fix accu_linesize_inbyte
			//accu_linesize_inbyte = accu_linesize_inbyte * src_bits_per_pixel/8;
			accu_linesize_inbyte = (m_numberofpixel_perline + extra_numberofpixel_perline) * m_pixelsize_inbyte * src_bits_per_pixel/8;
		}
		//poirier, end
		*/

		int src_extra_numberofpixel_perline = 0;
		int src_accu_linesize_inbyte = src_width * src_bits_per_pixel/8;
		if(PIXELSET_FORCE_4BYTES_ALIGNMENT)
		{
			if(src_width%4!=0)
			{
				src_extra_numberofpixel_perline = 4 - src_width%4;
				src_accu_linesize_inbyte = (src_width + src_extra_numberofpixel_perline) * src_bits_per_pixel/8;
			}
		}
		//copy image raster data, line by line 
		CPoint pt_offset(src_pSelectionRect->TopLeft());
		CPoint pt_final(src_pSelectionRect->BottomRight());
		CRect SrcRect(0, 0, src_width, src_height);
		if(SrcRect.PtInRect(pt_offset)!=TRUE)
		{
			ASSERT(FALSE); //point outside image, possible?
			pt_offset = SrcRect.TopLeft();
		}
		CPoint pt_inner(pt_final.x-1, pt_final.y-1);
		if(SrcRect.PtInRect(pt_inner)!=TRUE)
		{
			ASSERT(FALSE); //point outside image, possible?
			pt_final = SrcRect.BottomRight();
		}

		for(int i=0; i<src_pSelectionRect->Height(); i++)
		{
			int i_offsetaccusoft = (accu_numberofline_perchannel-1-i)*accu_linesize_inbyte; //(src_pSelectionRect->Height()-1-i)
			int src_i_offsetaccusoft =  pt_offset.x*src_bits_per_pixel/8 + (src_accu_numberofline_perchannel-1-i-pt_offset.y)*src_accu_linesize_inbyte;// + pt_offset.x; //*src_bits_per_pixel/8; //corresponding linestart in accusoft DIB buffer
			memcpy(&(pDIBData[i_offsetaccusoft]), &(src_pDIBData[src_i_offsetaccusoft]), accu_linesize_inbyte);
		}
	}
	GlobalUnlock(hGlobal);
	return TRUE;
}

int COWPixelset::UpdateIFFFrom(IplImage* pIplImage, 
							   CRect* pSelectionRect)//=NULL)
{
	BITMAPINFOHEADER* pDIB = CreateDIBFrom(pIplImage, pSelectionRect);
	if(pDIB)
	{
		UpdateIFFFrom(pDIB, pSelectionRect);
		free(pDIB);
		return TRUE;
	}
	return FALSE;
}

int COWPixelset::UpdateIFFFrom(class COWPixelset* pPixelset, 
							   CRect* pSelectionRect)//=NULL)
{
	int iresult = FALSE;
	if(m_imghandle==-1)
	{
		//this is the destination buffer that must be allocated
		//this pixelset's BII buffer should not be allocated
		ASSERT(FALSE);
		return FALSE;
	}
	if(pPixelset==NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//pick the buffer from source pixelset
	if(pPixelset->m_pData!=NULL)
	{
		//BII has the first priority to be picked
		iresult = UpdateIFFFromBII(PIXELSET_ALL, pPixelset->m_pData, NULL, pSelectionRect);
	}
	else if(pPixelset->m_pIplImage!=NULL)
	{
		//IPL has the second priority to be picked
		iresult = UpdateIFFFrom(pPixelset->m_pIplImage, pSelectionRect);
	}
	else if(pPixelset->m_pDIB!=NULL)
	{
		//DIB has the third priority to be picked
		iresult = UpdateIFFFrom(pPixelset->m_pDIB, pSelectionRect);
	}
	else if(pPixelset->m_imghandle!=-1)
	{
		//IFF has the fourth priority to be picked
		iresult = UpdateIFFFrom(pPixelset->m_imghandle, pSelectionRect);
	}
	else
	{
		//no buffer at all ???
		ASSERT(FALSE);
		return FALSE;
	}
	return iresult;
}

//after custom processing, accusoft img handle need to be refreshed
//used to be called RefreshAccusoftImgHandleFromImgBuffer2()
int COWPixelset::UpdateIFFFromBII(	int ichannel, //=-1 for all channels, 
									char* pSrcData/*=NULL*/, 
									CRect* pSrcDataRect/*=NULL*/, 
									CRect* pSelectionRect/*=NULL*/)
{

	/*
	//1)
	int lresult = RefreshDIBFromImgBuffer(	pSrcData, 
											pSrcDataRect, 
											pSelectionRect); 
	//2)

	*/


	//based on the rect dimension of this allocated pixelset image handle
	CRect myRect(0, 0, m_numberofpixel_perline, m_numberofline_perchannel);
	if(pSelectionRect!=NULL && pSrcDataRect==NULL)
	{
		//selection rect cannot be specified if pSrcDataRect is not
		ASSERT(FALSE);
		return FALSE;
	}
	if(pSrcDataRect==NULL)
	{
		//set data source rect to the whole image
		pSrcDataRect = &myRect;
	}
	if(pSelectionRect==NULL)
	{
		//set selection to the whole image
		pSelectionRect = &myRect;
	}
	CPoint pt_offset = pSelectionRect->TopLeft();
	ASSERT(pt_offset.x%4==0);

	if(pSrcData==NULL) 
	{
		pSrcData = m_pData;
		if(m_pData==NULL) 
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	if(m_imghandle==-1) 
	{
		ASSERT(FALSE);
		return FALSE;
	}

	int width, height, bits_per_pixel;
	HGLOBAL hDIB = OW_IFF_IMG_bitmap_info(m_imghandle, &width, &height, &bits_per_pixel);
	if(bits_per_pixel!=8 && bits_per_pixel!=24 && bits_per_pixel!=32)
	{
		//not supported for now
		ASSERT(FALSE);
		return FALSE;
	}
	if( m_numberofpixel_perline != (int)width
		|| m_numberofline_perchannel != (int)height
	//	|| m_pixelsize_inbyte != (int)bits_per_pixel/8)
		|| (m_pixelsize_inbyte * m_numberofchannel) != (int)bits_per_pixel/8)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if( m_numberofpixel_perline != (int)pSelectionRect->Width()
		|| m_numberofline_perchannel != (int)pSelectionRect->Height())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//get a pointer to image data
	BITMAPINFOHEADER* pBIH = (BITMAPINFOHEADER*) GlobalLock(hDIB);
	if(pBIH==NULL) 
	{
		ASSERT(FALSE);
		return FALSE; 
	}
	//pDIBData when no palette, for 24 and 32 bits images
	//2020july03, spi, begin
	//char* pDIBData = (char*) ((int)pBIH + (int)pBIH->biSize);
	char* pDIBData = (char*)((char*)pBIH + pBIH->biSize);
	//2020july03, spi, end

	////////////////////////////
	//for 8 bits grayscale image
	////////////////////////////
	if(m_numberofchannel==1 && bits_per_pixel==8)
	{	
		//generate a 256 gray scale color palette
		for(int i=0; i<256; i++)
		{
			((COLORREF*)pDIBData)[i] = RGB(i,i,i); //gray palette
		}
		//pDIBData when there is a 256 colors palette
		//2020july03, spi, begin
		//pDIBData = (char*) ((int)pDIBData + (int) 256*4);
		pDIBData = (char*)((char*)pDIBData + (int)(256 * 4));
		//2020july03, spi, end
	}

	////////////////////////////
	//for 8, 24 or 32 bits image
	////////////////////////////
	int extra_numberofpixel_perline = 0;
	int accu_linesize_inbyte = m_linesize_inbyte * m_numberofchannel;
	if(PIXELSET_FORCE_4BYTES_ALIGNMENT)
	{
		if(m_numberofpixel_perline%4!=0)
		{
			extra_numberofpixel_perline = 4 - m_numberofpixel_perline%4;
			accu_linesize_inbyte = (m_numberofpixel_perline + extra_numberofpixel_perline) * m_pixelsize_inbyte * m_numberofchannel;
		}
	}
	//CPoint pt_offset(pSelectionRect->TopLeft());
	CPoint pt_final(pSelectionRect->BottomRight());
	if(pSrcDataRect->PtInRect(pt_offset)!=TRUE)
	{
		ASSERT(FALSE); //point outside image, possible?
		pt_offset = pSrcDataRect->TopLeft();
	}
	CPoint pt_inner(pt_final.x-1, pt_final.y-1);
	if(pSrcDataRect->PtInRect(pt_inner)!=TRUE)
	{
		ASSERT(FALSE); //point outside image, possible?
		pt_final = pSrcDataRect->BottomRight();
	}

	////////////////////////////
	//for 8 bits grayscale image
	////////////////////////////
	if(m_numberofchannel==1 && bits_per_pixel==8)
	{
		for(int i=0; i<pSelectionRect->Height(); i++)
		{
			int i_offset = (i+pt_offset.y)*pSrcDataRect->Width() + pt_offset.x; 
			int i_offsetaccusoft = (m_numberofline_perchannel-1-i)*accu_linesize_inbyte; //corresponding linestart in accusoft DIB buffer
			memcpy(&(pDIBData[i_offsetaccusoft]), &(pSrcData[i_offset]), m_linesize_inbyte);
		}
	}
	/////////////////////
	//24 or 32 bits image
	/////////////////////
	else if(m_numberofchannel==3 && (bits_per_pixel==24 || bits_per_pixel==32))
	{
		int accu_pixelsize_inbyte = m_numberofchannel;
		char* pCharBuffer = (char*) malloc(accu_linesize_inbyte);//accu_numberofchannel*m_linesize_inbyte);		
		for(int i=0; i<pSelectionRect->Height(); i++)
		{
			int i_offset = (i+pt_offset.y)*pSrcDataRect->Width() + pt_offset.x; //linestart for channel 0
			int i_offsetaccusoft = (m_numberofline_perchannel-1-i)*accu_linesize_inbyte; //corresponding linestart in accusoft DIB buffer
			for(int j=0; j<m_numberofpixel_perline; j++)
			{
				//color, write to the 3 channel
				if(ichannel==-1 || ichannel&PIXELSET_BLUE)
				{
					//blue channel
					pCharBuffer[j*accu_pixelsize_inbyte] = m_pData[i_offset+2*m_channelsize_inbyte+j]; 
				}
				else
				{
					//blue channel
					pCharBuffer[j*accu_pixelsize_inbyte] = 0; 
				}
				if(ichannel==-1 || ichannel&PIXELSET_GREEN)
				{
					//green channel
					pCharBuffer[j*accu_pixelsize_inbyte+1] = m_pData[i_offset+m_channelsize_inbyte+j];
				}
				else
				{
					//green channel
					pCharBuffer[j*accu_pixelsize_inbyte+1] = 0;
				}
				if(ichannel==-1 || ichannel&PIXELSET_RED)
				{
					//red channel
					pCharBuffer[j*accu_pixelsize_inbyte+2] = m_pData[i_offset+j]; 
				}
				else
				{
					//red channel
					pCharBuffer[j*accu_pixelsize_inbyte+2] = 0;
				}
				
			}
			memcpy(&(pDIBData[i_offsetaccusoft]), pCharBuffer, accu_linesize_inbyte);
		}
		free(pCharBuffer);
	}
	GlobalUnlock(hDIB);
	return TRUE;
}

/*
//see new function RefreshAccusoftImgHandleFromImgBuffer2()
//see new function called UpdateIFFFromBII()
//after custom processing, accusoft img handle need to be refreshed
int COWPixelset::RefreshAccusoftImgHandleFromImgBuffer(char* pSrcData, 
														CRect* pSrcDataRect, 
														CRect* pSelectionRect)
{
	//poirier, sept 2001, begin
	return RefreshAccusoftImgHandleFromImgBuffer2(pSrcData, pSrcDataRect, pSelectionRect);
	//poirier, sept 2001, end

	//based on the rect dimension of this allocated pixelset image handle
	CRect myRect(0, 0, m_numberofpixel_perline, m_numberofline_perchannel);
	if(pSelectionRect!=NULL && pSrcDataRect==NULL)
	{
		//selection rect cannot be specified if pSrcDataRect is not
		ASSERT(FALSE);
		return FALSE;
	}
	if(pSrcDataRect==NULL)
	{
		//set data source rect to the whole image
		pSrcDataRect = &myRect;
	}
	if(pSelectionRect==NULL)
	{
		//set selection to the whole image
		pSelectionRect = &myRect;
	}
	CPoint pt_offset = pSelectionRect->TopLeft();
	ASSERT(pt_offset.x%4==0);

	if(pSrcData==NULL) 
	{
		pSrcData = m_pData;
		if(m_pData==NULL) 
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	if(m_imghandle==-1) 
	{
		ASSERT(FALSE);
		return FALSE;
	}

	int width, height, bits_per_pixel;
	HGLOBAL hGlobal = OW_IFF_IMG_bitmap_info(m_imghandle, &width, &height, &bits_per_pixel);
	if( m_numberofpixel_perline != (int)width
		|| m_numberofline_perchannel != (int)height
	//	|| m_pixelsize_inbyte != (int)bits_per_pixel/8)
		|| (m_pixelsize_inbyte * m_numberofchannel) != (int)bits_per_pixel/8)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if( m_numberofpixel_perline != (int)pSelectionRect->Width()
		|| m_numberofline_perchannel != (int)pSelectionRect->Height())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//get a pointer to image data
	BITMAPINFOHEADER* pBIH = (BITMAPINFOHEADER*) GlobalLock(hGlobal);
	if(pBIH==NULL) 
	{
		ASSERT(FALSE);
		return FALSE; 
	}
	char* pDIBData = (char*) ((int)pBIH + (int)pBIH->biSize);

	//if monochrome image
	if(m_numberofchannel==1)
	{	
		//copy palette
		for(int i=0; i<256; i++)
		{
			((COLORREF*)pDIBData)[i] = RGB(i,i,i); //gray palette
		}
		//assume there is a palette
		pDIBData = (char*) ((int)pDIBData + (int) 256*4);

		int extra_numberofpixel_perline = 0;
		int accu_linesize_inbyte = m_linesize_inbyte;
		if(m_numberofpixel_perline%4!=0)
		{
			extra_numberofpixel_perline = 4 - m_numberofpixel_perline%4;
			accu_linesize_inbyte = (m_numberofpixel_perline + extra_numberofpixel_perline) * m_pixelsize_inbyte;
		}

		CPoint pt_offset(pSelectionRect->TopLeft());
		CPoint pt_final(pSelectionRect->BottomRight());
		if(pSrcDataRect->PtInRect(pt_offset)!=TRUE)
		{
			ASSERT(FALSE); //point outside image, possible?
			pt_offset = pSrcDataRect->TopLeft();
		}
		CPoint pt_inner(pt_final.x-1, pt_final.y-1);
		if(pSrcDataRect->PtInRect(pt_inner)!=TRUE)
		{
			ASSERT(FALSE); //point outside image, possible?
			pt_final = pSrcDataRect->BottomRight();
		}
		for(i=0; i<pSelectionRect->Height(); i++)
		{
			int i_offset = (i+pt_offset.y)*pSrcDataRect->Width() + pt_offset.x; 
			int i_offsetaccusoft = (m_numberofline_perchannel-1-i)*accu_linesize_inbyte; //corresponding linestart in accusoft DIB buffer
			memcpy(&(pDIBData[i_offsetaccusoft]), &(pSrcData[i_offset]), m_linesize_inbyte);
			
			//int src_i_offsetaccusoft = (src_accu_numberofline_perchannel-1-i-pt_offset.y)*src_accu_linesize_inbyte + pt_offset.x; //corresponding linestart in accusoft DIB buffer
			//int i_offsetaccusoft = (accu_numberofline_perchannel-1-i)*accu_linesize_inbyte; //(src_pSelectionRect->Height()-1-i)
			//memcpy(&(pDIBData[i_offsetaccusoft]), &(src_pDIBData[src_i_offsetaccusoft]), accu_linesize_inbyte);
			
		}


	}
	else //color image
	{
		ASSERT(FALSE); //not supported for now
	}
	GlobalUnlock(hGlobal);

	return TRUE;
}
*/



COWPixelset* COWPixelset::Spectral_ConvertToMonochrome(int* pChannelINT, int nChannelINT, int iModeFlag)
{
	COWPixelset* pPixelset = NULL;
	char** pBIIData_src = (char**) new char*[nChannelINT];

	if(IsBIIPresent())
	{
		char* pBIIData_dest = NULL;
		//////////////
		//image buffer
		//////////////
		if(iModeFlag&PIXLESET_BUFFER_INPLACE) 
		{
			pBIIData_dest = &(m_pData[0]);
			int ichannel=0; 
			while(ichannel<nChannelINT) 
			{
				pBIIData_src[ichannel] = &(m_pData[ichannel*m_channelsize_inbyte]);
				ichannel++;
			}
		}
		else if(iModeFlag&PIXLESET_BUFFER_NEWWINDOW) 
		{
			int numberofchannel = 1;
			int numberofpixel_perline = m_numberofpixel_perline;	//column
			int numberofline_perchannel = m_numberofline_perchannel;	//row
			CRect imgRect(0,0, numberofpixel_perline, numberofline_perchannel);
			pPixelset = (COWPixelset*) new COWPixelset(NULL, &imgRect, 1, PIXELSET_IGNORE_APP_TEMPINFO);
			pBIIData_dest = &(pPixelset->m_pData[0]);
			int ichannel=0; while(ichannel<nChannelINT) 
			{
				pBIIData_src[ichannel] = &(m_pData[ichannel*m_channelsize_inbyte]);
				ichannel++;
			}
		}
		else 
		{
		}

		///////////////////////
		//compression operation
		///////////////////////
		int n_inputchannel = 0; 
		int ichannel=0; while(ichannel<nChannelINT) 
		{
			n_inputchannel += pChannelINT[ichannel];
			ichannel++;
		}
		//average
		if(iModeFlag&PIXLESET_SPECTRAL_AVERAGE)
		{
			for(int i_y=0; i_y<m_numberofline_perchannel; i_y++)
			{
				for(int i_x=0; i_x<m_numberofpixel_perline; i_x++)
				{
					int isum = 0;
					int ichannel=0; 
					while(ichannel<nChannelINT) 
					{
						if(pChannelINT[ichannel]) isum += GetPixelValue(i_x, i_y, ichannel);
						ichannel++;
					}
					isum = isum / n_inputchannel;
					pBIIData_dest[i_x+i_y*m_numberofpixel_perline] = isum;
				}
			}
		}
		//add
		else if(iModeFlag&PIXLESET_SPECTRAL_ADD)
		{
			for(int i_y=0; i_y<m_numberofline_perchannel; i_y++)
			{
				for(int i_x=0; i_x<m_numberofpixel_perline; i_x++)
				{
					int isum = 0;
					int ichannel=0; 
					while(ichannel<nChannelINT) 
					{
						if(pChannelINT[ichannel]) isum += GetPixelValue(i_x, i_y, ichannel);
						ichannel++;
					}
					pBIIData_dest[i_x+i_y*m_numberofpixel_perline] = isum;
				}
			}
		}
		//substract
		else if(iModeFlag&PIXLESET_SPECTRAL_SUBSTRACT)
		{
			for(int i_y=0; i_y<m_numberofline_perchannel; i_y++)
			{
				for(int i_x=0; i_x<m_numberofpixel_perline; i_x++)
				{
					int isub = 0;
					int ichannel=0; 
					while(ichannel<nChannelINT) 
					{
						if(pChannelINT[ichannel]) isub -= GetPixelValue(i_x, i_y, ichannel);
						ichannel++;
					}
					pBIIData_dest[i_x+i_y*m_numberofpixel_perline] = isub;
				}
			}
		}
		//multiply
		else if(iModeFlag&PIXLESET_SPECTRAL_MULTIPLY)
		{
			for(int i_y=0; i_y<m_numberofline_perchannel; i_y++)
			{
				for(int i_x=0; i_x<m_numberofpixel_perline; i_x++)
				{
					int iproduct = 0;
					int ichannel=0; 
					while(ichannel<nChannelINT) 
					{
						if(pChannelINT[ichannel]) iproduct *= GetPixelValue(i_x, i_y, ichannel);
						ichannel++;
					}
					pBIIData_dest[i_x+i_y*m_numberofpixel_perline] = iproduct;
				}
			}
		}
		//pick first
		else if(iModeFlag&PIXLESET_SPECTRAL_PICKFIRST)
		{
			int ichannel=0; while(ichannel<nChannelINT && pChannelINT[ichannel]==0) ichannel++;
			if(ichannel<nChannelINT && pChannelINT[ichannel])
			{
				for(int i_y=0; i_y<m_numberofline_perchannel; i_y++)
				{
					for(int i_x=0; i_x<m_numberofpixel_perline; i_x++)
					{
						int ifirst = GetPixelValue(i_x, i_y, ichannel);
						pBIIData_dest[i_x+i_y*m_numberofpixel_perline] = ifirst;
					}
				}
			}
			else
			{
				ASSERT(FALSE);
			}
		}
		else if(iModeFlag&PIXLESET_SPECTRAL_PICKLAST)
		{
			int ichannel=nChannelINT-1; while(ichannel>=0 && pChannelINT[ichannel]==0) ichannel--;
			if(ichannel<nChannelINT && pChannelINT[ichannel])
			{
				for(int i_y=0; i_y<m_numberofline_perchannel; i_y++)
				{
					for(int i_x=0; i_x<m_numberofpixel_perline; i_x++)
					{
						int ilast = GetPixelValue(i_x, i_y, ichannel);
						pBIIData_dest[i_x+i_y*m_numberofline_perchannel] = ilast;
					}
				}
			}
			else
			{
				ASSERT(FALSE);
			}
		}
		else if(iModeFlag&PIXLESET_SPECTRAL_STRONGEST)
		{
			//based on histo integral
			ASSERT(FALSE);
		}
		else if(iModeFlag&PIXLESET_SPECTRAL_WEAKEST)
		{
			//based on histo integral
			ASSERT(FALSE);
		}

		//free unused image buffer channels
		if(iModeFlag&PIXLESET_BUFFER_INPLACE)
		{
			//must kick out other bands
			//can we realloc (reducing size while maintaining first band untouched)
			pBIIData_dest = (char*) realloc(pBIIData_dest, m_channelsize_inbyte);
			m_numberofchannel = 1;
		}

		if(pPixelset->IsIFFPresent())
		{
			pPixelset->UpdateIFFFromBII();
		}

	}
	else 
	{
		ASSERT(FALSE);
	}

	if(pBIIData_src) delete[] pBIIData_src;
	return pPixelset;
}

//2012august16, poirier, begin
HGLOBAL COWPixelset::GetDIBCopy()
{ 
	if(m_pDIB==NULL)
	{
		ASSERT(FALSE);
		return INVALID_HANDLE_VALUE;
	}
	int nPALsize = GetDIBPaletteSize(m_pDIB);
	int nDATAsize = GetDIBDataSize(m_pDIB);
	int nDIBsize = sizeof(BITMAPINFOHEADER) + nPALsize + nDATAsize;
	
	HGLOBAL hDIB = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, nDIBsize);
	if(hDIB!=INVALID_HANDLE_VALUE)
	{
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
		memcpy(lpDIB, m_pDIB, nDIBsize);
	}
	return hDIB;
}
//2012august16, poirier, end








