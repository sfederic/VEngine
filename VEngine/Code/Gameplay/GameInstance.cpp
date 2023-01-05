#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "test2.vmap";

Properties GameInstance::GetInstanceSaveData()
{
	Properties props("GameInstance");
	props.Add("Continue Map", &mapToLoadOnContinue);
	return props;
}
