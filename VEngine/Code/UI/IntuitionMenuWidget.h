#pragma once
#include "Widget.h"

struct Intuition;

//Draws all current known intuitons the player has in a game setting.
struct IntuitionMenuWidget : Widget
{
	Intuition* selectedIntuition = nullptr;

	virtual void Draw(float deltaTime) override;
};
