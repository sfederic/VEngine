#include "World.h"

//Returns the current active world
World* GetWorld()
{
	return &g_CurrentWorld;
}