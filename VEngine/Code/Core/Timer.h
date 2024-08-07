#pragma once

#include <functional>

//Todo: Could Timers be replaced with C++'s coroutines?

struct TimerItem
{
	std::function<void()> functionToCall;
	double endTime = 0.0;
	double currentTime = 0.0;
	bool loop = false;
};

namespace Timer
{
	void Tick(float deltaTime);
	void Cleanup();
	TimerItem* SetTimer(float duration, std::function<void()> functionToCall, bool loop = false);
};
