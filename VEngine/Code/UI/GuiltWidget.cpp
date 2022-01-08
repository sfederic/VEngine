#include "GuiltWidget.h"

void GuiltWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.f, 0.f, 1.f, 1.f);
	Image("guilt.png", layout);
}
