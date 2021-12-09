#pragma once
#include <string>
#include "PickupSpawnData.h"

//Instance holding data over the entirety of the game
struct GameInstance
{
	inline static int currentHour = 8;
	inline static int currentMinute = 0;

	inline static const std::string startingMap = "church_room1.vmap";
	inline static std::string previousMapMovedFrom = startingMap;

	inline static PickupSpawnData pickupSpawnData;

	static void ProgressTime();
	static void ResetTime();
};
