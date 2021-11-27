#pragma once
#include "Widget.h"

struct HealthWidget : Widget
{
	int* healthPoints = nullptr;

	virtual void Tick(float deltaTime) override;
	void DrawHealth();
};
