#include "vpch.h"
#include "LevelCompleteWidget.h"
#include "Gameplay/GameUtils.h"
#include "UI/UISystem.h"

void LevelCompleteWidget::Draw(float deltaTime)
{
	auto rectLayout = PercentAlignLayout(0.3f, 0.3f, 0.7f, 0.7f);
	FillRect(rectLayout);
	
	auto buttonLayout = PercentAlignLayout(0.45f, 0.5f, 0.55f, 0.6f);
	if (Button(L"Done", buttonLayout))
	{
		GameUtils::LoadWorldDeferred("map_screen.vmap");
		UISystem::SetWidgetControlActive(false);
	}
}
