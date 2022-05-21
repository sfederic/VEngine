#include "vpch.h"
#include "Timer.h"

std::vector<TimerItem> timerItems;

void Timer::Tick(float deltaTime)
{
	for (int timerIndex = 0; timerIndex < timerItems.size(); timerIndex++)
	{
		timerItems[timerIndex].currentTime += deltaTime;

		if (timerItems[timerIndex].currentTime > timerItems[timerIndex].endTime)
		{
			timerItems[timerIndex].functionToCall();
		}
	}

	//This looks stupid, but the erase call has to be below the timer function call
	//for when world's load and all timers are cleared.
	for (int timerIndex = 0; timerIndex < timerItems.size(); timerIndex++)
	{
		if (timerItems[timerIndex].currentTime > timerItems[timerIndex].endTime)
		{
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
