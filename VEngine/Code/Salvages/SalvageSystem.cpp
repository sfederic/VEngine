#include "vpch.h"
#include "SalvageSystem.h"
#include "SalvageMission.h"
#include "TestSalvage.h"

SalvageMission* currentSalvageMission = new TestSalvage();

SalvageMission* SalvageSystem::GetCurrentSalvageMission()
{
	assert(currentSalvageMission);
	return currentSalvageMission;
}

bool SalvageSystem::CurrentSalvageContainsPhotoTag(std::string photoTag)
{
	return currentSalvageMission->ContainsPhotoTag(photoTag);
}
