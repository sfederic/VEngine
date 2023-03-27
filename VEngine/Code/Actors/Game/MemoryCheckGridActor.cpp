#include "vpch.h"
#include "MemoryCheckGridActor.h"
#include "Gameplay/GameUtils.h"
#include "Core/Log.h"

Properties MemoryCheckGridActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = "MemoryCheckGridActor";
	props.Add("Memory To Check", &memoryToCheck);
	return props;
}

bool MemoryCheckGridActor::CheckMemory()
{
	if (GameUtils::CheckIfMemoryExists(memoryToCheck))
	{
		Log("yep");
		return true;
	}

	return false;
}
