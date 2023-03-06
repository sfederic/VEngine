#include "vpch.h"
#include "MapSelectionInfoWidget.h"
#include "Gameplay/GameUtils.h"
#include "UI/UISystem.h"

void MapSelectionInfoWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.5f, 0.1f, 0.9f, 0.5f);
	FillRect(layout);

	Text(title, layout);
	layout.AddVerticalSpace(30.f);

	Text(description, layout);

	layout.PushToTop(30.f);
	if (Button("Enter", layout))
	{
		GameUtils::LoadWorldDeferred(levelToLoad);
		UISystem::SetWidgetControlActive(false);
	}
}
