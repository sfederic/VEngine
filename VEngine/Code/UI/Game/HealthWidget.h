#pragma once
#include "../Widget.h"

class HealthWidget : public Widget
{
public:
	HealthWidget() : Widget("HealthWidget") {}
	int healthPoints = 0;
	int maxHealthPoints = 1;

	virtual void Draw(float deltaTime) override;
};
