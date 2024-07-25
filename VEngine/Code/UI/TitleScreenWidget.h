#pragma once

#include "Widget.h"
#include <string>

class TitleScreenWidget : public Widget
{
public:
	void Start() override;
	void Draw(float deltaTime) override;

private:
	void SetToFadeOut();
	void MoveToStartingLevel();
	void MoveToContinueLevel();

	std::string _continueMapName;

	bool _isInteractable = true;
};
