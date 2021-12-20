#pragma once
#include <source_location>
#include <unordered_map>

struct TimeFrame
{
	const static int maxSampleSize = 60; //make this match the FPS of the program

	double elapsedTimes[maxSampleSize] {};
	int currentElapsedTimeIndex = 0;
	__int64 startTime = 0;
	__int64 endTime = 0;

	TimeFrame();
	TimeFrame(__int64 _startTime);
	void SetElapsedTime();
	double GetAverageTime();
};

namespace Profile
{
	extern std::unordered_map<std::string, TimeFrame> timeFrames;

	void Start(std::source_location location);
	void End(std::source_location location);
	void Reset();

	//Quick timing functions that need to be called once off without need for constant profiling.
	__int64 QuickStart();
	double QuickEnd(__int64 startTime);
}

#define PROFILE_START Profile::Start(std::source_location::current());
#define PROFILE_END Profile::End(std::source_location::current());
