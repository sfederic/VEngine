#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "canyon.vmap";

Properties GameInstance::GetInstanceSaveData()
{
	Properties props("GameInstance");
	props.Add("Continue Map", &mapToLoadOnContinue);
	return props;
}
