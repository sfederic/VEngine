#pragma once
#include "Widget.h"

//Displays a note in world position. Mainly used for debugging notes in levels, not for actual gameplay.
struct NoteWidget : Widget
{
	std::wstring noteText;

	virtual void Draw(float deltaTime) override;
};
