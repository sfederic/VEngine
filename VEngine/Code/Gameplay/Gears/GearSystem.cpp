#include "vpch.h"
#include "GearSystem.h"

void GearSystem::CreateGear(std::string gearName, Gear* gear)
{
	gearMap.emplace(gearName, gear);
}

Gear* GearSystem::GetGear(const std::string gearName)
{
	return gearMap.find(gearName)->second;
}

std::vector<Gear*> GearSystem::GetAllGears()
{
	std::vector<Gear*> gears;
	for (auto& gearPair : gearMap)
	{
		gears.push_back(gearPair.second);
	}
	return gears;
}
