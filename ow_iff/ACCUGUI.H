/****************************************************************************
;
;	HEADER:	  ACCUGUI.H -	Master include	header 
;
;
;	DATE:			01/11/1995
;
;
;			Copyright 1995, AccuSoft Corporation.
;
;****************************************************************************/

#ifdef ACCUAPI
#undef ACCUAPI
#endif

#ifdef __cplusplus 
extern "C" {
#endif

#if defined(WIN32)
#define ACCUAPI	__declspec(dllexport) PASCAL
#else
#define ACCUAPI	FAR PASCAL
#endif



/********************************************************************************
  Major and Minor DLL revision numbers used to compile the DLL
  
  test these values against the function GUI_DLL_rev( LPINT, LPINT) to 
  verify that your DLL matches this header files.
 *******************************************************************************/
#define	GUI_REV_MAJOR					1 /* Major function changes and improvements  {1..9}								*/
#define	GUI_REV_MINOR					2 /* New functions and minor re-orginzations  {00, 01, 02, ... 99}			*/
#define	GUI_REV_UPDATE					3 /* Bugs fixed, speed improvements           {0='a', 1='b', ..., 25='z'}	*/


typedef enum
	{
	DIB_HANDLE,
	DIB_POINTER,
	} DIBDATATYPE;

typedef int (FAR PASCAL *CROPSETCB)(LPVOID lpcbdata, const LPRECT lpcrop);
typedef int (FAR PASCAL *CROPGETCB)(LPVOID lpcbdata, LPRECT lpcrop);
typedef int (FAR PASCAL *SIZEGETCB)(LPVOID lpcbdata, LPSIZE lpsize);
typedef int (FAR PASCAL *PALETTEMODIFIEDCB)(LPVOID lpcbdata, UINT index);
typedef int (FAR PASCAL *PAINTCB)(LPVOID lpcbdata, HWND hwnd, HDC hdc, const LPRECT lpcrop);

int ACCUAPI 				GUI_rect_roi_begin(HWND hwnd);
int ACCUAPI 				GUI_rect_roi_get(HWND hwnd, LPRECT lprect);
int ACCUAPI					GUI_rect_roi_get_vb( HWND hwnd, LPINT ROI_left, LPINT ROI_top, LPINT ROI_right, LPINT ROI_bottom);
BOOL ACCUAPI 				GUI_rect_roi_exists(HWND hwnd);
BOOL ACCUAPI 				GUI_is_rect_mode(HWND hwnd);
int ACCUAPI 				GUI_rect_roi_end(HWND hwnd);

int ACCUAPI 				GUI_poly_roi_begin(HWND hwnd);
HGLOBAL ACCUAPI 			GUI_poly_roi_get(HWND hwnd, LPINT count);
BOOL ACCUAPI 				GUI_poly_roi_exists(HWND hwnd);
BOOL ACCUAPI 				GUI_is_poly_mode(HWND hwnd);
int ACCUAPI 				GUI_poly_roi_end(HWND hwnd);

int ACCUAPI 				GUI_intensity_profile_begin(HWND hwnd, LPVOID dibhandle, DIBDATATYPE ddtDIB);
BOOL ACCUAPI 				GUI_is_intensity_profile_mode(HWND hwnd);
int ACCUAPI 				GUI_intensity_profile_end(HWND hwnd);

int ACCUAPI 				GUI_pixel_dump_begin(HWND hwnd, LPVOID dibhandle, DIBDATATYPE ddtDIB);
int ACCUAPI 				GUI_pixel_dump_update(HWND hwnd);
BOOL ACCUAPI 				GUI_is_pixel_dump_mode(HWND hwnd);
int ACCUAPI 				GUI_pixel_dump_end(HWND hwnd);

int ACCUAPI 				GUI_magnify_begin(HWND hwnd, LPVOID dibhandle, DIBDATATYPE ddtDIB, HPALETTE hpal);
int ACCUAPI 				GUI_magnify_update(HWND hwnd, HPALETTE hpal);
BOOL ACCUAPI 				GUI_is_magnify_mode(HWND hwnd);
int ACCUAPI 				GUI_magnify_end(HWND hwnd);

int ACCUAPI 				GUI_histogram_begin(HWND hwnd, LPVOID dibhandle, DIBDATATYPE ddtDIB, HPALETTE hpal);
int ACCUAPI 				GUI_histogram_update(HWND hwnd, HPALETTE hpal);
BOOL ACCUAPI 				GUI_is_histogram_mode(HWND hwnd);
int ACCUAPI 				GUI_histogram_end(HWND hwnd);

int ACCUAPI 				GUI_palette_editor_begin(HWND hwnd, LPVOID dibhandle, DIBDATATYPE ddtDIB, HPALETTE hpal);
int ACCUAPI 				GUI_palette_editor_update(HWND hwnd, HPALETTE hpal);
BOOL ACCUAPI 				GUI_is_palette_modified(HWND hwnd);
BOOL ACCUAPI 				GUI_is_palette_editor_mode(HWND hwnd);
int ACCUAPI 				GUI_palette_editor_end(HWND hwnd);

int ACCUAPI 				GUI_pan_window_begin(HWND hwnd, LPVOID dibhandle, DIBDATATYPE ddtDIB, HPALETTE hpal);
int ACCUAPI 				GUI_pan_window_update(HWND hwnd, HPALETTE hpal);
BOOL ACCUAPI 				GUI_is_pan_window_mode(HWND hwnd);
int ACCUAPI 				GUI_pan_window_end(HWND hwnd);

int ACCUAPI 				GUI_crop_set_register(HWND hwnd, const LPVOID lpcbdata, CROPSETCB cropset);
int ACCUAPI 				GUI_crop_get_register(HWND hwnd, const LPVOID lpcbdata, CROPGETCB cropget);
int ACCUAPI 				GUI_size_get_register(HWND hwnd, const LPVOID lpcbdata, SIZEGETCB sizeget);
int ACCUAPI 				GUI_palette_modified_register(HWND hwnd, const LPVOID lpcbdata, PALETTEMODIFIEDCB palettemodified);
int ACCUAPI 				GUI_paint_register(HWND hwnd, const LPVOID lpcbdata, PAINTCB paint);
int ACCUAPI 				GUI_unregister(HWND hwnd);

int ACCUAPI 				GUI_point_window_to_image(const LPPOINT lpwnd, LPPOINT lpimg, const LPRECT lpcrop, const LPSIZE lpsize);
int ACCUAPI 				GUI_point_image_to_window(LPPOINT lpwnd, const LPPOINT lpimg, const LPRECT lpcrop, const LPSIZE lpsize);

int ACCUAPI 				GUI_rect_window_to_image(HWND hwnd, const LPRECT lpwnd, LPRECT lpimg);
int ACCUAPI 				GUI_rect_image_to_window(HWND hwnd, LPRECT lpwnd, const LPRECT lpimg);

int ACCUAPI 				GUI_point_screen_to_image(HWND hwnd, const LPPOINT lpwnd, LPPOINT lpimg);
int ACCUAPI 				GUI_point_image_to_screen(HWND hwnd, LPPOINT lpwnd, const LPPOINT lpimg);

HPALETTE ACCUAPI 			GUI_hpalette_dib_create(LPVOID dibhandle, DIBDATATYPE ddtDIB);
HPALETTE ACCUAPI 			GUI_hpalette_gray_create(VOID);
HPALETTE ACCUAPI 			GUI_hpalette_red_create(VOID);
HPALETTE ACCUAPI 			GUI_hpalette_green_create(VOID);
HPALETTE ACCUAPI 			GUI_hpalette_blue_create(VOID);

HINSTANCE ACCUAPI 		GUI_module_handle(VOID);

int ACCUAPI 				GUI_clipboard_copy(IP_IMAGE image, IP_ROI roi);
int ACCUAPI 				GUI_clipboard_paste(IP_IMAGE image, const POINT location);
BOOL ACCUAPI				GUI_clipboard_paste_available(VOID);
int ACCUAPI					GUI_clipboard_image_size(LPDWORD width, LPDWORD height, LPINT bitspixel);


int ACCUAPI					GUI_DLL_rev( LPINT, LPINT);
void ACCUAPI				GUI_DLL_rev_update( LPINT, LPSTR);
LPSTR	ACCUAPI				GUI_DLL_compile_date( void);


#ifdef __cplusplus 
}
#endif
