#include "vpch.h"
#include "InputIconWidget.h"
#include "Core/Core.h"

void InputIconWidget::Draw(float deltaTime)
{
	const auto layout = PercentAlignLayout(0.4f, 0.4f, 0.6, 0.6f);
	alpha = (sinf(Core::timeSinceStartup) * 0.25f) + 0.7f;
	Image(iconImageFilename, layout, alpha);
}
