#include "vpch.h"
#include "PauseGameWidget.h"
#include "Core/Core.h"

void PauseGameWidget::Draw(float deltaTime)
{
	const auto textLayout = PercentAlignLayout(0.4f, 0.4f, 0.6f, 0.6f);
	Text(L"Paused", textLayout);

	const auto backLayout = PercentAlignLayout(0.f, 0.f, 1.f, 1.f);
	FillRect(backLayout, { 0.5f, 0.5f, 0.5f, 1.f }, 0.5f);
}

void PauseGameWidget::ToggleGamePaused()
{
	if (IsInViewport())
	{
		Core::PauseGameWorld();
	}
	else
	{
		Core::UnPauseGameWorld();
	}
}
