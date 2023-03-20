#pragma once

#include "../Widget.h"

class HealthWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;
	void SetHealth(int health) { healthToDisplay = health; }

private: 
	int healthToDisplay = 0;
};
