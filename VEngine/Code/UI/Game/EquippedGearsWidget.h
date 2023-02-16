#pragma once

#include "../Widget.h"

//Display primary and seconary currently equipped gears player has in level.
class EquippedGearsWidget : public Widget
{
public:
	void Draw(float deltaTime) override;
};

