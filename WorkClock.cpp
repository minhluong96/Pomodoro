// WorkClock.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WorkClock.h"
#include <windowsX.h>
#include <winuser.h>
#include <commctrl.h>
#include <ctime>
#include <thread>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

#define MAX_LOADSTRING 100
#define IDC_BUTTON_1 110

struct Data
{
	HWND hWnd;
};

void onCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
void onPaint(HWND hWnd);
void onDestroy(HWND hWnd);
BOOL onCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
void stopWatch(int sec, HWND hWnd);
DWORD WINAPI myThread(__in LPVOID lpParameter);

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WORKCLOCK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WORKCLOCK));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WORKCLOCK));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WORKCLOCK);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, onCreate);
		HANDLE_MSG(hWnd, WM_PAINT, onPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, onDestroy);
		HANDLE_MSG(hWnd, WM_COMMAND, onCommand);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

HWND Watch, Button;

BOOL onCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

	// Take system's font
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	//Create thread
	std::thread thread1;

	//Create boxes
	Watch = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_CENTER | WS_BORDER, 100, 100, 100, 40, hWnd, NULL, hInst, NULL);
	SendMessage(Watch, WM_SETFONT, WPARAM(hFont), TRUE);
	
	Button = CreateWindowEx(0, L"BUTTON", L"Start", WS_CHILD | WS_VISIBLE, 100, 150, 100, 40, hWnd, (HMENU)IDC_BUTTON_1, hInst, NULL);
	SendMessage(Button, WM_SETFONT, WPARAM(hFont), TRUE);

	return true;
}

void onCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	WCHAR* buff1 = NULL;
	WCHAR* buff2 = NULL;
	WCHAR* buffKQ = NULL;

	int size1, size2, sizeKQ;

	double money, rate, result;

	switch (id)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;

	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;

	case IDC_BUTTON_1:
		DWORD myThreadID;
		HANDLE myHandle;
		/*Data *tData = new Data;
		tData->hWnd = hWnd;*/
		myHandle = CreateThread(0, 0, myThread, (LPVOID)hWnd, 0, &myThreadID);
		break;
	}
}

void onPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);
	// TODO: Add any drawing code here...
	EndPaint(hWnd, &ps);
}

void onDestroy(HWND hWnd)
{
	PostQuitMessage(0);
}

void stopWatch(int sec, HWND hWnd)
{
	WCHAR *time = new WCHAR[10];
	swprintf(time, 10, L"%d", sec);
	SetWindowText(Watch, time);
	UpdateWindow(hWnd);
	clock_t t1, t2;
	t1 = clock() / CLOCKS_PER_SEC;
	//t2 = t1 + 1;
	for (int i = sec; i > 0;)
	{
		t2 = t1 + 1;
		while (t1 < t2)
		{
			t1 = clock() / CLOCKS_PER_SEC;
		}
		i--;
		swprintf(time, 10, L"%d", i);
		SetWindowText(Watch, time);
		UpdateWindow(hWnd);
	}
}

DWORD WINAPI myThread(__in LPVOID lpParameter)
{
	EnableWindow(Button, false);
	//Data *tData = (struct Data*)lpParameter;
	HWND hWnd = (HWND)lpParameter;
	stopWatch(5, hWnd);
	/*WCHAR *time = new WCHAR[10];
	swprintf(time, 10, L"%d", tData->sec);
	SetWindowText(Watch, time);
	UpdateWindow(tData->hWnd);
	clock_t t1, t2;
	t1 = clock() / CLOCKS_PER_SEC;
	for (int i = tData->sec; i > 0;)
	{
		t2 = t1 + 1;
		while (t1 < t2)
		{
			t1 = clock() / CLOCKS_PER_SEC;
		}
		i--;
		swprintf(time, 10, L"%d", i);
		SetWindowText(Watch, time);
		UpdateWindow(tData->hWnd);
	}*/
	EnableWindow(Button, true);
	return 0;
}