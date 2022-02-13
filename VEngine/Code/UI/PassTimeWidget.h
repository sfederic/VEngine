#pragma once
#include "Widget.h"

//Let's player change time of day via UI input
struct PassTimeWidget : Widget
{
	int tempHour = 0;
	int tempMinute = 0;

	virtual void Draw(float deltaTime) override;
	void ConfirmPassageOfTime();
};
