#pragma once
#include "../Widget.h"

//Shows player action points remaining during combat.
struct PlayerActionBarWidget : Widget
{
	int actionPoints = 0;

	virtual void Draw(float deltaTime) override;
};
