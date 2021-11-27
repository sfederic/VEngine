#pragma once
#include "Widget.h"
#include <string>

struct InteractWidget : Widget
{
	std::wstring interactText;

	virtual void Tick(float deltaTime) override;
};
