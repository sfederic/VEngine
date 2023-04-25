#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "church_fire_room.vmap";

//Global properties

Properties GameInstance::GetGlobalProps()
{
	Properties props("GameInstance");
	return props;
}
