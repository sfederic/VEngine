#pragma once

#include "../Widget.h"

//Right now only shows if player is fatigued.
class PlayerStatusWidget : public Widget
{
public:
	void Draw(float deltaTime) override;
};
