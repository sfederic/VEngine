#include "Win32Util.h"
#include "UIContext.h"
#include "Input.h"
#include <stdio.h>

#pragma comment(lib, "Comctl32.lib")

#include <commctrl.h>
#include <mshtmcid.h>

HWND renderWindow;
HWND mainWindow;
const int windowWidth = 800;
const int windowHeight = 600;
MSG msg;

void Win32Util::SetupWindow(HINSTANCE instance, int cmdShow)
{
	WNDCLASS wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = "Window";
	wc.lpfnWndProc = WndProc;
	wc.hInstance = instance;
	wc.hCursor = LoadCursor(0, IDC_ARROW);

	RegisterClass(&wc);
	mainWindow = CreateWindow("Window", "d3d11", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, 0, 0, instance, 0);
	if (mainWindow)
	{
		ShowWindow(mainWindow, cmdShow);
		UpdateWindow(mainWindow);
	}
	else
	{
		throw;
	}

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = "RenderWindow";
	wc.lpfnWndProc = WndProc;
	wc.hInstance = instance;
	wc.hCursor = LoadCursor(0, IDC_CROSS);

	RegisterClass(&wc);
	renderWindow = CreateWindow("RenderWindow", "render", WS_CHILD | WS_OVERLAPPED, 50, 50, 750, 550, mainWindow, 0, instance, 0);
	if (renderWindow)
	{
		UpdateWindow(renderWindow);
		ShowWindow(renderWindow, TRUE);
	}
	else
	{
		throw;
	}

	HWND toolbarWindow = CreateSimpleToolbar(mainWindow);
	if (toolbarWindow)
	{
		UpdateWindow(toolbarWindow);
		ShowWindow(toolbarWindow, TRUE);
	}
	else
	{
		throw;
	}
}

void Win32Util::SetTimerFrequency()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	ticks = 1.0 / (double)freq;
	deltaAccum = 0.0;
}

void Win32Util::StartTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&start);
}

void Win32Util::EndTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&end);
	delta = ticks * (double)(end - start);
	if (delta < 0.0) { delta = 0.0; }
	deltaAccum += delta;
	frameAccum++;
	if (deltaAccum > 1.0)
	{
		char fpsText[128];
		snprintf(fpsText, sizeof(fpsText), "d3d11 | FPS: %d mspf %f.12", frameAccum, delta);
		SetWindowText(mainWindow, fpsText);
		frameAccum = 0;
		deltaAccum = 0.0;
	}
}

void Win32Util::HandleMessages(MSG msg)
{
	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

float Win32Util::GetAspectRatio()
{
	return (float)((float)windowWidth / (float)windowHeight);
}

HWND CreateSimpleToolbar(HWND hWndParent)
{
	// Declare and initialize local constants.
	const int ImageListID = 0;
	const int numButtons = 3;
	const int bitmapSize = 16;

	const DWORD buttonStyles = BTNS_AUTOSIZE;

	// Create the toolbar.
	HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
		WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0,
		hWndParent, NULL, GetModuleHandle(NULL), NULL);

	if (hWndToolbar == NULL)
		return NULL;

	// Create the image list.
	HIMAGELIST g_hImageList = ImageList_Create(bitmapSize, bitmapSize,   // Dimensions of individual bitmaps.
		ILC_COLOR16 | ILC_MASK,   // Ensures transparent background.
		numButtons, 0);

	// Set the image list.
	SendMessage(hWndToolbar, TB_SETIMAGELIST,
		(WPARAM)ImageListID,
		(LPARAM)g_hImageList);

	// Load the button images.
	SendMessage(hWndToolbar, TB_LOADIMAGES,
		(WPARAM)IDB_STD_SMALL_COLOR,
		(LPARAM)HINST_COMMCTRL);

	// Initialize button info.
	// IDM_NEW, IDM_OPEN, and IDM_SAVE are application-defined command constants.

	TBBUTTON tbButtons[numButtons] =
	{
		{ MAKELONG(STD_FILENEW,  ImageListID), IDM_NEW,  TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)L"New" },
		{ MAKELONG(STD_FILEOPEN, ImageListID), IDM_OPEN, TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)L"Open"},
		{ MAKELONG(STD_FILESAVE, ImageListID), IDM_SAVE, 0,               buttonStyles, {0}, 0, (INT_PTR)L"Save"}
	};

	// Add buttons.
	SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);

	// Resize the toolbar, and then show it.
	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(hWndToolbar, TRUE);

	return hWndToolbar;
}

LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		StoreKeyDownInput(wparam);

		//Close editor
		if (wparam == VK_ESCAPE)
		{
			DestroyWindow(window);
			return 0;
		}

		break;

	case WM_KEYUP:
		StoreKeyUpInput(wparam);
		break;

	case WM_LBUTTONUP:
		StoreMouseUpInput(wparam);
		break;

	case WM_LBUTTONDOWN:
		StoreMouseDownInput(wparam);
		break;
	}

	return DefWindowProc(window, message, wparam, lparam);
}