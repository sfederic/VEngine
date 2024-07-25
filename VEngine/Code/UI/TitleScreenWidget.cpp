#include "vpch.h"
#include "TitleScreenWidget.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/GameInstance.h"
#include "UI/Colours.h"

void TitleScreenWidget::Draw(float deltaTime)
{
	__super::Draw(deltaTime);

	Text(L"Vagrant Tactics", AlignLayout(300.f, 100.f, Align::Center), TextAlign::Center, Colours::White);

	if (Button(L"Begin", AlignLayout(100.f, 50.f, Align::BottomLeft), 1.f, TextAlign::Center, Colours::White))
	{
		GameUtils::LoadWorldDeferred("starting_dock.vmap");
	}

	const auto mapName = GameInstance::GetContinueMapName();
	const bool continueButtonActive = !mapName.empty();
	if (Button(L"Continue", AlignLayout(100.f, 50.f, Align::BottomRight), 1.f, TextAlign::Center,
		Colours::White, 1.f, continueButtonActive))
	{
		GameUtils::LoadGameInstanceData();
		GameUtils::LoadWorldDeferred(mapName);
	}
}
