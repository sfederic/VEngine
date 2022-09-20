#include "vpch.h"
#include "EnemyHealthWidget.h"

void EnemyHealthWidget::Draw(float deltaTime)
{
	int sx = 0, sy = 0;
	GetScreenSpaceCoords(sx, sy);

	Layout layout = CenterLayoutOnScreenSpaceCoords(400.f, 200.f, sx, sy);

	FillRect(layout);
	Text(std::to_wstring(healthPoints), layout);
}
