#pragma once
#include <string>

struct FileSystem
{
	void WriteAllActorSystems();
	void LoadWorld(std::string worldName);
	void ReloadCurrentWorld();
};

extern FileSystem fileSystem;
