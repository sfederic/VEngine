#include "vpch.h"
#include "InteractWidget.h"

void InteractWidget::Draw(float deltaTime)
{
	const auto layout = CenterLayoutOnScreenSpaceCoords(100.f, 100.f);
	FillRect(layout);
	Text(interactText, layout);
}
