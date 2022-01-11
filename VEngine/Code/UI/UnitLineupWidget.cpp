#include "UnitLineupWidget.h"
#include "Gameplay/BattleSystem.h"
#include "Actors/Game/Unit.h"
#include "VString.h"

void UnitLineupWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.9f, 0.1f, 1.f, 1.f);

	FillRect(layout);
	layout.PushToTop(30.f);

	for (auto unit : battleSystem.activeBattleUnits)
	{
		if (Button(VString::stows(unit->name.c_str()), layout))
		{
			unit->ShowUnitMovementPath();
		}

		layout.AddVerticalSpace(30.f);
	}
}
