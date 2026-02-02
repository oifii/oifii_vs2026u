// includes symbols shared by multipal resources 

// taken from resource.h
#define IDP_BUSY                        100
#define IDR_MAINFRAME                   2
#define IDR_IMAGE_TYPE                  3
#define IDR_COLOR_IMAGE                 4
#define IDR_GREY_IMAGE					5
#define IDR_COLOR_4COMP					6
#define IDR_STAT_TYPE                   7
#define IDR_STATDATA_TYPE               8

//SPoirier June 1st 1994, pattern matching
#define IDR_MODEL_TYPE					10
//SPoirier August 23rd 1994, result log window
#define IDR_RESULTLOG_TYPE				11
//SPoirier August 30th 1994, mil trace window
#define IDR_MILTRACE_TYPE				12

//GLachape September 14th, 1994, Gauging
#define IDR_SSHEETTYPE                  13

// blarivie - 940916 - for blob doc. templates
#define IDR_BLOB_ALL_RES_TYPE           14
#define IDR_BLOB_ONE_RES_TYPE           15
#define IDR_BLOB_TYPE                   16
//bei 940921 for script menu
#define IDR_SCRIPTTYPE                  17
//bei 940921 for proformview
#define IDR_PROCFORMVIEW_TYPE           18
//GLachape October 14th, 1994, Gauging
#define IDR_MEASTABVIEW                 20


// menu ID's - recommended range - 100 - 2000
// yoli - in SHARESYM.H to avoid confusion, lets stay in the range 300 -1000
// the values of the next ID_VIEW_ZOOM are important - please do not change
//  without a notice 
#define ID_VIEW_ZOOM_BASE               300
#define ID_VIEW_ZOOM_IN_11              301
#define ID_VIEW_ZOOM_IN_21              302
#define ID_VIEW_ZOOM_IN_31              303
#define ID_VIEW_ZOOM_IN_41              304
#define ID_VIEW_ZOOM_IN_51              305
#define ID_VIEW_ZOOM_IN_61              306
#define ID_VIEW_ZOOM_IN_71              307
#define ID_VIEW_ZOOM_IN_81              308
#define ID_VIEW_ZOOM_OUT_11             317
#define ID_VIEW_ZOOM_OUT_12             318
#define ID_VIEW_ZOOM_OUT_13             319
#define ID_VIEW_ZOOM_OUT_14             320
#define ID_VIEW_ZOOM_OUT_15             321
#define ID_VIEW_ZOOM_OUT_16             322
#define ID_VIEW_ZOOM_OUT_17             323
#define ID_VIEW_ZOOM_OUT_18             324
#define ID_VIEW_TOFIT                   332
#define ID_VIEW_ZOOM				    333	// does not exist in the menu;
											//  corresponds to the magnifying glass
											//  toolbar icon 
#define ID_BASE_POPUP_GRAPHMENU0        350
#define ID_BASE_POPUP_GRAPHMENU1        351
#define ID_BASE_POPUP_GRAPHMENU2        352
#define ID_BASE_POPUP_GRAPHMENU3        353
#define ID_BASE_POPUP_GRAPHMENU4        354
#define ID_BASE_POPUP_GRAPHMENU5        355
#define ID_BASE_POPUP_GRAPHMENU6        356
#define ID_BASE_POPUP_GRAPHMENU7        357
#define ID_BASE_POPUP_GRAPHMENU8        358
#define ID_BASE_POPUP_GRAPHMENU9        359
#define ID_BASE_POPUP_GRAPHMENU10       360
#define ID_BASE_POPUP_GRAPHMENU11       361
#define ID_BASE_POPUP_GRAPHMENU12       362
#define ID_BASE_POPUP_GRAPHMENU13       363
#define ID_BASE_POPUP_GRAPHMENU14       364
#define ID_BASE_POPUP_GRAPHMENU15       365
#define ID_BASE_POPUP_GRAPHMENU16       366
#define ID_BASE_POPUP_GRAPHMENU17       367
#define ID_BASE_POPUP_GRAPHMENU18       368
#define ID_BASE_POPUP_GRAPHMENU19       369
#define ID_BASE_POPUP_GRAPHMENU20       370

											
#define ID_VIEW_SELECTION_BAR           402
#define ID_VIEW_COMPOSITE               403
#define ID_VIEW_COMP3                   404
#define ID_VIEW_COMP4                   405
#define ID_VIEW_COMPMDI                 406
#define ID_VIEW_IMAGE_INFO              407

#define ID_VIEW_AS_LINEGRAPH            408
#define ID_VIEW_AS_VBARGRAPH            409
#define ID_VIEW_AS_HBARGRAPH            410
#define ID_VIEW_FIT_ALL                 411
#define ID_VIEW_FIT_ONEVALUEPERPIXEL    412
#define ID_VIEW_GRAPH                   413
#define ID_VIEW_GRAPHDATA               414

#define ID_TOOLS_MEASURES               416
#define ID_TOOLS_STATISTICS             417
#define ID_TOOLS_MAPPING                418
#define ID_TOOLS_ARITHMETIC             419
#define ID_TOOLS_FILTERS                420
#define ID_TOOLS_MORPHOLOGY             421
#define ID_TOOLS_COLOUR                 422
#define ID_TOOLS_GEOMETRIC              423
#define ID_TOOLS_ROI                    424
#define ID_TOOLS_HISTOGRAM              425
#define ID_TOOLS_XPROFILE               426
#define ID_TOOLS_YPROFILE               427
#define ID_TOOLS_LPROFILE               428
#define ID_TOOLS_HISTOEQUAL             429
#define ID_TOOLS_MEASURE_LENGTH         430
#define ID_TOOLS_MEASURE_ANGLE          431
#define ID_TOOLS_MAPPING_HISTOGRAMEQUALIZATION 432 //1057
#define ID_TOOLS_MAPPING_THRESHOLD      433 //1058
#define ID_TOOLS_MAPPING_BRIGHTNESSCONTRAST 434 //1059
#define ID_TOOLS_GEOMETRIC_SCALE        435
#define ID_TOOLS_GEOMETRIC_ROTATION     436
#define ID_TOOLS_GEOMETRIC_TRANSLATION  437
#define ID_TOOLS_GEOMETRIC_SYMMETRY     438

#define ID_TOOLS_SCRIPT_NEWSCRIPT		439
#define ID_TOOLS_SCRIPT_PAUSE			440
#define ID_TOOLS_SCRIPT_RESTART			441
#define ID_TOOLS_SCRIPT_STOP			442

#define ID_TOOLS_COLOR_NAMECOMPONENTS   443
#define ID_TOOLS_COLOR_EXTRACTCOMPONENT 444
#define ID_TOOLS_COLOR_MERGECOMPONENT   445
#define ID_TOOLS_COLOR_COLORCONVERSION  446
#define ID_TOOLS_COLOR_COLORCLASSIFY    447
#define ID_TOOLS_ROI_NEW                448
#define ID_TOOLS_ROI_DELETE             449
#define ID_TOOLS_ROI_SAVE               450
#define ID_TOOLS_ROI_PROPERTIES         451
#define ID_ANALYSIS_LOGRESULTS          452
#define ID_ANALYSIS_DRAWRESULTS         453
#define ID_ANALYSIS_VERBOSE             454
#define ID_PATTERN_NEWMODEL             455
#define ID_PATTERN_SEARCHMODEL          456
#define ID_PATTERN_NEWMASK              457
#define ID_PATTERN_CLEARMASK            458
#define ID_PATTERN_BRUSHSIZE1           459
#define ID_PATTERN_BRUSHSIZE2           460
#define ID_PATTERN_BRUSHSIZE3           461
#define ID_PATTERN_BRUSHSIZE5           462
#define ID_PATTERN_BRUSHSIZE10          463
#define ID_GAUGING_NEW_MARKER           464
#define ID_GAUGING_MEASURE              465
#define ID_PATTERN_MEASUREMENTTABLE     466
#define ID_GAUGING_RESET                467

#define ID_OPTIONS_CAMERASETTINGS       470
#define ID_OPTIONS_CALIBRATION          471
#define ID_OPTIONS_PREFERENCES          472
#define ID_OPTIONS_DISPLAYCONFIGURATION 473

#define ID_OPTIONS_AUTORESCALING		474
#define ID_OPTIONS_NOMARKER				475
#define ID_OPTIONS_ONEMARKER			476
#define ID_OPTIONS_TWOMARKER			477
#define ID_OPTIONS_MILTRACE             478

#define ID_ANALYSIS_BLOB                480

/////////////////////////////////////////////////////////////////////////////
// Standard Commands
// The specified range for each type of commands is already defined
//  in AFXRES.H
// File commands
//  from 0xE100 to 0xE113 are already defined
#define ID_FILE_GRAB                    500 //0xE114
#define ID_FILE_COLLECTION              501 //0xE115

// Edit commands
//  from 0xE120 to 0xE12C are already defined
#define ID_EDIT_DUPLICATE               502 //0xE12D

// Window commands
// from 0xE130 to 0xE135 are already defined
#define ID_WINDOW_CLOSE_ALL             503 //0xE136

//SPoirier May 9th 1994, BUG 38
#define ID_ROI                          505
#define ID_ARITHMETIC					506

// blarivie - 940720 - Undo menu items
#define ID_EDIT_UNDO_1					510
#define ID_EDIT_UNDO_2					511
#define ID_EDIT_UNDO_3					512
#define ID_LOWER_UNDO_LEVEL				513	// user-defined message

// Help and App commands
// from 0xE140 to 0xE143 are already defined

// OLE commands
// from 0xE200 to 0xE21F are already defined

// yoli 090411- drawing stuff IDs
// the values of the next IDs are important - please do not change
//  without a notice
#define ID_PALETTE_BASE                 600
#define ID_PALETTE_SELECT               600
#define ID_PALETTE_LINE                 601
#define ID_PALETTE_PROFILE_LINE         602
#define ID_PALETTE_ARROW                603
#define ID_PALETTE_ANGLE		        604
#define ID_PALETTE_RECT                 605
#define ID_PALETTE_ROI                  606
#define ID_PALETTE_ELLIPSE              607
#define ID_PALETTE_MEASURE_VERT         608
#define ID_PALETTE_MEASURE              609
#define ID_PALETTE_POLYLINE				610
#define ID_PALETTE_POLIGON              611
#define ID_PALETTE_FREEHAND             612
#define ID_PALETTE_TEXT                 613

// mainframe status bar indicators
#define ID_INSPTR_MAINFRM_STATUS_BAR    640
#define ID_INDICATOR_ZERO               641
#define ID_INDICATOR_ONE                642
#define ID_INDICATOR_TWO                643
#define ID_INDICATOR_THREE              644 
#define ID_INDICATOR_FOUR               645 

// imageframe status bar indicators
#define ID_IMG_FRM_STATUS_BAR           656
#define ID_IMG_FRM_INDCR_ZERO           657 
#define ID_IMG_FRM_INDCR_ONE            658 
#define ID_IMG_FRM_INDCR_TWO            659 
#define ID_IMG_FRM_INDCR_THREE          660 
#define ID_IMG_FRM_INDCR_FOUR           661 



#define IDM_PALETTE_TOGGLE              32768
#define IDM_PALETTE_2COLUMN             32804
#define IDM_PALETTE_3COLUMN             32805

