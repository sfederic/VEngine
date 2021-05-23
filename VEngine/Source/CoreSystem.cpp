#include "CoreSystem.h"
#include "UISystem.h"
#include "Input.h"
#include "Debug.h"
#include <stdio.h>
#include "RenderSystem.h"
#include "Camera.h"
#include "GlobalDefines.h"

CoreSystem gCoreSystem;

void CoreSystem::Init()
{
#ifndef EDITOR
	bGamePlayOn = true;
#endif
}

void CoreSystem::Tick()
{
	//Start play in editor
	if (gInputSystem.GetAsyncKey(Keys::Ctrl))
	{
		if (gInputSystem.GetKeyUpState('P'))
		{
			bGamePlayOn = !bGamePlayOn;
		}
	}
	
	//Pause game (freezes actor ticks)
	if (bGamePlayOn)
	{
		if (gInputSystem.GetKeyUpState('P', (int)Keys::Ctrl))
		{
			bGamePaused = !bGamePaused;
		}
	}
}

void CoreSystem::SetupWindow(HINSTANCE instance, int cmdShow)
{
	WNDCLASS wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = "Window";
	wc.lpfnWndProc = WndProc;
	wc.hInstance = instance;
	wc.hCursor = LoadCursor(0, IDC_CROSS);

	RegisterClass(&wc);
	mainWindow = CreateWindow("Window", "VEngine", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, 0, 0, instance, 0);
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
		//Win32 FPS windowtext
		char fpsText[128];
		snprintf(fpsText, sizeof(fpsText), "d3d11 | FPS: %d mspf %f.12", frameCount, deltaTime);
		SetWindowText(mainWindow, fpsText);

		finalFrameCount = frameCount;

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
		gInputSystem.StoreMouseLeftUpInput();
		break;

	case WM_LBUTTONDOWN:
		gInputSystem.StoreMouseLeftDownInput();
		break;

	case WM_RBUTTONUP:
		gInputSystem.StoreMouseRightUpInput();
		break;

	case WM_RBUTTONDOWN:
		gInputSystem.StoreMouseRightDownInput();
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

	case WM_SIZE:
		if (gRenderSystem.swapchain)
		{
			gRenderSystem.context->OMSetRenderTargets(0, 0, 0);

			// Release all outstanding references to the swap chain's buffers.
			for (int rtvIndex = 0; rtvIndex < gRenderSystem.frameCount; rtvIndex++)
			{
				gRenderSystem.rtvs[rtvIndex]->Release();
			}

			gRenderSystem.dsv->Release();

			//gUISystem.Cleanup();
			//gUISystem.Init();

			//ID3D11Debug* debug;
			//gRenderSystem.device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debug));
			//debug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
			//gRenderSystem.context->ClearState();
			//gRenderSystem.context->Flush();

			// Preserve the existing buffer count and format.
			// Automatically choose the width and height to match the client rect for HWNDs.

			UINT width = LOWORD(lparam);
			UINT height = HIWORD(lparam);
			gCoreSystem.windowWidth = width;
			gCoreSystem.windowHeight = height;

			HR(gRenderSystem.swapchain->ResizeBuffers(gRenderSystem.frameCount,
				gCoreSystem.windowWidth, gCoreSystem.windowHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

			gRenderSystem.CreateRTVAndDSV();

			gRenderSystem.matrices.proj = XMMatrixPerspectiveFovLH(XM_PI / 3, gCoreSystem.GetAspectRatio(), 0.01f, 1000.f);
			GetActiveCamera()->proj = gRenderSystem.matrices.proj;

			// Set up the viewport.
			gRenderSystem.viewport.Width = gCoreSystem.windowWidth;
			gRenderSystem.viewport.Height = gCoreSystem.windowHeight;
			gRenderSystem.viewport.MinDepth = 0.0f;
			gRenderSystem.viewport.MaxDepth = 1.0f;
			gRenderSystem.viewport.TopLeftX = 0;
			gRenderSystem.viewport.TopLeftY = 0;
			gRenderSystem.context->RSSetViewports(1, &gRenderSystem.viewport);
		}
	}

	return DefWindowProc(window, message, wparam, lparam);
}