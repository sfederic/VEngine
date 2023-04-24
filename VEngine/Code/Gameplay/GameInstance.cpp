#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "test.vmap";

//Global properties

Properties GameInstance::GetGlobalProps()
{
	Properties props("GameInstance");
	return props;
}
