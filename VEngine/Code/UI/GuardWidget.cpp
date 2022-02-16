#include "GuardWidget.h"

void GuardWidget::Draw(float deltaTime)
{
	Layout layout = AlignLayout(100.f, 100.f, Align::Center);
	Image("shield_icon.png", layout);

	switch(guardState)
	{
	case GuardState::Success:
		Image("check_mark.png", layout);
		break;
	case GuardState::Fail:
		Image("cross_mark.png", layout);
		break;
	}
}
