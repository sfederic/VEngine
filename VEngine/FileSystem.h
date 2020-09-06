#pragma once

#include <stdio.h>

class FileSystem
{
public:

	void Tick();
	void WriteAllActorSystems(class World* world, const char* filename);
	void ReadAllActorSystems(class World* world, const char* filename);

	FILE* file;
};

extern FileSystem gFileSystem;
