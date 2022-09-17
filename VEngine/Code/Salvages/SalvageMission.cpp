#include "vpch.h"
#include "SalvageMission.h"

bool SalvageMission::ContainsPhotoTag(std::string photoTag)
{
	auto photoTagIt = photoTagsToCapture.find(photoTag);
	if (photoTagIt != photoTagsToCapture.end())
	{
		return true;
	}

	return false;
}
