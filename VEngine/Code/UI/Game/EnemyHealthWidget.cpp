#include "vpch.h"
#include "EnemyHealthWidget.h"

void EnemyHealthWidget::Draw(float deltaTime)
{
	Layout layout = CenterLayoutOnScreenSpaceCoords(100.f, 50.f);

	FillRect(layout);
	Text(std::to_wstring(healthPoints), layout);
}
