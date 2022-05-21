#include "vpch.h"
#include "GuardWidget.h"

void GuardWidget::Draw(float deltaTime)
{
	Layout layout = AlignLayout(100.f, 100.f, Align::Center);
	Image("shield_icon.png", layout);

	//Show aura 'hint' based on memory of attacking unit (think player character remembering movesets)
	{
		Layout auraLayout;

		switch (attackingUnitAttackDirection)
		{
		case AttackDirection::Up:
			auraLayout = AlignLayout(100.f, 100.f, Align::Top);
			Image("aura_window.png", auraLayout);
			break;
		case AttackDirection::Down:
			auraLayout = AlignLayout(100.f, 100.f, Align::Bottom);
			Image("aura_window.png", auraLayout);
			break;
		case AttackDirection::Left:
			auraLayout = AlignLayout(100.f, 100.f, Align::Left);
			Image("aura_window.png", auraLayout);
			break;
		case AttackDirection::Right:
			auraLayout = AlignLayout(100.f, 100.f, Align::Right);
			Image("aura_window.png", auraLayout);
			break;
		}
	}

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
