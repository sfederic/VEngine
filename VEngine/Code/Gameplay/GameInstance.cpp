#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "dock_town.vmap";

void GameInstance::DeletePlayerMemories()
{
	playerMemories.clear();
}

Properties GameInstance::GetInstanceSaveData()
{
	Properties props("GameInstance");
	props.AddProp(mapToLoadOnContinue);
	return props;
}
