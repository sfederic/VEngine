#pragma once
#include "Widget.h"

//TODO: keep this widget around for now, NPCs are just using Widget for now, see if these is needed later.

//Shows dialogue text for actors
struct DialogueWidget : Widget
{
	virtual void Tick(float deltaTime) override;
};
