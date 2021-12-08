#pragma once
#include <string>
#include "Properties.h"

//Instance holding data over the entirety of the game
struct GameInstance
{
	inline static int currentHour;
	inline static int currentMinute;

	inline static const std::string startingMap = "church_room1.vmap";

	inline static std::string previousMapMovedFrom = startingMap;

	inline static std::vector<Properties> pickupProps;

	static void ProgressTime();
	static void ResetTime();
};
