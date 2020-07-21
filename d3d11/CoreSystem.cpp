#include "CoreSystem.h"
#include "UISystem.h"
#include "Input.h"
#include "Debug.h"
#include <stdio.h>

CoreSystem coreSystem;

void CoreSystem::SetupWindow(HINSTANCE instance, int cmdShow)
{
	WNDCLASS wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = "Window";
	wc.lpfnWndProc = WndProc;
	wc.hInstance = instance;
	wc.hCursor = LoadCursor(0, IDC_CROSS);

	RegisterClass(&wc);
	mainWindow = CreateWindow("Window", "d3d11", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, 0, 0, instance, 0);
	if (!mainWindow)
	{
		HR(GetLastError());
	}
	else
	{
		ShowWindow(mainWindow, cmdShow);
		UpdateWindow(mainWindow);
	}
}

void CoreSystem::SetTimerFrequency()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&tickFrequency);
	ticks = 1.0 / (double)tickFrequency;
	deltaAccum = 0.0;
}

void CoreSystem::StartTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&frameStartTime);
}

void CoreSystem::EndTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&frameEndTime);
	deltaTime = ticks * (double)(frameEndTime - frameStartTime);

	if (deltaTime < 0.0) 
	{ 
		deltaTime = 0.0; 
	}

	deltaAccum += deltaTime;
	frameCount++;

	if (deltaAccum > 1.0)
	{
		char fpsText[128];
		snprintf(fpsText, sizeof(fpsText), "d3d11 | FPS: %d mspf %f.12", frameCount, deltaTime);
		SetWindowText(mainWindow, fpsText);

		frameCount= 0;
		deltaAccum = 0.0;
	}
}

void CoreSystem::HandleMessages()
{
	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

float CoreSystem::GetAspectRatio()
{
	return (float)((float)windowWidth / (float)windowHeight);
}

LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		inputSystem.StoreKeyDownInput(wparam);

		//Close editor
		if (wparam == VK_ESCAPE)
		{
			DestroyWindow(window);
			return 0;
		}

		break;

	case WM_KEYUP:
		inputSystem.StoreKeyUpInput(wparam);
		break;

	case WM_LBUTTONUP:
		inputSystem.StoreMouseLeftUpInput(wparam);
		break;

	case WM_LBUTTONDOWN:
		inputSystem.StoreMouseLeftDownInput(wparam);
		break;

	case WM_RBUTTONUP:
		inputSystem.StoreMouseRightUpInput(wparam);
		break;

	case WM_RBUTTONDOWN:
		inputSystem.StoreMouseRightDownInput(wparam);
		break;

	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
		{
			inputSystem.StoreMouseWheelDown();
		}
		else
		{
			inputSystem.StoreMouseWheelUp();
		}
	}

	return DefWindowProc(window, message, wparam, lparam);
}