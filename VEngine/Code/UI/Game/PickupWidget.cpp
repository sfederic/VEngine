#include "vpch.h"
#include "PickupWidget.h"

void PickupWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.8f, 0.8f, 0.9f, 0.9f);
	Image(heldItemIconFilename, layout);
}
