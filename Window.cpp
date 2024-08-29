

/*

		PROJECT NAME : Sine Wave Simulator
		Developed By : Santosh Balu Phatangare
		Email : santoshphatangare12@gmail.com
		Technologies used : Win32 SDK
							COM (Component Object Model)
							SAPI (Speach API)
					
		Language : C++ Programming Language

*/

#include <windows.h>
#include <stdio.h>
#include <sapi.h>
#include <ctype.h>
#include <math.h>
#include <tchar.h>
#include <wchar.h>
#include <sphelper.h>

#include "Window.h"
#include "ClassFactoryDllServerHomework.h"


#define SBP_TIMER 100
#define TEXT_FILE_NUM_CHARACTER 2048
#define TIME_INTERVAL_IN_MS 1
#define NUM_POINTS 800
#define MAX_MATH 2000
#define POINTSIZE 20
#define MIN_FREQ = 12.0
#define MAX_FREQ = 5.0

//Declaring Interface Pointers
//ICircle *pICircle = NULL;
ISpVoice* pISpVoice = NULL; 
ISineWave* pISineWave = NULL; 

HWND hListBox;
HWND gHwnd = NULL;

HWND hwnd; 


HRESULT hr;

ISpObjectToken* voices[2];
//ISpObjectToken* pISpObjectToken = NULL;
ISpObjectToken* pISpObjectToken1 = NULL;
ISpObjectToken* pISpObjectToken2 = NULL;

IEnumSpObjectTokens * pIEnumSpObjectTokens = NULL; 

ISpStream* pISpStream = NULL;
CSpStreamFormat audioFormat; 
//int NUM_POINTS = 500;

WCHAR wszFirstName[265];
WCHAR wszSurname[265];
CHAR wszWavFileName[512];

WCHAR* descriptionString = NULL;

TCHAR szFileName[512];
TCHAR szFirstName[256];
TCHAR szSurname[256];
 
TCHAR str[255]; 

CHAR textBuffer[TEXT_FILE_NUM_CHARACTER];
TCHAR textBuffer_WideChar[TEXT_FILE_NUM_CHARACTER];

FILE* LogFile = NULL;


//unsigned int pointSize = 10;

static int x = 0;
int plotPointX[NUM_POINTS];
int plotPointY[NUM_POINTS];
int finalPlotPointX[NUM_POINTS];
int finalPlotPointY[NUM_POINTS];
int iPaintFlag = 0;
int selectedVoiceIndex = -1;
//int lbItem = 0;


INT noOfWaves = 1;
INT PointCount = 500;
DOUBLE noOfFreq = 5.0;
DOUBLE amplitude = 50.0;

//FLOAT noOfFreq = MAX_FREQ;

double phase;
double angle = 0.0;

//int retVal;





BOOL g_UserRegistered = FALSE;
BOOL g_dataRegister = FALSE; 
BOOL g_bRadioButtonChecked = FALSE;
BOOL g_CheckBoxChecked = FALSE;
BOOL g_ListBox= FALSE;
BOOL soundOff = FALSE;
BOOL g_LogFileRead = FALSE;
BOOL hasExportesWav = FALSE;
BOOL myFlag = 0; 
BOOL setCheck = TRUE;
BOOL bAudioLog = FALSE;

BOOL EnableDlgFlagButtons = FALSE;
BOOL EnableExportRadioButton = FALSE;
BOOL ExportAudioLog = FALSE;
BOOL isChecked = FALSE;
int isChecked1 = 0;
USHORT CurrentVolume = 0.0f;

BOOL convertTextToSpeech = FALSE; 

int SetVoiceForAudioLog = 0;

char filenamewav[100];


SYSTEMTIME localTime;
//GetLocalTime (&localTime); 


COLORREF g_chosencolor = RGB(255,165, 0);


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ; 
BOOL CALLBACK MyDlgProc(HWND, UINT, WPARAM, LPARAM) ;
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK RegisterDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL ValidateRegistration(const TCHAR* str);

void functionTextToSpeech(void);
void safeRelease(void);

//ULONG numVoices = 0l;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) 

{
	static TCHAR szAppName[] = TEXT ("MyWindow") ; 
	MSG msg ; 
	//HWND hwnd ; 
	HRESULT hr = S_OK;
	WNDCLASSEX wndclass ;

	//Code

	

	//Initialising COM
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("COM Library can not be Initialized.\nProgram Will Now Exit."), TEXT("Program Error"), MB_OK);
		exit(0);
	}
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW ; 
	wndclass.lpfnWndProc =WndProc ; 
	wndclass.cbClsExtra = 0 ; 
	wndclass.cbWndExtra = 0 ; 
	wndclass.hInstance =hInstance ; 
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(SBP_ICON));
	wndclass.hIconSm = LoadIcon (hInstance, MAKEINTRESOURCE(SBP_ICON)) ;
	wndclass.hCursor =LoadCursor (NULL, IDC_ARROW) ; 
	wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH) ; 
	wndclass.lpszMenuName = NULL ; 
	wndclass.lpszClassName =szAppName ; 
	
	RegisterClassEx(&wndclass);
	
	
	
	hwnd = CreateWindow(szAppName, 
		TEXT ("Santosh Phatangare : Sine Wave Application"), 
		WS_OVERLAPPEDWINDOW,
		0, 
		0, 
		640, 
		400, 
		NULL,
		NULL, 
		hInstance, 
		NULL) ; 
		
	gHwnd = hwnd;

		ShowWindow (hwnd, iCmdShow) ; 
		UpdateWindow (hwnd) ; 
		
		while(GetMessage (&msg, NULL, 0, 0)) 
		
		{ 
			TranslateMessage (&msg) ; 
			DispatchMessage (&msg) ; 
		}
		CoUninitialize();
	return msg.wParam ; 
} 



LRESULT CALLBACK WndProc (HWND hwnd, UINT message,WPARAM wParam, LPARAM lParam) 
{ 
	//variable declaration
	HRESULT hr = S_OK; 
	//HRESULT hrvoice = S_OK;
	HINSTANCE hInst = NULL;
	HDC hdc = NULL;
	HBRUSH hBrush = NULL;
	TCHAR str[255];
	ULONG numVoices = 0l;
	WCHAR* DescriptionString = NULL;
	//CSpStreamFormat audioFormat; 
	CHAR textBuffer[TEXT_FILE_NUM_CHARACTER]; 
	TCHAR textBuffer_WideChar[TEXT_FILE_NUM_CHARACTER]; 

	static RECT rect; 
	static int centerX = 0;
	static int centerY = 0;
	
	int x, y;
	PAINTSTRUCT ps;
	int pointX = 0;
	int pointY = 0;

	

	switch (message) 
	{ 
	case WM_CREATE:

		hr = CoCreateInstance(CLSID_ICircleSineWave, NULL, CLSCTX_INPROC_SERVER, IID_ISineWave, (void**)&pISineWave);
		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("ISineWave Interface can not be Obtained"), TEXT("ERROR"), MB_OK);
			DestroyWindow(hwnd);
		}


		hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void**)&pISpVoice);
		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("ISpVoice Interface can not be Obtained"), TEXT("ERROR"), MB_ICONERROR);
			DestroyWindow(hwnd);
		}
		
		
		pISpVoice->Speak(L"Press The Space Bar for Wave Setting ", SPF_ASYNC, NULL);

		for (int i = 0; i < NUM_POINTS; i++)
		{
			
			 plotPointX[i] = -(i * POINTSIZE);
		}

		for (int i = 0; i < NUM_POINTS; i++)
		{
			plotPointY[i] = -(i * POINTSIZE);
		}

		GetClientRect(hwnd, &rect);
		x = rect.left;

		SetTimer(hwnd, SBP_TIMER, TIME_INTERVAL_IN_MS, NULL);
		break;

	case WM_PAINT:
		//Get the Painter :HDC
		hdc = BeginPaint(hwnd, &ps);

		//Create Brush of Desired Color
		hBrush = CreateSolidBrush(g_chosencolor);

		//Give this brush to the Painter
		SelectObject(hdc, hBrush);

		switch (noOfWaves)
		{
		case 1:
			for (int i = 0; i < NUM_POINTS; i++)
			{
				phase = ((double)i / noOfFreq);
				pISineWave->PointOnSineWave(amplitude, angle, phase, &plotPointX[i], &plotPointY[i]);
				finalPlotPointX[i] = plotPointX[i];
				finalPlotPointY[i] = centerY + plotPointY[i] + (0 * WAVE_LEVEL_Y_MULTIPLE);
				if (i < PointCount)
				{
					finalPlotPointY[i] = centerY + plotPointY[i] + (0 * WAVE_LEVEL_Y_MULTIPLE);
					Ellipse(hdc, finalPlotPointX[i], finalPlotPointY[i], finalPlotPointX[i] + POINTSIZE, finalPlotPointY[i] + POINTSIZE);
				}
			}
			break;
		

		case 2:
			for (int i = 0; i < NUM_POINTS; i++)
			{
				phase = ((double)i / noOfFreq);
				pISineWave->PointOnSineWave(amplitude, angle, phase, &plotPointX[i], &plotPointY[i]);
				finalPlotPointX[i] = plotPointX[i];
				finalPlotPointY[i] = centerY + plotPointY[i] + (0 * WAVE_LEVEL_Y_MULTIPLE);
				if (i < PointCount)
				{
					finalPlotPointY[i] = centerY + plotPointY[i] + (0 * WAVE_LEVEL_Y_MULTIPLE);
					Ellipse(hdc, finalPlotPointX[i], finalPlotPointY[i], finalPlotPointX[i] + POINTSIZE, finalPlotPointY[i] + POINTSIZE);

					finalPlotPointY[i] = centerY + plotPointY[i] + (1 * WAVE_LEVEL_Y_MULTIPLE);
					Ellipse(hdc, finalPlotPointX[i], finalPlotPointY[i], finalPlotPointX[i] + POINTSIZE, finalPlotPointY[i] + POINTSIZE);

				}
			}
			break;

		case 3:
			for (int i = 0; i < NUM_POINTS; i++)
			{
				phase = ((double)i / noOfFreq);
				pISineWave->PointOnSineWave(amplitude, angle, phase, &plotPointX[i], &plotPointY[i]);
				finalPlotPointX[i] = plotPointX[i];
				finalPlotPointY[i] = centerY + plotPointY[i] + (0 * WAVE_LEVEL_Y_MULTIPLE);
				if (i < PointCount)
				{
					finalPlotPointY[i] = centerY + plotPointY[i] + (0 * WAVE_LEVEL_Y_MULTIPLE);
					Ellipse(hdc, finalPlotPointX[i], finalPlotPointY[i], finalPlotPointX[i] + POINTSIZE, finalPlotPointY[i] + POINTSIZE);

					finalPlotPointY[i] = centerY + plotPointY[i] + (1 * WAVE_LEVEL_Y_MULTIPLE);
					Ellipse(hdc, finalPlotPointX[i], finalPlotPointY[i], finalPlotPointX[i] + POINTSIZE, finalPlotPointY[i] + POINTSIZE);

					finalPlotPointY[i] = centerY + plotPointY[i] + (2 * WAVE_LEVEL_Y_MULTIPLE);
					Ellipse(hdc, finalPlotPointX[i], finalPlotPointY[i], finalPlotPointX[i] + POINTSIZE, finalPlotPointY[i] + POINTSIZE);

				}
			}
			break;

		case 4:
			for (int i = 0; i < NUM_POINTS; i++)
			{
				phase = ((double)i / noOfFreq);
				pISineWave->PointOnSineWave(amplitude, angle, phase, &plotPointX[i], &plotPointY[i]);
				finalPlotPointX[i] = plotPointX[i];
				finalPlotPointY[i] = centerY + plotPointY[i] + (0 * WAVE_LEVEL_Y_MULTIPLE);
				if (i < PointCount)
				{
					finalPlotPointY[i] = centerY + plotPointY[i] + (0 * WAVE_LEVEL_Y_MULTIPLE);
					Ellipse(hdc, finalPlotPointX[i], finalPlotPointY[i], finalPlotPointX[i] + POINTSIZE, finalPlotPointY[i] + POINTSIZE);

					finalPlotPointY[i] = centerY + plotPointY[i] + (1 * WAVE_LEVEL_Y_MULTIPLE);
					Ellipse(hdc, finalPlotPointX[i], finalPlotPointY[i], finalPlotPointX[i] + POINTSIZE, finalPlotPointY[i] + POINTSIZE);

					finalPlotPointY[i] = centerY + plotPointY[i] + (2 * WAVE_LEVEL_Y_MULTIPLE);
					Ellipse(hdc, finalPlotPointX[i], finalPlotPointY[i], finalPlotPointX[i] + POINTSIZE, finalPlotPointY[i] + POINTSIZE);

					finalPlotPointY[i] = centerY + plotPointY[i] + (-1 * WAVE_LEVEL_Y_MULTIPLE);
					Ellipse(hdc, finalPlotPointX[i], finalPlotPointY[i], finalPlotPointX[i] + POINTSIZE, finalPlotPointY[i] + POINTSIZE);

				}
			}
			break;

		case 5:
			for (int i = 0; i < NUM_POINTS; i++)
			{
				phase = ((double)i / noOfFreq);
				pISineWave->PointOnSineWave(amplitude, angle, phase, &plotPointX[i], &plotPointY[i]);
				finalPlotPointX[i] = plotPointX[i];
				finalPlotPointY[i] = centerY + plotPointY[i] + (0 * WAVE_LEVEL_Y_MULTIPLE);
				if (i < PointCount)
				{
					finalPlotPointY[i] = centerY + plotPointY[i] + (0 * WAVE_LEVEL_Y_MULTIPLE);
					Ellipse(hdc, finalPlotPointX[i], finalPlotPointY[i], finalPlotPointX[i] + POINTSIZE, finalPlotPointY[i] + POINTSIZE);

					finalPlotPointY[i] = centerY + plotPointY[i] + (1 * WAVE_LEVEL_Y_MULTIPLE);
					Ellipse(hdc, finalPlotPointX[i], finalPlotPointY[i], finalPlotPointX[i] + POINTSIZE, finalPlotPointY[i] + POINTSIZE);

					finalPlotPointY[i] = centerY + plotPointY[i] + (2 * WAVE_LEVEL_Y_MULTIPLE);
					Ellipse(hdc, finalPlotPointX[i], finalPlotPointY[i], finalPlotPointX[i] + POINTSIZE, finalPlotPointY[i] + POINTSIZE);

					finalPlotPointY[i] = centerY + plotPointY[i] + (-1 * WAVE_LEVEL_Y_MULTIPLE);
					Ellipse(hdc, finalPlotPointX[i], finalPlotPointY[i], finalPlotPointX[i] + POINTSIZE, finalPlotPointY[i] + POINTSIZE);

					finalPlotPointY[i] = centerY + plotPointY[i] + (-2 * WAVE_LEVEL_Y_MULTIPLE);
					Ellipse(hdc, finalPlotPointX[i], finalPlotPointY[i], finalPlotPointX[i] + POINTSIZE, finalPlotPointY[i] + POINTSIZE);

				}
			}
			break;

		default:
			break;

		 }

		

		//Color the Window Client Area with the selected Brush 
		//FillRect(hdc, &rect, hBrush);

		//Step 6:Release the Brush
		if (hBrush)
		{
			DeleteObject(hBrush);
			hBrush = NULL;
		}

		//Step 7:End Paint
			//EndPaint(hwnd, &ps);
		if (hdc)
		{

			EndPaint(hwnd, &ps);
			hdc = NULL;
		}


		break;
	case WM_TIMER:
		KillTimer(hwnd, SBP_TIMER);

		//Animate the point along a Circular path
		angle = angle + 0.05;
		if (angle > 360.0)
		{
			angle= angle + 0.0;
		}
		if ((angle * 20) > rect.right)
		{
			angle = 0.0;
		}

		//  Repaint / Update
		InvalidateRect(hwnd, NULL, TRUE);

		SetTimer(hwnd, SBP_TIMER, TIME_INTERVAL_IN_MS, NULL);

		break;

	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);

			break;

		case VK_SPACE:
			hInst = ((HINSTANCE)(GetWindowLongPtr(hwnd, GWLP_HINSTANCE)));

			DialogBox(hInst, TEXT("SELECTCOLOR"), hwnd, (DLGPROC)MyDlgProc);

			break;

		default:
			break;

		}
		break;



	case WM_SIZE:
		//step 2:Get the Client Area Rectangle :RECT
		GetClientRect(hwnd, &rect);
		x = rect.left;
		centerX = (rect.right - rect.left) / 2;
		centerY = (rect.bottom - rect.top) / 2;
		
		break;

	

	case WM_DESTROY: 
		
		SYSTEMTIME localTime; 
		GetLocalTime(&localTime);

		fprintf(LogFile, "\n User Quit the Application at Time : %0.2d Hours %0.2d Minutes %0.2d Seconds", localTime.wHour, localTime.wMinute, localTime.wSecond);

		fclose(LogFile);
		LogFile = NULL; 

		if (convertTextToSpeech) 
		{
			functionTextToSpeech();  
		}
		
		
		void safeRelease(void);
		PostQuitMessage(0);

		
		break;

	default:
		break;
			
	} 
	return DefWindowProc (hwnd, message, wParam, lParam) ;
} 



BOOL CALLBACK MyDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	HINSTANCE hInst = NULL;
	HWND hListBox;
	HWND ListBox;
	HINSTANCE hDlgReg;
	HINSTANCE hDlgAbt;
	TCHAR str[255];
	int retVal;

	HBRUSH hBrush = NULL;
	HBRUSH hbrBkgnd = NULL;

	CSpStreamFormat audioFormat;
	CHAR textBuffer[TEXT_FILE_NUM_CHARACTER];
	TCHAR textBuffer_WideChar[TEXT_FILE_NUM_CHARACTER];;
	ULONG numVoices = 0l;
	WCHAR* DescriptionString = NULL;

	BOOL setCheck = TRUE;
	HWND hCheckBox;
	HWND hwndCaption;
	HWND hwndGroupBox;
	HDC hdc;



	/*hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void**)&pISpVoice);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("ISpVoice Interface can not be Obtained"), TEXT("ERROR"), MB_OK);
		DestroyWindow(hwnd);
	}*/

	switch (message)
	{

	case WM_INITDIALOG:

		/*if (g_UserRegistered == FALSE)
		{
			pISpVoice->Speak(L"Register To Continue Setting ", SPF_ASYNC, NULL);
		}*/
		

		EnableWindow(GetDlgItem(hDlg, ID_RED), g_UserRegistered);
		EnableWindow(GetDlgItem(hDlg, ID_GREEN), g_UserRegistered);
		EnableWindow(GetDlgItem(hDlg, ID_BLUE), g_UserRegistered);
		EnableWindow(GetDlgItem(hDlg, ID_CYAN), g_UserRegistered);
		EnableWindow(GetDlgItem(hDlg, ID_MAGENTA), g_UserRegistered);
		EnableWindow(GetDlgItem(hDlg, ID_YELLOW), g_UserRegistered);
		EnableWindow(GetDlgItem(hDlg, ID_WHITE), g_UserRegistered);
		EnableWindow(GetDlgItem(hDlg, ID_GRAY), g_UserRegistered);
		EnableWindow(GetDlgItem(hDlg, ID_VIOLET), g_UserRegistered);
		EnableWindow(GetDlgItem(hDlg, ID_ORANGE), g_UserRegistered);

		EnableWindow(GetDlgItem(hDlg, ID_INCREASEPOINTS), g_UserRegistered);
		EnableWindow(GetDlgItem(hDlg, ID_INCREASEWAVES), g_UserRegistered);
		EnableWindow(GetDlgItem(hDlg, ID_INCREASEAMPLITUDES), g_UserRegistered);
		EnableWindow(GetDlgItem(hDlg, ID_INCREASEFREQUENCY), g_UserRegistered);

		EnableWindow(GetDlgItem(hDlg, ID_DECREASEPOINTS), g_UserRegistered);
		EnableWindow(GetDlgItem(hDlg, ID_DECREASEWAVES), g_UserRegistered);
		EnableWindow(GetDlgItem(hDlg, ID_DECREASEAMPLITUDES), g_UserRegistered);
		EnableWindow(GetDlgItem(hDlg, ID_DECREASEFREQUENCY), g_UserRegistered);
		
		EnableWindow(GetDlgItem(hDlg, ID_LISTBOX), g_UserRegistered); 
		EnableWindow(GetDlgItem(hDlg, ID_LISTBOXEXPORT), g_UserRegistered);
		EnableWindow(GetDlgItem(hDlg, ID_RESTARTWAVE), g_UserRegistered); 

		EnableWindow(GetDlgItem(hDlg, ID_REG_USER), !g_UserRegistered);   




		hr = pISpVoice->SetVolume(0.0f);
		
		SendDlgItemMessage(hDlg, ID_VOLUME_OFF, BM_SETCHECK, 1, 0);

		//Disable Export Audio log Button Initially
		EnableExportRadioButton = FALSE;

		//Get a List Of Voice Tokens
		hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &pIEnumSpObjectTokens);
		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("IEnumSpObjectTokens Interface can not be Obtained"), TEXT("ERROR"), MB_OK);
			//DestroyWindow(hwnd);
		}

		//From the list ofvoice tokens get the next voice tokens
		pIEnumSpObjectTokens->Next(1, &pISpObjectToken1, NULL);
		SpGetDescription(pISpObjectToken1, &descriptionString);

		//List  Box Control
		hListBox = GetDlgItem(hDlg, ID_LISTBOX); 
		SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM) "Microsoft David"); 
		SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM) "Microsoft Zira"); 

		//Set the current voice to the voice given in the above obtained Tokens
		pISpVoice->SetVoice(pISpObjectToken1);

		pIEnumSpObjectTokens->Next(1, &pISpObjectToken2, NULL);
		SpGetDescription(pISpObjectToken2, &descriptionString); 


		hListBox = GetDlgItem(hDlg, ID_LISTBOXEXPORT);
		SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM) "Microsoft David");
		SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM) "Microsoft Zira");

		
		SetTimer(hDlg, 1, 1, NULL);
		return TRUE;
		break;

		

	case WM_CTLCOLORDLG:
	{
		HDC hdcDlg = (HDC)wParam;
		SetBkColor(hdcDlg, RGB(135, 206, 235));
		return(INT_PTR)CreateSolidBrush(RGB(135, 206, 235));
	}

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		//SetTextColor(hdcStatic, RGB(173, 216, 230));
		SetBkColor(hdcStatic, RGB(135, 206, 235));

		if (hBrush == NULL)
		{
			hBrush = CreateSolidBrush(RGB(135, 206, 235));
		}
		return((INT_PTR)hBrush);

	}

	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED)
		{
			switch (LOWORD(wParam))
			{
			case ID_VOLUME_ON:
				hr = pISpVoice->SetVolume(100.0f);
				SendDlgItemMessage(hDlg, ID_VOLUME_ON, BM_SETCHECK, TRUE, 0);
				SendDlgItemMessage(hDlg, ID_VOLUME_OFF, BM_SETCHECK, FALSE, 0);
				EnableWindow(GetDlgItem(hDlg, ID_LISTBOX), TRUE);
				EnableExportRadioButton = TRUE;
				//fprintf(LogFile, "Audio On\n");
				break;

			case ID_VOLUME_OFF:
				hr = pISpVoice->SetVolume(0.0f);
				SendDlgItemMessage(hDlg, ID_VOLUME_OFF, BM_SETCHECK, TRUE, 0);
				SendDlgItemMessage(hDlg, ID_VOLUME_ON, BM_SETCHECK, FALSE, 0);
				EnableWindow(GetDlgItem(hDlg, ID_LISTBOX), FALSE);
				EnableExportRadioButton = FALSE;
				//fprintf(LogFile, "Audio Off\n");
				break;

			}
		}
		

		switch (LOWORD(wParam))
		{
		case ID_REG_USER:

			hDlgReg = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
			//DialogBox(hDlgReg,MAKEINTRESOURCE(ID_DIALOG_REGISTER),hwnd, (DLGPROC)RegisterDlgProc);
			DialogBox(hInst, TEXT("REGISTER"), hDlg, (DLGPROC)RegisterDlgProc);
			SendMessage(hDlg, WM_INITDIALOG, 0, 0);
			if (g_UserRegistered)
			{
				//pISpVoice->Speak(L"Registration Done Successfully", SPF_ASYNC, NULL);
				EnableWindow(GetDlgItem(hDlg, ID_REG_USER), FALSE); 
			}
			break;

		case ID_RED:
			g_chosencolor = RGB(255, 0, 0);
			
			fprintf(LogFile, "Changing Color To Red\n");
			pISpVoice->Speak(L"Changing Color To Red", SPF_ASYNC, NULL);

			break;

		case ID_GREEN:
			//MessageBox(hDlg, TEXT("about"), TEXT("ok"), MB_OK);
			g_chosencolor = RGB(0, 255, 0);
			
			fprintf(LogFile, "Changing Color To Green\n");
			pISpVoice->Speak(L"Changing Color To Green ", SPF_ASYNC, NULL);
			break;

		case ID_BLUE:
			//MessageBox(hDlg, TEXT("about"), TEXT("ok"), MB_OK);
			g_chosencolor = RGB(0, 0, 255);
			
			fprintf(LogFile, "Changing Color To Blue\n");
			pISpVoice->Speak(L"Changing Color To Blue", SPF_ASYNC, NULL);
			break;

		case ID_CYAN:
			//MessageBox(hDlg, TEXT("about"), TEXT("ok"), MB_OK);
			g_chosencolor = RGB(0, 255, 255);
			
			fprintf(LogFile, "Changing Color To Cyan\n");
			pISpVoice->Speak(L"Changing Color To Cyan", SPF_ASYNC, NULL);
			break;

		case ID_MAGENTA:
			//MessageBox(hDlg, TEXT("about"), TEXT("ok"), MB_OK);
			g_chosencolor = RGB(255, 0, 255);
			//SetSelectedVoices(hDlg);
			fprintf(LogFile, "Changing Color To Magenta\n");
			pISpVoice->Speak(L"Changing Color To Magenta", SPF_ASYNC, NULL);
			break;

		case ID_YELLOW:
			//MessageBox(hDlg, TEXT("about"), TEXT("ok"), MB_OK);
			g_chosencolor = RGB(255, 255, 0);
			
			fprintf(LogFile, "Changing Color To Yellow\n");
			pISpVoice->Speak(L"Changing Color To Yellow", SPF_ASYNC, NULL);
			break;

		case ID_WHITE:
			//MessageBox(hDlg, TEXT("about"), TEXT("ok"), MB_OK);
			g_chosencolor = RGB(255, 255, 255);
			
			fprintf(LogFile, "Changing Color To White\n");
			pISpVoice->Speak(L"Changing Color To White", SPF_ASYNC, NULL);
			break;

		case ID_GRAY:
			//MessageBox(hDlg, TEXT("about"), TEXT("ok"), MB_OK);
			g_chosencolor = RGB(128, 128, 128);
			//SetSelectedVoices(hDlg);
			fprintf(LogFile, "Changing Color To Gray\n");
			pISpVoice->Speak(L"Changing Color To Gray", SPF_ASYNC, NULL);
			break;

		case ID_VIOLET:
			//MessageBox(hDlg, TEXT("about"), TEXT("ok"), MB_OK);
			g_chosencolor = RGB(143, 0, 255);
			
			fprintf(LogFile, "Changing Color To Violet\n");
			pISpVoice->Speak(L"Changing Color To Violet", SPF_ASYNC, NULL);
			break;

		case ID_ORANGE:
			//MessageBox(hDlg, TEXT("about"), TEXT("ok"), MB_OK);
			g_chosencolor = RGB(255, 165, 0);
			
			fprintf(LogFile, "Changing Color To Orange\n");
			pISpVoice->Speak(L"Changing Color To Orange", SPF_ASYNC, NULL);
			break;


		case ID_INCREASEPOINTS:
			if (PointCount < 3000)
			{
				PointCount = PointCount + 100;
			}

			fprintf(LogFile, "Increased Number of Points\n");
			pISpVoice->Speak(L"Increased Number of Points", SPF_ASYNC, NULL);

			break;

		case ID_DECREASEPOINTS:
			if (PointCount > 500)
			{
				PointCount = PointCount - 100;
			}
			
			fprintf(LogFile, "Decreased Number of Points\n");
			pISpVoice->Speak(L"Decreased Number of POints", SPF_ASYNC, NULL);

			break;

		case ID_INCREASEWAVES:
			if (noOfWaves < 5)
			{
				noOfWaves = noOfWaves + 1;
			}
			
			fprintf(LogFile, "Increasing Number of Wave\n");
			pISpVoice->Speak(L"Increasing Number of Wave", SPF_ASYNC, NULL);
			break;

		case ID_DECREASEWAVES:
			if (noOfWaves > 1)
			{
				noOfWaves = noOfWaves - 1;
			}
			
			fprintf(LogFile, "Decreasing Number of Wave\n");
			pISpVoice->Speak(L"Decreasing Number of Wave", SPF_ASYNC, NULL);
			break;

		case ID_INCREASEAMPLITUDES:
			if (amplitude < 350)
			{
				amplitude = amplitude + 50;
			}
			
			fprintf(LogFile, "Increased Amplitude \n");
			pISpVoice->Speak(L"Increased Amplitude", SPF_ASYNC, NULL);
			break;

		case ID_DECREASEAMPLITUDES:
			if (amplitude > 50)
			{
				amplitude = amplitude - 50;
			}
			
			fprintf(LogFile, "Decreased Amplitude\n");
			pISpVoice->Speak(L"Decreased Amplitude", SPF_ASYNC, NULL);
			break;

		case ID_INCREASEFREQUENCY:
			if (noOfFreq > 2.0) 
			{
				noOfFreq = noOfFreq - 1.0;

			}
			
			fprintf(LogFile, "Increased Frequency\n");
			pISpVoice->Speak(L"Increased Frequency", SPF_ASYNC, NULL);
			break;

		case ID_DECREASEFREQUENCY:
			if (noOfFreq < 12.0)
			{
				noOfFreq = noOfFreq + 1.0;

			}
			
			fprintf(LogFile, "Decreased Frequency\n");
			pISpVoice->Speak(L"Decreased Frequency", SPF_ASYNC, NULL);
			break;

		case ID_RESTARTWAVE:
			for (int i = 0; i < NUM_POINTS; i++)
			{
				plotPointX[i] = -(i * POINTSIZE);
			}
			
			fprintf(LogFile, "User Restarted Wave\n");
			pISpVoice->Speak(L"User Restarted Wave", SPF_ASYNC, NULL);
			//SetTimer(GetActiveWindow(), SBP_TIMER, TIME_INTERVAL_IN_MS, NULL);
			break;

		case IDOK:
			EndDialog(hDlg, IDOK);
			return TRUE;
			break;

		case IDABOUT:
			/*hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void**)&pISpVoice);
			if (FAILED(hr))
			{
				MessageBox(NULL, TEXT("ISpVoice Interface can not be Obtained"), TEXT("ERROR"), MB_ICONERROR);
				DestroyWindow(hwnd);
			}


			pISpVoice->Speak(L"Clicked on About", SPF_ASYNC, NULL);*/
			
			DialogBox(hInst, TEXT("ABOUT"), hDlg, (DLGPROC)AboutDlgProc);
			
			//pISpVoice->Speak(L"Clicked on About", 0, NULL);
			fprintf(LogFile, "User Clicked on About Button\n");
			return 0;
			break;

		case IDEXIT:
			//pISpVoice->Speak(L"Do You Really Want To Exit Application ", SPF_ASYNC, NULL);
			retVal = MessageBox(hDlg, TEXT("Do You Really Want To Exit Application ?"), TEXT("Exit Application"), MB_YESNO | MB_ICONWARNING);

			switch (retVal)
			{
			case IDYES:
				DestroyWindow(gHwnd);
				break;

			case IDNO:
				EndDialog(hDlg, IDEXIT);
				break;
			}
			//TextToSpeech();

		case ID_LISTBOX:
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:

				int selectedIndex = SendMessage(GetDlgItem(hDlg, ID_LISTBOX), LB_GETCURSEL, 0, 0);

				if (selectedIndex != LB_ERR)
				{
					if (selectedIndex == 0)
					{
						pISpVoice->SetVoice(pISpObjectToken1); 
						selectedIndex++;
					}
					else if (selectedIndex == 1)
					{
						pISpVoice->SetVoice(pISpObjectToken2);
					}
				}
				break;
			}
			break;

		case ID_CHECK_EXP_LOG:

			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				EnableWindow(GetDlgItem(hDlg, ID_LISTBOXEXPORT), TRUE);
				SendMessage(GetDlgItem(hDlg, ID_CHECK_EXP_LOG), BM_SETCHECK, BST_CHECKED, 0);
				break;
			}
			break;

		case ID_LISTBOXEXPORT:
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				int selectedIndex = SendMessage(GetDlgItem(hDlg, ID_LISTBOXEXPORT), LB_GETCURSEL, 0, 0);
				if (selectedIndex != LB_ERR)
				{
					convertTextToSpeech = TRUE;
					if (selectedIndex == 0)
					{
						SetVoiceForAudioLog = 1;
						//pISpVoice->SetVoice(pISpObjectToken);
						selectedIndex++;
					}
					else if (selectedIndex == 1)
					{
						SetVoiceForAudioLog = 2;
						//pISpVoice->SetVoice(pISpObjectToken);
					}
				}
				break;
			}
			break;

			break;

		default:

			break;
		}
		break;

	case WM_TIMER:

		

		if (EnableExportRadioButton) 
		{
			EnableWindow(GetDlgItem(hDlg, ID_CHECK_EXP_LOG), TRUE);
		}
		break;

	default:

		return FALSE;
	}

	if (hBrush)
	{
		DeleteObject(hBrush);
		hBrush = NULL;
	}
	return TRUE;
	 

}



BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HICON hIcon;
	HDC hdcStatic;
	HBRUSH hBrush = NULL;
	HBRUSH bbrBkgnd = NULL;
	switch (message)
	{
	case WM_INITDIALOG:

		//g_hFont = CreateFont();

		

		hIcon = LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(SBP_ICON));
		if (hIcon != NULL)
		{
			SendDlgItemMessage(hDlg, SBP_ICON, STM_SETICON, ((WPARAM)(hIcon)), 0);
		}

		return TRUE;

	
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(0, 255, 0));
		SetBkColor(hdcStatic, RGB(0,0,0));

		if (hBrush == NULL)
		{
			hBrush = CreateSolidBrush(RGB(0, 0, 0));
		}
		return((INT_PTR)hBrush);

	}

	case WM_CTLCOLORDLG:
	{
		HDC hdcDlg = (HDC)wParam;
		SetBkColor(hdcDlg, RGB(0, 0, 0));

		hBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
		return((INT_PTR)hBrush);

	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, 0);
			return TRUE; 
			break;
		}
		break;
	}

	return FALSE;
}



BOOL CALLBACK RegisterDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInst = NULL;
	HBRUSH hBrush = NULL;
	HBRUSH hbrBknd = NULL;

	HDC hdcStatic;
	//HDC hdc;

	static BOOL isValidFName = TRUE;
	static BOOL isValidSurname = TRUE;

	COLORREF g_TextColor = RGB(255, 0, 0);

	switch (message)
	{

	case WM_INITDIALOG:
		EnableWindow(GetDlgItem(hDlg, ID_REGISTER), g_UserRegistered);

		return(INT_PTR)TRUE;
		break;

	case WM_CTLCOLORDLG:
	{
		HDC hdcDlg = (HDC)wParam;
		SetBkColor(hdcDlg, RGB(255, 250, 205)); //255,250,205
		return(INT_PTR)CreateSolidBrush(RGB(255, 250, 205));
		break;

	}
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		//SetTextColor(hdcStatic, RGB(255, 250, 205));
		SetBkColor(hdcStatic, RGB(255, 250, 205));

		if (hBrush == NULL)
		{
			hBrush = CreateSolidBrush(RGB(255, 250, 205));
		}

		if ((HWND)lParam == GetDlgItem(hDlg, ID_FIRST_NAME))
		{
			if (!isValidFName)
			{
				SetTextColor(hdcStatic, RGB(255, 0, 0));
				SetBkColor(hdcStatic, RGB(255, 250, 205));
				g_dataRegister = FALSE;
			}
			else
			{
				SetTextColor(hdcStatic, RGB(0, 0, 0));
				SetBkColor(hdcStatic, RGB(255, 250, 205));
				g_dataRegister = TRUE;
			}
		}

		if ((HWND)lParam == GetDlgItem(hDlg, ID_SURNAME))
		{
			if (!isValidSurname)
			{
				SetTextColor(hdcStatic, RGB(255, 0, 0));
				SetBkColor(hdcStatic, RGB(255, 250, 205));
				g_dataRegister = FALSE;
			}
			else
			{
				SetTextColor(hdcStatic, RGB(0, 0, 0));
				SetBkColor(hdcStatic, RGB(255, 250, 205));
				g_dataRegister = TRUE;
			}
		}
		SetBkColor(hdcStatic, RGB(255, 250, 205));
		hBrush = (HBRUSH)CreateSolidBrush(RGB(255, 250, 205));
		return((INT_PTR)hBrush);

		//return((INT_PTR)hBrush);
		break;

	}

	case WM_COMMAND:
		if (HIWORD(wParam) == EN_KILLFOCUS)
		{
			GetDlgItemText(hDlg, ID_ETFIRST_NAME, szFirstName, sizeof(szFirstName) / sizeof(szFirstName[0]));
			GetDlgItemText(hDlg, ID_ETSURNAME, szSurname, sizeof(szSurname) / sizeof(szSurname[0]));

			isValidFName = !ValidateRegistration(szFirstName);
			isValidSurname = !ValidateRegistration(szSurname);

			SendDlgItemMessage(hDlg, ID_FIRST_NAME, WM_ENABLE, TRUE, 0L);
			SendDlgItemMessage(hDlg, ID_SURNAME, WM_ENABLE, TRUE, 0L);

			EnableWindow(GetDlgItem(hDlg, ID_REGISTER), isValidFName && isValidSurname);
		}

		switch (LOWORD(wParam))
		{
		case ID_REGISTER:
			GetDlgItemText(hDlg, ID_ETFIRST_NAME, szFirstName, sizeof(szFirstName) / sizeof(szFirstName[0]));
			GetDlgItemText(hDlg, ID_ETSURNAME, szSurname, sizeof(szSurname) / sizeof(szSurname[0]));

			SYSTEMTIME localTime;
			GetLocalTime(&localTime);

			//wsprintf(szFileName, TEXT("%s-%s -%0.2d %0.2d %0.2d -%0.2d %0.2d %0.2d.txt"), szFirstName, szSurname, localTime.wDay, localTime.wMonth, localTime.wYear, localTime.wHour, localTime.wMinute, localTime.wSecond);
			wsprintf(szFileName, TEXT("%s-%s %0.2d-%0.2d-%0.2d %0.2d-%0.2d-%0.2d.txt"), szFirstName, szSurname, localTime.wDay, localTime.wMonth, localTime.wYear, localTime.wHour, localTime.wMinute, localTime.wSecond);

			LogFile = fopen(szFileName, "w");

			fprintf(LogFile, "User Name is : %s %s\n", szFirstName, szSurname);
			fprintf(LogFile, "User Registration Date :- %0.2d - %0.2d - %0.2d\n", localTime.wDay, localTime.wMonth, localTime.wYear);
			fprintf(LogFile, "User Registration Time :- %0.2d Hours  %0.2d Minutes  %0.2d Seconds\n", localTime.wHour, localTime.wMinute, localTime.wSecond);
			fprintf(LogFile, "\n");
			fprintf(LogFile, ".........User Activities are ..........: \n\n");

			
			g_UserRegistered = TRUE;

			EndDialog(hDlg, IDCANCEL);
			break;

		default:

			break;

		}
		break;
	default:

		return FALSE;

	}
	return TRUE;
}


	

BOOL ValidateRegistration(const TCHAR* str)  
{
	while (*str)
	{
		if (!isalpha(*str))
		{
			return TRUE;
		}
		str++;
	}
	return FALSE;

}



void functionTextToSpeech(void)
{
	HRESULT hr = NULL;
	CHAR textBuffer[TEXT_FILE_NUM_CHARACTER];
	TCHAR textBuffer_WideCharacter[TEXT_FILE_NUM_CHARACTER];

	fopen_s(&LogFile, szFileName, "r");
	if (LogFile == NULL)
	{
		MessageBox(hwnd, TEXT("Can not open File"), TEXT("Error"), MB_OK);
	}

	SYSTEMTIME localTime;
	GetLocalTime(&localTime);

	if (SetVoiceForAudioLog == 1)
	{
		sprintf(filenamewav, "%s-%s %0.2d %0.2d %0.2d %0.2d %0.2d %0.2d-David.wav", szFirstName, szSurname, localTime.wDay, localTime.wMonth, localTime.wYear, localTime.wHour, localTime.wMinute, localTime.wSecond);
	}

	if (SetVoiceForAudioLog == 2)
	{
		sprintf(filenamewav, "%s-%s %0.2d %0.2d %0.2d %0.2d %0.2d %0.2d-Zira.wav", szFirstName, szSurname, localTime.wDay, localTime.wMonth, localTime.wYear, localTime.wHour, localTime.wMinute, localTime.wSecond);
	}

	hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &pIEnumSpObjectTokens);
	if (FAILED(hr))
	{
		MessageBox(hwnd, TEXT("SpEnumTokens Failed to Obtained"), TEXT("Error"), MB_OK);
	}

	//Assigning Audio Format
	audioFormat.AssignFormat(SPSF_44kHz16BitStereo);

	hr = SPBindToFile(filenamewav, SPFM_CREATE_ALWAYS, &pISpStream, &audioFormat.FormatId(), audioFormat.WaveFormatExPtr());
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("spBind file Failed to obtained"), TEXT("ERROR"), MB_OK);
		//DestroyWindow(hwnd);
	}

	//set audio output stram to the above created stream
	hr = pISpVoice->SetOutput(pISpStream, FALSE);
	if (FAILED(hr))
	{
		MessageBox(hwnd, TEXT("Set OutPut Failed"), TEXT("ERROR"), MB_OK);
		//DestroyWindow(hwnd);
	}

	//Read fromm MySelf.txt Text File into the textBuffer Array
	fread(&textBuffer, sizeof(char), TEXT_FILE_NUM_CHARACTER, LogFile);

	//After the above call to fread we will have the content of the text file in ANSI character buffer text Buffer but speak() requires wide char string is its 1st parameter
	MultiByteToWideChar(CP_UTF8, 0, textBuffer, TEXT_FILE_NUM_CHARACTER, (LPWSTR)textBuffer_WideChar, TEXT_FILE_NUM_CHARACTER);

	//from the list of voice tokens get the next voice tokens
	pIEnumSpObjectTokens->Next(1, &pISpObjectToken1, NULL);

	//from the list of voice tokens get the next voice tokens
	pIEnumSpObjectTokens->Next(1, &pISpObjectToken1, NULL);

	if (SetVoiceForAudioLog == 1)
	{
		//Set voice to David Voice
		hr = pISpVoice->SetVoice(pISpObjectToken1);
	}
	else if (SetVoiceForAudioLog == 2)
	{
		//Set voice to Zira voice
		hr = pISpVoice->SetVoice(pISpObjectToken2);
	}

	//Convert Text To Speech in the voice set in the above set
	hr = pISpVoice->Speak((LPWSTR)textBuffer_WideChar, 0, NULL);

	fclose(LogFile);
	LogFile = NULL;


}



void safeRelease(void)
{
	if (pISpStream)
	{
		pISpStream->Release();
		pISpStream = NULL;
	}

	if (pISpObjectToken2)
	{
		pISpObjectToken2->Release();
		pISpObjectToken2 = NULL;
	}

	if (pISpObjectToken1)
	{
		pISpObjectToken1->Release();
		pISpObjectToken1 = NULL;
	}

	if (pIEnumSpObjectTokens)
	{
		pIEnumSpObjectTokens->Release();
		pIEnumSpObjectTokens = NULL;
	}

	if (pISpVoice)
	{
		pISpVoice->Release();
		pISpVoice = NULL;
	}

	if (pISineWave)
	{
		pISineWave->Release();
		pISineWave = NULL;
	}

	if (LogFile)
	{
		fclose(LogFile);
		LogFile = NULL;
	}

}

