#pragma once

#include <set>
#include <string>

//Base class holding data linked to salvage quests player can undertake.
class SalvageMission
{
public:
	bool ContainsPhotoTag(std::string photoTag);

	std::set<std::string> GetAllPhotoTags() { return photoTagsToCapture; }

protected:
	std::set<std::string> photoTagsToCapture;
};
