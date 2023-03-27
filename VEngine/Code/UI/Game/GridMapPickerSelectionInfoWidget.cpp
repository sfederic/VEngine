#include "vpch.h"
#include "GridMapPickerSelectionInfoWidget.h"
#include "Actors/Game/GridActor.h"
#include "Gameplay/BattleCards/TrapCard.h"

void GridMapPickerSelectionInfoWidget::Draw(float deltaTime)
{
	DrawGridActorInfo();
	DrawTrapNodeInfo();
}

void GridMapPickerSelectionInfoWidget::DrawGridActorInfo()
{
	auto layout = PercentAlignLayout(0.6f, 0.2f, 0.9f, 0.4f);

	FillRect(layout);

	if (selectedGridActor)
	{
		Text(selectedGridActor->GetName(), layout);
	}
}

void GridMapPickerSelectionInfoWidget::DrawTrapNodeInfo()
{
	auto layout = PercentAlignLayout(0.6f, 0.6f, 0.9f, 0.8f);

	FillRect(layout);

	if (selectedTrapCard)
	{
		Text(selectedTrapCard->desc, layout);
	}
}
