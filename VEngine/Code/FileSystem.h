#pragma once
#include <string>

struct FileSystem
{
	void WriteAllActorSystems(std::string levelName);
};

extern FileSystem fileSystem;
