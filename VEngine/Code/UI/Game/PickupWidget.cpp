#include "vpch.h"
#include "PickupWidget.h"

void PickupWidget::Draw(float deltaTime)
{
	auto layout = CenterLayoutOnScreenSpaceCoords(200.f, 100.f);
	FillRect(layout);
	Text(L"Pick up", layout);
}
