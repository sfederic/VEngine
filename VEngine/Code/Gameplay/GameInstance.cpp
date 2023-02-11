#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "test3.vmap";

//Global properties
static bool ramielDefeated = false;

Properties GameInstance::GetGlobalProps()
{
	Properties props("GameInstance");
	props.Add("Continue Map", &mapToLoadOnContinue);
	props.Add("RamielDefeated", &ramielDefeated);
	return props;
}
