#pragma once
#include "Widget.h"

//Displays player's three current weapons to choose from.
struct WeaponDisplayWidget : Widget
{
	virtual void Draw(float deltaTime) override;
};
