/****************************************************************************
;
;	HEADER:	  IPLERRS.H -	IPL Error codes
;
;
;	Date created:			06/01/1994 DF
;
;	$Date: 1995/12/12 14:44:18 $
;	$Revision: 1.5 $
;
;			Copyright 1994-95, AccuSoft Corporation.
;
;****************************************************************************/


#define	AE_NOT_DONE_YET								-1


/*	Lowlevel error codes */
#define	AE_OUT_OF_MEMORY								-1000	/* No more global memory is available for allocation, reduced used resources									*/
#define	AE_AI_HANDLE_NOT_GLOBAL						-1010	/* A global AI_HANDLE was expected and not recieved																	*/
#define	AE_AI_HANDLE_NOT_LOCAL						-1020	/* A global AI_HANDLE was expected and not recieved																	*/
#define	AE_EVAL_DLL_TIMEOUT_HAS_EXPIRED			-1030	/* The DLL is an Evaluation copy and as timed out - contact ACCUSOFT to purchase a release copy			*/
#define	AE_INVALID_STANDARD_KERNEL					-1040	/* The kernel expected should be one of the predefined ones, your could not be found						*/
#define	AE_INTERNAL_ERROR								-1050	/*	An internal error has occured, contact ACCUSOFT techical support												*/
#define	AE_CLIP_BOARD_NOT_SUPPORTED				-1060	/**/

/*	Mid level error codes */
#define	AE_WRONG_DIB_BIT_COUNT						-2000	/* The DIB has bit with the wrong bit count for this routine														*/
#define	AE_LOCK_FAILED									-2010 /* Memory required for this routine could not be locked, most likly running out of memory resources	*/
#define	AE_ALLOC_FAILED								-2020 /* Memory required for this routine could not be allocated, free up some resources and try again		*/
#define	AE_FREE_FAILED									-2030	/* An internal memory free has failed, ussally a bad handle, or corrupted system								*/
#define	AE_BAD_KERN_TYPE 								-2040	/**/
#define	AE_AI_HANDLES_USED_UP						-2050	/* The maximum number od AccuSoft handles has been used - no more left.  Free up some and try again	*/
#define	AE_AI_HANDLE_INVALID							-2060	/* This routine requires an AccuSoft handle.  The handle passed in was not allocated by AccuSoft		*/
#define	AE_DIBS_ARE_INCOMPATIBLE					-2070	/* The images are not compatible for this function, either dimension, bit count, or both					*/
#define	AE_DIB_DIMENSIONS_NOT_EQUAL				-2090	/* The images must be the same dimensions																					*/
#define	AE_DIB_BIT_COUNTS_NOT_EQUAL				-2100	/*	The images must have the same bit count																				*/
#define	AE_DIB_HAS_NO_PALETTE						-2101	/* The image passed in does not have a palette and this routine requires one									*/
#define	AE_ROI_WRONG_TYPE								-2110	/**/
#define	AE_REQUIRES_CONVEX_ROI						-2120	/* This function requires a convex ROI. The one passed in must be convex										*/
#define	AE_INVALID_RAMP_DIRECTION					-2130	/* The contrast ramps direction is not supported																		*/
#define	AE_INVALID_LUT_ARITH_FUNC					-2140	/* The LUT_ARITH_FUNC is not a valid function number, check the constant										*/
#define	AE_INVALID_KERN_MOTION_EXTENT				-2150	/**/
#define	AE_INVALID_NOISE_TYPE						-2160	/**/
#define	AE_INVALID_KERN_NORMALIZER					-2170	/**/
#define	AE_INVALID_SIGMA								-2180	/**/
#define	AE_INVALID_SKEW_POINTS						-2190	/* A valid line could not be drawn through the two point provided.  Y1==Y2 									*/
#define	AE_TILE_IS_LARGER_THAN_IMAGE				-2200	/* The tile image must be the same size or smaller in both dimensions than the original source			*/
#define	AE_COLOR_SPACE_INVALID						-2210	/* Invalid type of color space MODE																							*/
#define	AE_DIB_POINTER_IS_NULL						-2220	/* The DIB pointer about to be used is NULL (invalid)																	*/

#define	AE_FILE_CANT_BE_OPENED						-2230	/* An attempt to open a file failed, it may not exist in the provided path										*/
#define	AE_FILE_TO_SMALL_TO_BE_BMFH				-2240	/* The file is too small to be a valid BMFH																				*/
#define	AE_FILE_IS_NOT_BMP							-2250	/* The BMFH Magic number is invalid																							*/
#define	AE_FILE_TO_SMALL_TO_BE_BMIH				-2260	/* The file is too small to be a valid BMIH																				*/
#define	AE_BMP_IS_COMPRESSED							-2270	/* The BMP file is in compressed (RLE) format																			*/
#define	AE_CANT_READ_PALETTE							-2280
#define	AE_CANT_READ_PIXELS							-2290


/*	Application	level	error	codes	*/
#define	AE_PROC_INVAL_FOR_BIT_COUNT				-3000	/* This function can not be used on images with this one's bit count												*/
#define	AE_PROC_INVAL_FOR_PALETTE_IMG				-3010	/* This function can not be used on 8-bit color images - try to promote to 24-bit							*/
#define	AE_PARAMETER_OUT_OF_LIMITS					-3020	/* A parameter is out of its legal range																					*/


/*	VBX level	error	codes	*/
#define AE_VBX_INVALID_FUNCTION_NUM					-8000	/* Invalid IPL-VBX function number																							*/


/*	Application error codes must be less that the following last number used	*/
#define	AE_LAST_ERROR_NUMBER							-9999	

