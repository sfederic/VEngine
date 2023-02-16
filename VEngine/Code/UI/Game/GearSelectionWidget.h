#pragma once

#include "../Widget.h"

//Widget to scroll through and select player gear for the highlighted level.
class GearSelectionWidget : public Widget
{
public:
	void Draw(float deltaTime) override;
};
