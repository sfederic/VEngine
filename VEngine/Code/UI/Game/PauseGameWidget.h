#pragma once

#include "../Widget.h"

class PauseGameWidget : public Widget
{
public:
	PauseGameWidget() : Widget("PauseGameWidget") {}
	void Draw(float deltaTime) override;
	void ToggleGamePaused();
};
