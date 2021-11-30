#include "HealthWidget.h"

void HealthWidget::Tick(float deltaTime)
{
	int sx, sy;
	GetScreenSpaceCoords(sx, sy);

	D2D1_RECT_F rect = CenterLayoutOnScreenSpaceCoords(25.f, 25.f, (float)sx, (float)sy);

	for (int i = 0; i < healthPoints; i++)
	{
		rect.left += 50.f * i;
		rect.right += 50.f * i;
		Image("heart_icon.png", rect);
	}
}
