#pragma once
#include "Widget.h"

//Shows guard information when enemies are attacking player
struct GuardWidget : Widget
{
	bool guardSuccessful = false;

	virtual void Draw() override;
};
