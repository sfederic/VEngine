#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "mines_gear_showcase.vmap";
std::string GameInstance::previousMapMovedFrom = startingMap;

//Used when continuing from game save files
std::string GameInstance::mapToLoadOnContinue;

bool GameInstance::useGameSaves = false;

//GAME SPECIFIC VARS
static std::string heldPlayerItem;

//Global properties
//...
static bool churchClockLock = false;
static bool churchSwordLock = false;
static bool churchStatueRotateLock = false;
//...

Properties GameInstance::GetGlobalProps()
{
	Properties props("GameInstance");
	props.Add("ChurchClockLock", &churchClockLock);
	props.Add("ChurchSwordLock", &churchSwordLock);
	props.Add("ChurchStatueRotateLock", &churchStatueRotateLock);
	return props;
}

std::string GameInstance::GetHeldPlayerItem()
{
	return heldPlayerItem;
}

void GameInstance::SetHeldPlayerItem(std::string_view heldItem)
{
	heldPlayerItem = heldItem;
}

void GameInstance::ClearHeldPlayerItem()
{
	heldPlayerItem.clear();
}
