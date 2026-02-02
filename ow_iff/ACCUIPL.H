#ifndef __ACCUAIPL_H__
#define __ACCUAIPL_H__

#ifdef __cplusplus 
extern "C" {
#endif

/****************************************************************************
;
;	HEADER:	  ACCUAIPL.H -	Master include	heade
;
;
;	Date created:			06/01/1994 DF
;
;	$Date: 1995/12/13 19:25:06 $
;	$Revision: 1.33 $
;
;			Copyright 1994-95, AccuSoft Corporation.
;
;****************************************************************************/



/********************************************************************************
  Major and Minor DLL revision numbers used to compile the DLL
  
  test these values against the function IPL_DLL_rev( LPINT, LPINT) to 
  verify that your DLL matches this header files.
 *******************************************************************************/
#define	IPL_REV_MAJOR					1 /* Major function changes and improvements  {1..9}								*/
#define	IPL_REV_MINOR					3 /* New functions and minor re-orginzations  {00, 01, 02, ... 99}			*/
#define	IPL_REV_UPDATE					2 /* Bugs fixed, speed improvements           {0='a', 1='b', ..., 25='z'}       */


/* This removes the majority of this file for compiling with OCX */
#if !defined(__MKTYPLIB__)



#if !defined(WINVER)
/* This header makes uses of many #defines and typedefs found in the Microsoft
   Window header files: WINDOWS.H.  Therefore the WINDOWS.H header must be included
   in your application source code before this file is included.  If you fail to
   include the headers in the correct order the next line will cause a compiler
   error to remind you.
*/
#error  WINDOWS.H must be included before this header is included.
#error      See TECHQ&A.TXT and your manual for more details.
#endif



/* Check for a Windows 32 bit Compiler */
#if defined(WIN32)
#define	COMPILER_WIN32

/* Check for a Windows 16 bit Compiler */
#else
#define	COMPILER_WIN16

#endif




/********************************************************************************
  Platform and compiler specific stuff
 *******************************************************************************/

#ifdef COMPILER_WIN16
#define	Fstrcpy							_fstrcpy
#define	Fstrlen							_fstrlen
#define	Fmemcpy							_fmemcpy
#define	Fmemset							_fmemset
#define	Hmemcpy							hmemcpy
#define	INLINE							__inline
#define	HUGE_PNTR						huge
#define	BMIH								BITMAPINFOHEADER
#define	LPBMIH							LPBITMAPINFOHEADER 
#endif

#ifdef COMPILER_WIN32
#define	Fstrcpy							strcpy
#define	Fstrlen							strlen
#define	Fmemcpy							memcpy
#define	Fmemset							memset
#define	Hmemcpy							memcpy
#define	INLINE							__inline
#define	HUGE_PNTR
#define	BMIH								BITMAPINFOHEADER
#define	LPBMIH							LPBITMAPINFOHEADER
#endif





/********************************************************************************
	Defines
 *******************************************************************************/


/*	AccuSoft	Application	Programming	Interface				*/
#ifndef ACCUAPI
#define	ACCUAPI							FAR PASCAL
#endif


/*	Bitmapped color components										*/
#define	COLOR_COMP_R					0x0001
#define	COLOR_COMP_G					0x0002
#define	COLOR_COMP_B					0x0004
#define	COLOR_COMP_RGB					(COLOR_COMP_R | COLOR_COMP_G | COLOR_COMP_B)


/*	BOOL function return	values									*/
#define	SUCCESS							  0		  	/*	same as AI_error_check() when	no	errors have	occured	*/
#ifdef FAILED		/* Win32 incompatibility, it uses FAILED as a MACRO */
#undef FAILED
#endif
#define	FAILED							(-1)


/*	Common defines														*/
#define	YES								TRUE
#define	NO									FALSE

#define	EOL								'\000'

#define	PI									3.141592654
#define	TWOPI								(PI *	2.0)
#define	HALFPI							(PI /	2.0)


/*	Directions															*/
#define	DIRECTION_HORIZONTAL				1
#define	DIRECTION_VERTICAL				2

#define	DIRECTION_NW						0
#define	DIRECTION_N							1
#define	DIRECTION_NE						2
#define	DIRECTION_W							3
#define	DIRECTION_E							4
#define	DIRECTION_SW						5
#define	DIRECTION_S							6
#define	DIRECTION_SE						7

#define	DIRECTION_LEFT						DIRECTION_W
#define	DIRECTION_RIGHT					DIRECTION_E


#define	DIRECTION_LEFT_RIGHT				0
#define	DIRECTION_RIGHT_LEFT				1
#define	DIRECTION_TOP_DOWN				2
#define	DIRECTION_BOTTOM_UP				3


/*	Types	of	common memory allocations							*/
#define	MEM_TYPE_EMPTY						0		/* Used internally to mark empty handle slots		*/
#define	MEM_TYPE_ROI						1		/* ROI structures												*/
#define	MEM_TYPE_POLY						2		/* polygon structures										*/
#define	MEM_TYPE_BUFF						3		/* gen purpose memery buff - mostly raster buffer	*/
#define	MEM_TYPE_HISTO						4		/* Histogram structures										*/
#define	MEM_TYPE_IMAGE_STRUCT			5		/* Image Data structure										*/
#define	MEM_TYPE_DIB						6		/* DLL allocated DIBs										*/

#define	MEM_TYPE_USER_DEFINED			50		/* Applications can use this value or greater to	*/
															/* mark private memory blocks								*/


/*	Types	of	ROI														*/
#define	ROI_SEGMENTS						0
#define	ROI_SQUARE							1
#define	ROI_SQUARE2							2
#define	ROI_CIRCLE							3
#define	ROI_RECT								4


/*	Types	of	HISTO sorting											*/
#define	HISTO_SORT_ASCENDING				0
#define	HISTO_SORT_DESCENDING			1


/*	Types	of	contrast	ramps											*/
#define	RAMP_HORIZONTAL					1
#define	RAMP_VERTICAL						2
#define	RAMP_PYRAMID						3

#define	RAMP_FORWARD						10
#define	RAMP_REVERSE						11


/*	Types	of	Arithmetic Operations								*/
#define	ARITH_ADD							1
#define	ARITH_SUB							2
#define	ARITH_MULTI							3
#define	ARITH_DIVIDE						4
#define	ARITH_AND							5
#define	ARITH_OR								6
#define	ARITH_XOR							7
#define	ARITH_ADD_SIGN_CENTERED			8		/*	0 is in center of 8-bit range (-127=0, 0=127, +127=255) */


/*	Types	of	structuring	Elements	and Convolution kernels	*/
#define	MAX_KERN_HEIGHT					16
#define	MAX_KERN_WIDTH						16

#define	BSE_CIRCLE							100
#define	BSE_RECTANGLE						101
#define	GSE_CYLINDER						200
#define	GSE_SPHERE							201
#define	GSE_BRICK							202
#define	GSE_CONE								203
#define	CONV_GAUSSIAN						300
#define	CONV_LAPLACIAN						301  /* DOG	  */
#define	CONV_SHARPEN						302
#define	CONV_AVERAGE						303


/*	8-bit contrast adjustment										*/
#define	CONTRAST_ADJ_PALETTE				0
#define	CONTRAST_ADJ_PIXELS				1


/*	Types	of	spatial resample										*/
#define	RESAMPLE_IN_CENTER				0
#define	RESAMPLE_IN_AVE					1
#define	RESAMPLE_IN_MIN					2
#define	RESAMPLE_IN_MAX					3
                                    	
#define	RESAMPLE_OUT_SQUARE				0
#define	RESAMPLE_OUT_CIRCLE				1


/*	Types	of	convolution	data output	formats					*/
#define	CONV_RESULT_RAW					0
#define	CONV_RESULT_ABS					1
#define	CONV_RESULT_8BIT_SIGNED			2
#define	CONV_RESULT_SIGN_CENTERED		3 


/*	Types	of	24 bit convolution data inputs					*/
#define	CONV_24_INTENSITY					0
#define	CONV_24_RGB							1
#define	CONV_24_R							2
#define	CONV_24_G							3
#define	CONV_24_B							4


/*	Types	of	Twisting & Rotating (except any)					*/
#define	ROTATE_0								0		/* do not change this values - ROTATE_RANDOM uses 0-3*/
#define	ROTATE_90							1
#define	ROTATE_180							2
#define	ROTATE_270							3
#define	ROTATE_RANDOM						4


/*	Types	of	noise	ditributions									*/
#define	NOISE_LINEAR						0
#define	NOISE_GAUSSIAN						1
#define	NOISE_BINARY						2


/*	Types	of	skew fills												*/
#define	SKEW_FILL_AUTO						256	/*	must be outside of legal gray-scale	range*/
#define	SKEW_FILL_NONE						257	/*	must be outside of legal gray-scale	range*/


/*	Types	of	device independant color RGB values				*/
#define	COLOR_RGB_NTSC						0
#define	COLOR_RGB_CIE						1
#define	COLOR_RGB_SMPTE					2


/*	Types	of	color	output modes									*/
#define	COLOR_RGB							0
#define	COLOR_I								1
#define	COLOR_IHS							2
#define	COLOR_HSL							3
#define	COLOR_Lab							4
#define	COLOR_YIQ							5
#define	COLOR_CMY							6
#define	COLOR_CMYK							7
#define	COLOR_YCrCb							8


/*	Types	of	LUT transform operations							*/
#define	LUT_XFORM_ADD						0
#define	LUT_XFORM_SUB						1
#define	LUT_XFORM_MULTI					2
#define	LUT_XFORM_DIVIDE					3
#define	LUT_XFORM_CEILING					4
#define	LUT_XFORM_FLOOR					5
#define	LUT_XFORM_AND						6
#define	LUT_XFORM_OR						7
#define	LUT_XFORM_XOR						8
#define	LUT_XFORM_NOT						9
#define	LUT_XFORM_ABS						10


/*	Types	of	drawing graphics mode								*/
#define	DRAW_MODE_REPLACE					0


/*	Types	of	image	blending	modes											*/
#define	BLEND_ON_HUE						0
#define	BLEND_ON_INTENSITY				1
#define	BLEND_ON_IMAGE						2


/*	Types	of	clipboard copy & paste resuts								*/

#define	CLIPBOARD_NOT_SUPPORTED			-1		/* Clipboard not supported for this platform		*/

#define	CLIPBOARD_PASTE_SUCCESS			0		/* Paste was successful									*/
#define	CLIPBOARD_PASTE_NO_DIB_AVIAL	1		/* No DIB in clipboard to baste						*/

#define	CLIPBOARD_COPY_SUCCESS			0		/* Copy was successful									*/
#define	CLIPBOARD_COPY_NO_MEMORY		1		/* Not enough memory available to copy DIB to	*/



/*	Types	of	rotation interpolation										*/
#define	ROTATE_ONE_TO_ONE					0
#define	ROTATE_BILINEAR_INTERPOLATE	1
#define	ROTATE_AVERAGE						2



/*	Types	of	tint primary													*/
#define	COLOR_PRIMARY_RED					0
#define	COLOR_PRIMARY_GREEN				1
#define	COLOR_PRIMARY_BLUE				2
#define	COLOR_PRIMARY_CYAN				3
#define	COLOR_PRIMARY_MAGENTA			4
#define	COLOR_PRIMARY_YELLOW				5


/*	Types	of	barcodes															*/
#define	UPC_TYPE_A							0
#define	UPC_TYPE_E							1
#define	CODABAR								2




/********************************************************************************
	AccuSoft Error codes
 *******************************************************************************/

#include "ipl_errs.h"




/********************************************************************************
	Macros
 *******************************************************************************/
#define	BETWEEN(l,v,h)				(v=(v<l)?l:((v>h)?h:v))				/*	force	v between l	and h				*/
#define	CEILING(v,c)				(v=(v>c)?c:v);							/*	force	v below c						*/
#define	FLOOR(v,f)					(v=(v<f)?f:v);							/*	force	v above f						*/

/*#define	CBYTE(a,b)					a=((a)&~(b))*/						/*	Clear-bits (byte,	mask)				*/
/*#define	SBYTE(a,b)					a=((a)|(b))*/						/*	Set-bits	(byte, mask)	  			*/
/*#define	MBYTE(a,b,c)				a=(((a)&~(b))|((b)&(c)))*/		/*	Modify-bits	(byte, mask, value)	*/

#define	SIGN(x)						((x)>0?1:((x)==0?0:(-1)))			/*	retrieves sign	of	x					*/


#define	DEGREES_TO_RADIANS( d )	((d*TWOPI) / 360.0)
#define	RADIANS_TO_DEGREES( r )	((r*360.0) / TWOPI)



/******************************************************************************
	structures and	typedefs
 *******************************************************************************/



typedef int				LENGTH;			/*	used for	length of arrays						*/
typedef int	FAR*		LPLENGTH;

typedef int				PIXPOS;			/*	used for	x or y in a	coordinate				*/
typedef int	FAR*		LPPIXPOS;

typedef int				DIMENSION;		/*	used for	the width or height of an image	*/
typedef int	FAR*		LPDIMENSION;

typedef BYTE			PIXEL;
typedef PIXEL FAR*	LPPIXEL;

typedef WORD			MODE;				/*	used for	mode constants							*/
typedef MODE FAR*		LPMODE;


typedef struct	tagRUNS
		{
		PIXPOS	x;
		LENGTH	x_length;
		LENGTH	num_of_runs;
		LPPIXPOS	lpLengthsArray;
		}
	RUNS;
typedef RUNS	FAR*	LPRUNS;



typedef struct	tagBARCODE_UPC
		{
		MODE		UPC_type;						/* UPC_TYPE_A, UPC_TYPE_E	*/

		int		number_system;
		int		manufacturer_code[5];
		int		product_code[5];
		int		check_digit;
		
		int		supplemental_length;			/* 2 or 5 */
		int		supplemental_code[5];
		}
	BARCODE_UPC;
typedef BARCODE_UPC	FAR*	LPBARCODE_UPC;


typedef struct	tagBARCODE_CODABAR
		{
		BOOL		transmit_ss_on_read;				/* Transmit the start and stop chars when the code is read TRUE or FALSE	*/
		BOOL		require_3_data_char;				/* Data string must contain at least 3 chars to be considered valid TRUE or FALSE-still must be at least 1	*/

		int		string_length;
		char		string[100];
		}
	BARCODE_CODABAR;
typedef BARCODE_CODABAR	FAR*	LPBARCODE_CODABAR;


typedef struct	tagBARCODE_BOOKLAND
		{
		int		country_code[3];					/* 9, 7, 8	*/
		int		ISBN[13];
		int		supplemental_code[5];			/* Country code(5=US, 6=CAN), price*/
		}
	BARCODE_BOOKLAND;
typedef BARCODE_BOOKLAND	FAR*	LPBARCODE_BOOKLAND;


typedef struct	tagBARCODE_ISSN
		{
		int		country_code[3];					/* 9, 7, 7			*/
		int		ISSN[13];
		int		supplemental_code[2];			/* Month	01-12		*/
		}
	BARCODE_ISSN;
typedef BARCODE_ISSN	FAR*	LPBARCODE_ISSN;


typedef struct	tagBARCODE_39
		{
		int		length;
		LPBYTE	lpString;
		}
	BARCODE_39;
typedef BARCODE_39	FAR*	LPBARCODE_39;

typedef struct	tagBARCODE_POSTNET
		{
		int		num_of_digits;						/* 5, 9, or 11	*/
		int		digits[11];
		long		zip;
		int		zip_plus_4;
		int		delivery_point;
		}
	BARCODE_POSTNET;
typedef BARCODE_POSTNET	FAR*	LPBARCODE_POSTNET;


typedef struct	tagBARCODE_interleaved_2_of_5
		{
		int x; /* ?? */
		}
	BARCODE_interleaved_2_of_5;
typedef BARCODE_interleaved_2_of_5	FAR*	LPBARCODE_interleaved_2_of_5;


typedef struct	tagBARCODE_128
		{
		int		length;
		LPBYTE	lpString;
		}
	BARCODE_128;
typedef BARCODE_128	FAR*	LPBARCODE_128;


typedef struct	tagBARCODE_93
		{
		int		length;
		LPBYTE	lpString;
		}
	BARCODE_93;
typedef BARCODE_93	FAR*	LPBARCODE_93;


/*typedef RGBTRIPLE	 	RGB;	*/
/*typedef LPRGBTRIPLE	LPRGB;*/
typedef struct	tagRGB
		{
		PIXEL				b;
		PIXEL				g;
		PIXEL				r;
		}
	RGB;
typedef RGB FAR*	LPRGB;


typedef struct	tagLPYCrCb
		{
		PIXEL				Y;
		PIXEL				Cr;
		PIXEL				Cb;
		}
	YCrCb;
typedef YCrCb FAR*	LPYCrCb;


typedef struct	tagLPLab
		{
		PIXEL				L;		/* 0-255						*/
		PIXEL				a;		/* 8-bit signed values	(TIFF is correct as is, PSD uses sign centered, sub 127)	*/
		PIXEL				b;		/* 8-bit signed values	(TIFF is correct as is, PSD uses sign centered, sub 127)	*/
		}
	Lab;
typedef Lab	FAR*	LPLab;


typedef struct	tagLPXYZ
		{
		double			X;
		double			Y;
		double			Z;
		}
	XYZ;
typedef XYZ	FAR*	LPXYZ;


typedef struct	tagLPYIQ
		{
		PIXEL				Y;
		PIXEL				I;
		PIXEL				Q;
		}
	YIQ;
typedef YIQ	FAR*	LPYIQ;


typedef struct	tagLPCMY
		{
		PIXEL				c;
		PIXEL				m;
		PIXEL				y;
		}
	CMY;
typedef CMY	FAR*	LPCMY;


typedef struct	tagLPCMYK
		{
		PIXEL				c;
		PIXEL				m;
		PIXEL				y;
		PIXEL				k;
		}
	CMYK;
typedef CMYK FAR*	LPCMYK;


typedef struct	tagIHS
		{
		PIXEL				i;
		PIXEL				h;
		PIXEL				s;
		}
	IHS;
typedef IHS	FAR*	LPIHS;


typedef struct	tagHSL
		{
		PIXEL				h;
		PIXEL				s;
		PIXEL				l;
		}
	HSL;
typedef HSL	FAR*	LPHSL;


typedef struct	tagHSV
		{
		PIXEL				h;
		PIXEL				s;
		PIXEL				v;
		}
	HSV;
typedef HSV	FAR*	LPHSV;


typedef struct	tagKERN
		{
		DIMENSION		width;
		DIMENSION		height;
		PIXPOS			start_x;
		PIXPOS			start_y;
		PIXPOS			end_x;
		PIXPOS			end_y;
		double			normalizer;						/*	value	that is divided into	matrix sum				*/
		MODE				result_form;					/*	CONV_RESULT_RAW, CONV_RESULT_8BIT_SIGNED,	...	*/
		int				kern[MAX_KERN_HEIGHT][MAX_KERN_WIDTH];
		}
	KERN;
typedef KERN FAR*	LPKERN;


/*typedef PIXEL 			LUT;		Not defined	because "LUT" is spelled thesame as the variable "LUT" and it	*/
/*											probably	won't	be	used much anyway - use PIXEL to define	LUTs					*/
typedef LPPIXEL			LPLUT;


typedef struct	tagVECTOR
		{
		POINT	 			base;
		POINT	 			tail;
		} 
	VECTOR;
typedef VECTOR	FAR* LPVECTOR;


typedef struct	tagDPOINT
		{
		double			x;
		double			y;
		} 
	DPOINT;
typedef DPOINT	FAR*	LPDPOINT;


typedef struct	tagLINE
		{
		POINT	 			p1;
		POINT	 			p2;
		} 
	LINE;
typedef LINE FAR*	LPLINE;


typedef struct	tagAOI
		{
		POINT	 			p1;						/*	Well formed	AOI has P1 at top-left corner			*/
		POINT	 			p2;						/*	Well formed	AOI has P2 at bottom-right	corner	*/
		} 
	AOI;
typedef AOI	FAR*	LPAOI;


typedef struct	tagSQUARE
		{
		POINT				p;
		LENGTH			length;
		} 
	SQUARE;
typedef SQUARE	FAR*	LPSQUARE;


typedef struct	tagCIRCLE
		{
		POINT				center;
		WORD				radius;
		} 
	CIRCLE;
typedef CIRCLE	FAR*	LPCIRCLE;


typedef struct	tagPOLYGON
		{
		WORD				num_of_nodes;
		WORD				max_num_of_nodes;
		POINT				node[1];
		} 
	POLYGON;
typedef POLYGON FAR*		LPPOLYGON;


typedef struct	tagSEGMENT
		{
		PIXPOS			y;							/*	raster line	number of segment							*/
		PIXPOS			x;							/*	first	x on raster											*/
		LENGTH			length;					/*	# of pixels	on	this segment							*/
		}
	SEGMENT;
typedef SEGMENT FAR*		LPSEGMENT;


typedef struct	tagSEG_LIST
		{
		WORD				num_of_segments;
		WORD				max_num_of_segments;
		LPSEGMENT		segment;
		} 
	SEG_LIST;
typedef SEG_LIST FAR*	LPSEG_LIST;


typedef struct	tagROI_STATS
		{
		DWORD				area;
		DWORD				perimeter;
		POINT				centroid;
		}
	ROI_STATS;
typedef ROI_STATS	FAR*	IP_ROI_STATS;


typedef struct	tagHISTO_STATS
		{
		WORD				start;					/*	first_pix_value_considered;	*/
		WORD				end;						/*	last_pix_value_considered		*/
		WORD				min_pix_value;
		WORD				max_pix_value;
		WORD				most_common_pix_value;
		WORD				least_common_pix_value;
		DWORD				count_of_most_common_pix_value;
		DWORD				count_of_least_common_pix_value;
		DWORD				sum;
		DWORD				sum_below_start;
		DWORD				sum_above_end;
		double			ave;
		double			var;
		double			sd;
		RECT				AOI;
		WORD				segment_incr;
		}
	HISTO_STATS;
typedef HISTO_STATS FAR*	LPHISTO_STATS;


typedef struct	tagHISTOGRAM
		{
		WORD				number_of_bins;
		DWORD				bin[256];
		RECT				AOI;
		WORD				segment_incr;
		HISTO_STATS		seg_stats;
		HISTO_STATS		total_stats;
		} 
	HISTOGRAM;
typedef HISTOGRAM	FAR*		LPHISTOGRAM;
typedef DWORD					HISTO_BIN;
typedef DWORD FAR*			LPHISTO_BINS;


/***************************************************************/
/* AI_ handles use the common memory alloc/lock/unlock/free		*/
/* these are internal DLL handles - not Windows handles			*/
/* using Windows memory mangement routine will cause system		*/
/* crashes																		*/
/***************************************************************/

typedef int						AI_HANDLE;
typedef AI_HANDLE	FAR *		LPAI_HANDLE;

typedef AI_HANDLE				HROI;			/* Handles used for ROI				*/
/*typedef HROI FAR *			LPHROI; - no longer used (replaced by IP_ROI)*/

typedef AI_HANDLE				HHISTO;		/* Handles used for Histograms	*/
typedef HHISTO	FAR *			LPHHISTO;

typedef AI_HANDLE				HLUT;			/* Handles used for LUTs			*/
typedef HLUT FAR *			LPHLUT;

typedef AI_HANDLE				HPOLY;		/* Handles used for Polygons		*/
typedef HPOLY FAR	*			LPHPOLY;

typedef AI_HANDLE				HDIB;			/* Handles used for DIBs			*/
#if !defined(BORLAND)
typedef HDIB FAR *			LPHDIB;
#endif



/********************************************************************************
	Private data structures are of type VOID for developers
 *******************************************************************************/

#ifndef USE_PRIVATE_TYPEDEFS
typedef VOID NEAR *			IP_IMAGE;
typedef VOID FAR  *			IP_ROI;
#else
#include "C:\ACCUSOFT\IPL\SOURCE\PRIVATE.H"
#endif



/********************************************************************************
	Pro-Imaging	Toolkit function prototypes
 *******************************************************************************/


/***	Pseudocolor 		********************************************************/

void ACCUAPI pc_highlight_small_grads( LPRGBQUAD, const int, const int);
void ACCUAPI pc_color_limits( LPRGBQUAD, const int, const int, const int, const LPRGB, const LPRGB);
void ACCUAPI pc_tint( LPRGBQUAD, const int, const double, const double);
void ACCUAPI pc_tint_primary( LPRGBQUAD, const int, const MODE);
void ACCUAPI pc_set_palette_linear( LPRGBQUAD, const int);


/***	Clipboard support	********************************************************/

MODE ACCUAPI clipboard_copy( const IP_IMAGE, const IP_ROI);
MODE ACCUAPI clipboard_paste( IP_IMAGE, const POINT);
BOOL ACCUAPI clipboard_paste_available( void);


/***	AOI (Area Of Interset pixel access	**************************************/
void ACCUAPI AOI_rotate( IP_IMAGE, const LPRECT, const MODE, LPVOID);
void ACCUAPI AOI_shuffle( IP_IMAGE, const LPRECT, const LPPIXPOS, const LPPIXPOS);
void ACCUAPI AOI_get_center( const IP_IMAGE, const LPRECT, LPVOID);
void ACCUAPI AOI_get_ave( const IP_IMAGE, const LPRECT, LPVOID);
void ACCUAPI AOI_get_max( const IP_IMAGE, const LPRECT, LPVOID);
void ACCUAPI AOI_get_min( const IP_IMAGE, const LPRECT, LPVOID);
void ACCUAPI AOI_get( const IP_IMAGE, const LPRECT, LPVOID, const BOOL);
void ACCUAPI AOI_get_packed( const IP_IMAGE, const LPRECT, LPVOID, const BOOL);
void ACCUAPI AOI_set( IP_IMAGE, const LPRECT, const LPVOID, const BOOL);
void ACCUAPI AOI_set_packed( IP_IMAGE, const LPRECT, const LPVOID, const BOOL);
void ACCUAPI AOI_set_k( IP_IMAGE, const LPRECT, const LPVOID);


/***	Column pixel access	******************************************************/
void ACCUAPI col_get(	const	IP_IMAGE, const LPSEGMENT, LPVOID);
void ACCUAPI col_set(	IP_IMAGE, const LPSEGMENT, const LPVOID);
void ACCUAPI col_set_k( IP_IMAGE, const LPSEGMENT, const LPVOID);

void ACCUAPI col1_get( const IP_IMAGE, const LPSEGMENT, LPPIXEL);
void ACCUAPI col1_set( IP_IMAGE, const LPSEGMENT, const LPPIXEL);
void ACCUAPI col1_set_k( IP_IMAGE, const LPSEGMENT, const PIXEL);

void ACCUAPI col4_get( const IP_IMAGE, const LPSEGMENT, LPPIXEL);
void ACCUAPI col4_set( IP_IMAGE, const LPSEGMENT, const LPPIXEL);
void ACCUAPI col4_set_k( IP_IMAGE, const LPSEGMENT, const PIXEL);

void ACCUAPI col8_get( const IP_IMAGE, const LPSEGMENT, LPPIXEL);
void ACCUAPI col8_set( IP_IMAGE, const LPSEGMENT, const LPPIXEL);
void ACCUAPI col8_set_k( IP_IMAGE, const LPSEGMENT, const PIXEL);

void ACCUAPI col24_get( const IP_IMAGE, const LPSEGMENT, LPRGB);
void ACCUAPI col24_set( IP_IMAGE, const LPSEGMENT, const LPRGB);
void ACCUAPI col24_set_k( IP_IMAGE, const LPSEGMENT, const LPRGB);
void ACCUAPI col24r_get( const	IP_IMAGE, const LPSEGMENT, LPPIXEL);
void ACCUAPI col24r_set( IP_IMAGE, const LPSEGMENT, const LPPIXEL);
void ACCUAPI col24r_set_k( IP_IMAGE, const LPSEGMENT, const PIXEL);
void ACCUAPI col24g_get( const	IP_IMAGE, const LPSEGMENT, LPPIXEL);
void ACCUAPI col24g_set( IP_IMAGE, const LPSEGMENT, const LPPIXEL);
void ACCUAPI col24g_set_k( IP_IMAGE, const LPSEGMENT, const PIXEL);
void ACCUAPI col24b_get( const	IP_IMAGE, const LPSEGMENT, LPPIXEL);
void ACCUAPI col24b_set( IP_IMAGE, const LPSEGMENT, const LPPIXEL);
void ACCUAPI col24b_set_k( IP_IMAGE, const LPSEGMENT, const PIXEL);

/***	General line pixel access	******************************************************/
WORD ACCUAPI line_get( const IP_IMAGE, const LPLINE, LPVOID);
WORD ACCUAPI line_set( IP_IMAGE, const LPLINE, const LPVOID);
WORD ACCUAPI line_set_k(	IP_IMAGE, const LPLINE, const LPVOID);

WORD ACCUAPI line1_get( const IP_IMAGE, const LPLINE, LPPIXEL);
WORD ACCUAPI line1_set( IP_IMAGE, const LPLINE, const LPPIXEL);
WORD ACCUAPI line1_set_k( IP_IMAGE, const LPLINE, const PIXEL);
WORD ACCUAPI line1_get_packed( const IP_IMAGE, const LPLINE, LPPIXEL);
WORD ACCUAPI line1_set_packed( IP_IMAGE, const LPLINE, const LPPIXEL);

WORD ACCUAPI line4_get( const IP_IMAGE, const LPLINE, LPPIXEL);
WORD ACCUAPI line4_set( IP_IMAGE, const LPLINE, const LPPIXEL);
WORD ACCUAPI line4_set_k( IP_IMAGE, const LPLINE, const PIXEL);

WORD ACCUAPI line8_get( const IP_IMAGE, const LPLINE, LPPIXEL);
WORD ACCUAPI line8_set( IP_IMAGE, const LPLINE, const LPPIXEL);
WORD ACCUAPI line8_set_k( IP_IMAGE, const LPLINE, const PIXEL);

WORD ACCUAPI line24_get( const	IP_IMAGE, const LPLINE, LPRGB);
WORD ACCUAPI line24_set( IP_IMAGE, const LPLINE, const LPRGB);
WORD ACCUAPI line24_set_k( IP_IMAGE, const LPLINE, const LPRGB);
WORD ACCUAPI line24r_get( const IP_IMAGE, const LPLINE, LPPIXEL);
WORD ACCUAPI line24r_set( IP_IMAGE, const LPLINE, const LPPIXEL);
WORD ACCUAPI line24r_set_k(	IP_IMAGE, const LPLINE, const PIXEL);
WORD ACCUAPI line24g_get( const IP_IMAGE, const LPLINE, LPPIXEL);
WORD ACCUAPI line24g_set( IP_IMAGE, const LPLINE, const LPPIXEL);
WORD ACCUAPI line24g_set_k(	IP_IMAGE, const LPLINE, const PIXEL);
WORD ACCUAPI line24b_get( const IP_IMAGE, const LPLINE, LPPIXEL);
WORD ACCUAPI line24b_set( IP_IMAGE, const LPLINE, const LPPIXEL);
WORD ACCUAPI line24b_set_k(	IP_IMAGE, const LPLINE, const PIXEL);

/***	Single pixel access	******************************************************/
void ACCUAPI pix_get(	const	IP_IMAGE, const PIXPOS, const PIXPOS, LPVOID);
void ACCUAPI pix_set(	IP_IMAGE, const PIXPOS, const PIXPOS, const LPVOID);

void ACCUAPI pix_get_from_palette( const IP_IMAGE, const PIXPOS, const PIXPOS, LPRGB);
void ACCUAPI pix_set_from_palette( IP_IMAGE, const PIXPOS, const PIXPOS, const LPRGB);

PIXEL	ACCUAPI	pix1_get( const IP_IMAGE, const PIXPOS, const PIXPOS);
void ACCUAPI	pix1_set( IP_IMAGE, const PIXPOS, const PIXPOS, const PIXEL);
PIXEL	ACCUAPI	pix1_get_packed( const IP_IMAGE, const PIXPOS, const PIXPOS);
void ACCUAPI	pix1_set_packed( IP_IMAGE, const PIXPOS, const PIXPOS, const PIXEL);
LPPIXEL ACCUAPI	pix1_raster_pointer_get( const IP_IMAGE, const PIXPOS);

PIXEL	ACCUAPI	pix4_get( const IP_IMAGE, const PIXPOS, const PIXPOS);
void ACCUAPI	pix4_set( IP_IMAGE, const PIXPOS, const PIXPOS, const PIXEL);

PIXEL	ACCUAPI	pix8_get( const IP_IMAGE, const PIXPOS, const PIXPOS);
void ACCUAPI	pix8_set( IP_IMAGE, const PIXPOS, const PIXPOS, const PIXEL);

void ACCUAPI	pix24_get( const IP_IMAGE, const PIXPOS, const PIXPOS, LPRGB);
void ACCUAPI	pix24_set( IP_IMAGE, const PIXPOS, const PIXPOS, const LPRGB);
PIXEL	ACCUAPI	pix24r_get(	const	IP_IMAGE, const PIXPOS, const PIXPOS);
void ACCUAPI	pix24r_set(	IP_IMAGE, const PIXPOS, const PIXPOS, const PIXEL);
PIXEL	ACCUAPI	pix24g_get(	const	IP_IMAGE, const PIXPOS, const PIXPOS);
void ACCUAPI	pix24g_set(	IP_IMAGE, const PIXPOS, const PIXPOS, const PIXEL);
PIXEL	ACCUAPI	pix24b_get(	const	IP_IMAGE, const PIXPOS, const PIXPOS);
void ACCUAPI	pix24b_set(	IP_IMAGE, const PIXPOS, const PIXPOS, const PIXEL);

/***	Row pixel access 	******************************************************/
void ACCUAPI row_get(	const	IP_IMAGE, const LPSEGMENT, LPVOID);
void ACCUAPI row_set(	IP_IMAGE, const LPSEGMENT, const LPVOID);
void ACCUAPI row_set_k( IP_IMAGE, const LPSEGMENT, const LPVOID);

void ACCUAPI row_get_from_palette( const IP_IMAGE, const LPSEGMENT, LPRGB);
void ACCUAPI row_set_from_palette( IP_IMAGE, const LPSEGMENT, const LPRGB);
void ACCUAPI row_set_k_from_palette(	IP_IMAGE, const LPSEGMENT, const LPRGB);

void ACCUAPI row1_get( const IP_IMAGE, const LPSEGMENT, LPPIXEL);
void ACCUAPI row1_get_packed( const IP_IMAGE, const LPSEGMENT, LPPIXEL);
void ACCUAPI row1_get_runs(	const IP_IMAGE, const LPSEGMENT,	LPRUNS);
void ACCUAPI row1_set( IP_IMAGE, const LPSEGMENT, const LPPIXEL);
void ACCUAPI row1_set_packed( IP_IMAGE, const LPSEGMENT, const LPPIXEL);
void ACCUAPI row1_set_runs(	const IP_IMAGE, const LPSEGMENT,	LPRUNS);
void ACCUAPI row1_set_k(	IP_IMAGE, const LPSEGMENT, const PIXEL);

void ACCUAPI row4_get( const IP_IMAGE, const LPSEGMENT, LPPIXEL);
void ACCUAPI row4_set( IP_IMAGE, const LPSEGMENT, const LPPIXEL);
void ACCUAPI row4_set_k(	IP_IMAGE, const LPSEGMENT, const PIXEL);

void ACCUAPI row8_get( const IP_IMAGE, const LPSEGMENT, LPPIXEL);
void ACCUAPI row8_set( IP_IMAGE, const LPSEGMENT, const LPPIXEL);
void ACCUAPI row8_set_k(	IP_IMAGE, const LPSEGMENT, const PIXEL);

void ACCUAPI row24_get( const IP_IMAGE, const LPSEGMENT, LPRGB);
void ACCUAPI row24_set( IP_IMAGE, const LPSEGMENT, const LPRGB);
void ACCUAPI row24_set_k( IP_IMAGE, const LPSEGMENT, const LPRGB);
void ACCUAPI row24r_get(	const	IP_IMAGE, const LPSEGMENT, LPPIXEL);
void ACCUAPI row24r_set(	IP_IMAGE, const LPSEGMENT, const LPPIXEL);
void ACCUAPI row24r_set_k( IP_IMAGE, const LPSEGMENT, const PIXEL);
void ACCUAPI row24g_get(	const	IP_IMAGE, const LPSEGMENT, LPPIXEL);
void ACCUAPI row24g_set(	IP_IMAGE, const LPSEGMENT, const LPPIXEL);
void ACCUAPI row24g_set_k( IP_IMAGE, const LPSEGMENT, const PIXEL);
void ACCUAPI row24b_get(	const	IP_IMAGE, const LPSEGMENT, LPPIXEL);
void ACCUAPI row24b_set(	IP_IMAGE, const LPSEGMENT, const LPPIXEL);
void ACCUAPI row24b_set_k( IP_IMAGE, const LPSEGMENT, const PIXEL);

/***	Image Arithmetic **********************************************************/
BOOL ACCUAPI arith( const IP_IMAGE, const IP_IMAGE, IP_IMAGE, const IP_ROI, const MODE);
BOOL ACCUAPI arith_tile_same_depth( const IP_IMAGE, const IP_IMAGE, IP_IMAGE, const IP_ROI, const LPPOINT, const MODE);
BOOL ACCUAPI arith_tile_24_and_8(	const	IP_IMAGE, const IP_IMAGE, IP_IMAGE,	const	IP_ROI, const LPPOINT, const MODE);

/***	Image Blending	*********************************************************/
BOOL ACCUAPI blend_with_LUTs( IP_IMAGE, const IP_ROI, const IP_IMAGE, const LPLUT, const LPLUT, const MODE, const IP_IMAGE, const WORD);
void ACCUAPI blend_set_chroma_key_LUTs(	const	double, const double, LPLUT, LPLUT,	const	WORD);

/***	Color space conversion	 ********************************************************/
BOOL ACCUAPI color_separate( const IP_IMAGE, const MODE, IP_IMAGE, IP_IMAGE, IP_IMAGE, IP_IMAGE, BOOL (ACCUAPI *)( LPVOID, int, int), const LPVOID);
BOOL ACCUAPI color_combine( IP_IMAGE, const MODE, const IP_IMAGE, const IP_IMAGE, const IP_IMAGE, const IP_IMAGE, BOOL (ACCUAPI *)( LPVOID, int, int), const LPVOID);

WORD ACCUAPI color_RGB_system_get( void);
BOOL ACCUAPI color_RGB_system_set( const WORD);
void ACCUAPI color_CMYK_BG_func_install( int (ACCUAPI *)(int));
void ACCUAPI color_CMYK_UCR_func_install( int	(ACCUAPI	*)(int));
void ACCUAPI color_RGB_to_IHS(	const	LPRGB, LPIHS);
void ACCUAPI color_RGB_to_IH( const LPRGB, LPIHS);
BYTE ACCUAPI color_RGB_to_I( const LPRGB);
void ACCUAPI color_RGB_to_HSL(	const	LPRGB, LPHSL);
void ACCUAPI color_RGB_to_YIQ(	const	LPRGB, LPYIQ);
void ACCUAPI color_RGB_to_CMY(	const	LPRGB, LPCMY);
void ACCUAPI color_RGB_to_XYZ(	const	LPRGB, LPXYZ);
void ACCUAPI color_RGB_to_Lab(	const	LPRGB, LPLab);
void ACCUAPI color_RGB_to_CMYK( const LPRGB,LPCMYK);
void ACCUAPI color_RGB_to_YCrCb( const LPRGB,	LPYCrCb);

void ACCUAPI color_IHS_to_RGB(	const	LPIHS, LPRGB);
void ACCUAPI color_HSL_to_RGB(	const	LPHSL, LPRGB);
void ACCUAPI color_YIQ_to_RGB(	const	LPYIQ, LPRGB);
void ACCUAPI color_CMY_to_RGB(	const	LPCMY, LPRGB);
void ACCUAPI color_XYZ_to_RGB(	const	LPXYZ, LPRGB);
void ACCUAPI color_Lab_to_RGB(	const	LPLab, LPRGB);
void ACCUAPI color_CMYK_to_RGB( const LPCMYK,	LPRGB);
void ACCUAPI color_YCrCb_to_RGB( const LPYCrCb, LPRGB);

/***	Contrast transforms	 *****************************************************/
BOOL ACCUAPI contrast_gain_and_level( IP_IMAGE, const IP_ROI, const double, const double, const MODE);
BOOL ACCUAPI contrast_eq_with_histo8( IP_IMAGE, const IP_ROI, const LPHISTOGRAM, const MODE);
BOOL ACCUAPI contrast_eq_with_histo24( IP_IMAGE, const IP_ROI, const LPHISTOGRAM, const LPHISTOGRAM, const LPHISTOGRAM);
BOOL ACCUAPI contrast_eq_auto(	IP_IMAGE, const IP_ROI, const WORD, const MODE);
BOOL ACCUAPI contrast_stretch_with_histo8(	IP_IMAGE, const IP_ROI, const LPHISTOGRAM, const MODE);
BOOL ACCUAPI contrast_stretch_with_histo24( IP_IMAGE, const IP_ROI, const LPHISTOGRAM, const LPHISTOGRAM, const LPHISTOGRAM);
BOOL ACCUAPI contrast_stretch_auto( IP_IMAGE, const IP_ROI, const WORD, const MODE);

/***	General convolutions		***************************************************/
BOOL ACCUAPI convolve8( IP_IMAGE, const IP_ROI, const LPKERN, const MODE);
BOOL ACCUAPI convolve24( IP_IMAGE, const IP_ROI, const LPKERN, const MODE, const MODE);


/***	BMIH Utilities	******************************************************/
WORD ACCUAPI BMIH_get_offset_to_bitmap( const LPBMIH);
WORD ACCUAPI BMIH_get_offset_to_palette( const LPBMIH);


/***	DIB Utilities	******************************************************/
void ACCUAPI		ip_image_associate( const LPBMIH, IP_IMAGE);
IP_IMAGE ACCUAPI	ip_image_alloc( void);
void ACCUAPI		ip_image_free( IP_IMAGE);

HDIB ACCUAPI	DIB_alloc( const DWORD,	const	DWORD, const WORD);
void ACCUAPI	DIB_free( const HDIB);
void ACCUAPI	DIB_unlock(	const	HDIB);
LPBMIH ACCUAPI	DIB_lock( const HDIB);

LPBMIH ACCUAPI	DIB_get_pntr_to_DIB( const IP_IMAGE);
LPPIXEL ACCUAPI		DIB_get_pntr_to_bitmap( const IP_IMAGE);
LPRGBQUAD ACCUAPI	DIB_get_pntr_to_palette( const IP_IMAGE);

DWORD	ACCUAPI				DIB_get_size_of_DIB(	const	DWORD, const DWORD, const WORD);
WORD ACCUAPI				DIB_get_size_of_header( void);
DWORD	ACCUAPI				DIB_get_size_of_bitmap( const DWORD, const DWORD, const WORD);
WORD ACCUAPI				DIB_get_size_of_palette( const WORD);

WORD ACCUAPI				DIB_get_entries_in_palette( const IP_IMAGE);
void ACCUAPI				DIB_get_rect( const IP_IMAGE, LPRECT);
DIMENSION ACCUAPI			DIB_get_width( const IP_IMAGE);
DIMENSION ACCUAPI			DIB_get_height( const IP_IMAGE);
int ACCUAPI					DIB_get_bits_per_pixel( const IP_IMAGE);
DIMENSION ACCUAPI			DIB_get_bytes_per_raster( const DWORD,	const	int);
DIMENSION ACCUAPI			DIB_get_bytes_in_raster_buff(	const	DWORD, const int);

BOOL ACCUAPI				DIB_copy( IP_IMAGE, const IP_IMAGE);
void ACCUAPI				DIB_copy_header( IP_IMAGE, const IP_IMAGE);
BOOL ACCUAPI				DIB_copy_bitmap( IP_IMAGE, const IP_IMAGE);	
BOOL ACCUAPI				DIBs_are_compatible(	const	IP_IMAGE, const IP_IMAGE);
BOOL ACCUAPI				DIBs_are_same_size( const IP_IMAGE, const IP_IMAGE);
BOOL ACCUAPI				DIB_is_sub_image(	const	IP_IMAGE, const IP_IMAGE, const BOOL);
BOOL ACCUAPI				DIB_is_gray( const IP_IMAGE);

BOOL ACCUAPI				DIB_file_read_dimension( const LPSTR, LPDIMENSION, LPDIMENSION, LPWORD, const BOOL);
BOOL ACCUAPI				DIB_file_read( const LPSTR, LPBMIH, const DIMENSION, const DIMENSION, const WORD, const BOOL);



/***	Graphics		******************************************************/
void ACCUAPI draw_pen_color_set( const LPRGB);
void ACCUAPI draw_pen_color_get( LPRGB);
void ACCUAPI draw_mode_set(	const	MODE);
MODE ACCUAPI draw_mode_get(	void);
void ACCUAPI draw_box( IP_IMAGE, const LPRECT, const BOOL);
void ACCUAPI draw_circle( IP_IMAGE, const LPCIRCLE, const BOOL);
void ACCUAPI draw_roi( IP_IMAGE, const IP_ROI, const BOOL);
void ACCUAPI draw_polygon( IP_IMAGE, const LPPOLYGON, const BOOL);
void ACCUAPI draw_line( IP_IMAGE, const LPLINE, const WORD);
void ACCUAPI draw_ramp( IP_IMAGE, const IP_ROI, const WORD, const WORD, const MODE, const MODE);
void ACCUAPI draw_frame(	IP_IMAGE, const WORD);
void ACCUAPI draw_grid( IP_IMAGE, const LPRECT, const WORD, const WORD);
void ACCUAPI draw_string( IP_IMAGE, const LPFONT_BITMAPS, const LPSTR, const LPPOINT);
void ACCUAPI draw_char( IP_IMAGE, const LPFONT_BITMAPS, const char, const LPPOINT, const LPDIMENSION);
void ACCUAPI draw_insert( IP_IMAGE, const IP_IMAGE, const IP_ROI, const LPPOINT);

/***	Error handling		******************************************************/
BOOL ACCUAPI				IPL_error_check( void);
void ACCUAPI				IPL_error_clear( void);
BOOL ACCUAPI				IPL_error_set(	LPCSTR, const int, const long, const int);
void ACCUAPI				IPL_error_get(	LPSTR, LPINT, long FAR *, LPINT, const int);
BOOL ACCUAPI				IPL_error_debug_set( const BOOL);
BOOL ACCUAPI				IPL_error_debug_get( void);
int ACCUAPI					IPL_error_max_file_name_len( void);

/***	Flips		******************************************************/
BOOL ACCUAPI flip_horizontal( IP_IMAGE, const IP_ROI);
BOOL ACCUAPI flip_vertical(	IP_IMAGE, const IP_ROI);
void ACCUAPI flip_pixel_array( LPBYTE, const LENGTH, const int);

/***	Rotations		******************************************************/
void ACCUAPI rotate_compute_area_needed( const DIMENSION, const DIMENSION, const double, LPDIMENSION, LPDIMENSION, const LPPOINT);
BOOL ACCUAPI rotate_whole_multiple_of_90( IP_IMAGE, const IP_IMAGE, BOOL (ACCUAPI *)( LPVOID, int, int), const LPVOID, const MODE);
BOOL ACCUAPI rotate_whole_any_angle( IP_IMAGE, const IP_IMAGE, BOOL (ACCUAPI *)( LPVOID, int, int), const LPVOID, const double, const MODE, const LPRGB, const LPPOINT);
BOOL ACCUAPI rotate_whole_any_angle_fast(IP_IMAGE, const IP_IMAGE, BOOL (ACCUAPI *)( LPVOID, int, int), const LPVOID, const double, const LPRGB, const LPPOINT);

/***	Image Filters	*********************************************************/
BOOL ACCUAPI filter_sharpen( IP_IMAGE, const IP_ROI, const DIMENSION, const DIMENSION, const double, const MODE);
BOOL ACCUAPI filter_edge_map_Laplacian(	IP_IMAGE, const IP_ROI, const DIMENSION, const DIMENSION, const double, const MODE, const MODE);
BOOL ACCUAPI filter_edge_map_Sobel( IP_IMAGE, const IP_ROI);
BOOL ACCUAPI filter_edge_map_Prewitt( IP_IMAGE, const IP_ROI);
BOOL ACCUAPI filter_edge_map_Roberts( IP_IMAGE, const IP_ROI);
BOOL ACCUAPI filter_smooth_Gaussian(	IP_IMAGE, const IP_ROI, const DIMENSION, const DIMENSION, const double, const MODE);
BOOL ACCUAPI filter_smooth_average_rect( IP_IMAGE, const IP_ROI, const DIMENSION, const DIMENSION, const MODE);
BOOL ACCUAPI filter_smooth_average_circle(	IP_IMAGE, const IP_ROI, const DIMENSION, const DIMENSION, const MODE);

/***	Special Effects *************************************************************/
BOOL ACCUAPI fx_blend_back_modulator( IP_IMAGE, const IP_IMAGE,	const IP_IMAGE, const IP_ROI);
BOOL ACCUAPI fx_blend_percent( IP_IMAGE, const IP_IMAGE, const IP_ROI, const double);
BOOL ACCUAPI fx_blur(	IP_IMAGE, const IP_ROI, const int);
BOOL ACCUAPI fx_chroma_key(	IP_IMAGE, const IP_ROI, const IP_IMAGE, const double,	const	double, const WORD, const WORD);
BOOL ACCUAPI fx_diffuse(	IP_IMAGE, const IP_ROI, const DIMENSION, const DIMENSION, const DIMENSION, const DIMENSION);
BOOL ACCUAPI fx_emboss( IP_IMAGE, const IP_ROI, const WORD, const double, const MODE);
BOOL ACCUAPI fx_intensity_dot( IP_IMAGE, const IP_ROI, const POINT, const DIMENSION, const DIMENSION, const PIXEL);
BOOL ACCUAPI fx_motion( IP_IMAGE, const IP_ROI, const double, const WORD, const MODE);
BOOL ACCUAPI fx_noise_add( IP_IMAGE, const IP_ROI, const WORD, const double, const int, const double,	const	WORD);
BOOL ACCUAPI fx_pixelate_dot( IP_IMAGE, const IP_ROI, const DIMENSION, const DIMENSION, const MODE, const WORD, const LPRGB);
BOOL ACCUAPI fx_pixelate_square( IP_IMAGE, const IP_ROI, const DIMENSION, const DIMENSION, const MODE);
BOOL ACCUAPI fx_posterize( IP_IMAGE, const IP_ROI, const WORD, const MODE);
BOOL ACCUAPI fx_stitch( IP_IMAGE, const IP_ROI, const WORD, const double);
BOOL ACCUAPI fx_texture_tile( IP_IMAGE, const IP_ROI, const IP_IMAGE);
BOOL ACCUAPI fx_twist( IP_IMAGE, const IP_ROI, const LENGTH, const MODE);
BOOL ACCUAPI fx_watermark( IP_IMAGE, const IP_ROI, const IP_IMAGE);

/***	Histogram support	*********************************************************/
HHISTO ACCUAPI				histo_alloc( void);
void ACCUAPI				histo_free(	const	HHISTO);
LPHISTOGRAM	ACCUAPI			histo_lock(	const	HHISTO);
void ACCUAPI				histo_unlock( const HHISTO);
void ACCUAPI				histo_clear( LPHISTOGRAM);

void ACCUAPI				histo_tabulate( const IP_IMAGE, LPHISTOGRAM, const IP_ROI, const WORD);
void ACCUAPI				histo_tabulate_rgb( const IP_IMAGE, LPHISTOGRAM, const WORD, const IP_ROI, const WORD);
void ACCUAPI				histo_tabulate_24( const IP_IMAGE, LPHISTOGRAM, LPHISTOGRAM, LPHISTOGRAM, const IP_ROI, const WORD);

void ACCUAPI				histo_sort(	LPHISTOGRAM, const MODE, const LPBYTE);
void ACCUAPI				histo_compute_stats(	LPHISTOGRAM, const WORD, const WORD);
BOOL ACCUAPI				histo_tabulate_whole( const IP_IMAGE, LPHISTOGRAM, const WORD);
BOOL ACCUAPI				histo_tabulate_aoi( const IP_IMAGE, LPHISTOGRAM, const LPRECT, const WORD);
void ACCUAPI				histo_tabulate_roi( const IP_IMAGE, LPHISTOGRAM, const IP_ROI, const WORD);
void ACCUAPI				histo_bin_copy( const LPHISTOGRAM, LPHISTO_BINS);
void ACCUAPI				histo_stats_copy(	LPHISTO_STATS,	const	LPHISTO_STATS);
void ACCUAPI				histo_accumulate(	const	LPHISTOGRAM, LPHISTOGRAM);

/***	Kernel generation	 ********************************************************/
void ACCUAPI				kern_flood(	LPKERN, const int);
int ACCUAPI					kern_max_width_get( void);
int ACCUAPI					kern_max_height_get(	void);
void ACCUAPI				kern_init( LPKERN, const DIMENSION, const DIMENSION, const PIXPOS, const PIXPOS);
double ACCUAPI				kern_compute_normalizer( const LPKERN);
void ACCUAPI 				kern_build_Gaussian(	LPKERN, const DIMENSION, const DIMENSION,	const	PIXPOS, const PIXPOS, const double);
void ACCUAPI 				kern_build_Laplacian( LPKERN,	const	DIMENSION, const DIMENSION, const PIXPOS,	const	PIXPOS, const double, const WORD);
void ACCUAPI 				kern_build_sharpen( LPKERN, const MODE, const double);
void ACCUAPI				kern_build_emboss( LPKERN,	const	WORD,	const	double);	
void ACCUAPI 				kern_build_average_rect( LPKERN,	const	DIMENSION, const DIMENSION, const PIXPOS,	const	PIXPOS);
void ACCUAPI 				kern_build_average_circle(	LPKERN, const DIMENSION, const DIMENSION,	const	PIXPOS, const PIXPOS);
void ACCUAPI 				kern_build_GSE_brick( LPKERN,	const	DIMENSION, const DIMENSION, const PIXPOS,	const	PIXPOS, const int);
void ACCUAPI 				kern_build_GSE_circle(	LPKERN, const DIMENSION, const DIMENSION,	const	PIXPOS, const PIXPOS, const int,	const	WORD);
void ACCUAPI 				kern_build_BSE_circle( LPKERN, const DIMENSION,	const	DIMENSION, const PIXPOS, const PIXPOS);
void ACCUAPI 				kern_build_BSE_rect(	LPKERN, const DIMENSION, const DIMENSION,	const	PIXPOS, const PIXPOS);
BOOL ACCUAPI				kern_build_motion( LPKERN,	const	double, const WORD);

/***	LUT set and transforms	************************************************************/
void ACCUAPI				LUT_set_sqrt( LPLUT,	const	WORD,	const	WORD,	const	double, const double, const double);
void ACCUAPI				LUT_set_quad( LPLUT,	const	WORD,	const	WORD,	const	double, const double, const double,	const	double);
void ACCUAPI				LUT_set_log( LPLUT, const WORD, const WORD, const double, const double,	const	double);
void ACCUAPI				LUT_set_gamma(	LPLUT, const WORD, const WORD, const double);
void ACCUAPI				LUT_set_invert( LPLUT, const WORD, const WORD);
void ACCUAPI 				LUT_set_abs( LPLUT, const WORD, const WORD);
void ACCUAPI				LUT_set_linear( LPLUT, const WORD, const WORD, const double, const double);
void ACCUAPI				LUT_set_constant(	LPLUT, const WORD, const WORD, const PIXEL);
void ACCUAPI				LUT_stretch( LPLUT, const WORD, const WORD, const PIXEL,	const	PIXEL);
void ACCUAPI				LUT_equalize( LPLUT,	const	LPHISTOGRAM, const DWORD);
void ACCUAPI				LUT_set_signed_table( LPLUT, const MODE, const LPVOID);

BOOL ACCUAPI				xform_LUT1(	IP_IMAGE, const IP_ROI, const LPLUT);
BOOL ACCUAPI				xform_LUT8(	IP_IMAGE, const IP_ROI, const LPLUT);
BOOL ACCUAPI				xform_LUT24( IP_IMAGE, const IP_ROI, const LPLUT, const LPLUT, const LPLUT);

BOOL ACCUAPI				xform_invert_intensity( IP_IMAGE, const IP_ROI, const MODE, const MODE);
BOOL ACCUAPI				xform_gamma_correct( IP_IMAGE, const IP_ROI, const double, const MODE, const MODE);
BOOL ACCUAPI				xform_arith_one_image( IP_IMAGE, const IP_ROI, const PIXEL, const MODE, const MODE);

/***	Memory common allocations	 ********************************************************/
AI_HANDLE ACCUAPI			memory_global_alloc( const DWORD, const MODE);
BOOL ACCUAPI				memory_global_free( AI_HANDLE);
LPVOID ACCUAPI				memory_global_lock( AI_HANDLE);
BOOL ACCUAPI				memory_global_unlock( AI_HANDLE);

VOID NEAR* ACCUAPI		memory_local_alloc( const	WORD, const MODE);
BOOL ACCUAPI				memory_local_free( VOID NEAR*);

void ACCUAPI				memory_free_all( void);
int ACCUAPI					memory_max_handles( void);
int ACCUAPI					memory_handles_in_use( void);


/***	Morphilogical Filters		******************************************************/
BOOL ACCUAPI				morph_dilate( IP_IMAGE, const IP_ROI, const DIMENSION, const DIMENSION);
BOOL ACCUAPI				morph_erode( IP_IMAGE, const IP_ROI, const DIMENSION, const DIMENSION);
BOOL ACCUAPI				morph_open(	IP_IMAGE, const IP_ROI, const DIMENSION, const DIMENSION);
BOOL ACCUAPI				morph_close( IP_IMAGE, const IP_ROI, const DIMENSION, const DIMENSION);

BOOL ACCUAPI				morph_b_fill_bridges( IP_IMAGE, const IP_ROI);
BOOL ACCUAPI				morph_b_fill_diags(	IP_IMAGE, const IP_ROI);
BOOL ACCUAPI				morph_b_fill_single_holes( IP_IMAGE, const IP_ROI);
BOOL ACCUAPI				morph_b_remove_single_pix( IP_IMAGE, const IP_ROI);
BOOL ACCUAPI				morph_b_remove_spurs( IP_IMAGE, const IP_ROI);
BOOL ACCUAPI				morph_b_remove_interioirs( IP_IMAGE, const IP_ROI);
BOOL ACCUAPI				morph_b_break_Hs( IP_IMAGE, const IP_ROI);
BOOL ACCUAPI				morph_b_fatten( IP_IMAGE, const IP_ROI);
BOOL ACCUAPI				morph_b_majority_rule(	IP_IMAGE, BOOL (ACCUAPI *)( LPVOID, int, int), const LPVOID);
BOOL ACCUAPI				morph_b_remove_rh( IP_IMAGE, const IP_ROI);
BOOL ACCUAPI				morph_b_connect_vlines( IP_IMAGE, const IP_ROI);
BOOL ACCUAPI				morph_b_connect_hlines( IP_IMAGE, const IP_ROI);


void ACCUAPI				morph_b_erode(	IP_IMAGE, const IP_ROI, const LPKERN);
void ACCUAPI				morph_b_dilate( IP_IMAGE, const IP_ROI, const LPKERN);

void ACCUAPI				morph_b_hm_xform(	IP_IMAGE, const IP_ROI, const LPBYTE, const WORD, const DIMENSION, const DIMENSION, const PIXPOS,	const	PIXPOS);
void ACCUAPI				morph_b_hm_lut_fatten( const BOOL, LPLUT);
void ACCUAPI				morph_b_hm_lut_fill_interior(	const	WORD,	const	BOOL,	LPLUT);
void ACCUAPI				morph_b_hm_lut_fill_diag( const BOOL, LPLUT);
void ACCUAPI				morph_b_hm_lut_fill_bridge( const BOOL, LPLUT);
void ACCUAPI				morph_b_hm_lut_remove_isolated( const WORD, const BOOL, LPLUT);
void ACCUAPI				morph_b_hm_lut_remove_spur( const BOOL, LPLUT);
void ACCUAPI				morph_b_hm_lut_remove_interior( const WORD, const BOOL, LPLUT);
void ACCUAPI				morph_b_hm_lut_h_break(	const	BOOL,	LPLUT);
void ACCUAPI				morph_b_hm_lut_majority_rule(	LPLUT);

/***	Median filter	*********************************************************/
BOOL ACCUAPI 				median( IP_IMAGE, const IP_ROI, const DIMENSION, const DIMENSION);
void ACCUAPI				median8(	IP_IMAGE, const IP_ROI, const DIMENSION, const DIMENSION);
void ACCUAPI				median24( IP_IMAGE, const IP_ROI, const DIMENSION, const DIMENSION, const MODE);

/***	Noise generation **********************************************************/
BOOL ACCUAPI				noise_add_1_bit( IP_IMAGE, const IP_ROI, const int);
BOOL ACCUAPI				noise_add_8_bit( IP_IMAGE, const IP_ROI, const MODE, const double, const int, const double);
BOOL ACCUAPI				noise_add_24_bit(	IP_IMAGE, const IP_ROI, const MODE, const double, const int, const double, const WORD);

/***	Palette control	******************************************************/
void ACCUAPI				palette_xlat_pix_to_rgb( const IP_IMAGE, const BYTE, LPRGB);
BYTE ACCUAPI				palette_xlat_rgb_to_pix( const IP_IMAGE, LPRGB);
void ACCUAPI				palette_xlat_pix_a_to_rgb_a( const IP_IMAGE, const LPBYTE, LPRGB, const LENGTH);
void ACCUAPI				palette_xlat_rgb_a_to_pix_a( const IP_IMAGE, LPBYTE, const LPRGB, const LENGTH);
void ACCUAPI				palette_get( const LPRGBQUAD,	const	WORD,	LPRGB, const WORD);
void ACCUAPI				palette_get_range( const LPRGBQUAD,	const	WORD,	LPRGB, const WORD, const WORD);
void ACCUAPI				palette_set( LPRGBQUAD,	const	WORD,	const	LPRGB, const WORD);
void ACCUAPI				palette_set_range( LPRGBQUAD,	const	WORD,	const	LPRGB, const WORD, const WORD);
void ACCUAPI				palette_set_range_k(	LPRGBQUAD, const WORD, const LPRGB,	const	WORD,	const	WORD);
BOOL ACCUAPI				palette_is_gray( const LPRGBQUAD, const WORD);
BOOL ACCUAPI				palette_copy( IP_IMAGE, const IP_IMAGE);

/***	Polygon support	******************************************************/
HPOLY	ACCUAPI				poly_alloc(	const	WORD);
BOOL ACCUAPI				poly_free( const HPOLY);
LPPOLYGON ACCUAPI			poly_lock( const HPOLY);
void ACCUAPI				poly_unlock( const HPOLY);
void ACCUAPI				poly_node_add(	LPPOLYGON, const LPPOINT);
void ACCUAPI				poly_node_insert(	LPPOLYGON, const WORD, const LPPOINT);
void ACCUAPI				poly_node_remove(	LPPOLYGON, const WORD);
int ACCUAPI					poly_node_find_first_index( const LPPOLYGON,	const	LPPOINT);
void ACCUAPI				poly_centroid_get( const LPPOLYGON,	LPPOINT);
void ACCUAPI				poly_copy( LPPOLYGON, const LPPOLYGON);
WORD ACCUAPI				poly_number_of_nodes( const LPPOLYGON);
void ACCUAPI				poly_move( LPPOLYGON, const LPPOINT);
void ACCUAPI				poly_rotate( LPPOLYGON,	const	LPPOINT,	const	double);
void ACCUAPI				poly_zoom( LPPOLYGON, const LPPOINT, const double);
void ACCUAPI				poly_bounding_rect_get(	const	LPPOLYGON, LPRECT);
void ACCUAPI				poly_to_ROI( const LPPOLYGON,	IP_ROI, const LPRECT);

/***	Resampling transformations *******************************************************/
BOOL ACCUAPI	 			resample_spatial(	IP_IMAGE, const IP_ROI, const PIXPOS, const PIXPOS, const MODE, const MODE, const WORD, const LPRGB);
BOOL ACCUAPI				resample_intensity( IP_IMAGE, const IP_ROI, const WORD, const MODE);
                        
/***	ROI Support	************************************************************/
HROI ACCUAPI				ROI_alloc( const MODE, const LPRECT, const WORD);
BOOL ACCUAPI				ROI_free( const HROI);
IP_ROI ACCUAPI				ROI_lock( const HROI);
void	ACCUAPI				ROI_unlock(	const	HROI);

BOOL ACCUAPI				ROI_copy( IP_ROI, const IP_ROI);
double ACCUAPI				ROI_aspect_ratio_get( const IP_ROI);
void ACCUAPI				ROI_aspect_ratio_set( IP_ROI, const double);
WORD ACCUAPI				ROI_number_of_segments(	const	IP_ROI);
LENGTH ACCUAPI				ROI_max_segment_length(	const	IP_ROI);
BOOL ACCUAPI				ROI_first_segment_get( const IP_ROI, LPSEGMENT);
BOOL ACCUAPI				ROI_next_segment_get( const IP_ROI, LPSEGMENT);
void ACCUAPI				ROI_status_bar_callback_register( IP_ROI, const LPVOID, BOOL (ACCUAPI *)(LPVOID, int, int));
BOOL ACCUAPI				ROI_status_bar_update( IP_ROI, int, int);
MODE ACCUAPI				ROI_type_get( const IP_ROI);
void ACCUAPI				ROI_move( IP_ROI, const LPPOINT);
void ACCUAPI				ROI_rotate(	IP_ROI, const double);
void ACCUAPI				ROI_intersect(	IP_ROI, const IP_ROI);
void ACCUAPI				ROI_union( IP_ROI, const IP_ROI);
void ACCUAPI				ROI_zoom( IP_ROI, const LPPOINT, const double);
void ACCUAPI				ROI_clip_rect_get( const IP_ROI, LPRECT);
void ACCUAPI				ROI_clip_rect_set( IP_ROI, const LPRECT);
void ACCUAPI				ROI_clip_rect_set_for_kern( const IP_IMAGE, IP_ROI, const LPKERN);
void ACCUAPI				ROI_bounding_rect_get( const IP_ROI, LPRECT);
BOOL ACCUAPI				ROI_point_on_border_get( const IP_ROI, LPPOINT);
BOOL ACCUAPI				ROI_is_point_inside(	const	IP_ROI, const LPPOINT);
BOOL ACCUAPI				ROI_is_convex(	const	IP_ROI);
BOOL ACCUAPI				ROI_is_symetric( const IP_ROI, const WORD);
BOOL ACCUAPI				ROI_statistics_get( const IP_ROI, IP_ROI_STATS);

BOOL ACCUAPI				ROI_seg_add( IP_ROI, const LPSEGMENT);
BOOL ACCUAPI				ROI_seg_insert( IP_ROI, const WORD, const LPSEGMENT);
BOOL ACCUAPI				ROI_seg_remove( IP_ROI, const WORD);
BOOL ACCUAPI				ROI_seg_get( const IP_ROI, const WORD, LPSEGMENT);
WORD ACCUAPI				ROI_seg_number_of_segments_get( const IP_ROI);
void ACCUAPI				ROI_seg_clip( LPSEGMENT, const LPRECT);

void ACCUAPI				ROI_sqr_set( IP_ROI, const LPSQUARE);
void ACCUAPI				ROI_sqr_get( const IP_ROI, LPSQUARE);
void ACCUAPI				ROI_sqr_move( IP_ROI, const LPPOINT);

void ACCUAPI				ROI_rect_set( IP_ROI, const LPRECT);
void ACCUAPI				ROI_rect_get( const IP_ROI, LPRECT);
void ACCUAPI				ROI_rect_move(	IP_ROI, const LPPOINT);

void ACCUAPI				ROI_circle_set( IP_ROI, const LPCIRCLE);
void ACCUAPI				ROI_circle_get( const IP_ROI, LPCIRCLE);
void ACCUAPI				ROI_circle_move( IP_ROI, const LPPOINT);
void ACCUAPI				ROI_circle_segment_get(	const	IP_ROI, const WORD, LPSEGMENT);

/***	Image skew ***********************************************************/
BOOL ACCUAPI				skew_horiz_using_2pnts( IP_IMAGE, BOOL (ACCUAPI *)( LPVOID, int, int), const LPVOID, const LPPOINT,const LPPOINT, const MODE);
BOOL ACCUAPI				skew_horiz_using_disp_array( IP_IMAGE, BOOL (ACCUAPI *)( LPVOID, int, int), const LPVOID, const LPINT, const MODE);
BOOL ACCUAPI				skew_horiz_auto_find_edge( IP_IMAGE, BOOL (ACCUAPI *)( LPVOID, int, int), const LPVOID,  const LPRGB, const MODE, const MODE, const LENGTH);
BOOL ACCUAPI				skew_horiz_auto_find_doc( IP_IMAGE, BOOL (ACCUAPI *)( LPVOID, int, int), const LPVOID, const MODE, const PIXPOS, const PIXPOS, const PIXPOS, const PIXPOS, const LENGTH);


/***	Misc. support	******************************************************/
void ACCUAPI				pack_RGB_into_long( const int, const int, const int, LPLONG);
void ACCUAPI				pack_long_into_RGB( const LPLONG, LPRGB);
void ACCUAPI				int_sort( LPINT, LPINT);
void ACCUAPI				rect_sort( LPRECT);
void ACCUAPI				rect_center( const LPRECT, LPPIXPOS, LPPIXPOS);
void ACCUAPI				rect_rotate( const LPRECT,	const double, LPRECT, const LPPOINT);
void ACCUAPI				rect_intersection( LPRECT,	const	LPRECT, const LPRECT);
void ACCUAPI				rect_union(	LPRECT, const LPRECT, const LPRECT);
void ACCUAPI				rect_copy( LPRECT, const LPRECT);
void ACCUAPI				rect_make( LPRECT, const LPPOINT, const LPPOINT);
BOOL ACCUAPI				rect_is_empty(	const	LPRECT);
BOOL ACCUAPI				rect_is_equal(	const	LPRECT, const LPRECT);
BOOL ACCUAPI				rect_is_a_sub_rect(	const	LPRECT, const LPRECT);
BOOL ACCUAPI				point_is_in_rect(	const	LPPOINT,	const	LPRECT);
BOOL ACCUAPI				point_is_on_rect(	const	LPPOINT,	const	LPRECT);
int ACCUAPI					double_to_string(	const	double, const int, LPSTR);
int ACCUAPI					IPL_DLL_rev( LPINT, LPINT);
void ACCUAPI				IPL_DLL_rev_update( LPINT, LPSTR);
LPSTR	ACCUAPI				IPL_DLL_compile_date( void);
int ACCUAPI					IPL_DLL_days_left_to_time_out( void);


/***	Barcodes ***********************************************************/
BOOL ACCUAPI				barcode_read_UPC( const IP_IMAGE, const LPRECT, const MODE, const DIMENSION, LPBARCODE_UPC);
BOOL ACCUAPI				barcode_draw_UPC( IP_IMAGE, const LPRECT, const LPBARCODE_UPC);

BOOL ACCUAPI				barcode_read_codabar( const IP_IMAGE, const LPRECT, const MODE, const DIMENSION, LPBARCODE_CODABAR);
BOOL ACCUAPI				barcode_draw_codabar( IP_IMAGE, const LPRECT, const LPBARCODE_CODABAR);



/***	Find ***********************************************************/
BOOL ACCUAPI				find_doc_left_edge( IP_IMAGE, BOOL (ACCUAPI *)( LPVOID, int, int), const LPVOID, PIXPOS, PIXPOS, LPPIXEL, LENGTH, LPPOINT);



/***	Graphics ***********************************************************/

BOOL ACCUAPI				line_clip_to_rect( const LPRECT, LPLINE);



/*#ifndef __ACCUAIPL_H__*/
#endif

#ifdef __cplusplus 
}
#endif

/* #if !defined(WINVER) */
#endif
