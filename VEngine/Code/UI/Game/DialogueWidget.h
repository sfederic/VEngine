#pragma once

#include "../Widget.h"

//Shows dialogue text for actors
class DialogueWidget : public Widget
{
public:
	void Draw(float deltaTime) override;
	void SetText(std::wstring_view text) { fullDialogueText = text; }

	//Some DialogueWidgets will stay with certain actors, other will fire off and delete themselves.
	void DeleteOnTextProgressEnd() { setToDeleteOnTextProgressEnd = true; }

private:
	void ProgressCurrentText(float deltaTime);

	std::wstring currentDialogueText;
	std::wstring fullDialogueText;
	float textProgressTimer = 0.f;
	float removeWidgetTimer = 0.f;
	int currentDialogueCharIndex = 0;
	bool setToDeleteOnTextProgressEnd = false;
};
