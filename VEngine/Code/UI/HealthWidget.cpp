#include "HealthWidget.h"

void HealthWidget::Tick(float deltaTime)
{
	DrawHealth();
}

void HealthWidget::DrawHealth()
{
	int sx, sy;
	GetScreenSpaceCoords(sx, sy);

	for (int i = 0; i < *healthPoints; i++)
	{
		Image("heart_icon.png", sx + (i * 50), sy, 50.f, 50.f);
	}
}