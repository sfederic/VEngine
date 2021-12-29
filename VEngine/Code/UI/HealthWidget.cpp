#include "HealthWidget.h"

void HealthWidget::Draw()
{
	int sx, sy;
	GetScreenSpaceCoords(sx, sy);

	Layout layout = CenterLayoutOnScreenSpaceCoords(25.f, 25.f, (float)sx, (float)sy);

	for (int i = 0; i < healthPoints; i++)
	{
		layout.AddHorizontalSpace(50.f * i);
		Image("heart_icon.png", layout);
	}
}
