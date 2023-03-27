#pragma once

#include <string>

class PlayerUnit;

//System that fuses PlayerUnits by using their typename as a key into a map of FusionUnits.
namespace FusionSystem
{
	void Init();
	PlayerUnit* Fuse(std::string unit1TypeName, std::string unit2TypeName);
}
