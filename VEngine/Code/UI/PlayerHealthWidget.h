#pragma once
#include "Widget.h"

struct PlayerHealthWidget : Widget
{
	int healthPoints = 0;

	virtual void Draw(float deltaTime) override;
};
