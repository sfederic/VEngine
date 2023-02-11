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

	inline static bool bossDefeated = false;

	//Global save data
	static Properties GetGlobalProps();

	template <typename T>
	static T& GetGlobalProp(const std::string name)
	{
		Properties globalProps = GetGlobalProps();
		T* data = globalProps.GetData<T>(name);
		return *data;
	}
};
