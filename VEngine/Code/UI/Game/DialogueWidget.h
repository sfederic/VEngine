#pragma once

#include "../Widget.h"

//Shows on screen dialogue from a character in-game.
class DialogueWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;

	std::wstring speakerName;
	std::wstring dialogueText;
};
