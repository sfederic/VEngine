#pragma once

#include "../Widget.h"

//Displays prompt to pick up a Pickup GridActor.
class PickupWidget : public Widget
{
public:
	void Draw(float deltaTime) override;
};
