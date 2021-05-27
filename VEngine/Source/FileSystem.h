#pragma once

#include <stdio.h>
#include <string>

class FileSystem
{
public:

	void Tick();
	void LoadWorld(const std::string& levelName);
	void WriteAllActorSystems(class World* world, const std::string& levelName);

	FILE* file;
};

extern FileSystem gFileSystem;
