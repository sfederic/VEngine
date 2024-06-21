#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "catacombs_wall_puzzle.vmap";
std::string GameInstance::previousMapMovedFrom = startingMap;

bool GameInstance::useGameSaves = false;

//GAME SPECIFIC VARS
static std::string heldPlayerItem;

//Global properties
//...

//Used when continuing from game save files
static std::string mapToLoadOnContinue;
//...

Properties GameInstance::GetGlobalProps()
{
	Properties props("GameInstance");
	props.Add("MapToLoadOnContinue", &mapToLoadOnContinue);
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

std::string GameInstance::GetContinueMapName()
{
	return mapToLoadOnContinue;
}

void GameInstance::SetContinueMapName(std::string_view name)
{
	mapToLoadOnContinue = name;
}
