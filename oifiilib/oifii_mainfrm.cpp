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

// mainfrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "oifiilib.h" //spi 2014
#include "oifii_app.h"

#include "c_pointset.h"
#include "c_xyzi.h"

#include "logdoc.h"

//2012august23, poirier, begin
#include "portaudio.h"
#include "pa_asio.h"
#include <map>
#include <string>
using namespace std;
//#include "spiws_WavSet.h" //spi 2014
#include "spiwavsetlib.h"
//2012august23, poirier, end

#include "oifii_mainfrm.h"
#include "afxpriv.h"

#include "logmdi.h"
#include "logview.h"

//#include "imglib.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

//2012august23, poirier, begin
#define MAINFRM_FRAMESPERBUFFER	2048
//#define MAINFRM_FRAMESPERBUFFER	1024
#define MAINFRM_PLAYING_THREAD			0
#define MAINFRM_SWAPPING_THREAD			1
#define MAINFRM_SUMMING_THREAD			2
#define MAINFRM_NUMTHREADS		3
HANDLE global_lpThreadHANDLE[MAINFRM_NUMTHREADS]; 
DWORD global_lpThreadID[MAINFRM_NUMTHREADS]; 
PaStream* global_pPaStream;

class WavSet* global_pWavSet11;
class WavSet* global_pWavSet12;
class WavSet* global_pWavSet21;
class WavSet* global_pWavSet22;

bool global_stopplaying;

PaStreamParameters global_outputParameters;
PaError global_PaError;
int global_outputAudioChannelSelectors[2];
PaAsioStreamInfo global_asioOutputInfo;
map<std::string,int> global_devicemap;
string global_audiodevicename;

void TraceAudioError()
{
	CString myString;
	TRACE("An error occured while using the portaudio stream\n");
	myString.Format(L"Error number: %d\n", global_PaError);
	TRACE(myString);
	myString.Format(L"Error message: %s\n", Pa_GetErrorText(global_PaError));
	TRACE(myString);
}

// This routine will be called by the PortAudio engine when audio is needed.
// It may called at interrupt level on some machines so don't do anything
// that could mess up the system like calling malloc() or free().
static int playCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{
	 if(global_stopplaying) return 1; //to stop all streams

    //Cast data passed through stream to our structure. 
	WavSet* pWavSet = (WavSet*)userData;//paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned int i;
    (void) inputBuffer; // Prevent unused variable warning. 

	int idSegmentToPlay = pWavSet->idSegmentSelected+1;
	if(idSegmentToPlay>(pWavSet->numSegments-1)) idSegmentToPlay=0;
#ifdef _DEBUG
	printf("idSegmentToPlay=%d\n",idSegmentToPlay);
#endif //_DEBUG
	float* pSegmentData = pWavSet->GetPointerToSegmentData(idSegmentToPlay);
	ASSERT(pWavSet->numChannels==2);
	for( i=0; i<framesPerBuffer; i++ )
    {
		*out++ = *(pSegmentData+2*i);  // left 
		*out++ = *(pSegmentData+2*i+1);  // right 
        /*
		// Generate simple sawtooth phaser that ranges between -1.0 and 1.0.
        data->left_phase += 0.01f;
        // When signal reaches top, drop back down. 
        if( data->left_phase >= 1.0f ) data->left_phase -= 2.0f;
        // higher pitch so we can distinguish left and right. 
        data->right_phase += 0.03f;
        if( data->right_phase >= 1.0f ) data->right_phase -= 2.0f;
		*/
    }
	pWavSet->idSegmentSelected = idSegmentToPlay;
    return 0;
}

DWORD WINAPI playingThrdFunc(LPVOID n)
{
	//2020july03, spi, begin
    //int TNumber = (int)n;
	int TNumber = PtrToUlong(n);
	//2020july03, spi, end
	WavSet* prev_pWavSet = NULL;
    WavSet* inuse_pWavSet = NULL; 
    //Here we will wait for the messages
    while (1)
    {
        MSG msg;
        //BOOL    MsgReturn  =  PeekMessage ( &msg , NULL , 
        //    THRD_MESSAGE_SOMEWORK , THRD_MESSAGE_EXIT , PM_REMOVE );
        BOOL MsgReturn = GetMessage(&msg, NULL, OWM_PLAYING_EXECUTE , OWM_PLAYING_EXIT);
        if (MsgReturn)
        {
            switch (msg.message)
            {
            case OWM_PLAYING_EXECUTE:
                //cout << "Working Message.... for Thread Number " << TNumber << endl;
				TRACE("Working Message... for Playing Thread\n");

				///////////////////////
				// Initialize portaudio 
				///////////////////////
				//already initialized

				////////////////////////
				//audio device selection
				////////////////////////
				//already selected
				
				/////////////////////////////
				//play wavset using portaudio
				/////////////////////////////
				while(global_stopplaying==FALSE)
				{
					/*
					//if(prev_pWavSet!=NULL || prev_pWavSet==global_pWavSet11) inuse_pWavSet=global_pWavSet12;
					//  else inuse_pWavSet=global_pWavSet11;
					if(prev_pWavSet==NULL) 
					{
						inuse_pWavSet=global_pWavSet11;
						//cout << "using global_pWavSet11" << endl;
						TRACE("using global_pWavSet11\n");
					}
					else if(inuse_pWavSet==global_pWavSet11) 
					{
						inuse_pWavSet=global_pWavSet12;
						//cout << "using global_pWavSet12" << endl;
						TRACE("using global_pWavSet12\n");
					}
					else 
					{
						inuse_pWavSet=global_pWavSet11;
						//cout << "using global_pWavSet11" << endl;
						TRACE("using global_pWavSet11\n");
					}
					*/
					if(inuse_pWavSet==NULL) continue;

					//open stream
					global_PaError = Pa_OpenStream(
								&global_pPaStream,
								&global_outputParameters, 
								NULL, // no output
								44100,
								MAINFRM_FRAMESPERBUFFER,
								paClipOff,      // we won't output out of range samples so don't bother clipping them 
								playCallback, //NULL, // no callback, use blocking API 
								inuse_pWavSet ); //NULL ); // no callback, so no callback userData 
					if(global_PaError!=paNoError)
					{
						TraceAudioError();
						//PostThreadMessage(global_lpThreadID[MAINFRM_PLAYING_THREAD], OWM_PLAYING_EXIT, 0 , 0);
						break; //while
					}
					//start stream
					global_PaError = Pa_StartStream(global_pPaStream);
					if(global_PaError!=paNoError) 
					{
						TraceAudioError();
						break; //while
					}
					/*
					prev_pWavSet = inuse_pWavSet;
					totalrecordedtime_s = totalrecordedtime_s + SPIRECORD_SMALLWAVSETLENGTH_S;
					cout << "thread " << TNumber << " recorded " << totalrecordedtime_s << " sec" << endl;
					PostThreadMessage(global_lpThreadID[SPIRECORD_COPYING_THREAD], SPIRECORD_WMMSG_COPYING_EXECUTE, 0 , (LPARAM)prev_pWavSet);
					MSG msg2;
					BOOL MsgReturn2 = PeekMessage(&msg2, NULL, SPIRECORD_WMMSG_RECORDING_EXECUTE , SPIRECORD_WMMSG_RECORDING_EXIT, PM_NOREMOVE);
					if (MsgReturn2 && msg2.message==SPIRECORD_WMMSG_RECORDING_EXIT)
					{
						break;
					}
					*/
				}
				PostThreadMessage(global_lpThreadID[MAINFRM_PLAYING_THREAD], OWM_PLAYING_EXIT, 0 , 0);
                break;
            case OWM_PLAYING_EXIT:
                //cout << "Exiting Message... for Thread Number " << TNumber << endl;
				TRACE("Exiting Message... for Playing Thread\n");
				if(global_pPaStream)
				{
					global_PaError = Pa_CloseStream(global_pPaStream);
					if(global_PaError!=paNoError) 
					{
						TraceAudioError();
					}
				}
				PostThreadMessage(global_lpThreadID[MAINFRM_SWAPPING_THREAD], OWM_SWAPPING_EXIT, 0 , 0);
				PostThreadMessage(global_lpThreadID[MAINFRM_SUMMING_THREAD], OWM_SUMMING_EXIT, 0 , 0);
                return 0;
            }
        }
    } 
    return 0;
}

DWORD WINAPI swappingThrdFunc(LPVOID n)
{
	//2020july03, spi, begin
	//int TNumber = (int)n;
	int TNumber = PtrToUlong(n);
	//2020july03, spi, end
    //WavSet* prev_pWavSet = NULL; 
    //WavSet* inuse_pWavSet = global_pWavSet21; 
	//int dst_offset_frame = 0;
    //Here we will wait for the messages
    while (1)
    {
        MSG msg;
        //BOOL    MsgReturn  =  PeekMessage ( &msg , NULL , 
        //    THRD_MESSAGE_SOMEWORK , THRD_MESSAGE_EXIT , PM_REMOVE );
        BOOL MsgReturn = GetMessage(&msg, NULL, OWM_SWAPPING_EXECUTE , OWM_SWAPPING_EXIT);
        //WavSet* tocopy_pWavSet = NULL;     
        if (MsgReturn)
        {
            switch (msg.message)
            {
            case OWM_SWAPPING_EXECUTE:
				//cout <<"Working Message.... for Thread Number " << TNumber << endl;
				TRACE("Working Message... for Swapping Thread\n");
				/*
				tocopy_pWavSet = (WavSet*)msg.lParam;
				inuse_pWavSet->CopyNoMalloc(tocopy_pWavSet, tocopy_pWavSet->totalFrames, 0, dst_offset_frame);
				//cout <<"Working Message.... for Thread Number " << TNumber << endl;
				cout << "thread " << TNumber << " copied " << SPIRECORD_SMALLWAVSETLENGTH_S << " sec" << endl;
				global_copied += 1;
				dst_offset_frame+=tocopy_pWavSet->totalFrames;
				if(global_copied==SPIRECORD_LARGEWAVSETLENGTH_S/SPIRECORD_SMALLWAVSETLENGTH_S)
				{
					prev_pWavSet = inuse_pWavSet;
					PostThreadMessage(global_lpThreadID[SPIRECORD_WRITINGTODISK_THREAD], SPIRECORD_WMMSG_WRITINGTODISK_EXECUTE, 0 , (LPARAM)prev_pWavSet);
					global_copied = 0;
					dst_offset_frame = 0;
					if(inuse_pWavSet==global_pWavSet21) inuse_pWavSet=global_pWavSet22;
						else inuse_pWavSet=global_pWavSet21;
				}
				*/
                break;
            case OWM_SWAPPING_EXIT:
                //cout<<"Exiting Message... for Thread Number " <<TNumber<<endl;
				TRACE("Exiting Message... for Swapping Thread\n");
                return 0;
            }
        }
    } 
    return 0;
}

DWORD WINAPI summingThrdFunc(LPVOID n)
{
	//2020july03, spi, begin
    //int TNumber = (int)n;
	int TNumber = PtrToUlong(n);
	//2020july03, spi, end
	int i=0;
    //Here we will wait for the messages
    while (1)
    {
        MSG msg;
        //BOOL    MsgReturn  =  PeekMessage ( &msg , NULL , 
        //    THRD_MESSAGE_SOMEWORK , THRD_MESSAGE_EXIT , PM_REMOVE );
        BOOL MsgReturn = GetMessage(&msg, NULL, OWM_SUMMING_EXECUTE , OWM_SUMMING_EXIT);
        //WavSet* towritetodisk_pWavSet = NULL;     
            
        if (MsgReturn)
        {
            switch (msg.message)
            {
            case OWM_SUMMING_EXECUTE:
                //cout << "Working Message.... for Thread Number " << TNumber << endl;
				TRACE("Working Message... for Summing Thread\n");
				/*
				towritetodisk_pWavSet = (WavSet*)msg.lParam;
				cout << "thread " << TNumber << " wrote to disk " << SPIRECORD_LARGEWAVSETLENGTH_S << " sec" << endl;
				//write wavset
				if(i==0)
				{
					towritetodisk_pWavSet->WriteWavFile(global_filename.c_str());
				}
				else
				{
					towritetodisk_pWavSet->AppendWavFile(global_filename.c_str());
				}
				i++;
				*/
                break;
            case OWM_SUMMING_EXIT:
                //cout<<"Exiting Message... for Thread Number " <<TNumber<<endl;
				TRACE("Exiting Message... for Summing Thread\n");
                return 0;
            }
        }
    } 
    return 0;
}

//2012august23, poirier, end



/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars
	
// toolbar buttons - IDs are command buttons
/*
static UINT BASED_CODE buttons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
		ID_SEPARATOR,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
		ID_SEPARATOR,
	ID_FILE_PRINT,
	ID_APP_ABOUT,
	ID_CONTEXT_HELP,
};
*/
static UINT BASED_CODE buttons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
	ID_SEPARATOR,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
	ID_SEPARATOR,
	ID_SEPARATOR, /* combo goes here, slot 8 */
	ID_SEPARATOR,
	ID_ZOOM_PERCENT,
	ID_ZOOM_IN,
	ID_ZOOM_OUT,
	ID_ZOOM_POINTIN,
	ID_ZOOM_POINTOUT,
	ID_ZOOM_RECTIN,
	ID_SEPARATOR,
	ID_ROI_FLOATING,
	ID_ROI_RECT,
	ID_ROI_POLYLINE,
	ID_ROI_FREEHAND,
	ID_SEPARATOR,
	ID_CONTEXT_HELP,
};

/*
static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
*/
static UINT BASED_CODE indicators[] =
{
    ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_POINT,
    ID_INDICATOR_ZOOMP,
    ID_INDICATOR_ZOOMF,
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_TEST_POINTSET, OnTestPointset)
	ON_COMMAND(ID_TEST_SEGMENTSET, OnTestSegmentset)
	ON_COMMAND(ID_TEST_TREEFINDER, OnTestTreefinder)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_ACQUIRE, OnFileAcquire)
	ON_UPDATE_COMMAND_UI(ID_FILE_ACQUIRE, OnUpdateFileAcquire)
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_LOGWINDOW, OnViewLogwindow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LOGWINDOW, OnUpdateViewLogwindow)
	//}}AFX_MSG_MAP
	// Global help commands
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_LOCATION,OnUpdateLocation)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_ZOOMP,OnUpdatePercent)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_ZOOMF,OnUpdateLevel)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_POINT,OnUpdatePoint)
	ON_COMMAND(ID_HELP_INDEX, CMDIFrameWnd::OnHelpIndex)
	ON_COMMAND(ID_HELP_USING, CMDIFrameWnd::OnHelpUsing)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpIndex)
	ON_WM_DESTROY()
	ON_COMMAND(ID_TRANSPORT_PLAY, &CMainFrame::OnTransportPlay)
	ON_UPDATE_COMMAND_UI(ID_TRANSPORT_PLAY, &CMainFrame::OnUpdateTransportPlay)
	ON_COMMAND(ID_TRANSPORT_PAUSE, &CMainFrame::OnTransportPause)
	ON_UPDATE_COMMAND_UI(ID_TRANSPORT_PAUSE, &CMainFrame::OnUpdateTransportPause)
	ON_COMMAND(ID_TRANSPORT_STOP, &CMainFrame::OnTransportStop)
	ON_UPDATE_COMMAND_UI(ID_TRANSPORT_STOP, &CMainFrame::OnUpdateTransportStop)
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
	m_bInitCoorValid = FALSE;
	RestoreFromINI();
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    const int nDropHeight = 100;
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
		!m_wndToolBar.SetButtons(buttons,
		  sizeof(buttons)/sizeof(UINT)))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,
												IDR_MAINFRAME_STATUS_BAR ) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

    //Create combobox.
    m_wndToolBar.SetButtonInfo(7,ID_SEPARATOR,TBBS_SEPARATOR,12);
    m_wndToolBar.SetButtonInfo(8,IDW_COMBO,TBBS_SEPARATOR,100);
    m_wndToolBar.SetButtonInfo(9,ID_SEPARATOR,TBBS_SEPARATOR,12);
    CRect rect;
    m_wndToolBar.GetItemRect(8, &rect);
    rect.top = 3;
    rect.bottom = rect.top + nDropHeight;
    if (!m_wndToolBar.m_comboBox.Create(CBS_DROPDOWN|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL,
					rect, &m_wndToolBar, IDW_COMBO))
	{
		TRACE0("Failed to create combo-box\n");
		return FALSE;
    }
    
    //  Fill the combo box
    CString szStyle;
    m_wndToolBar.m_comboBox.AddString(_T("10"));
    m_wndToolBar.m_comboBox.AddString(_T("20"));
    m_wndToolBar.m_comboBox.AddString(_T("40"));
    m_wndToolBar.m_comboBox.AddString(_T("80"));
    m_wndToolBar.m_comboBox.AddString(_T("100"));
    m_wndToolBar.m_comboBox.AddString(_T("200"));
    m_wndToolBar.m_comboBox.AddString(_T("300"));
    m_wndToolBar.m_comboBox.AddString(_T("400"));
    m_wndToolBar.m_comboBox.AddString(_T("600"));
    m_wndToolBar.m_comboBox.AddString(_T("800"));
    m_wndToolBar.m_comboBox.AddString(_T("1000"));
    m_wndToolBar.m_comboBox.AddString(_T("1500"));
    m_wndToolBar.m_comboBox.AddString(_T("2000"));
    m_wndToolBar.m_comboBox.AddString(_T("2500"));
    m_wndToolBar.m_comboBox.AddString(_T("3000"));

	// Select the combo box
    m_wndToolBar.m_comboBox.SelectString(-1,_T("100"));
    //  Create a font for the combobox
    LOGFONT logFont;
    memset(&logFont, 0, sizeof(logFont));
    
    if (!::GetSystemMetrics(SM_DBCSENABLED))
	{
	    // Since design guide says toolbars are fixed height so is the font.
#ifndef _MAC
	    logFont.lfHeight = -12;
#else
	    logFont.lfHeight = -14;     // looks better than 12 on Mac
#endif
	    logFont.lfWeight = FW_BOLD;
	    logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	    _tcscpy(logFont.lfFaceName, _T("MS Sans Serif"));
	    if (!m_wndToolBar.m_font.CreateFontIndirect(&logFont))
		TRACE(_T("Could Not create font for combo\n"));
	    else
		m_wndToolBar.m_comboBox.SetFont(&m_wndToolBar.m_font);
	}
    else
	{
		m_wndToolBar.m_font.Attach(::GetStockObject(SYSTEM_FONT));
		m_wndToolBar.m_comboBox.SetFont(&m_wndToolBar.m_font);
    }

	HWND myHWND = GetSafeHwnd();
	::SetWindowLong(myHWND, GWL_EXSTYLE, GetWindowLong(myHWND, GWL_EXSTYLE) | WS_EX_LAYERED);
	::SetLayeredWindowAttributes(myHWND, 0, 200, LWA_ALPHA);
	return 0;
}

//2012august23, poirier, begin
bool CMainFrame::InitializeAudio()
{
	///////////////////////
	// Initialize portaudio 
	///////////////////////
	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	pLogDocument->AddText("Initializing portaudio library ... \r\n");
	global_PaError = Pa_Initialize();
	if( global_PaError != paNoError ) 
	{
		//goto error;
		Pa_Terminate();
		DisplayAudioError();
		return FALSE;
	}
	else
	{
		pLogDocument->AddText("Initialized OK.\r\n");
	}
	pLogDocument->AddText("Selecting audio device ... \r\n");
	if(SelectAudioDevice()==FALSE) return FALSE;
	pLogDocument->AddText("Device selected OK.\r\n");
	return TRUE;
}

bool CMainFrame::UninitializeAudio()
{
	Pa_Terminate();
	return TRUE;
}

bool CMainFrame::SelectAudioDevice()
{
	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	CString myString;
	//spi, nov 2018, begin
	std::string mystring;
	//spi, nov 2018, end

	global_audiodevicename="E-MU ASIO"; //"E-MU ASIO", "Speakers (2- E-MU E-DSP Audio Processor (WDM))", etc.
	global_outputAudioChannelSelectors[0] = 10; // on emu patchmix ASIO device channel 11 (left)
	global_outputAudioChannelSelectors[1] = 11; // on emu patchmix ASIO device channel 12 (right)

	const PaDeviceInfo* deviceInfo;
    int numDevices = Pa_GetDeviceCount();
    for( int i=0; i<numDevices; i++ )
    {
        deviceInfo = Pa_GetDeviceInfo( i );
		std::string devicenamestring = deviceInfo->name;
		global_devicemap.insert(pair<std::string,int>(devicenamestring,i));
	}

	int deviceid = Pa_GetDefaultOutputDevice(); // default output device 
	map<std::string,int>::iterator it;
	it = global_devicemap.find(global_audiodevicename);
	if(it!=global_devicemap.end())
	{
		deviceid = (*it).second;
		//myString.Format("devicename %s maps to deviceid %d\r\n", global_audiodevicename.c_str(), deviceid);
		//pLogDocument->AddText(myString);
		deviceInfo = Pa_GetDeviceInfo(deviceid);
	}
	else
	{
		for(it=global_devicemap.begin(); it!=global_devicemap.end(); it++)
		{
			//spi, nov 2018, begin
			/*
			myString.Format(L"devicename %s maps to deviceid %d\r\n", (*it).first.c_str(), (*it).second);
			pLogDocument->AddTextW(myString);
			*/
			/*
			myString = (*it).first.c_str();
			pLogDocument->AddTextW(myString);
			pLogDocument->AddTextW(L"\r\n");
			*/
			mystring = "devicename " + (*it).first + " maps to deviceid ";
			mystring += std::to_string((*it).second);
			mystring += "\r\n";
			pLogDocument->AddText(mystring.c_str());
			
			//spi, nov 2018, end
		}
		//Pa_Terminate();
		//return -1;
		pLogDocument->AddText("Error, audio device not found, will use default\r\n");
		deviceid = Pa_GetDefaultOutputDevice();
	}


	global_outputParameters.device = deviceid; 
	if (global_outputParameters.device == paNoDevice) 
	{
		pLogDocument->AddText("Error: No default output device.\r\n");
		Pa_Terminate();
		DisplayAudioError();
		return false;
	}
	else
	{
        deviceInfo = Pa_GetDeviceInfo(global_outputParameters.device);
		//spi, nov 2018, begin
		/*
		myString.Format(L"Device: %s\r\n", deviceInfo->name);
		pLogDocument->AddTextW(myString);
		*/
		mystring = "Device: ";
		mystring += deviceInfo->name;
		mystring += "\r\n";
		pLogDocument->AddText(mystring.c_str());
		//spi, nov 2018, end
	}
	global_outputParameters.channelCount = 2;
	global_outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
	global_outputParameters.suggestedLatency = Pa_GetDeviceInfo( global_outputParameters.device )->defaultLowOutputLatency;
	//inputParameters.hostApiSpecificStreamInfo = NULL;

	//Use an ASIO specific structure. WARNING - this is not portable. 
	//PaAsioStreamInfo asioInputInfo;
	global_asioOutputInfo.size = sizeof(PaAsioStreamInfo);
	global_asioOutputInfo.hostApiType = paASIO;
	global_asioOutputInfo.version = 1;
	global_asioOutputInfo.flags = paAsioUseChannelSelectors;
	global_asioOutputInfo.channelSelectors = global_outputAudioChannelSelectors;
	if(deviceid==Pa_GetDefaultInputDevice())
	{
		global_outputParameters.hostApiSpecificStreamInfo = NULL;
	}
	else if(Pa_GetHostApiInfo(Pa_GetDeviceInfo(deviceid)->hostApi)->type == paASIO) 
	{
		global_outputParameters.hostApiSpecificStreamInfo = &global_asioOutputInfo;
		int iasionumericchannelname_left = global_outputAudioChannelSelectors[0]+1;
		int iasionumericchannelname_right = global_outputAudioChannelSelectors[1]+1;
		myString.Format(L"Channels: %d and %d\r\n", iasionumericchannelname_left, iasionumericchannelname_right);
		pLogDocument->AddTextW(myString);
	}
	else if(Pa_GetHostApiInfo(Pa_GetDeviceInfo(deviceid)->hostApi)->type == paWDMKS) 
	{
		global_outputParameters.hostApiSpecificStreamInfo = NULL;
	}
	else
	{
		//assert(false);
		global_outputParameters.hostApiSpecificStreamInfo = NULL;
	}
	return true;
}

void CMainFrame::DisplayAudioError()
{
	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	pLogDocument->AddText("An error occured while using the portaudio stream\r\n");
	CString myString;
	myString.Format(L"Error number: %d\r\n", global_PaError);
	pLogDocument->AddTextW(myString);
	myString.Format(L"Error message: %s\r\n", Pa_GetErrorText(global_PaError));
	pLogDocument->AddTextW(myString);
}
//2012august23, poirier, end

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::MyUpdatePane ()
{
    m_wndStatusBar.SendMessage(WM_IDLEUPDATECMDUI); // Include afxpriv.h 
    m_wndStatusBar.UpdateWindow();
}                                 

void CMainFrame::MyChangePane(int nPane, LPCTSTR text )
{ 
    m_wndStatusBar.SetPaneText(nPane,text,TRUE) ;
    MyUpdatePane();
}


void CMainFrame::OnUpdatePercent(CCmdUI * pCmdUI)
{
    pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateLevel(CCmdUI * pCmdUI)
{
    pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateLocation(CCmdUI * pCmdUI)
{
    pCmdUI->Enable(TRUE);
    pCmdUI->SetText(_T("foobar"));
}

void CMainFrame::OnUpdatePoint(CCmdUI * pCmdUI)
{
    pCmdUI->Enable(TRUE);
}

void CMainFrame::OnTestPointset() 
{
	/*
    char*   pszFileExtension;
    int     i,ii;
	char	pszCharBuf[255];
	

	CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	pLogDocument->AddText("Will now perform a test of POINTSET Library\r\n");

	
	CTime InitialTime = CTime::GetCurrentTime();

    POINTSET* pPointset = (POINTSET*) NewPointset(POINTSET_INITIAL_NUMBEROFPOINTS);
    if(pPointset==NULL)
    {
		pLogDocument->AddText("     Fatal error, pPointset is NULL\r\n");
		return;
    }



    ///////////////////////
    //now, read data points
    ///////////////////////
	char filename[255];
	strcpy(filename, "img1.xyi");

    // check if .xyi file-format.  note: .nbr are actualy of type .xyi 
    // nbr file format are created by Voronoi for Windows 1.0 CRG App. 
    pszFileExtension = strchr(filename,'.');
    if( pszFileExtension != NULL &&
	( (i=strcmp(pszFileExtension, ".xyi")) == 0 ||
	  (ii=strcmp(pszFileExtension, ".nbr")) == 0
	)
      )
    {
		//
		// open xyzi file and read xyzi header 
		//
		pLogDocument->AddText("     opening as .xyi file-format\r\n");
		if ((xyzi_open_and_readheader(filename)) == FALSE)
		{
			wsprintf(pszCharBuf, "     trimake: ne peut ouvrir le fichier d'entree '%s'\r\n",filename);
		  	pLogDocument->AddText(pszCharBuf);
		  	return;
		}
	
		//
		// validate data file
		//
		if ( xyzi_header.nombre_objet < 3)
		{
		  pLogDocument->AddText("     Fatal error, less than 3 points in file\r\n");
		  return;
		}
		if ( xyzi_header.nombre_objet > POINTSET_INITIAL_NUMBEROFPOINTS)
		{
		  pLogDocument->AddText("     Fatal error, too many points in file\r\n");
		  return;
		}
		if ( xyzi_header.nombre_coor < 2)
		{
		  pLogDocument->AddText("     Fatal error, minimum of 2 coordinates required to create spatialy referenced data structure\r\n");
		  return;
		}
	
		// optional, allocate pPointset->pfStatistics 
		pPointset->nStatPerPoint = xyzi_header.nombre_stat + POINTSET_EXTRA_NUMBEROFSTAT; // POINTSET_EXTRA_NUMBEROFSTAT, +1 for voroarea 
		pPointset->nSizeStatInByte = sizeof(double)
						    				*(xyzi_header.nombre_objet) 
						    				*(pPointset->nStatPerPoint); // +1 for voroarea 
		// *2 for list of merge segments 
		pPointset->pfStatistics = (double*) malloc( (pPointset->nSizeStatInByte) *2 ); // sizeof(float) replaced by *2 
		if( pPointset->pfStatistics == NULL )
		{
		    pLogDocument->AddText("     Fatal error, cannot allocate pfStatistics\r\n");
		    return;
		}
		for(i=0; i<xyzi_header.nombre_objet; i++)
		{
			// read xyzi line 
			if( xyzi_readline() == FALSE)
			{
			    // error 
			    pLogDocument->AddText("     Error reading line from xyzi file\r\n");
			    return;
			}
			pPointset->px[pPointset->npts] = xyzi_line.coor[0];
			pPointset->py[pPointset->npts] = xyzi_line.coor[1];
			// set voronoi area to 0.0 
			pPointset->pfStatistics[i*(pPointset->nStatPerPoint)] = 0.0;
			
			for(ii=0; ii<xyzi_header.nombre_stat; ii++)
			{   
				// transfering all statistics from input file 				
			    pPointset->pfStatistics[i*(pPointset->nStatPerPoint)+ii+POINTSET_EXTRA_NUMBEROFSTAT] = xyzi_line.stat[ii]; // +1 on the left side, because first stat is voroarea 
			}
			// find min/max x-y ranges 
			pPointset->xmin=(pPointset->xmin < pPointset->px[pPointset->npts] ) ? pPointset->xmin : pPointset->px[pPointset->npts];
			pPointset->ymin=(pPointset->ymin < pPointset->py[pPointset->npts] ) ? pPointset->ymin : pPointset->py[pPointset->npts];
			pPointset->xmax=(pPointset->xmax > pPointset->px[pPointset->npts] ) ? pPointset->xmax : pPointset->px[pPointset->npts];
			pPointset->ymax=(pPointset->ymax > pPointset->py[pPointset->npts] ) ? pPointset->ymax : pPointset->py[pPointset->npts];
			pPointset->npts++;
		}
	
		// close xyzi file 
		xyzi_close();
    }
    if(pPointset!=NULL) 
    {
		wsprintf(pszCharBuf, "     nombre de points lus = %d\r\n",pPointset->npts);
    	pLogDocument->AddText(pszCharBuf);
	}


    /////////////////////////////////////////////////
    // Construction du reseau de triangles en memoire
    /////////////////////////////////////////////////
    pLogDocument->AddText("     building triangle network ... \r\n");
    BuildTriangleNetwork(pPointset);
    pLogDocument->AddText("     triangle network built.\r\n");


    
    ////////////////////////////////////////////////////////////////
    // Calcul des centres qui sont les sommets des polygones voronoi
    ////////////////////////////////////////////////////////////////
    
    pLogDocument->AddText("     computing triangle centers ... ");
    ComputeAllTriangleCenters(pPointset);
    pLogDocument->AddText("done.\r\n");
    
    
    /////////////////////////////////////////////////////////////
    // Calcul des statistiques qui sont les sommets des triangles
    /////////////////////////////////////////////////////////////
    
    pLogDocument->AddText("     computing voronoi area and density .. ");
    ComputeVoronoiAreaForAllVertex(pPointset);
    pLogDocument->AddText("done.\r\n");

    pLogDocument->AddText("     computing neighboring voronoi density .. ");
    // ComputeNeighboringVoronoiDensityForAllVertex(pPointset); 
	ApplyAverageFilterToAllVertex(	pPointset, 
									POINTSET_OFFSETSTAT_VORODENSITY,
									POINTSET_OFFSETSTAT_NEIGHVOROAREA, 1);    
    pLogDocument->AddText("done.\r\n");

    pLogDocument->AddText("     computing neighboring tree intensity .. ");
	ApplyAverageFilterToAllVertex(	pPointset, 
									POINTSET_OFFSETSTAT_INTENSITY,
									POINTSET_OFFSETSTAT_NEIGHINTENSITY, 1);    
    pLogDocument->AddText("done.\r\n");
      
    pLogDocument->AddText("     computing neighboring tree area .. ");
	ApplyAverageFilterToAllVertex(	pPointset, 
									POINTSET_EXTRA_NUMBEROFSTAT + POINTSET_XYZI_TREEAREA,
									POINTSET_OFFSETSTAT_NEIGHTREEAREA, 1);    
    pLogDocument->AddText("done.\r\n");
      
	CTime FinalTime = CTime::GetCurrentTime();
	CTimeSpan DiffTime = FinalTime - InitialTime;

	wsprintf(pszCharBuf, "Process terminated successfully in %d min and %d sec.\r\n", DiffTime.GetMinutes(), DiffTime.GetSeconds());
	pLogDocument->AddText(pszCharBuf);

	DeletePointset(pPointset);
	*/

    return;
}

void CMainFrame::OnTestSegmentset() 
{
	return;
}

void CMainFrame::OnTestTreefinder() 
{
	return;
}

/*
//should be set accordinly with macro definition list, bottom of file imglib.h
static char* OFF_image_type_stringtable[] =
{    
	"TIFF_UNCOMPRESSED", 
	"BMP_UNCOMPRESSED", 	
	"PCX",  	
	"TARGA", 
	"GIF", 
	"WPG",
	"WMF",
	"TIFF_HUFFMAN",
	"TIFF_G3_FAX",
	"TIFF_LZW",
	"TIFF_G4_FAX",
	"DCX",
	"BMP_COMPRESSED",
	"JPEG",
	"EPS",
	"PICT",
	"TIFF_PACK",
	"TIFF_2D",
	"CALS",
	"LASER_DATA",
	"XBM",
	"MACPAINT",
	"GX2",
	"KOFAX",
	"IOCA",
	"ICON",
	"IFF_ILBM",
	"CLIP",
	"IMG",
	"BROOK_TROUT",
	"MSP",
	"CUT",
	"TARGA16",
	"CCITT_G3",
	"CCITT_G4",
	"XPM",
	"XWD",
	"RAST",
	"ASCII",
	"PHOTOCD",
	"TIFF_JPEG",
	"PHOTOSHOP",
	"IMNET",
	"MAXFILTERS"
};

static char* OFF_image_fileextension_stringtable[] =
{    
	".TIF", 
	".BMP", 	
	".PCX",  	
	".TGA", //targa
	".GIF", 
	".WPG", //word perfect graphic
	".WMF", //windows meta file
	".TIF",
	".TIF",
	".TIF",
	".TIF",
	".DCX",
	".BMP",
	".JPG",
	".EPS",
	".PIC",
	".TIF",
	".TIF",
	".CAL",
	".LAS",
	".XBM",
	".TIF", //MACPAINT always TIF?
	".GX2",
	".FAX", //kofax
	".IOC", //IOCA
	".ICO", //ICON
	".IFF", //IFF_ILBM
	".CLI", //CLIP
	".IMG",
	".BRO", //BROOK_TROUT
	".MSP",
	".CUT",
	".TGA", //TARGAT 16BIT
	".CG3", //CCITT_G3
	".CG4", //CCITT_G4
	".XPM",
	".XWD",
	".RAS", //RAST
	".ASC", //ASCII
	".PHO", //PHOTOCD
	".TIF", //TIFF_JPEG
	".PSD", //PHOTOSHOP
	".IMN", //IMNET
	".MAX"  //MAXFILTERS
};
*/

/*
char* pszFilter[]= {"BMP Image File (*.bmp)\n", "*.bmp",
					"TIF Image File (*.tif)", "*.tif",
					"XYZI Text Files (*.xyi)","*.xyi",
					"XYZ Text Files (*.xyz)", "*.xyz",
					"SET Segmentation Files (*.set)", "*.set",
					"All Files (*.*)", "*.*", 
					"" };
*/
/*
char* pszFilter[]= {"Image Files (.bmp;.tif;.jpg;.psd)\0*.bmp;*.tif;*.jpg;*.psd\0",
					 "Text Files (.txt;.xyi;.xyz)\0*.txt;*.xyi;*.xyz\0",
					 "Segmentation Files (.set;.seg)\0*.set;*.seg\0",
					 "All Files (*.*)\0*.*\0\0"};
*/

void CMainFrame::OnFileOpen() 
{
	OPENFILENAME ofn;           
	memset(&ofn, 0, sizeof(OPENFILENAME));	//set structure to zero

	/*
	char* pszFilter[]= {"Image Files (*.bmp;*.tif;*.jpg;*.psd)\0*.bmp;*.tif;*.jpg;*.psd\0
		Tiff Files (*.tif;*.tff;*.tiff;*.lzw)\0*.tif;*.tff;*.tiff;*.lzw\0
		All Image Files (*.gif;*.iff;*.tga;*.pcx;*.pic;*.ico;*.pho;...)\0*.gif;*.iff;*.tga;*.pcx;*.pic;*.ico;*.pho;*.cal;*.las;*.xbm;*.ioc;*.cli;*.img;*.bro;*.msp;*.gx2;*.dcx;*.cg3;*.cg4;*.cut;*.xpm;*.xwd;*.ras;*.imn;*.max\0
		Fax Files (*.fax)\0*.fax\0
		Graphic Files (*.eps;*.wpg;*.wmf)\0*.eps;*.wpg;*.wmf\0
		Raw Files (*.raw;*.ascii;*.bin)\0*.raw;*.ascii;*.bin\0	
		Text Files (*.txt;*.xyi;*.xyz)\0*.txt;*.xyi;*.xyz\0
		Segmentation Files (*.set;*.seg)\0*.set;*.seg\0
		All Files (*.*)\0*.*\0
		\0"};
    */
	//char* pszFilter[]= {"Image Files (*.bmp;*.tif;*.jpg;*.psd)\0*.bmp;*.tif;*.jpg;*.psd\0Tiff Files (*.tif;*.tff;*.tiff;*.lzw)\0*.tif;*.tff;*.tiff;*.lzw\0All Image Files (*.gif;*.iff;*.tga;*.pcx;*.pic;*.ico;*.pho;...)\0*.gif;*.iff;*.tga;*.pcx;*.pic;*.ico;*.pho;*.cal;*.las;*.xbm;*.ioc;*.cli;*.img;*.bro;*.msp;*.gx2;*.dcx;*.cg3;*.cg4;*.cut;*.xpm;*.xwd;*.ras;*.imn;*.max\0Fax Files (*.fax)\0*.fax\0Graphic Files (*.eps;*.wpg;*.wmf)\0*.eps;*.wpg;*.wmf\0Raw Files (*.raw;*.ascii;*.bin)\0*.raw;*.ascii;*.bin\0Text Files (*.txt;*.xyi;*.xyz)\0*.txt;*.xyi;*.xyz\0Segmentation Files (*.set;*.seg)\0*.set;*.seg\0All Files (*.*)\0*.*\0\0"};
	WCHAR* pszFilter[]= {L"2D World File (*.ow2)\0*.ow2\0Image Files (*.bmp;*.tif;*.jpg;*.psd)\0*.bmp;*.tif;*.jpg;*.psd\0Tiff Files (*.tif;*.tff;*.tiff;*.lzw)\0*.tif;*.tff;*.tiff;*.lzw\0All Image Files (*.gif;*.iff;*.tga;*.pcx;*.pic;*.ico;*.pho;...)\0*.gif;*.iff;*.tga;*.pcx;*.pic;*.ico;*.pho;*.cal;*.las;*.xbm;*.ioc;*.cli;*.img;*.bro;*.msp;*.gx2;*.dcx;*.cg3;*.cg4;*.cut;*.xpm;*.xwd;*.ras;*.imn;*.max\0Fax Files (*.fax)\0*.fax\0Graphic Files (*.eps;*.wpg;*.wmf)\0*.eps;*.wpg;*.wmf\0Raw Files (*.raw;*.ascii;*.bin)\0*.raw;*.ascii;*.bin\0Text Files (*.txt;*.xyi;*.xyz)\0*.txt;*.xyi;*.xyz\0Segmentation Files (*.set;*.seg)\0*.set;*.seg\0All Files (*.*)\0*.*\0\0"};
	
	WCHAR pszPathName[1024];
	WCHAR pszFileName[1024];			
	WCHAR pszFileTitle[1024];

	wcscpy(pszPathName, L"");		
	wcscpy(pszFileName, L"");		
	wcscpy(pszFileTitle, L"");		

	ofn.lStructSize       = sizeof (OPENFILENAME);
	ofn.hwndOwner         = AfxGetApp()->m_pMainWnd->GetSafeHwnd();
	ofn.lpstrFilter       = pszFilter[0];
	ofn.nFilterIndex      = 1;
	ofn.lpstrFile         = pszFileName;
	ofn.nMaxFile          = sizeof (pszFileName);
	ofn.lpstrFileTitle    = pszFileTitle;
	ofn.nMaxFileTitle     = sizeof (pszFileTitle);
	ofn.lpstrInitialDir   = L""; //pszPathName 
	ofn.lpstrTitle		  = L"Open File";
	ofn.Flags             = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt		  = L"";	

    
    if (GetOpenFileName(&ofn))
 	{
   		//CStdioFile cfile(ofn.lpstrFile, CFile::modeRead);
		//pSIDoc->SetTitle(ofn.lpstrFile);
        //pSIDoc->OnOpenDocument(ofn.lpstrFile);

		((COIIIApp*)AfxGetApp())->m_pOW2DocTemplate->OpenDocumentFile(ofn.lpstrFile);
		return;  // if it successes
   	}

	return; //user canceled, return safely
}

void CMainFrame::OnFileAcquire() 
{
	//int imghandle = IMG_scan_acquire(this->GetSafeHwnd(), TRUE);
	
}

void CMainFrame::OnUpdateFileAcquire(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	//before resizing

	CMDIFrameWnd::OnSize(nType, cx, cy);
	
	if(((COIIIApp*)AfxGetApp())->IsLogDocumentPresent())
	{
		//after resizing
		CLogDocument* pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
		POSITION pos = pLogDocument->GetFirstViewPosition();
		CLogEditView* pLogView = (CLogEditView*) pLogDocument->GetNextView(pos);
		CMDIChildWnd* pMDIChildWnd = (CMDIChildWnd*) pLogView->GetParentFrame();

		//get mainframe window rect
		CRect AppRect, ClientAppRect;
		AfxGetApp()->m_pMainWnd->GetWindowRect(&AppRect);
		AfxGetApp()->m_pMainWnd->GetClientRect(&ClientAppRect);
		ClientToScreen(&ClientAppRect);
		//get child window rect
		CRect ChildRect;
		pMDIChildWnd->GetWindowRect(&ChildRect);
		CRect NewChildRect(ChildRect);
		//if(nType==SIZE_MAXIMIZED) 
		if(nType==SIZE_MAXIMIZED || nType==SIZE_RESTORED) 
		{
			//NewChildRect.OffsetRect(AppRect.right-ChildRect.right, AppRect.bottom-ChildRect.bottom-((AppRect.Height()-ClientAppRect.Height())));
			NewChildRect.OffsetRect(AppRect.right-AppRect.left-ChildRect.right, AppRect.bottom-ChildRect.bottom-((AppRect.Height()-ClientAppRect.Height())));
			pMDIChildWnd->MoveWindow(NewChildRect);
		}
	}
}

void CMainFrame::OnViewLogwindow() 
{
	if(((COIIIApp*)AfxGetApp())->IsLogDocumentPresent())
	{
		//close log window
		CLogDocument* pLogDoc = ((COIIIApp*)AfxGetApp())->GetLogDocument();
		pLogDoc->OnCloseDocument();
	}
	else
	{
		//open log window
		((COIIIApp*)AfxGetApp())->GetLogDocument();
	}
	return;
}

void CMainFrame::OnUpdateViewLogwindow(CCmdUI* pCmdUI) 
{	
	if(((COIIIApp*)AfxGetApp())->IsLogDocumentPresent()) pCmdUI->SetCheck(TRUE);
	  else pCmdUI->SetCheck(FALSE);
}


void CMainFrame::OnDestroy()
{
	//in workspace coordinate
	CRect myRect;
	GetWindowRect(&myRect);
	StoreToINI( myRect.left, myRect.top, myRect.Width(), myRect.Height());

	CMDIFrameWnd::OnDestroy();
	// TODO: Add your message handler code here
}

// get the globals from the INI/Registry
void CMainFrame::RestoreFromINI()
{
	CString szSection("Settings");
	CString szEntry;
	CString strBuffer;
	m_bInitCoorValid = FALSE;

	szEntry = "x";
	strBuffer = AfxGetApp()->GetProfileString(szSection, szEntry);
	if (strBuffer.IsEmpty()) return;
	m_ix = _wtol(strBuffer);
	szEntry = "y";
	strBuffer = AfxGetApp()->GetProfileString(szSection, szEntry);
	if (strBuffer.IsEmpty()) return;
	m_iy = _wtol(strBuffer);
	szEntry = "cx";
	strBuffer = AfxGetApp()->GetProfileString(szSection, szEntry);
	if (strBuffer.IsEmpty()) return;
	m_icx = _wtol(strBuffer);
	szEntry = "cy";
	strBuffer = AfxGetApp()->GetProfileString(szSection, szEntry);
	if (strBuffer.IsEmpty()) return;
	m_icy = _wtol(strBuffer);
	CRect myMainWndRect(m_ix, m_iy, m_ix+m_icx, m_iy+m_icy);

	CRect myDesktopRect;
	int nmonitor = GetSystemMetrics(SM_CMONITORS);
	int nscale = GetSystemMetrics(SM_SAMEDISPLAYFORMAT);
	myDesktopRect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	myDesktopRect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
	myDesktopRect.right = myDesktopRect.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
	myDesktopRect.bottom = myDesktopRect.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);
 
	if(myDesktopRect.PtInRect(myMainWndRect.TopLeft())) 
	{
		if(myDesktopRect.PtInRect(myMainWndRect.BottomRight())) m_bInitCoorValid = TRUE;
	}
}

void CMainFrame::StoreToINI(int x, int y, int cx, int cy)
{
	CString szSection("Settings");
	CString szEntry;
	CString strBuffer;

	szEntry = "x";
	strBuffer.Format(L"%d", x);
	AfxGetApp()->WriteProfileString(szSection, szEntry, strBuffer);
	szEntry = "y";
	strBuffer.Format(L"%d", y);
	AfxGetApp()->WriteProfileString(szSection, szEntry, strBuffer);
	szEntry = "cx";
	strBuffer.Format(L"%d", cx);
	AfxGetApp()->WriteProfileString(szSection, szEntry, strBuffer);
	szEntry = "cy";
	strBuffer.Format(L"%d", cy);
	AfxGetApp()->WriteProfileString(szSection, szEntry, strBuffer);
	return;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if(cs.hInstance && m_bInitCoorValid) 
	{
		cs.x = m_ix;
		cs.y = m_iy;
		cs.cx = m_icx;
		cs.cy = m_icy;
	}

	return CMDIFrameWnd::PreCreateWindow(cs);
}


void CMainFrame::OnTransportPlay()
{
	// TODO: Add your command handler code here
}


void CMainFrame::OnUpdateTransportPlay(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CMainFrame::OnTransportPause()
{
	// TODO: Add your command handler code here
}


void CMainFrame::OnUpdateTransportPause(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CMainFrame::OnTransportStop()
{
	// TODO: Add your command handler code here
}


void CMainFrame::OnUpdateTransportStop(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

BOOL CMainFrame::CreateAllAudioThreads()
{
	CLogDocument* pLogDoc = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	////////////////////
	//create all threads
	////////////////////
    global_lpThreadHANDLE[MAINFRM_PLAYING_THREAD]=CreateThread(NULL, 0, 
        (LPTHREAD_START_ROUTINE)playingThrdFunc, 
        (LPVOID)0, 0, &global_lpThreadID[MAINFRM_PLAYING_THREAD]);
    if (!global_lpThreadHANDLE[MAINFRM_PLAYING_THREAD])
    {
        pLogDoc->AddText("Error creating playing thread,,,,.exiting");
        return FALSE;
    }
    Sleep(100);
    global_lpThreadHANDLE[MAINFRM_SWAPPING_THREAD]=CreateThread(NULL, 0, 
        (LPTHREAD_START_ROUTINE)swappingThrdFunc, 
        (LPVOID)1, 0, &global_lpThreadID[MAINFRM_SWAPPING_THREAD]);
    if (!global_lpThreadHANDLE[MAINFRM_SWAPPING_THREAD])
    {
        pLogDoc->AddText("Error creating swapping thread,,,,.exiting");
        return FALSE;
    }
    Sleep(100);
    global_lpThreadHANDLE[MAINFRM_SUMMING_THREAD]=CreateThread(NULL, 0, 
        (LPTHREAD_START_ROUTINE)summingThrdFunc, 
        (LPVOID)2, 0, &global_lpThreadID[MAINFRM_SUMMING_THREAD]);
    if (!global_lpThreadHANDLE[MAINFRM_SUMMING_THREAD])
    {
        pLogDoc->AddText("Error creating summing thread,,,,.exiting");
        return FALSE;
    }
    Sleep(100);
	return TRUE;
}

BOOL CMainFrame::CloseAllAudioThreads()
{
	PostThreadMessage(global_lpThreadID[MAINFRM_PLAYING_THREAD], OWM_PLAYING_EXIT, 0 , 0);
	WaitForMultipleObjects(MAINFRM_NUMTHREADS, global_lpThreadHANDLE, true, INFINITE);
	CloseHandle(global_lpThreadHANDLE[MAINFRM_PLAYING_THREAD]);
	CloseHandle(global_lpThreadHANDLE[MAINFRM_SWAPPING_THREAD]);
	CloseHandle(global_lpThreadHANDLE[MAINFRM_SUMMING_THREAD]);
	return TRUE;
}