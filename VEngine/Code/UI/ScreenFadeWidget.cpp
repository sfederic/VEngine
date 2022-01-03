#include "ScreenFadeWidget.h"

void ScreenFadeWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.f, 0.f, 1.f, 1.f);

	FillRect(layout, { 0.f, 0.f, 0.f, 1.f }, fadeValue);

	if (fadeOut)
	{
		fadeValue += (deltaTime * 0.4f);
	}
	else
	{
		fadeValue -= (deltaTime * 0.4f);
	}

	if (fadeValue >= 1.0f)
	{
		fadeOut = false;
		this->RemoveFromViewport();
	}
}
