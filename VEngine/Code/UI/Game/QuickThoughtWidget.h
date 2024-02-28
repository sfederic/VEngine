#pragma once

#include "DialogueWidget.h"

class QuickThoughtWidget : public DialogueWidget
{
public:
	void Draw(float deltaTime) override;
	void OnRemoveFromViewport() override;
};
