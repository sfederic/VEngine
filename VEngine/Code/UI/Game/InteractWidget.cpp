#include "vpch.h"
#include "InteractWidget.h"

void InteractWidget::Draw(float deltaTime)
{
	auto layout = CenterLayoutOnScreenSpaceCoords(75.f, 25.f);
	FillRect(layout);
	Text(interactText, layout);
}
