#pragma once

#include <string>
#include "Core/Properties.h"

//Instance holding data over the entirety of the game.
//GameInstane is also used as a global save file of sorts, seperate from .vmaps.
struct GameInstance
{
	static std::string startingMap;
	inline static std::string previousMapMovedFrom = startingMap;

	//Used when continuing from game save files
	inline static std::string mapToLoadOnContinue;

	static Properties GetInstanceSaveData();
};
