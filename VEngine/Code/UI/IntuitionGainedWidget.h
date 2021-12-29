#pragma once
#include "Widget.h"

struct Intuition;

//Notification to show when an intuition is gained during gameplay.
struct IntuitionGainedWidget : Widget
{
	Intuition* intuitionToDisplay = nullptr;

	virtual void Draw() override;
	void DestroyAndRemove();
};
