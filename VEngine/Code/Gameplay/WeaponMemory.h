#pragma once
#include "Memory.h"
#include "WeaponData.h"

//Memory for holding weapon data
struct WeaponMemory : Memory
{
	WeaponData weaponData;
};
