#include "Profiler.h"
#include <locale>
#include <Windows.h>

Profiler gProfiler;

void Profiler::Init()
{

}

void Profiler::Clean()
{
	timeFrames.empty();
}

void Profiler::Start()
{
	__int64 startTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
	timeFrames.push_back(TimeFrame(startTime));
}

void Profiler::End(const wchar_t* functionName)
{
	__int64 endTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&endTime);

	TimeFrame* currentTimeFrame = &timeFrames.back();
	currentTimeFrame->endTime = endTime;
	currentTimeFrame->SetElapsedTime();
}

void TimeFrame::SetElapsedTime()
{
	__int64 cpuFreq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cpuFreq);
	double ticks = 1.0 / (double)cpuFreq;

	if (currentElapsedTimeIndex < 60)
	{
		elapsedTimes[currentElapsedTimeIndex] = ticks * (double)(endTime - startTime);
		currentElapsedTimeIndex++;
	}
	else
	{
		currentElapsedTimeIndex = 0;
	}
}

double TimeFrame::GetAverageTime()
{
	double averageTime = 0.0;

	for (int i = 0; i < 60; i++)
	{
		averageTime += elapsedTimes[i];
	}

	averageTime /= 60;
	return averageTime;
}
