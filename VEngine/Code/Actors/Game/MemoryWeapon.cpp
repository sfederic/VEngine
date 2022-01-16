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
	props.Add("WeaponEnergy", &weaponData.energyCost);
	props.Add("WeaponAttack", &weaponData.attackPoints);
	props.Add("WeaponDefend", &weaponData.defendPoints);
	props.Add("WeaponWeight", &weaponData.weight);
	return props;
}
