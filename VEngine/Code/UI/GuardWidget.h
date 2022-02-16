#pragma once
#include "Widget.h"

//Shows guard information when enemies are attacking player
struct GuardWidget : Widget
{
	enum class GuardState
	{
		None,
		Success,
		Fail
	};

	GuardState guardState;

	bool guardSuccessful = false;

	virtual void Draw(float deltaTime) override;
};
