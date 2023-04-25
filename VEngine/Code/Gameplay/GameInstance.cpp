#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "church_fire_room.vmap";
std::string GameInstance::previousMapMovedFrom = startingMap;

//Used when continuing from game save files
std::string GameInstance::mapToLoadOnContinue;

std::map<std::string, Memory*> GameInstance::playerMemories;

int GameInstance::maxPlayerActionPoints = 8;

bool GameInstance::useGameSaves = false;

//Global properties

Properties GameInstance::GetGlobalProps()
{
	Properties props("GameInstance");
	return props;
}
