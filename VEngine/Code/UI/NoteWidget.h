#pragma once
#include "Widget.h"

struct NoteWidget : Widget
{
	std::wstring noteText;

	virtual void Draw(float deltaTime) override;
};
