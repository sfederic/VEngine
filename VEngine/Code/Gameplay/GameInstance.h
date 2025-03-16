#pragma once

import <string>;
#include <set>
import Core.Properties;
#include "Core/Log.h"

//Instance holding data over the entirety of the game.
//GameInstance is also used as a global save file of sorts, separate from .vmaps.
namespace GameInstance
{
	extern std::string startingMap;
	extern std::string previousMapMovedFrom;

	extern bool useGameSaves;

	//Global save data
	Properties GetGlobalProps();

	//GAME SPECIFIC FUNCS
	std::string GetHeldPlayerItem();
	void SetHeldPlayerItem(std::string_view heldItem);
	void ClearHeldPlayerItem();

	std::set<std::string> GetAllPlayerUpgradeNames();

	template <typename T>
	T* GetGlobalProp(const std::string name)
	{
		Properties globalProps = GetGlobalProps();
		if (globalProps.Find(name))
		{
			T* data = globalProps.GetData<T>(name);
			return data;
		}

		Log("Global property [%s] not found.", name.c_str());
		return nullptr;
	}

	template <typename T>
	void SetGlobalProp(const std::string name, T value)
	{
		Properties globalProps = GetGlobalProps();
		T* data = globalProps.GetData<T>(name);
		*data = value;
	}

	std::string GetContinueMapName();
	void SetContinueMapName(std::string_view name);
};
