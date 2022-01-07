#include "Timer.h"

void Timer::Tick(float deltaTime)
{
	for (int timerIndex = 0; timerIndex < timerItems.size(); timerIndex++)
	{
		timerItems[timerIndex].currentTime += deltaTime;

		if (timerItems[timerIndex].currentTime > timerItems[timerIndex].endTime)
		{
			timerItems[timerIndex].functionToCall();
			timerItems.erase(timerItems.begin() + timerIndex);
		}
	}
}

void Timer::Cleanup()
{
	timerItems.clear();
}

TimerItem* Timer::SetTimer(float duration, std::function<void()> functionToCall)
{
	TimerItem timerItem = {};
	timerItem.endTime = duration;
	timerItem.currentTime = 0.0f;
	timerItem.functionToCall = functionToCall;

	timerItems.push_back(timerItem);

	return &timerItems.back();
}
