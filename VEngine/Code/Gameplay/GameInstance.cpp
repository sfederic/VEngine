#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "cellar.vmap";

//Global properties

Properties GameInstance::GetGlobalProps()
{
	Properties props("GameInstance");
	return props;
}
