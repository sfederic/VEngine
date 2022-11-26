#include "vpch.h"
#include "GameInstance.h"
#include "Memory.h"

std::string GameInstance::startingMap = "test.vmap";

void GameInstance::DeletePlayerMemories()
{
	playerMemories.clear();
}

void GameInstance::AddPlayerMemory(Memory* memory)
{
	playerMemories.emplace(memory->name, memory);
}

Properties GameInstance::GetInstanceSaveData()
{
	Properties props("GameInstance");
	props.AddProp(mapToLoadOnContinue);
	return props;
}
