#pragma once

#include "../Widget.h"

class ComboBarWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;

	int comboPoints = 0;

private:
	float comboBarDecreaseTimer = 0.5f;
};
