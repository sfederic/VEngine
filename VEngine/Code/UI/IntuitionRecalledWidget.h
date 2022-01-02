#pragma once
#include "Widget.h"

struct Intuition;

//Shows up when a Intuition is "checked" to let the player know what's triggering new interacts/dialogue/etc.
struct IntuitionRecalledWidget : Widget
{
	Intuition* recalledIntuition = nullptr;

	virtual void Draw(float deltaTime) override;
};
