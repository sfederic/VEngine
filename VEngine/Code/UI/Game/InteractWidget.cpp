#include "vpch.h"
#include "InteractWidget.h"

void InteractWidget::Draw(float deltaTime)
{
	const auto layout = CenterLayoutOnScreenSpaceCoords(80.f, 40.f);
	FillRect(layout);
	Text(interactText, layout);
}
