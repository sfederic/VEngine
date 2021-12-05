#include "GameInstance.h"

void GameInstance::ProgressTime()
{
	currentMinute += 15;
	if (currentMinute >= 60)
	{
		currentHour++;
		currentMinute = 0;
	}
}
