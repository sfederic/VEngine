#pragma once
#include "Widget.h"

struct HealthWidget : Widget
{
	int healthPoints = 0;

	virtual void Draw(float deltaTime) override;
};
