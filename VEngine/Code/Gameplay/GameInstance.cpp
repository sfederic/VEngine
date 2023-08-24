#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "millennium_puzzle_1.vmap";
std::string GameInstance::previousMapMovedFrom = startingMap;

//Used when continuing from game save files
std::string GameInstance::mapToLoadOnContinue;

bool GameInstance::useGameSaves = false;

//GAME SPECIFIC VARS
static std::string heldPlayerItem;

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
