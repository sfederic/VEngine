#pragma once
#include "../Widget.h"

//Shows hour and minutes for time of day during gameplay
struct TimeOfDayWidget : Widget
{
	virtual void Draw(float deltaTime) override;
};
