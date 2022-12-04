#include "vpch.h"
#include "ActivateTrapWidget.h"
#include "Actors/Game/Unit.h"
#include "Gameplay/TrapNodes/TrapNode.h"

void ActivateTrapWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.4f, 0.7f, 0.6f, 0.8f);
	if (Button(L"Activate Trap?", layout))
	{
		linkedUnit->isInTrapNode = false;
		linkedTrapNode->Activate(linkedUnit);
	}
}
