#include "vpch.h"
#include "GearSelectionWidget.h"
#include "Gameplay/Gears/GearSystem.h"
#include "Gameplay/Gears/PlayerGear.h"

void GearSelectionWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.5f, 0.5f, 0.9f, 0.9f);

	auto gears = GearSystem::Get().GetAllGears();
	for (auto gear : gears)
	{
		Button(gear->GetName(), layout);
		layout.AddVerticalSpace(30.f);
	}
}
