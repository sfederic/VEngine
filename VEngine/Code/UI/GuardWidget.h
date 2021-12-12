#pragma once
#include "Widget.h"

//Shows guard information when enemies are attacking player
struct GuardWidget : Widget
{
	virtual void Tick(float deltaTime) override;
};
