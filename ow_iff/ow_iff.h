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

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the OW_IFF_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// OW_IFF_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef OW_IFF_EXPORTS
#define OW_IFF_API __declspec(dllexport)
#else
#define OW_IFF_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////////
//FROM IMGLIB.H, ACCUSOFT IMAGE FORMAT LIBRARY
//
//BEGIN
//////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif

#ifndef ACCUAPI
#define	ACCUAPI FAR PASCAL
#endif

#define	IFL_MAJOR		5
#define IFL_MINOR       10
#define IFL_UPDATE      0


////////////////////
////////////////////
////////////////////
//CALLBACK FUNCTIONS									

typedef int (FAR PASCAL *STATUSBARCB)(
	void FAR *private_data,
	int		lines
);

typedef int (*READCB)(
	int 				fh,
	char FAR 		*buffer,
	unsigned int	size
);

typedef int (*WRITECB)(
	int 		fh,
	char FAR *buffer,
	unsigned int	size
);

typedef long (*SEEKCB)(
	int				fh,
	long				offset,
	unsigned int	seek_type
);

typedef int (*PUTDIBDATACB)(
	char FAR	*src_ptr,
	void FAR *private_data,
	int		ypos,
	int		rast_size
);

typedef int (*GETDIBDATACB)(
	char FAR *private_data,
	char FAR *dst_ptr,
	int 		ypos,
	int 		rast_size
);
	
typedef int (*PUTHEADERCB)(
	LPBITMAPINFOHEADER	lpbih,
	void FAR* private_data
);

//////////////////////
//////////////////////
//////////////////////
//HIGH LEVEL FUNCTIONS									

int ACCUAPI IMG_decompress_bitmap(
	char FAR	*filename
);

int ACCUAPI IMG_decompress_bitmap_fd(
	int				fd,
	unsigned long	offset,
	int				page
);
	
int ACCUAPI IMG_decompress_bitmap_mem(
	char FAR *image_data,
	int		page
);

int ACCUAPI IMG_decompress_fax(
	int	fd,
	int	xsize,
	int	ysize,
	int	type,
	int	fill_order
);

int ACCUAPI	IMG_set_statusbar(
	void FAR		*private_data,
	STATUSBARCB	statusbar,
	int			on_off_flag
);

int ACCUAPI	IMG_scan_open_source(
	HWND	hwnd
);
	
int ACCUAPI	IMG_scan_acquire(
	HWND	hwnd,
	int	showui
);

int ACCUAPI	IMG_scan_pages(
	HWND		hwnd,
	char FAR	*filename,
	int		filetype,
	int		show_ui
);

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//STRUCTURE FOR SETTING SCANNING PARAMETERS WITH SHOWUI OFF   

	typedef struct _scan_caps
	{
		int		bitspix; 		/* bits per pixel.					*/
		double	left; 			/* window x start pos in inches	*/
		double	top;				/* window y start pos in inches.	*/
		double	right;			/* window x size	in inches		*/
		double	bottom;			/* window y size	in inches		*/
		int		hres; 			/* x resolution in pixels.			*/
		int		vres; 			/* y resolution in pixels.			*/
	} SCAN_CAPS;

int ACCUAPI	IMG_scan_set_caps(
	SCAN_CAPS FAR *new_caps
);


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//SAVE AND DISPLAY BITMAP   


int ACCUAPI IMG_save_bitmap
(
	int		imghandle,
	char FAR *filename,
	int		type
);

int ACCUAPI IMG_save_bitmap_fd(
	int	imghandle,
	int	fd,
	int	type
);

long ACCUAPI IMG_save_bitmap_mem(
	int		imghandle,
	char FAR	*memptr,
	int		type
);

int ACCUAPI IMG_display_bitmap(
	int	imghandle,
	HDC	hdc,
	int	xpos,
	int	ypos,
	int	width,
	int	height
);

int ACCUAPI IMG_display_bitmap_transp(
	int	imghandle,
	HDC	hdc,
	int	xpos,
	int	ypos,
	int	width,
	int	height,
	long	color
);

HBITMAP ACCUAPI IMG_dib_to_ddb(
	int	imghandle,
	int	width,
	int	height
);

int ACCUAPI IMG_display_ddb(
	HBITMAP	hbitmap,
	HDC		hdc,
	int		xpos,
	int		ypos
);

//////////////////////////////////
//////////////////////////////////
//////////////////////////////////
//IMG DISPLAY DDB EFFECT CONSTANTS       

#define	IDE_TOPWIPE		0x01
#define	IDE_BOTTOMWIPE	0x02
#define	IDE_LEFTWIPE	0x03
#define	IDE_RIGHTWIPE	0x04
#define	IDE_BLINDS		0x05
#define	IDE_BLOCKS		0x06
#define	IDE_CENTRE		0x07
#define	IDE_LOUVRES		0x08
#define	IDE_SQUAREOUT	0x09
#define	IDE_ZIGZAG		0x0A

int ACCUAPI IMG_display_ddb_effect(
	HBITMAP	hbitmap,
	HDC		hdc,
	int		xpos,
	int		ypos,
	int		effect
);

HPALETTE ACCUAPI IMG_get_bitmap_palette(
	int	imghandle
);

int ACCUAPI	IMG_repaint_scroll
(
	int				 imghandle,
	HDC				 hdc,
	HWND				 hwnd,
	int				 scroll_dir,
	PAINTSTRUCT	FAR *ps
);

int ACCUAPI	IMG_scroll_bitmap(
	int		imghandle,
	HWND		hwnd,
	WPARAM	wparam,
	LPARAM	lparam,
	UINT		message
);

long ACCUAPI IMG_zoom_bitmap_1_to_1(
	int	imghandle,
	HWND	hwnd,
	int	zoom_level,
	int	center_flag
);

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//VALID DEFINES FOR THE MODE FIELD OF IMG_zoom_bitmap_rect()	

#define SCREEN_COORDS	0
#define IMAGE_COORDS		1

long ACCUAPI IMG_zoom_bitmap_rect(
	int		imghandle,
	HWND		hwnd,
	LPRECT	lpzoom,
	LPRECT	lpclient,
	int		mode
);

int ACCUAPI	IMG_zoom_bitmap(
	int	imghandle,
	HWND	hwnd,
	int	zoom_factor
);

int ACCUAPI IMG_print_bitmap(
	int	imghandle,
	HDC	printerDC,
	int	xpos,
	int	ypos,
	int	width,
	int	height
);

int ACCUAPI IMG_print_bitmap_fast(
	int	imghandle,
	HDC	printerDC,
	int	xpos,
	int	ypos,
	int	width,
	int	height
);

int ACCUAPI IMG_delete_bitmap(
	int	imghandle
);

int ACCUAPI IMG_delete_bitmap_keep(
	int	imghandle
);

HGLOBAL ACCUAPI IMG_bitmap_info(
	int		imghandle,
	int FAR *width,
	int FAR *height,
	int FAR *bits_per_pixel
);

int ACCUAPI IMGLOW_get_fileinfo(
	char FAR 				*filename,
	LPBITMAPINFOHEADER	lpbih
);

long ACCUAPI IMGLOW_get_transp_color(

	int	imghandle
);

int ACCUAPI IMG_save_clipboard(
	char FAR *filename,
	HWND		hwnd
);

int ACCUAPI IMG_get_croprect(
	int		imghandle,
	int FAR *xpos,
	int FAR *ypos,
	int FAR *width,
	int FAR *height
);

int ACCUAPI IMG_set_croprect(
	int	imghandle,
	int	xpos,
	int	ypos,
	int	width,
	int	height
);

int ACCUAPI IMG_bitmap_palette(
	int	imghandle,
	HDC	hdc
);

int ACCUAPI IMG_create_handle(
	LPBITMAPINFOHEADER	lpbih
);

int ACCUAPI IMG_create_handle_keep(
	LPBITMAPINFOHEADER	lpbih
);

int ACCUAPI IMG_merge_bitmap(
	int		imghandle,
	char FAR *filename,
	long		raster_operation,
	int		xpos,
	int		ypos);

int ACCUAPI IMG_create_handle_ddb(
	HBITMAP	hbitmap,
	HPALETTE	hpalette
);
	
int ACCUAPI IMG_promote_8(
	int	imghandle
);

int ACCUAPI IMG_promote_24(
	int	imghandle
);


int ACCUAPI IMGLOW_set_rop(
	int	imghandle,
	long	ternary_raster_op
);

int ACCUAPI IMGLOW_set_wipedelay(

	int	wipe_delay
);

int ACCUAPI IMGLOW_set_fileio(

	READCB	newread_func,
	WRITECB  newwrite_func,
	SEEKCB	newseek_func
);

int ACCUAPI IMG_decompress_bitmap_display(
	char FAR *filename,
	HDC		hdc,
	int		xpos,
	int		ypos,
	int		width,
	int		height,
	int		aspect
);

int ACCUAPI IMG_ifl_version(
	int FAR *major,
	int FAR *minor
);

/////////////////
/////////////////
/////////////////
//ASCII FUNCTIONS		

	typedef struct tagASCIITEXTATTR
	{
		LONG	asciiFlags;						/* determines which fields to use  */
		WORD	asciiXDpi;						/* horizontal dots per inch 		  */
		WORD	asciiYDpi;						/* vertical dots per inch   		  */
		RECT	asciiMargin;					/* margins (1/1000 inches)			  */
		WORD	asciiTabStop;					/* # of chars between tab stops    */
		LONG	asciiPageWidth;				/* width of page (1/1000 inches)	  */
		LONG	asciiPageHeight;				/* height of page (1/1000 inches)  */
		int	asciiPointSize;				/* point size of the font			  */
		WORD	asciiCharsPerLine;			/* number of characters per line	  */
		WORD	asciiLinesPerPage;			/* number of lines per page		  */
		BOOL	asciiWeight;					/* normal = 0, bold = 1			 	  */
		BOOL	asciiItalic;					/* normal = 0, italic = 1			  */
		char	asciiTypeFace[32];			/* name of the font to use			  */
	} ASCIITEXTATTR, FAR *LPASCIITEXTATTR;


/////////////////////
/////////////////////
/////////////////////
//ASCIITEXTATTR FLAGS 		

#define	ASCIIXDPI			(1L<<0)
#define	ASCIIYDPI			(1L<<1)
#define	ASCIIMARGIN			(1L<<2)
#define	ASCIITABSTOP		(1L<<3)
#define	ASCIIPAGEWIDTH		(1L<<4)
#define	ASCIIPAGEHEIGHT	(1L<<5)
#define	ASCIIPOINTSIZE		(1L<<6)
#define	ASCIICHARSPERLINE	(1L<<7)
#define	ASCIILINESPERPAGE	(1L<<8)
#define	ASCIIWEIGHT			(1L<<9)
#define	ASCIIITALIC			(1L<<10)
#define	ASCIITYPEFACE		(1L<<11)

/////////////////////
/////////////////////
/////////////////////
//STANDARD PAGE SIZES 											

#define	ASCIIXSIZELETTER		8500L			/* 8 1/2"  */
#define	ASCIIYSIZELETTER		11000L		/* 11"	  */

#define	ASCIIXSIZELEGAL		8500L			/* 8 1/2"  */
#define	ASCIIYSIZELEGAL		14000L		/* 14"	  */

#define	ASCIIXSIZEEXECUTIVE	7250L			/* 7 1/4"  */
#define	ASCIIYSIZEEXECUTIVE	10500L		/* 10 1/2" */

#define	ASCIIXSIZEENVELOPE	4125L			/* 4 1/8"  */
#define	ASCIIYSIZEENVELOPE	9500L			/* 9 1/2"  */

int ACCUAPI IMG_import_ascii(
	char FAR *filename,
	int		page
);

int ACCUAPI IMGLOW_set_ascii_attributes(
	LPASCIITEXTATTR lpattributes
);

int ACCUAPI IMGLOW_get_ascii_attributes(
	LPASCIITEXTATTR lpattributes
);

long ACCUAPI IMGLOW_get_ascii_page_width(
	char FAR *filename
);


/////////////////////
/////////////////////
/////////////////////
//LOW LEVEL FUNCTIONS 						

int ACCUAPI IMGLOW_decompress_bitmap(
	int				fd,
	unsigned long	offset,
	int				page,
	PUTDIBDATACB	put_dib_data,
	void FAR			*private_data,
	PUTHEADERCB		put_header
);

int ACCUAPI IMGLOW_decompress_bitmap_mem(
	char FAR 		*imageptr,
	int				page,
	PUTDIBDATACB	put_dib_data,
	void FAR			*private_data,
	PUTHEADERCB		put_header
);

int ACCUAPI IMGLOW_save_bitmap(
	int						fd,
	LPBITMAPINFOHEADER	lpbih,
	int						type,
	GETDIBDATACB			get_dib_data,
	void FAR					*private_data
);

long ACCUAPI IMGLOW_save_bitmap_mem(
	char FAR 				*imageptr,
	LPBITMAPINFOHEADER	lpbih,
	int						type,
   GETDIBDATACB			get_dib_data,
   void FAR					*private_data
);

int ACCUAPI IMGLOW_get_filetype(
	char far *filename
);

int ACCUAPI IMGLOW_get_filetype_fd(
	int	fh
);

int ACCUAPI IMGLOW_get_filetype_mem(
	char FAR *ptr16
);

int ACCUAPI IMGLOW_get_bitmap_name(
	char FAR *nameptr,
	char FAR *dataptr
);

int ACCUAPI IMGLOW_set_bitmap_name(
	char FAR *nameptr,
	char FAR *dateptr
);

int ACCUAPI IMGLOW_get_bitmap_header(
	int						imghandle,
	LPBITMAPINFOHEADER	header
);

int ACCUAPI IMGLOW_set_comp_quality(
	int	quality
);

int ACCUAPI IMGLOW_get_pages(
	char FAR *filename
);

int ACCUAPI IMGLOW_get_image_orientation(
	char FAR *filename
);

int ACCUAPI IMGLOW_autocolor(
	int	on_off
);

//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////
//SCREEN DISPLAY DITHER MODE CONSTANTS	

#define	DITHER_NONE			0
#define	DITHER_BAYER		1
#define	DITHER_DIFFUSION	2

int ACCUAPI IMGLOW_set_dithermode(
	int	dither_mode
);

int ACCUAPI IMGLOW_set_decompsize(
	int	width,
	int	height
);

int ACCUAPI IMGLOW_get_palette(
	int			imghandle,
	RGBQUAD FAR *prgb
);

int ACCUAPI IMGLOW_put_palette(
	int			imghandle,
	RGBQUAD FAR *prgb
);

int ACCUAPI IMGLOW_set_decomp_reduction(
	int	bits_per_pix
);

int ACCUAPI IMGLOW_set_auto_detect(
	int	type
);

int ACCUAPI IMGLOW_unset_auto_detect(
	int	type
);

int ACCUAPI IMGLOW_get_auto_detect(
	int	type
);

////////////////////////////
////////////////////////////
////////////////////////////
//IMAGE PROCESSING FUNCTIONS

int ACCUAPI IMG_rotate_bitmap(
	int	imghandle,
	int	angle
);

int ACCUAPI IMG_flip_bitmapx(
	int	imghandle
);

int ACCUAPI IMG_flip_bitmapy(
	int	imghandle
);

int ACCUAPI IMG_invert_bitmap(
	int	imghandle
);

int ACCUAPI IMG_resize_bitmap(
	int	imghandle,
	int	width,
	int	height
);

int ACCUAPI IMG_sharpen_bitmap(
	int	imghandle,
	int	sharpness
);

int ACCUAPI IMG_resize_bitmap_interp(
	int	imghandle,
	int	width,
	int	height
);

int ACCUAPI IMG_set_lut(
	int	imghandle,
	int	contrast,
	int	brightness
);

int ACCUAPI IMG_set_gamma(
	int	gamma,
	int	on_off
);

int ACCUAPI IMG_process_bitmap(
	int		imghandle,
	int		type,
	char FAR *matrix
);

////////////////////////////////
////////////////////////////////
////////////////////////////////
//BIT DEPTH CONVERSION FUNCTIONS 

int ACCUAPI IMG_bayer_mono(
	int	imghandle
);

int ACCUAPI IMG_halftone_mono(
	int	imghandle
);

int ACCUAPI IMG_diffusion_mono(
	int	imghandle
);

int ACCUAPI IMG_bayer_color(
	int	imghandle
);

int ACCUAPI IMG_diffusion_color(
	int	imghandle
);

int ACCUAPI IMG_popularity_color(
	int	imghandle
);

int ACCUAPI IMG_color_gray(
	int	imghandle
);

int ACCUAPI IMG_mediancut_color(
	int	imghandle
);

int ACCUAPI IMG_octree_color(
	int			imghandle,
	int			bits_per_pixel,
	RGBQUAD FAR *input_prgb,
	int			entries
);

/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////
//COMPRESSION AND FILE TYPE CONSTANTS

#define	TIFF_UNCOMPRESSED	0
#define	BMP_UNCOMPRESSED	1
#define	PCX					2
#define	TARGA					3
#define	GIF					4
#define	WPG					5
#define	WMF					6
#define	TIFF_HUFFMAN 		7
#define	TIFF_G3_FAX			8
#define	TIFF_LZW				9
#define	TIFF_G4_FAX			10
#define	DCX					11
#define	BMP_COMPRESSED		12
#define	JPEG		         13
#define	EPS		         14
#define	PICT		         15
#define	TIFF_PACK         16
#define	TIFF_2D	         17
#define	CALS	         	18
#define	LASER_DATA	   	19
#define	XBM	         	20
#define	MACPAINT	         21
#define	GX2	         	22
#define	KOFAX	         	23
#define	IOCA	         	24
#define	ICON	         	25
#define	IFF_ILBM	        	26
#define	CLIP	        		27
#define	IMG	        		28
#define	BROOK_TROUT   		29
#define	MSP   				30
#define	CUT   				31
#define	TARGA16 				32
#define	CCITT_G3				33
#define	CCITT_G4				34
#define	XPM					35
#define	XWD					36
#define	RAST					37
#define	ASCII					38
#define	PHOTOCD 				39
#define	TIFF_JPEG			40
#define	PHOTOSHOP			41
#define	IMNET					42

#define	MAXFILTERS			43

#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////////////////////////////////
//FROM IMGLIB.H, ACCUSOFT IMAGE FORMAT LIBRARY
//
//END
//////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////
//FROM IMG_ERR.H, ACCUSOFT IMAGE FORMAT LIBRARY
//
//BEGIN
//////////////////////////////////////////////////////////////////////////////

//define standard error returns for bitmap dll 

#define  OUT_OF_MEMORY		-1		/* failed on memory allocation */
#define  FILE_NOT_FOUND    -2		/* open call failed */
#define  CORRUPTED_FILE    -3		/* file format bad, or unreadable */ 
#define  BAD_STRING        -4
#define  BAD_RETURN        -5
#define  CANT_CREATE_FILE  -6		/* fail on saving when creating a new file */
#define  FORMAT_NOT_ALLOWED -7	/* bad hres,vres or bits per pixel */ 
#define  NO_BITMAP_FOUND   -8		/* Getobject() call failed to return bitmap header */
#define  DISK_FULL    		-9		/* error writing data to the disk */
#define  BAD_DISPLAY_AREA  -10	/* tried to display with negative coordinates or out of range */
#define  PAGE_NOT_FOUND    -11	/* used for multipage file format support */
#define  DISK_READ_ERROR   -12	/* file format was truncated and tried to read past end of file */
#define  BAD_HANDLE    		-13	/* application passed bad Accusoft image handle */
#define  NO_CLIPBOARD_IMAGE -14	/* image not found on clipboard */
#define  NO_SCANNER_FOUND   -15	/* scanner driver not installed */
#define  ERROR_OPENING_SCANNER -16 /* bad scanner driver or driver not configured properly */
#define  CANT_FIND_TWAIN_DLL -17	/* twain.dll file not found */
#define  USER_CANCEL 		 -18	/* used for canceling out of low level save or low level decompress */
#define  EVAL_TIMEOUT       -19	/* the data on an eval copy of Accusoft product has expired */
#define  USING_RUNTIME      -20	/* vbx runtime version not allowed for design mode */
#define  PIXEL_DEPTH_UNSUPPORTED -21 /* tried to save an image to format not supported bits pixel */
#define  PALETTE_IMAGES_NOT_ALLOWED -22 /* operation will not work on palette images */
#define  PALETTE_IMAGES_NOT_ALLOWED -22 /* operation will not work on palette images */
#define  NO_LZW_VERSION					-23 /* no lzw code or gif in this version */
#define  DLL_NOT_LOADED					-24 /* dll not loaded */
#define  FORMAT_WILL_NOT_OTFLY		-25 /* format will not support on the fly */
#define  NO_TCOLOR_FOUND				-26 /* transparancy found */



//////////////////////////////////////////////////////////////////////////////
//FROM IMG_ERR.H, ACCUSOFT IMAGE FORMAT LIBRARY
//
//END
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
//FROM OW_IFF.H, OFFWORLD IMAGE FILE FORMAT LIBRARY
//
//BEGIN
//////////////////////////////////////////////////////////////////////////////

// This class is exported from the ow_iff.dll
class OW_IFF_API COW_iff 
{
public:
	COW_iff(void);
	// TODO: add your methods here.
};

extern OW_IFF_API int nOW_iff;

OW_IFF_API int fnOW_iff(void);



OW_IFF_API int ACCUAPI OW_IFF_IMGLOW_get_filetype(char far *filename);

OW_IFF_API int ACCUAPI OW_IFF_IMGLOW_get_ascii_attributes(LPASCIITEXTATTR lpattributes);

OW_IFF_API int ACCUAPI OW_IFF_IMGLOW_set_ascii_attributes(LPASCIITEXTATTR lpattributes);

OW_IFF_API int ACCUAPI OW_IFF_IMGLOW_get_pages(char FAR *filename);


OW_IFF_API HGLOBAL ACCUAPI OW_IFF_IMG_bitmap_info(	int imghandle,
													int FAR *width,
													int FAR *height,
													int FAR *bits_per_pixel);

OW_IFF_API int ACCUAPI OW_IFF_IMGLOW_get_bitmap_header(	int imghandle, 
														LPBITMAPINFOHEADER pBIH);


OW_IFF_API HBITMAP ACCUAPI OW_IFF_IMG_dib_to_ddb(	int	imghandle,
													int	width,
													int	height);

OW_IFF_API int ACCUAPI OW_IFF_IMG_display_ddb(	HBITMAP	hbitmap,
												HDC	hdc,
												int	xpos,
												int	ypos);

OW_IFF_API int ACCUAPI OW_IFF_IMG_decompress_bitmap(char FAR* filename);

OW_IFF_API int ACCUAPI OW_IFF_IMG_get_croprect(	int	imghandle,
												int FAR *xpos,
												int FAR *ypos,
												int FAR *width,
												int FAR *height);

OW_IFF_API int ACCUAPI OW_IFF_IMG_set_croprect(	int	imghandle,
												int	xpos,
												int	ypos,
												int	width,
												int	height);

OW_IFF_API int ACCUAPI OW_IFF_IMG_import_ascii(	char FAR *filename,
												int	page);

OW_IFF_API int ACCUAPI OW_IFF_IMG_create_handle(LPBITMAPINFOHEADER	lpbih);

OW_IFF_API int ACCUAPI OW_IFF_IMG_delete_bitmap(int imghandle);

OW_IFF_API int ACCUAPI OW_IFF_IMG_promote_8(int imghandle);

OW_IFF_API int ACCUAPI OW_IFF_IMG_promote_24(int imghandle);

OW_IFF_API int ACCUAPI OW_IFF_IMG_save_bitmap(	int	imghandle,
												char FAR *filename,
												int	type);

//////////////////////////////////////////////////////////////////////////////
//FROM OW_IFF.H, OFFWORLD IMAGE FILE FORMAT LIBRARY
//
//END
//////////////////////////////////////////////////////////////////////////////

