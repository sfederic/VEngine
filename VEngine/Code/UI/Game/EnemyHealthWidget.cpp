#include "vpch.h"
#include "EnemyHealthWidget.h"

void EnemyHealthWidget::Draw(float deltaTime)
{
	int sx = 0, sy = 0;
	GetScreenSpaceCoords(sx, sy);

	Layout layout = CenterLayoutOnScreenSpaceCoords(100.f, 50.f, sx, sy);

	FillRect(layout);
	Text(std::to_wstring(healthPoints), layout);
}
