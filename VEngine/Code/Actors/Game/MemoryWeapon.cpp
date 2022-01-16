#include "MemoryWeapon.h"

MemoryWeapon::MemoryWeapon()
{
}

void MemoryWeapon::Start()
{
	__super::Start();
}

void MemoryWeapon::Tick(float deltaTime)
{
	__super::Tick(deltaTime);
}

Properties MemoryWeapon::GetProps()
{
	auto props = __super::GetProps();
	props.Add("Energy", &weaponData.energyCost);
	props.Add("Attack", &weaponData.attackPoints);
	props.Add("Defend", &weaponData.defendPoints);
	props.Add("Weight", &weaponData.weight);
	return props;
}
