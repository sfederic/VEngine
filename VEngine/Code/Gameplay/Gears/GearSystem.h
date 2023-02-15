#pragma once

#include <string>

class PlayerGear;

namespace GearSystem
{
	void CreateGear(std::string gearName, PlayerGear* gear);
	PlayerGear* GetGear(const std::string gearName);
};
