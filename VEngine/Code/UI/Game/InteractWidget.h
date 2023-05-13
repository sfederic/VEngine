#pragma once

#include "../Widget.h"

//Displays prompt to interact with a GridActor.
class InteractWidget : public Widget
{
public:
	std::wstring interactText;
	void Draw(float deltaTime) override;
};
