#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "church_library.vmap";
std::string GameInstance::previousMapMovedFrom = startingMap;

//Used when continuing from game save files
std::string GameInstance::mapToLoadOnContinue;

std::map<std::string, Memory*> GameInstance::playerMemories;

int GameInstance::maxPlayerActionPoints = 8;

bool GameInstance::useGameSaves = false;

bool GameInstance::isPlayerInMemory = false;
bool GameInstance::playerBackedOutOfMemoryLevel = false;

Transform GameInstance::previousPlayerTransformBeforeEnteringMemory;

//Global properties

Properties GameInstance::GetGlobalProps()
{
	Properties props("GameInstance");
	return props;
}
