#pragma once
#include "../Widget.h"

//Shows dialogue text for actors
struct DialogueWidget : Widget
{
	std::wstring dialogueText;

	virtual void Draw(float deltaTime) override;
	void SetText(std::wstring text);
};
