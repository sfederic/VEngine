#include "vpch.h"
#include "GridMapPickerSelectionInfoWidget.h"
#include "Actors/Game/GridActor.h"

void GridMapPickerSelectionInfoWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.3f, 0.3f, 0.7f, 0.7f);

	FillRect(layout);

	if (selectedGridActor)
	{
		Text(selectedGridActor->GetName(), layout);
	}
}
