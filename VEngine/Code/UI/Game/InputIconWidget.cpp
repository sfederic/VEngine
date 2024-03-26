#include "vpch.h"
#include "InputIconWidget.h"

void InputIconWidget::Draw(float deltaTime)
{
	const auto layout = PercentAlignLayout(0.4f, 0.4f, 0.6, 0.6f);
	Image("UI/enter_key_icon.jpg", layout);
}
