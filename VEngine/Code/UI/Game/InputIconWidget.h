#pragma once

#include "../Widget.h"

//Shows input key icon to act as a tutorial for input.
class InputIconWidget : public Widget
{
public:
	void Draw(float deltaTime) override;
};
