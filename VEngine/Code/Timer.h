#pragma once
#include <vector>
#include <functional>

struct TimerItem
{
	double endTime;
	double currentTime;
	std::function<void()> functionToCall;
};

struct Timer
{
	inline static std::vector<TimerItem> timerItems;

	static void Tick(float deltaTime);
	static void Cleanup();
	static TimerItem* SetTimer(float duration, std::function<void()> functionToCall);
};
