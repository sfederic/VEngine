#pragma once
#include <string>
#include "PickupSpawnData.h"

//Instance holding data over the entirety of the game
struct GameInstance
{
	//this is to set in the editor to know whether to use map files from WorldMaps/ vs GameSaves/
	inline static bool useGameSaves = false;

	inline static int currentHour = 0;
	inline static int currentMinute = 0;

	inline static const std::string startingMap = "church_hall.vmap";
	inline static std::string previousMapMovedFrom = startingMap;

	inline static PickupSpawnData pickupSpawnData;

	//Player stats
	inline static int maxPlayerActionPoints = 10;

	static void ProgressTime();
	static void ResetTime();
};
