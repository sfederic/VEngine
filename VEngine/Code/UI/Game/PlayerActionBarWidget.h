#pragma once

#include "../Widget.h"

//Shows player action points remaining during combat.
class PlayerActionBarWidget : public Widget
{
public:
	int actionPoints = 0;

	virtual void Draw(float deltaTime) override;
};
