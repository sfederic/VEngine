#include "vpch.h"
#include "GuardWidget.h"

void GuardWidget::Draw(float deltaTime)
{
	Layout layout = AlignLayout(100.f, 100.f, Align::Center);
	Image("UI/shield_icon.png", layout);

	switch (guardState)
	{
		case GuardState::Success:
		{
			Image("UI/check_mark.png", layout);
			break;
		}
		case GuardState::Fail:
		{
			Image("UI/cross_mark.png", layout);
			break;
		}
	}
}
