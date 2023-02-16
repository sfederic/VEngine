#include "vpch.h"
#include "GearSystem.h"
#include "Gameplay/GameInstance.h"

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

Gear* GearSystem::GetPrimaryGear()
{
	auto primaryGearName = GameInstance::GetGlobalProp<std::string>("PrimaryGear");
	auto gear = GetGear(*primaryGearName);
	return gear;
}

Gear* GearSystem::GetSecondaryGear()
{
	auto secondaryGearName = GameInstance::GetGlobalProp<std::string>("SecondaryGear");
	auto gear = GetGear(*secondaryGearName);
	return gear;
}
