#pragma once
#include <Windows.h>
#include <cstdint>

//Any utility functions or variables that are 'core' to the engine live here.
namespace Core
{
	extern bool mainLoop;
	extern bool gameplayOn;

	extern __int64 tickFrequency;
	extern __int64 frameStartTime;
	extern __int64 frameEndTime;

	extern double timeSinceStartup;

	extern uint32_t frameCount;
	extern uint32_t finalFrameCount;

	double GetDeltaTime();
	void SetTimerFrequency();
	void Init();
	void StartTimer();
	void EndTimer();
	void StartGame();
	void EndGame();
	void HandleWin32MessagePump(UINT message, WPARAM wparam, LPARAM lparam);
	LRESULT __stdcall WndProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
}
