#include "vpch.h"
#include "ScanningEnemyAlertWidget.h"

void ScanningEnemyAlertWidget::Draw(float deltaTime)
{
	__super::Draw(deltaTime);

	auto layout = CenterLayoutOnScreenSpaceCoords(35.f, 55.f);
	layout.AddVerticalSpace(-75.f);
	Image("alert.png", layout);
}
