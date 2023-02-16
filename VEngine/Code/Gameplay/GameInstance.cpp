#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "map_screen.vmap";

//Global properties
static bool ramielDefeated = false;

std::string equippedGear0;
std::string equippedGear1;

Properties GameInstance::GetGlobalProps()
{
	Properties props("GameInstance");
	props.Add("Continue Map", &mapToLoadOnContinue);

	props.Add("RamielDefeated", &ramielDefeated);

	props.Add("EquippedGear0", &equippedGear0);
	props.Add("EquippedGear1", &equippedGear1);

	return props;
}
