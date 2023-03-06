#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "map_screen.vmap";

//Global properties
static bool ramielDefeated = false;

std::string primaryGear;
std::string secondaryGear;

XMFLOAT3 playerMapScreenPos;

Properties GameInstance::GetGlobalProps()
{
	Properties props("GameInstance");
	props.Add("Continue Map", &mapToLoadOnContinue);

	props.Add("RamielDefeated", &ramielDefeated);

	props.Add("PrimaryGear", &primaryGear);
	props.Add("SecondaryGear", &secondaryGear);

	props.Add("PlayerMapScreenPos", &playerMapScreenPos);

	return props;
}
