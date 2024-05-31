#include "vpch.h"
#include "PauseGameWidget.h"
#include "Core/Core.h"

void PauseGameWidget::Draw(float deltaTime)
{
	const auto layout = PercentAlignLayout(0.4f, 0.4f, 0.6f, 0.6f);
	Text(L"Paused", layout);
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
