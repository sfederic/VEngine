#include "vpch.h"
#include "GuardWidget.h"

void GuardWidget::Draw(float deltaTime)
{
	Layout layout = AlignLayout(100.f, 100.f, Align::Center);
	Image("UI/shield_icon.png", layout);

	//Show aura 'hint' based on memory of attacking unit (think player character remembering movesets)
	{
		Layout auraLayout;

		switch (attackingUnitAttackDirection)
		{
		case AttackDirection::Up:
			auraLayout = AlignLayout(100.f, 100.f, Align::Top);
			Image("UI/aura_window.png", auraLayout);
			break;
		case AttackDirection::Down:
			auraLayout = AlignLayout(100.f, 100.f, Align::Bottom);
			Image("UI/aura_window.png", auraLayout);
			break;
		case AttackDirection::Left:
			auraLayout = AlignLayout(100.f, 100.f, Align::Left);
			Image("UI/aura_window.png", auraLayout);
			break;
		case AttackDirection::Right:
			auraLayout = AlignLayout(100.f, 100.f, Align::Right);
			Image("UI/aura_window.png", auraLayout);
			break;
		}
	}

	switch(guardState)
	{
	case GuardState::Success:
		Image("UI/check_mark.png", layout);
		break;
	case GuardState::Fail:
		Image("UI/cross_mark.png", layout);
		break;
	}
}
