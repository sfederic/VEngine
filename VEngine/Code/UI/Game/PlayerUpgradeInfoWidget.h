#pragma once

#include "../Widget.h"
#include <unordered_map>

//Displays the currently obtained player upgrade info.
class PlayerUpgradeInfoWidget : public Widget
{
public:
	void Draw(float deltaTime) override;

private:
	//Upgrade's name as GameInstance global property to its description.
	std::unordered_map<std::string, std::string> _upgradeDescriptions;
};
