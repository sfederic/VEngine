#include "vpch.h"
#include "PauseGameWidget.h"

void PauseGameWidget::Draw(float deltaTime)
{
	const auto layout = PercentAlignLayout(0.4f, 0.4f, 0.6f, 0.6f);
	Text(L"Paused", layout);
}
