#include "vpch.h"
#include "UnitLineupWidget.h"
#include "Gameplay/BattleSystem.h"
#include "Actors/Game/Unit.h"
#include "Core/VString.h"
#include "Gameplay/GameUtils.h"

void UnitLineupWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.9f, 0.1f, 1.f, 1.f);

	FillRect(layout);
	layout.PushToTop(30.f);

	for (auto unit : battleSystem.activeBattleUnits)
	{
		if (Button(VString::stows(unit->GetName().c_str()), layout))
		{
			unit->ShowUnitMovementPath();
			GameUtils::SetActiveCameraTarget(unit);

			//@Todo: do a little 'unit info' widget here on click
		}

		layout.AddVerticalSpace(30.f);
		Text(VString::stows(unit->battleState.GetValue()), layout);

		layout.AddVerticalSpace(30.f);
	}
}
