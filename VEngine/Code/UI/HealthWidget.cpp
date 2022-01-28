#include "HealthWidget.h"
#include "VString.h"

void HealthWidget::Draw(float deltaTime)
{
	int sx, sy;
	GetScreenSpaceCoords(sx, sy);

	Layout layout = CenterLayoutOnScreenSpaceCoords(50.f, 25.f, (float)sx, (float)sy);

	FillRect(layout);
	Text(VString::wformat(L"%d/%d", healthPoints, maxHealthPoints), layout);
}
