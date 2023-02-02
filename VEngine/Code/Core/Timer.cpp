#include "vpch.h"
#include "Timer.h"

std::vector<TimerItem> timerItems;

void Timer::Tick(float deltaTime)
{
	for(auto& timerItem : timerItems)
	{
		timerItem.currentTime += deltaTime;

		if (timerItem.currentTime > timerItem.endTime)
		{
			timerItem.functionToCall();

			if (timerItem.loop)
			{
				timerItem.currentTime = 0.0;
			}
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

TimerItem* Timer::SetTimer(float duration, std::function<void()> functionToCall, bool loop)
{
	TimerItem timerItem = {};
	timerItem.endTime = duration;
	timerItem.currentTime = 0.0f;
	timerItem.loop = loop;
	timerItem.functionToCall = functionToCall;

	timerItems.emplace_back(timerItem);

	return &timerItems.back();
}
