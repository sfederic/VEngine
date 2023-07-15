#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "market_roof_hop.vmap";
std::string GameInstance::previousMapMovedFrom = startingMap;

//Used when continuing from game save files
std::string GameInstance::mapToLoadOnContinue;

bool GameInstance::useGameSaves = false;

//Global properties
static int heldCrystalCount = 0;

//Church
static bool churchCrystal0Active = true;

Properties GameInstance::GetGlobalProps()
{
	Properties props("GameInstance");

	props.Add("HeldCrystalCount", &heldCrystalCount);

	//Church
	props.Add("ChurchCrystal0Active", &churchCrystal0Active);

	return props;
}
