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
	props.Add("Energy", &weaponData.energyCost);
	props.Add("Attack", &weaponData.attackPoints);
	props.Add("Defend", &weaponData.defendPoints);
	props.Add("Weight", &weaponData.weight);
	return props;
}
