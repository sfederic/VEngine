#include "vpch.h"
#include "MemoryCheckActor.h"
#include "Gameplay/GameUtils.h"
#include "Core/Log.h"

Properties MemoryCheckActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = "MemoryCheckGridActor";
	props.Add("Memory To Check", &memoryToCheck);
	return props;
}

bool MemoryCheckActor::CheckMemory()
{
	if (GameUtils::CheckIfMemoryExists(memoryToCheck))
	{
		Log("yep");
		return true;
	}

	return false;
}
