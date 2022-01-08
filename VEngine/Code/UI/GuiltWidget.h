#pragma once
#include "Widget.h"

//Displays a 'dirt mask' relative to the player's guilt over combat
struct GuiltWidget : Widget
{
	virtual void Draw(float deltaTime) override;
};
