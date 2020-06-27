#include "Win32Util.h"
#include "UIContext.h"
#include "Input.h"
#include <stdio.h>

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
	wc.hCursor = LoadCursor(0, IDC_CROSS);

	RegisterClass(&wc);
	mainWindow = CreateWindow("Window", "d3d11", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, 0, 0, instance, 0);
	ShowWindow(mainWindow, cmdShow);
	UpdateWindow(mainWindow);
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

LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	bool wasDown = (lparam & (1 << 38)) != 0;
	bool isDown = (lparam & (1 << 31)) == 0;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		StoreKeyDownInput(wparam);

		if (wparam == VK_RETURN)
		{
			DestroyWindow(window);
			return 0;
		}

		break;

	case WM_KEYUP:
		StoreKeyUpInput(wparam);
		break;

	case WM_LBUTTONUP:
		mouseUp = true;
		break;
	}

	return DefWindowProc(window, message, wparam, lparam);
}