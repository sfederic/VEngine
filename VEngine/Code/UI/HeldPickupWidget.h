#pragma once
#include "Widget.h"

//Shows currently held pickup player has
struct HeldPickupWidget : Widget
{
	std::string pickupName;

	virtual void Draw(float deltaTime) override;
};
