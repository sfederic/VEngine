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
	extern std::unordered_map<std::string, TimeFrame*> timeFrames;

	//TODO: source_location is fucking around a bit with being called in-function. Need to figure 
	//out a way to use it.
	void Start(std::source_location location);
	void End(std::source_location location);
	void Reset();
}

#define PROFILE_START Profile::Start(std::source_location::current());
#define PROFILE_END Profile::End(std::source_location::current());
