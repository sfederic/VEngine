#pragma once
#include <string>

struct FileSystem
{
	void WriteAllActorSystems();
	void LoadWorld(std::string worldName);
	void ReloadCurrentWorld();

	//Creates an equivalent map save to load during gameplay (to avoid having a seperate save file format)
	void CreateGameplayWorldSave(std::string worldName);
};

extern FileSystem fileSystem;
