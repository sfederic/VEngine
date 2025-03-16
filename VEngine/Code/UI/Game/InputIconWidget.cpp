
#include "InputIconWidget.h"
import Core.Core;

void InputIconWidget::Draw(float deltaTime)
{
	__super::Draw(deltaTime);

	const auto layout = PercentAlignLayout(0.4f, 0.4f, 0.6f, 0.6f);
	alpha = (sinf(Core::timeSinceStartup) * 0.25f) + 0.7f;
	Image(iconImageFilename, layout, alpha);
}
