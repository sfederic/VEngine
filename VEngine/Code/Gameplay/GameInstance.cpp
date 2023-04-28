#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "test.vmap";
std::string GameInstance::previousMapMovedFrom = startingMap;

//Used when continuing from game save files
std::string GameInstance::mapToLoadOnContinue;

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
