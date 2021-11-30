#pragma once
#include "Widget.h"

//Shows player action points remaining during combat.
struct PlayerActionBarWidget : Widget
{
	int* actionPoints = nullptr;

	virtual void Tick(float deltaTime) override;
};
