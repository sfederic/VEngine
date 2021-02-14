#pragma once

#include <vector>
#include <string>

//An period of measurement between Start() and End() calls.
struct TimeFrame
{
	TimeFrame(const char* _functionName, __int64 _startTime)
	{
		functionName = (wchar_t*)_functionName;
		startTime = _startTime;
		elapsedTime = 0.0;
	}

	double elapsedTime;
	__int64 startTime;
	__int64 endTime;
	std::wstring functionName;
};

//Simpler profile calls to push into function head and tails for measurement.
class Profiler
{
public:
	static void Start();
	static void End();

	static std::vector<TimeFrame> timeFrames;
};
