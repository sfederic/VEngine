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

void GameInstance::ResetTime()
{
	currentHour = 0;
	currentMinute = 0;
}
