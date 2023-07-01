#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "mine_main_room.vmap";
std::string GameInstance::previousMapMovedFrom = startingMap;

//Used when continuing from game save files
std::string GameInstance::mapToLoadOnContinue;

bool GameInstance::useGameSaves = false;

//Global properties
static bool crystal0 = true;

Properties GameInstance::GetGlobalProps()
{
	Properties props("GameInstance");
	props.Add("Crystal0", &crystal0);
	return props;
}
