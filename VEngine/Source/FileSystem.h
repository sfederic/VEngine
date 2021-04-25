#pragma once

#include <stdio.h>

class FileSystem
{
public:

	void Tick();
	void LoadWorld(const char* levelName);
	void WriteAllActorSystems(class World* world, const char* filename);
	void ReloadAllActorSystems(class World* world, const char* filename);

	FILE* file;
};

extern FileSystem gFileSystem;
