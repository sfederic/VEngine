#include "World.h"

World currentWorld;

//Returns the current active world
World* GetWorld()
{
	return &currentWorld;
}