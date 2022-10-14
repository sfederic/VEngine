#pragma once

#include <string>
#include "Properties.h"

struct Memory;

//Instance holding data over the entirety of the game.
//GameInstane is also used as a global save file of sorts, seperate from .vmaps.
struct GameInstance
{
	//this is to set in the editor to know whether to use map files from WorldMaps/ vs GameSaves/
	inline static bool useGameSaves = false;

	inline static int currentHour = 0;
	inline static int currentMinute = 0;

	inline static std::string startingMap = "catacomb_3.vmap";
	inline static std::string previousMapMovedFrom = startingMap;

	//Used when continuing from game save files
	inline static std::string mapToLoadOnContinue;

	//Player stats
	inline static int maxPlayerActionPoints = 10;

	inline static std::map<std::string, Memory*> playerMemories;
	static void DeletePlayerMemories();

	static void ProgressTime();
	static void ResetTime();

	static void AddGuilt();

	static Properties GetInstanceSaveData();
};
