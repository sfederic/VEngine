#pragma once

#include <vector>
#include <string>

//An period of measurement between Start() and End() calls.
struct TimeFrame
{
	TimeFrame(__int64 _startTime)
	{
		startTime = _startTime;
		memset(elapsedTimes, 0.0, sizeof(double) * 60);
	}

	void SetElapsedTime();
	double GetAverageTime();

	double elapsedTimes[60];
	int currentElapsedTimeIndex = 0;
	__int64 startTime;
	__int64 endTime;
	std::wstring functionName;
};

//Simpler profile calls to push into function head and tails for measurement.
class Profiler
{
public:
	void Init();
	void Clean(); //Call at end of each frame.
	void Start();
	void End(const wchar_t* functionName);

	std::vector<TimeFrame> timeFrames;
};

extern Profiler gProfiler;