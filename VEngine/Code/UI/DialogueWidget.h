#pragma once
#include "Widget.h"

//Shows dialogue text for actors
struct DialogueWidget : Widget
{
	virtual void Tick(float deltaTime) override;
};
