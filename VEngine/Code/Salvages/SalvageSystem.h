#pragma once

#include <string>

class SalvageMission;

namespace SalvageSystem
{
	SalvageMission* GetCurrentSalvageMission();
	bool CurrentSalvageContainsPhotoTag(std::string photoTag);
};
