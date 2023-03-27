#pragma once

#include <string>
#include "Core/Properties.h"

class Memory;

//Instance holding data over the entirety of the game.
//GameInstane is also used as a global save file of sorts, seperate from .vmaps.
struct GameInstance
{
	static std::string startingMap;
	inline static std::string previousMapMovedFrom = startingMap;

	//Used when continuing from game save files
	inline static std::string mapToLoadOnContinue;

	inline static std::map<std::string, Memory*> playerMemories;

	inline static int maxPlayerActionPoints = 8;

	inline static bool useGameSaves = false;

	//Global save data
	static Properties GetGlobalProps();

	template <typename T>
	static T* GetGlobalProp(const std::string name)
	{
		Properties globalProps = GetGlobalProps();
		T* data = globalProps.GetData<T>(name);
		return data;
	}

	template <typename T>
	static void SetGlobalProp(const std::string name, T value)
	{
		Properties globalProps = GetGlobalProps();
		T* data = globalProps.GetData<T>(name);
		*data = value;
	}
};
