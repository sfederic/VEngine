#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "aqueduct_room.vmap";
std::string GameInstance::previousMapMovedFrom = startingMap;

//Used when continuing from game save files
std::string GameInstance::mapToLoadOnContinue;

bool GameInstance::useGameSaves = false;

//Global properties
static int heldCrystalCount = 0;

Properties GameInstance::GetGlobalProps()
{
	Properties props("GameInstance");
	props.Add("HeldCrystalCount", &heldCrystalCount);
	return props;
}
