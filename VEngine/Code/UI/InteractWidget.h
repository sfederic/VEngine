#pragma once
#include "Widget.h"
#include <string>

struct InteractWidget : Widget
{
	std::wstring interactText;

	virtual void Draw(float deltaTime) override;
	void SetText(std::string text);
};
