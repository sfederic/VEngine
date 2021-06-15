#pragma once

#include <Windows.h>
#include "Debug.h"
#include <stdint.h>

LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

class CoreSystem
{
public:
	void Init();
	void Tick();
	void SetupWindow(HINSTANCE instance, int cmdShow);
	void SetTimerFrequency();
	void StartTimer();
	void EndTimer();
	void HandleMessages();
	void HandleWin32MessagePump(UINT message, WPARAM wparam, LPARAM lparam);
	float GetAspectRatio();

	void Exit();

	HWND mainWindow;
	MSG msg;
	int windowWidth;
	int windowHeight;

	__int64 frameStartTime;
	__int64 frameEndTime; 
	__int64 tickFrequency;
	double deltaTime;
	double deltaAccum;
	double ticks;
	double timeSinceStartup;
	uint32_t frameCount;
	uint32_t finalFrameCount;

	bool bMainLoop = true;
	bool bGamePaused = false;
	bool bGamePlayOn = false; //Runs the game state in editor within the viewport.
};

extern CoreSystem gCoreSystem;