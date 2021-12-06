#pragma once
#include <string>

//Instance holding data over the entirety of the game
struct GameInstance
{
	inline static int currentHour;
	inline static int currentMinute;

	inline static const std::string startingMap = "church_hall.vmap";

	inline static std::string previousMapMovedFrom = startingMap;

	static void ProgressTime();
	static void ResetTime();
};
