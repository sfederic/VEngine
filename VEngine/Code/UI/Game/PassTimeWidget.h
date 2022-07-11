#pragma once
#include "../Widget.h"

//Let's player change time of day via UI input. Only hours are allowed to be changed, minute
//input was too difficult to do, and GridActors only change based on the hour anyway.
struct PassTimeWidget : Widget
{
	int hoursToPass = 0;

	bool confirmClicked = false;

	virtual void Draw(float deltaTime) override;
	void ConfirmPassageOfTime();
};
