#include "CoreSystem.h"
#include "UISystem.h"
#include "Input.h"
#include "Debug.h"
#include <stdio.h>

CoreSystem gCoreSystem;

void CoreSystem::SetupWindow(HINSTANCE instance, int cmdShow)
{
	windowWidth = 1600;
	windowHeight = 900;

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

		RECT window_r; RECT desktop_r;
		GetWindowRect(mainWindow, &window_r);
		GetWindowRect(GetDesktopWindow(), &desktop_r);
		int xPos = (desktop_r.right - (window_r.right - window_r.left)) / 2;
		int yPos = (desktop_r.bottom - (window_r.bottom - window_r.top)) / 2;

		SetWindowPos(mainWindow, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
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
	else if (deltaTime > 1.0)
	{
		deltaTime = 1.0f / 60.f; //Just in case of breakpoint stuff
	}

	deltaAccum += deltaTime;
	timeSinceStartup += deltaTime;
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

//Helper for exiting the engine in the main loop
void CoreSystem::Exit()
{
	msg.message = WM_QUIT;
}

LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		gInputSystem.StoreKeyDownInput(wparam);

		//Close editor
		if (wparam == VK_ESCAPE)
		{
			DestroyWindow(window);
			return 0;
		}

		break;

	case WM_KEYUP:
		gInputSystem.StoreKeyUpInput(wparam);
		break;

	case WM_LBUTTONUP:
		gInputSystem.StoreMouseLeftUpInput(wparam);
		break;

	case WM_LBUTTONDOWN:
		gInputSystem.StoreMouseLeftDownInput(wparam);
		break;

	case WM_RBUTTONUP:
		gInputSystem.StoreMouseRightUpInput(wparam);
		break;

	case WM_RBUTTONDOWN:
		gInputSystem.StoreMouseRightDownInput(wparam);
		break;

	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
		{
			gInputSystem.StoreMouseWheelDown();
		}
		else
		{
			gInputSystem.StoreMouseWheelUp();
		}
	}

	return DefWindowProc(window, message, wparam, lparam);
}