#pragma once

#include "../Widget.h"

//Shows dialogue text for actors
class DialogueWidget : public Widget
{
public:
	void Draw(float deltaTime) override;
	void SetText(std::wstring_view text) { dialogueText = text; }

private:
	std::wstring dialogueText;
};
