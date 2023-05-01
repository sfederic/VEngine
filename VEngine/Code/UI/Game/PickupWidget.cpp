#include "vpch.h"
#include "PickupWidget.h"

void PickupWidget::Draw(float deltaTime)
{
	auto layout = CenterLayoutOnScreenSpaceCoords(75.f, 25.f);
	FillRect(layout);
	Text(L"Pick up", layout);
}
