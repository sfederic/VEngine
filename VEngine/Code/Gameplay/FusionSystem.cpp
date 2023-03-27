#include "vpch.h"
#include "FusionSystem.h"
#include "Actors/Game/FusionUnits/DoubleAttackFusionUnit.h"
#include "Core/Log.h"

std::map<std::pair<std::string, std::string>, std::function<PlayerUnit*()>> fusionUnitsMap;

void FusionSystem::Init()
{
	fusionUnitsMap.emplace(std::make_pair("AttackUnit", "AttackUnit"), []() { return DoubleAttackFusionUnit::system.Add(); });
}

PlayerUnit* FusionSystem::Fuse(std::string unit1TypeName, std::string unit2TypeName)
{
	auto typenamePair = std::make_pair(unit1TypeName, unit2TypeName);
	auto fusionUnitsMapIt = fusionUnitsMap.find(typenamePair);
	if (fusionUnitsMapIt != fusionUnitsMap.end())
	{
		PlayerUnit* newFusedUnit = fusionUnitsMapIt->second();
		return newFusedUnit;
	}

	Log("Could not fuse PlayerUnits of types [%s] and [%s]", unit1TypeName.c_str(), unit1TypeName.c_str());
	return nullptr;
}
