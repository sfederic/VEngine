#include "TitleScreenWidget.h"

void TitleScreenWidget::Tick(float deltaTime)
{
	Text(L"Vagrant Tactics", { 0.f, 0.f, 200.f, 200.f });
	if (Button(L"Start", { 0.f, 0.f, 100.f, 100.f }))
	{
	}
	Image("floor.jpg", 0, 0, 200, 200);
}
