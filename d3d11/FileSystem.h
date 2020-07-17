#pragma once

#include <stdio.h>

//Wondering whether this should all be tied to World class or not
//Will this sort of structure help with async stuff?
class FileSystem
{
public:

	void WriteAllActorSystems(class World* world, const char* filename);
	void ReadAllActorSystems(class World* world, const char* filename);

	FILE* file;
};

static FileSystem g_FileSystem;
