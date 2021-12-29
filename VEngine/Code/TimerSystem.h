#pragma once
#include <vector>
#include <functional>

struct TimerItem
{
	double endTime;
	double currentTime;
	std::function<void()> functionToCall;
};

struct TimerSystem
{
	std::vector<TimerItem> timerItems;

	void Tick(float deltaTime);
	void Cleanup();
	void SetTimer(float duration, std::function<void()> functionToCall);
};

extern TimerSystem timerSystem;
