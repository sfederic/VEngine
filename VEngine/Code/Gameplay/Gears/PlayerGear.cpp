#include "vpch.h"
#include "PlayerGear.h"
#include "GearSystem.h"

PlayerGear::PlayerGear(std::string name_)
{
	name = name_;
	GearSystem::Get().CreateGear(name, this);
}
