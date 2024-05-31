#pragma once

#include "../Widget.h"

class PauseGameWidget : public Widget
{
public:
	void Draw(float deltaTime) override;
	void ToggleGamePaused();
};
