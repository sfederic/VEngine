#pragma once

#include "../Widget.h"

//Displays the currently obtained player upgrade info.
class PlayerUpgradeInfoWidget : public Widget
{
public:
	void Draw(float deltaTime) override;
};
