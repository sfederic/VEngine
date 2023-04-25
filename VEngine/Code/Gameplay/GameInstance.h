#pragma once

#include <string>
#include "Core/Properties.h"

struct Memory;

//Instance holding data over the entirety of the game.
//GameInstance is also used as a global save file of sorts, separate from .vmaps.
namespace GameInstance
{
	extern std::string startingMap;
	extern std::string previousMapMovedFrom;

	//Used when continuing from game save files
	extern std::string mapToLoadOnContinue;

	extern std::map<std::string, Memory*> playerMemories;

	extern int maxPlayerActionPoints;

	extern bool useGameSaves;

	//Global save data
	Properties GetGlobalProps();

	template <typename T>
	T* GetGlobalProp(const std::string name)
	{
		Properties globalProps = GetGlobalProps();
		T* data = globalProps.GetData<T>(name);
		return data;
	}

	template <typename T>
	void SetGlobalProp(const std::string name, T value)
	{
		Properties globalProps = GetGlobalProps();
		T* data = globalProps.GetData<T>(name);
		*data = value;
	}
};
