#include "vpch.h"
#include "GearSystem.h"

void GearSystem::CreateGear(std::string gearName, PlayerGear* gear)
{
	gearMap.emplace(gearName, gear);
}

PlayerGear* GearSystem::GetGear(const std::string gearName)
{
	return gearMap.find(gearName)->second;
}
