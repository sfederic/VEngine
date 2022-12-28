#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "space_elevator.vmap";

Properties GameInstance::GetInstanceSaveData()
{
	Properties props("GameInstance");
	props.Add("Continue Map", &mapToLoadOnContinue);
	return props;
}
