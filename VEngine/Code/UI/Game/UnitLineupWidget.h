#pragma once
#include "../Widget.h"

//Shows units in battle so the player can click on them to show stats and movement/attack range.
struct UnitLineupWidget : Widget
{
	virtual void Draw(float deltaTime) override;
};
