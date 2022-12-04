#include "ActivateTrapWidget.h"

void ActivateTrapWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.4f, 0.7f, 0.6f, 0.8f);
	FillRect(layout);
	Text(L"Activate Trap?", layout);
}
