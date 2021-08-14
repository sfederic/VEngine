#include "Profile.h"
#include <Windows.h>
#include <cassert>
#include <source_location>

std::unordered_map<std::string, TimeFrame*> Profile::timeFrames;

TimeFrame::TimeFrame() 
{
}

TimeFrame::TimeFrame(__int64 _startTime)
{
	startTime = _startTime;
}

void TimeFrame::SetElapsedTime()
{
	__int64 cpuFreq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cpuFreq);
	double ticks = 1.0 / (double)cpuFreq;

	if (currentElapsedTimeIndex < maxSampleSize)
	{
		elapsedTimes[currentElapsedTimeIndex] = ticks * (double)(endTime - startTime);
		currentElapsedTimeIndex++;
	}
	else
	{
		currentElapsedTimeIndex = 0;
	}

	endTime = 0;
	startTime = 0;
}

double TimeFrame::GetAverageTime()
{
	double averageTime = 0.0;

	for (int i = 0; i < maxSampleSize; i++)
	{
		averageTime += elapsedTimes[i];
	}

	double retVal = averageTime / (double)maxSampleSize;
	return retVal;
}

void Profile::Reset()
{
	timeFrames.empty();
}

void Profile::Start(std::source_location location)
{
	__int64 startTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	auto functionName = location.function_name();

	auto timeFramesIt = timeFrames.find(functionName);
	if (timeFramesIt == timeFrames.end())
	{
		TimeFrame* timeFrame = new TimeFrame(startTime);
		timeFrames[functionName] = timeFrame;
	}
	else
	{
		timeFrames[functionName]->startTime = startTime;
	}
}

void Profile::End(std::source_location location)
{
	__int64 endTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&endTime);

	auto functionName = location.function_name();

	auto timeFramesIt = timeFrames.find(functionName);
	assert(timeFramesIt->second && "Check for matching PROFILE_START in function.");

	TimeFrame* currentTimeFrame = timeFramesIt->second;
	currentTimeFrame->endTime = endTime;
	currentTimeFrame->SetElapsedTime();
}
