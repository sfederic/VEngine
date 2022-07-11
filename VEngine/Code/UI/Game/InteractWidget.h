#pragma once
#include "../Widget.h"

struct InteractWidget : Widget
{
	std::wstring interactText;

	virtual void Draw(float deltaTime) override;
};
