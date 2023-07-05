#include "vpch.h"
#include "ScreenFadeWidget.h"

void ScreenFadeWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.f, 0.f, 1.f, 1.f);

	FillRect(layout, { 0.f, 0.f, 0.f, fadeValue }, fadeValue);

	switch (fade)
	{
	case FadeValue::FadeOut:
		fadeValue += deltaTime;
		if (fadeValue < 0.f)
		{
			RemoveFromViewport();
		}
		break;

	case FadeValue::FadeIn:
		fadeValue -= deltaTime;
		if (fadeValue > 1.f)
		{
			RemoveFromViewport();
		}
		break;
	}
}

void ScreenFadeWidget::SetToFadeIn()
{
	fade = FadeValue::FadeIn;
	fadeValue = 1.f;
}

void ScreenFadeWidget::SetToFadeOut()
{
	fade = FadeValue::FadeOut;
	fadeValue = 0.f;
}
