#include "vpch.h"
#include "TitleScreenWidget.h"
#include "Core/Timer.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/GameUtils.h"
#include "UI/Colours.h"
#include "UI/ScreenFadeWidget.h"
#include "UI/UISystem.h"

void TitleScreenWidget::Start()
{
	__super::Start();

	//Load the "map to continue from" string in.
	GameUtils::LoadGameInstanceData();
}

void TitleScreenWidget::Draw(float deltaTime)
{
	__super::Draw(deltaTime);

	Text(L"Vagrant Tactics", AlignLayout(300.f, 100.f, Align::Center), TextAlign::Center, Colours::White);

	if (!_isInteractable)
	{
		return;
	}

	if (Button(L"Begin", AlignLayout(100.f, 50.f, Align::BottomLeft), 1.f, TextAlign::Center, Colours::White))
	{
		_isInteractable = false;
		SetToFadeOut();
		Timer::SetTimer(2.f, std::bind(&TitleScreenWidget::MoveToStartingLevel, this));
	}

	const auto mapName = GameInstance::GetContinueMapName();
	const bool continueButtonActive = !mapName.empty();
	if (Button(L"Continue", AlignLayout(100.f, 50.f, Align::BottomRight), 1.f, TextAlign::Center,
		Colours::White, 1.f, continueButtonActive))
	{
		_isInteractable = false;
		_continueMapName = mapName;
		SetToFadeOut();
		Timer::SetTimer(2.f, std::bind(&TitleScreenWidget::MoveToContinueLevel, this));
	}
}

void TitleScreenWidget::SetToFadeOut()
{
	UISystem::screenFadeWidget->SetToFadeOut();
	UISystem::screenFadeWidget->AddToViewport();
}

void TitleScreenWidget::MoveToStartingLevel()
{
	GameUtils::LoadWorldDeferred("starting_dock.vmap");
}

void TitleScreenWidget::MoveToContinueLevel()
{
	GameUtils::LoadWorldDeferred(_continueMapName);
}
