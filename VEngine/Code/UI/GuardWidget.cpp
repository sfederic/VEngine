#include "GuardWidget.h"

void GuardWidget::Draw(float deltaTime)
{
	Layout layout = AlignLayout(100.f, 100.f, Align::Center);
	Image("shield_icon.png", layout);

	if (guardSuccessful)
	{
		Image("check_mark.png", layout);
	}
}
