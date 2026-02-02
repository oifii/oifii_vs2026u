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

#ifndef _PIXELSET_H
#define _PIXELSET_H


//poirier, sept 2001, begin
//these 2 includes to be able to derive COWPixelset from COWSet
#include "c_pointset.h"
#include "owpointset.h"
//poirier, sept 2001, end

//poirier, sept 2001, begin
//this include to convert image buffers from and to intel image processing library
#include "ow_ipl.h"
//poirier, sept 2001, end



#define PIXELSET_DIB_PALETTE_MONORED		7
#define PIXELSET_DIB_PALETTE_4COLORTONES	6
#define PIXELSET_DIB_PALETTE_16COLORTONES	5
#define PIXELSET_DIB_PALETTE_256COLORTONES	4

#define PIXELSET_DIB_PALETTE_MONOCHROME		3
#define PIXELSET_DIB_PALETTE_4GRAYTONES		2
#define PIXELSET_DIB_PALETTE_16GRAYTONES	1
#define PIXELSET_DIB_PALETTE_256GRAYTONES	0

//2012august15, poirier, begin
#define PIXELSET_FORCE_4BYTES_ALIGNMENT		1
//#define PIXELSET_FORCE_4BYTES_ALIGNMENT	0
//2012august15, poirier, end

#define PIXELSET_USE_APP_TEMPINFO		0
#define PIXELSET_IGNORE_APP_TEMPINFO	1

#define PIXELSET_CHANNEL_MAXNUMBER		10

#define PIXELSET_HISTO_MAXLEVEL			256


#define PIXELSET_FILTER_ERROR		   					1
#define PIXELSET_FILTER_NULL_POINTER_ERROR	   			2
#define PIXELSET_FILTERING_ERROR		   				3
#define PIXELSET_FILTER_PARAM_VALIDATION_ERROR  		4

#define PIXELSET_FILTER_CANNOT_FILTER_4BIT_PACKED_DATA 	11
#define PIXELSET_FILTER_SPATIAL_WINDOW_WIDTH_ERROR	  	13
#define PIXELSET_FILTER_SPECTRAL_WINDOW_WIDTH_ERROR	  	14
#define PIXELSET_FILTER_POSSIBLE_COEF_ERROR		  		15


#define PIXELSET_KERNEL_MAXDIMENSION_X			51
#define PIXELSET_KERNEL_MAXDIMENSION_Y			51
#define PIXELSET_KERNEL_MAXDIMENSION_Z			5

#define PIXELSET_ALL		-1
#define PIXELSET_BLUE		1
#define PIXELSET_GREEN		2
#define PIXELSET_BLUE_GREEN	3
#define PIXELSET_RED		4
#define PIXELSET_BLUE_RED	5
#define PIXELSET_GREEN_RED	6
#define PIXELSET_BGR		7

#define PIXELSET_CHANNEL4	8
#define PIXELSET_CHANNEL5	16
#define PIXELSET_CHANNEL6	32
#define PIXELSET_CHANNEL7	64
#define PIXELSET_CHANNEL8	128
#define PIXELSET_CHANNEL9	256


#define PIXLESET_BUFFER_INPLACE		1
#define PIXLESET_BUFFER_NEWWINDOW	2

#define PIXLESET_SPECTRAL_AVERAGE		4
#define PIXLESET_SPECTRAL_ADD			8
#define PIXLESET_SPECTRAL_SUBSTRACT		16
#define PIXLESET_SPECTRAL_MULTIPLY		32

#define PIXLESET_SPECTRAL_PICKFIRST		64
#define PIXLESET_SPECTRAL_PICKLAST		128
#define PIXLESET_SPECTRAL_STRONGEST		256
#define PIXLESET_SPECTRAL_WEAKEST		512


struct OIFIILIB_API LocalMaxParam
{
   int selection_x_start;	   /* in pixel */
   int selection_y_start;	   /* in pixel */
   int selection_x_end;	   /* in pixel */
   int selection_y_end;	   /* in pixel */
   int selection_channel_start; /* channel number */
   int selection_channel_end;	 /* channel number */

   int window_spatial_width;	 /* odd number (1,3,5, ..., 51)	*/
   int window_spectral_width;	 /* odd number (1,3,5)		*/
};

struct OIFIILIB_API FSSRFilterParam
{
   int selection_x_start;	   /* in pixel */
   int selection_y_start;	   /* in pixel */
   int selection_x_end;	   /* in pixel */
   int selection_y_end;	   /* in pixel */
   int selection_channel_start; /* channel number */
   int selection_channel_end;	 /* channel number */

   int window_spatial_width;	 /* odd number (1,3,5, ..., 51)	*/
   int window_spectral_width;	 /* odd number (1,3,5)		*/

   int i_coef[PIXELSET_KERNEL_MAXDIMENSION_X][PIXELSET_KERNEL_MAXDIMENSION_Y][PIXELSET_KERNEL_MAXDIMENSION_Z];
};

struct OIFIILIB_API TwoDFFTParam
{
   int selection_x_start;	   /* in pixel */
   int selection_y_start;	   /* in pixel */
   int selection_x_end;	   /* in pixel */
   int selection_y_end;	   /* in pixel */
   int selection_channel;	   /* channel number */
};



////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
//COWPixelset class definition
//
//
//image info can be either stored in accusoft img handle (DIB, pixel interleaf raster format 
//enabling visualization and accusoft image processing),
// 
//or 
//
//the image info can be stored using the other COWPixelset variables (using m_pData to access
//raster data ordered in band interleaf format). 
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
class OIFIILIB_API COWPixelset : public COWSet
{
protected:
	RECT m_croprect; //either set to full image size or is a subrectangle

	/////////////////////////////////////
	//OFFWORLD BII - BAND INTERLEAF IMAGE
	char* m_pData;	//pixelset image buffer
	int m_datasize_inbyte;
	int m_pixelsize_inbyte;
	int m_linesize_inbyte;
	int m_channelsize_inbyte;
	int m_numberofpixel_perline;	//column
	int m_numberofline_perchannel;	//row
	int m_numberofchannel;			//band
	//2020july16, spi, begin
	size_t m_hsvdatasize_inbyte;
	//2020july16, spi, end

	///////////////////////////////////////////////
	//OFFWORLD IPL - INTEL IMAGE PROCESSING LIBRARY
	IplImage* m_pIplImage;

	///////////////////////////////////////
	//OFFWORLD DIB - WINDOWS BITMAP SUPPORT
	BITMAPINFOHEADER* m_pDIB;

	////////////////////////////////////////////////////////
	//OFFWORLD IFF - ACCUSOFT IFL BGR PIXEL INTERLEAF FORMAT, to store gray or rgb image (from source image data file and for display)
	//OFFWORLD IPL - ACCUSOFT IMAGE PROCESSING LIBRARY
	int m_imghandle;//accusoft image buffer
	int m_ipl_image; //accusoft ipl image buffer


	//2012july21, poirier, begin
	BITMAPINFOHEADER m_tempBIH;
	//2012july21, poirier, end

public:
	COWPixelset();
	COWPixelset(class COWPixelset* pPixelset, 
				CRect* pRect=NULL, 
				int nchannel=-1, 
				int iflag=PIXELSET_USE_APP_TEMPINFO,
				int ivalue=-1);
	~COWPixelset();
	void DefaultInitialization();

	//is buffer present
	BOOL IsBIIPresent();
	BOOL IsIPLPresent();
	BOOL IsDIBPresent();
	BOOL IsIFFPresent();
	//get buffer
	char* GetBIIPointer();
	IplImage* GetIPLPointer();
	BITMAPINFOHEADER* GetDIBPointer();
	int GetIFFHandle();
	//set buffer
	BOOL SetBIIPointer(char* pData);
	BOOL SetIPLPointer(IplImage* pIplImage);
	BOOL SetDIBPointer(BITMAPINFOHEADER* pDIB); //pDIB should be allocated (pBIH + palette + data)
	BOOL SetIFFHandle(int imghandle);
	//is valid allocated dib
	BOOL IsDIBValid(BITMAPINFOHEADER* pDIB);
	//is channel present
	BOOL IsRGB();
	BOOL IsColor();
	BOOL IsGray();
	BOOL IsRedPresent();
	BOOL IsGreenPresent();
	BOOL IsBluePresent();
	BOOL IsHuePresent();
	BOOL IsSaturationPresent();
	BOOL IsIntensityPresent();
	//2020july16, spi, begin
	BOOL IsHSV();
	//2020july16, spi, end
	//sub rectangle
	RECT GetCropRect();
	BOOL SetCropRect(RECT* pRECT);

	int GetPixelsetNumberOfChannel();

	/////////////////////////////////
	//OFFWORLD BII - MEMBER FUNCTIONS

	//global histogram vector channels
	int m_pGlobalHistogram[PIXELSET_CHANNEL_MAXNUMBER][PIXELSET_HISTO_MAXLEVEL]; //256 step, 10 channel max
	int m_pIsGlobalHistoChannelDirty[PIXELSET_CHANNEL_MAXNUMBER];
	void SetGlobalHistoChannelDirty(int i_channel=PIXELSET_ALL, BOOL bIsDirty=TRUE); 
	void ClearGlobalHistoChannel(int i_channel=PIXELSET_ALL); //default i_channel=PIXELSET_ALL
	//local histogram vector channels
	int m_pLocalHistogram[PIXELSET_CHANNEL_MAXNUMBER][PIXELSET_HISTO_MAXLEVEL]; //256 step, 10 channel max
	void ClearLocalHistoChannel(int i_channel=PIXELSET_ALL); //default i_channel=PIXELSET_ALL
	//for global or local 
	int GetHistogramChannel(int i_channel, int* pUserData, POINT* pPOINT, int npoints, int nlevels);
	int GetHistogramChannel(int i_channel, int* pUserData=NULL, CRect* pRect=NULL);
    int BuildHistogramChannel(int i_channel=PIXELSET_ALL, CRect* pRect=NULL);

	int RenderPixelsetDataAsTextBuffer(CString* pszTextBuffer, int i_channel=0, RECT* pROIRect=NULL);
	int GetWidth();
	int GetHeight();
	int GetMaxValue(int i_channel=0);
	int GetMinValue(int i_channel=0);

	int CopyPixelsetTo(class COWPixelset* pOutputPixelset = NULL);

    int GetPixelValue(int i_x, int i_y,	int i_channel);
    int SetPixelToValue(int i_x, int i_y, int i_channel, int i_val);

    //int ExtractLocalMax(class CPointset* = NULL);
    int ExtractLocalMax(class COWPointset* = NULL);
    int ValidateFilterParameters( struct LocalMaxParam* = NULL);
    //int DoFSSMovingWindow(class CPointset* = NULL, struct LocalMaxParam* = NULL);
    int DoFSSMovingWindow(class COWPointset* = NULL, struct LocalMaxParam* = NULL);

    int ConvolutionFilter(class COWPixelset* = NULL);
    int ValidateFilterParameters( struct FSSRFilterParam* = NULL);
    int DoFSSRFiltering(class COWPixelset* = NULL, struct FSSRFilterParam* = NULL);
    int Do2DimensionFFT(class COWPixelset* = NULL, struct TwoDFFTParam* = NULL);

	int m_iTCDThreshold; //level
	int m_iTCDScale; //pixel
	BOOL SetTCDThreshold(int iNewThres_inlevel);
	int GetTCDThreshold();
	BOOL SetTCDScale(int iNewScale_inpixel);
	int GetTCDScale();

	/////////////////////////////////
	//OFFWORLD BII - MEMBER FUNCTIONS
	int AllocBII(int depth, int width, int height, int ivalue=-1);
	int AllocBII(BITMAPINFOHEADER* pDIB, CRect* pSelectionRect=NULL, int ivalue=-1);
	int AllocBII(int imghandle, CRect* pSelectionRect=NULL, int ivalue=-1);
	int AllocBII(IplImage* pIplImage, CRect* pSelectionRect=NULL, int ivalue=-1);
	int AllocBII(class COWPixelset* pPixelset, CRect* pSelectionRect=NULL, int ivalue=-1);
	//2020july16, spi, begin
	int ReallocBIIforHSVandUpdateBIIComputingHSV();
	//2020july16, spi, end
	int FreeBII();
	int FillBII(int ivalue=0);
	int UpdateBIIFrom(BITMAPINFOHEADER* pDIB, CRect* pSelectionRect=NULL);
	int UpdateBIIFrom(int imghandle, CRect* pSelectionRect=NULL);
	int UpdateBIIFrom(IplImage* pIplImage, CRect* pSelectionRect=NULL);
	int UpdateBIIFrom(class COWPixelset* pPixelset, CRect* pSelectionRect=NULL);
	int UpdateBIIFromBII(int ichannel=-1, char* pSrcData=NULL, CRect* pSrcDataRect=NULL, CRect* pSelectionRect=NULL);

	int CreateBIHFromBII(BITMAPINFO* pBI, CRect* pSelectionRect=NULL);
	int CreateBIHFromBII(BITMAPINFOHEADER* pBIH, CRect* pSelectionRect=NULL);


	///////////////////////////////////////////
	//OFFWORLD - INTEL IMAGE PROCESSING LIBRARY
	int AllocIPL(int depth, int width, int height, int ivalue=-1);
	int AllocIPL(BITMAPINFOHEADER* pDIB, CRect* pSelectionRect=NULL, int ivalue=-1);
	int AllocIPL(int imghandle, CRect* pSelectionRect=NULL, int ivalue=-1);
	int AllocIPL(IplImage* pIplImage, CRect* pSelectionRect=NULL, int ivalue=-1);
	int AllocIPL(class COWPixelset* pPixelset, CRect* pSelectionRect=NULL, int ivalue=-1);
	IplImage* CreateIPLFrom(int depth, int width, int height, int ivalue=-1);
	IplImage* CreateIPLFrom(BITMAPINFOHEADER* pDIB, CRect* pSelectionRect=NULL, int ivalue=-1);
	IplImage* CreateIPLFrom(int imghandle, CRect* pSelectionRect=NULL, int ivalue=-1);
	IplImage* CreateIPLFrom(IplImage* pIplImage, CRect* pSelectionRect=NULL, int ivalue=-1);
	int FreeIPL();
	int FillIPL(int ivalue=0);
	int UpdateIPLFrom(BITMAPINFOHEADER* pDIB, CRect* pSelectionRect=NULL);
	int UpdateIPLFrom(int imghandle, CRect* pSelectionRect=NULL);
	int UpdateIPLFrom(IplImage* pIplImage, CRect* pSelectionRect=NULL);
	int UpdateIPLFrom(class COWPixelset* pPixelset, CRect* pSelectionRect=NULL);
	int UpdateIPLFromBII(int ichannel=-1, char* pSrcData=NULL, CRect* pSrcDataRect=NULL, CRect* pSelectionRect=NULL);
	IplImage* CreateIPLHeader(BITMAPINFOHEADER* pBIH);

	///////////////////////////////////
	//OFFWORLD - WINDOWS BITMAP SUPPORT
	int AllocDIB(int depth, int width, int height, int ivalue=-1);
	int AllocDIB(BITMAPINFOHEADER* pDIB, CRect* pSelectionRect=NULL, int ivalue=-1);
	int AllocDIB(int imghandle, CRect* pSelectionRect=NULL, int ivalue=-1);
	int AllocDIB(IplImage* pIplImage, CRect* pSelectionRect=NULL, int ivalue=-1);
	int AllocDIB(class COWPixelset* pPixelset, CRect* pSelectionRect=NULL, int ivalue=-1);
	BITMAPINFOHEADER* CreateDIBFrom(int depth, int width, int height, int ivalue=-1);
	BITMAPINFOHEADER* CreateDIBFrom(BITMAPINFOHEADER* pDIB, CRect* pSelectionRect=NULL, int ivalue=-1);
	BITMAPINFOHEADER* CreateDIBFrom(int imghandle, CRect* pSelectionRect=NULL, int ivalue=-1);
	BITMAPINFOHEADER* CreateDIBFrom(IplImage* pIplImage, CRect* pSelectionRect=NULL);
	int FreeDIB();
	int FillDIB(int ivalue=0);
	int UpdateDIBFrom(BITMAPINFOHEADER* pDIB, CRect* pSelectionRect=NULL);
	int UpdateDIBFrom(int imghandle, CRect* pSelectionRect=NULL);
	int UpdateDIBFrom(IplImage* pIplImage, CRect* pSelectionRect=NULL);
	int UpdateDIBFrom(class COWPixelset* pPixelset, CRect* pSelectionRect=NULL);
	int UpdateDIBFromBII(int ichannel=-1, char* pSrcData=NULL, CRect* pSrcDataRect=NULL, CRect* pSelectionRect=NULL);
	BITMAPINFOHEADER* GetDIBHeaderPointer();
	RGBQUAD* GetDIBPalettePointer(BITMAPINFOHEADER* pDBI=NULL);
	int GetDIBPaletteSize(BITMAPINFOHEADER* pDBI=NULL);
	char* GetDIBDataPointer(BITMAPINFOHEADER* pDBI=NULL);
	int GetDIBDataSize(BITMAPINFOHEADER* pDBI=NULL);
	HANDLE GetDIBCopy();

	int CreateBIHFrom(IplImage* pIplImage, BITMAPINFOHEADER* pBIH, CRect* pSelectionRect=NULL);
	int CreateBIHFrom(int depth, int width, int height, BITMAPINFOHEADER* pBIH, CRect* pSelectionRect=NULL);
	int CreateDIBPalette(BITMAPINFOHEADER* pBIH, RGBQUAD* pRGBQUAD, int flag=0); //0 for PIXELSET_DIB_PALETTE_256GRAY
	int AttachDataToBIH(BITMAPINFOHEADER*, char* pData, CRect* pSelectionRect=NULL);
	RGBQUAD m_pPalette_256GrayTones[256];
	RGBQUAD m_pPalette_16Tones[16];
	RGBQUAD m_pPalette_4Tones[4];
	RGBQUAD m_pPalette_Monochrome[2];
	int InitializeDefaultPalettes();
	RGBQUAD* GetDefaultPalettePointer(int biBitCount);
	int SaveDIB(const char* filename);

	/////////////////////////////////////////////////////
	//OFFWORLD - IFF - ACCUSOFT IMAGE FILE FORMAT LIBRARY
	int AllocIFF(int depth, int width, int height, int ivalue=-1);
	int AllocIFF(BITMAPINFOHEADER* pDIB, CRect* pSelectionRect=NULL, int ivalue=-1);
	int AllocIFF(int imghandle, CRect* pSelectionRect=NULL, int ivalue=-1);
	int AllocIFF(IplImage* pIplImage, CRect* pSelectionRect=NULL, int ivalue=-1);
	int AllocIFF(class COWPixelset* pPixelset, CRect* pSelectionRect=NULL, int ivalue=-1);
	int FreeIFF();
	int FillIFF(int ivalue=0);
	int UpdateIFFFrom(BITMAPINFOHEADER* pDIB, CRect* pSelectionRect=NULL);
	int UpdateIFFFrom(int imghandle, CRect* pSelectionRect=NULL);
	int UpdateIFFFrom(IplImage* pIplImage, CRect* pSelectionRect=NULL);
	int UpdateIFFFrom(class COWPixelset* pPixelset, CRect* pSelectionRect=NULL);
	int UpdateIFFFromBII(int ichannel=-1, char* pSrcData=NULL, CRect* pSrcDataRect=NULL, CRect* pSelectionRect=NULL);
	BITMAPINFOHEADER* GetIFFHeaderPointer(int imghandle=-1);


	//////////////////////////////////////////////////////
	//Spectral Transformation
	COWPixelset* Spectral_ConvertToMonochrome(int* pChannelINT, int nChannelINT, int iModeFlag=PIXLESET_BUFFER_INPLACE);
};



#endif //_PIXELSET_H
