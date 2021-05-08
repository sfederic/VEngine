#pragma once

#include <unordered_map>
#include <string>


#define PROFILE_START gProfiler.Start(__func__); 
#define PROFILE_END gProfiler.End(__func__); 

//An period of measurement between Start() and End() calls.
struct TimeFrame
{
	TimeFrame() {}
	TimeFrame(__int64 _startTime)
	{
		startTime = _startTime;
		memset(elapsedTimes, 0.0, sizeof(double) * maxSampleSize);
	}

	void SetElapsedTime();
	double GetAverageTime();

	const static int maxSampleSize = 60;

	double elapsedTimes[maxSampleSize];
	int currentElapsedTimeIndex = 0;
	__int64 startTime;
	__int64 endTime;
};

//Simpler profile calls to push into function head and tails for measurement.
//Due to it working off a map, you can only have one profiling pair per function for now.
class Profiler
{
public:
	void Init();
	void Reset(); //Call at end of each frame in the main loop
	void Start(const char* functionName);
	void End(const char* functionName);

	//Each time frame is a sample between a Begin() and End() call.
	std::unordered_map<std::string, TimeFrame*> timeFrames;
};

extern Profiler gProfiler;