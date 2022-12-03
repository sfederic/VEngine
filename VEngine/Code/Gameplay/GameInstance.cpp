#include "vpch.h"
#include "GameInstance.h"
#include "Memory.h"

std::string GameInstance::startingMap = "combat_maze_test.vmap";

void GameInstance::DeletePlayerMemories()
{
	playerMemories.clear();
}

void GameInstance::AddPlayerMemory(Memory* memory)
{
	playerMemories.emplace(memory->name, memory);
}

void GameInstance::DeletePlayerMemory(const std::string memoryName)
{
	playerMemories.erase(memoryName);
}

Properties GameInstance::GetInstanceSaveData()
{
	Properties props("GameInstance");
	props.AddProp(mapToLoadOnContinue);
	return props;
}
