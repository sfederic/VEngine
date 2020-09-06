#pragma once

#include <vector>

struct TimerItem
{
	double endTime;
	double currentTime;
	void (*functionToCall)();
};

class TimerSystem
{
public:
	void Tick(float deltaTime);
	void SetTimer(double duration, void(*functionToCall)());

	std::vector<TimerItem> timerItems;
};

extern TimerSystem gTimerSystem;