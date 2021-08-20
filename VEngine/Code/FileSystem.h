#pragma once
#include <string>

struct FileSystem
{
	void WriteAllActorSystems(std::string worldName);
	void LoadWorld(std::string worldName);
};

extern FileSystem fileSystem;
