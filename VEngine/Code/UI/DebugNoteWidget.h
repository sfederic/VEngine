#pragma once

#include "Widget.h"

//Displays a note in world position. Mainly used for debugging notes in levels, not for actual gameplay.
class DebugNoteWidget : public Widget
{
public:
	void Draw(float deltaTime) override;
	void SetNoteText(std::wstring_view text) { noteText = text; }

private:
	std::wstring noteText;
};
