#pragma once

#include <Windows.h>
#include "Debug.h"

LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

class CoreSystem
{
public:
	void SetupWindow(HINSTANCE instance, int cmdShow);
	void SetTimerFrequency();
	void StartTimer();
	void EndTimer();
	void HandleMessages();
	float GetAspectRatio();

	HWND mainWindow;
	MSG msg;
	const int windowWidth = 800;
	const int windowHeight = 600;

	__int64 frameStartTime;
	__int64 frameEndTime; 
	__int64 tickFrequency;
	double deltaTime;
	double deltaAccum;
	double ticks;
	int frameCount;
};

extern CoreSystem coreSystem;