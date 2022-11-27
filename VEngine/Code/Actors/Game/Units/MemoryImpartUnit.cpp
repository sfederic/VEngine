#include "vpch.h"
#include "MemoryImpartUnit.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/Memory.h"

MemoryImpartUnit::MemoryImpartUnit()
{
}

void MemoryImpartUnit::Start()
{
	__super::Start();
}

void MemoryImpartUnit::Tick(float deltaTime)
{
	__super::Tick(deltaTime);
}

void MemoryImpartUnit::ImpartMemoryToPlayerOnAttack()
{
	auto mem = new Memory("testmem",
		"desc",
		"", "",
		"Memories/Symbol1.jpg");
	DisplayImpartedMemoryToPlayerOnAttack(mem);
	GameInstance::AddPlayerMemory(mem);
}
