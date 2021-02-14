#include "Profiler.h"
#include <locale>
#include <Windows.h>

void Profiler::Start()
{
	__int64 startTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
	timeFrames.push_back(TimeFrame(__func__, startTime));
}

void Profiler::End()
{
	__int64 endTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&endTime);

	TimeFrame currentTimeFrame = timeFrames.back();
}
