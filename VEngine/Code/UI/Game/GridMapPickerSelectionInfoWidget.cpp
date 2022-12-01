#include "vpch.h"
#include "GridMapPickerSelectionInfoWidget.h"
#include "Actors/Game/GridActor.h"

void GridMapPickerSelectionInfoWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.6f, 0.4f, 0.9f, 0.7f);

	FillRect(layout);

	if (selectedGridActor)
	{
		Text(selectedGridActor->GetName(), layout);
	}
}
