#pragma once
#include "Widget.h"

//@Todo: want a widget item control that when mouse over, show an image of the intuition or more text
//detail so the player can remember intuitions better.

//Draws all current known intuitons the player has in a game setting.
struct IntuitionMenuWidget : Widget
{
	virtual void Tick(float deltaTime) override;
};
