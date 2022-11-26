#include "vpch.h"
#include "MemoryMerger.h"
#include "Gameplay/Memory.h"
#include "Gameplay/GameInstance.h"
#include "TestMergedMemory.h"

std::map<std::string, Memory*> mergerMemories;

void MemoryMerger::Init()
{
	mergerMemories.emplace("Symbol 1|Symbol 2", new TestMergedMemory(
		"TestMerge",
		"No desc.",
		"",
		"",
		"Memories/Symbol1.jpg"));
}

void MemoryMerger::MergeMemories(const std::string memoryName1, const std::string memoryName2)
{
	const std::string mergedNameKey = memoryName1 + "|" + memoryName2;
	auto foundMemoryIt = mergerMemories.find(mergedNameKey);
	if (foundMemoryIt != mergerMemories.end())
	{
		Memory* foundMemory = foundMemoryIt->second;
		GameInstance::AddPlayerMemory(foundMemory);
	}
}
