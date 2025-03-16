
#include "InteractWidget.h"

void InteractWidget::Draw(float deltaTime)
{
	__super::Draw(deltaTime);

	const auto layout = CenterLayoutOnScreenSpaceCoords(100.f, 100.f);
	FillRect(layout);
	Text(interactText, layout);
}
