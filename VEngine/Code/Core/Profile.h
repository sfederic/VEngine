#pragma once

#include <source_location>
#include <unordered_map>

struct TimeFrame
{
	const static int maxSampleSize = 60; //make this match the FPS of the program

	__int64 startTime = 0;
	__int64 endTime = 0;

	double elapsedTimes[maxSampleSize] {};
	int currentElapsedTimeIndex = 0;
	
	TimeFrame();
	TimeFrame(__int64 _startTime);
	void SetElapsedTime();
	double GetAverageTime();
};

namespace Profile
{
	extern std::unordered_map<std::string, TimeFrame> timeFrames;

	void Start(std::source_location location = std::source_location::current());
	void End(std::source_location location = std::source_location::current());
	void Reset();

	//Quick timing functions that need to be called once off without need for constant profiling.
	__int64 QuickStart();
	double QuickEnd(__int64 startTime);
}
