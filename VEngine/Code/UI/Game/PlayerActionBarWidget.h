#pragma once

#include "../Widget.h"

//Shows player action points remaining during combat.
class PlayerActionBarWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;

	int actionPoints = 0;
};
