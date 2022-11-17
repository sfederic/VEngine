#include "vpch.h"
#include "GameInstance.h"

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
