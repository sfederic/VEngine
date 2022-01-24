#pragma once
#include "Widget.h"

//Shows remaining player bullet count during combat and bullet types.
struct BulletWidget : Widget
{
	virtual void Draw(float deltaTime) override;
};
