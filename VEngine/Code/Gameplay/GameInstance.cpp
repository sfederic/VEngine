#include "vpch.h"
#include "GameInstance.h"

std::string GameInstance::startingMap = "test.vmap";

//Global properties
static bool ramielDefeated = false;

static std::string primaryGear;
static std::string secondaryGear;

static DirectX::XMFLOAT3 playerMapScreenPos;

Properties GameInstance::GetGlobalProps()
{
	Properties props("GameInstance");
	props.Add("Continue Map", &mapToLoadOnContinue);

	props.Add("RamielDefeated", &ramielDefeated);

	props.Add("PrimaryGear", &primaryGear);
	props.Add("SecondaryGear", &secondaryGear);

	props.Add("PlayerMapScreenPos", &playerMapScreenPos);

	return props;
}
