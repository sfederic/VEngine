#include "vpch.h"
#include "MemoryMerger.h"
#include "Gameplay/Memory.h"
#include "TestMergedMemory.h"

std::map<std::string, Memory*> mergerMemories;

void MemoryMerger::Init()
{
	mergerMemories.emplace("Symbol 1|Symbol2", new TestMergedMemory();
}

void MemoryMerger::MergeMemories(std::string memoryName1, std::string memoryName2)
{

}
