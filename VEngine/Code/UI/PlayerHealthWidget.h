#pragma once
#include "Widget.h"

struct PlayerHealthWidget : Widget
{
	int healthPoints = 0;

	virtual void Tick(float deltaTime) override;
};
