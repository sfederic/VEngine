#include "vpch.h"
#include "Gear.h"
#include "GearSystem.h"

Gear::Gear(std::string_view name_)
{
	name = name_;
	GearSystem::Get().CreateGear(name, this);
}
