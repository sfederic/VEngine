#pragma once
#include <functional>

struct TimerItem
{
	double endTime;
	double currentTime;
	std::function<void()> functionToCall;
};

namespace Timer
{
	void Tick(float deltaTime);
	void Cleanup();
	TimerItem* SetTimer(float duration, std::function<void()> functionToCall);
};
