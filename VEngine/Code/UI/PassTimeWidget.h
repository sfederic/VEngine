#pragma once
#include "Widget.h"

//Let's player change time of day via UI input
struct PassTimeWidget : Widget
{
	int hoursToPass = 0;

	bool confirmClicked = false;

	virtual void Draw(float deltaTime) override;
	void ConfirmPassageOfTime();
};
