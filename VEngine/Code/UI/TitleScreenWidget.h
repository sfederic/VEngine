#pragma once

#include "Widget.h"
import <string>;

class TitleScreenWidget : public Widget
{
public:
	TitleScreenWidget() : Widget("TitleScreenWidget") {}
	void Start() override;
	void Draw(float deltaTime) override;

private:
	void SetToFadeOut();
	void MoveToStartingLevel();
	void MoveToContinueLevel();

	std::string _continueMapName;

	bool _isInteractable = true;
};
