#pragma once
#include "Widget.h"

//Shows dialogue text for actors
struct DialogueWidget : Widget
{
	std::wstring dialogueText;

	virtual void Draw() override;
	void SetText(std::string text);
};
