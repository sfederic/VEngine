#pragma once

#include "Widget.h"

//@Todo: the fucking UISystem keeps cleaning up widgets on swpchain resize.
//make a bool in Widget to denote not to delete shit.

class UVPaintWidget : public Widget
{
public:
	void Draw(float deltaTime) override;

private:
	void UpdateWorldEditor();

	float previousMouseX = 0.f;
	float previousMouseY = 0.f;
};
