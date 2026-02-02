/*
 *	Copyright 1992, AccuSoft Corporation.
 */

/* define standard error returns for bitmap dll */

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
