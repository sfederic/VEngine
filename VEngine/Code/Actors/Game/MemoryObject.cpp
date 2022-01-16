#include "MemoryObject.h"

MemoryObject::MemoryObject()
{
}

void MemoryObject::Start()
{
	__super::Start();
}

void MemoryObject::Tick(float deltaTime)
{
	__super::Tick(deltaTime);
}

Properties MemoryObject::GetProps()
{
	auto props = __super::GetProps();
	return props;
}
