#pragma once
#include "Widget.h"

//Throws a fullscreen rect over the viewport and fades transparency.
//There's maybe a nicer way to do it with D3D/GL and RenderTargets or something.
struct ScreenFadeWidget : Widget
{
	float fadeValue = 0.f;

	bool fadeOut = true;

	virtual void Draw(float deltaTime) override;
};
