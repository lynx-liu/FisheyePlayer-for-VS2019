// FisheyePlayer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "FisheyePlayer.h"
#include <commdlg.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
HWND hWnd;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	OpenFile(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	SaveFile(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int iCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	wsprintf(szTitle, L"%s %S %S", szTitle, __DATE__, __TIME__);
	LoadString(hInstance, IDC_FISHEYEPLAYER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, iCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FISHEYEPLAYER));

	/* enable OpenGL for the window */
	screenRenderer.EnableOpenGL(hWnd);

	BOOL bQuit = FALSE;
	/* program main loop */
	while (!bQuit)
	{
		/* check for messages */
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			/* handle or dispatch messages */
			if (msg.message == WM_QUIT)
			{
				bQuit = TRUE;
			}
			else if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			screenRenderer.ReadFrame();
			screenRenderer.DrawFrame();
			Sleep(1);
		}
	}

	/* shutdown OpenGL */
	screenRenderer.DisableOpenGL(hWnd);

	/* destroy the window explicitly */
	DestroyWindow(hWnd);
	return 0;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FISHEYEPLAYER));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_FISHEYEPLAYER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			PostQuitMessage(0);
			break;
		case ID_FILE_OPEN:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_OPENFILEBOX), hWnd, OpenFile);
			break;
		case ID_MODE_VR:
			setMode(VR);
			break;
		case ID_MODE_SPHERE:
			setMode(Sphere);
			break;
		case ID_MODE_NORMAL:
			setMode(Normal);
			break;
		case ID_FILE_SAVE:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_SAVEFILEBOX), hWnd, SaveFile);
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_CREATE:
		SetTimer(hWnd, WM_USER + 100, 1000, NULL);
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_DESTROY:
		KillTimer(hWnd, WM_USER + 100);
		break;

	case WM_TIMER:
	{
					 time_t second = time(NULL);
					 struct tm *stime = localtime(&second);

					 TCHAR szTime[MAX_PATH] = { 0 };
					 wsprintf(szTime, L"%04d-%02d-%02d %02d:%02d:%02d", stime->tm_year + 1900, stime->tm_mon, stime->tm_mday, stime->tm_hour, stime->tm_min, stime->tm_sec);
					 screenRenderer.setText(szTime, Point2f(-1.0f, -1.0f), RGB(255, 255, 0), 24);
	}
		break;

	case WM_SIZE:
		screenRenderer.onSize(hWnd);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'X':
			break;
		case 'Y':
			break;
		case 'D':
			screenRenderer.showLine();
			break;
		case 'O':
			OpenFile();
			break;
		case 'S':
			SaveFile();
			break;
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		case VK_TAB:
		{
					   switch (screenRenderer.getMode())
					   {
					   case Normal:
						   setMode(Sphere);
						   break;
					   case Sphere:
						   setMode(VR);
						   break;
					   case VR:
					   default:
						   setMode(Normal);
						   break;
					   }
		}
			break;
		case VK_RETURN:
			break;
		case VK_SPACE:
			break;
		case VK_UP:
			break;
		case VK_DOWN:
			break;
		case VK_LEFT:
			break;
		case VK_RIGHT:
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		POINT point;
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		screenRenderer.onLButtonDown(point);
		break;

	case WM_MOUSEMOVE:
		if (wParam&MK_LBUTTON) {//选择文件时，会触发这里，所以用pointDown.x+pointDown.y>0过滤
			POINT point;
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			screenRenderer.onMouseMove(point);
		}
		break;

	case WM_LBUTTONUP:
		screenRenderer.onLButtonUp();
		break;

	case WM_MOUSEWHEEL:
		screenRenderer.onMouseWheel((short)HIWORD(wParam));
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return TRUE;
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

// Message handler for openfile box.
INT_PTR CALLBACK OpenFile(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetDlgItemTextW(hDlg, IDC_EDIT_URL, L"rtsp://192.168.43.1:8086");
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			if (LOWORD(wParam) == IDOK)
			{
				TCHAR *szFileName = (TCHAR *)malloc(MAX_PATH*sizeof(TCHAR));
				if (szFileName)
				{
					memset(szFileName, 0, MAX_PATH*sizeof(TCHAR));
					GetDlgItemTextW(hDlg, IDC_EDIT_URL, szFileName, MAX_PATH);
					loadMedia(szFileName);
					free(szFileName);
					szFileName = NULL;
				}
			}
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDB_OPENFILE)
		{
			if (OpenFile())
			{
				EndDialog(hDlg, IDOK);
			}
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL loadMedia(TCHAR *szFileName)
{
	screenRenderer.SetDevice(L"Sphere.dat");
	if (screenRenderer.OpenFile(szFileName))
		return TRUE;
	return FALSE;
}

// Message handler for savefile box.
INT_PTR CALLBACK SaveFile(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetDlgItemTextW(hDlg, IDC_EDIT_URL, L"rtsp://127.0.0.1:554");
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			if (LOWORD(wParam) == IDOK)
			{
				TCHAR *szFileName = (TCHAR *)malloc(MAX_PATH*sizeof(TCHAR));
				if (szFileName)
				{
					memset(szFileName, 0, MAX_PATH*sizeof(TCHAR));
					GetDlgItemTextW(hDlg, IDC_EDIT_URL, szFileName, MAX_PATH);
					screenRenderer.SaveFile(szFileName);
					free(szFileName);
					szFileName = NULL;
				}
			}
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDB_SAVEFILE)
		{
			if (SaveFile())
			{
				EndDialog(hDlg, IDOK);
			}
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL SaveFile()
{
	TCHAR szFileName[MAX_PATH] = { L"Sphere" };
	TCHAR szFileExt[MAX_LOADSTRING] = { 0 };//扩展名缓冲区

	OPENFILENAMEW ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFileName;
	ofn.lpstrDefExt = szFileExt;
	ofn.nMaxFile = sizeof(szFileName);
	ofn.lpstrFilter = TEXT("Mp4 Files(*.mp4)\0*.mp4\0JPEG Files(*.jpeg;*jpg)\0*.jpeg;*.jpg\0PNG Files(*.png)\0*.png\0Bitmap Files(*.bmp)\0*.bmp\0AVI(*.avi)\0*.avi\0All Files(*.*)\0*.*\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&ofn) == TRUE)
	{
		return screenRenderer.SaveFile(szFileName);
	}
	return FALSE;
}

void setMode(DispMode mode)
{
	switch (mode)
	{
	case VR:
		CheckMenuItem(GetMenu(hWnd), ID_MODE_VR, MF_CHECKED);
		CheckMenuItem(GetMenu(hWnd), ID_MODE_SPHERE, MF_UNCHECKED);
		CheckMenuItem(GetMenu(hWnd), ID_MODE_NORMAL, MF_UNCHECKED);
		break;

	case Sphere:
		CheckMenuItem(GetMenu(hWnd), ID_MODE_VR, MF_UNCHECKED);
		CheckMenuItem(GetMenu(hWnd), ID_MODE_SPHERE, MF_CHECKED);
		CheckMenuItem(GetMenu(hWnd), ID_MODE_NORMAL, MF_UNCHECKED);
		break;

	case Normal:
	default:
		CheckMenuItem(GetMenu(hWnd), ID_MODE_VR, MF_UNCHECKED);
		CheckMenuItem(GetMenu(hWnd), ID_MODE_SPHERE, MF_UNCHECKED);
		CheckMenuItem(GetMenu(hWnd), ID_MODE_NORMAL, MF_CHECKED);
		break;
	}
	screenRenderer.setMode(mode);
}

BOOL OpenFile()
{
	TCHAR szFileName[MAX_PATH] = { 0 };

	OPENFILENAMEW ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFileName);
	ofn.lpstrFilter = TEXT("Medai Files(*.bmp;*.jpeg;*jpg;*.png;*.mp4)\0*.bmp;*.jpeg;*.jpg;*.png;*.mp4\0Mp4 Files(*.mp4)\0*.mp4\0JPEG Files(*.jpeg;*jpg)\0*.jpeg;*.jpg\0PNG Files(*.png)\0*.png\0Bitmap Files(*.bmp)\0*.bmp\0All Files(*.*)\0*.*\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

	if (GetOpenFileNameW(&ofn) == TRUE)
	{
		return loadMedia(szFileName);
	}
	return FALSE;
}