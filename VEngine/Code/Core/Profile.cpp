import Core.Profile;

#include <Windows.h>

import <unordered_map>;

import Core.VAssert;

//Todo: profiler needs to be moved into its own class
std::unordered_map<std::string, TimeFrame> timeFrames;

void TimeFrame::SetElapsedTime()
{
	__int64 cpuFreq = 0;
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

double TimeFrame::GetAverageTime() const
{
	double averageTime = 0.0;

	for (int i = 0; i < maxSampleSize; i++)
	{
		averageTime += elapsedTimes[i];
	}

	return averageTime / (double)maxSampleSize;
}

void Profile::Reset()
{
	timeFrames.clear();
}

double Profile::GetTotalFrameTime()
{
	double totalTime = 0.0;
	for (auto& [funcName, timeFrame] : timeFrames)
	{
		totalTime += timeFrame.GetAverageTime();
	}
	return totalTime;
}

__int64 Profile::QuickStart()
{
	__int64 startTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
	return startTime;
}

double Profile::QuickEnd(__int64 startTime)
{
	__int64 endTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&endTime);

	__int64 cpuFreq = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cpuFreq);
	double ticks = 1.0 / (double)cpuFreq;

	return ticks * double(endTime - startTime);
}

std::unordered_map<std::string, TimeFrame> Profile::GetTimeFrames()
{
	return timeFrames;
}

void Profile::Start(std::source_location location)
{
	Start(location.function_name());
}

void Profile::Start(std::string funcName)
{
	__int64 startTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	auto timeFramesIt = timeFrames.find(funcName);
	if (timeFramesIt == timeFrames.end())
	{
		TimeFrame timeFrame = TimeFrame(startTime);
		timeFrames[funcName] = timeFrame;
	}
	else
	{
		timeFrames[funcName].startTime = startTime;
	}
}

void Profile::End(std::source_location location)
{
	End(location.function_name());
}

void Profile::End(std::string funcName)
{
	__int64 endTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&endTime);

	auto timeFramesIt = timeFrames.find(funcName);
	VAssert(timeFramesIt != timeFrames.end(), "Check for matching PROFILE_START in function.");

	TimeFrame& currentTimeFrame = timeFramesIt->second;
	currentTimeFrame.endTime = endTime;
	currentTimeFrame.SetElapsedTime();
}
