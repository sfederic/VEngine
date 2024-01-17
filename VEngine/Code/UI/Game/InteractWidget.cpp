#include "vpch.h"
#include "InteractWidget.h"

void InteractWidget::Draw(float deltaTime)
{
	auto layout = CenterLayoutOnScreenSpaceCoords(150.f, 100.f);
	FillRect(layout);
	Text(interactText, layout);
}
