#include "ScreenFadeWidget.h"

void ScreenFadeWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.f, 0.f, 1.f, 1.f);

	FillRect(layout, { 0.f, 0.f, 0.f, 1.f }, fadeValue);

	switch (fade)
	{
	case FadeValue::FadeOut:
		fadeValue += (deltaTime * 0.4f);
		break;

	case FadeValue::FadeIn:
		fadeValue -= (deltaTime * 0.4f);
		break;
	}

	if (fadeValue >= 1.0f || fadeValue <= 0.f)
	{
		this->RemoveFromViewport();
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
