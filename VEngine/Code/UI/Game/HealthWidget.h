#pragma once
#include "../Widget.h"

struct HealthWidget : Widget
{
	int healthPoints = 0;
	int maxHealthPoints = 1;

	virtual void Draw(float deltaTime) override;
};
