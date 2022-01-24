#pragma once
#include "Widget.h"

//Shows remaining player bullet count during combat and bullet types.
struct BulletWidget : Widget
{
	int numBulletsPlayerHas = 2;

	virtual void Draw(float deltaTime) override;
};
