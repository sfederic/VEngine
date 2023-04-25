#include "vpch.h"
#include "MemoryEntranceTrigger.h"
#include "Components/BoxTriggerComponent.h"

MemoryEntranceTrigger::MemoryEntranceTrigger()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent = boxTrigger;
}

Properties MemoryEntranceTrigger::GetProps()
{
	auto props = __super::GetProps();
	props.Add("MemoryLevel", &memoryLevelToMoveTo);
	return props;
}
