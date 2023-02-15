#include "vpch.h"
#include "PlayerGear.h"
#include "GearSystem.h"

PlayerGear::PlayerGear(std::string name)
{
	GearSystem::CreateGear(name, this);
}
