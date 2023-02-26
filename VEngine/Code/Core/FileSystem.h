#pragma once

#include <string>

namespace FileSystem
{
	void SerialiseAllSystems();

	void WriteAllSystemsToBinary();
	void ReadAllSystemsFromBinary();

	void LoadWorld(std::string worldName);
	void ReloadCurrentWorld();

	//Creates an equivalent map save to load during gameplay (to avoid having a seperate save file format)
	void CreateGameplayWorldSave(std::string worldName);

	void ResetWorldState();

	void SetDeferredWorldLoad(const std::string_view filename);

	//called at end of the frame to avoid world loads happening inside Tick/Draw functions.
	void DeferredWorldLoad();
};
