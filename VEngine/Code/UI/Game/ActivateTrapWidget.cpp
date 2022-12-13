#include "vpch.h"
#include "ActivateTrapWidget.h"
#include "Actors/Game/Unit.h"
#include "Gameplay/BattleCards/TrapCard.h"

void ActivateTrapWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.4f, 0.7f, 0.6f, 0.75f);

	if (Button(L"Activate Trap", layout))
	{
		linkedUnit->isInTrapNode = false;
		linkedTrapCard->Activate();
		RemoveFromViewportAndDelete();
		return;
	}

	layout.AddVerticalSpace(30.f);

	if (Button(L"Ignore Trap", layout))
	{
		linkedUnit->isInTrapNode = false;
		RemoveFromViewportAndDelete();
		return;
	}
}
