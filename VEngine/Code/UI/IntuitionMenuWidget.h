#pragma once
#include "Widget.h"

//Draws all current known intuitons the player has in a game setting.
struct IntuitionMenuWidget : Widget
{
	virtual void Tick(float deltaTime) override;
};
