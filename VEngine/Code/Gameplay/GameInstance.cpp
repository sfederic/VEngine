#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "test3.vmap";

Properties GameInstance::GetGlobalProps()
{
	Properties props("GameInstance");
	props.Add("Continue Map", &mapToLoadOnContinue);
	props.Add("BossDefeated", &bossDefeated);
	return props;
}
