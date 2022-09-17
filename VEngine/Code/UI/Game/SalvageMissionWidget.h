#pragma once

#include "../Widget.h"

//Displays information about a salvage mission that can be undertaken.
class SalvageMissionWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;
};
