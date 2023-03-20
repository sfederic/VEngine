#include "vpch.h"
#include "HealthWidget.h"

void HealthWidget::Draw(float deltaTime)
{
	auto layout = CenterLayoutOnScreenSpaceCoords(10 * healthToDisplay, 10);
	FillRect(layout);
}
